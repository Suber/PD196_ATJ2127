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
 * \file     key_rcode_scan.c
 * \brief    key scan and message deal
 * \author   reagan
 * \version 1.0
 * \date  2011-9-2
 *******************************************************************************/

#include "key_inner.h"

#define KEY_MSG_DEPTH   8//gui 消息队列深度
#define NO_KEY KEY_NULL
#define DOWN_KEY_TIMER  3//按键按下消息时间为60ms
#define LONG_KEY_TIMER  5//按键长按消息时间为 60ms + (5-1)*HOLD_KEY_TIMER = 700ms
#define HOLD_KEY_TIMER  8//按键连续hold消息间隔为160ms
unsigned char newkey = NO_KEY;
unsigned char oldkey = NO_KEY;
unsigned char tmpkey = NO_KEY;
unsigned char tmp_count, hold_count;
unsigned char wait_keyup = 0;

//第一个byte固定为play，其他按照线控按键顺序，从小到大安排
unsigned char key_map[16];

#ifdef lock_key
unsigned char TheLastHoldKey = Msg_KeyUnHold;
unsigned char RTCHold_Counter=0;
unsigned char TheFirstHold=0;
#endif

unsigned int g_rtc_counter = 0;
unsigned int key_count = 0; //当前按键消息发生的次数
unsigned int key_value = KEY_NULL; //当前按键的值
int time_irq;
unsigned char g_speakonoff;
unsigned char g_debounce_earphone = 0;

bool g_earphone_status = FALSE;

//ADC data

#define ADC_KEY_NUM  (4)

const unsigned char Adc_data[ADC_KEY_NUM] =
{ 0x06, 0x0a, 0x1a, 0x23 };

/******************************************************************************/
/*!
 * \par  Description:
 *    charge loop
 * \param[in]    none
 * \param[in]    none
 * \param[in]    none
 * \param[out]   none
 * \ingroup      key drvier
 * \par          exmaple code
 * \code
 *               list example code in here
 * \endcode
 * \note
 * 充电处理，包括充电涓流充电及充电电压设置，以及充满检查
 *******************************************************************************/
void charge_loop(void)
{
    if (((act_readb(SYSTEM_VOL) & 0x80) != 0) && (TheCharge == 1)) //使用SYSTEM_VOL判别USB连接
    {
        if (TrickleTimeCounter == 100)//关闭充电，检查电量,100作为标志位
        {
            BatteryFullCheck();//g_rtc_counter在BatteryFullCheck被设置
        }
        else
        {
            chargedeal();//g_rtc_counter在chargedeal被设置
        }
    }
    else
    {
        stop_charge_exit();
        g_rtc_counter = 2900;
    }
}

/******************************************************************************/
/*!
 * \par  Description:
 *    earphone key check
 * \param[in]    unsigned char adc
 * \param[in]    none
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      key drvier
 * \par          exmaple code
 * \code
 *               list example code in here
 * \endcode
 * \note
 * 检查耳机接入状态
 *******************************************************************************/

void RTC_EarphoneCheck(unsigned char adc)
{

    if (g_speakonoff == 0)
    {
        g_debounce_earphone = 0;
        return;
    }
    if ((adc > 0x2B) && (adc < 0x34))
    {
        //earphone in
        g_debounce_earphone++;
        if (g_debounce_earphone > 3)
        {
            g_debounce_earphone = 0;
            act_writel(act_readl(GPIO_ADAT) & 0xfffffffd, GPIO_ADAT);//enable speak control mute
        }

    }
    else if (adc > 0x39)
    {
        g_debounce_earphone = 0;
        act_writel(act_readl(GPIO_ADAT) | 0x02, GPIO_ADAT);//disable speak control mute
    }
    else
    {
    }
}

/******************************************************************************/
/*!
 * \par  Description:
 *    void post_Earphone_msg(unsigned char adc)
 * \param[in]    unsigned char adc
 * \param[in]    none
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      key drvier
 * \par          exmaple code
 * \code
 *               list example code in here
 * \endcode
 * \note
 * 耳机插拔消息发送
 *******************************************************************************/
void post_Earphone_msg(unsigned char adc)
{
    bool status;
    bool need_send = FALSE;
    //private_msg_t cur_send_pmsg;
    uint16 msg;
    static uint8 count1 = 0;
    static uint8 count2 = 0;

    if ((adc > 0x2B) && (adc < 0x34))
    {
        count1++;
        count2 = 0;
        //连续检测到20  次
        if (count1 > 20)
        {
            count1 = 0;

            //当前检测到有耳机
            status = TRUE;

            if (status != g_earphone_status) //耳机插上
            {
                g_earphone_status = TRUE;
                need_send = TRUE;
                //cur_send_pmsg.msg.type = MSG_EARPHONE_IN;
                msg = MSG_EARPHONE_IN;
            }
        }

    }
    else if (adc > 0x39)
    {
        count2++;
        count1 = 0;

        if (count2 > 20)
        {
            count2 = 0;
            //当前检测到无耳机
            status = FALSE;
            if (status != g_earphone_status) //耳机拔出
            {
                g_earphone_status = FALSE;
                need_send = TRUE;
                //cur_send_pmsg.msg.type = MSG_EARPHONE_OUT;
                msg = MSG_EARPHONE_OUT;
            }
        }
    }
    else
    {
    }

    if (need_send == TRUE)
    {
        //需发送耳机插拔消息
        //sys_mq_send(MQ_ID_DESK, (void *)&cur_send_pmsg);                 //发送到前台私有消息队列
        sys_mq_send(MQ_ID_SYS, &msg); //发送到系统消息队列
    }
}

