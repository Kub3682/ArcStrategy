# Arc3 C 程序代码对应的架构策略说明

## 外部系统变化时，软件开始需要架构策略

### 应对硬件的变化：

引脚复用是 MCU 硬件的 “刚需设计”
MCU 的芯片面积、引脚数量是有限的，但外设功能（GPIO、ADC、SPI 等）需求越来越多 —— 为了在有限引脚下支持更多功能，MCU 厂商必然采用 “引脚复用” 设计（一个 Pin 关联多个外设模块，通过配置寄存器选择激活的功能）。

- “引脚配置的集中初始化”
- 根据配置表，批量初始化多个引脚的功能模式和驱动特性

通过 “PORT Driver 配置硬件 + DIO Driver 操作功能” 的分层设计，实现 “硬件抽象” 与 “功能复用”，同时明确 MCAL 层的职责边界。
I/O HW Abstraction Software（硬件抽象软件）
PORT Driver：
包含 PORT_Init（初始化端口）、PORT_Config（配置端口）等函数，职责是直接操作硬件寄存器，完成引脚的 “功能模式配置”“方向配置”（比如将 Pin 配置为 GPIO 模式、设置为输入 / 输出）。
→ 是 “硬件配置者”，负责让引脚具备特定功能（如 GPIO）。
DIO Driver：
包含 DIO_WriteChannel（写单个引脚）、DIO_ReadPort（读整个端口）等函数，职责是基于 PORT Driver 配置好的 GPIO 引脚，实现数字 I/O 的功能操作。
→ 是 “功能使用者”，不直接配置硬件，只操作 PORT Driver 已初始化好的 GPIO 引脚。

![PORT driver and DIO driver structure and functions within the MCAL](image.png)

AUTOSAR MCAL 层设计的 3 个核心思想：
（MCU 的引脚复用是硬件设计的必然选择，AUTOSAR 是对这一硬件共性的软件标准化。AUTOSAR 成为行业标准后，开始反向影响 MCU 的硬件设计，形成 “硬件适配软件标准，软件标准引导硬件优化” 的循环）

1. 主张 “关注点分离”：PORT 负责 “硬件配置”，DIO 负责 “功能操作”
   PORT Driver 只做 “让引脚具备某种功能（如 GPIO）” 的硬件配置，不参与功能的实际使用；
   DIO Driver 只做 “已配置为 GPIO 的引脚” 的数字 I/O 操作，不涉及硬件配置；
   → 这种分离让硬件配置与功能操作解耦：修改硬件引脚配置只需改 PORT Driver，修改 I/O 逻辑只需改 DIO Driver。
2. 主张 “硬件抽象”：通过 MCAL 层屏蔽硬件细节
   上层软件（如应用层）不需要直接操作硬件寄存器，只需调用 PORT/DIO Driver 的抽象接口（如 PORT_Init、DIO_WriteChannel）——MCAL 层承担了 “硬件与上层软件的隔离”，让上层软件不依赖具体 MCU 的硬件设计，提升可移植性。
3. 主张 “分层架构”：明确 MCAL 层的职责边界
   截图用虚线框出了 PORT Driver 的范围，同时区分了 “硬件抽象软件 →MCAL 层 → 片内寄存器 → 硬件” 的层级：
   每一层只与相邻层交互，避免跨层依赖；
   PORT 与 DIO Driver 在 MCAL 层内分工明确，共同完成 I/O 功能的 “配置 + 使用” 闭环。
   这种架构是为了满足汽车电子软件的 **“可移植性、可维护性、可靠性”** 要求：
   可移植性：上层软件依赖 MCAL 的抽象接口，换 MCU 只需替换 MCAL 层的 PORT/DIO Driver，上层代码无需修改；
   可维护性：硬件配置与功能操作分离，修改硬件或功能时只需调整对应层的代码，不影响其他部分；
   可靠性：分层架构让职责边界清晰，降低了代码耦合，减少了 bug 的扩散范围。

![alt text](image-1.png)

按 AUTOSAR“关注点分离 + 分层架构” 思想，结合 Port/DIO 驱动职责边界，代码划分为 4 层 3 模块，结构如下：
层级 模块名 核心文件 包含代码内容
MCAL 层（Port 驱动） Port 配置模块 port_cfg.c/port_cfg.h 引脚硬件配置（IfxPort_setPinMode）、硬件符号定义（LED1/BUTTON 宏）
MCAL 层（DIO 驱动） DIO 操作模块 dio.c/dio.h 引脚数字状态读写（IfxPort_setPinState/IfxPort_getPinState）
应用逻辑层 LED 控制逻辑模块 led_ctrl.c/led_ctrl.h 按键 →LED 的控制规则（常亮 / 闪烁逻辑、延时）、模式切换（LED_CTL_TYPE 分支）
主调度层 系统初始化与调度模块 core0_main.c 模块初始化调用、主循环调度
