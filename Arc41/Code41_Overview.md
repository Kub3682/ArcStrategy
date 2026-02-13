基于IFX TC4开发板（型号为：TriBoard TC4X9 STD V1.1 TBAEK8FQ）的LED灯控制系统，软件架构以及对应工程文件结构设计如下：

> ```
>  LED/
>  ├── Dd/ // Device Driver（驱动依赖抽象层：封装 DIO 接口）
>  │ ├── led_dd.c
>  │ └── led_dd.h
>  ├── Fun/ // Function（行为层：具体灯效实现）
>  │ ├── alwayson.c // 常亮灯效行为的逻辑实例
>  │ ├── alwayson.h
>  │ ├── blink.c // 闪烁灯效行为的逻辑实例
>  │ ├── blink.h
>  │ ├── lightfun.c // 行为抽象接口
>  │ └── lightfun.h // 行为抽象接口
>  └── Ctl/ // Control（控制层：核心控制逻辑应用层）
>    ├── led_ctrl.c
>    └── led_ctrl.h
>  MCAL/
>  ├── DIO/ // Digital Input Output（DIO）模块（MCU Port与MCU外设功能配置层）
>  │ ├── dio.c
>  │ ├── dio.h
>  │ └── dio_types.h  //DIO电平状态枚举（屏蔽底层IFX库函数依赖）
>  └── PortCfg/ // MCU Port Configuration（配置引脚硬件模式（对应AUTOSAR Port_SetPinMode功能））
>    ├── port_cfg.c
>    └── port_cfg.h // 行为抽象接口
>  Conf/
>  └── LEDCfg/ // Configuration（配置层：配置灯效类型等）
>    ├── led_cfg.c
>    └── led_cfg.h
>  Cpu0_Main.c
>
> ```

> 以上所有.c的具体代码如下（IFX相关的库函数，以及启动代码等不在此处展示）：

## Cpu0_Main.c

```c
#include <Ctl/led_ctrl.h>   // LED业务逻辑接口
#include "Ifx_Types.h"  //定义 TriCore 架构的通用数据类型（如uint8_t/uint32_t、boolean）
#include "Ifx_Cfg.h"    //工程的全局配置头文件，包含 SSW、外设等模块的配置宏
#include "IfxCpu.h"     //TriCore CPU 核心的寄存器 / 函数定义（如中断使能、核 ID 获取），是操作 CPU 核心功能的基础头文件。
#include "IfxWtu.h"     //Watchdog Timer Unit（看门狗单元）的函数定义，包含看门狗的使能、禁用、喂狗等函数
#include "IfxPort.h"
#include "port_cfg.h"   // Port配置接口
#include "dio.h"
#include "led_ctrl.h"
#include "led_cfg.h"


void core0_main(void)   //Core0 的主函数入口,由 SSW 启动代码最终调用
{
    IfxCpu_enableInterrupts();  //使能 Core0 的全局中断

    /* !!WATCHDOG0 AND SAFETY WATCHDOG ARE DISABLED HERE!!
     * Enable the watchdogs and service them periodically if it is required
     */
    IfxWtu_disableCpuWatchdog(IfxWtu_getCpuWatchdogPassword());  //禁用 Core0 的 CPU 看门狗和系统安全看门狗。
    IfxWtu_disableSystemWatchdog(IfxWtu_getSystemWatchdogPassword());

    /**
     * Core0主函数入口
     * 仅负责初始化与主循环任务调度，无底层硬件与具体应用逻辑的依赖（符合AUTOSAR分层规范）
     * 核心职责是 “系统级集成与调度”（确保模块初始化顺序、任务调度正常），而非 “业务细节的实现”（如 LED 初始状态、闪烁逻辑）
     */

    // 1. Port驱动初始化（必须优先执行，符合AUTOSAR Port驱动初始化时序）
    Port_Cfg_Init();

    // 2. LED模块初始化（集成工程师仅需调用接口，同时确认关键配置型号，无需关心内部逻辑）
    //Led_Ctrl_Init();
    Led_Ctrl_Init(&LuxuryCarConfig);

    // 3. 主循环：调度LED业务逻辑（集成工程师仅需调用接口，无需关心内部逻辑）
    while(1)
    {
        Led_Ctrl_Handle(); // 调用业务逻辑模块
    }

}

```

## port_cfg.c

```c


#include "port_cfg.h"

// 结合项目的系统-硬件框图以及 芯片硬件引脚映射表，在此文件中实现对各个引脚的硬件功能模式配置
// Port驱动核心职责：配置引脚硬件模式（对应AUTOSAR Port_SetPinMode功能）
void Port_Cfg_Init(void)
{

    // 配置LED1为GPIO推挽输出（硬件功能模式配置）
    //IfxPort_setPinMode(LED1_PORT, LED1_PIN, IfxPort_Mode_outputPushPullGeneral);
    IfxPort_setPinMode(LED1, IfxPort_Mode_outputPushPullGeneral);

    // 配置BUTTON为GPIO上拉输入（硬件电气特性配置，对应[SWS_Port_00081]）
    //IfxPort_setPinMode(BUTTON_PORT, BUTTON_PIN, IfxPort_Mode_inputPullUp);
    IfxPort_setPinMode(BUTTON, IfxPort_Mode_inputPullUp);

}



```

## led_ctrl.c

