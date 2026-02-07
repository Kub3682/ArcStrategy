#include "IfxPort.h"
#include "port_cfg.h"
#include "dio.h"
#include "led_ctrl.h"

void core0_main(void)
{
    // 1. 硬件初始化（Port驱动优先初始化，符合AUTOSAR [SWS_Port_00078]）
    Port_Cfg_Init();
    
    // 2. LED模块初始化（集成工程师仅需调用接口，无需关心内部逻辑）
    Led_Ctrl_Init();

    // 3. 主循环调度业务逻辑
    while (1)
    {
        Led_Ctrl_Handle(); // 调用业务逻辑模块
    }
}