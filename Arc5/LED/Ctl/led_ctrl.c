#include "led_ctrl.h"
#include "led_dd.h"
#include "button_dd.h"
#include "stddef.h"


// led_ctrl.c 优化后
static const CarModelConfig* g_currentConfig = NULL; //初始值：NULL 是 C 语言中定义的空指针常量（通常数值为 0）。
//显式地写出 = NULL 是一种极好的编程习惯。明确传达了一个信息：“这个指针目前还没有指向任何有效的车型配置，使用前必须先初始化。”
//通过赋值为 NULL，强制要求系统必须显式地经过 Init 过程。
void Led_Ctrl_Init(const CarModelConfig* config) {
    g_currentConfig = config;
    Led_DD_SetIniState(LED_STATE_OFF);  // 初始状态为高电平
}


void Led_Ctrl_Handle(void) {
    if (g_currentConfig == NULL || g_currentConfig->lightCount == 0) {
        return; // 保留安全校验
    }
     // 遍历每个灯型，执行对应灯效（差异化控制）
    for (uint8_t i = 0; i < g_currentConfig->lightCount; i++) {
        const LightTypeConfigItem* currentLight = &g_currentConfig->lights[i];
        if (currentLight->activeEffect != NULL) {
            // 读取按键状态
            uint8_t btnState = Button_DD_ReadButton();
            if(btnState == BUTTON_STATE_ON) // 按键按下
            {
                // 把灯型配置传给灯效函数，让灯效知道控制哪些LED
                // 需先修改LightFunc类型为：typedef void (*LightFunc)(const LightTypeConfigItem*);
                currentLight->activeEffect(currentLight);
            }
            else // 按键松开
            {
                // 熄灭LED
                //Led_DD_SetState(light, DIO_STATE_HIGH);
                Led_DD_SetIniState(LED_STATE_OFF);
            }
        }
    }

}
