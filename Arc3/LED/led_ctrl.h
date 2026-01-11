#ifndef LED_CTRL_H
#define LED_CTRL_H

// 控制模式配置（业务逻辑参数）
#define LED_CTL_TYPE 1U // 0=常亮，1=闪烁

// 业务逻辑函数：按键控制LED
void Led_Ctrl_Handle(void);

#endif // LED_CTRL_H