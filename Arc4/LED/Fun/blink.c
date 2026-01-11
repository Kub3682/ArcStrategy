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