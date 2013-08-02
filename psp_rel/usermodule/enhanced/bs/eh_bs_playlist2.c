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
 * \file     eh_bs_playlist.c
 * \brief    播放列表浏览
 * \author   liminxian
 * \par      GENERAL DESCRIPTION:
 *               这里对文件进行描述
 * \par      EXTERNALIZED FUNCTIONS:
 *               文件系统
 * \version 1.0
 * \date  2011/9/15
 *******************************************************************************/

#include "eh_bs.h"

extern plist_tree_t plist_cur_tree;
extern uint8 bs_temp_buf[SECTOR_SIZE];

static const uint8 album_jpg_name[] = "Folder";
static const uint8 album_jpg_name2[] =
{ 0xff, 0xfe, 'F', 0x00, 'o', 0x00, 'l', 0x00, 'd', 0x00, 'e', 0x00, 'r', 0x00, 0x00, 0x00, };

extern bool bs_read_sector_data(uint16 sector_offset) __FAR__;
extern uint16 bs_get_file_index_byno(uint16 sector_start,uint16 index_offset,uint16 t_file_no) __FAR__; //切bank

/******************************************************************************/
/*
 * \par  Description:
 *
 * \param[in]    file_offset 文件的序号。0~N

 * \param[out]

 * \return       文件信息结构

 * \note
 *******************************************************************************/
//增加相同功能接口，避免切bank
static plist_f_info_t * bs_get_file_info2(uint16 file_offset)
{
    uint16 buf_offset;
    uint16 sector_offset; //第一个扇区为头

    sector_offset = plist_info_offset + file_offset / (SECTOR_SIZE / 256);
    buf_offset = (file_offset * 256) % SECTOR_SIZE;
    bs_read_sector_data(sector_offset);

    return (plist_f_info_t*) (bs_temp_buf + buf_offset);
}

/******************************************************************************/
/*
 * \par  Description:
 *
 * \param[in]   tree_start_offset--tree的起始扇区偏移 0~N
 tree_offset --tree 偏移，0~N

 * \param[out]

 * \return        tree信息结构指针

 * \note
 *******************************************************************************/
//增加相同功能接口，避免切bank
static plist_tree_t *bs_read_tree_data2(uint16 tree_start_offset, uint16 tree_offset)
{
    uint16 buf_offset;
    uint16 sector_offset;

    if (tree_offset != 0)
    {
        sector_offset = tree_start_offset + tree_offset / (SECTOR_SIZE / sizeof(plist_tree_t));
    }
    else
    {
        sector_offset = tree_start_offset;
    }

    buf_offset = sizeof(plist_tree_t) * (tree_offset % (SECTOR_SIZE / sizeof(plist_tree_t)));

    bs_read_sector_data(sector_offset);

    return (plist_tree_t *) (bs_temp_buf + buf_offset);
}

/******************************************************************************/
/*
 * \par  Description:
 *
 * \param[in]    sector_start--tree 's sector offset,
 tree_offset-- 0~N
 dir_num-- 1~N
 change_offset_flag--改变plist_layer_offset的值标记

 * \param[out]   tree's data

 * \return

 * \note
 *******************************************************************************/
//增加相同功能接口，避免切bank
static plist_tree_t* bs_get_tree_info_byno2(uint16 sector_start, uint16 tree_offset, uint16 dir_num,
        uint8 change_offset_flag)
{
    //    uint16 sector_offset;
    //    uint16 buf_offset;
    plist_tree_t *plist_tree_p;

    plist_tree_p = 0;
    //    while(dir_num--)
    for (; dir_num > 0; dir_num--)
    {
        while (1)
        {
            plist_tree_p = bs_read_tree_data2(sector_start, tree_offset);
            if (plist_tree_p->flag == 1)
            {
                break;
            }
            tree_offset++;
        }
        tree_offset++;
    }

    if (change_offset_flag == 1)
    {
        plist_layer_offset = tree_offset - 1;//记录读出的tree的偏移
    }
    return plist_tree_p;

}

/******************************************************************************/
/*
 * \par  Description: 根据file_index获取album 信息
 *
 * \param[in]    album_buf--信息buf
 t_file_index--文件序号

 * \param[out]

 * \return       TRUE OR FALSE

 * \note
 *******************************************************************************/
