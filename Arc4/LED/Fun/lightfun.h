#ifndef LIGHTFUN_H
#define LIGHTFUN_H



// 灯效函数指针类型（抽象“灯效行为”）
// 作用：让控制层（ctrl）仅依赖抽象接口，不依赖具体灯效实现
typedef void (*LightFunc)(void);

void controlLight(LightFunc func);

#endif /* LIGHTFUN_H */
