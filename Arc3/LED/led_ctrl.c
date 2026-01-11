#include "led_ctrl.h"
#include "dio.h"
#include "port_cfg.h"

// 软件延时（业务逻辑内的辅助功能）
static void DelayMs(uint32_t ms)
{
    for(volatile uint32_t i=0; i<ms*10000; i++){__asm__("nop");}
}

// 核心业务逻辑：按键状态→LED控制规则
void Led_Ctrl_Handle(void)
{
    if(Dio_ReadPin(BUTTON_PORT, BUTTON_PIN) == IfxPort_State_low) // 按键按下
    {
        #if (LED_CTL_TYPE == 0U)
        // 常亮逻辑
        Dio_WritePin(LED1_PORT, LED1_PIN, IfxPort_State_low);
        #endif

        #if (LED_CTL_TYPE != 0U)
        // 闪烁逻辑（按键按下期间持续执行）
        while(Dio_ReadPin(BUTTON_PORT, BUTTON_PIN) == IfxPort_State_low)
        {
            Dio_WritePin(LED1_PORT, LED1_PIN, IfxPort_State_low); // 亮
            DelayMs(200);
            Dio_WritePin(LED1_PORT, LED1_PIN, IfxPort_State_high); // 灭
            DelayMs(200);
        }
        #endif
    }
    else // 按键松开
    {
        Dio_WritePin(LED1_PORT, LED1_PIN, IfxPort_State_high); // 熄灭
    }
}