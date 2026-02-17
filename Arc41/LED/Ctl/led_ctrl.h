#ifndef LED_CTRL_H
#define LED_CTRL_H

#include "port_cfg.h"
#include "lightfun.h"


/**
 * LED模块初始化接口
 * 包含LED初始状态设置、硬件适配等所有LED初始化逻辑
 * 供集成工程师调用，无需关心内部实现
 */


/*
typedef struct {
    LightType activeType; // 当前激活的LED类型
    LightFunc activeEffect; // 当前车型选用的灯效策略
} CarModelConfig;
*/


// 车型完整配置（灯光配置+车型）
typedef struct {
    uint8_t lightCount;
    LightTypeConfigItem lights[5]; // 暂先固定长度5
} CarModelConfig;


void Led_Ctrl_Init(const CarModelConfig* config);

// 控制层核心函数：传入灯效函数指针，实现按键控制LED
void Led_Ctrl_Handle(void);

#endif /* LED_CTRL_H */
