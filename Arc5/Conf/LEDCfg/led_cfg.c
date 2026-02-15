/* led_cfg.c */
#include "led_cfg.h"
#include "blink.h"
#include "alwayson.h"

// 建议增加 static 或 const 限制
const CarModelConfig LuxuryCarConfig = { .activeEffect = LightFunc_Blink };
const CarModelConfig BasicCarConfig  = { .activeEffect = LightFunc_AlwaysOn };