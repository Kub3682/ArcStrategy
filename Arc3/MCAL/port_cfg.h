#ifndef PORT_CFG_H
#define PORT_CFG_H



// 硬件引脚符号定义（AUTOSAR要求的引脚符号化配置，对应[SWS_Port_00006]）
//#define LED1_PORT    &MODULE_P33
//#define LED1_PIN     0U
//#define BUTTON_PORT  &MODULE_P33
//#define BUTTON_PIN   11U

#define LED1     &MODULE_P33, 0             /* Port pin for the LED     */
#define BUTTON   &MODULE_P33, 11            /* Port pin for the button  */

// Port配置函数声明（初始化引脚硬件模式）
void Port_Cfg_Init(void);

#endif // PORT_CFG_H