```c

#include "led_ctrl.h"
#include "dio.h"
#include "port_cfg.h"
#include "dio_types.h"
#include "lightfun.h"
#include "alwayson.h"
#include "blink.h"


// led_ctrl.c 优化后
static const CarModelConfig* g_currentConfig = NULL; //初始值：NULL 是 C 语言中定义的空指针常量（通常数值为 0）。
//显式地写出 = NULL 是一种极好的编程习惯。明确传达了一个信息：“这个指针目前还没有指向任何有效的车型配置，使用前必须先初始化。”
//通过赋值为 NULL，强制要求系统必须显式地经过 Init 过程。
void Led_Ctrl_Init(const CarModelConfig* config) {
    g_currentConfig = config;
    Led_DD_SetState(DIO_STATE_HIGH);  // 初始状态为高电平
}

void Led_Ctrl_Handle(void) {
    if(g_currentConfig && g_currentConfig->activeEffect) {
        // 调用配置中指定的函数指针，如果 g_currentConfig 是 NULL，if 条件为假，代码就不会往下执行。
        //这保证了模块在“未注入灵魂（未初始化）”的情况下，能够安全地保持静默，而不是胡乱运行。
        controlLight(g_currentConfig->activeEffect);
    }
}



```

## led_dd.c

```c

#include "led_dd.h"
#include "dio.h"
#include "port_cfg.h"

// 设置LED状态（封装Dio_WritePin）
void Led_DD_SetState(Dio_StateType state)
{
    Dio_WritePin(LED1, state);
}

// 读取按键状态（封装Dio_ReadPin）
Dio_StateType Led_DD_ReadButton(void)
{
    return Dio_ReadPin(BUTTON);
}


```

## dio.c

```c

#include "dio.h"
#include "IfxPort.h"
#include "stdint.h"

// ========== 核心修复：纠正对iLLD库接口的理解 ==========
// 错误根源：IfxPort_getPinState返回uint32_t(0/1)，而非IfxPort_State枚举
// 修正1：读引脚转换——基于原始电平值（0/1），而非枚举值
static inline Dio_StateType Dio_ConvertFromRawState(uint32_t raw_state)
{
    return (raw_state == 0) ? DIO_STATE_LOW : DIO_STATE_HIGH;
}

// 修正2：写引脚转换——仅映射语义，不依赖枚举数值（鲁棒性提升）
static inline IfxPort_State Dio_ConvertToPortState(Dio_StateType state)
{
    IfxPort_State port_state;
    switch(state)
    {
        case DIO_STATE_LOW:
            port_state = IfxPort_State_low;
            break;
        case DIO_STATE_HIGH:
            port_state = IfxPort_State_high;
            break;
        default:
            port_state = IfxPort_State_high; // 默认高电平（灭）
            break;
    }
    return port_state;
}

// ========== LED层调用的接口（保持你要求的格式） ==========
void Dio_WritePin(Ifx_P *port, uint8_t pin, Dio_StateType state)
{
    // 调用函数指针指向的底层实现（英飞凌）
    IfxPort_setPinState((void *)port, pin, Dio_ConvertToPortState(state));
}

Dio_StateType Dio_ReadPin(Ifx_P *port, uint8_t pin)
{
    // 调用函数指针指向的底层实现（英飞凌）
    return Dio_ConvertFromRawState(IfxPort_getPinState((void *)port, pin));
}



```

## lightfun.c

```c

#include "lightfun.h"
#include "stdint.h"
#include "dio_types.h"


void controlLight(LightFunc func)
{
    // 读取按键状态
    uint8_t btnState = Led_DD_ReadButton();
    if(btnState == DIO_STATE_LOW) // 按键按下
    {
        // 调用传入的抽象接口
        func();
    }
    else // 按键松开
    {
        // 熄灭LED
        Led_DD_SetState(DIO_STATE_HIGH);
    }
}



```

## blink.c

```c

#include "blink.h"
#include "../dd/led_dd.h"
#include "dio_types.h"

// 私有延时函数
static void DelayMs(uint32_t ms)
{
    for(volatile uint32_t i = 0; i < ms * 10000; i++)
    {
        __asm__("nop");
    }
}

// 闪烁灯效实现（仅依赖led_dd的抽象接口）

void LightFunc_Blink(void)
{
    // 亮200ms
    Led_DD_SetState(DIO_STATE_LOW);
    DelayMs(200);
    // 灭200ms
    Led_DD_SetState(DIO_STATE_HIGH);
    DelayMs(200);
}

//以上，为了简便，暂采取使用了 for 循环死等（Busy-wait），在闪烁期间，CPU 被锁死在循环里，无法响应其他任务（除非用中断）。
//后续工程优化，可考虑引入一个 Tick 计数器，让闪烁逻辑变成“状态机”。例如：if (currentTime - lastTime > 200ms) { toggleLED; }。这样 Led_Ctrl_Handle 就能实现真正的并发。
//如此，可实现非阻塞逻辑


```

## alwayson.c

```c

#include "alwayson.h"
#include "../dd/led_dd.h"
#include "dio_types.h"

// 常亮灯效实现（仅依赖led_dd的抽象接口）

void LightFunc_AlwaysOn(void)
{
    Led_DD_SetState(DIO_STATE_LOW);
}


```

## led_cfg.c

```c

/* led_cfg.c */
#include "led_cfg.h"
#include "blink.h"
#include "alwayson.h"

// 建议增加 static 或 const 限制
const CarModelConfig LuxuryCarConfig = { .activeEffect = LightFunc_Blink };
const CarModelConfig BasicCarConfig  = { .activeEffect = LightFunc_AlwaysOn };

```
