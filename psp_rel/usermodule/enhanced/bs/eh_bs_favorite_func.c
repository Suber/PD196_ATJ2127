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
 * \file     eh_bs_favorite_func.c
 * \brief    收藏夹浏览
 * \author   liminxian
 * \par      GENERAL DESCRIPTION:
 *               接口处理
 * \par      EXTERNALIZED FUNCTIONS:
 *               文件系统，VRAM
 * \version 1.0
 * \date  2011/9/15
 *******************************************************************************/

#include "eh_bs.h"

static userpl_file_t *bs_user_pl _BANK_DATA_ATTR_;

extern uint8 bs_temp_buf[SECTOR_SIZE];

extern bool bs_read_sector_data(uint16 sector_offset) __FAR__;
extern bool bs_update_last(void);
extern bool bs_plist_delete_func(brow_del_e type,uint16 list_no ,uint8 *file_dir_info) __FAR__;
extern bool bs_m3ufile_delete_func(brow_del_e type, uint16 list_no, uint8 *file_dir_info) __FAR__;
/******************************************************************************/
/*
 * \par  Description:读指定偏移号的文件信息
 *
 * \param[in]    t_file_offset--文件的位置偏移序号0~n

 * \param[out]

 * \return       文件信息的buffer指针

 * \note
 *******************************************************************************/
static userpl_file_t* bs_uspl_read_info(uint16 t_file_offset)
{
    uint16 sector_offset;
    uint16 buffer_offset;

    sector_offset = t_file_offset / USERPL_PRE_SEC + USERPL_HEAD_SEC;//add head sector
    buffer_offset = (t_file_offset % USERPL_PRE_SEC) * USERPL_FILE_SIZE;
    bs_read_sector_data(sector_offset);

    return (userpl_file_t*) (bs_temp_buf + buffer_offset);
}

/******************************************************************************/
/*
 * \par  Description: 定位到选择的文件
 *
 * \param[in]    num-- 选中号 1~N

 * \param[out]

 * \return       TRUE OR FALSE

 * \note        更新值 bs_uspl_cur 和 uspl_file_offset
 *******************************************************************************/
static bool bs_uspl_set_pos(uint16 num)
{
    uint16 i;
    //    uint16 temp;

    if (0 == bs_file_total)
    {
        return FALSE;
    }

    if (num > bs_file_total)
    {
        return FALSE;
    }

    save_read_offset = 0xffff;
    uspl_file_offset = uspl_first_offset;
    i = 1;
    bs_user_pl = bs_uspl_read_info(uspl_file_offset);
    while (i != num)
    {
        if (bs_user_pl->flag != 1)
        {
            return FALSE;
        }
        uspl_file_offset = bs_user_pl->next_offset;
        bs_user_pl = bs_uspl_read_info(uspl_file_offset);
        i++;
    }
    bs_uspl_cur = num;
    return TRUE;
}
/******************************************************************************/
/*
 * \par  Description: 判断文件是否已在列表上
 *
 * \param[in]    file_dir_info--文件的目录项位置信息

 * \param[out]

 * \return       TRUE OR FALSE

 * \note
 *******************************************************************************/
static bool bs_uspl_judge_exist(uint8 *file_dir_info)
{
    uint16 temp;
    bool ret_val = TRUE;

    if (0 == uspl_file_total)
    {
        return FALSE;
    }

    temp = 0;
    uspl_file_offset = uspl_first_offset;

    while (temp < uspl_file_total)
    {
        bs_user_pl = bs_uspl_read_info(uspl_file_offset); //读当前收藏夹中最新记录的扇区
        if (0 == libc_memcmp(file_dir_info, &bs_user_pl->location.cluster_no, 8))
        {
            bs_uspl_cur = temp + 1;
            break;
        }
        uspl_file_offset = bs_user_pl->next_offset;
        temp++;
    }

    if (temp == uspl_file_total)
    {
        ret_val = FALSE;
    }

    return ret_val;

}

/******************************************************************************/
/*
 * \par  Description: 把选中的文件从收藏夹中删除(公共接口)
 *
 * \param[in]        type--删除类型，(只支持删除单个文件)
 list_no--选中序号
 file_dir_info--文件的目录项位置信息(输入or输出)
 * \param[out]

 * \return           TRUE OR FALSE

 * \note            若list_no不为0，使用list_no为依据删除，file_dir_info 为输出
 若list_no为0，使用file_dir_info来删除文件，file_dir_info 为输入
 删除使用变量uspl_file_total
 *******************************************************************************/
