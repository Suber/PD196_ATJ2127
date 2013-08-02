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

static plist_item_info_t plist_item_info_tmp _BANK_DATA_ATTR_;
static plist_f_info_t *plist_f_info_p _BANK_DATA_ATTR_;
static plist_head_t *plist_head_p _BANK_DATA_ATTR_;
static plist_tree_t *plist_tree_p _BANK_DATA_ATTR_;

extern uint8 change_temp_buffer[SECTOR_SIZE];
extern handle change_object_handle;

static uint16 change_file_total _BANK_DATA_ATTR_;

static uint16 chg_get_file_no(uint16 sector_start, uint16 pos_start, uint16 t_file_index);
extern plist_tree_t *chg_read_tree_data(uint16 tree_start_offset,uint16 tree_offset) __FAR__;
extern uint16 chg_get_object_index(uint16 index_start_offset,uint16 index) __FAR__;

static void chg_get_listlayer_info(plist_location_t *dest_location, uint8 dest_type);
static uint16 chg_get_list_no(uint16 tree_start_offset, uint16 list_start_offset, uint16 tree_offset);

//#define MUSIC_EXT_NUM 13
//
////定义const data 所有后缀数据
//static const uint8 support_ext[][4] =
//{
//    "MP1",
//    "MP2",
//    "MP3",
//    "WMA",
//    "WMV",
//    "ASF",
//    "APE",
//    "FLA",
//    "OGG",
//    "AAC",
//    "AAX",
//    "M4A",
//    "AA "
//};
//
//static const id3_type_e support_id3_type[] =
//{
//    ID3_TYPE_MP3,
//    ID3_TYPE_MP3,
//    ID3_TYPE_MP3,
//    ID3_TYPE_WMA,
//    ID3_TYPE_WMA,
//    ID3_TYPE_WMA,
//    ID3_TYPE_APE,
//    ID3_TYPE_FLAC,
//    ID3_TYPE_OGG,
//    ID3_TYPE_AAC,
//    ID3_TYPE_AAX,
//    ID3_TYPE_AAX,
//    ID3_TYPE_AA
//};
///******************************************************************************/
///*!
// * \par  Description:
// * \id3_type_e eh_change_checktype(char * extname)
// * \获取后缀名
// * \param[in]    extname后缀名buffer  para1
// * \param[in]
// * \param[out]   none
// * \return       void the result
// * \retval       none
// * \retval       none
// * \ingroup      music_playing_getid3.c
// * \note
// */
///*******************************************************************************/
//id3_type_e chg_change_checktype(uint8 * extname)
//{
//    uint8 cnt;
//    id3_type_e ret;
//    //转换当前后缀名(转成大写)
//    extname[0] = ((extname[0] >= 'a') && (extname[0] <= 'z')) ? (extname[0] - 'a' + 'A') : extname[0];
//    extname[1] = ((extname[1] >= 'a') && (extname[1] <= 'z')) ? (extname[1] - 'a' + 'A') : extname[1];
//    extname[2] = ((extname[2] >= 'a') && (extname[2] <= 'z')) ? (extname[2] - 'a' + 'A') : extname[2];
//    //比较后缀
//    for (cnt = 0; cnt < MUSIC_EXT_NUM ;cnt++)
//    {
//        if (libc_strncmp(extname, support_ext[cnt], sizeof(support_ext[cnt])) == 0)
//        {
//            ret = support_id3_type[cnt];
//            break;
//        }
//    }
//    if (cnt == MUSIC_EXT_NUM)
//    {
//        ret = ID3_TYPE_END;
//    }
//    return ret;
//}


/******************************************************************************/
/*
 * \par  Description:查文件转成播放列表的location
 *
 * \param[in]
 * \param[out]   dest_path 转成的目的location
 * \param[in]    dest_type 目location的list类型
 * \param[in]

 * \return        TRUE or FALSE

 * \note
 *******************************************************************************/
