/*******************************************************************************
 *                              us212A
 *                            Module: key驱动
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved. 
 *
 * History:         
 *      <author>    <time>           <version >             <desc>
 *       cailizhen  2013-1-24 14:39     1.0             build this file 
*******************************************************************************/
/*!
 * \file     key_inner.h
 * \brief    key驱动模块相关数据结构，宏定义，接口声明等
 * \author   cailizhen
 * \version  1.0
 * \date     2013-1-24
*******************************************************************************/
#ifndef _KEY_INNER_H			//防止重定义
#define _KEY_INNER_H

#include "psp_includes.h"
#include "key_interface.h"
#include "gui_msg.h"
#include "app_msg.h"

extern charge_state_e key_inner_chargeget(void* null0, void* null1, void* null2);
extern uint8 key_inner_chargeset(charge_control_e setting, charge_current_e current, battery_full_t param3);
extern uint8 key_inner_beep(uint8 value, void* null0, void* null1);
extern uint8 *key_inner_getkeytabaddress(void* null0, void* null1, void* null2);
extern uint8 key_inner_holdcheck(void* null0, void* null1, void* null2);
extern uint8 *key_inner_speakcheck(uint8 onoff, void* null1, void* null2);

extern void PutSysMsg(unsigned int key);
extern void key_scan(void);
extern void chargedeal(void);
extern void stop_charge_exit(void)__FAR__;
extern uint8 BatteryFullCheck(void);

#define lock_key

/*define key message of key driver*/

#define Msg_KeyShortUp        0x30    //短按键抬起
#define Msg_KeyLongUp         0x31    //长按键抬起
//#define Msg_KeyStandby          0x21
#define Msg_KeyHold           0x32        //避免和Msg_KeyLoop|AP_KEY_UP冲突，Mars，2008-04-16
#define Msg_KeyUnHold         0x34

//Hold min and max adc
#define MAX_HOLD_ADC 0x39
#define MIN_HOLD_ADC 0x33
/* Hold state */
typedef enum
{
    HOLD_NONE, HOLD_LOCK
} hold_t;

//声明变量区
extern uint8 ChargeCurrent; //充电电流选择
extern uint8 TrickleTimeCounter; //触发计数器，30秒计数
extern uint8 BatteryFullFlag; //充满标识
extern uint8 LowestCurrentCnt;
extern uint8 CurrentSetFlag;
extern uint8 BatteryRefVol; //充电电压值
extern unsigned int g_rtc_counter;
extern unsigned char key_map[16];
extern uint8 TheCharge; //充电标识，0,stop；1，start
extern uint8 BatteryFullFlag; //充满标识
extern uint8 TrickleTimeCounter;
extern int time_irq;
extern unsigned char g_speakonoff;
#endif/*_KEY_H*/
