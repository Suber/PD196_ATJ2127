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
 * \file     *.c
 * \brief    随机机制
 * \author   liminxian
 * \par      GENERAL DESCRIPTION:
 *               这里对文件进行描述
 * \par      EXTERNALIZED FUNCTIONS:
 *
 * \version 1.0
 * \date  2011/9/15
 *******************************************************************************/

#include "eh_fsel.h"

//#define  OVERTURN_DEF  //反转bitmap播放功能


static const uint8 byte_set_val[8] =
{ 0x01, 0x03, 0x07, 0x0f, 0x1f, 0x3f, 0x7f, 0xff };
static const uint8 byte_check_val[8] =
{ 0x01, 0x02, 0x4, 0x08, 0x10, 0x20, 0x40, 0x80 };

static void init_member_map(void);
static bool set_bitmap_bit(uint8*dest_map, uint16 num);
static uint16 find_bitmap_bit(uint8 *bitmap, uint8 count_max, uint16 rand_num);
static uint16 get_group_num(void);
static uint16 get_member_num(void);
static uint16 get_nextlist_num(void);
static uint16 get_prevlist_num(void);
static uint16 get_shuffle_num(void);

/******************************************************************************/
/*
 * \par  Description:根据组号初始化组内成员的map状态
 *
 * \param[in]

 * \param[out]

 * \return        TRUE OR FALSE

 * \note
 *******************************************************************************/

void init_member_map(void)
{
    uint8 i, j;
    for (i = 0; i < (member_avg / 8); i++)
    {
        member_map[i] = 0xff;
    }

    j = member_avg % 8;

    member_max = member_avg;

    if (cur_group < member_avg_group)
    {
        //组号在平均值组的前面时
        j++;
        member_max++;
    }

    if (j != 0)
    {
        member_map[i] = byte_set_val[j - 1];
    }

    return;

}

/******************************************************************************/
/*
 * \par  Description:初始shuffle机制
 *
 * \param[in]

 * \param[out]

 * \return        TRUE OF FALSE

 * \note        
 *******************************************************************************/
bool init_shuffle(uint16 num_total)
{
    uint8 i, j;
//在此处赋值为0，初始化不过滤文件
    filtrate_shuffle_no = 0;// 

    if (num_total == 0)
    {
        return FALSE;
    }

    libc_memset(group_map, 0x00, sizeof(group_map));
    libc_memset(member_map, 0x00, sizeof(member_map));
    use_member_flag = FALSE;
    cur_group = 0;
    cur_member = 0;
    //    cur_save_count=0;
    //    cur_play_pos=0;
    cur_save_pt = SET_PREV_MAX;
    cur_save_prev_count = 1;
    cur_save_next_count = 1;
    cur_overturn_flag = 0;

    shuffle_total = num_total;

    if (num_total > SET_GROUP_MAX)
    {//个数大于组数,启用组内成员shuffle
        use_member_flag = TRUE;
        libc_memset(group_map, 0xff, sizeof(group_map));
        group_max = SET_GROUP_MAX;
    }
    else
    {
        //个数少于组数,只用组shuffle就可以了
        group_max = (uint8) num_total;
        for (i = 0; i < (num_total / 8); i++)
        {
            group_map[i] = 0xff;
        }

        if ((num_total % 8) != 0)
        {
            //            j=num_total%8;
            //            group_map[i]=byte_set_val[j-1];
            j = (num_total % 8) - 1;
            group_map[i % (SET_GROUP_MAX / 8)] = byte_set_val[j];
        }
    }

    if (FALSE != use_member_flag)
    {
        member_avg = num_total / SET_GROUP_MAX;
        member_avg_group = (num_total % SET_GROUP_MAX) + 1;
    }

    return TRUE;

}

/******************************************************************************/
/*
 * \par  Description:把map翻转
 *
 * \param[in]

 * \param[out]

 * \return        TRUE OR FALSE

 * \note
 *******************************************************************************/

#ifdef OVERTURN_DEF

