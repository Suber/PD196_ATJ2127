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
 * \file     key_banka_state.c
 * \brief    Get hold state and key map address
 * \author   reagan
 * \version 1.0
 * \date  2011-9-2
 *******************************************************************************/

#include "key_inner.h"



/******************************************************************************/
/*!
 * \par  Description:
 *    return key map address to application
 * \param[in]    none
 * \param[out]   unsigned int
 * \ingroup      key drvier
 * \par          exmaple code
 * \code
 *               list example code in here
 * \endcode
 * \note
 * speak 设置开关配置
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
 *    return key map address to application
 * \param[in]    none
 * \param[out]   unsigned int
 * \ingroup      key drvier
 * \par          exmaple code
 * \code
 *               list example code in here
 * \endcode
 * \note
 * 返回按键影射地址给应用
 *******************************************************************************/
uint8 *key_inner_getkeytabaddress(void* null0, void* null1, void* null2)
{
    return key_map;
}

/******************************************************************************/
/*!
 * \par  Description:
 *    charge flow deal
 * \param[in]    none
 * \param[in]    none
 * \param[in]    none
 * \param[out]   uint8
 * \return       the result
 * \retval           0 no lock
 * \retval           1 lock
 * \ingroup      key drvier
 * \par          exmaple code
 * \code
 *               list example code in here
 * \endcode
 * \note
 * 负责充电处理，这个会被定时器轮询的。
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

