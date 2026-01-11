#include "dio.h"
#include "IfxPort.h"
#include "stdint.h"

// ========== 核心修复：纠正对iLLD库接口的理解 ==========
// 错误根源：IfxPort_getPinState返回uint32_t(0/1)，而非IfxPort_State枚举
// 修正1：读引脚转换——基于原始电平值（0/1），而非枚举值
static inline Dio_StateType Dio_ConvertFromRawState(uint32_t raw_state)
{
    return (raw_state == 0) ? DIO_STATE_LOW : DIO_STATE_HIGH;
}

// 修正2：写引脚转换——仅映射语义，不依赖枚举数值（鲁棒性提升）
static inline IfxPort_State Dio_ConvertToPortState(Dio_StateType state)
{
    IfxPort_State port_state;
    switch(state)
    {
        case DIO_STATE_LOW:
            port_state = IfxPort_State_low;
            break;
        case DIO_STATE_HIGH:
            port_state = IfxPort_State_high;
            break;
        default:
            port_state = IfxPort_State_high; // 默认高电平（灭）
            break;
    }
    return port_state;
}

// ========== LED层调用的接口（保持你要求的格式） ==========
void Dio_WritePin(Ifx_P *port, uint8_t pin, Dio_StateType state)
{
    // 调用函数指针指向的底层实现（英飞凌）
    IfxPort_setPinState((void *)port, pin, Dio_ConvertToPortState(state));
}

Dio_StateType Dio_ReadPin(Ifx_P *port, uint8_t pin)
{
    // 调用函数指针指向的底层实现（英飞凌）
    return Dio_ConvertFromRawState(IfxPort_getPinState((void *)port, pin));
} 