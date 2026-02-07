#include "led_ctrl.h"
#include "dio.h"
#include "port_cfg.h"
#include "dio_types.h"
#include "lightfun.h"
#include "alwayson.h"
#include "blink.h"


void Led_Ctrl_Init(void)
{
    // LED初始状态：默认熄灭（高电平）—— 归属于LED模块，由应用工程师维护
    //Dio_WritePin(LED1, DIO_STATE_HIGH);
    Led_DD_SetState(DIO_STATE_HIGH);
}

void Led_Ctrl_Handle(void)
{
    controlLight(LightFunc_Blink);
    //controlLight(LightFunc_AlwaysOn);

}

