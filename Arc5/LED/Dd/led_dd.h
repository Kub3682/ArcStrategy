#ifndef LED_DD_H
#define LED_DD_H

#include "dio_types.h"
#include "led_ctrl.h"
#include "stdint.h"


typedef enum
{
    LED_STATE_ON  = DIO_STATE_LOW,  // 低电平
    LED_STATE_OFF = DIO_STATE_HIGH   // 高电平
} LED_StateType;


// 封装DIO接口为LED专用函数（屏蔽DIO的端口/引脚细节）
// 作用：让上层（ctrl/fun）无需关心硬件引脚，仅调用LED专用接口
void Led_DD_SetIniState(LED_StateType state);  // 设置LED初始状态
void Led_DD_SetState(const LightTypeConfigItem* light, uint8_t i, LED_StateType state);  // 设置LED状态


#endif /* LED_DD_H */
