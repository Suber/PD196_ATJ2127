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
 * \brief    功能接口
 * \author   liminxian
 * \par      GENERAL DESCRIPTION:
 *               这里对文件进行描述
 * \par      EXTERNALIZED FUNCTIONS:
 *               FS
 * \version 1.0
 * \date  2011/9/15
 *******************************************************************************/

#include "eh_fsel.h"

#ifdef USE_83NAME_CREAT
static const char userpl_name[USERPL_MAX][12]=
{   "USERPL1 PL ","USERPL2 PL ","USERPL3 PL "};
static const char plist_lib_name[PL_LIB_MAX][12]=
{   "MUSIC   LIB","AUDIBLE LIB","VIDEO   LIB","PICTURE LIB","EBOOK   LIB","M3U     LIB"};
#else
static const char userpl_name[USERPL_MAX][12] =
{ "USERPL1.PL", "USERPL2.PL", "USERPL3.PL" };
static const char plist_lib_name[PL_LIB_MAX][12] =
{ "MUSIC.LIB", "AUDIBLE.LIB", "VIDEO.LIB", "PICTURE.LIB", "EBOOK.LIB", "M3U.LIB" };
#endif

//static handle obj_handle _BANK_DATA_ATTR_;;
static uint8 fsel_function_buf[SECTOR_SIZE] _BANK_DATA_ATTR_;

/******************************************************************************/
/*
 * \par  Description: mbyte to wchar
 *
 * \param[in]    dest，src

 * \param[out]

 * \return  UNICODE 个数，不包含标识符和结束符

 * \note    ASC转成UNICODE，自动加入标识符和结束符
 *******************************************************************************/
#ifndef NOTE_LIB_OFFSET

static uint16 mbyte_to_wchar(uint8 *dest, uint8 *src)
{
    uint16 num = 0;

#ifdef USE_83NAME_CREAT

    while(*src!=0x00)
    {
        *dest=*src;
        dest++;
        src++;
        num++;
    }

    dest=0x00;

#else

    *dest = 0xff;
    *(dest + 1) = 0xfe;
    dest += 2;

    while (*src != 0x00)
    {
        *dest = *src;
        *(dest + 1) = 0x00;
        dest += 2;
        src++;
        num++;
    }

    *dest = 0x00;
    *(dest + 1) = 0x00;

#endif

    return num;

}

#endif

/******************************************************************************/
/*
 * \par  Description: 读取tree结构信息
 *
 * \param[in]   tree_start_offset--tree的起始扇区偏移 0~N
 tree_offset --tree 偏移，0~N

 * \param[out]

 * \return      tree信息结构指针

 * \note
 *******************************************************************************/
static plist_tree_t *fsel_func_read_tree_data(uint16 tree_start_offset, uint16 tree_offset)
{
    uint16 buf_offset;
    uint16 sector_offset;

    if (tree_offset != 0)
    {
        sector_offset = tree_start_offset + (tree_offset * sizeof(plist_tree_t)) / SECTOR_SIZE;
    }
    else
    {
        sector_offset = tree_start_offset;
    }
    vfs_file_seek(vfs_mount, sector_offset * SECTOR_SIZE, SEEK_SET, obj_handle);
    vfs_file_read(vfs_mount, fsel_function_buf, SECTOR_SIZE, obj_handle);

    buf_offset = (tree_offset * sizeof(plist_tree_t)) % SECTOR_SIZE;

    return (plist_tree_t*) (fsel_function_buf + buf_offset);
}

/******************************************************************************/
/*
 * \par  Description: 获取index号
 *
 * \param[in]    index_start_offset--index列表的扇区偏移
 index--序号 0~N

 * \param[out]

 * \return       object_index 0~N

 * \note
 *******************************************************************************/
static uint16 fsel_func_get_object_index(uint16 index_start_offset, uint16 index)
{
    uint16 sector_offset;
    uint16 buf_offset;

    sector_offset = index_start_offset + index / 256;
    buf_offset = (index * 2) % SECTOR_SIZE;

    vfs_file_seek(vfs_mount, sector_offset * SECTOR_SIZE, SEEK_SET, obj_handle);
    vfs_file_read(vfs_mount, fsel_function_buf, SECTOR_SIZE, obj_handle);

    return *(uint16 *) (fsel_function_buf + buf_offset);

}

/******************************************************************************/
/*
 * \par  Description:打开文件后重新获取列表的信息
 *
 * \param[in]
 * \param[out]

 * \return       TRUE OR FALSE

 * \note
 *******************************************************************************/
//路径设置不成功，默认为所有歌曲播放
static bool fsel_func_update_plist_info(void)
{
    plist_head_t *plist_h_p;
    plist_tree_t *plist_tree_p;
    plist_item_t *plist_item_p;

    plist_h_p = (plist_head_t *) fsel_function_buf;

    //get head info
    plist_file_total = plist_h_p->file_total;
    plist_info_offset = plist_h_p->file_info_offset;

    //get plist_type_selector's items
    plist_item_p = (plist_item_t*) (fsel_function_buf + sizeof(plist_head_t));
    plist_item_p += (plist_type_selector & 0x0f);
    libc_memcpy(&plist_item_info, &plist_item_p->son_tree_num, sizeof(plist_item_info_t));

    if (dir_layer > 0)
    {
        plist_tree_p = fsel_func_read_tree_data(plist_item_info.son_tree_offset[dir_layer - 1], plist_layer_offset);
        if (plist_tree_p->flag == 1)
        {
            file_total = plist_tree_p->file_total;
            plist_start_sort_index = fsel_func_get_object_index(plist_item_info.sort_index_offset,
                    plist_tree_p->file_index);

        }
        else
        {
            //列表已删除，默认回到0层
            dir_layer = 0;
            file_no = 0;
        }
    }

    if (0 == dir_layer)
    {
        plist_layer_offset = 0;
        plist_start_sort_index = 0;
        file_total = plist_file_total;
    }

    return TRUE;
}

