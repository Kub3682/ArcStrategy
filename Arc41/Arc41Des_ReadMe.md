# Arc41 C 程序代码对应的架构策略说明

- Arc4的代码对“变化点”的判断是完全正确的
- 1. 灯效是变化点 → 单独放在 Fun 层
  - alwayson.c / blink.c
- 2. 灯效只依赖 led_dd，不碰 DIO / Port。依赖方向完全正确
- 3. 用函数指针抽象灯效行为
     - typedef void (\*LightFunc)(void);
     - 这是 C 语言里最轻量、最合理的“策略抽象”

> 但Arc4还差最后一步：
> 灯效“选择权”现在在 led_ctrl.c 的代码里，而不在“配置”里.

> 在Arc4代码里：
> Fun/（行为层）：相当于“零件库”。常亮、闪烁是通用的零件。
> led_ctrl.c：目前的角色就是**“车型实现”**。
> led_ctrl.c 中，直接写死了 controlLight(LightFunc_Blink)。这意味着如果你要为一个新车型改用“常亮”效果，你必须修改这个 .c 文件，重新编译。这就没有彻底符合“对修改关闭”的原则。

> 以上，需要引入配置化设计或依赖注入。
> 不要在 led_ctrl.c 内部写死调用哪个函数，而是通过一个配置变量来决定。
>
> 步骤 1：定义配置结构体
> 在 led_ctrl.h 中定义一个车型配置，由外部传入或链接。

```c
// led_ctrl.h
typedef struct {
    LightFunc activeEffect; // 当前车型选用的灯效策略
} CarModelConfig;

void Led_Ctrl_Init(const CarModelConfig* config);

```

> 步骤 2：在车型专属文件中定义数据（假设车型配置业务场景）
> 创建不同的文件代表不同车型（或者通过宏定义/链接脚本切换）：

```c
// Car_Luxury_Model.c (豪车版：常亮)
const CarModelConfig myCarConfig = {
    .activeEffect = LightFunc_AlwaysOn
};

// Car_Sport_Model.c (运动版：闪烁)
const CarModelConfig myCarConfig = {
    .activeEffect = LightFunc_Blink
};

```

> 步骤 3：核心控制逻辑通用化
> led_ctrl.c 变成了一个“执行引擎”，它只管执行配置好的策略：

```c

// led_ctrl.c 优化后
static const CarModelConfig* g_currentConfig = NULL;

void Led_Ctrl_Init(const CarModelConfig* config) {
    g_currentConfig = config;
    Led_DD_SetState(DIO_STATE_HIGH);
}

void Led_Ctrl_Handle(void) {
    if(g_currentConfig && g_currentConfig->activeEffect) {
        // 调用配置中指定的函数指针
        controlLight(g_currentConfig->activeEffect);
    }
}

```
