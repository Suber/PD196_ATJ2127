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
 * \file     eh_lrc_get.c
 * \brief    获取歌词
 * \author   liminxian
 * \par      GENERAL DESCRIPTION:
 *               这里对文件进行描述
 * \par      EXTERNALIZED FUNCTIONS:
 *               FS,VRAM
 * \version 1.0
 * \date  2011/9/15
 *******************************************************************************/

#include "eh_lrc.h"

//static const uint16 ms_table[10] =
//{ 0, 100, 200, 300, 400, 500, 600, 700, 800, 900 }; //毫秒取值表

static uint8 *lrc_vram_read_p _BANK_DATA_ATTR_;
static lrc_lable_t *lrc_lab_read_p _BANK_DATA_ATTR_; // 存时间标签指针
static uint8 lrc_read_buf[SECTOR_SIZE] _BANK_DATA_ATTR_; //读lrc缓存

uint16 get_num_bytime(uint32 time, uint16 start_num, uint16 count);

/******************************************************************************/
/*
 * \par  Description: 读取歌词初始化
 *
 * \param[in]
 * \param[out]

 * \return        TRUE OR FALSE

 * \note          从vm中读出lrc_lab_count值
 *******************************************************************************/
bool lyric_get_init(void)
{
    sys_vm_read(&lrc_lab_count, LRC_INFO_VM, sizeof(lrc_lab_count));
    lrc_cur_num = 0;
    lrc_prev_time = 0;
    lrc_cur_time = 0;
    lrc_next_time = 0;

    if (lrc_lab_count != 0)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/******************************************************************************/
/*
 * \par  Description: 根据标签号获取歌词
 *
 * \param[in]    lrc_buf,存放获取到的歌词，可以为NULL
 len,值lrc_buf的长度（单位byte）
 page_num，标签号（1~N）

 * \param[out]

 * \return       TRUE OR FALSE

 * \note
 *******************************************************************************/
bool get_lrc_byno(uint8 *lrc_buf, uint8 len, uint16 page_num)
{
    uint16 temp;

    if ((page_num > lrc_lab_count) || (lrc_lab_count == 0))
    {
        return FALSE;
    }
    if (page_num == 1)
    {
        temp = 1;
    }
    else
    {
        temp = 2;
    }
    lrc_vram_read_p = (uint8*) (LRC_LABLE_VM + (page_num - temp) * sizeof(lrc_lable_t));
    lrc_lab_read_p = (lrc_lable_t*) lrc_read_buf;
    sys_vm_read(lrc_lab_read_p, lrc_vram_read_p, SECTOR_SIZE);

    lrc_prev_time = 0;
    lrc_next_time = LRC_TIME_MAX;

    if ((page_num - 1) > 0)
    {
        lrc_prev_time = (uint32) lrc_lab_read_p->min * 60000 + (uint32) lrc_lab_read_p->sec * 1000
                + (uint32) lrc_lab_read_p->p_ms * 10;
        lrc_lab_read_p++;
    }

    lrc_cur_time = (uint32) lrc_lab_read_p->min * 60000 + (uint32) lrc_lab_read_p->sec * 1000
            + (uint32) lrc_lab_read_p->p_ms * 10;

    lrc_lab_read_p++;

    if ((page_num + 1) <= lrc_lab_count)
    {
        lrc_next_time = (uint32) lrc_lab_read_p->min * 60000 + (uint32) lrc_lab_read_p->sec * 1000
                + (uint32) lrc_lab_read_p->p_ms * 10;
    }

    if ((len == 0) || (lrc_buf == NULL))
    {
        return TRUE;
    }

    lrc_lab_read_p--;

    temp = lrc_lab_read_p->lrc_len;

    if (temp > (len - 1))
    {
        temp = len - 1;
    }
    if (temp != 0)//防止歌词长度为0时出错
    {
        sys_vm_read(lrc_buf, (void*) (lrc_lab_read_p->offset_byte + LRC_CONTENT_VM), (uint32) temp);
    }
    lrc_buf[temp] = '\0';
    return TRUE;

}

/******************************************************************************/
/*
 * \par  Description:顺序获取下一条标签的歌词
 *
 * \param[in]    lyric_str，歌词内容
 len，lyric_str的长度（单位byte）
 * \param[out]

 * \return       TRUE OR FALSE

 * \note
 *******************************************************************************/
bool lyric_get_next(char * lyric_str, uint8 len)
{

    if ((0 == len) || (NULL == lyric_str))
    {
        return FALSE;
    }

    if ((lrc_cur_num + 1) > lrc_lab_count)
    {
        return FALSE;
    }
    lrc_cur_num++;
    return get_lrc_byno(lyric_str, len, lrc_cur_num);
}

/******************************************************************************/
/*
 * \par  Description: 获取上一天标签的歌词
 *
 * \param[in]    lyric_str，歌词内容
 len，lyric_str的长度（单位byte）

 * \param[out]

 * \return       TRUE OR FALSE

 * \note
 *******************************************************************************/
bool lyric_get_prev(char * lyric_str, uint8 len)
{

    if ((0 == len) || (NULL == lyric_str))
    {
        return FALSE;
    }

    if (lrc_cur_num == 1)
    {
        return FALSE;
    }
    lrc_cur_num--;
    return get_lrc_byno(lyric_str, len, lrc_cur_num);
}

/******************************************************************************/
/*
 * \par  Description:获取类型时间
 *
 * \param[in]    time_type，时间类型
 time，填入时间，uint32型，单位:MS
 * \param[out]

 * \return        TRUE OR FALSE

 * \note
 *******************************************************************************/
bool lyric_get_time(lrc_time_e time_type, uint32 *time)
{
    bool result = TRUE;

    if (time == NULL)
    {
        return FALSE;
    }

    switch (time_type)
    {
        case LYRIC_NEXT_TIME:
        *time = lrc_next_time;
        break;

        case LYRIC_CUR_TIME:
        *time = lrc_cur_time;
        break;

        case LYRIC_PREV_TIME:
        *time = lrc_prev_time;
        break;

        default:
        result = FALSE;
        break;
    }

    return result;
}

/******************************************************************************/
/*
 * \par  Description: 检查当前时间是否要换歌词标签
 *
 * \param[in]    time，当前歌曲播放时间 ms

 * \param[out]

 * \return        TRUE OR FALSE

 * \note
 *******************************************************************************/
bool lyric_check_query_time(uint32 time)
{

    if ((time >= lrc_next_time) || (time < lrc_cur_time))
    {
        return TRUE;
    }

    return FALSE;

}

/******************************************************************************/
/*
 * \par  Description: 根据输入时间获取歌词内容
 *
 * \param[in]    lyric_str，填入歌词内容buffer
 len，lyric_str的长度，单位byte
 time，当前播放时间
 * \param[out]

 * \return        TRUE OR FALSE

 * \note
 *******************************************************************************/
bool lyric_seek_for(char *lyric_str, uint8 len, uint32 time)
{
    uint16 temp_num;
    //    uint32 temp_time;

    if ((lyric_str == NULL) || (lrc_lab_count == 0))
    {
        return FALSE;
    }

    if ((len == 0) || (time >= LRC_TIME_MAX))
    {
        return FALSE;
    }

    //tiem=0,的处理返回失败
    if (time == 0)
    {
        //       lrc_cur_num=1;
        //       return get_lrc_byno(lyric_str,len,1);
        lrc_cur_num = 0;
        return FALSE;
    }

    temp_num = (lrc_lab_count + 1) / 2;

    get_lrc_byno(NULL, 0, temp_num);

    if (time >= lrc_cur_time)
    {
        if (time > lrc_next_time)
        {
            //向下查time
            temp_num = get_num_bytime(time, temp_num, lrc_lab_count - temp_num + 1);
        }
    }
    else
    {
        if (time >= lrc_prev_time)
        {
            temp_num -= 1;
        }
        else
        {
            //向上查time
            temp_num = get_num_bytime(time, 1, temp_num);
        }
    }

    if (temp_num == 0)
    {
        return FALSE;
    }

    lrc_cur_num = temp_num;

    return get_lrc_byno(lyric_str, len, temp_num);

}

/******************************************************************************/
/*
 * \par  Description: 从指定起始号查指定时间对应的歌词标签号
 *
 * \param[in]     time，指定时间
 start_num，起始便签好
 count，查找的标签个数

 * \param[out]

 * \return        非0为成功，0失败

 * \note
 *******************************************************************************/
uint16 get_num_bytime(uint32 time, uint16 start_num, uint16 count)
{
    //    uint8 copy_len;
    uint16 label_cn;
    uint16 temp_num;
    uint32 temp_time;

    if ((start_num > lrc_lab_count) || (count == 0))
    {
        return 0;
    }

    lrc_vram_read_p = (uint8*) (LRC_LABLE_VM + (start_num - 1) * sizeof(lrc_lable_t));
    label_cn = SECTOR_SIZE;
    temp_time = 0;
    temp_num = start_num;

    while (count > 0)
    {
        count--;
        if (label_cn >= SECTOR_SIZE)
        {
            label_cn = 0;
            lrc_lab_read_p = (lrc_lable_t*) lrc_read_buf;
            sys_vm_read(lrc_lab_read_p, lrc_vram_read_p, SECTOR_SIZE);
            lrc_vram_read_p += SECTOR_SIZE;
        }

        temp_time = (uint32) lrc_lab_read_p->min * 60000 + (uint32) lrc_lab_read_p->sec * 1000
                + (uint32) lrc_lab_read_p->p_ms * 10;

        if (temp_time > time)
        {
            return temp_num - 1;
        }

        temp_num++;
        lrc_lab_read_p++;
        label_cn = label_cn + sizeof(lrc_lable_t);

    }

    //最后一条歌词在1.5s内则显示
    if (temp_time + 90000 > time)
    {
        return temp_num - 1;
    }

    return 0;

}

