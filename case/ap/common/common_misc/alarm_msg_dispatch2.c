/*******************************************************************************
 *                              us212A
 *                            Module: common
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 *
 * History:         
 *      <author>    <time>           <version >             <desc>
 *       lzcai     2012-4-13 16:53     1.0             build this file 
*******************************************************************************/
/*!
 * \file     _alarm_msg_dispatch2.c
 * \brief    
 * \author   lzcai
 * \par      GENERAL DESCRIPTION:
 *               独立算法函数
 * \par      EXTERNALIZED FUNCTIONS:
 *               
 * \version  1.0
 * \date     2012-4-13
*******************************************************************************/

#include "common_ui.h"

/*! \cond */
/******************************************************************************/
/*!
 * \par  Description:
 *    根据输入的年月日计算星期
 * \param[in]    uint16 y
 * \param[in]    uint8 m
 * \param[in]    uint8 d
 * \param[out]   none
 * \return       uint8 result
 * \ingroup      
 * \note
 *******************************************************************************/
uint8 __get_weekofday(uint16 y, uint8 m, uint8 d)
{
    int8 wk; //星期
    uint8 ic; //年份的前两位
    uint8 ir; //年份的后两位

    if (m < 3)
    {
        m += 12;
        y--;
    }
    ic = (uint8) (y / 100);
    ir = (uint8) (y % 100);
    //Taylor's formula
    wk = (int8)((ic / 4) + ir + (ir / 4) + (26 * (m + 1)) / 10 - 2 * ic + d - 1); // 避免负数
    while (wk >= 7) //矫正结果
    {
        wk -= 7;
    }
    while (wk < 0)
    {
        wk += 7;
    }
    return wk;
}


/******************************************************************************/
/*!
 * \par  Description:
 *    比较 cur_time、time1、time2、time3的时间先后顺序
 * \param[in]    time_t *Cur_T
 * \param[in]    time_t *timer1
 * \param[in]    time_t *timer2
 * \param[in]    uint8 num
 * \param[out]   none
 * \return       uint8 result
 * \ingroup      
 * \note
 *******************************************************************************/
uint8 compare_curtime(time_t *Cur_T, time_t *timer1, time_t *timer2, /*time_t *timer3,*/ uint8 num)
{
    uint32 time_n[4], ser_n[4], i, j, temp_min, temp_i;
    uint8 same_time_flag = 0;
    time_n[0] = (Cur_T->hour) *60*60 + Cur_T->minute*60 + Cur_T->second;    //cur_time
    time_n[1] = (timer1->hour) *60*60 + timer1->minute*60; //FM_time
    time_n[2] = (timer2->hour) *60*60 + timer2->minute*60; //alarm1_time
//    time_n[3] = (timer3->hour) * 60 + timer3->minute; //alarm2_time
    if (num > 4)
    {
        return 0;
    }
    if (time_n[1] == time_n[2])
    {
        same_time_flag = 1;
        num = num - 1;
    }
    for (i = 0; i < num; i++)
    {
        ser_n[i] = i;
    }
    for (j = 0; j < (num - 1); j++)
    {
        for (i = 0; i < (num - 1 - j); i++)
        {
            if (time_n[i] > time_n[i + 1])
            {
                temp_min = time_n[i];
                time_n[i] = time_n[i + 1];
                time_n[i + 1] = temp_min;
                temp_i = ser_n[i];
                ser_n[i] = ser_n[i + 1];
                ser_n[i + 1] = temp_i;
            }
        }
    }
    for (i = 0; i < num; i++)
    {
        if (ser_n[i] == 0)
        {
            if (i == (num - 1))
            {
                return ser_n[0];
            }
            else
            {
                if (same_time_flag == 1)
                {
                    return 4;
                }
                else
                {
                    return ser_n[i + 1];
                }
            }

        }
    }
    return 0;
}

/*! \endcond */
