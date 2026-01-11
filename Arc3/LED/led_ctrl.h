#ifndef LED_CTRL_H
#define LED_CTRL_H

// 控制模式配置（业务逻辑参数）
#define LED_CTL_TYPE 1U // 0=常亮，1=闪烁

/**
 * LED模块初始化接口
 * 包含LED初始状态设置、硬件适配等所有LED初始化逻辑
 * 供集成工程师调用，无需关心内部实现
 */
void Led_Ctrl_Init(void);

/**
 * LED控制核心业务逻辑
 * 供集成工程师调用，无需关心内部实现
 */
void Led_Ctrl_Handle(void);

#endif /* LED_CTRL_H */