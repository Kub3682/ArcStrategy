#include "led_dd.h"
#include "dio.h"
#include "port_cfg.h"

// 设置LED初始状态（封装Dio_WritePin）
void Led_DD_SetIniState(Dio_StateType state)
{
    Dio_WritePin(LED1, state);
    Dio_WritePin(LED2, state);
    Dio_WritePin(LED3, state);
    Dio_WritePin(LED4, state);
    Dio_WritePin(LED5, state);
    Dio_WritePin(LED6, state);
    Dio_WritePin(LED7, state);
    Dio_WritePin(LED8, state);
}

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
