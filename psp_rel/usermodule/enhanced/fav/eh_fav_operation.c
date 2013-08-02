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
 *               FS
 * \version 1.0
 * \date  2011/9/15
 *******************************************************************************/

#include "eh_module.h"
#include "eh_rdata_ex.h"

//#ifdef USE_83NAME_CREAT
//static const char userpl_name[USERPL_MAX][12]=
//{   "USERPL1 PL ","USERPL2 PL ","USERPL3 PL "};
//#else
//static const char userpl_name[USERPL_MAX][12] =
//{ "USERPL1.PL", "USERPL2.PL", "USERPL3.PL" };
//#endif

static uint8 fav_opt_temp_buf[SECTOR_SIZE] _BANK_DATA_ATTR_;
static uint16 fav_opt_read_record _BANK_DATA_ATTR_;
//static uint32 fav_opt_file_size _BANK_DATA_ATTR_;
//static uint8 fav_opt_write_add_flag _BANK_DATA_ATTR_;

userpl_file_t *fav_opt_user_pl_p _BANK_DATA_ATTR_;
userpl_head_t fav_opt_user_pl_head _BANK_DATA_ATTR_;
uint16 fav_opt_existfile _BANK_DATA_ATTR_;
uint8 fav_obj_open_mode _BANK_DATA_ATTR_;

extern handle fav_opt_open_userpl(uint8 sn,uint8 *buf,uint8 creat_flag) __FAR__;
extern bool fav_opt_judge_exist(flist_location_t *location,uint8 *buf,handle t_handle) __FAR__;
extern bool fav_opt_update_head(uint8 *buf,handle t_handle) __FAR__;
extern bool fav_opt_delete_sub(uint16 del_file_offset,uint8 *buf,handle t_handle) __FAR__;
//static bool fav_update_last(void);
extern bool fav_opt_set_pos(uint16 num, uint8 *buf, handle t_handle);

/******************************************************************************/
/*
 * \par  Description:关闭文件
 *
 * \param[in]
 * \param[out]
 * \return

 * \note
 *******************************************************************************/
void fav_opt_close_userpl(handle t_handle)
{
    if (0xee != fav_obj_open_mode)
    {
        vfs_file_close(vfs_mount, t_handle);
    }

}

/******************************************************************************/
/*
 * \par  Description: 读收藏夹中指定扇区位置数据
 *
 * \param[in]    offset--扇区偏移
 buf-- 缓存buffer
 w_flag--回写数据标记
 t_handle--文件句柄

 * \param[out]

 * \return       TRUE OR FALSE

 * \note
 *******************************************************************************/
//读收藏夹中指定文件所在的扇区
bool fav_opt_read_data(uint16 offset, uint8 *buf, uint8 w_flag, handle t_handle)
{
    if (offset != fav_opt_read_record)
    {
        if (w_flag != 0)
        {
            if (0xee == fav_obj_open_mode)
            {
                //回写VRAM，obj_handle存放VM地址
                sys_vm_write(buf, t_handle + fav_opt_read_record * SECTOR_SIZE);
            }
            else
            {
                vfs_file_seek(vfs_mount, 0 - SECTOR_SIZE, SEEK_CUR, t_handle);
                vfs_file_write(vfs_mount, buf, SECTOR_SIZE, t_handle);
            }

        }

        if (0xee == fav_obj_open_mode)
        {
            sys_vm_read(buf, t_handle + offset * SECTOR_SIZE, SECTOR_SIZE);
        }
        else
        {
            vfs_file_seek(vfs_mount, offset * SECTOR_SIZE, SEEK_SET, t_handle);
            vfs_file_read(vfs_mount, buf, SECTOR_SIZE, t_handle);
        }

        fav_opt_read_record = offset;
    }

    return TRUE;

}

/******************************************************************************/
/*
 * \par  Description: 读收藏夹中指定文件的信息
 *
 * \param[in]    offset--文件偏移
 buf-- 缓存buffer
 w_flag--回写数据标记
 t_handle--文件句柄

 * \param[out]

 * \return       userpl_file_t

 * \note
 *******************************************************************************/
userpl_file_t *fav_opt_read_file(uint16 offset, uint8 *buf, uint8 w_flag, handle t_handle)
{
    uint16 temp_offset;

    temp_offset = offset / USERPL_PRE_SEC + USERPL_HEAD_SEC;//add head's sectors
    fav_opt_read_data(temp_offset, buf, w_flag, t_handle);
    return (userpl_file_t*) (buf + (offset % USERPL_PRE_SEC) * USERPL_FILE_SIZE);
}

