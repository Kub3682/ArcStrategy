#include "led_ctrl.h"
#include "dio.h"
#include "port_cfg.h"
#include "dio_types.h"
#include "lightfun.h"
#include "alwayson.h"
#include "blink.h"


// led_ctrl.c 优化后
static const CarModelConfig* g_currentConfig = NULL; //初始值：NULL 是 C 语言中定义的空指针常量（通常数值为 0）。
//显式地写出 = NULL 是一种极好的编程习惯。明确传达了一个信息：“这个指针目前还没有指向任何有效的车型配置，使用前必须先初始化。”
//通过赋值为 NULL，强制要求系统必须显式地经过 Init 过程。
void Led_Ctrl_Init(const CarModelConfig* config) {
    g_currentConfig = config;
    Led_DD_SetIniState(DIO_STATE_HIGH);  // 初始状态为高电平
}

void Led_Ctrl_Handle(void) {
    if(g_currentConfig && g_currentConfig->activeEffect) {
        // 调用配置中指定的函数指针，如果 g_currentConfig 是 NULL，if 条件为假，代码就不会往下执行。
        //这保证了模块在“未注入灵魂（未初始化）”的情况下，能够安全地保持静默，而不是胡乱运行。
        controlLight(g_currentConfig->activeEffect);
    }
}
