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

static void sort_author(char *str_buf);
static void sort_book(char *str_buf);

extern void init_file_index(void) __FAR__;
extern void build_ex_index(uint16 *ex_buf) __FAR__;
extern void tidy_same_table(uint16 *tidy_buf,uint16 *record_buf,uint16 *index_buf\
,uint8 *depend_buf,uint8 cmp_len) __FAR__;
extern void son_sort_index(uint16 *data_buf,uint16 *count_buf,uint16 *depend_buf,uint8 cmp_type) __FAR__;
extern void tidy_son_unit(uint16 *record_buf,uint16 *data_buf,uint16 *same_buf,uint16 *depend_buf) __FAR__;
extern bool creat_index_map(uint16 *map_buf,uint16 *index_buf,uint16 *count_buf) __FAR__;
extern uint16* tidy_son_table(uint16 *data_buf,uint16 *record_buf,uint16 count,uint16 *depend_buf) __FAR__;

extern void list_sort(char *base, uint16 num, uint16 width,void *depond_buf,uint8 cmp_type,uint8 cmp_len) __FAR__;
extern uint16 creat_plist_tree(uint16 *index_buf,uint16 *cur_buf,uint16 *son_buf,uint16 *parent_buf,uint8 type) __FAR__;

/******************************************************************************/
/*
 * \par  Description: 排序book的名称
 *
 * \param[in]         str_buf--排序数据缓存地址

 * \param[out]

 * \return

 * \note             title
 *******************************************************************************/
//audible 第一排序
static void sort_book(char *str_buf)
{
    init_file_index();
    list_sort((char*) file_index_buf, sort_file_total, 2, str_buf, SORT_CMP_STRING, CMPSTR_MAX_LEN1); //64+8k

    //必须在title排序后才能请buffer，因为和title的数据区复用
    libc_memset(sort_index_buf, 0, INDEX_BUFFER_SIZE);

    build_ex_index(sort_index_buf); //8+8k

    ap_file_index_offset[0] = ap_pos_file_offset;
    save_to_file((uint8*) file_index_buf, sort_file_total * 2);

    ap_sort_index_offset[0] = ap_pos_file_offset;
    save_to_file((uint8*) sort_index_buf, sort_file_total * 2);

    return;
}
/******************************************************************************/
/*
 * \par  Description: 排序作者
 *
 * \param[in]       str_buf--排序数据缓存地址

 * \param[out]

 * \return

 * \note           author->title
 *******************************************************************************/
//audible 第二排序

static void sort_author(char *str_buf)
{
    init_file_index();
    list_sort((char*) file_index_buf, sort_file_total, 2, str_buf, SORT_CMP_STRING, CMPSTR_MAX_LEN2); //64+8k

    plist_get_app_msg();

    tidy_same_table(tidy_buf_a, count_buf_a, file_index_buf, str_buf, CMPSTR_MAX_LEN2);//8+8+8
    son_sort_index(file_index_buf, count_buf_a, sort_index_buf, SORT_CMP_WORD); //8+8+8+8

    ap_son_tree_offset[2][0] = ap_pos_file_offset;
    ap_son_tree_num[2][0] = creat_plist_tree(file_index_buf, count_buf_a, NULL, NULL, PLIST_AUTHOR);

    build_ex_index(sort_index_buf);//8+8+8+8

    plist_get_app_msg();

    index_parent_buf = (uint16*) COUNT_BUF_B_ADDR;//空间复用，注意

    creat_index_map(index_parent_buf, file_index_buf, count_buf_a);

    ap_file_index_offset[2] = ap_pos_file_offset;
    save_to_file((uint8*) file_index_buf, sort_file_total * 2);

    ap_sort_index_offset[2] = ap_pos_file_offset;
    save_to_file((uint8*) sort_index_buf, sort_file_total * 2);

    ap_index_parent_offset[2] = ap_pos_file_offset;
    save_to_file((uint8*) index_parent_buf, sort_file_total * 2);

    return;
}

/******************************************************************************/
/*
 * \par  Description: 排序列表
 *
 * \param[in]

 * \param[out]

 * \return

 * \note
 *******************************************************************************/
void sort_audible_list(void)
{
    char *str_buf;

    plist_get_app_msg();

    //clear buffer
    libc_memset(file_index_buf, 0, FILE_INFO_BUF_SIZE);

    sort_file_total = ap_plist_file_total;

    if (sort_file_total > PLIST_SAVE_MAX)
    {
        str_buf = (char*) (TRACK_DATA_ADDR - TITLE_VRAM_SIZE);//64k;
        read_from_vram(str_buf, (uint8 *) TITLE_VRAM_ADDR, sort_file_total * TITLE_CMP_LENGTH);
    }
    else
    {
        str_buf = (char*) TITLE_DATA_ADDR;
    }

    sort_book(str_buf);

    plist_get_app_msg();

    if (sort_file_total > PLIST_SAVE_MAX)
    {
        str_buf = (char*) (TRACK_DATA_ADDR - ARTIST_VRAM_SIZE);//32k;
        read_from_vram(str_buf, (uint8 *) ARTIST_VRAM_ADDR, sort_file_total * ARTIST_CMP_LENGTH);
    }
    else
    {
        str_buf = (char*) ARTIST_DATA_ADDR;
    }

    sort_author(str_buf);

    plist_get_app_msg();
    return;
}

