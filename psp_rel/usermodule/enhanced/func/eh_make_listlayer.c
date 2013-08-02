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

extern void makelyr_listlayer_info(plist_layer_t *layer_buffer,plist_location_t *location, uint8 dest_type) __FAR__;

handle makelayer_object_handle _BANK_DATA_ATTR_;
uint8  makelayer_temp_buffer[SECTOR_SIZE] _BANK_DATA_ATTR_;
static uint16 current_read_offset _BANK_DATA_ATTR_;

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
 * \param[in]   tree_offset --tree 偏移，0~N

 * \param[out]

 * \return      tree信息结构指针

 * \note
 *******************************************************************************/
plist_tree_t *makelyr_read_tree_data(uint16 tree_start_offset, uint16 tree_offset)
{
    uint16 buf_offset;
    uint16 sector_offset;

    sector_offset = tree_start_offset + tree_offset / (SECTOR_SIZE / sizeof(plist_tree_t));
    buf_offset = sizeof(plist_tree_t) * (tree_offset % (SECTOR_SIZE / sizeof(plist_tree_t)));

    if(current_read_offset != sector_offset)
    {
        current_read_offset = sector_offset;
        vfs_file_seek(vfs_mount, sector_offset * SECTOR_SIZE, SEEK_SET, makelayer_object_handle);
        vfs_file_read(vfs_mount, makelayer_temp_buffer, SECTOR_SIZE, makelayer_object_handle);
    }
    return (plist_tree_t *) (makelayer_temp_buffer + buf_offset);
}


/******************************************************************************/
/*
 * \par  Description:根据播放列表的location,生产表项结构
 *
 * \param[in]    location
 * \param[out]   layer_buffer

 * \return        TRUE or FALSE

 * \note
 *******************************************************************************/
bool fsel_make_plistlayer(plist_layer_t *layer_buffer,plist_location_t * location)
{
    uint8 dest_type;
    uint16 i;

    fsel_error = FSEL_NO_ERR;
    current_read_offset = 0xffff;

    if ((layer_buffer == NULL) || (location == NULL))
    {
        return FALSE;
    }

    dest_type = (uint8)location->list_type;
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
    vfs_file_dir_offset(vfs_mount, makelayer_temp_buffer, makelayer_temp_buffer + 128, 0);

    vfs_cd(vfs_mount, CD_ROOT, 0);
    mbyte_to_wchar(makelayer_temp_buffer + 256, (uint8 *) &plist_lib_name[i]);
    makelayer_object_handle = vfs_file_open(vfs_mount, makelayer_temp_buffer + 256, R_NORMAL_SEEK);

    vfs_file_dir_offset(vfs_mount, makelayer_temp_buffer, makelayer_temp_buffer + 128, 1);

    if (0 == makelayer_object_handle)
    {
        return FALSE;
    }
        
    makelyr_listlayer_info(layer_buffer,location, dest_type);

    vfs_file_close(vfs_mount, makelayer_object_handle);
    return TRUE;
}
    
   

