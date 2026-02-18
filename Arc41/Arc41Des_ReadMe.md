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

> 应用：核心控制逻辑通用化
> 注意：比如为了执行 Blink，需要避免直接通过 Led_Ctrl_Init(LightFunc_Blink)的调用形式来实现。
> 正确的做法是：在车型专属文件中定义一个“配置说明书”（CarModelConfig），然后在 Main 函数中根据当前车型选择注入哪份说明书。
>
> A. 扩展性：防止“接口大改”
> 如果明年新车型需求增加了：不仅要选灯效，还要设置灯的颜色和闪烁周期。
>
> - 直接传函数（差方案）： 你得修改 Led_Ctrl_Init 的参数，变成 Init(func, color, period)。这会导致所有引用过这个函数的地方都要改，甚至导致底层库重新编译。
> - 传结构体（好方案）： 你只需要在 CarModelConfig 结构体里加个成员。Led_Ctrl_Init(config) 的函数签名永远不变。这叫“对修改关闭”。
>
> B. 属性关联性
> 一个车型不仅仅是一个动作，它是一组属性的集合。通过结构体，我们可以把属于“豪华车”的所有参数打包在一起。这就好比你去面试，不是只带“会写代码”这一项技能，而是带一份“简历（结构体）”，里面包含了你的姓名、技能、工作经验。
>
> C. 内存与安全性（ROM化）
> 在汽车电子（如英飞凌 TC4）开发中，这些配置通常被定义为 static const，这意味着它们会被存放在 Flash（只读存储器） 中。 通过传递指针，Led_Ctrl 只是“引用”了这份存在 Flash 里的只读说明书，既节省了昂贵的 RAM 空间，又防止了配置信息被意外修改。

> 这种“在初始化时选择参数”的行为，在设计模式中叫 依赖注入 (Dependency Injection)。
>
> - Led_Ctrl 模块：它是一个“通用的执行引擎”，它内部是空的，没有灵魂。
> - Led_Ctrl_Init()：它是一个“注入孔”。
> - LuxuryCarConfig：它是“灵魂/配置”。
>
> 这种“决定权的上移”，正是架构设计的精髓：让底层的零件尽量通用，让高层的逻辑决定组合方式。
> 当新增相关需求时，增加的只有 Fun/ 下的算法插件和 Cfg/ 下的车型表。其他代码都不需要修改。

```c

// 1. 先定义好不同车型的“配置说明书”（通常放在 Cfg 层）
const CarModelConfig LuxuryCarConfig = { .activeEffect = LightFunc_Blink };
const CarModelConfig BasicCarConfig  = { .activeEffect = LightFunc_AlwaysOn };

// 2. 在程序启动（Main函数）时，根据当前车型选择注入哪份说明书
void core0_main(void) {
    // 如果这台车是豪华版，就传入豪华版的配置指针
    Led_Ctrl_Init(&LuxuryCarConfig);

    while(1) {
        Led_Ctrl_Handle();
    }
}

```

> 进阶：如何让“车型选择”也变成更灵活的架构范式？
> 在 main.c 中根据硬件 ID 动态注入车型配置。

```c

/* main.c 执行范式示例 */
void core0_main(void)
{
    const CarModelConfig* selectedCfg;

    // 思维范式：从硬件或跳线读取车型 ID
    uint8_t modelId = Hardware_ReadModelID();

    if (modelId == 0x01) {
        selectedCfg = &LuxuryCarConfig;
    } else {
        selectedCfg = &BasicCarConfig;
    }

    // 注入执行范式
    Led_Ctrl_Init(selectedCfg);

    while(1) {
        Led_Ctrl_Handle();
    }
}

```

以上，配置化还能进一步设计为：
典型的“查表法”。通过数组和结构体描述车型差异（灯的数量、Pin 脚映射、效果），这极大增加灵活性以及降低了变体管理（Variant Management）的难度。

如：

```c

// 极简版LEDConfig：仅封装MCAL层的端口+引脚，无多余字段
typedef struct {
    Ifx_P* port;  // 对应宏中的&MODULE_P33
    uint8_t pin;  // 对应宏中的0/1/4等
} LEDConfig;


// 灯型完整配置（灯型+硬件+灯效）
typedef struct LightTypeConfigItem {
    const char* lightTypeName;      // 灯型名称（如"BrakeLight"）
    uint8_t LEDCount; // LED数量
    LEDConfig LEDList[8]; // LED列表(对应MCAL里的LED配置)，暂先固定长度8,   #include "port_cfg.h"
    LightFunc activeEffect; // 当前车型选用的灯效策略
} LightTypeConfigItem;

// 车型完整配置（灯光配置+车型）
typedef struct {
    uint8_t lightCount;
    LightTypeConfigItem lights[5]; // 暂先固定长度5
} CarModelConfig;


```

这样，在车型专属文件中，只需要定义一个“车型配置”（CarModelConfig），然后在 Main 函数中根据当前车型选择注入哪份说明书。

## 当前架构问题与局限性：

### 存在的问题与风险

阻塞型设计 (Blocking Code): blink.c 中使用了 DelayMs（忙等）。在单线程 CPU 架构中，这会导致 Led_Ctrl_Handle 锁死，无法处理其他并发任务（如通信或按键响应）。

接口定义的循环依赖隐患: led_cfg.h 包含了 led_ctrl.h，而 led_ctrl.h 又包含 lightfun.h。虽然目前使用了 ifndef 保护，但层级耦合较重。

内存空间浪费: LightTypeConfigItem 中的 LEDList[8] 和 CarModelConfig 中的 lights[5] 使用固定长度。在资源极度受限的 MCU 中，这种为了对齐而做的冗余配置会占用较多 Flash/RAM。

防御性编程不足: 数组遍历时主要依赖 lightCount，若配置数据出错（如 lightCount 填错），可能导致非法内存访问。
下一步的演进重点还可放在实时性优化（去阻塞）和内存管理精细化上。

### 存在的局限性

当前的架构（Arc41）在单核环境下表现优秀，但在多核（Multicore）系统中存在显著的局限性。
从多核并行、负载均衡、异构核心支持及软件工程原则（如开闭原则）出发，引出如下待优化设计点：

1. 全局配置锁定：g_currentConfig 是一个静态全局变量。在多核环境下，多个核心同时读取或初始化该变量会引发一致性问题，无法支持“不同核心加载不同配置”的并行需求。
2. Led_Ctrl_Handle 是一个粗粒度的遍历函数，它在当前核心上一次性处理所有灯型。这使得任务无法根据核心负载进行动态拆分或静态绑定。
3. LightFunc_Blink 中的 DelayMs（忙等）在多核系统中尤为致命。它不仅白白浪费当前核心的算力，还可能因为持有某些共享资源的锁而导致其他核心长时间等待。
