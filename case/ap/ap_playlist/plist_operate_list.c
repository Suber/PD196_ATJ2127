/*******************************************************************************
 *                              US212A
 *                            Module: PLAYLSIT
 *                 Copyright(c) 2003-2011 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       liminxian  2011-9-15 15:37    1.0             build this file
 *******************************************************************************/
/*!
 * \file     *.c
 * \brief    这里填写文件的概述
 * \author   liminxian
 * \par      GENERAL DESCRIPTION:
 *               这里对文件进行描述
 * \par      EXTERNALIZED FUNCTIONS:
 *               这里描述调用到外面的模块
 * \version 1.0
 * \date  2011/9/15
 *******************************************************************************/
#include "plist.h"
#include "rdata_ex.h"

//系统默认创建文件使用长名
#ifdef USE_83NAME_CREAT
static const char plist_lib_name[PL_LIB_MAX][12]=
{   "MUSIC   LIB","AUDIBLE LIB","VIDEO   LIB","PICTURE LIB","EBOOK   LIB","M3U     LIB"};
#else
static const char plist_lib_name[PL_LIB_MAX][12] =
{ "MUSIC.LIB", "AUDIBLE.LIB", "VIDEO.LIB", "PICTURE.LIB", "EBOOK.LIB", "M3U.LIB" };
#endif

static const uint8 plist_libname_len[PL_LIB_MAX] =
{ 5, 7, 5, 7, 5, 3 };

static const uint32 plist_lib_size[PL_LIB_MAX] =
{ MUSIC_LIB_SIZE, AUDIBLE_LIB_SIZE, VIDEO_LIB_SIZE, PICTURE_LIB_SIZE, EBOOK_LIB_SIZE, M3U_LIB_SIZE };

//char plist_name[]={"PLISTLIB"};
static const char allsong_name[] =
{ "ALLSONG" };
static const char album_name[] =
{ "ALBUM" };
static const char artist_name[] =
{ "ARTIST" };
static const char genre_name[] =
{ "GENRE" };
static const char a_book_name[] =
{ "BOOK" };
static const char a_author_name[] =
{ "AUTHOR" };
static const char allfile_name[] = "ALLFILE";

static const uint8 stg_space_err[] =
{ S_PLIST_C_SPACE_ERR, S_PLIST_H_SPACE_ERR, S_PLIST_U_SPACE_ERR };

static bool mdy_file_head(uint8 type);

extern void plist_reflash_string(uint8 str_id) __FAR__;

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
 * \par  Description: 删除lib文件,并判断磁盘空间
 *
 * \param[in]    type--列表类型,tmp_drv_sn--介质序号

 * \param[out]

 * \return  0--no err; >0--err

 * \note    统一删除，文件系统只建表一次；每个lib预留1M空间
 *******************************************************************************/
int8 delete_lib_file(uint8 lib_bitmap, uint8 tmp_drv_sn)
{

    /*

     uint8 i;
     uint32 need_space,fs_space;

     vfs_cd(ap_vfs_mount, CD_ROOT, 0);

     plist_get_app_msg();

     need_space=0x800;
     fs_space=0;
     for (i = 0; i < PL_LIB_MAX; i++)
     {
     if ((lib_bitmap & 0x01) != 0)
     {
     mbyte_to_wchar(temp_plist_buf, (uint8*) &plist_lib_name[i]);
     vfs_file_dir_remove(ap_vfs_mount, temp_plist_buf, FS_FILE);
     need_space += 0x800;//1M，每个lib预留1M空间
     plist_get_app_msg();
     }
     lib_bitmap = lib_bitmap >> 1;
     }

     if(need_space != 0)
     {
     vfs_get_space(ap_vfs_mount,&fs_space,1);
     if(need_space>fs_space)//fs_space
     {
     plist_reflash_string(stg_space_err[tmp_drv_sn&0x03]);
     sys_os_time_dly(35);//显示界面使用
     return -1;
     }

     plist_get_app_msg();
     }

     */

    return 0;
}

/******************************************************************************/
/*
 * \par  Description: 创建对应列表文件（lib格式）
 *
 * \param[in]    type--列表类型

 * \param[out]

 * \return       TRUE OR FALSE

 * \note
 *******************************************************************************/
