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
//static plist_f_info_t *plist_f_info_p _BANK_DATA_ATTR_;
static plist_head_t *plist_head_p _BANK_DATA_ATTR_;
static plist_tree_t *plist_tree_p _BANK_DATA_ATTR_;

extern uint8 makelayer_temp_buffer[SECTOR_SIZE];
extern handle makelayer_object_handle;

//static uint16 change_file_total _BANK_DATA_ATTR_;

extern plist_tree_t *makelyr_read_tree_data(uint16 tree_start_offset,uint16 tree_offset) __FAR__;

static uint16 makelyr_get_list_no(uint16 tree_start_offset, uint16 list_start_offset, uint16 tree_offset);

/******************************************************************************/
/*
 * \par  Description: 配置listlayer结构体信息
 *
 * \param[in] location,dest_type
 * \param[out] layer_buffer

 * \return

 * \note
 *******************************************************************************/
void makelyr_listlayer_info(plist_layer_t *layer_buffer,plist_location_t *location, uint8 dest_type)
{
    uint16 i, temp;
    uint16 temp_start_offset[4];
    uint16 temp_son_offset[4];
        
    vfs_file_read(vfs_mount, makelayer_temp_buffer, SECTOR_SIZE, makelayer_object_handle);
    plist_head_p = (plist_head_t *) makelayer_temp_buffer;

    temp = (uint16) (sizeof(plist_head_t) + dest_type * sizeof(plist_item_t) + 8) % SECTOR_SIZE;

    libc_memcpy(&plist_item_info_tmp, makelayer_temp_buffer + temp, sizeof(plist_item_info_t));

//    change_file_total = plist_head_p->file_total;

//    vfs_file_seek(vfs_mount, SECTOR_SIZE * plist_head_p->file_info_offset, SEEK_SET, makelayer_object_handle);

//    plist_f_info_p = (plist_f_info_t *) makelayer_temp_buffer;
    

    for (i = 0; i < 4; i++)
    {
        //initialize
        layer_buffer[i].num = 0;
        layer_buffer[i].total=0;
    }

    i = location->list_layer&0x03;//0~3
    layer_buffer[i].num = location->file_num;
    
    if ((i == 0) || (0 == dest_type))
    {
        //all song
        layer_buffer[0].total = location->file_total;
        return;
    }
            
    //取出当前最低级list层
    plist_tree_p = makelyr_read_tree_data(plist_item_info_tmp.son_tree_offset[i - 1], location->layer_list_offset);
    layer_buffer[i].total = plist_tree_p->file_total;
    i--;
    temp_son_offset[i] = location->layer_list_offset;
    
    while ((i > 0) && (i < 4))
    {
        //取父级tree
        temp = plist_tree_p->parent_offset;
        plist_tree_p = makelyr_read_tree_data(plist_item_info_tmp.son_tree_offset[i - 1], temp);
        //确定子级的起始位置
        temp_start_offset[i] = plist_tree_p->son_offset;
        layer_buffer[i].total = plist_tree_p->son_num;
        i--;
        temp_son_offset[i] = temp;
    }

    temp_start_offset[0] = 0;
    //统计各级排列序号
    for (i = 0; i < location->list_layer; i++)
    {
        layer_buffer[i].num = makelyr_get_list_no(plist_item_info_tmp.son_tree_offset[i], temp_start_offset[i],
                temp_son_offset[i]);
    }   
    
    layer_buffer[0].total = plist_item_info_tmp.son_tree_num[0];
    
}

/******************************************************************************/
/*
 * \par  Description: 计算偏移tree的有效序号
 *
 * \param[in]tree_start_offset--tree的起始扇区偏移 0~N
 * \param[in]list_start_offset,tree_offset --tree 偏移，0~N

 * \param[out]  
 * \return      有效序号 1~N

 * \note
 *******************************************************************************/
static uint16 makelyr_get_list_no(uint16 tree_start_offset, uint16 list_start_offset, uint16 tree_offset)
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
        plist_tree_p = makelyr_read_tree_data(tree_start_offset, list_start_offset);
        if (plist_tree_p->flag == 1)
        {
            list_no++;
        }

        tree_count--;
        list_start_offset++;
    }

    return list_no;
}