/******************************************************************************/
/*
 * \par  Description:添加文件到收藏夹
 *
 * \param[in]    location--文件的location操作
 target_fsel_type--目标收藏夹

 * \param[out]

 * \return       TRUE OR FALSE

 * \note
 *******************************************************************************/
bool fsel_favorite_add(flist_location_t *location, fsel_type_e target_fsel_type)
{
    uint16 temp_prev;
    uint16 temp_next;
    handle t_handle;
    bool err_flag;

    if (location == NULL)
    {
        return FALSE;
    }

    err_flag = TRUE;
    fav_opt_read_record = 0xffff;

    //目的判断
    if ((target_fsel_type >= FSEL_TYPE_LISTFAVOR1) && (target_fsel_type <= FSEL_TYPE_LISTFAVOR3))
    {
        target_fsel_type = target_fsel_type - FSEL_TYPE_LISTFAVOR1;
    }
    else
    {
        fsel_error = FSEL_ERR_INPUT;
        return FALSE;
    }

    t_handle = fav_opt_open_userpl(target_fsel_type, fav_opt_temp_buf, 1);

    if (t_handle == 0)
    {
        fsel_error = FSEL_ERR_FAIL;
        return FALSE;
    }

    if (FALSE != fav_opt_judge_exist(location, fav_opt_temp_buf, t_handle))
    {
        //文件已在收藏夹上
        fav_opt_close_userpl(t_handle);
        fsel_error = FSEL_ERR_EXIST;
        return FALSE;
    }

    fav_opt_user_pl_p = (userpl_file_t *) fav_opt_temp_buf;

    if (fav_opt_user_pl_head.file_num < USERPL_FILE_MAX)
    {
        //收藏夹未满，在最后的文件追加
        fav_opt_user_pl_head.file_num++;
        if (1 == fav_opt_user_pl_head.file_num)
        {
            //原收藏夹为空，从头加入
            temp_next = fav_opt_user_pl_head.first_offset;
            temp_prev = temp_next;
        }
        else
        {
            fav_opt_user_pl_p = (userpl_file_t *) fav_opt_read_file(fav_opt_user_pl_head.last_offset, fav_opt_temp_buf,
                    0, t_handle);
            temp_next = fav_opt_user_pl_p->next_offset;
            temp_prev = fav_opt_user_pl_head.last_offset;
        }

        //添加item
        //读当前收藏夹中最新记录
        fav_opt_user_pl_p = (userpl_file_t *) fav_opt_read_file(temp_next, fav_opt_temp_buf, 0, t_handle);

        libc_memcpy(&fav_opt_user_pl_p->location, location, sizeof(flist_location_t));
        fav_opt_user_pl_p->prev_offset = temp_prev;
        fav_opt_user_pl_head.last_offset = temp_next;

        if (fav_opt_user_pl_p->flag == 0) //,若为2则表示已删除了
        {
            //表示之前未用过
            temp_next++;
            fav_opt_user_pl_p->next_offset = temp_next;
        }

        fav_opt_user_pl_p->flag = 1;

        //更新cover_offset
        fav_opt_user_pl_head.cover_offset = fav_opt_user_pl_head.first_offset;

    }
    else
    {
        //收藏夹已满，直接覆盖
        fav_opt_user_pl_p = (userpl_file_t *) fav_opt_read_file(fav_opt_user_pl_head.cover_offset, fav_opt_temp_buf, 0,
                t_handle);
        fav_opt_user_pl_head.cover_offset = fav_opt_user_pl_p->next_offset;
        if (fav_opt_user_pl_head.cover_offset >= USERPL_FILE_MAX)
        {
            fav_opt_user_pl_head.cover_offset = fav_opt_user_pl_head.first_offset;
        }
        libc_memcpy(&fav_opt_user_pl_p->location, location, sizeof(flist_location_t));
    }

    //回写最后一笔数据,修改扇区头
    fav_opt_update_head(fav_opt_temp_buf, t_handle);
    fav_opt_close_userpl(t_handle);

    return err_flag;
}

/******************************************************************************/
/*
 * \par  Description: 清空收藏夹操作
 *
 * \param[in]    target_fsel_type--操作的目标收藏夹

 * \param[out]

 * \return       TRUE OR FALSE

 * \note
 *******************************************************************************/