static void bs_get_album_info(album_brow_t *album_buf, uint16 t_file_index)
{
    uint8 result = 1;
    uint16 str_len;
    plist_f_info_t *plist_f_p;

    plist_f_p = bs_get_file_info2(t_file_index);

    //get album 's name
    str_len = album_buf->name_len - 2;
    if (str_len > ID3_ITEM_SIZE)
    {
        str_len = ID3_ITEM_SIZE;
    }

    libc_memcpy(album_buf->name_buf, plist_f_p->album_string, (uint32) str_len);
    album_buf->name_buf[str_len] = 0x00;
    album_buf->name_buf[str_len + 1] = 0x00;

    //find album jpg
    album_buf->album_flag = 0;
    if (0 != vfs_file_dir_offset(vfs_mount, &plist_f_p->dir_layer_info, &plist_f_p->cluster_no, 1))
    {
        if (0 == vfs_dir(vfs_mount, DIR_HEAD, album_jpg_name, EXT_NAME_JPG))
        {
            //try uncoide name
            if (0 == vfs_dir(vfs_mount, DIR_HEAD, album_jpg_name2, EXT_NAME_JPG))
            {
                result = 0;
            }
        }

        if (result == 1)
        {
            //保持专辑图片位置
            vfs_file_dir_offset(vfs_mount, NULL, &album_buf->cluster_no, 0);
            album_buf->album_flag = 1;//set album jpg flag
        }
    }

}

/******************************************************************************/
/*
 * \par  Description: browser获取显示列表信息
 *
 * \param[in]    decode_param--显示参数
 album_list--显示buf

 * \param[out]

 * \return

 * \note
 *******************************************************************************/
bool bs_plist_get_albums(brow_decode_t *decode_param, void *album_list)
{
    album_brow_t *album_items;
    plist_tree_t *plist_tree_p;
    bool ret_val = TRUE;
    uint8 count;
    uint8 out_flag;//信息buf，填0标志
    uint16 i;

    save_read_offset = 0xffff;
    out_flag = 0;

    if (0 == bs_open_object(R_NORMAL_SEEK))
    {
        return FALSE;
    }

    album_items = (album_brow_t*) album_list;
    count = decode_param->num;
    i = decode_param->top; //最少值为1

    if (i <= bs_dir_total)
    {
        for (; count > 0; count--)
        {
            if (out_flag == 1)
            {
                album_items->name_buf[0] = 0x00;
                album_items->album_flag = 0x00;
            }
            else
            {
                plist_tree_p = bs_get_tree_info_byno2(plist_item_info.son_tree_offset[dir_layer],
                        plist_cur_tree.son_offset, i, 0);
                if (NULL != plist_tree_p)
                {
                    bs_get_album_info(album_items, plist_tree_p->file_index);

                    if ((i + 1) > bs_dir_total)
                    {
                        out_flag = 1;
                    }
                }
            }
            i++;
            album_items++;
        }
    }
    else
    {
        ret_val = FALSE;//无文件和文件夹
    }

    bs_close_object();
    return ret_val;
}

/******************************************************************************
 M3U 列表删除处理接口
 ******************************************************************************/

/******************************************************************************/
/*
 * \par  Description: 判断文件是否已在列表上
 *
 * \param[in]    file_dir_info--文件的目录项位置信息

 * \param[out]

 * \return       TRUE OR FALSE

 * \note        找到赋值del_file_index
 *******************************************************************************/
static bool bs_m3u_judge_exist(uint8 *file_dir_info, uint16 *del_index_p)
{
    uint16 temp, temp_index;
    uint16 sector_offset, t_file_total;
    //    plist_f_info_t *plist_f_info_p;
    uint8 *file_dir_pos;
    bool ret_val = TRUE;

    //查表，从表中读取文件总数t_file_total，应为LIB中总的文件个数
    bs_read_sector_data(0);
    t_file_total = *(uint16*) (bs_temp_buf + M3U_DAT_HEADOFFSET);
    *del_index_p = PLIST_INVALID_INDEX;

    if (0 == t_file_total)
    {
        return FALSE;
    }
    temp = 0;
    temp_index = 0;

    //temp 有效个数
    //temp_index file_index
    sector_offset = M3U_DAT_POS_INDEXOFFSET / SECTOR_SIZE;
    while ((temp < t_file_total) && (temp_index < M3U_DAT_FILE_MAX))
    {
        if ((temp_index % SECTOR_SIZE) == 0)
        {
            bs_read_sector_data(sector_offset);
            sector_offset++;
            file_dir_pos = (uint8*) bs_temp_buf;
        }

        if (*file_dir_pos != 0xff)// 0xff 为已删除标记
        {
            if (0 == libc_memcmp(file_dir_info, file_dir_pos, 8))
            {
                *del_index_p = temp_index;
                break;
            }
            else
            {
                temp++;
            }
        }
        temp_index++;
        file_dir_pos = file_dir_pos + 8;
    }

    if ((temp == t_file_total) || (temp_index == M3U_DAT_FILE_MAX))
    {

        ret_val = FALSE;
    }

    return ret_val;
}

/******************************************************************************/
/*
 * \par  Description: 根据file_index删除列表中某个文件
 *
 * \param[in]    t_file_index (0~N)

 * \param[out]

 * \return       TRUE OR FALSE

 * \note
 *******************************************************************************/
