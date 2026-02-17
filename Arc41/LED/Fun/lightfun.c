#include "lightfun.h"
#include "stdint.h"
#include "dio_types.h"


/*void controlLight(LightFunc func)
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
*/

// 辅助函数：执行单个灯效（绑定灯型，解决上下文问题）
void controlLight(const LightTypeConfigItem* light, LightFunc func) {
    if (light == NULL || func == NULL) return;
    // 读取按键状态
        uint8_t btnState = Led_DD_ReadButton();
        if(btnState == DIO_STATE_LOW) // 按键按下
        {
            // 把灯型配置传给灯效函数，让灯效知道控制哪些LED
            // 需先修改LightFunc类型为：typedef void (*LightFunc)(const LightTypeConfigItem*);
            func(light);
        }
        else // 按键松开
        {
            // 熄灭LED
            Led_DD_SetState(light, DIO_STATE_HIGH);
        }
}
