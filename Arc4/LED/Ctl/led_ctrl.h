#ifndef LED_CTRL_H
#define LED_CTRL_H

#include "../fun/lightfun.h"

// 控制层核心函数：传入灯效函数指针，实现按键控制LED
void Led_Ctrl_Handle(LightFunc func);

#endif /* LED_CTRL_H */