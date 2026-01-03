# Arc2 C 程序代码对应的架构策略说明

## 外部系统变化时，软件开始需要架构策略

- 例如需要增加闪烁灯或调光灯
- 为了避免简单的 Clone AND Owned，引入了 Arc2 的设计方法：向上兼容性设计
  例如如下代码（Clone Arc1 AND Owned new）：

```c
  #include "IfxPort.h"
  void core0_main(void)
  {
    #define LED1 &MODULE_P33, 1
    #define BUTTON &MODULE_P33, 11

        IfxPort_setPinMode(LED1, IfxPort_Mode_outputPushPullGeneral);
        IfxPort_setPinMode(BUTTON, IfxPort_Mode_inputPullUp);

        while (1)
        {

            if(IfxPort_getPinState(BUTTON) == 0)
            {
                while(IfxPort_getPinState(BUTTON) == 0)
                {
                IfxPort_setPinState(LED1, IfxPort_State_low); // 亮
                for(volatile uint32_t i=0; i<5000000; i++){**asm**("nop");}
                IfxPort_setPinState(LED1, IfxPort_State_high); // 灭
                for(volatile uint32_t i=0; i<5000000; i++){**asm**("nop");}
                }
            }
            else
                {
                IfxPort_setPinState(LED1, IfxPort_State_high);
                }
    }
  }
```
