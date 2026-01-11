#include "port_cfg.h"
#include "IfxPort.h"

// 结合项目的系统-硬件框图以及 芯片硬件引脚映射表，在此文件中实现对各个引脚的硬件功能模式配置
// Port驱动核心职责：配置引脚硬件模式（对应AUTOSAR Port_SetPinMode功能）
void Port_Cfg_Init(void)
{

    // 配置LED1为GPIO推挽输出（硬件功能模式配置）
    //IfxPort_setPinMode(LED1_PORT, LED1_PIN, IfxPort_Mode_outputPushPullGeneral);
    IfxPort_setPinMode(LED1, IfxPort_Mode_outputPushPullGeneral);
    
    // 配置BUTTON为GPIO上拉输入（硬件电气特性配置，对应[SWS_Port_00081]）
    //IfxPort_setPinMode(BUTTON_PORT, BUTTON_PIN, IfxPort_Mode_inputPullUp); 
    IfxPort_setPinMode(BUTTON, IfxPort_Mode_inputPullUp);

}