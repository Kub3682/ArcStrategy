#include "IfxPort.h"
void core0_main(void) 
{
    #define LED1 &MODULE_P33, 1
    #define BUTTON &MODULE_P33, 11
    #define LED_CTL_TYPE (1U)

    IfxPort_setPinMode(LED1, IfxPort_Mode_outputPushPullGeneral);
    IfxPort_setPinMode(BUTTON, IfxPort_Mode_inputPullUp);
    // 初始状态：LED熄灭
    IfxPort_setPinState(LED1, IfxPort_State_high);

    while (1)
    {
        if(IfxPort_getPinState(BUTTON) == 0)
        {
            #if (LED_CTL_TYPE == 0U)
            IfxPort_setPinState(LED1, IfxPort_State_low);
            #endif

            #if (LED_CTL_TYPE != 0U)
            while(IfxPort_getPinState(BUTTON) == 0)
                {
                IfxPort_setPinState(LED1, IfxPort_State_low); // 亮
                for(volatile uint32_t i=0; i<5000000; i++){__asm__("nop");} 
                IfxPort_setPinState(LED1, IfxPort_State_high); // 灭
                for(volatile uint32_t i=0; i<5000000; i++){__asm__("nop");} 
                }
            #endif
        }
        else
        {
            IfxPort_setPinState(LED1, IfxPort_State_high);
        }
    }
}