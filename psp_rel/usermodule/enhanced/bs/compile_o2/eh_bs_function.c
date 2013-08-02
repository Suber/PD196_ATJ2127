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
 * \file     eh_bs_function.c
 * \brief    浏览公共接口
 * \author   liminxian
 * \par      GENERAL DESCRIPTION:
 *               这里对文件进行描述
 * \par      EXTERNALIZED FUNCTIONS:
 *               文件系统
 * \version 1.0
 * \date  2011/9/15
 *******************************************************************************/

#include "../eh_bs.h"

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

uint8 bs_function_buf[SECTOR_SIZE] _BANK_DATA_ATTR_;

extern plist_tree_t plist_cur_tree;

/******************************************************************************/
/*
 * \par  Description: mbyte to wchar
 *
 * \param[in]    dest，src

 * \param[out]

 * \return  UNICODE 个数，不包含标识符和结束符

 * \note    ASC转成UNICODE，自动加入标识符和结束符
 *******************************************************************************/
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
static plist_tree_t *bs_func_read_tree_data(uint16 tree_start_offset, uint16 tree_offset)
{
    uint16 buf_offset;
    uint16 sector_offset;

    sector_offset = tree_start_offset + (tree_offset * sizeof(plist_tree_t)) / SECTOR_SIZE;

    vfs_file_seek(vfs_mount, sector_offset * SECTOR_SIZE, SEEK_SET, obj_handle);
    vfs_file_read(vfs_mount, bs_function_buf, SECTOR_SIZE, obj_handle);

    buf_offset = (tree_offset * sizeof(plist_tree_t)) % SECTOR_SIZE;

    return (plist_tree_t*) (bs_function_buf + buf_offset);
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
static bool bs_func_update_plist_info(void)
{
    plist_head_t *plist_h_p;
    plist_tree_t *plist_tree_p;
    plist_item_t *plist_item_p;

    plist_h_p = (plist_head_t *) bs_function_buf;

    //get head info
    plist_file_total = plist_h_p->file_total;
    plist_info_offset = plist_h_p->file_info_offset;

    //get plist_type_selector's items
    plist_item_p = (plist_item_t*) (bs_function_buf + sizeof(plist_head_t));
    plist_item_p += (plist_type_selector & 0x0f);
    libc_memcpy(&plist_item_info, &plist_item_p->son_tree_num, sizeof(plist_item_info_t));

    if (dir_layer > 0)
    {
        plist_tree_p = bs_func_read_tree_data(plist_item_info.son_tree_offset[dir_layer - 1], plist_layer_offset);
        if (1 == plist_tree_p->flag)
        {
            bs_dir_total = plist_tree_p->son_num;
            if (0 == bs_dir_total)
            {
                bs_file_total = plist_tree_p->file_total;
            }
            else
            {
                bs_file_total = 0;
            }

            libc_memcpy(&plist_cur_tree, plist_tree_p, sizeof(plist_tree_t));
        }
        else
        {
            //列表已删除，默认回到0层
            dir_layer = 0;
            plist_layer_offset = 0;
        }
    }

    if (0 == dir_layer)
    {
        plist_start_sort_index = 0;
        bs_dir_total = plist_item_info.son_tree_num[0];
        if (0 == bs_dir_total)
        {
            bs_file_total = plist_file_total;
        }
        else
        {
            bs_file_total = 0;
        }
        libc_memset(&plist_cur_tree, 0x00, sizeof(plist_tree_t));
    }

    return TRUE;
}

/******************************************************************************/
/*
 * \par  Description: open 文件
 *
 * \param[in]
 open_mode--模式
 * \param[out]

 * \return

 * \note        并读出第一个扇区
 *******************************************************************************/
static void bs_open_file(fsel_type_e type, uint8 open_mode)
{
    uint8 *file_name;

    save_read_offset = 0xffff; //
    write_data_flag = FALSE; //

    //赋值obj_open_type
    obj_open_type = type;

    if ((type >= FSEL_TYPE_LISTFAVOR1) && (fsel_init_val.disk == DISK_C))
    {
        //打开nand的收藏夹
        obj_open_mode = 0xee;//标记是VM的句柄
        obj_handle = VM_USERPL_INFO + (type - FSEL_TYPE_LISTFAVOR1) * USERPL_VM_SIZE;
        //读出0扇区
        sys_vm_read(bs_function_buf, obj_handle, SECTOR_SIZE);
        //return;
        goto open_ok;
    }

    if (obj_handle != 0)
    {
        vfs_file_close(vfs_mount, obj_handle);
        obj_handle = 0;
    }

    obj_open_mode = open_mode;

    //默认进入根目录
    vfs_file_dir_offset(vfs_mount, bs_function_buf, bs_function_buf + 128, 0);

    vfs_cd(vfs_mount, CD_ROOT, 0);

#ifdef NOTE_LIB_OFFSET

    if(type == fsel_init_val.fsel_type)
    {
        //        vfs_file_dir_offset(vfs_mount, bs_function_buf + 256, bs_function_buf + 384, 0);//必须传送两个参数，否则fs会有问题
        //        vfs_file_dir_offset(vfs_mount, bs_function_buf + 256, &lib_pfile_offset, 1);
        vfs_file_dir_offset(vfs_mount, NULL, &lib_pfile_offset, 1);
        obj_handle = vfs_file_open(vfs_mount, NULL, open_mode);
        goto open_end;
    }

#endif

    if (type < FSEL_TYPE_LISTFAVOR1)
    {
        file_name = (uint8*) &plist_lib_name[(type - FSEL_TYPE_PLAYLIST) % PL_LIB_MAX];
    }
    else
    {
        file_name = (uint8*) &userpl_name[(type - FSEL_TYPE_LISTFAVOR1) % USERPL_MAX];
    }
    mbyte_to_wchar(bs_function_buf + 256, file_name);
    obj_handle = vfs_file_open(vfs_mount, bs_function_buf + 256, open_mode);

    open_end:

    vfs_file_dir_offset(vfs_mount, bs_function_buf, bs_function_buf + 128, 1);

    if (0 != obj_handle)
    {
        //读出0扇区
        vfs_file_read(vfs_mount, bs_function_buf, SECTOR_SIZE, obj_handle);
    }

    open_ok:
    //判断表是否有效
    if ((bs_function_buf[510] != 0x55) || (bs_function_buf[511] != 0xaa))
    {
        bs_close_object();
        obj_handle = 0;
    }

    return;
}

/******************************************************************************/
/*
 * \par  Description: open 列表文件
 *
 * \param[in]
 open_mode--模式
 * \param[out]

 * \return       句柄

 * \note         若之前已打开文件，会自动先关闭。句柄已赋值给obj_handle

 该接口提供给播放获取当前列表或收藏夹中的文件信息下使用，
 若是删除打开调接口bs_open_other
 *******************************************************************************/
uint32 bs_open_object(uint8 open_mode)
{
    userpl_head_t *upl_h;
    bs_open_file(fsel_init_val.fsel_type, open_mode);

    if (0 != obj_handle)
    {
        //bs_open_file已，读出0扇区，del 放入bs_open_file执行
        if (m3u_curlist_index != 0xffff)
        {
            //m3u 子表
            m3udat_head_t *m3u_h_p = (m3udat_head_t *)(bs_function_buf + M3U_DAT_HEADOFFSET);
            plist_file_total = m3u_h_p->list_file_num[m3u_curlist_index];
//            plist_file_total = *(uint16*) (bs_function_buf + ((M3U_DAT_HEADOFFSET + 2) + ((m3u_curlist_index << 1))
//                    % SECTOR_SIZE));
            bs_file_total = plist_file_total;
            bs_dir_total = 0;
            dir_layer = 1;
            plist_info_offset = (M3U_DAT_FILEOFFSET / SECTOR_SIZE);
            libc_memset(&plist_item_info, 0, sizeof(plist_item_info_t));
            plist_item_info.file_index_offset = (M3U_DAT_INDEXOFFSET / SECTOR_SIZE) + m3u_curlist_index
                    * (M3U_DAT_INDEXSIZE / SECTOR_SIZE);
            plist_layer_offset = m3u_curlist_index;
        }
        else if (obj_open_type < FSEL_TYPE_LISTFAVOR1)
        {
            bs_func_update_plist_info();
        }
        else
        {
            //update_plist_info
            upl_h = (userpl_head_t*) bs_function_buf;
            bs_dir_total = 0;
            bs_file_total = upl_h->file_num;
            uspl_file_total = bs_file_total;
            uspl_first_offset = upl_h->first_offset;
            uspl_last_offset = upl_h->last_offset;

        }

        if ((bs_select_listno != 0xffff) && (bs_select_listno > bs_dir_total + bs_file_total))
        {
            //表示有删除文件操作，重定位
            bs_select_listno = 0;
        }
    }
    else
    {
        return 0;
    }

    return obj_handle;

}

/******************************************************************************/
/*
 * \par  Description: open 列表文件
 *
 * \param[in]    type--fsel_type_e
 open_mode--模式
 * \param[out]

 * \return       句柄

 * \note         若之前已打开文件，会自动先关闭。句柄已赋值给obj_handle

 该接口提供给删除列表或收藏夹中的文件场景下使用
 *******************************************************************************/
uint32 bs_open_other(fsel_type_e type, uint8 open_mode)
{
    bs_open_file(type, open_mode);

    if (0 != obj_handle)
    {
        //读出0扇区，del 放入bs_open_file执行
        //        vfs_file_read(vfs_mount, bs_function_buf, SECTOR_SIZE, obj_handle);
        if (obj_open_type < FSEL_TYPE_LISTFAVOR1)
        {
            if (obj_open_type == FSEL_TYPE_M3ULIST)
            {
                plist_file_total = *(uint16*) (bs_function_buf + M3U_DAT_HEADOFFSET);
                plist_info_offset = (M3U_DAT_FILEOFFSET / SECTOR_SIZE);
                libc_memset(&plist_item_info, 0, sizeof(plist_item_info_t));
                plist_item_info.file_index_offset = (M3U_DAT_INDEXOFFSET / SECTOR_SIZE);
            }
            else
            {
                plist_head_t *plist_h_p;
                plist_h_p = (plist_head_t *) bs_function_buf;
                //get head info
                plist_file_total = plist_h_p->file_total;
                plist_info_offset = plist_h_p->file_info_offset;
            }

        }
        else
        {
            //update_plist_info
            userpl_head_t *upl_h;
            upl_h = (userpl_head_t*) bs_function_buf;
            //get head info
            uspl_file_total = upl_h->file_num;
            uspl_first_offset = upl_h->first_offset;
            uspl_last_offset = upl_h->last_offset;
        }

    }

    return obj_handle;
}

/******************************************************************************/
/*
 * \par  Description: 关闭 列表文件
 *
 * \param[in]
 open_mode--模式
 * \param[out]

 * \return       句柄

 * \note
 *******************************************************************************/
bool bs_close_object(void)
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

