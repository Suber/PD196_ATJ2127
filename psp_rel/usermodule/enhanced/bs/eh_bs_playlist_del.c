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
 * \file     eh_bs_playlist_del.c
 * \brief    播放列表浏览
 * \author   liminxian
 * \par      GENERAL DESCRIPTION:
 *               这里对文件进行描述
 * \par      EXTERNALIZED FUNCTIONS:
 *               文件系统，VRAM
 * \version 1.0
 * \date  2011/9/15
 *******************************************************************************/

#include "eh_bs.h"

//保证使用以下变量时，没有发生切bank
static uint16 del_file_index _BANK_DATA_ATTR_;
static uint16 next_file_index _BANK_DATA_ATTR_;
static bool del_tree_flag _BANK_DATA_ATTR_;

static plist_item_info_t temp_plist_item_info _BANK_DATA_ATTR_;

extern uint8 bs_temp_buf[SECTOR_SIZE];

extern bool bs_read_sector_data(uint16 sector_offset) __FAR__;

extern uint16 bs_get_file_index_byno(uint16 sector_start,uint16 index_offset,uint16 t_file_no) __FAR__; //切bank
/******************************************************************************/
/*
 * \par  Description:获取playlist里的文件结构信息
 *
 * \param[in]    file_offset 文件的序号0~N

 * \param[out]

 * \return       文件信息结构

 * \note
 *******************************************************************************/
static plist_f_info_t * bs_plist_get_file_info(uint16 file_offset)
{
    uint16 buf_offset;
    uint16 sector_offset; //第一个扇区为头

    sector_offset = plist_info_offset + file_offset / (SECTOR_SIZE / PLIST_FILE_SIZE);
    buf_offset = (file_offset * PLIST_FILE_SIZE) % SECTOR_SIZE;

    bs_read_sector_data(sector_offset);

    return (plist_f_info_t*) (bs_temp_buf + buf_offset);
}

/******************************************************************************/
/*
 * \par  Description: 获取指定的TREE信息
 *
 * \param[in]    tree_start_offset--tree存放的扇区偏移(0~N)
 tree_offset--读取的tree位置偏移(0~N)

 * \param[out]

 * \return       TREE信息

 * \note
 *******************************************************************************/
plist_tree_t *bs_plist_read_tree_data(uint16 tree_start_offset, uint16 tree_offset)
{
    uint16 buf_offset;
    uint16 sector_offset;

    sector_offset = tree_start_offset + (tree_offset * sizeof(plist_tree_t)) / SECTOR_SIZE;
    buf_offset = (tree_offset * sizeof(plist_tree_t)) % SECTOR_SIZE;
    bs_read_sector_data(sector_offset);

    return (plist_tree_t*) (bs_temp_buf + buf_offset);
}

/******************************************************************************/
/*
 * \par  Description: 获取类型ITEM信息
 *
 * \param[in]       type--指定类型

 * \param[out]      item_info--返回

 * \return          TRUE OR FALSE

 * \note
 *******************************************************************************/
