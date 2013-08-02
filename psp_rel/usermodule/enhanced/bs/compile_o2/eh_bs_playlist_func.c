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
 * \file     eh_bs_playlist_func.c
 * \brief    播放列表浏览
 * \author   liminxian
 * \par      GENERAL DESCRIPTION:
 *               这里对文件进行描述
 * \par      EXTERNALIZED FUNCTIONS:
 *               文件系统，VRAM
 * \version 1.0
 * \date  2011/9/15
 *******************************************************************************/

//#include "eh_bs.h"
#include "../eh_bs.h"

extern plist_tree_t plist_cur_tree;
//extern uint16 save_read_offset; //保存最后读地址
//extern uint8   write_data_flag;   //回写数据标记
extern uint8 bs_temp_buf[SECTOR_SIZE];

static bool bs_get_total(void);
static bool bs_plist_enter_son(uint16 dir_num);
static bool bs_plist_enter_parent(void);
//static plist_tree_t* bs_get_tree_info(uint16 sector_start,uint16 tree_offset);
static bool bs_set_into_layer(uint8 list_layer, uint16 list_layer_offset);

extern plist_tree_t* bs_get_tree_info_byno(uint16 sector_start, uint16 tree_offset, uint16 dir_num,
        uint8 change_offset_flag);
extern bool bs_get_display_info(uint8*name_buf, uint8 name_len, uint8 type, uint16 t_file_index);
extern plist_tree_t *bs_read_tree_data(uint16 tree_start_offset, uint16 tree_offset);

//extern uint16 bs_get_object_index(uint16 index_start_offset,uint16 index);
extern uint16 bs_get_file_index_byno(uint16 sector_start, uint16 index_offset, uint16 t_file_no);
//extern plist_f_info_t * bs_get_file_info(uint16 file_offset);
//extern bool bs_get_item_info(plist_item_info_t *item_info,plist_type_e type);

extern bool bs_uspl_delete_func(brow_del_e type,uint16 list_no ,uint8 *file_dir_info) __FAR__;
extern bool bs_plist_delete_func(brow_del_e type,uint16 list_no ,uint8 *file_dir_info) __FAR__;
extern bool bs_m3ufile_delete_func(brow_del_e type, uint16 list_no, uint8 *file_dir_info) __FAR__;

extern bool bs_read_sector_data(uint16 sector_offset) __FAR__;
extern bool bs_del_ebookbmk(void) __FAR__;

void bs_m3u_get_total(uint16 list_no);

/******************************************************************************/
/*
 * \par  Description: 获取指定类型的ITEM信息
 *
 * \param[in]     item_info --存放ITEM信息
 type--类型

 * \param[out]

 * \return        NULL

 * \note
 *******************************************************************************/
bool bs_get_item_info(plist_item_info_t *item_info, plist_type_e type)
{
    plist_item_t *plist_item_p;

    bs_read_sector_data(0);
    plist_item_p = (plist_item_t *) (bs_temp_buf + sizeof(plist_head_t));
    plist_item_p += ((uint8) type & 0x0f);
    libc_memcpy(item_info, &plist_item_p->son_tree_num, sizeof(plist_item_info_t));
    return TRUE;

}

/******************************************************************************/
/*
 * \par  Description: 进入子表
 *
 * \param[in]    选择的list number 1～N

 * \param[out]

 * \return       TRUE OR FALSE

 * \note
 *******************************************************************************/
