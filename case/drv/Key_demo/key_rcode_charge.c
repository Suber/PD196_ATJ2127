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
 * \file     key_rcode_charge.c
 * \brief    charge flow control
 * \author   reagan
 * \version 1.0
 * \date  2011-9-2
 *******************************************************************************/

#include "key_inner.h"

#define  BATFAST_VEL     0x60//50 mA充电中 3.6V，为低电充电1.5分钟（250 mA*5 = 350 mA*3.5）

uint8 TheCharge; //充电标识，0,stop；1，start
uint8 ChargeCurrent; //充电电流选择
uint8 TrickleTimeCounter = 0; //触发计数器，30秒计数
uint8 BatteryFullFlag = 0; //充满标识
uint8 LowestCurrentCnt = 0;
uint8 CurrentSetFlag = 0;
uint8 BatteryRefVol; //充电电压值
uint8 LowCurrentChargeCounter;
uint8 ChargeCurrent_backup;
uint8 FastChargeFlag;
uint8 FastChargeCounter;

/******************************************************************************/
/*!
 * \par  Description:
 *    battery full check
 * \param[in]    none
 * \param[out]   uint8
 * \return       the result
 * \retval           0 battery is not full
 * \retval           1 battery is full
 * \ingroup      key drvier
 * \par          exmaple code
 * \code
 *               list example code in here
 * \endcode
 * \note
 *******************************************************************************/
uint8 BatteryFullCheck(void)
{
    g_rtc_counter = 0;
    
    //低于接近满电池时才检查电量，直接返回
    if ((TrickleTimeCounter != 100) && (act_readb(BATADC_DATA) < BatteryRefVol))
    {
        return 0;
    }
    if (TrickleTimeCounter < 5) //5min检查电量
    {
        //3.2.1.1.1.1.1.1 未加充，加充计时器累加1，退出
        TrickleTimeCounter++;
        return 0;
    }

    if ((LowCurrentChargeCounter == 0) && (act_readb(CHG_CTL) & 0x20))//检查电量前，先关闭充电控制，确保电量检查是正确的
    {
        act_writeb((act_readb(CHG_CTL) & 0xdf), CHG_CTL);
        TrickleTimeCounter = 100; //delay
        g_rtc_counter = 2950; //等待(3000-2950)*20ms=1s后进行检测，如果没充满，立即重启充电
        return 0;
    }
    
    if ((LowCurrentChargeCounter > 0) || (act_readb(BATADC_DATA) < BatteryRefVol))//未到达满电，恢复充电
    {
        if(LowCurrentChargeCounter == 0)
        {
            ChargeCurrent_backup = act_readb(CHG_CTL) & 0x0f;//备份暂停充电电流
            //以50mA使能充电，等待200ms
            act_writeb(0x32, CHG_CTL);
        }
        
        //3.2.1.1.1.1.1.2.1.1 否，则打开充电电路，初始化加充定时器，退出
        LowCurrentChargeCounter++;
        if(LowCurrentChargeCounter >= 10)
        {
            uint8 tmp1;
            tmp1 = (act_readb(CHG_CTL) & 0xf0) | 0x0f;
            act_writeb((ChargeCurrent_backup | 0xf0) & tmp1, CHG_CTL);
            
            TrickleTimeCounter = 0; //delay
            LowCurrentChargeCounter = 0;
        }
        else
        {
            g_rtc_counter = 2999;//20ms延时
        }
        
        return 0;
    }
    else
    {
        BatteryFullFlag = 1;
        TrickleTimeCounter = 0;
        stop_charge_exit();
        return 1;
    }
}
/******************************************************************************/
/*!
 * \par  Description:
 *    charge flow deal
 * \param[in]    none
 * \param[out]   none
 * \ingroup      key drvier
 * \par          exmaple code
 * \code
 *               list example code in here
 * \endcode
 * \note
 * 负责充电处理，这个会被定时器轮询的。
 *******************************************************************************/

