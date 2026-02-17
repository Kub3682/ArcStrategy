/* led_cfg.c */
#include "led_cfg.h"
#include "blink.h"
#include "alwayson.h"
#include "port_cfg.h"


//const CarModelConfig LuxuryCarConfig = { .activeEffect = LightFunc_Blink };
//const CarModelConfig BasicCarConfig  = { .activeEffect = LightFunc_AlwaysOn };

const CarModelConfig BasicCarConfig = {
    .lightCount = 2,  // 标配车型共2种灯型
    .lights = {
        // 第1个灯型：刹车灯（1个LED，LED1，常亮）
        {
            .lightTypeName = "BrakeLight",
            .LEDCount = 1,
            .LEDList = {LED1, LED_INVALID, LED_INVALID, LED_INVALID, 
                        LED_INVALID, LED_INVALID, LED_INVALID, LED_INVALID},
            .activeEffect = LightFunc_AlwaysOn
        },
        // 第2个灯型：转向灯（1个LED，LED2，频闪）
        {
            .lightTypeName = "TurnSignalLight",
            .LEDCount = 1,
            .LEDList = {LED2, LED_INVALID, LED_INVALID, LED_INVALID, 
                        LED_INVALID, LED_INVALID, LED_INVALID, LED_INVALID},
            .activeEffect = LightFunc_Blink
        },
        // 剩余灯型位置（固定长度5）：填充无效值（不影响使用）
        {.lightTypeName = NULL, .LEDCount = 0, .LEDList = {LED_INVALID}, .activeEffect = NULL},
        {.lightTypeName = NULL, .LEDCount = 0, .LEDList = {LED_INVALID}, .activeEffect = NULL},
        {.lightTypeName = NULL, .LEDCount = 0, .LEDList = {LED_INVALID}, .activeEffect = NULL}
    }
};

// -------------------------- 标配车型配置（2种灯型） --------------------------
const CarModelConfig LuxuryCarConfig = {
    .lightCount = 2,  // 豪华车型共2种灯型
    .lights = {
        // 第1个灯型：刹车灯（3个LED，LED1-LED3，常亮）
        {
            .lightTypeName = "BrakeLight",
            .LEDCount = 3,
            .LEDList = {LED1, LED2, LED3, LED_INVALID, 
                        LED_INVALID, LED_INVALID, LED_INVALID, LED_INVALID},
            .activeEffect = LightFunc_AlwaysOn
        },
        // 第2个灯型：转向灯（4个LED，LED5-LED8，频闪）
        {
            .lightTypeName = "TurnSignalLight",
            .LEDCount = 4,
            .LEDList = {LED5, LED6, LED7, LED8, 
                        LED_INVALID, LED_INVALID, LED_INVALID, LED_INVALID},
            .activeEffect = LightFunc_Blink
        },
        // 第3个灯型：氛围灯（6个LED，LED3-LED8，流水式）
        /*{
            .lightTypeName = "AmbientLight",
            .LEDCount = 6,
            .LEDList = {LED3, LED4, LED5, LED6, LED7, LED8, 
                        LED_INVALID, LED_INVALID},
            .activeEffect = LightFunc_WaterFlow
        },*/
        // 剩余3个灯型位置：填充无效值
        {.lightTypeName = NULL, .LEDCount = 0, .LEDList = {LED_INVALID}, .activeEffect = NULL},
        {.lightTypeName = NULL, .LEDCount = 0, .LEDList = {LED_INVALID}, .activeEffect = NULL},
        {.lightTypeName = NULL, .LEDCount = 0, .LEDList = {LED_INVALID}, .activeEffect = NULL}
    }
};