static bool bs_plist_enter_son(uint16 dir_num)
{
    plist_tree_t *plist_tree_p;

    //M3U  特殊处理
    if (fsel_init_val.fsel_type != FSEL_TYPE_M3ULIST)
    {
        if ((dir_num == 0) || (bs_dir_total == 0))
        {
            //只是文件，没有列表
            return FALSE;
        }
    }

    if (dir_num > (bs_dir_total + bs_file_total))
    {
        return FALSE;
    }

    dir_layer++;
    //M3U  特殊处理
    if (fsel_init_val.fsel_type == FSEL_TYPE_M3ULIST)
    {
        if (dir_layer == 1)
        {
            m3u_curlist_index = bs_get_file_index_byno(plist_item_info.file_index_offset, 0, dir_num);
            if (m3u_curlist_index != 0xffff)
            {
                //m3u 子表：重新打开文件，刷新数据
                bs_close_object();
                bs_open_object(R_NORMAL_SEEK);
            }
            else
            {
                return FALSE;
            }
        }
        else
        {
            return FALSE;
        }
    }
    else
    {
        plist_tree_p = bs_get_tree_info_byno(plist_item_info.son_tree_offset[dir_layer - 1], plist_cur_tree.son_offset,
                dir_num, 1);
        libc_memcpy(&plist_cur_tree, plist_tree_p, sizeof(plist_tree_t));
        bs_get_total();
    }

    return TRUE;
}

/******************************************************************************/
/*
 * \par  Description: 退到父级
 *
 * \param[in]    NULL

 * \param[out]

 * \return       TRUE OR FALSE

 * \note
 *******************************************************************************/
static bool bs_plist_enter_parent(void)
{
    plist_tree_t *plist_tree_p;

    if (dir_layer == 0)
    {
        return FALSE;
    }

    dir_layer--;
    if (dir_layer == 0)
    {
        plist_layer_offset = 0;
        libc_memset(&plist_cur_tree, 0, sizeof(plist_tree_t));
        if (m3u_curlist_index != 0xffff)
        {
            m3u_curlist_index = 0xffff;
            //重新打开文件，获取M3U列表文件总数
            bs_close_object();
            bs_open_object(R_NORMAL_SEEK);
        }
        bs_get_total();
        return TRUE;
    }

    if (plist_cur_tree.parent_offset != 0xffff)
    {
        plist_layer_offset = plist_cur_tree.parent_offset;
        plist_tree_p = bs_read_tree_data(plist_item_info.son_tree_offset[dir_layer - 1], plist_cur_tree.parent_offset);
        libc_memcpy(&plist_cur_tree, plist_tree_p, sizeof(plist_tree_t));
        bs_get_total();
        return TRUE;
    }
    else
    {
        return FALSE;
    }

}

/******************************************************************************/
/*
 * \par  Description:操作列表
 *
 * \param[in]    type--操作类型（进入 子级 or 父级 or 当前级）
 list_no--操作 list number 1～N

 * \param[out]   dir_brow   --文件夹参数

 * \return       TRUE OR FALSE

 * \note
 *******************************************************************************/
