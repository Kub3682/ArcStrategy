#include "led_ctrl.h"
#include "led_dd.h"
#include "button_dd.h"
#include "stddef.h"
#include "string.h"


// led_ctrl.c 优化后
const CarModelConfig* g_currentConfig = NULL; //初始值：NULL 是 C 语言中定义的空指针常量（通常数值为 0）。
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


void Led_Ctrl_LightHandle(uint8_t i, const char* iTypeName) {
    if (g_currentConfig == NULL || g_currentConfig->lightCount == 0) {
        return; // 基本安全校验
    }
        // 安全校验：下标越界（i >= 灯型总数）→ 直接返回
    if (i >= g_currentConfig->lightCount) {
        return;
    }

    const LightTypeConfigItem* currentLight = &g_currentConfig->lights[i];

    // 修复：用strcmp比较字符串内容，先判空避免空指针
        if (currentLight == NULL || currentLight->lightTypeName == NULL || iTypeName == NULL ||
            strcmp(currentLight->lightTypeName, iTypeName) != 0) {
            return;
        }
    
        if (currentLight->activeEffect != NULL) {
            // 读取按键状态
            uint8_t btnState = Button_DD_ReadButton();
            if(btnState == BUTTON_STATE_ON) // 按键按下：执行灯效
            {
                // 把灯型配置传给灯效函数，让灯效知道控制哪些LED
                // 需先修改LightFunc类型为：typedef void (*LightFunc)(const LightTypeConfigItem*);
                currentLight->activeEffect(currentLight);
            }
            else // 按键松开：熄灭当前灯型的所有LED（仅控制该灯型，非全局）
            {
                // 熄灭LED
                //Led_DD_SetState(light, DIO_STATE_HIGH);
                //Led_DD_SetIniState(LED_STATE_OFF);
                for (uint8_t ledIdx = 0; ledIdx < currentLight->LEDCount; ledIdx++) {
                    // 调用驱动函数，仅熄灭当前灯型的LED（符合多核分工）
                    Led_DD_SetState(currentLight, ledIdx, LED_STATE_OFF); 
                }
            }
        }
}
