#include "IfxPort.h"
#include "port_cfg.h"
#include "dio.h"
#include "led_ctrl.h"

void core0_main(void)
{
    // 1. 硬件初始化（Port驱动优先初始化，符合AUTOSAR [SWS_Port_00078]）
    Port_Cfg_Init();
    
    // 2. 初始状态配置（DIO操作，基于Port配置的GPIO）
    Dio_WritePin(LED1_PORT, LED1_PIN, IfxPort_State_high); // LED初始熄灭

    // 3. 主循环调度业务逻辑
    while (1)
    {
        Led_Ctrl_Handle(); // 调用业务逻辑模块
    }
}