bool bs_plist_enter(brow_dir_e type, uint16 list_no, dir_brow_t *dir_brow)
{
    bool ret_val = FALSE;

    save_read_offset = 0xffff;

    if (type == ROOT_DIR)
    {
        //处理M3U列表显示的情况
        m3u_curlist_index = 0xffff;//在OPEN前赋值
        dir_layer = 0;
    }

    if (0 == bs_open_object(R_NORMAL_SEEK))
    {
        return FALSE;
    }

    switch (type)
    {
        //重新解析当前目录
        case CUR_DIR:
        ret_val = bs_get_total();
        break;

        //进入子目录
        case SON_DIR:
        ret_val = bs_plist_enter_son(list_no);
        break;

        //返回父目录
        case PARENT_DIR:
        ret_val = bs_plist_enter_parent();
        m3u_curlist_index = 0xffff;
        break;

        //进入根目录
        case ROOT_DIR:

        //        m3u_curlist_index = 0xffff;
        //        if(fsel_init_val.fsel_type == FSEL_TYPE_M3ULIST)
        //        {
        //            //重新打开文件,读出列表项数据
        //            bs_close_object();
        //            bs_open_object(R_NORMAL_SEEK);
        //        }
        bs_get_item_info(&plist_item_info, plist_type_selector);
        dir_layer = 0;
        bs_select_listno = 0;
        //    plist_list_layerno[0]=0;
        plist_layer_offset = 0;
        plist_start_sort_index = 0;
        libc_memset(&plist_cur_tree, 0x00, sizeof(plist_tree_t));

        bs_dir_total = plist_item_info.son_tree_num[0];

        if (0 == bs_dir_total)
        {
            bs_file_total = plist_file_total;//title
        }
        else
        {
            bs_file_total = 0;
        }
        ret_val = TRUE;
        //添加代码
        break;

        default:
        ret_val = FALSE;
        break;

    }

    if (dir_brow != NULL)
    {

        dir_brow->dir_total = bs_dir_total;
        dir_brow->file_total = bs_file_total;
        dir_brow->layer_no = dir_layer;

        if ((FALSE != ret_val) && (dir_brow->name_len > 0))
        {
            if ((0x00 == dir_layer) || (m3u_curlist_index != 0xffff))
            {
                dir_brow->name_buf[0] = 0x00;
            }
            else
            {
                bs_get_display_info(dir_brow->name_buf, dir_brow->name_len, plist_cur_tree.type,
                        plist_cur_tree.file_index);
            }
        }

        //M3U  特殊处理
        //,dir_layer =0,时bs_file_total 作为 bs_dir_total 值外送
        if (fsel_init_val.fsel_type == FSEL_TYPE_M3ULIST)
        {
            if (0 == dir_layer)
            {
                dir_brow->dir_total = dir_brow ->file_total;
                dir_brow ->file_total = 0;
            }
        }
    }

    bs_select_listno = 0;

    bs_close_object();

    return ret_val;
}

/******************************************************************************/
/*
 * \par  Description:获取浏览文件的总数
 *
 * \param[in]    NULL

 * \param[out]

 * \return      TRUE OR FALSE

 * \note
 *******************************************************************************/
static bool bs_get_total(void)
{
    //M3U  特殊处理
    if (fsel_init_val.fsel_type == FSEL_TYPE_M3ULIST)
    {
        if (dir_layer == 1)
        {
            return TRUE;
        }
    }

    if (0 == dir_layer)
    {
        bs_dir_total = plist_item_info.son_tree_num[0];
    }
    else
    {
        bs_dir_total = plist_cur_tree.son_num;
    }

    if (bs_dir_total == 0)
    {
        if (plist_cur_tree.flag == 1)
        {
            bs_file_total = plist_cur_tree.file_total;
        }
        else
        {
            bs_file_total = plist_file_total;
        }
    }
    else
    {
        bs_file_total = 0;
    }

    return TRUE;
}

/******************************************************************************/
/*
 * \par  Description:获取 级表的TREE
 *
 * \param[in]    sector_start--TREE的起始扇区偏移0～N
 tree_offset--TREE 的偏移位置 0～N

 * \param[out]

 * \return      plist_tree_t*

 * \note
 *******************************************************************************/
/*
 static plist_tree_t* bs_get_tree_info(uint16 sector_start,uint16 tree_offset)
 {
 return bs_read_tree_data(sector_start,tree_offset);
 }
 */

/******************************************************************************/
/*
 * \par  Description:设置断点层信息
 *
 * \param[in]    list_layer--层数
 list_layer_offset--最后一层的TREE偏移

 * \param[out]

 * \return       TRUE OR FALSE

 * \note
 *******************************************************************************/
//路径设置不成功，默认为所有歌曲播放
static bool bs_set_into_layer(uint8 list_layer, uint16 list_layer_offset)
{
    //    uint8 i=0;
    plist_tree_t *plist_tree_p;

    if (list_layer > 0)
    {
        plist_tree_p = bs_read_tree_data(plist_item_info.son_tree_offset[list_layer - 1], list_layer_offset);
        if (plist_tree_p->flag != 1)
        {
            return FALSE;
        }
        plist_layer_offset = list_layer_offset;
        libc_memcpy(&plist_cur_tree, plist_tree_p, sizeof(plist_tree_t));

    }
    dir_layer = list_layer;
    //    libc_memcpy(plist_list_layerno,list_layer_no,sizeof(plist_list_layerno));

    return TRUE;
}