//删除列表中的一个文件，更新列表
static bool bs_m3u_del_by_index(uint16 t_file_index)
{
    uint8 t_m3ulist_index[M3U_LIST_MAX];
    uint16 i, j, m, n, t_total;
    uint16 *index_p;
    m3ulist_f_info_t *m3ulist_f_p;
    m3udat_head_t *m3udat_head_p;

    i = (M3U_DAT_POS_INDEXOFFSET / SECTOR_SIZE) + (t_file_index * 8) / SECTOR_SIZE;
    bs_read_sector_data(i);
    bs_temp_buf[(t_file_index * 8) % SECTOR_SIZE] = 0xff; //del file_pos

    write_data_flag = TRUE; //启动回写入

    m3ulist_f_p = (m3ulist_f_info_t*) bs_get_file_info2(t_file_index);
    m3ulist_f_p->flag = 2; //del file_info
    libc_memcpy(&t_m3ulist_index, m3ulist_f_p->m3u_index, M3U_LIST_MAX);

    bs_read_sector_data(0);
    m3udat_head_p = (m3udat_head_t*) (bs_temp_buf + M3U_DAT_HEADOFFSET);
    m3udat_head_p->dat_file_total--;

    //mdy dat'head	,更新头信息中列表的个数
    for (i = 0; i < M3U_LIST_MAX; i++)
    {
        if (t_m3ulist_index[i] != 0xff)
        {
            if ((t_m3ulist_index[i] & 0x80) != 0)
            {
                //文件是audible标记
                t_m3ulist_index[i] = t_m3ulist_index[i] & 0x7f;//删除标记
                m3udat_head_p->audible_file_num[t_m3ulist_index[i]]--;

            }
            m3udat_head_p->list_file_num[t_m3ulist_index[i]]--;
        }
        else
        {
            break;
        }
    }
    //del file_index ，更新file_index队列
    for (i = 0; i < M3U_LIST_MAX; i++)
    {
        m = 0;
        if (t_m3ulist_index[i] != 0xff)
        {
            bs_read_sector_data(0);
            t_total = m3udat_head_p->list_file_num[t_m3ulist_index[i]];
            if (t_total != 0)
            {
                m = M3U_DAT_INDEXOFFSET / SECTOR_SIZE + t_m3ulist_index[i] * (M3U_DAT_INDEXSIZE / SECTOR_SIZE);
            }
        }
        else
        {
            break;
        }

        if (m != 0)
        {
            for (j = 0, n = 0; j < t_total + 1;)
            {
                if (n % (SECTOR_SIZE / 2) == 0)
                {
                    bs_read_sector_data(m);
                    index_p = (uint16*) bs_temp_buf;
                    m++;
                }

                if (*index_p != 0xffff)//判断file_index 是否有效
                {
                    if ((*index_p & 0x1fff) == t_file_index)
                    {
                        *index_p = 0xffff;
                        break;//for(;;)
                    }
                    j++;
                }

                index_p++;
                n++;
                if (n == M3U_DAT_FILE_MAX)//异常处理
                {
                    break;
                }
            }
        }
    }

    bs_read_sector_data(1);//SAVE LAST'SECTOR DATA
    write_data_flag = FALSE;
    return TRUE;
}

/******************************************************************************/
/*
 * \par  Description: 把选中的文件从列表中删除(公共接口)
 *
 * \param[in]        type--删除类型，(只支持删除单个文件)
 list_no--选中序号(1~N)
 file_dir_info--文件的目录项位置信息(输入or输出)
 * \param[out]

 * \return           TRUE OR FALSE

 * \note            若list_no不为0，使用list_no为依据删除，file_dir_info 为输出
 若list_no为0，使用file_dir_info来删除文件，file_dir_info 为输入
 删除使用变量plist_file_total
 *******************************************************************************/
bool bs_m3ufile_delete_func(brow_del_e type, uint16 list_no, uint8 *file_dir_info)
{
    bool ret_val = TRUE;
    uint16 t_del_file_index;

    save_read_offset = 0xffff;

    if (0 != list_no)
    {
        plist_f_info_t * tem_plist_f_p;
        //减少代码空间做成要切bank
        t_del_file_index = bs_get_file_index_byno(plist_item_info.file_index_offset, 0, list_no);
        if (NULL != file_dir_info)
        {
            tem_plist_f_p = bs_get_file_info2(t_del_file_index);
            libc_memcpy(file_dir_info, &tem_plist_f_p->cluster_no, 8);
        }
    }
    else
    {
        if (NULL == file_dir_info)
        {
            ret_val = FALSE;
        }
        else
        {
            //找到会赋值 t_del_file_index
            ret_val = bs_m3u_judge_exist(file_dir_info, &t_del_file_index);
        }

    }

    //ADD CODE
    if (FALSE != ret_val)
    {
        ret_val = bs_m3u_del_by_index(t_del_file_index);
    }

    return ret_val;
}

