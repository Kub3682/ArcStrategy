#include "circularflow.h"
#include "../dd/led_dd.h"
#include "dio_types.h"


static uint8_t g_flowCurrentIndex = 0;   // 当前点亮的LED下标



// 私有延时函数
static void DelayMsForFlow(uint32_t ms)
{
    for(volatile uint32_t i = 0; i < ms * 10000; i++)
    {
        __asm__("nop");
    }
}

// 闪烁灯效实现（仅依赖led_dd的抽象接口）

void LightFunc_CircularFlow(const LightTypeConfigItem* light)
{
    // 第一步：熄灭当前灯型下的所有LED
    for (uint8_t i = 0; i < light->LEDCount; i++) {
        Led_DD_SetState(light, i, DIO_STATE_HIGH); // 高电平熄灭（适配你的硬件逻辑）
    }
    DelayMsForFlow(200);

    //  第二步：点亮当前下标对应的LED
    if (g_flowCurrentIndex < light->LEDCount) {
        Led_DD_SetState(light, g_flowCurrentIndex, DIO_STATE_LOW); // 低电平点亮
    }
    DelayMsForFlow(200);

    //  第三步：更新下标，循环往复
    g_flowCurrentIndex++;
    if (g_flowCurrentIndex >= light->LEDCount) {
        g_flowCurrentIndex = 0; // 回到第一个LED，循环
    }

}

//以上，为了简便，暂采取使用了 for 循环死等（Busy-wait），在闪烁期间，CPU 被锁死在循环里，无法响应其他任务（除非用中断）。
//后续工程优化，可考虑引入一个 Tick 计数器，让闪烁逻辑变成“状态机”。例如：if (currentTime - lastTime > 200ms) { toggleLED; }。这样 Led_Ctrl_Handle 就能实现真正的并发。
//如此，可实现非阻塞逻辑
