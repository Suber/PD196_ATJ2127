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
 * \file     key_scan.c
 * \brief    key scan and message deal
 * \author   reagan
 * \version 1.0
 * \date  2011-9-2
 *******************************************************************************/

#include "../key_inner.h"

#define KEY_MSG_DEPTH   8//gui 消息队列深度
#define NO_KEY KEY_NULL
#define DOWN_KEY_TIMER  3//按键按下消息时间为60ms
#define LONG_KEY_TIMER  5//按键长按消息时间为 60ms + (5-1)*HOLD_KEY_TIMER = 700ms
#define HOLD_KEY_TIMER  8//按键连续hold消息间隔为160ms

extern unsigned int g_rtc_counter;
extern unsigned int key_count; //当前按键消息发生的次数
extern unsigned int key_value; //当前按键的值

/******************************************************************************/
/*!
 * \par  Description:
 *	  Post key package message to OS
 * \param[in]    *input_msg
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
 * 封装按键信息，并发送给消息队列
 *******************************************************************************/
bool post_key_msg(input_gui_msg_t *input_msg)
{
    uint32 key_data;

    //消息压缩
    key_data = (uint32) input_msg->type;//byte 0
    if (input_msg->type == INPUT_MSG_KEY)
    {
        key_data |= ((uint32) input_msg->data.kmsg.val << 8);//byte 1
        key_data |= ((uint32) input_msg->data.kmsg.type << 16);//byte 2-3
    }
    else if (input_msg->type == INPUT_MSG_TOUCH)
    {
        key_data |= ((uint32) input_msg->data.tmsg.x << 8);//bit[8-17]
        key_data |= ((uint32) input_msg->data.tmsg.y << 18);//bit[18-27]
        key_data |= ((uint32) input_msg->data.tmsg.type << 28);//bit[28-31]
    }
    else
    {

    }

    if ((int) sys_mq_send(MQ_ID_GUI, (void *) &key_data) == 0)
    {
        //libc_print("key", key_data);
        return TRUE;
    }

    //消息队列已满，返回false
    return FALSE;
}

/******************************************************************************/
/*!
 * \par  Description:
 *	  Key message deal
 * \param[in]    key
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
 * 处理长短按按键，并发送相应按键消息
 *******************************************************************************/
void PutSysMsg(unsigned int key)
{
    input_gui_msg_t input_msg;
    input_msg.type = INPUT_MSG_KEY;
    if (key == key_value)
    {

        //g_rtc_counter = 0;
        key_count++;

        /*按住按键时，160ms发送一次相同按键消息*/
        if (key_count == LONG_KEY_TIMER)
        {
            //key = (unsigned int) (key | AP_KEY_LONG); //=0.7s,发长按键消息
            input_msg.data.kmsg.val = key_value;
            input_msg.data.kmsg.type = KEY_TYPE_LONG;
        }
        else if (key_count > LONG_KEY_TIMER)
        {
            //key = (unsigned int) (key | AP_KEY_HOLD); //>0.7s,每160ms发Key hold消息
            input_msg.data.kmsg.val = key_value;
            input_msg.data.kmsg.type = KEY_TYPE_HOLD;
        }
        else
        {
            return;
        }
        //return key;   //putmsg

        post_key_msg(&input_msg);
        return;

    }
    switch (key)
    {
        case Msg_KeyShortUp:
        //gui消息队列已都是HOLD消息，可先取走一个再发 SHORT_UP ，这样保证 SHORT_UP 消息能够发送成功
        if(key_count >= LONG_KEY_TIMER + KEY_MSG_DEPTH)
        {
            //先接收gui消息，保证gui消息队列非满
            uint32 key_data;
            sys_mq_receive(MQ_ID_GUI, (void *)&key_data);
        }
        key_count = 0;
        //g_rtc_counter = 0;
        /*key_value的置需改变*/
        input_msg.data.kmsg.val = key_value;
        input_msg.data.kmsg.type = KEY_TYPE_SHORT_UP;
        key_value = KEY_NULL;

        break;
        case Msg_KeyHold:
        key_count = 0;
        //g_rtc_counter = 0;
        /*key_value的置需改变*/
        input_msg.data.kmsg.val = KEY_LOCK;
        input_msg.data.kmsg.type = KEY_TYPE_DOWN;
        key_value = KEY_NULL;
        break;
        case Msg_KeyUnHold:
        key_count = 0;
        //g_rtc_counter = 0;
        /*key_value的置需改变*/
        input_msg.data.kmsg.val = KEY_UNLOCK;
        input_msg.data.kmsg.type = KEY_TYPE_DOWN;
        key_value = KEY_NULL;
        break;
        default: /*新按键处理*/
        //g_rtc_counter = 0;
        key_value = key;
        input_msg.data.kmsg.val = key;
        input_msg.data.kmsg.type = KEY_TYPE_DOWN;
        key_count = 1;
        break;
    }
    //puts key msg in here

    post_key_msg(&input_msg);
    return;
}

