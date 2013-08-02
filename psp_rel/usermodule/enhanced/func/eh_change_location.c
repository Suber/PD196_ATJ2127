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
 * \brief    转换成播放列表location
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

#ifdef USE_83NAME_CREAT
static const char plist_lib_name[PL_LIB_MAX][12]=
{   "MUSIC   LIB","AUDIBLE LIB","VIDEO   LIB","PICTURE LIB","EBOOK   LIB","M3U     LIB"};
#else
static const char plist_lib_name[PL_LIB_MAX][12] =
{ "MUSIC.LIB", "AUDIBLE.LIB", "VIDEO.LIB", "PICTURE.LIB", "EBOOK.LIB", "M3U.LIB" };
#endif
//static pdir_layer_t pdir_layer_buffer _BANK_DATA_ATTR_;
//static pfile_offset_t pfile_offset_buffer _BANK_DATA_ATTR_;
handle change_object_handle _BANK_DATA_ATTR_;
uint16 change_file_total _BANK_DATA_ATTR_;

uint8 change_temp_buffer[SECTOR_SIZE] _BANK_DATA_ATTR_;

bool chg_change_filepath_sub(void* source_location,plist_location_t *dest_location,uint8 dest_type) __FAR__;
//id3_type_e chg_change_checktype(uint8 * extname) __FAR__;


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
 * \par  Description:
 *
 * \param[in]   tree_start_offset--tree的起始扇区偏移 0~N
 tree_offset --tree 偏移，0~N

 * \param[out]

 * \return      tree信息结构指针

 * \note
 *******************************************************************************/
plist_tree_t *chg_read_tree_data(uint16 tree_start_offset, uint16 tree_offset)
{
    uint16 buf_offset;
    uint16 sector_offset;

    sector_offset = tree_start_offset + tree_offset / (SECTOR_SIZE / sizeof(plist_tree_t));
    buf_offset = sizeof(plist_tree_t) * (tree_offset % (SECTOR_SIZE / sizeof(plist_tree_t)));

    vfs_file_seek(vfs_mount, sector_offset * SECTOR_SIZE, SEEK_SET, change_object_handle);
    vfs_file_read(vfs_mount, change_temp_buffer, SECTOR_SIZE, change_object_handle);

    return (plist_tree_t *) (change_temp_buffer + buf_offset);
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
uint16 chg_get_object_index(uint16 index_start_offset, uint16 index)
{
    uint16 sector_offset;
    uint16 buf_offset;

    sector_offset = index_start_offset + index / 256;
    buf_offset = (index * 2) % SECTOR_SIZE;

    vfs_file_seek(vfs_mount, sector_offset * SECTOR_SIZE, SEEK_SET, change_object_handle);
    vfs_file_read(vfs_mount, change_temp_buffer, SECTOR_SIZE, change_object_handle);

    return *(uint16 *) (change_temp_buffer + buf_offset);
}

/******************************************************************************/
/*
 * \par  Description:把其他location转成播放列表的location
 *
 * \param[in]    source_path 源location
 * \param[out]   dest_path 转成的playlist 的location
 * \param[in]    source_type 源location类型
 * \param[in]    dest_type  playlist 的列表类型

 * \return        TRUE or FALSE

 * \note
 *******************************************************************************/
bool fsel_change_filepath(void * source_location, plist_location_t *dest_location, fsel_type_e source_type,
        plist_type_e dest_type)
{
    uint16 i;
    bool ret_val;

    fsel_error = FSEL_NO_ERR;

    if ((source_location == NULL) || (dest_location == NULL))
    {
        return FALSE;
    }

    dest_location->disk = *(uint8*) source_location;
    dest_location->list_type = dest_type;

    if (dest_type > PLIST_GENRE)
    {
        i = 1;
        dest_type = (dest_type & 0x0f);
    }
    else
    {
        i = 0;
    }

    //保存路径
    vfs_file_dir_offset(vfs_mount, change_temp_buffer, change_temp_buffer + 128, 0);

    vfs_cd(vfs_mount, CD_ROOT, 0);
    mbyte_to_wchar(change_temp_buffer + 256, (uint8 *) &plist_lib_name[i]);
    change_object_handle = vfs_file_open(vfs_mount, change_temp_buffer + 256, R_NORMAL_SEEK);

    vfs_file_dir_offset(vfs_mount, change_temp_buffer, change_temp_buffer + 128, 1);

    if (0 == change_object_handle)
    {
        return FALSE;
    }

    ret_val = chg_change_filepath_sub(source_location, dest_location, dest_type);

    vfs_file_close(vfs_mount, change_object_handle);

    return ret_val;
}

