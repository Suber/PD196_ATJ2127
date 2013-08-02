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
 * \file     eh_bs_directory_func.c
 * \brief    浏览磁盘
 * \author   liminxian
 * \par      GENERAL DESCRIPTION:
 *               接口处理
 * \par      EXTERNALIZED FUNCTIONS:
 *               文件系统，VRAM
 * \version 1.0
 * \date  2011/9/15
 *******************************************************************************/

#include "eh_bs.h"

extern uint8 bs_temp_buf[SECTOR_SIZE];
//extern bs_dir_location_t g_bs_dir_loc;

extern bool bs_dir_function(uint8 dir_type, uint32 object_type);
extern bool bs_dir_set_dirpos(uint32 object_type, uint16 dir_num);
extern bool bs_plist_delete_func(brow_del_e type,uint16 list_no ,uint8 *file_dir_info) __FAR__;
extern bool bs_uspl_delete_func(brow_del_e type,uint16 list_no ,uint8 *file_dir_info) __FAR__;
extern bool bs_m3ufile_delete_func(brow_del_e type, uint16 list_no, uint8 *file_dir_info) __FAR__;

extern bool bs_del_ebookbmk(void) __FAR__;

static uint32 check_ext_bitmap(uint32 tmp_ext_name);

//对应BROM固化表，不能修改，NOTE!!!!!!
static const char ext_vec_rom[32][4] =
{ "***", "MP3", "WMA", "WAV", "AAC", "FLAC", "APE", "AVI", "TXT", "JPG", "JPEG", "BMP", "GIF", "LRC", "ASF", "OGG",
        "WMV", "AMV", "AAX", "AA ", "MP1", "MP2", "M4A", "** ", "** ", "** ", "** ", "** ", "** ", "**", "*f", "*d" };

/////bit<31> = 0h表示对应ROM中的ext_vec_rom后缀名数组
////#define MUSIC_BITMAP      0x7e03be00
//#define MUSIC_BITMAP        0x76000000
//#define AUDIBLE_BITMAP      0x08003200
//#define VIDEO_BITMAP        0x01004000
//#define PICTURE_BITMAP      0x00780000
//#define TEXT_BITMAP         0x00840000
//#define ALLALL_BITMAP       0x00000004
//#define ALLFILE_BITMAP      0x00000002
//#define ALLDIR_BITMAP       0x00000001
///*! \brief 后台音乐播放时的图片文件名过滤 */
//#define PICTURE_BITMAP1     0x00600000

static bool del_object_plist(uint8 type, uint8 *file_dir_info);
static bool del_object_uspl(uint8 type, uint8 *file_dir_info);

void deal_if_83name(uint8 *name_buf);

/******************************************************************************/
/*
 * \par  Description: 把选中的文件或文件夹(必须是空的)从磁盘中删除
 *
 * \param[in]        type--删除类型
 list_no--选中序号
 location--文件的location
 * \param[out]

 * \return           TRUE OR FALSE

 * \note            若list_no不为0，使用list_no为依据删除
 若list_no为0，使用location来删除文件
 *******************************************************************************/