static bool bs_plist_get_item_info(plist_item_info_t *item_info, plist_type_e type)
{
    plist_item_t *plist_item_p;
    bs_read_sector_data(0);
    plist_item_p = (plist_item_t*) (bs_temp_buf + sizeof(plist_head_t));
    plist_item_p += ((uint8) type & 0x0f);
    libc_memcpy(item_info, &plist_item_p->son_tree_num, sizeof(plist_item_info_t));
    return TRUE;

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
static uint16 bs_plist_get_object_index(uint16 index_start_offset, uint16 index)
{
    uint16 sector_offset;
    uint16 buf_offset;

    sector_offset = index_start_offset + index / 256;
    buf_offset = (index * 2) % SECTOR_SIZE;
    bs_read_sector_data(sector_offset);

    return *(uint16 *) (bs_temp_buf + buf_offset);
}

/******************************************************************************/
/*
 * \par  Description: 删除指定的index，并返回删除前的index
 *
 * \param[in]    index_start_offset--index的起始扇区偏移0~N
 index--位置偏移0~N

 * \param[out]

 * \return       index 0~N

 * \note
 *******************************************************************************/
//返回删除前的index
static uint16 bs_plist_del_object_index(uint16 index_start_offset, uint16 index)
{
    uint16 sector_offset;
    uint16 buf_offset;
    uint16 object_index;

    sector_offset = index_start_offset + (index << 1) / SECTOR_SIZE;
    buf_offset = (index << 1) % SECTOR_SIZE;

    bs_read_sector_data(sector_offset);
    object_index = *(uint16 *) (bs_temp_buf + buf_offset);
    *(uint16 *) (bs_temp_buf + buf_offset) = PLIST_INVALID_INDEX;

    return object_index;
}

/******************************************************************************/
/*
 * \par  Description: 删除指定tree，返回父tree的偏移
 *
 * \param[in]    tree_start_offset--tree存放的起始扇区偏移(0~N)
 tree_offset--当前起始的tree的位置偏移(0~N)
 type--指定类型
 * \param[out]

 * \return       父tree的偏移

 * \note
 *******************************************************************************/
//返回object 的tree 偏移
//TYPE=1，返回父的偏移，TYPE=0，返回子的偏移
static uint16 bs_plist_del_object_tree(uint16 tree_start_offset, uint16 tree_offset, uint8 type)
{
    uint16 t_object_offset;
    uint16 t_prev_offset;
    uint16 t_next_offset;
    //    uint16 sector_offset;
    plist_tree_t *temp_tree;

    temp_tree = bs_plist_read_tree_data(tree_start_offset, tree_offset);

    if (temp_tree->flag == 1)
    {
        if (type == 1)
        {
            t_object_offset = temp_tree->parent_offset;
        }
        else
        {
            t_object_offset = temp_tree->son_offset;
        }

        if (temp_tree->file_total == 1)
        {
            //删除整个tree
            temp_tree->flag = 2;
            t_prev_offset = temp_tree->prev_offset;
            t_next_offset = temp_tree->next_offset;

            if (t_prev_offset != 0xffff)
            {
                temp_tree = bs_plist_read_tree_data(tree_start_offset, t_prev_offset);
                temp_tree->next_offset = t_next_offset;
            }
            if (t_next_offset != 0xffff)
            {
                temp_tree = bs_plist_read_tree_data(tree_start_offset, t_next_offset);
                temp_tree->prev_offset = t_prev_offset;
            }

            del_tree_flag = TRUE;
        }
        else
        {
            //只做个数减1
            temp_tree->file_total--;
            if (temp_tree->file_index == del_file_index)
            {
                //若删除的是第一个，替换file_index
                temp_tree->file_index = next_file_index;
            }
            if (TRUE == del_tree_flag)// &&temp_tree->son_num>0
            {
                //若下级已删除tree,上级son_num的个数减一
                temp_tree->son_num--;
            }

            del_tree_flag = FALSE;
        }
        //        }
    }
    else
    {
        t_object_offset = PLIST_INVALID_INDEX;
    }

    return t_object_offset;

}

/******************************************************************************/
/*
 * \par  Description: 根据显示序号，获取文件的index
 *
 * \param[in]    index_start_offset--index 的存放起始扇区偏移(0~N)
 start_index--起始播放号偏移(0~N)
 t_file_no--文件的file_no(1~N)
 * \param[out]

 * \return       index(0~N)

 * \note
 *******************************************************************************/
//get index
//static uint16 bs_plist_get_index_byno(uint16 index_start_offset,uint16 start_index,uint16 t_file_no)
//{
//    uint16 sector_offset;
//    uint16 buf_offset;
//    uint16 *buf_pt;
//
//    sector_offset =index_start_offset + (start_index<<1)/SECTOR_SIZE;
//    buf_offset=(start_index<<1)/SECTOR_SIZE;
//
//    buf_pt=(uint16*)(bs_temp_buf+buf_offset);
//    bs_read_sector_data(sector_offset);
//
//    while(0<t_file_no--)
//    {
//
//        while(1)
//        {
//            if(buf_pt>=(uint16 *)(bs_temp_buf+SECTOR_SIZE))
//            {
//                sector_offset++;
//                bs_read_sector_data(sector_offset);
//                buf_pt=(uint16 *)bs_temp_buf;
//            }
//            if(*buf_pt!=PLIST_INVALID_INDEX)
//            {
//                break;
//            }
//            buf_pt++;
//
//        }
//        buf_pt++;
//
//    }
//
//    return  *(buf_pt-1);
//}

/******************************************************************************/
/*
 * \par  Description: 判断文件是否已在列表上
 *
 * \param[in]    file_dir_info--文件的目录项位置信息

 * \param[out]

 * \return       TRUE OR FALSE

 * \note        找到赋值del_file_index
 *******************************************************************************/
static bool bs_plist_judge_exist(uint8 *file_dir_info)
{
    uint16 temp, temp_index;
    plist_f_info_t *plist_f_info_p;
    bool ret_val = TRUE;

    del_file_index = PLIST_INVALID_INDEX;
    if (0 == plist_file_total)
    {
        return FALSE;
    }
    temp = 0;
    temp_index = 0;

    //temp 有效个数
    //temp_index file_index  少于4000
    while ((temp < plist_file_total) && (temp_index < 4000))
    {
        plist_f_info_p = bs_plist_get_file_info(temp_index); //
        if (1 == plist_f_info_p->flag)
        {

            if (0 == libc_memcmp(file_dir_info, &plist_f_info_p->cluster_no, 8))
            {
                del_file_index = temp_index;
                break;
            }
            else
            {
                temp++;
            }
        }
        temp_index++;
    }

    if ((temp == plist_file_total) || (temp_index == 4000))
    {

        ret_val = FALSE;
    }

    return ret_val;
}

/******************************************************************************/
/*
 * \par  Description: 更新item到列表文件中
 *
 * \param[in]    item_info--item信息
 type--指定类型
 * \param[out]

 * \return       TRUE OR FALSE

 * \note
 *******************************************************************************/
static bool bs_plist_update_item_info(plist_item_info_t *item_info, plist_type_e type)
{
    plist_item_t *plist_item_p;

    bs_read_sector_data(0);
    plist_item_p = (plist_item_t *) (bs_temp_buf + sizeof(plist_head_t));
    plist_item_p += ((uint8) type & 0x0f);
    libc_memcpy(&plist_item_p->son_tree_num, item_info, sizeof(plist_item_info_t));

    return TRUE;

}
/******************************************************************************/
/*
 * \par  Description: 根据file_index删除列表中某个文件
 *
 * \param[in]    t_file_index (0~N)

 * \param[out]

 * \return       TRUE OR FALSE

 * \note
 *******************************************************************************/
//删除列表中的一个文件，更新列表
static bool bs_plist_del_by_index(uint16 t_file_index)
{
    uint8 i, count;
    uint16 j;
    uint16 t_sort_index;
    uint16 t_index_parent;
    plist_f_info_t *plist_f_p;
    plist_head_t * plist_head_p;

    plist_f_p = bs_plist_get_file_info(t_file_index);

    plist_f_p->flag = 2;

    i = 0;
    count = 4;

    while (count > 0)
    {
        count--;
        bs_plist_get_item_info(&temp_plist_item_info, i);

        if (temp_plist_item_info.file_index_offset == 0x0000)
        {
            //picture，vedio，ebook会存在为0x00
            i++;
            continue;//break;
            //此处不能使用break，因为audible表有title 和artist
        }

        t_sort_index = bs_plist_del_object_index(temp_plist_item_info.sort_index_offset, t_file_index);

        next_file_index = PLIST_INVALID_INDEX;
        j = 1;
        while (next_file_index == PLIST_INVALID_INDEX)
        {
            next_file_index = bs_plist_get_object_index(temp_plist_item_info.file_index_offset, t_sort_index + j);
            j++;
        }

        bs_plist_del_object_index(temp_plist_item_info.file_index_offset, t_sort_index);

        if (temp_plist_item_info.index_parent_offset != 0x00)
        {
            t_index_parent = bs_plist_del_object_index(temp_plist_item_info.index_parent_offset, t_file_index);
        }
        else
        {
            t_index_parent = PLIST_INVALID_INDEX;
        }

        if (t_index_parent == PLIST_INVALID_INDEX)
        {
            i++;
            continue;
        }

        j = 3;
        del_tree_flag = FALSE;
        while (j > 0)
        {
            j--;
            if (temp_plist_item_info.son_tree_num[j] > 0)
            {
                t_index_parent = bs_plist_del_object_tree(temp_plist_item_info.son_tree_offset[j], t_index_parent, 1);

                if (TRUE == del_tree_flag)
                {
                    temp_plist_item_info.son_tree_num[j]--;
                }

                if (t_index_parent == PLIST_INVALID_INDEX)
                {
                    break;
                }
            }

        }

        bs_plist_update_item_info(&temp_plist_item_info, i);
        i++;
    }

    //更新文件总数
    bs_read_sector_data(0);
    plist_head_p = (plist_head_t *) bs_temp_buf;
    plist_file_total--;
    plist_head_p->file_total = plist_file_total;
    bs_read_sector_data(1); //updata 0 sector

    return TRUE;
}

/******************************************************************************/
/*
 * \par  Description: 把选中的文件从列表中删除(公共接口)
 *
 * \param[in]        type--删除类型，(只支持删除单个文件)
 list_no--选中序号(1~N)
 file_dir_info--文件的目录项位置信息(输入or输出)
 * \param[out]

 * \return           TRUE OR FALSE

 * \note            若list_no不为0，使用list_no为依据删除，file_dir_info 为输出
 若list_no为0，使用file_dir_info来删除文件，file_dir_info 为输入
 删除使用变量plist_file_total
 *******************************************************************************/
bool bs_plist_delete_func(brow_del_e type, uint16 list_no, uint8 *file_dir_info)
{
    bool ret_val = TRUE;

    save_read_offset = 0xffff;

    if (0 != list_no)
    {
        plist_f_info_t * tem_plist_f_p;
        //减少代码空间做成要切bank
        del_file_index = bs_get_file_index_byno(plist_item_info.file_index_offset, plist_start_sort_index, list_no);
        //      del_file_index=bs_plist_get_index_byno(plist_item_info.file_index_offset,plist_start_sort_index,list_no);

        if (NULL != file_dir_info)
        {
            tem_plist_f_p = bs_plist_get_file_info(del_file_index);
            libc_memcpy(file_dir_info, &tem_plist_f_p->cluster_no, 8);
        }

    }
    else
    {
        if (NULL == file_dir_info)
        {
            ret_val = FALSE;
        }
        else
        {
            //找到会赋值del_file_index
            ret_val = bs_plist_judge_exist(file_dir_info);
        }

    }

    //ADD CODE
    if (FALSE != ret_val)
    {
        write_data_flag = TRUE;
        ret_val = bs_plist_del_by_index(del_file_index);
        write_data_flag = FALSE;
    }

    if (FALSE != ret_val)
    {
        if ((obj_open_type <= FSEL_TYPE_LISTEBOOK) && (fsel_init_val.disk == DISK_C))//只有删除nand才更新总数
        {
            //更新main_menu的显示个数

            //#define  VM_FILE_NUM     0x00003800
            uint16* tmp_cp;
            tmp_cp = (uint16*) bs_temp_buf;
            sys_vm_read(bs_temp_buf, VM_MAINMENU_CNT_INFO, SECTOR_SIZE); //
            tmp_cp[(obj_open_type - FSEL_TYPE_PLAYLIST) % MAX_APP_NUM] = plist_file_total;
            sys_vm_write(bs_temp_buf, VM_MAINMENU_CNT_INFO);

        }
    }

    return ret_val;
}

