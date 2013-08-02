/*******************************************************************************
 *                              US212A
 *                            Module: ENHANCED
 *                 Copyright(c) 2003-2011 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       liminxian  2011-9-15 15:37    1.0             build this file
 *******************************************************************************/
/*!
 * \file     eh_lrc_load_sub3.c
 * \brief    解析歌词
 * \author   liminxian
 * \par      GENERAL DESCRIPTION:
 *               这里对文件进行描述
 * \par      EXTERNALIZED FUNCTIONS:
 *               FS
 * \version 1.0
 * \date  2011/9/15
 *******************************************************************************/

#include "eh_lrc.h"
#include "Simulator.h"

extern uint32 lrc_pos_buffer;
extern uint32 lrc_pos_file;
extern uint32 lrc_file_len;

extern uint8 positiveflag; //TRUE--'+',FALSE---'-'
extern uint8 offset_sec; // 1 s
extern uint8 offset_ms; //10ms


extern lrc_lable_t *lrc_lab_p; // 存时间标签指针
extern uint8 *buf_point; //lrc的buf解析指针
extern uint8 *save_lrc_buf_p; //存歌词内容指针
extern uint8 *lrc_vram_p;

extern uint8 get_time_buf[8]; //获取时间标签临时缓存
extern uint8 *get_time_p; //缓存单个时间标签指针


extern uint16 same_str_count; //同歌词的时间标签总数,1-只有1个标签，2-有2个标签
extern uint16 lrc_str_len; //每条歌词的长度
extern uint16 lab_lrc_offset; //每条歌词在内容中的偏移位置


extern uint8 lrc_temp_buf[SECTOR_SIZE]; //读lrc缓存
extern uint8 lrc_save_buf[SECTOR_SIZE]; //存歌词内容缓存

/******************************************************************************/
/*
 * \par  Description: 所有时间标签都加入歌词差异时间
 *
 * \param[in]

 * \param[out]

 * \return        TRUE OR FALSE

 * \note
 *******************************************************************************/
bool add_time_offset(void)
{
    uint8 i, j;
    uint8 count;

    lrc_lab_p = (lrc_lable_t *) LRC_LABLE_ADDR;

    if ((offset_ms == 0) && (offset_sec == 0))
    {
        //如要加入的时间为0，则不要加
        return TRUE;
    }

    if (FALSE != positiveflag)
    {
        for (count = 0; count < time_lab_count; count++)
        {
            //为正数
            //ClearWatchDog();
            if (((lrc_lab_p->sec != 0) || (lrc_lab_p->min != 0)) || (lrc_lab_p->p_ms != 0))
            {
                //10ms(0.01s)数位加
                j = offset_ms + lrc_lab_p->p_ms;
                i = 0;
                if (j >= 100)
                {
                    j = j - 100;
                    i = 1;
                }
                lrc_lab_p->p_ms = j;

                //秒数位加
                j = offset_sec + i + lrc_lab_p->sec;
                i = 0;
                if (j >= 60)
                {
                    j = j - 60;
                    i = 1;
                }

                lrc_lab_p->sec = j;
                //分位数加
                lrc_lab_p->min += i;
            }
            lrc_lab_p++; //指针加一
        }
    }
    else
    {
        //offset_sec有可能大于60，可能超过1分钟，只是小于100
        //时间标签的秒钟已经调整了，其超过60会向分钟进位，所以
        //当偏移是80秒时，原来时间为1分钟 10秒 100毫秒时，
        //比较仍然不成立，但却是错误的
        //tmpbuf1[0] = offset_ms;
        //tmpbuf1[1] = offset_sec;
        //tmpbuf1[2] = 0;
        //tmpbuf1[3] = 0;

        uint8 tmpbuf1[4];
        uint8 tmpbuf2[4];

        if (offset_sec >= 60)
        {
            tmpbuf1[3] = 1;
            tmpbuf1[2] = offset_sec - 60;
        }
        else
        {
            tmpbuf1[3] = 0;
            tmpbuf1[2] = offset_sec;
        }
        tmpbuf1[1] = offset_ms;
        tmpbuf1[0] = 0;

        for (count = 0; count < time_lab_count; count--)
        {
            //ClearWatchDog();
            if ((lrc_lab_p->sec != 0) || (lrc_lab_p->min != 0) || (lrc_lab_p->p_ms != 0))
            {
                tmpbuf2[3] = lrc_lab_p->min;
                tmpbuf2[2] = lrc_lab_p->sec;
                tmpbuf2[1] = lrc_lab_p->p_ms;
                tmpbuf2[0] = 0;

                if (*(uint32 *) tmpbuf1 >= *(uint32 *) tmpbuf2)
                {
                    //处理时间条比OFFSET值小的情况
                    lrc_lab_p->min = 0;
                    lrc_lab_p->sec = 0;
                    lrc_lab_p->p_ms = 0;
                    lrc_lab_p++; //指针加一
                    continue;
                }

                //100ms(0.1s)数位减
                if (lrc_lab_p->p_ms >= offset_ms)
                {
                    j = lrc_lab_p->p_ms;
                    i = 0;
                }
                else
                {
                    j = lrc_lab_p->p_ms + 10;
                    i = 1;//借了1
                }
                lrc_lab_p->p_ms = j - offset_ms;

                //秒数位减
                if (lrc_lab_p->sec >= (tmpbuf1[2] + i))
                {
                    j = lrc_lab_p->sec - i;
                    i = 0;
                }
                else
                {
                    j = lrc_lab_p->sec + 60 - i;
                    i = 1;
                }
                lrc_lab_p->sec = j - tmpbuf1[2];

                //分位数减
                lrc_lab_p->min -= (i + tmpbuf1[3]);
            }
            lrc_lab_p++; //指针加一
        }

    }

    return TRUE;

}