bool bs_dir_delete(brow_del_e type, uint16 list_no, file_location_t *location)
{
    bool ret_val;
    uint8 del_type;
    uint8 tmp_file_dir_info[8];
    uint32 tmp_ext_name;
    uint32 tmp_bitmap;

    ret_val = TRUE;

    vfs_file_dir_offset(vfs_mount, bs_temp_buf, bs_temp_buf + 128, 0);

    if (list_no == 0)
    {
        //使用location闪存
        del_type = FS_FILE;
        vfs_file_dir_offset(vfs_mount, &location->dir_layer_info, &location->cluster_no, 1);
        //       vfs_get_name(vfs_mount, &tmp_ext_name,0);
        //       ret_val= vfs_file_dir_remove(vfs_mount,NULL,FS_FILE);
        libc_memcpy(&tmp_file_dir_info, &location->cluster_no, 8);
    }
    else
    {
        if (list_no > bs_dir_total)
        {
            //定位文件
            ret_val = bs_dir_set_dirpos(fsel_init_val.file_type_bit, list_no - bs_dir_total);
            del_type = FS_FILE;
        }
        else
        {
            //定位文件夹
            ret_val = bs_dir_set_dirpos(EH_GET_EXT_DIR, list_no);
            del_type = FS_DIR;
        }

        if (FALSE != ret_val)
        {
            //只取8字节就可以删除文件
            vfs_file_dir_offset(vfs_mount, NULL, &tmp_file_dir_info, 0);
        }
    }

    if (FALSE != ret_val)
    {
        vfs_get_name(vfs_mount, &tmp_ext_name, 0);
        tmp_bitmap = check_ext_bitmap(tmp_ext_name);

        if ((tmp_bitmap & TEXT_BITMAP) != 0)
        {
            //删除书签
            bs_del_ebookbmk();
        }

        ret_val = vfs_file_dir_remove(vfs_mount, NULL, del_type);
    }

    vfs_file_dir_offset(vfs_mount, bs_temp_buf, bs_temp_buf + 128, 1);

    if ((FALSE != ret_val) && (del_type == FS_FILE))
    {
        //同步更新播放列表或收藏夹文件
        if ((tmp_bitmap & MUSIC_BITMAP) != 0)
        {
            del_object_plist(FSEL_TYPE_PLAYLIST, tmp_file_dir_info);
            del_object_uspl(FSEL_TYPE_LISTFAVOR1, tmp_file_dir_info);
            del_object_plist(FSEL_TYPE_M3ULIST, tmp_file_dir_info);

        }
        else if ((tmp_bitmap & AUDIBLE_BITMAP) != 0)
        {
            del_object_plist(FSEL_TYPE_LISTAUDIBLE, tmp_file_dir_info);
            del_object_uspl(FSEL_TYPE_LISTFAVOR1, tmp_file_dir_info);
            del_object_plist(FSEL_TYPE_M3ULIST, tmp_file_dir_info);
        }
        else if ((tmp_bitmap & VIDEO_BITMAP) != 0)
        {
            del_object_plist(FSEL_TYPE_LISTVIDEO, tmp_file_dir_info);
        }
        else if ((tmp_bitmap & PICTURE_BITMAP) != 0)
        {
            del_object_plist(FSEL_TYPE_LISTPICTURE, tmp_file_dir_info);
            //            if ((tmp_bitmap & PICTURE_BITMAP1) != 0)
            //            {
            //                del_object_plist(FSEL_TYPE_LIST_PIC2, tmp_file_dir_info);
            //            }
        }
        else if ((tmp_bitmap & TEXT_BITMAP) != 0)
        {
            del_object_plist(FSEL_TYPE_LISTEBOOK, tmp_file_dir_info);
        }
        //        else if ((tmp_bitmap & PICTURE_BITMAP1) != 0)
        //        {
        //            del_object_plist(FSEL_TYPE_LIST_PIC2, tmp_file_dir_info);
        //        }
        else
        {
            ;//不处理
        }

    }

    return ret_val;
}

//************************************
static bool del_object_plist(uint8 type, uint8 *file_dir_info)
{
    if (0 != bs_open_other(type, OPEN_MODIFY))
    {
        if (type == FSEL_TYPE_M3ULIST)
        {
            bs_m3ufile_delete_func(FS_FILE, 0, file_dir_info);
        }
        else
        {
            bs_plist_delete_func(FS_FILE, 0, file_dir_info);
        }
        bs_close_object();
    }
    return TRUE;
}

//************************************
static bool del_object_uspl(uint8 type, uint8 *file_dir_info)
{
    uint8 count = 3;
    while (count > 0)
    {
        count--;
        if (0 != bs_open_other(type, OPEN_MODIFY))
        {
            bs_uspl_delete_func(FS_FILE, 0, file_dir_info);
            bs_close_object();
        }
        type++;
    }
    return TRUE;
}
//************************************
static uint32 check_ext_bitmap(uint32 tmp_ext_name)
{
    uint8 i;
    uint32 bit_map = 0x80000000;

    for (i = 0; i < 32; i++)
    {
        if (0 == libc_memcmp(ext_vec_rom[i], &tmp_ext_name, 4))
        {
            break;
        }
        else
        {
            bit_map = bit_map >> 1;
        }
    }

    if (i == 32)
    {
        return 0;
    }

    return bit_map;

}

/******************************************************************************/
/*
 * \par  Description: 获取目录的列表信息
 *
 * \param[in]    decode_param--指定获取参数

 * \param[out]   file_list--列表信息

 * \return       TRUE OR FALSE

 * \note
 *******************************************************************************/