//创建库文件
bool creat_plist_lib(uint8 type)
{
    uint32 temp32 = 0;
    plist_head_t *head_p;
    plist_item_t *item_p;
    bool ret_val;

    //先清offset
    libc_memset(plist_lib_offset[type], 0xff, 8);

    vfs_cd(ap_vfs_mount, CD_ROOT, 0);

    mbyte_to_wchar(temp_plist_buf, (uint8*) &plist_lib_name[type]);

    ap_obj_handle = vfs_file_open(ap_vfs_mount, temp_plist_buf, OPEN_MODIFY);

    if (0 != ap_obj_handle)
    {
        vfs_file_get_size(ap_vfs_mount, &temp32, ap_obj_handle, 0);
        if (temp32 < plist_lib_size[type])
        {
            //lib文件变大了，需要重新创建
            vfs_file_close(ap_vfs_mount, ap_obj_handle);
            ap_obj_handle = 0;
        }
    }

    if (0 == ap_obj_handle)
    {
        vfs_file_dir_remove(ap_vfs_mount, temp_plist_buf, FS_FILE);
        ap_obj_handle = vfs_file_create(ap_vfs_mount, temp_plist_buf, plist_lib_size[type]);

        if (0 == ap_obj_handle)
        {
            //显示磁盘空间不足
            plist_reflash_string(stg_space_err[drv_sn & 0x03]);
            sys_os_time_dly(35);//显示界面使用
            return FALSE;
        }

#ifdef HIDE_FILE_OPT
        vfs_file_attralter(ap_vfs_mount, ATTR_HIDDEN, ap_obj_handle, 0);
#endif

        vfs_file_close(ap_vfs_mount, ap_obj_handle);
        ap_obj_handle = vfs_file_open(ap_vfs_mount, temp_plist_buf, OPEN_MODIFY);
    }

    if (0 != ap_obj_handle)
    {
        vfs_file_dir_offset(ap_vfs_mount, NULL, plist_lib_offset[type], 0);

        ap_pos_file_offset = 0;
        libc_memset(temp_plist_buf, 0x00, SECTOR_SIZE);
        head_p = (plist_head_t*) temp_plist_buf;
        item_p = (plist_item_t*) (temp_plist_buf + sizeof(plist_head_t));
        libc_memcpy(&head_p->plist_name, &plist_lib_name[type][0], (uint32) plist_libname_len[type]);
        if (type == 0)//music
        {
            libc_memcpy(item_p->item_name, allsong_name, sizeof(allsong_name));
            item_p++;
            libc_memcpy(item_p->item_name, album_name, sizeof(album_name));
            item_p++;
            libc_memcpy(item_p->item_name, artist_name, sizeof(artist_name));
            item_p++;
            libc_memcpy(item_p->item_name, genre_name, sizeof(genre_name));

        }
        else if (type == 1)//audible
        {
            //title
            libc_memcpy(item_p->item_name, a_book_name, sizeof(a_book_name));
            item_p += 2;
            //artist
            libc_memcpy(item_p->item_name, a_author_name, sizeof(a_author_name));
        }
        else//others
        {
            libc_memcpy(item_p->item_name, allfile_name, sizeof(allfile_name));
        }

        ret_val = save_to_file(temp_plist_buf, SECTOR_SIZE);

    }
    else
    {
        ret_val = FALSE;
    }

    return ret_val;

}
/******************************************************************************/
/*
 * \par  Description: 保存创建的列表
 *
 * \param[in]     type--列表类型

 * \param[out]

 * \return       TRUE OR FALSE

 * \note
 *******************************************************************************/
//关闭库文件
bool close_plist_lib(uint8 type)
{
    bool ret_val;

    ret_val = vfs_file_close(ap_vfs_mount, ap_obj_handle);
    if (ret_val != FALSE)
    {
        ret_val = mdy_file_head(type);
    }

    return ret_val;
}
/******************************************************************************/
/*
 * \par  Description: 修改列表文件的头信息
 *
 * \param[in]    type--列表类型

 * \param[out]

 * \return       TRUE OR FALSE

 * \note
 *******************************************************************************/
//写入头信息
static bool mdy_file_head(uint8 type)
{
    bool ret_val = 1;
    uint8 i;
    plist_head_t *plist_h_p;
    plist_item_t *plist_item_p;

    // 更新文件列表
    vfs_cd(ap_vfs_mount, CD_ROOT, 0);

    mbyte_to_wchar(temp_plist_buf, (uint8*) &plist_lib_name[type]);
    ap_obj_handle = vfs_file_open(ap_vfs_mount, temp_plist_buf, OPEN_MODIFY);

    if (0 != ap_obj_handle)
    {
        vfs_file_read(ap_vfs_mount, temp_plist_buf, SECTOR_SIZE, ap_obj_handle);

        //加入标识符
        temp_plist_buf[510] = 0x55;
        temp_plist_buf[511] = 0xAA;

        plist_h_p = (plist_head_t*) temp_plist_buf;
        plist_item_p = (plist_item_t*) (temp_plist_buf + sizeof(plist_head_t));

        plist_h_p->file_total = ap_plist_file_total;
        plist_h_p->file_info_offset = ap_plist_file_offset;
        //        plist_h_p->audible_num=audible_num;
        //        plist_h_p->audible_info_offset=audible_info_offset;

        for (i = 0; i < 4; i++)
        {
            plist_item_p->file_index_offset = ap_file_index_offset[i];
            plist_item_p->sort_index_offset = ap_sort_index_offset[i];
            plist_item_p->index_parent_offset = ap_index_parent_offset[i];

            libc_memcpy(plist_item_p->son_tree_num, &ap_son_tree_num[i], 6);
            libc_memcpy(plist_item_p->son_tree_offset, &ap_son_tree_offset[i], 6);

            plist_item_p++;
        }

        vfs_file_seek(ap_vfs_mount, 0, SEEK_SET, ap_obj_handle);
        vfs_file_write(ap_vfs_mount, temp_plist_buf, SECTOR_SIZE, ap_obj_handle);

        vfs_file_close(ap_vfs_mount, ap_obj_handle);

    }

    return ret_val;

}
/******************************************************************************/
/*
 * \par  Description: 调试信息打印
 *
 * \param[in]

 * \param[out]

 * \return

 * \note
 *******************************************************************************/
void list_printf_debug(char *buf, uint16*file_dex, uint16 count, uint8 len)
{

#ifdef _VC_DEBUG
    uint16 i=0;
    char temp_str_buf[128];
    uint16 offset;
    uint16 t_file_num;

    printf("\n");
    while(i<count)
    {
        t_file_num=*file_dex;
        offset=len*t_file_num;
        memcpy(temp_str_buf,buf+offset,len);
        temp_str_buf[len]=0x00;
        printf("\n");
        printf("(--%d)-- %s",i,temp_str_buf);

        i++;
        file_dex++;
    }
#endif
}