bool chg_change_filepath_sub(void* source_location, plist_location_t *dest_location, uint8 dest_type)
{
    uint16 i, temp;

    bool ret_val = TRUE;

    vfs_file_read(vfs_mount, change_temp_buffer, SECTOR_SIZE, change_object_handle);
    plist_head_p = (plist_head_t *) change_temp_buffer;

    temp = (uint16) (sizeof(plist_head_t) + dest_type * sizeof(plist_item_t) + 8) % SECTOR_SIZE;

    libc_memcpy(&plist_item_info_tmp, change_temp_buffer + temp, sizeof(plist_item_info_t));

    change_file_total = plist_head_p->file_total;

    vfs_file_seek(vfs_mount, SECTOR_SIZE * plist_head_p->file_info_offset, SEEK_SET, change_object_handle);

    plist_f_info_p = (plist_f_info_t *) change_temp_buffer;

    //查找范围大于实际个数
    //temp为文件个数计数器，i为file_index
    i = 0;
    for (temp = 0; temp < change_file_total;)
    {
        if ((i & 0x01) == 0)
        {
            //一次读两个文件信息出来
            vfs_file_read(vfs_mount, change_temp_buffer, SECTOR_SIZE, change_object_handle);
            plist_f_info_p = (plist_f_info_t *) change_temp_buffer;
        }
        else
        {
            plist_f_info_p = (plist_f_info_t *) &change_temp_buffer[PLIST_FILE_SIZE];
        }
        //有效文件时，比较信息
        if (1 == plist_f_info_p->flag)
        {
            if (0 == libc_memcmp((uint8*) source_location + 8, &plist_f_info_p->cluster_no, 8))
            {
                break;
            }
            temp++;
        }

        i++;

    }

    if (temp < change_file_total)
    {
        dest_location->file_index = i;
        libc_memcpy(&dest_location->cluster_no, &plist_f_info_p->cluster_no, 8);
        libc_memcpy(&dest_location->dir_layer_info, &plist_f_info_p->dir_layer_info, sizeof(pdir_layer_t));
        libc_memcpy(&dest_location->filename, &plist_f_info_p->filename, 4);

        //查找到文件父级偏移
        if (0x00 == dest_type)
        {
            //ALL SONG 没有父级
            temp = 0xffff;
        }
        else
        {
            temp = chg_get_object_index(plist_item_info_tmp.index_parent_offset, i);
        }

        if (temp != 0xffff)
        {
            //确定层次
            for (i = 3; i > 0; i--)
            {
                if (plist_item_info_tmp.son_tree_num[i - 1] > 0)
                {
                    break;
                }
            }
            dest_location->layer_list_offset = temp;
            dest_location->list_layer = (uint8) i;

            //取父级tree
            plist_tree_p = chg_read_tree_data(plist_item_info_tmp.son_tree_offset[(i - 1) % 3], temp);
            dest_location->file_total = plist_tree_p->file_total;

            //获取队列的file_index的起始位置
            i = chg_get_object_index(plist_item_info_tmp.sort_index_offset, plist_tree_p->file_index);
        }
        else
        {
            //all song
            dest_location->list_layer = 0;
            dest_location->file_total = change_file_total;
            dest_location->layer_list_offset = 0;
            i = 0;
        }

        dest_location->file_num = chg_get_file_no(plist_item_info_tmp.file_index_offset, i, dest_location->file_index);

        ret_val = TRUE;
    }
    else
    {
        ret_val = FALSE;
    }
    
//功能已调试成功，暂不启用    
/*
    if (TRUE == ret_val)
    {
        chg_get_listlayer_info(dest_location, dest_type);
    }
*/
    return ret_val;
}

/******************************************************************************/
/*
 * \par  Description: 计算文件的播放序号
 *
 * \param[in]    sector_start---file_index的扇区偏移0~N
 pos_start--播放的起始偏移0~N
 t_file_index--文件的序号0~N

 * \param[out]

 * \return       文件播放序号 1~N ，0为无效

 * \note
 *******************************************************************************/
