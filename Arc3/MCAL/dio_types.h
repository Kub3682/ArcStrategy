
#ifndef DIO_TYPES_H
#define DIO_TYPES_H

#include "stdint.h"

// 统一的DIO电平类型（LED层仅依赖此类型，无库依赖）
typedef enum
{
    DIO_STATE_LOW  = 0,  // 低电平
    DIO_STATE_HIGH = 1   // 高电平
} Dio_StateType;


#endif /* DIO_TYPES_H */