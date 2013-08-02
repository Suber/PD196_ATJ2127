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
 * \brief    收藏夹添加，删除和清空操作
 * \author   liminxian
 * \par      GENERAL DESCRIPTION:
 *               这里对文件进行描述
 * \par      EXTERNALIZED FUNCTIONS:
 *               这里描述调用到外面的模块
 * \version 1.0
 * \date  2011/9/15
 *******************************************************************************/

#include "eh_module.h"
#include "eh_rdata_ex.h"

#ifdef USE_83NAME_CREAT
static const char userpl_name[USERPL_MAX][12]=
{   "USERPL1 PL ","USERPL2 PL ","USERPL3 PL "};
#else
static const char userpl_name[USERPL_MAX][12] =
{ "USERPL1.PL", "USERPL2.PL", "USERPL3.PL" };
#endif

extern userpl_file_t *fav_opt_user_pl_p _BANK_DATA_ATTR_;
extern userpl_head_t fav_opt_user_pl_head _BANK_DATA_ATTR_;
extern uint16 fav_opt_existfile _BANK_DATA_ATTR_;
extern uint8 fav_obj_open_mode _BANK_DATA_ATTR_;

extern bool fav_opt_read_data(uint16 offset,uint8 *buf,uint8 w_flag,handle t_handle) __FAR__;
extern userpl_file_t *fav_opt_read_file(uint16 offset, uint8 *buf, uint8 w_flag, handle t_handle) __FAR__;

/******************************************************************************/
/*
 * \par  Description: 判断文件是否已在列表上
 *
 * \param[in]    location--文件的location
 buf-- buffer地址
 t_handle-- 句柄

 * \param[out]

 * \return       TRUE OR FALSE

 * \note
 *******************************************************************************/
bool fav_opt_judge_exist(flist_location_t *location, uint8 *buf, handle t_handle)
{
    //    uint32 t_file_offset;
    uint16 temp_total;
    uint16 temp_offset;
    uint16 temp;
    bool ret_val = FALSE;

    if (fav_opt_user_pl_head.file_num == 0)
    {
        return FALSE;
    }

    temp = 0;
    temp_offset = fav_opt_user_pl_head.first_offset;
    temp_total = fav_opt_user_pl_head.file_num;

    while (temp < temp_total)
    {
        fav_opt_user_pl_p = (userpl_file_t*) fav_opt_read_file(temp_offset, buf, 0, t_handle);

        if (0 == libc_memcmp(&location->cluster_no, &fav_opt_user_pl_p->location.cluster_no, 8))
        {
            fav_opt_existfile = temp_offset;
            ret_val = TRUE;
            break;
        }
        temp_offset = fav_opt_user_pl_p->next_offset;
        temp++;
    }

    return ret_val;

}

/******************************************************************************/
/*
 * \par  Description: 更新收藏夹头信息
 *
 * \param[in]    buf--使用的缓存buffer
 t_handle--句柄

 * \param[out]

 * \return       TRUE OR FALSE

 * \note
 *******************************************************************************/
bool fav_opt_update_head(uint8 *buf, handle t_handle)
{
    //回写最后一笔数据,修改扇区头
    fav_opt_read_data(0, buf, 1, t_handle);
    libc_memcpy(buf, &fav_opt_user_pl_head, sizeof(userpl_head_t));
    fav_opt_read_data(1, buf, 1, t_handle);
    return TRUE;
}

/******************************************************************************/
/*
 * \par  Description: 定位到选择的文件
 *
 * \param[in]    num-- 选中号 1~N
 buf-- buffer地址
 t_handle-- 句柄
 * \param[out]

 * \return       TRUE OR FALSE

 * \note        更新值 fav_opt_existfile
 *******************************************************************************/
