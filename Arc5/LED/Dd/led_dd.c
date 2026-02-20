#include "led_dd.h"
#include "dio.h"
#include "port_cfg.h"

// 设置LED初始状态（封装Dio_WritePin）
void Led_DD_SetIniState(LED_StateType state)
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
void Led_DD_SetState(const LightTypeConfigItem* light, uint8_t i, LED_StateType state) {
    if (light == NULL || light->LEDCount == 0) return;
    const LEDConfig* led = &light->LEDList[i];
    // 校验无效LED，避免硬件异常
    Dio_WritePin(led->port, led->pin, state);
    if (led->port != NULL) {
        Dio_WritePin(led->port, led->pin, state);
    }
}



