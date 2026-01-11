#include "port_cfg.h"   // Port配置接口
#include "led_ctrl.h"   // LED业务逻辑接口

/**
 * Core0主函数入口
 * 仅负责初始化与主循环任务调度，无底层硬件与具体应用逻辑的依赖（符合AUTOSAR分层规范）
 * 核心职责是 “系统级集成与调度”（确保模块初始化顺序、任务调度正常），而非 “业务细节的实现”（如 LED 初始状态、闪烁逻辑）
 */
void core0_main(void)
{
    // 1. Port驱动初始化（必须优先执行，符合AUTOSAR Port驱动初始化时序）
    Port_Cfg_Init();

    // 2. LED模块初始化（集成工程师仅需调用接口，无需关心内部逻辑）
    Led_Ctrl_Init();

    // 3. 主循环：调度LED业务逻辑（集成工程师仅需调用接口，无需关心内部逻辑）
    while(1)
    {
        Led_Ctrl_Handle();
    }
}