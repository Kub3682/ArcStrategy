#ifndef LED_CTRL_H
#define LED_CTRL_H

#include "stdint.h"
#include "IfxPort_regdef.h"



// 作用：让后续的LightFunc定义能识别LightTypeConfigItem，无需等完整定义
typedef struct LightTypeConfigItem LightTypeConfigItem;

// 灯效函数指针类型（抽象“灯效行为”）
// 作用：让控制层（ctrl）仅依赖抽象接口，不依赖具体灯效实现
typedef void (*LightFunc)(const LightTypeConfigItem*);


// 极简版LEDConfig：仅封装MCAL层的端口+引脚，无多余字段
typedef struct {
    Ifx_P* port;  // 对应宏中的&MODULE_P33
    uint8_t pin;  // 对应宏中的0/1/4等
} LEDConfig;


// 灯型完整配置（灯型+硬件+灯效）
typedef struct LightTypeConfigItem {
    const char* lightTypeName;      // 灯型名称（如"BrakeLight"）
    uint8_t LEDCount; // LED数量
    LEDConfig LEDList[8]; // LED列表(对应MCAL里的LED配置)，暂先固定长度8,   #include "port_cfg.h"
    LightFunc activeEffect; // 当前车型选用的灯效策略
} LightTypeConfigItem;


// 车型完整配置（灯光配置+车型）
typedef struct {
    uint8_t lightCount;
    LightTypeConfigItem lights[5]; // 暂先固定长度5
} CarModelConfig;

// 全局配置声明（仅CPU0初始化，从核只读）
extern const CarModelConfig* g_currentConfig;

void Led_Ctrl_Init(const CarModelConfig* config);

// 控制层核心函数：传入灯效函数指针，实现按键控制LED
void Led_Ctrl_Handle(void);

/**
 * @brief 多核LED精准控制函数（下标+名称双校验）
 * @param i 灯型在CarModelConfig->lights数组中的下标
 * @param lightTypeName 灯型名称（如"TurnSignalLight"），用于校验
 * @note 1. 下标i直接定位灯型，无遍历，效率高；2. 名称校验避免传错下标；3. 适配多核分工
 */
void Led_Ctrl_LightHandle(uint8_t i, const char* lightTypeName);



#endif /* LED_CTRL_H */