void overturn_map(void)
{
    uint8 i;
    uint8 temp8;

    cur_overturn_flag=(cur_overturn_flag+1)%2;

    for(i=0;i<(group_max/8);i++)
    {
        group_map[i]=(~group_map[i]);
    }

    temp8=group_max%8;

    if(temp8!=0)
    {
        group_map[i]=(~group_map[i])&byte_set_val[temp8-1];
    }

    temp8=member_max;

    if(cur_group<member_avg_group)
    {
        //组号在平均值组的前面时
        temp8++;
    }

    for(i=0;i<temp8/8;i++)
    {
        member_map[i]=~member_map[i];
    }

    temp8=temp8%8;

    if(temp8!=0)
    {
        member_map[i]=(~member_map[i])&member_map[temp8-1];
    }

}

#endif

/******************************************************************************/
/*
 * \par  Description:获取shuffle的组号
 *
 * \param[in]

 * \param[out]

 * \return      group_num(1~n)

 * \note
 *******************************************************************************/

static uint16 get_group_num(void)
{
    uint16 rand_num;
    uint8 i = 3;

    if (0 == group_max)
    {
        return 1;
    }

    while (i != 0)
    {
        i--;
        //try 3次
        rand_num = (sys_random() % group_max) + 1;
        if (FALSE != set_bitmap_bit(group_map, rand_num))
        {
            return rand_num;
        }
    }

    return find_bitmap_bit(group_map, group_max, rand_num);
}

/******************************************************************************/
/*
 * \par  Description:获取组内成员的shuffle号
 *
 * \param[in]

 * \param[out]

 * \return        member_num(1~n)

 * \note
 *******************************************************************************/

static uint16 get_member_num(void)
{
    uint16 rand_num;
    uint16 temp8;
    uint8 i = 3;

    if (0 == member_max)
    {
        return 1;
    }

    temp8 = member_max;

    if (cur_group < member_avg_group)
    {
        //组号在平均值组的前面时
        temp8++;
    }

    while (i != 0)
    {
        i--;
        //try 3次
        rand_num = (sys_random() % temp8) + 1;
        if (FALSE != set_bitmap_bit(member_map, rand_num))
        {
            return rand_num;
        }
    }

    return find_bitmap_bit(member_map, member_max, rand_num);

}

/******************************************************************************/
/*
 * \par  Description: 设置占用组的状态
 *
 * \param[in]    bitmap-位图表地址
 num-第几个bit(1~8)

 * \param[out]

 * \return        TRUE OR FALSE

 * \note
 *******************************************************************************/

static bool set_bitmap_bit(uint8*bitmap, uint16 num)
{
    uint8 i, j;

    num--;//变为从0开始
    i = num / 8;
    j = num % 8;

    if ((bitmap[i] & byte_check_val[j]) > 0)
    {
        bitmap[i] = bitmap[i] & (255 - byte_check_val[j]);
        return TRUE;
    }

    return FALSE;
}

/******************************************************************************/
/*
 * \par  Description:查找未播放过的成员
 *
 * \param[in]

 * \param[out]

 * \return        1~n

 * \note
 *******************************************************************************/

static uint16 find_bitmap_bit(uint8 *bitmap, uint8 count_max, uint16 rand_num)
{
    uint16 i;

    if ((rand_num & 0x01) > 0)
    {
        //从头往前找
        for (i = 1; i < (count_max + 1); i++)
        {
            if (FALSE != set_bitmap_bit(bitmap, i))
            {
                return i;
            }
        }
    }
    else
    {
        //从末往回找
        for (i = count_max; i > 0; i--)
        {
            if (FALSE != set_bitmap_bit(bitmap, i))
            {
                return i;
            }
        }

    }

    return 0;
}

/******************************************************************************/
/*
 * \par  Description:获取回溯队列中的下一个
 *
 * \param[in]

 * \param[out]

 * \return       file_no (1~file_total)

 * \note
 *******************************************************************************/

