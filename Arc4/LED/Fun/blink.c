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
void LightFun_Blink(void)
{
    Led_DD_SetState(DIO_STATE_LOW);  // 亮
    DelayMs(200);
    Led_DD_SetState(DIO_STATE_HIGH); // 灭
    DelayMs(200);
}