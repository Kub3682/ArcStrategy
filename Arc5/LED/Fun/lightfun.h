#ifndef LIGHTFUN_H
#define LIGHTFUN_H


#include "port_cfg.h"
#include "stdint.h"

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



//void controlLight(LightFunc func);
void controlLight(const LightTypeConfigItem* light, LightFunc func);

#endif /* LIGHTFUN_H */