static uint16 get_nextlist_num(void)
{
    if (cur_save_pt >= (SET_PREV_MAX - 1))
    {
        //循环buffer的方式存放
        cur_save_pt = 0;
    }
    else
    {
        cur_save_pt++;
    }

    if (cur_save_prev_count < SET_PREV_MAX)
    {
        cur_save_prev_count++;
    }

    cur_save_next_count--;

    return play_num_list[cur_save_pt];

}

/******************************************************************************/
/*
 * \par  Description:获取回溯队列中的上一个
 *
 * \param[in]

 * \param[out]

 * \return        file_no (1~file_total)

 * \note
 *******************************************************************************/

static uint16 get_prevlist_num(void)
{
    if (cur_save_pt == 0)
    {
        //循环buffer的方式存放
        cur_save_pt = SET_PREV_MAX - 1;
    }
    else
    {
        cur_save_pt--;
    }

    if (cur_save_next_count < SET_PREV_MAX)
    {
        cur_save_next_count++;
    }

    cur_save_prev_count--;

    return play_num_list[cur_save_pt];
}

/******************************************************************************/
/*
 * \par  Description:获取播放的shuffle的number
 *
 * \param[in]

 * \param[out]

 * \return        file_no (1~file_total)

 * \note
 *******************************************************************************/

static uint16 get_shuffle_num(void)
{
    uint16 temp_num;

    if (FALSE == use_member_flag)
    {
        //只使用组号shuffle
        cur_group = (uint8) get_group_num();
        return (uint16) cur_group;
    }

    //使用组内shuffle

    if (0 == cur_group)
    {//首次进入
        cur_group = (uint8) get_group_num();
        if (0 == cur_group)
        {
            //所有组已经播放完
            return 0;
        }
        init_member_map();
    }

    cur_member = get_member_num();
    if (0 == cur_member)
    {
        //当前组成员已全部播放完
        cur_group = (uint8) get_group_num();
        if (0 == cur_group)
        {
            //所有组已经播放完
            return 0;
        }
        init_member_map();
        cur_member = get_member_num();
        if (0 == cur_member)
        {
            fsel_error = FSEL_ERR_SHUFFLE;
            return 0;
        }
    }

    temp_num = SET_GROUP_MAX * (cur_member - 1) + cur_group;
    return temp_num;
}

/******************************************************************************/
/*
 * \par  Description:获取shuffle机制的下一个
 *
 * \param[in]

 * \param[out]

 * \return        file_no (1~file_total)

 * \note
 *******************************************************************************/
//shuffle mode  get next
uint16 shuffle_next_file(void)
{
    uint16 temp_num;

    if (shuffle_total < file_total)
    {
        //文件个数变多了(增加文件)，重新初始化
        init_shuffle(file_total);
    }
    else
    {
        //重新赋值防止 file_total<shuffle_total的情况(文件删除)
        shuffle_total = file_total;
    }

    if (shuffle_total == 0)
    {
        return 0;
    }

    while (cur_save_next_count > 1)
    {
        temp_num = get_nextlist_num();
        if (temp_num <= shuffle_total)
        {
            if (temp_num != 0)
            {
                return temp_num;
            }
            else
            {
                break;
            }
        }

        cur_save_next_count--;
    }

#ifdef OVERTURN_DEF
    if(cur_save_pt!=SET_PREV_MAX && cur_overturn_flag==0)
    {
        overturn_map();
    }
#endif

    //list can't find,use shuffle al

    //保证随机出来的值在shuffle_total范围内,并与file_no不相同
    do
    {
        temp_num = get_shuffle_num();
        if (temp_num == 0)
        {
            break;
        }

    } while ((filtrate_shuffle_no == temp_num) || (temp_num > shuffle_total));

    if (0 != temp_num)
    {
        //SAVE TO PLAY_NUM_LIST
        if (cur_save_pt >= (SET_PREV_MAX - 1))
        {
            //循环buffer的方式存放
            cur_save_pt = 0;
        }
        else
        {
            cur_save_pt++;
        }

        if (cur_save_prev_count < SET_PREV_MAX)
        {
            cur_save_prev_count++;
        }
        play_num_list[cur_save_pt] = temp_num;
    }

    return temp_num;
}

