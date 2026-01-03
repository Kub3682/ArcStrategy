#include "IfxPort.h"

void core0_main(void) 
{
    #define LED1 &MODULE_P33, 0
    #define BUTTON &MODULE_P33, 11

    IfxPort_setPinMode(LED1, IfxPort_Mode_outputPushPullGeneral);
    IfxPort_setPinMode(BUTTON, IfxPort_Mode_inputPullUp);
    // 初始状态：LED熄灭
    IfxPort_setPinState(LED1, IfxPort_State_high);

    while (1)
    {
        if(IfxPort_getPinState(BUTTON) == 0)
            {
                IfxPort_setPinState(LED1, IfxPort_State_low);
            }
        else
            {
                IfxPort_setPinState(LED1, IfxPort_State_high);
            }
    }
}