/******************************************************************************/
/*
 * \par  Description:设置浏览的断点
 *
 * \param[in]    location--文件location
 type--location的类型（no used）

 * \param[out]

 * \return       TRUE OR FALSE

 * \note
 *******************************************************************************/
bool bs_plist_set_location(plist_location_t *location, fsel_type_e type)
{
    //    plist_f_info_t *plist_f_p;
    bool ret_val = TRUE;

    type = type;

    if (0 == bs_open_object(R_NORMAL_SEEK))
    {
        return FALSE;
    }

    if (NULL == location)//此处不能判断filename是否为空,因为要取list_type
    {
        ret_val = FALSE;
        plist_type_selector = PLIST_TITLE;
    }
    else
    {
        plist_type_selector = location->list_type;
    }

    bs_get_item_info(&plist_item_info, (plist_type_selector & 0x0f));
    dir_layer = 0;
    bs_select_listno = 0;
    //    plist_list_layerno[0]=0;
    plist_layer_offset = 0;
    plist_start_sort_index = 0;
    bs_dir_total = plist_item_info.son_tree_num[0];
    libc_memset(&plist_cur_tree, 0x00, sizeof(plist_tree_t));

    if (0 == bs_dir_total)
    {
        bs_file_total = plist_file_total;//title
    }
    else
    {
        bs_file_total = 0;
    }

    if ((FALSE == ret_val) || (0x00 == *(uint32*) &location->filename))
    {
        //默认在第0层
        bs_close_object();
        return FALSE;
    }

    if (0 == libc_memcmp(location->filename, "ROOT", 4))
    {
        bs_close_object();
        return TRUE;
    }

    if ((fsel_init_val.fsel_type == FSEL_TYPE_M3ULIST) && (location->list_layer == 1))
    {
        //m3u 子表
        dir_layer = 1;
        m3u_curlist_index = location->layer_list_offset & 0x7f;//请 audible标记
        //重新获取信息
        bs_close_object();
        bs_open_object(R_NORMAL_SEEK);
        ret_val = TRUE;
    }
    else
    {
        ret_val = bs_set_into_layer(location->list_layer, location->layer_list_offset);
    }
    if (FALSE != ret_val)
    {
        if (location->file_num <= bs_file_total)
        {
            bs_select_listno = location->file_num;
        }
        else
        {
            bs_select_listno = 0;
        }
        //赋值更新cur_ext_name
        libc_memcpy(&cur_ext_name, location->filename, 4);
    }

    bs_close_object();
    return TRUE;
}

/******************************************************************************/
/*
 * \par  Description: 把选中的文件从列表中删除
 *
 * \param[in]        type--删除类型，(只支持删除单个文件)
 list_no--选中序号(1~N)
 location--文件的location
 * \param[out]

 * \return           TRUE OR FALSE

 * \note            若list_no不为0，使用list_no为依据删除
 若list_no为0，使用location来删除文件
 *******************************************************************************/