bool bs_uspl_delete_func(brow_del_e type, uint16 list_no, uint8 *file_dir_info)
{
    uint16 temp_prev_offset;
    uint16 temp_next_offset;
    uint16 temp_del_offset;
    uint16 temp_last_next;
    userpl_head_t *upl_h;

    bool ret_val = TRUE;

    save_read_offset = 0xffff;

    if (0 == list_no)
    {
        if ((NULL != file_dir_info) && (FALSE != bs_uspl_judge_exist(file_dir_info)))
        {
            list_no = bs_uspl_cur;
        }
        else
        {
            fsel_error = FSEL_ERR_NO_EXIST;
            ret_val = FALSE;
        }
    }

    if (FALSE != ret_val)
    {
        bs_user_pl = bs_uspl_read_info(uspl_last_offset);
        temp_last_next = bs_user_pl->next_offset;//先记录
        bs_uspl_set_pos(list_no);
        bs_user_pl = bs_uspl_read_info(uspl_file_offset);
        uspl_file_total--;
        bs_user_pl->flag = 2;
        temp_del_offset = uspl_file_offset;

        if (NULL != file_dir_info)
        {
            libc_memcpy(file_dir_info, &bs_user_pl->location.cluster_no, 8);
        }
        write_data_flag = TRUE; //启动回写标志
        if (uspl_file_offset == uspl_first_offset)
        {
            //删除的是第一个
            if (0 != uspl_file_total)//bs_file_total
            {
                //不止一个文件,删除后插入有效的链表尾
                uspl_first_offset = bs_user_pl->next_offset;
                bs_user_pl->next_offset = temp_last_next;
                bs_user_pl->prev_offset = uspl_last_offset;
            }
            else
            {
                temp_del_offset = 0xffff;
            }
        }
        else if (uspl_file_offset == uspl_last_offset)
        {
            //删除的是最后一个
            uspl_last_offset = bs_user_pl->prev_offset;
            temp_del_offset = 0xffff;
        }
        else
        {
            //删除中间的
            temp_prev_offset = bs_user_pl->prev_offset;
            temp_next_offset = bs_user_pl->next_offset;
            bs_user_pl->next_offset = temp_last_next;

            //更新上一个指针
            bs_user_pl = bs_uspl_read_info(temp_prev_offset);
            bs_user_pl->next_offset = temp_next_offset;
            //更新下一个指针
            bs_user_pl = bs_uspl_read_info(temp_next_offset);
            bs_user_pl->prev_offset = temp_prev_offset;

        }

        //更新最后的文件的指向
        if (temp_del_offset != 0xffff)
        {
            bs_user_pl = bs_uspl_read_info(uspl_last_offset);
            bs_user_pl->next_offset = temp_del_offset;

            bs_user_pl = bs_uspl_read_info(temp_last_next);
            bs_user_pl->prev_offset = temp_del_offset;
        }
        else
        {
            if (0 == uspl_file_total)
            {
                uspl_last_offset = uspl_first_offset;
            }
        }

        //更新头信息
        bs_read_sector_data(0);
        upl_h = (userpl_head_t*) bs_temp_buf;

        upl_h->file_num = uspl_file_total;
        upl_h->first_offset = uspl_first_offset;
        upl_h->last_offset = uspl_last_offset;

        //回写
        bs_read_sector_data(1);

        //        vfs_file_seek(vfs_mount, 0, SEEK_SET, obj_handle);
        //        vfs_file_write(vfs_mount, bs_temp_buf, SECTOR_SIZE, obj_handle);

    }

    write_data_flag = FALSE;

    return ret_val;
}

/******************************************************************************/
/*
 * \par  Description: 把选中的文件删除
 *
 * \param[in]        type--删除类型，(只支持删除单个文件)
 list_no--选中序号
 location--文件的location
 * \param[out]

 * \return           TRUE OR FALSE

 * \note            若list_no不为0，使用list_no为依据删除
 若list_no为0，使用location来删除文件
 *******************************************************************************/
bool bs_uspl_delete(brow_del_e type, uint16 list_no, flist_location_t *location)
{
    bool ret_val;
    uint8 tmp_file_dir_info[8];

    //    return fsel_favorite_delete(FSEL_TYPE_LISTFAVOR1,list_no,location);

    if (0 == bs_open_object(OPEN_MODIFY))
    {
        return FALSE;
    }

    if (list_no != 0)
    {
        ret_val = bs_uspl_delete_func(FS_FILE, list_no, tmp_file_dir_info);
    }
    else
    {
        libc_memcpy(&tmp_file_dir_info, &location->cluster_no, 8);
        ret_val = bs_uspl_delete_func(FS_FILE, list_no, (uint8*) &location->cluster_no);
    }

    bs_close_object();

    if (FALSE != ret_val)
    {
        uint8 count, temp;

        bs_file_total--;

        //同步其他两个收藏夹
        count = 3;
        temp = FSEL_TYPE_LISTFAVOR1;
        while (count > 0)
        {
            count--;

            if (temp != fsel_init_val.fsel_type)
            {
                if (0 != bs_open_other(temp, OPEN_MODIFY))
                {
                    bs_uspl_delete_func(FS_FILE, 0, tmp_file_dir_info);
                    bs_close_object();
                }
            }
            temp++;
        }

        if (0 != bs_open_other(FSEL_TYPE_PLAYLIST, OPEN_MODIFY))
        {
            bs_plist_delete_func(FS_FILE, 0, tmp_file_dir_info);
            bs_close_object();
        }

        if (0 != bs_open_other(FSEL_TYPE_M3ULIST, OPEN_MODIFY))
        {
            bs_m3ufile_delete_func(FS_FILE, 0, tmp_file_dir_info);
            bs_close_object();
        }

        //更新磁盘删除，删除只用8字节就可以了
        vfs_file_dir_offset(vfs_mount, NULL, bs_temp_buf, 0);
        vfs_file_dir_offset(vfs_mount, NULL, &tmp_file_dir_info, 1);
        vfs_file_dir_remove(vfs_mount, NULL, FS_FILE);
        vfs_file_dir_offset(vfs_mount, NULL, bs_temp_buf, 1);
    }

    return ret_val;
}

/******************************************************************************/
/*
 * \par  Description: 定位到文件在磁盘的目录项位置
 *
 * \param[in]    location -- 文件的位置信息
 type --location类型（no used）

 * \param[out]

 * \return       TRUE OR FALSE

 * \note
 *******************************************************************************/
bool bs_uspl_set_file(flist_location_t*location, fsel_type_e type)
{
    type = type;
    return vfs_file_dir_offset(vfs_mount, &location->dir_layer_info, &location->cluster_no, 1);
}

