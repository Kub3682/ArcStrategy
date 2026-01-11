#ifndef DIO_H
#define DIO_H

#include "dio_types.h"
#include "IfxPort.h"


// LED层调用的核心接口（保持你要求的参数格式）
void Dio_WritePin(Ifx_P *port, uint8_t pin, Dio_StateType state);
Dio_StateType Dio_ReadPin(Ifx_P *port, uint8_t pin);

#endif /* DIO_H */