bool fav_opt_set_pos(uint16 num, uint8 *buf, handle t_handle)
{
    uint16 i;
    uint16 temp_file_offset;
    userpl_file_t *tmp_user_pl;

    if (num > fav_opt_user_pl_head.file_num)
    {
        return FALSE;
    }
    temp_file_offset = fav_opt_user_pl_head.first_offset;
    i = 1;
    tmp_user_pl = fav_opt_read_file(temp_file_offset, buf, 0, t_handle);
    while (i != num)
    {
        if (tmp_user_pl->flag != 1)
        {
            return FALSE;
        }
        temp_file_offset = tmp_user_pl->next_offset;
        tmp_user_pl = fav_opt_read_file(temp_file_offset, buf, 0, t_handle);
        i++;
    }
    fav_opt_existfile = temp_file_offset;
    return TRUE;
}
/******************************************************************************/
/*
 * \par  Description: 从收藏夹中删除文件
 *
 * \param[in]
 del_file_offset--删除文件的offset
 buf-- buffer地址
 t_handle-- 句柄

 * \param[out]

 * \return       TRUE OR FALSE

 * \note
 *******************************************************************************/

bool fav_opt_delete_sub(uint16 del_file_offset, uint8 *buf, handle t_handle)
{
    uint16 temp_prev_offset;
    uint16 temp_next_offset;
    uint16 temp_del_offset;
    uint16 temp_last_next;

    if (fav_opt_user_pl_head.file_num == 0)
    {
        return FALSE;
    }

    fav_opt_user_pl_p = fav_opt_read_file(fav_opt_user_pl_head.last_offset, buf, 0, t_handle);
    temp_last_next = fav_opt_user_pl_p->next_offset;//先记录

    temp_del_offset = del_file_offset; //之前查找时已对fav_opt_existfile赋值

    fav_opt_user_pl_p = fav_opt_read_file(temp_del_offset, buf, 0, t_handle);
    fav_opt_user_pl_head.file_num--; //个数减一
    fav_opt_user_pl_p->flag = 2; //置删除标记

    if (temp_del_offset == fav_opt_user_pl_head.first_offset)
    {
        //删除的是第一个
        if (0 != fav_opt_user_pl_head.file_num)//
        {
            //不止一个文件,删除后插入有效的链表尾
            fav_opt_user_pl_head.first_offset = fav_opt_user_pl_p->next_offset;
            fav_opt_user_pl_p->next_offset = temp_last_next;
            fav_opt_user_pl_p->prev_offset = fav_opt_user_pl_head.last_offset;
        }
        else
        {
            temp_del_offset = 0xffff;
        }
    }
    else if (temp_del_offset == fav_opt_user_pl_head.last_offset)
    {
        //删除的是最后一个
        fav_opt_user_pl_head.last_offset = fav_opt_user_pl_p->prev_offset;
        temp_del_offset = 0xffff;
    }
    else
    {
        //删除中间的
        temp_prev_offset = fav_opt_user_pl_p->prev_offset;
        temp_next_offset = fav_opt_user_pl_p->next_offset;
        fav_opt_user_pl_p->next_offset = temp_last_next;

        //更新上一个指针
        fav_opt_user_pl_p = fav_opt_read_file(temp_prev_offset, buf, 1, t_handle);
        fav_opt_user_pl_p->next_offset = temp_next_offset;
        //更新下一个指针
        fav_opt_user_pl_p = fav_opt_read_file(temp_next_offset, buf, 1, t_handle);
        fav_opt_user_pl_p->prev_offset = temp_prev_offset;

    }

    //更新最后的文件的指向
    if (temp_del_offset != 0xffff)
    {
        fav_opt_user_pl_p = fav_opt_read_file(fav_opt_user_pl_head.last_offset, buf, 1, t_handle);
        fav_opt_user_pl_p->next_offset = temp_del_offset;

        fav_opt_user_pl_p = fav_opt_read_file(temp_last_next, buf, 1, t_handle);
        fav_opt_user_pl_p->prev_offset = temp_del_offset;
    }
    else
    {
        if (0 == fav_opt_user_pl_head.file_num)
        {
            fav_opt_user_pl_head.last_offset = fav_opt_user_pl_head.first_offset;
        }
    }

    //回写最后一笔数据,修改扇区头
    fav_opt_update_head(buf, t_handle);

    return TRUE;
}