/******************************************************************************/
/*
 * \par  Description: open 列表文件
 *
 * \param[in]    type --列表类型
 open_mode--模式
 * \param[out]

 * \return       句柄

 * \note         若之前已打开文件，会自动先关闭。句柄已赋值给obj_handle
 *******************************************************************************/
uint32 fsel_open_object(uint8 open_mode)
{
#ifndef NOTE_LIB_OFFSET
    uint8 *file_name;
#endif

    uint8 type = fsel_init_val.fsel_type;
    userpl_head_t *upl_h;

    save_read_offset = 0xffff; //
    write_data_flag = FALSE; //
    upl_h = (userpl_head_t*) fsel_function_buf;

    if ((type >= FSEL_TYPE_LISTFAVOR1) && (fsel_init_val.disk == DISK_C))
    {
        //nand 分支
        //打开nand的收藏夹
        obj_open_mode = 0xee;//标记是VM的句柄
        obj_handle = VM_USERPL_INFO + (type - FSEL_TYPE_LISTFAVOR1) * USERPL_VM_SIZE;
        sys_vm_read(fsel_function_buf, obj_handle, SECTOR_SIZE);

    }
    else
    {
        //card ,host 分支
        if (obj_handle != 0)
        {
            vfs_file_close(vfs_mount, obj_handle);
            obj_handle = 0;
        }

        obj_open_mode = open_mode;

#ifndef NOTE_LIB_OFFSET
        if (type < FSEL_TYPE_LISTFAVOR1)
        {
            file_name = (uint8*) &plist_lib_name[(type - FSEL_TYPE_PLAYLIST) % PL_LIB_MAX];
        }
        else
        {
            file_name = (uint8*) &userpl_name[(type - FSEL_TYPE_LISTFAVOR1) % USERPL_MAX];
        }
#endif

        //只保存文件位置就可以了，不需要保存目录路径(此时目录路径已无用)
        vfs_file_dir_offset(vfs_mount, fsel_function_buf, fsel_function_buf + 128, 0);
        //默认进入根目录
        vfs_cd(vfs_mount, CD_ROOT, 0);

#ifdef NOTE_LIB_OFFSET
        //        vfs_file_dir_offset(vfs_mount, fsel_function_buf + 256, fsel_function_buf + 384, 0);
        //        vfs_file_dir_offset(vfs_mount, fsel_function_buf + 256, &lib_pfile_offset, 1);//必须传送两个参数，否则fs会有问题
        vfs_file_dir_offset(vfs_mount, NULL, &lib_pfile_offset, 1);
        obj_handle = vfs_file_open(vfs_mount, NULL, open_mode);
#else

        mbyte_to_wchar(fsel_function_buf + 256, file_name);
        obj_handle = vfs_file_open(vfs_mount, fsel_function_buf + 256, open_mode);
#endif

        vfs_file_dir_offset(vfs_mount, fsel_function_buf, fsel_function_buf + 128, 1);

        //读0扇区信息
        vfs_file_read(vfs_mount, fsel_function_buf, SECTOR_SIZE, obj_handle);

    }

    if (0 != obj_handle)
    {

        if ((fsel_function_buf[510] != 0x55) || (fsel_function_buf[511] != 0xaa))
        {
            fsel_close_object();
            obj_handle = 0;
        }
        else
        {
            if (m3u_curlist_index != 0xffff)
            {
                m3udat_head_t *head_tp = (m3udat_head_t*) (fsel_function_buf + M3U_DAT_HEADOFFSET);

                plist_file_total = head_tp->audible_file_num[m3u_curlist_index];

                //m3u 子表
                if ((plist_layer_offset & 0x80) == 0)
                {
                    //MUSIC 播放
                    plist_file_total = head_tp->list_file_num[m3u_curlist_index] - plist_file_total;
                }

                file_total = plist_file_total;
                plist_info_offset = (M3U_DAT_FILEOFFSET / SECTOR_SIZE);
                libc_memset(&plist_item_info, 0, sizeof(plist_item_info_t));
                plist_item_info.file_index_offset = (M3U_DAT_INDEXOFFSET / SECTOR_SIZE) + m3u_curlist_index
                        * (M3U_DAT_INDEXSIZE / SECTOR_SIZE);
                //            	plist_layer_offset = m3u_curlist_index;
            }
            else if (type < FSEL_TYPE_LISTFAVOR1)
            {
                fsel_func_update_plist_info();
            }
            else
            {
                //update_plist_info
                file_total = upl_h->file_num;
                uspl_first_offset = upl_h->first_offset;
                uspl_last_offset = upl_h->last_offset;
            }

            if (file_no > file_total)
            {
                //表示有删除文件操作，重定位播放位置
                file_no = 0;
            }

            return obj_handle;
        }
    }

    file_total = 0;
    return 0;
}

/******************************************************************************/
/*
 * \par  Description: 关闭 列表文件
 *
 * \param[in]    type --列表类型
 open_mode--模式
 * \param[out]

 * \return       句柄

 * \note
 *******************************************************************************/
bool fsel_close_object(void)
{
    if (obj_handle != 0)
    {
        if (0xee != obj_open_mode)
        {
            vfs_file_close(vfs_mount, obj_handle);
        }

        obj_handle = 0;
    }
    return TRUE;
}