void chargedeal(void)
{
    uint8 tmp1;
    
    g_rtc_counter = 0;
    FastChargeCounter++;

    //3.2若电池电压大于3.0V，判断是否已设定用户充电电流
    if (CurrentSetFlag == 1)
    {
        //3.2.1若已设定用户电流，检测status是否为0
        if ((act_readb(CHG_DET) & 0x01) == 0)
        {
            //3.2.1.1 status=0，判断充电电流是否大于25ma
            if ((act_readb(CHG_CTL) & 0x0f) > ChargeCurrent25mA)
            {
                //3.2.1.1.1 电流>25mA，说明负载能力不足，逐级减小50ma充电电流
                LowestCurrentCnt = 0; //低电计数器清0
                tmp1 = (act_readb(CHG_CTL) & 0x0f) - 1;
                act_writeb((act_readb(CHG_CTL) & 0xf0) | tmp1, CHG_CTL);

                BatteryFullCheck();
            }
            else
            {
                //3.2.1.1.2 电流<25mA，低电计数器加一
                LowestCurrentCnt++;

                //3.2.1.1.2.1 判断低电计数器是否>3
                if (LowestCurrentCnt > 3)
                {
                    //计数器>3，表明电池过充被保护或者负载能力不足
                    //置充电已满，停止，退出
                    BatteryFullFlag = 1;
                    stop_charge_exit();
                }
                else
                {
                    //计数器<3
                    //转到3.2.1.1.1.1，检测电池电压>4.2V
                    BatteryFullCheck();
                    //goto BatFullCheck;
                }
            }
        }
        else
        {
            if((FastChargeFlag == 1) && (FastChargeCounter >= 5))
            {
                //设置用户充电电流 
                tmp1 = (act_readb(CHG_CTL) & 0xf0) | 0x0f;
                act_writeb((ChargeCurrent | 0xf0) & tmp1, CHG_CTL);
                FastChargeFlag = 0;
            }
                
            //3.2.1.2 status!=0,跳转3.2.1.1.1.1，检测电压(BATADC)是否>4.2V
            //goto BatFullCheck;
            BatteryFullCheck();
        }
    }
    else
    {
        //3.2.2若未设定用户电流，检测电池电压是否<4.2V
        if ((LowCurrentChargeCounter > 0) || (act_readb(BATADC_DATA) < BatteryRefVol))
        {
            //屏蔽涓流，以50mA使能充电，等待200ms
            if(LowCurrentChargeCounter == 0)
            {
                act_writeb(0x32, CHG_CTL);
                act_writeb((act_readb(CHG_ASSISTANT) | 0x01), CHG_ASSISTANT);//4.23V
            }
            
            LowCurrentChargeCounter++;
            if(LowCurrentChargeCounter >= 10)
            {
                //低电充电，进入快速充电状态，充电电流为 350 ma
                if(act_readb(BATADC_DATA) < BATFAST_VEL)
                {
                    //设置用户充电电流 350 ma
                    tmp1 = (act_readb(CHG_CTL) & 0xf0) | 0x0f;
                    act_writeb(0xf8 & tmp1, CHG_CTL);
                    FastChargeCounter = 0;
                    FastChargeFlag = 1;//快速充电标志
                }
                else
                {
                    //设置用户充电电流
                    tmp1 = (act_readb(CHG_CTL) & 0xf0) | 0x0f;
                    act_writeb((ChargeCurrent | 0xf0) & tmp1, CHG_CTL);
                }
                
                LowCurrentChargeCounter = 0;
                CurrentSetFlag = 1; //设置用户电流设置标识
            }
            else
            {
                g_rtc_counter = 2999;//等待20ms重新进入
            }
        }
        else
        {
            //3.2.2.2 电压>4.2V，说明电池已满,置充电已满标志
            BatteryFullFlag = 1;
            stop_charge_exit();
        }
    }
}
/******************************************************************************/
/*!
 * \par  Description:
 *    charge stop deal
 * \param[in]    none
 * \param[out]   none
 * \ingroup      key drvier
 * \par          exmaple code
 * \code
 *               list example code in here
 * \endcode
 * \note
 *
 *******************************************************************************/
void stop_charge_exit(void)
{
    act_writeb(act_readb(CHG_CTL) & 0xDF, CHG_CTL);//停止充电
    TheCharge = 0;
    CurrentSetFlag = 0;
    LowestCurrentCnt = 0;
}
