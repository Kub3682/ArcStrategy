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

void LightFunc_Blink(const LightTypeConfigItem* light)
{
    // 亮200ms
    //Led_DD_SetState(light, DIO_STATE_LOW);
    for (uint8_t i = 0; i < light->LEDCount; i++) {
        Led_DD_SetState(light, i, DIO_STATE_LOW); // 假设低电平点亮
    }
    DelayMs(200);
    // 灭200ms
    //Led_DD_SetState(light, DIO_STATE_HIGH);
        for (uint8_t i = 0; i < light->LEDCount; i++) {
        Led_DD_SetState(light, i, DIO_STATE_HIGH); // 假设低电平点亮
    }
    DelayMs(200);
}

//以上，为了简便，暂采取使用了 for 循环死等（Busy-wait），在闪烁期间，CPU 被锁死在循环里，无法响应其他任务（除非用中断）。
//后续工程优化，可考虑引入一个 Tick 计数器，让闪烁逻辑变成“状态机”。例如：if (currentTime - lastTime > 200ms) { toggleLED; }。这样 Led_Ctrl_Handle 就能实现真正的并发。
//如此，可实现非阻塞逻辑