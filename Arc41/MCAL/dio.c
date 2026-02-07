#include "dio.h"

void Dio_WritePin(Ifx_P *port, uint8_t pin, IfxPort_State state)
{
    IfxPort_setPinState(port, pin, state);
}

IfxPort_State Dio_ReadPin(Ifx_P *port, uint8_t pin)
{
    return IfxPort_getPinState(port, pin);
}