bool bs_plist_delete(brow_del_e type, uint16 list_no, plist_location_t *location)
{
    bool ret_val = TRUE;
    uint8 tmp_fsel_type;
    uint8 count = 3;
    uint8 tmp_file_dir_info[8];//同步数据使用

    if (0 == bs_open_object(OPEN_MODIFY))
    {
        return FALSE;
    }

    if (list_no == 0)
    {
        if (NULL != location)
        {
            libc_memcpy(tmp_file_dir_info, &location->cluster_no, 8);
        }
        else
        {
            ret_val = FALSE;
        }
    }

    if (ret_val != FALSE)
    {
        if (m3u_curlist_index != 0xffff)
        {
            ret_val = bs_m3ufile_delete_func(type, list_no, tmp_file_dir_info);
            plist_file_total--;
        }
        else
        {
            ret_val = bs_plist_delete_func(type, list_no, tmp_file_dir_info);
        }

    }

    bs_close_object();

    if (FALSE != ret_val)
    {
        bs_file_total--;

        if ((fsel_init_val.fsel_type == FSEL_TYPE_PLAYLIST) || (m3u_curlist_index != 0xffff))
        {
            //M3U子表，MUSIC播放列表才需要，更新收藏夹删除
            tmp_fsel_type = FSEL_TYPE_LISTFAVOR1;
            while (count > 0)
            {
                count--;
                if (0 != bs_open_other(tmp_fsel_type, OPEN_MODIFY))
                {
                    bs_uspl_delete_func(FS_FILE, 0, tmp_file_dir_info);
                    bs_close_object();
                }
                tmp_fsel_type++;
            }
        }

        if ((fsel_init_val.fsel_type == FSEL_TYPE_PLAYLIST) || (fsel_init_val.fsel_type == FSEL_TYPE_LISTAUDIBLE))
        {
            if (0 != bs_open_other(FSEL_TYPE_M3ULIST, OPEN_MODIFY))
            {
                bs_m3ufile_delete_func(FS_FILE, 0, tmp_file_dir_info);
                bs_close_object();
            }
        }

        if (m3u_curlist_index != 0xffff)//m3u删除文件
        {
            if (0 != bs_open_other(FSEL_TYPE_PLAYLIST, OPEN_MODIFY))
            {
                bs_plist_delete_func(FS_FILE, 0, tmp_file_dir_info);
                bs_close_object();
            }

            if (0 != bs_open_other(FSEL_TYPE_LISTAUDIBLE, OPEN_MODIFY))
            {
                bs_plist_delete_func(FS_FILE, 0, tmp_file_dir_info);
                bs_close_object();
            }
        }

        //更新磁盘删除，删除只用8字节就可以了
        vfs_file_dir_offset(vfs_mount, NULL, bs_temp_buf, 0);
        vfs_file_dir_offset(vfs_mount, NULL, tmp_file_dir_info, 1);

        if (FSEL_TYPE_LISTEBOOK == fsel_init_val.fsel_type)
        {
            //ebook应用下删除书签
            bs_del_ebookbmk();
        }

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
bool bs_plist_set_file(plist_location_t *location, fsel_type_e type)
{
    type = type;
    return vfs_file_dir_offset(vfs_mount, &location->dir_layer_info, &location->cluster_no, 1);
}

//***************************************************
//list_no,0~n
//void bs_m3u_get_total(uint16 list_no)
//{
//    handle handle_pt;
//    m3udat_head_t *m3u_dat_pt;
//
//    list_no--;
//    m3u_curlist_index = list_no;
//    bs_dir_total = 0;
//    plist_info_offset = (M3U_DAT_FILEOFFSET/SECTOR_SIZE);
//
//    vfs_file_dir_offset(vfs_mount, NULL, &lib_pfile_offset, 1);
//    handle_pt = vfs_file_open(vfs_mount, NULL, R_NORMAL_SEEK);
//    if(handle_pt!=0)
//    {
//        vfs_file_read(vfs_mount, bs_temp_buf, SECTOR_SIZE, handle_pt);
//        m3u_dat_pt = (m3udat_head_t*)(bs_temp_buf + M3U_DAT_HEADOFFSET);
//        bs_file_total = m3u_dat_pt->list_file_num[list_no];
//        plist_file_total = m3u_dat_pt->dat_file_total;
//        vfs_file_close(vfs_mount,handle_pt);
//    }
//    else
//    {
//        bs_file_total = 0;
//    }
//
//}