static uint16 chg_get_file_no(uint16 sector_start, uint16 pos_start, uint16 t_file_index)
{
    uint16 t_file_no;
    uint16 sector_offset;
    uint16 buf_offset;
    uint16 *t_file_no_p;

    sector_offset = sector_start + (pos_start * 2) / SECTOR_SIZE;
    buf_offset = (pos_start * 2) % SECTOR_SIZE;

    vfs_file_seek(vfs_mount, sector_offset * SECTOR_SIZE, SEEK_SET, change_object_handle);
    vfs_file_read(vfs_mount, change_temp_buffer, SECTOR_SIZE, change_object_handle);

    t_file_no_p = (uint16*) (change_temp_buffer + buf_offset);

    for (t_file_no = 0; t_file_no < change_file_total;)
    {
        if (t_file_no_p >= (uint16*) (change_temp_buffer + SECTOR_SIZE))
        {
            vfs_file_read(vfs_mount, change_temp_buffer, SECTOR_SIZE, change_object_handle);
            t_file_no_p = (uint16*) change_temp_buffer;
        }

        //file_index,有效才能比较
        if (*t_file_no_p != PLIST_INVALID_INDEX)
        {
            if (*t_file_no_p == t_file_index)
            {
                return t_file_no + 1;//t_file_no从1开始
            }

            t_file_no++;

        }

        t_file_no_p++;

    }

    return 0;

}

/******************************************************************************/
/*
 * \par  Description: 配置listlayer结构体信息
 *
 * \param[in]
 dest_location
 dest_type

 * \param[out]

 * \return

 * \note
 *******************************************************************************/
static void chg_get_listlayer_info(plist_location_t *dest_location, uint8 dest_type)
{
    uint16 i, temp;
    uint16 temp_start_offset[4];
    uint16 temp_son_offset[4];

    for (i = 0; i < 4; i++)
    {
        //initialize
        dest_location->list_layer_no[i] = 0xffff;
        temp_start_offset[i] = 0;
        temp_son_offset[i] = 0;
    }

    i = dest_location->list_layer;//0~3
    dest_location->list_layer_no[i] = dest_location->file_num;

    if ((i == 0) || (0 == dest_type))
    {
        //all song
        return;
    }

    i--;
    temp_son_offset[i] = dest_location->layer_list_offset;
    //取出当前最低级list层
    plist_tree_p = chg_read_tree_data(plist_item_info_tmp.son_tree_offset[i], temp_son_offset[i]);

    while ((i > 0) && (i < 4))
    {
        //取父级tree
        temp = plist_tree_p->parent_offset;
        plist_tree_p = chg_read_tree_data(plist_item_info_tmp.son_tree_offset[i - 1], temp);
        //确定子级的起始位置
        temp_start_offset[i] = plist_tree_p->son_offset;
        i--;
        temp_son_offset[i] = temp;
    }

    //统计各级排列序号
    for (i = 0; i < dest_location->list_layer; i++)
    {
        dest_location->list_layer_no[i] = chg_get_list_no(plist_item_info_tmp.son_tree_offset[i], temp_start_offset[i],
                temp_son_offset[i]);
    }

/*
    //add for debug

    for (i = 0; i < 4; i++)
    {
        ((void(*)(UINT8 *, UINT32, UINT8)) 0xbfc03d79)(NULL, (UINT32) dest_location->list_layer_no[i], 1);
    }
    //add end
*/    
    
}

/******************************************************************************/
/*
 * \par  Description: 计算偏移tree的有效序号
 *
 * \param[in]

 tree_start_offset--tree的起始扇区偏移 0~N
 list_start_offset,tree_offset --tree 偏移，0~N

 * \param[out]  有效序号 1~N

 * \return

 * \note
 *******************************************************************************/
static uint16 chg_get_list_no(uint16 tree_start_offset, uint16 list_start_offset, uint16 tree_offset)
{
    uint16 list_no, tree_count;

    if (tree_offset < list_start_offset)
    {
        return 1;
    }

    tree_count = tree_offset - list_start_offset + 1;
    list_no = 0;

    while (tree_count > 0)
    {
        plist_tree_p = chg_read_tree_data(tree_start_offset, list_start_offset);
        if (plist_tree_p->flag == 1)
        {
            list_no++;
        }

        tree_count--;
        list_start_offset++;
    }

    return list_no;
}

