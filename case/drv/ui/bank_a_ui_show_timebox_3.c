/*******************************************************************************
 *                              US212A
 *                            Module: ui driver
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>           <time>          <version >             <desc>
 *       liutingting     2011-09-05 10:00     1.0             build this file
 *******************************************************************************/
/*!
 * \file    ui_show_timebox_3.c
 * \brief    时间数值处理转换模块
 * \author   liutingting
 * \par      GENERAL DESCRIPTION:
 *               实现时间数值的转换处理
 * \par      EXTERNALIZED FUNCTIONS:
 *               none
 * \version  1.0
 * \date  2011/9/05
 *******************************************************************************/
#include "display.h"
#include "ui_driver.h"

#define TIMEBOX_COUNT_OPTION_SHIFT      (4)
#define TIMEBOX_COUNT_OPTION            (0x0001 << TIMEBOX_COUNT_OPTION_SHIFT)
#define COUNT_OPTION_ONE                (0x0000 << TIMEBOX_COUNT_OPTION_SHIFT)
#define COUNT_OPTION_TWO                (0x0001 << TIMEBOX_COUNT_OPTION_SHIFT)
#define TIMEBOX_ZERO_EN(a)              (0x0001 << (5+(a)))
#define TIMEBOX_FIRST_DIGIT_SHIFT(a)    (7 + 3*(a))
#define TIMEBOX_FIRST_DIGIT(attrib, a)  \
(((attrib) & (0x0007 << TIMEBOX_FIRST_DIGIT_SHIFT(a))) >> TIMEBOX_FIRST_DIGIT_SHIFT(a))

static uint8 count_digit(uint16 num)
{
    uint8 dight = 1;
    
    while(num >= 10)
    {
        dight++;
        num = num / 10;
    }
    
    return dight;
}

/*! \cond */
/******************************************************************************/
/*!
 * \par  Description:
 *    字符串显示timebox
 * \param[in]   time：应用传递的timebox的私有数据结构体指针
 * \param[in]   timebox：timebox控件数据结构体指针
 * \param[out]  time_buffer：保存时间数值的buffer指针
 * \param[out]  disp_bits：保存时、分、秒显示位数的buffer指针
 * \return      总共要显示几个数字
 * \note
 *******************************************************************************/
uint8 TimeConvert(timebox_private_t *time, timebox_t *timebox, uint16 *time_buffer, uint8 *disp_bits)
{
    uint16 buffer[8];
    uint8 num_bits[8];
    uint8 i, j;

    libc_memset(buffer, -1, sizeof(buffer));
    libc_memset(num_bits, 0, sizeof(num_bits));
    if ((timebox->attrib & TIMEBOX_COUNT_OPTION) == COUNT_OPTION_ONE)
    {
        j = 1;
    }
    else
    {
        j = 2;
    }
    for (i = 0; i < j; i++)
    {
        switch(time->times[i].mode)
        {
        //将年、月、日的值转换成需要的格式
        case DATE_DISP_YYMMDD:
        case DATE_DISP_YYMM:
            buffer[4 * i] = time->times[i].union_time.date.year;
            buffer[4 * i + 1] = time->times[i].union_time.date.month;
            if (time->times[i].mode == DATE_DISP_YYMMDD)
            {
                buffer[4 * i + 2] = time->times[i].union_time.date.day;
            }
            
            //年份位数由第一个数字的位数指定
            num_bits[4 * i] = (uint8)TIMEBOX_FIRST_DIGIT(timebox->attrib, i);
            num_bits[4 * i + 1] = 2;
            if (time->times[i].mode == DATE_DISP_YYMMDD)
            {
                num_bits[4 * i + 2] = 2;
            }
            break;

        case DATE_DISP_MMDDYY:
        case DATE_DISP_MMDD:
            buffer[4 * i] = time->times[i].union_time.date.month;
            buffer[4 * i + 1] = time->times[i].union_time.date.day;
            if (time->times[i].mode == DATE_DISP_MMDDYY)
            {
                buffer[4 * i + 2] = time->times[i].union_time.date.year;
            }
            
            num_bits[4 * i] = 2;
            num_bits[4 * i + 1] = 2;
            if (time->times[i].mode == DATE_DISP_MMDDYY)
            {
                //年份位数由第一个数字的位数指定
                num_bits[4 * i + 2] = (uint8)TIMEBOX_FIRST_DIGIT(timebox->attrib, i);
                if(num_bits[4 * i + 2] == 4)
                {
                    buffer[4 * i + 2] += 2000;
                }
            }
            break;
        
        //将时分秒的值转成需要的格式
        case TIME_DISP_HHMMSS:
        case TIME_DISP_HHMM:
            buffer[4 * i] = time->times[i].union_time.time.hour;
            buffer[4 * i + 1] = time->times[i].union_time.time.minute;
            if (time->times[i].mode == TIME_DISP_HHMMSS)
            {
                buffer[4 * i + 2] = time->times[i].union_time.time.second;
            }
            
            //小时位数由第一个数字的位数指定
            num_bits[4 * i] = (uint8) TIMEBOX_FIRST_DIGIT(timebox->attrib, i);
            if ((timebox->attrib & TIMEBOX_ZERO_EN(i)) != TIMEBOX_ZERO_EN(i))
            {
                //不显示前置0，如果真实位数少于指定位数，以真实位数显示
                if(count_digit(buffer[4 * i]) < num_bits[4 * i])
                {
                    num_bits[4 * i] = count_digit(buffer[4 * i]);
                }
            }
            num_bits[4 * i + 1] = 2;
            if (time->times[i].mode == TIME_DISP_HHMMSS)
            {
                num_bits[4 * i + 2] = 2;
            }
            break;
            
        case TIME_DISP_MMSS:
            buffer[4 * i + 1] = time->times[i].union_time.time.minute;
            buffer[4 * i + 2] = time->times[i].union_time.time.second;
            
            //分钟位数由第一个数字的位数指定
            num_bits[4 * i + 1] = (uint8) TIMEBOX_FIRST_DIGIT(timebox->attrib, i);
            if (num_bits[4 * i + 1] > 2)//最多显示2位
            {
                num_bits[4 * i + 1] = 2;
            }
            if ((timebox->attrib & TIMEBOX_ZERO_EN(i)) != TIMEBOX_ZERO_EN(i))
            {
                //不显示前置0，如果真实位数少于指定位数，以真实位数显示
                if(count_digit(buffer[4 * i + 1]) < num_bits[4 * i + 1])
                {
                    num_bits[4 * i + 1] = count_digit(buffer[4 * i + 1]);
                }
            }
            num_bits[4 * i + 2] = 2; //秒总是显示2位
            break;
            
        default:
            //do nothing for QAC
            break;
        }
    }
    j = 0;
    for (i = 0; i < 8; i++)
    {
        if (buffer[i] != (uint16) - 1)
        {
            time_buffer[j] = buffer[i];
            disp_bits[j] = num_bits[i];
            j++;
        }
    }
    return j;
}
/*! \endcond */
