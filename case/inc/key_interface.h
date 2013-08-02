#ifndef _KEY_INTERFACE_H            //防止重定义
#define _KEY_INTERFACE_H

#include "psp_includes.h"

/*1.定义提供的具体接口命令，这里是按键驱动统一提供的接口*/

typedef enum
{
    KEY_CHARGEGET = 0,
    KEY_CHARGESET , 
    KEY_KEYTABADDR,
    KEY_HOLDSTATE,
    KEY_SPEAKCHECK,
    KEY_MAX    
} key_cmd_e;

/*charge state*/
typedef enum
{
    CHARGE_NONE = 0, 
    CHARGE_CHARGING, 
    CHARGE_FULL, 
    CHARGE_NOBATTERY
} charge_state_e;

/*charge switch*/
typedef enum
{
    CHARGE_START = 0,
    CHARGE_STOP
} charge_control_e;

//charge current set
typedef enum
{
    ChargeCurrent25mA = 1,
    ChargeCurrent50mA,
    ChargeCurrent100mA,
    ChargeCurrent150mA,
    ChargeCurrent200mA,
    ChargeCurrent250mA,
    ChargeCurrent300mA,
    ChargeCurrent350mA,
    ChargeCurrent400mA,
    ChargeCurrent450mA,
    ChargeCurrent500mA
} charge_current_e;
//full battery set
typedef enum
{
    BATFULL_LEVEL0 = 0x73, //voltage = 4.1v full
    BATFULL_LEVEL1 = 0x77
    //voltage = 4.2v full
} battery_full_t;

extern void *key_op_entry(void *param1, void *param2, void *param3, key_cmd_e cmd)__FAR__;

#define key_chargeget(a)        key_op_entry((void*)(a),0,0,KEY_CHARGEGET)
#define key_chargeset(a,b,c)    key_op_entry((void*)(a),(void*)(b),(void*)(c),KEY_CHARGESET)
#define key_getkeytabaddress()  key_op_entry((void*)0,(void*)0,(void*)0,KEY_KEYTABADDR)
#define key_holdcheck()         key_op_entry((void*)0,(void*)0,(void*)0,KEY_HOLDSTATE)
#define key_speakcheck(a)       key_op_entry((void*)(a),0,0,KEY_SPEAKCHECK)
#endif