/******************************************************************************/
/*!
 * \par  Description:
 *    Hold key check
 * \param[in]    unsigned char adc
 * \param[in]    none
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      key drvier
 * \par          exmaple code
 * \code
 *               list example code in here
 * \endcode
 * \note
 * 检查hold键状态，并有状态变化则发送相应的hold消息
 *******************************************************************************/
#ifdef lock_key
void RTC_HoldCheck(unsigned char adc)
{
    if((adc>MIN_HOLD_ADC)&&(adc<MAX_HOLD_ADC))
    {
        //hold on
        TheFirstHold++;
        if(TheFirstHold>3)
        {
            TheFirstHold = 0;
            if(TheLastHoldKey != Msg_KeyHold )
            {
                PutSysMsg(Msg_KeyHold); //发键值消息
                TheLastHoldKey = Msg_KeyHold;
            }
        }

    }
    else if(adc>MAX_HOLD_ADC) //no key

    {
        if(TheLastHoldKey == Msg_KeyHold )
        {
            PutSysMsg(Msg_KeyUnHold); //发键值消息
            TheLastHoldKey = Msg_KeyUnHold;
            TheFirstHold = 0;
        }
    }
    else
    {

    }
}
#endif
/******************************************************************************/
/*!
 * \par  Description:
 *    key scan of times
 * \param[in]    none
 * \param[in]    none
 * \param[in]    none
 * \param[out]   none
 * \return       the result
 * \retval           0
 * \retval           1
 * \ingroup      key drvier
 * \par          exmaple code
 * \code
 *               list example code in here
 * \endcode
 * \note
 * 按键扫描以及充电轮询，每隔20ms扫描一次，按键已经含有去抖处理
 *******************************************************************************/
void key_scan(void)
{

    unsigned char adcdat;
    unsigned char key_val, i;

    //充电保持DC5V存在
    if ((act_readb(SYSTEM_VOL) & 0x80) != 0)
    {
        g_rtc_counter++;
        //60s
        if (g_rtc_counter == 3000)
        {
            charge_loop();

        }
    }
    else
    {
        stop_charge_exit();
    }

    key_val = KEY_NULL;

    adcdat = act_readb(LRADC1_DATA) & 0x3f;

    for (i = 0; i < ADC_KEY_NUM; i++)
    {
        if (adcdat < Adc_data[i])
        {
            key_val = key_map[i + 1]; //first key map for play

            break;
        }
    }
    if ((act_readl(SYSTEM_CTL_RTCVDD) & 0x80000000) != 0) //bit6，7不为0，长按/短按play，统一发play，转到AP去处理
    {
        key_val = key_map[0];
        adcdat = 0;//skip hold state

    }

    if (key_val == KEY_NULL)
    {
        RTC_EarphoneCheck(adcdat);
        post_Earphone_msg(adcdat);
    }

#ifdef lock_key
    RTCHold_Counter++;
    if(RTCHold_Counter>4)
    {
        RTCHold_Counter = 0;
        RTC_HoldCheck(adcdat);
    }
#endif

    if (key_val != oldkey) //如果本次扫描值不等于上次按键值
    {
        if (key_val == tmpkey) //如果本次键值等于前次扫描值
        {
            tmp_count++; //累加扫描次数

            if (tmp_count > DOWN_KEY_TIMER)
            {
                tmp_count = DOWN_KEY_TIMER;
            }

            if (tmp_count == DOWN_KEY_TIMER) //扫描三次，是同一键值，确定有按键按下
            {
                if (key_val != NO_KEY) //如果本次扫描捕获键值不为空
                {
                    oldkey = tmpkey;
                    wait_keyup = 1;
                    PutSysMsg(oldkey);
                }
                else //本次键值为NO_KEY
                {
                    hold_count = 0;
                    oldkey = NO_KEY;
                    tmpkey = NO_KEY;

                    if (wait_keyup == 1) //等待KEYUP
                    {
                        wait_keyup = 0;
                        tmp_count = 0; //清扫描次数
                        PutSysMsg(Msg_KeyShortUp); //发keyup消息
                    }
                }
            }
        }
        else //如果本次扫描值不等于前次扫描值，则重置键值，重新计数
        {
            tmpkey = key_val;
            if (!wait_keyup)
            {
                tmp_count = 0;
            }
        }
    }
    else //如果本次键值等于上次按键值,累计8次(160ms)，发一次按键消息
    {
        if (key_val != NO_KEY) //
        {
            hold_count++;

            if (hold_count == HOLD_KEY_TIMER)
            {
                hold_count = 0;
                PutSysMsg(oldkey); //发键值消息
            }
        }
        else
        {
            tmpkey = key_val;
            hold_count = 0;
            if (!wait_keyup)
            {
                tmp_count = 0;
            }
        }
    }

}

