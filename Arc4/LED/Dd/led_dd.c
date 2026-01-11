#include "led_dd.h"
#include "dio.h"
#include "port_cfg.h"

// 设置LED状态（封装Dio_WritePin）
void Led_DD_SetState(Dio_StateType state)
{
    Dio_WritePin(LED1, state);
}

// 读取按键状态（封装Dio_ReadPin）
Dio_StateType Led_DD_ReadButton(void)
{
    return Dio_ReadPin(BUTTON);
}