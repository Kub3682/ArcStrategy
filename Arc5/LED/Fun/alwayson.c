#include "alwayson.h"
#include "../dd/led_dd.h"
#include "dio_types.h"

// 常亮灯效实现（仅依赖led_dd的抽象接口）

/*void LightFunc_AlwaysOn(void)
{
    Led_DD_SetState(DIO_STATE_LOW);
}
*/

void LightFunc_AlwaysOn(const LightTypeConfigItem* light) {
    // 仅控制当前灯型的LED常亮（而非全局）
    Led_DD_SetState(light, DIO_STATE_LOW); // 假设低电平点亮
}