#ifndef LED_DD_H
#define LED_DD_H

#include "dio.h"
#include "dio_types.h"
#include "port_cfg.h"

// 封装DIO接口为LED专用函数（屏蔽DIO的端口/引脚细节）
// 作用：让上层（ctrl/fun）无需关心硬件引脚，仅调用LED专用接口
void Led_DD_SetState(Dio_StateType state);  // 设置LED状态
Dio_StateType Led_DD_ReadButton(void);       // 读取按键状态

#endif /* LED_DD_H */