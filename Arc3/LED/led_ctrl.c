#include "led_ctrl.h"
#include "dio.h"
#include "dio_types.h"
#include "port_cfg.h"

/**
 * @brief 软件延时函数（业务层内部辅助函数）
 * @param ms 延时毫秒数（适配TC4主频）
 */
static void DelayMs(uint32_t ms)
{
    for(volatile uint32_t i = 0; i < ms * 10000; i++)
    {
        __asm__("nop");  // 空指令延时
    }
}


void Led_Ctrl_Init(void)
{
    // LED初始状态：默认熄灭（高电平）—— 归属于LED模块，由应用工程师维护
    Dio_WritePin(LED1, DIO_STATE_HIGH);
}


/**
 * LED控制业务逻辑实现，让 “LED 的所有 knowhow（初始状态、控制逻辑、硬件适配）”完全内聚在 LED 模块中，形成 “高内聚、低耦合” 的业务单元
 * 完全依赖DIO抽象接口，无底层硬件依赖
 */
void Led_Ctrl_Handle(void)
{
    // 检测按键是否按下（低电平表示按下）
    if(Dio_ReadPin(BUTTON) == DIO_STATE_LOW)
    {
        #if (LED_CTL_TYPE == 0U)
        // 模式0：按键按下时LED常亮
        Dio_WritePin(LED1, DIO_STATE_LOW);
        #endif

        #if (LED_CTL_TYPE != 0U)
        // 模式1：按键按下时LED闪烁，松开则停止
        while(Dio_ReadPin(BUTTON) == DIO_STATE_LOW)
        {
            Dio_WritePin(LED1, DIO_STATE_LOW);  // LED亮
            DelayMs(200);
            Dio_WritePin(LED1, DIO_STATE_HIGH); // LED灭
            DelayMs(200);
        }
        #endif
    }
    else
    {
        // 按键松开时LED熄灭
        Dio_WritePin(LED1, DIO_STATE_HIGH);
    }
}