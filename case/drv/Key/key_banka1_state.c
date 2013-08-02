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
 * \file     key_state.c
 * \brief    Get hold state and key map address
 * \author   reagan
 * \version 1.0
 * \date  2011-9-2
 *******************************************************************************/

#include "key_inner.h"



/******************************************************************************/
/*!
 * \par  Description:
 *    设置喇叭可用与否。
 * \param[in]    onoff 1表示可用，当耳机拔出时会喇叭发生，0表示不可用
 * \param[out]   uint8 设置成功与否，0表示成功，1表示失败。
 * \ingroup      key_settings
 * \par          exmaple code
 * \code
 * //设置喇叭可用
 * key_speakcheck(1);
 *
 * //设置喇叭不可用
 * key_speakcheck(0);
 * \endcode
 * \note
 *******************************************************************************/
uint8 *key_inner_speakcheck(uint8 onoff, void* null1, void* null2)
{
    g_speakonoff = onoff ;
    if(g_speakonoff == 0)
    {
        act_writel(act_readl(GPIO_ADAT) &0xfffffffd, GPIO_ADAT);//enable speak control mute
    }
    return 0;
}

/******************************************************************************/
/*!
 * \par  Description:
 *    获取按键映射表地址。
 * \param[in]    none
 * \param[out]   uint8* 按键映射表地址，该表在按键驱动常驻数据空间中。
 * \ingroup      key_infor
 * \par          exmaple code
 * \code
 *  //获取按键映射表地址，然后更新用户个性化映射表
 *  key_table_addr = key_getkeytabaddress();
 *  libc_memcpy(key_table_addr, g_config_var.keyID, sizeof(g_config_var.keyID));
 * \endcode
 * \note
 *******************************************************************************/
uint8 *key_inner_getkeytabaddress(void* null0, void* null1, void* null2)
{
    return key_map;
}

/******************************************************************************/
/*!
 * \par  Description:
 *    获取拨动hold按键状态。
 * \param[in]    none
 * \param[out]   uint8
 * \return       the result
 * \retval           0 hold没有锁住
 * \retval           1 hold锁住
 * \ingroup      key_infor
 * \par          exmaple code
 * \code
 *  //获取hold按键状态，校正应用hold状态
 *  hold_state = key_holdcheck();
 * \endcode
 * \note
 *******************************************************************************/
uint8 key_inner_holdcheck(void* null0, void* null1, void* null2)
{
    unsigned char key_val;
    unsigned char ret;

    key_val = act_readb(LRADC1_DATA) & 0x3f;
    if ((key_val < MIN_HOLD_ADC) || (key_val > MAX_HOLD_ADC))
    {
        ret = 0; //unhold
    }
    else
    {
        ret = 1; //hold
    }

    return ret;
}

