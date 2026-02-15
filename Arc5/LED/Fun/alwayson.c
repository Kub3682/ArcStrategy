#include "alwayson.h"
#include "../dd/led_dd.h"
#include "dio_types.h"

// 常亮灯效实现（仅依赖led_dd的抽象接口）

void LightFunc_AlwaysOn(void)
{
    Led_DD_SetState(DIO_STATE_LOW);
}
