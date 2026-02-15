
#ifndef MCAL_PORTCFG_PORT_CFG_H_
#define MCAL_PORTCFG_PORT_CFG_H_

/*********************************************************************************************************************/
/*-----------------------------------------------------Includes------------------------------------------------------*/
/*********************************************************************************************************************/

#include "IfxPort.h"

/*********************************************************************************************************************/
/*------------------------------------------------------Macros-------------------------------------------------------*/
/*********************************************************************************************************************/

// 硬件引脚符号定义（AUTOSAR要求的引脚符号化配置，对应[SWS_Port_00006]）
//#define LED1_PORT    &MODULE_P33
//#define LED1_PIN     0U
//#define BUTTON_PORT  &MODULE_P33
//#define BUTTON_PIN   11U

#define LEDA_PORT    &MODULE_P33         /* Port for the LED*/
#define LEDB_PORT    &MODULE_P13         /* Port for the LED*/
#define LED1     &MODULE_P33, 0             /* Port pin for the LED     */
#define LED2     &MODULE_P33, 1             /* Port pin for the LED     */
#define LED3     &MODULE_P33, 4             /* Port pin for the LED     */
#define LED4     &MODULE_P33, 5             /* Port pin for the LED     */
#define LED5     &MODULE_P13, 0             /* Port pin for the LED     */
#define LED6     &MODULE_P13, 1             /* Port pin for the LED     */
#define LED7     &MODULE_P13, 2             /* Port pin for the LED     */
#define LED8     &MODULE_P13, 3             /* Port pin for the LED     */

#define BUTTON   &MODULE_P33, 11            /* Port pin for the button  */

#define ACCESS_GRP  0                   /* Access protection group to be configured and assigned to LED pin         */

// Port配置函数声明（初始化引脚硬件模式）
void Port_Cfg_Init(void);
void initMulticoreAPSet(void);


#endif /* MCAL_PORTCFG_PORT_CFG_H_ */
