/*******************************************************************************
 *                              US212A
 *                            Module: Key driver
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       reagan     2011-9-2 14:45    1.0             build this file
 *******************************************************************************/
/*!
 * \file     key_charge.c
 * \brief    Get and set charge state
 * \author   reagan
 * \version 1.0
 * \date  2011-9-2
 *******************************************************************************/
#include "key_inner.h"

extern uint8 LowCurrentChargeCounter;
extern uint8 FastChargeFlag;

#define     BATLOW_VEL      0x44    //3.0V，检测低电
#define     BATNON_VEL      0x5d    //3.6V,检测无电池
/******************************************************************************/
/*!
 * \par  Description:
 *    获取电池充电状态。
 * \param[in]    none
 * \param[out]   charge_state_e
 * \return       the result
 * \retval           CHARGE_NONE 没有充电，正在使用电池供电
 * \retval           CHARGE_CHARGING 正在充电
 * \retval           CHARGE_FULL 充电已满
 * \retval           CHARGE_NOBATTERY 没有电池
 * \ingroup      key_charge
 * \par          exmaple code
 * \code
 *  //获取电池充电状态，并根据状态分别处理
 *  uint8 charge_state
 *  charge_state = (uint8)(int)key_chargeget(0);
 *  if (charge_state == CHARGE_FULL)
 *  {
 *      //充电已满
 *  }
 *  else if (charge_state == CHARGE_CHARGING)
 *  {
 *      //正在充电
 *  }
 *  else if (charge_state == CHARGE_NONE)
 *  {
 *      //没有充电
 *  }
 *  else 
 *  {
 *  }
 * \endcode
 * \note
 *******************************************************************************/
charge_state_e key_inner_chargeget(void* null0, void* null1, void* null2)
{
    //uint8 setting = (uint8)(param1);
    charge_state_e ret = CHARGE_NONE;

    //get charge status
    if (TheCharge == 1)
    {
        ret = CHARGE_CHARGING;

    }
    else
    {
        if (BatteryFullFlag == 1)
        {
            ret = CHARGE_FULL;
        }
        else if (BatteryFullFlag == 2)
        {
            ret = CHARGE_NOBATTERY;
        }
        else
        {
            
        }
    }

    return ret;
}

/******************************************************************************/
/*!
 * \par  Description:
 *    电池充电控制，设置开始充电和停止充电。
 * \param[in]      setting 选择开始充电或停止充电，详细参考 charge_control_e 定义
 * \param[in]      current 选择充电电流，详细参考 charge_current_e 定义
 * \param[in]      param3  选择充电满电参考电压，详细参考 battery_full_t 定义
 * \param[out]   charge state
 * \return       设置成功与否
 * \retval           0 sucess
 * \retval           1 failed
 * \ingroup      key_charge
 * \par          exmaple code
 * \code
 *  //启动电池充电，充电电流 250mA，满电参考电压为 4.2V 
 *  key_chargeset(CHARGE_START, ChargeCurrent250mA, BATFULL_LEVEL1);
 * \endcode
 * \note
 *******************************************************************************/
uint8 key_inner_chargeset(charge_control_e setting, charge_current_e current, battery_full_t param3)
{
    uint8 ret = 0;

    if (setting == CHARGE_START)
    {
        //start charge,set charge flag
        TheCharge = 1;
        ChargeCurrent = current;
        BatteryRefVol = param3;
        BatteryFullFlag = 0;
        g_rtc_counter = 2900; //快速进入充电过程
        CurrentSetFlag = 0; //设置用户电流设置标识
        LowCurrentChargeCounter = 0;
        FastChargeFlag = 0;
        //3.检测电池电压是否小于3.0V,判断是否需要检测有无电池
        if (act_readb(BATADC_DATA) < BATLOW_VEL)
        {
            //3.1若小于3.0V，屏蔽涓流，打开充电电流50ma充电，delay 500ms
            act_writeb((((act_readb(CHG_CTL) & 0xf0) | 0x02) | 0x10), CHG_CTL);
            act_writeb(act_readb(SYSTEM_VOL) | 0x60, SYSTEM_VOL);
            act_writeb((act_readb(CHG_CTL) | 0x20), CHG_CTL);
            sys_mdelay(500);

            //3.1.1检测电池电压是否小于3.6V
            if (act_readb(BATADC_DATA) < BATNON_VEL)
            {
                g_rtc_counter = 0; //快速进入充电过程
            }
            else
            {
                //no battery
                stop_charge_exit();
                BatteryFullFlag = 2; //作为无电池标志
            }
        }
    }
    else //CHARGE_STOP
    {
        //stop charge, clr charge flag
        //TheCharge = 0;
        stop_charge_exit();
    }

    return ret;
}