/******************************************************************************/
/*
 * \par  Description:获取shuffle机制的上一个
 *
 * \param[in]

 * \param[out]

 * \return      file_no (1~file_total)

 * \note
 *******************************************************************************/
//shuffle mode  get prev
uint16 shuffle_prev_file(void)
{
    uint16 temp_num;

    if (shuffle_total < file_total)
    {
        //文件个数变多了(增加文件)，重新初始化
        init_shuffle(file_total);
    }
    else
    {
        //重新赋值防止 file_total<shuffle_total的情况(文件删除)
        shuffle_total = file_total;
    }

    if (shuffle_total == 0)
    {
        return 0;
    }

    while (cur_save_prev_count > 1)
    {
        temp_num = get_prevlist_num();

        if (temp_num <= shuffle_total)
        {
            if (temp_num != 0)
            {
                return temp_num;
            }
            else
            {
                break;
            }
        }

        cur_save_prev_count--;
    }

#ifdef OVERTURN_DEF
    if(cur_save_pt!=SET_PREV_MAX && cur_overturn_flag==1)
    {
        overturn_map();
    }
#endif

    //list can't find, use shuffle al

    //保证随机出来的值在shuffle_total范围内,并与file_no不相同
    do
    {
        temp_num = get_shuffle_num();
        if (temp_num == 0)
        {
            break;
        }

    } while ((filtrate_shuffle_no == temp_num) || (temp_num > shuffle_total));

    if (0 != temp_num)
    {
        //SAVE TO PLAY_NUM_LIST
        if (cur_save_pt == 0)
        {
            //循环buffer的方式存放
            cur_save_pt = SET_PREV_MAX - 1;
        }
        else
        {
            cur_save_pt--;
        }

        if (cur_save_next_count < SET_PREV_MAX)
        {
            cur_save_next_count++;
        }

        play_num_list[cur_save_pt] = temp_num;

    }

    return temp_num;
}

///******************************************************************************/
///*
// * \par  Description:在shuffle机制的设置指定文件FLAG
// *
// * \param[in]
//
// * \param[out]
//
// * \return     TRUE OR FALSE
//
// * \note
//*******************************************************************************/
//bool shuffle_set_flag(uint16 t_file_num)
//{
//    if(shuffle_total<file_total)
//    {
//        //文件个数变多了(增加文件)，重新初始化
//        init_shuffle(file_total);
//    }
//    else
//    {
//        //重新赋值防止 file_total<shuffle_total的情况(文件删除)
//        shuffle_total=file_total;
//    }
//
//    if(t_file_num>shuffle_total)
//    {
//        return FALSE;
//    }
//
//}


/******************************************************************************/
/*
 * \par  Description:获取shuffle机制的下一个
 *
 * \param[in]

 * \param[out]

 * \return

 * \note
 *******************************************************************************/
//shuffle mode  get next
//uint16 shuffle_next_file(void)
//{
//    if(shuffle_total<file_total)
//    {
//        //文件个数变多了(增加文件)，重新初始化
//        init_shuffle(file_total);
//    }
//    else
//    {
//        //重新赋值防止 file_total<shuffle_total的情况(文件删除)
//        shuffle_total=file_total;
//    }
//
//    if(shuffle_total==0)
//    {
//        return 0;
//    }
//
//    return shuffle_next_file_sub();
//
//}

/******************************************************************************/
/*
 * \par  Description:获取shuffle机制的上一个
 *
 * \param[in]

 * \param[out]

 * \return

 * \note
 *******************************************************************************/
//shuffle mode  get prev
//uint16 shuffle_prev_file(void)
//{
//    if(shuffle_total<file_total)
//    {
//        //文件个数变多了(增加文件)，重新初始化
//        init_shuffle(file_total);
//    }
//    else
//    {
//        //重新赋值防止 file_total<shuffle_total的情况(文件删除)
//        shuffle_total=file_total;
//    }
//
//    if(shuffle_total==0)
//    {
//        return 0;
//    }
//
//    return shuffle_prev_file_sub();
//}


