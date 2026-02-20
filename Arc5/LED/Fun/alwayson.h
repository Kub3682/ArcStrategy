#ifndef ALWAYSON_H
#define ALWAYSON_H

#include "led_ctrl.h"

// 常亮灯效函数声明（适配LightFunc类型）
//void LightFunc_AlwaysOn(void);
void LightFunc_AlwaysOn(const LightTypeConfigItem* light);

#endif /* ALWAYSON_H */