bool fsel_favorite_clear(fsel_type_e target_fsel_type)
{
    uint16 temp;
    uint16 temp_total;
    uint16 temp_offset;
    //    userpl_head_t *upl_h;
    handle t_handle;
    fav_opt_read_record = 0xffff;

    //浏览场景中才会有清空功能。

    if ((target_fsel_type >= FSEL_TYPE_LISTFAVOR1) && (target_fsel_type <= FSEL_TYPE_LISTFAVOR3))
    {
        target_fsel_type = target_fsel_type - FSEL_TYPE_LISTFAVOR1;
    }
    else
    {
        fsel_error = FSEL_ERR_INPUT;
        return FALSE;
    }

    t_handle = fav_opt_open_userpl(target_fsel_type, fav_opt_temp_buf, 0);

    if (0 == t_handle)
    {
        fsel_error = FSEL_ERR_FAIL;
        return FALSE;
    }

    if (0 == fav_opt_user_pl_head.file_num)
    {
        fav_opt_close_userpl(t_handle);
        return FALSE;
    }

    temp_total = fav_opt_user_pl_head.file_num;
    fav_opt_user_pl_head.file_num = 0;
    temp_offset = fav_opt_user_pl_head.first_offset;
    temp = 0;
    fav_opt_user_pl_head.first_offset = 0;
    fav_opt_user_pl_head.last_offset = 0;
    fav_opt_user_pl_head.cover_offset = 0;

    while (temp < temp_total)
    {
        fav_opt_user_pl_p = (userpl_file_t*) fav_opt_read_file(temp_offset, fav_opt_temp_buf, 1, t_handle);

        temp_offset = fav_opt_user_pl_p->next_offset;
        fav_opt_user_pl_p->flag = 2; //置删除标记
        temp++;
    }

    //回写最后一笔数据,修改扇区头
    fav_opt_update_head(fav_opt_temp_buf, t_handle);
    fav_opt_close_userpl(t_handle);

    return TRUE;

}

/******************************************************************************/
/*
 * \par  Description: 把文件从收藏夹中删除
 *
 * \param[in]    target_fsel_type--操作的目标收藏夹
 list_no--选中序号
 location--文件的location
 * \param[out]

 * \return           TRUE OR FALSE

 * \note            若list_no不为0，使用list_no为依据删除
 若list_no为0，使用location来删除文件
 *******************************************************************************/
bool fsel_favorite_delete(fsel_type_e target_fsel_type, uint16 list_no, flist_location_t *location)
{
    bool ret_val = TRUE;
    handle t_handle;
    fav_opt_read_record = 0xffff;
    //目的判断

    if ((target_fsel_type >= FSEL_TYPE_LISTFAVOR1) && (target_fsel_type <= FSEL_TYPE_LISTFAVOR3))
    {
        target_fsel_type = target_fsel_type - FSEL_TYPE_LISTFAVOR1;
    }
    else
    {
        fsel_error = FSEL_ERR_INPUT;
        return FALSE;
    }

    t_handle = fav_opt_open_userpl(target_fsel_type, fav_opt_temp_buf, 1);

    if (t_handle == 0)
    {
        fsel_error = FSEL_ERR_FAIL;
        return FALSE;
    }

    if (list_no != 0)
    {
        ret_val = fav_opt_set_pos(list_no, fav_opt_temp_buf, t_handle);
    }
    else
    {
        if (NULL == location)
        {
            ret_val = FALSE;
        }
        else
        {
            ret_val = fav_opt_judge_exist(location, fav_opt_temp_buf, t_handle);
        }
    }

    if (FALSE != ret_val)
    {
        ret_val = fav_opt_delete_sub(fav_opt_existfile, fav_opt_temp_buf, t_handle);
    }
    else
    {
        //文件不在收藏夹上
        fsel_error = FSEL_ERR_NO_EXIST;
    }

    fav_opt_close_userpl(t_handle);
    return ret_val;

}

/******************************************************************************/
/*
 * \par  Description: 获取收藏夹总数
 *
 * \param[in]    target_fsel_type--操作的目标收藏夹

 * \param[out]

 * \return       收藏夹文件总数

 * \note
 *******************************************************************************/
uint16 fsel_favorite_get_total(fsel_type_e target_fsel_type)
{
    handle t_handle;
    //目的判断
    if ((target_fsel_type >= FSEL_TYPE_LISTFAVOR1) && (target_fsel_type <= FSEL_TYPE_LISTFAVOR3))
    {
        target_fsel_type = target_fsel_type - FSEL_TYPE_LISTFAVOR1;
    }
    else
    {
        return 0;
    }

    t_handle = fav_opt_open_userpl(target_fsel_type, fav_opt_temp_buf, 1);
    fav_opt_close_userpl(t_handle);

    return fav_opt_user_pl_head.file_num;
}

