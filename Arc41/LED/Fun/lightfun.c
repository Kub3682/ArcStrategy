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