//获取显示信息
bool bs_dir_get_items(brow_decode_t *decode_param, void *file_list)
{
    file_brow_t *file_name;
    uint8 i;
    uint8 out_flag = 0;//填空标志

    file_name = (file_brow_t *) file_list;
    i = decode_param->num;

    if (decode_param->top > bs_dir_total)
    {
        //get file name
        if (FALSE == bs_dir_set_dirpos(fsel_init_val.file_type_bit, decode_param->top - bs_dir_total))
        {
            out_flag = 1;
        }
        //        while(i-->0)
        for (; i > 0; i--)
        {
            if (out_flag == 0)
            {

                vfs_get_name(vfs_mount, file_name->name_buf, file_name->name_len / 2);
                deal_if_83name(file_name->name_buf);
                libc_memcpy(&file_name->ext, &cur_ext_name, 4);
                if (FALSE == bs_dir_function(DIR_NEXT, fsel_init_val.file_type_bit))
                {
                    out_flag = 1;
                }
            }
            else
            {
                file_name->name_buf[0] = '\0';
                file_name->ext[0] = '\0';
            }
            file_name++;
        }

    }
    else if ((decode_param->top + i - 1) <= bs_dir_total)
    {
        //get dir name
        if (FALSE == bs_dir_set_dirpos(EH_GET_EXT_DIR, decode_param->top))
        {
            out_flag = 1;
        }
        //        while(i-->0)
        for (; i > 0; i--)
        {
            if (out_flag == 0)
            {
                vfs_get_name(vfs_mount, file_name->name_buf, file_name->name_len / 2);
                deal_if_83name(file_name->name_buf);
                //file_name->ext[0]='\0';
                libc_memcpy(&file_name->ext, &cur_ext_name, 4);
                if (FALSE == bs_dir_function(DIR_NEXT, EH_GET_EXT_DIR))
                {
                    out_flag = 1;
                }
            }
            else
            {
                file_name->name_buf[0] = '\0';
                file_name->ext[0] = '\0';
            }
            file_name++;
        }

    }
    else
    {
        //get dir and file name
        uint16 j;
        j = decode_param->top;
        if (FALSE == bs_dir_set_dirpos(EH_GET_EXT_DIR, j))
        {
            out_flag = 1;
        }
        //        while(i-->0)
        for (; i > 0; i--)
        {

            if (out_flag == 1)
            {
                file_name->name_buf[0] = '\0';
                file_name->ext[0] = '\0';
            }
            else
            {
                vfs_get_name(vfs_mount, file_name->name_buf, file_name->name_len / 2);
                deal_if_83name(file_name->name_buf);
                libc_memcpy(&file_name->ext, &cur_ext_name, 4);
                if (j > bs_dir_total)
                {
                    //                    libc_memcpy(&file_name->ext,&cur_ext_name,4);
                    if (FALSE == bs_dir_function(DIR_NEXT, fsel_init_val.file_type_bit))
                    {
                        out_flag = 1;
                    }
                }
                else if (j < bs_dir_total)
                {
                    //file_name->ext[0]='\0';
                    if (FALSE == bs_dir_function(DIR_NEXT, EH_GET_EXT_DIR))
                    {
                        out_flag = 1;
                    }
                }
                else
                {
                    //file_name->ext[0]='\0';
                    if (FALSE == bs_dir_function(DIR_HEAD, fsel_init_val.file_type_bit))
                    {
                        out_flag = 1;
                    }
                }
            }

            j++;
            file_name++;

        }
    }

    return TRUE;
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
bool bs_dir_set_file(file_location_t *location, fsel_type_e type)
{
    type = type;
    return vfs_file_dir_offset(vfs_mount, &location->dir_layer_info, &location->cluster_no, 1);
}

/******************************************************************************/
/*
 * \par  Description: 过滤空格,转换为 xx.yyy格式，作显示
 *
 * \param[in]    name_buf

 * \param[out]

 * \return

 * \note
 *******************************************************************************/

//过滤空格,转换为 xx.yyy格式
void deal_if_83name(uint8 *name_buf)
{
    uint8 i, j;

    if ((name_buf[0] == 0xff) && (name_buf[1] == 0xfe))
    {
        return;
    }

    //扩展名往后移1个字节
    for (i = 11; i > 8; i--)
    {
        name_buf[i] = name_buf[i - 1];
    }

    //过滤文件名空格
    for (i = 7; i > 0; i--)
    {
        if (name_buf[i] != 0x20)
        {
            break;
        }
    }

    //加点
    i++;
    name_buf[i] = 0x2e; //加"."

    //过滤扩展名空格
    for (j = 9; j < 12; j++)
    {
        if (name_buf[j] != 0x20)
        {
            i++;
            name_buf[i] = name_buf[j];
        }
    }

    if (name_buf[i] != 0x2e)
    {
        i++;
    }

    name_buf[i] = 0x00;

    return;
}
