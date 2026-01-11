#ifndef LED_CTRL_H
#define LED_CTRL_H


/**
 * LED模块初始化接口
 * 包含LED初始状态设置、硬件适配等所有LED初始化逻辑
 * 供集成工程师调用，无需关心内部实现
 */
void Led_Ctrl_Init(void);

// 控制层核心函数：传入灯效函数指针，实现按键控制LED
void Led_Ctrl_Handle(void);

#endif /* LED_CTRL_H */