/******************************************************************************/
/*
 * \par  Description: 排序时间标签
 *
 * \param[in]

 * \param[out]

 * \return

 * \note
 *******************************************************************************/
void sort_lrc_label(void)
{
    uint16 i, j;
    lrc_lab_p = (lrc_lable_t *) LRC_LABLE_ADDR;

    //冒泡算法排序
    for (i = 1; i <= time_lab_count; i++)
    {
        for (j = i; j > 0; j--)
        {
            //ClearWatchDog();
            if (lrc_lab_p[j].min > lrc_lab_p[j - 1].min)
            {
                break;//continue;
            }
            else if ((lrc_lab_p[j].min == lrc_lab_p[j - 1].min) && (lrc_lab_p[j].sec > lrc_lab_p[j - 1].sec))
            {
                break;//continue;
            }
            else if ((lrc_lab_p[j].min == lrc_lab_p[j - 1].min) && (lrc_lab_p[j].sec == lrc_lab_p[j - 1].sec)
                    && ((lrc_lab_p[j].p_ms) >= (lrc_lab_p[j - 1].p_ms)))
            {
                break;//continue;
            }
            else//一定要小于才交换，保证歌词顺序
            {
                libc_memcpy(lrc_temp_buf, lrc_lab_p + j, sizeof(lrc_lable_t));
                libc_memcpy(lrc_lab_p + j, lrc_lab_p + j - 1, sizeof(lrc_lable_t));
                libc_memcpy(lrc_lab_p + j - 1, lrc_temp_buf, sizeof(lrc_lable_t));
            }
        }
    }
    return;
}

/******************************************************************************/
/*
 * \par  Description: 解析完歌词后处理流程
 *
 * \param[in]

 * \param[out]

 * \return

 * \note
 *******************************************************************************/
void explain_ok_deal(void)
{
    uint16 temp;
    uint8 *t_lrc_lab_p;

    // 把剩下的歌词内容写入VRAM
    if ((lab_lrc_offset % SECTOR_SIZE) != 0)
    {
        sys_vm_write(lrc_save_buf, lrc_vram_p);
    }

    // 排序标签
    sort_lrc_label();

    //把时间标签写入VRAM
    temp = (sizeof(lrc_lable_t) * time_lab_count - 1) / SECTOR_SIZE + 1;
    t_lrc_lab_p = (uint8*) LRC_LABLE_ADDR;
    lrc_vram_p = (uint8*) LRC_LABLE_VM;

    while (temp > 0)
    {
        temp--;
        sys_vm_write(t_lrc_lab_p, lrc_vram_p);
        lrc_vram_p += SECTOR_SIZE;
        t_lrc_lab_p += SECTOR_SIZE;
    }

    return;
}

