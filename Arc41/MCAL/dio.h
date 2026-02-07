// dio.h
#ifndef DIO_H
#define DIO_H

#include "IfxPort.h"
#include "port_cfg.h"

// DIO驱动职责：操作已配置的GPIO引脚状态（对应AUTOSAR Dio_WriteChannel/Dio_ReadChannel）
void Dio_WritePin(Ifx_P *port, uint8_t pin, IfxPort_State state);
IfxPort_State Dio_ReadPin(Ifx_P *port, uint8_t pin);

#endif // DIO_H