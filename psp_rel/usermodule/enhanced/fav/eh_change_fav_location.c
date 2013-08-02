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
 * \brief    转换成收藏夹location
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

static uint8 change_fav_buffer[SECTOR_SIZE] _BANK_DATA_ATTR_;

#define MUSIC_EXT_NUM 13

//定义const data 所有后缀数据
static const uint8 support_ext[][4] =
{ "MP1", "MP2", "MP3", "WMA", "WMV", "ASF", "APE", "FLA", "OGG", "AAC", "AAX", "M4A", "AA " };

static const id3_type_e support_id3_type[] =
{ ID3_TYPE_MP3, ID3_TYPE_MP3, ID3_TYPE_MP3, ID3_TYPE_WMA, ID3_TYPE_WMA, ID3_TYPE_WMA, ID3_TYPE_APE, ID3_TYPE_FLAC,
        ID3_TYPE_OGG, ID3_TYPE_AAC, ID3_TYPE_AAX, ID3_TYPE_AAX, ID3_TYPE_AA };
/******************************************************************************/
/*!
 * \par  Description:
 * \id3_type_e eh_change_checktype(char * extname)
 * \获取后缀名
 * \param[in]    extname后缀名buffer  para1
 * \param[in]
 * \param[out]   none
 * \return       void the result
 * \retval       none
 * \retval       none
 * \ingroup      music_playing_getid3.c
 * \note
 */
/*******************************************************************************/
static id3_type_e chg_change_checktype(uint8 * extname)
{
    uint8 cnt;
    id3_type_e ret;
    //转换当前后缀名(转成大写)
    extname[0] = ((extname[0] >= 'a') && (extname[0] <= 'z')) ? (extname[0] - 'a' + 'A') : extname[0];
    extname[1] = ((extname[1] >= 'a') && (extname[1] <= 'z')) ? (extname[1] - 'a' + 'A') : extname[1];
    extname[2] = ((extname[2] >= 'a') && (extname[2] <= 'z')) ? (extname[2] - 'a' + 'A') : extname[2];
    //比较后缀
    for (cnt = 0; cnt < MUSIC_EXT_NUM; cnt++)
    {
        if (libc_strncmp(extname, support_ext[cnt], sizeof(support_ext[cnt])) == 0)
        {
            ret = support_id3_type[cnt];
            break;
        }
    }
    if (cnt == MUSIC_EXT_NUM)
    {
        ret = ID3_TYPE_END;
    }
    return ret;
}

/******************************************************************************/
/*
 * \par  Description:把其他location转成收藏夹的location
 *
 * \param[in]    source_location 源location
 * \param[out]   dest_location 收藏夹的location
 * \param[in]    source_type 源location类型
 * \param[in]    id3_info_buf ID3信息结构

 * \return        TRUE or FALSE

 * \note
 *******************************************************************************/
bool fsel_change_fav_location(void *source_location, flist_location_t *dest_location, fsel_type_e source_type,
        id3_info_t *id3_info_buf)
{
    uint8 type_case;
    id3_type_e file_id3_type;

    bool ret_val = TRUE;

    if ((source_location == NULL) || (dest_location == NULL) || (id3_info_buf == NULL))
    {
        return FALSE;
    }

    type_case = 1;
    switch (source_type)
    {
        case FSEL_TYPE_LISTFAVOR1:
        case FSEL_TYPE_LISTFAVOR2:
        case FSEL_TYPE_LISTFAVOR3:
        {
            type_case = 0;
            libc_memcpy(dest_location, source_location, sizeof(flist_location_t));
            break;
        }

        case FSEL_TYPE_COMMONDIR:
        case FSEL_TYPE_DISKSEQUNCE:
        {
            file_location_t *location_p;
            location_p = source_location;
            libc_memcpy(&dest_location->dir_layer_info, &location_p->dir_layer_info, sizeof(pdir_layer_t));
            break;
        }

        default:
        {
            plist_location_t *location_p;
            location_p = source_location;
            libc_memcpy(&dest_location->dir_layer_info, &location_p->dir_layer_info, sizeof(pdir_layer_t));
            break;
        }

    }

    if (0 == type_case)
    {
        return TRUE;
    }

    libc_memcpy(dest_location, source_location, 22);

    //取标题信息
    id3_info_buf->tit2_buffer = dest_location->title; //待存文件标题buffer
    id3_info_buf->tpe1_buffer = 0; //待存作者buffer
    id3_info_buf->talb_buffer = 0; //待存专辑buffer
    id3_info_buf->genre_buffer = 0; //待存流派buffer
    id3_info_buf->drm_buffer = 0; //待存drm的buffer
    id3_info_buf->track_buffer = 0; //待存音轨buffer
    id3_info_buf->tit2_length = USERPL_TITLE_MAX; //文件标题长度
    id3_info_buf->tpe1_length = 0; //作者长度
    id3_info_buf->talb_length = 0; //专辑长度
    id3_info_buf->genre_length = 0; //流派长度
    id3_info_buf->drm_length = 0; //drm长度
    id3_info_buf->track_length = 0; //音轨长度
    id3_info_buf->apic_flag = 0; //获取图片偏移标记
    id3_info_buf->ldb_flag = 0; //获取ldb歌词偏移标记


    file_id3_type = chg_change_checktype(dest_location->filename);

    vfs_file_dir_offset(vfs_mount, change_fav_buffer, change_fav_buffer + 128, 0);

    vfs_file_dir_offset(vfs_mount, &dest_location->dir_layer_info, &dest_location->cluster_no, 1);
    vfs_get_name(vfs_mount, dest_location->title, USERPL_TITLE_MAX / 2);

    vfs_file_dir_offset(vfs_mount, change_fav_buffer, change_fav_buffer + 128, 1);

    if (file_id3_type != ID3_TYPE_END)
    {
        //必须定义在堆栈上，因为获取ID3都要使用B1和B2
        pdir_layer_t tmp_pdir_layer_buffer;
        pfile_offset_t tmp_pfile_offset_buffer;

        vfs_file_dir_offset(vfs_mount, &tmp_pdir_layer_buffer, &tmp_pfile_offset_buffer, 0);

        vfs_file_dir_offset(vfs_mount, &dest_location->dir_layer_info, &dest_location->cluster_no, 1);
        ret_val = get_id3_info(id3_info_buf, NULL, file_id3_type);

        vfs_file_dir_offset(vfs_mount, &tmp_pdir_layer_buffer, &tmp_pfile_offset_buffer, 1);
    }

    return ret_val;

}

