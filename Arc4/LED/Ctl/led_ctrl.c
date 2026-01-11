#include "led_ctrl.h"
#include "../dd/led_dd.h"
#include "dio_types.h"

// 控制层核心逻辑：仅依赖抽象接口（LightFunc + led_dd）
void Led_Ctrl_Handle(LightFunc func)
{
    // 读取按键状态（依赖led_dd的抽象接口）
    Dio_StateType btnState = Led_DD_ReadButton();
    
    if(btnState == DIO_STATE_LOW)  // 按键按下
    {
        func();  // 调用抽象灯效接口（不关心具体是常亮/闪烁）
    }
    else  // 按键松开
    {
        Led_DD_SetState(DIO_STATE_HIGH);  // 熄灭LED
    }
}