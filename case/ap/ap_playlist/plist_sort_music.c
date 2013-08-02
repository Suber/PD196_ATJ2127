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

static void sort_album(char *str_buf);
static void sort_genre(char *str_buf);
static void sort_artist(char *str_buf);
static void sort_title(char *str_buf);

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
 * \par  Description: 排序title 列表
 *
 * \param[in]    str_buf--排序数据缓存地址

 * \param[out]

 * \return

 * \note
 *******************************************************************************/
//music 第一排序
static void sort_title(char *str_buf)
{
    init_file_index();

#ifdef _VC_DEBUG
    list_printf_debug(str_buf,file_index_buf,sort_file_total,CMPSTR_MAX_LEN1);
#endif

    list_sort((char*) file_index_buf, sort_file_total, 2, str_buf, SORT_CMP_STRING, CMPSTR_MAX_LEN1); //64+8k

#ifdef _VC_DEBUG
    list_printf_debug(str_buf,file_index_buf,sort_file_total,CMPSTR_MAX_LEN1);
#endif

    plist_get_app_msg();

    //必须在title排序后才能请buffer，因为和title的数据区复用
    libc_memset(sort_index_buf, 0, INDEX_BUFFER_SIZE);

    build_ex_index(sort_index_buf); //8+8k

    ap_file_index_offset[0] = ap_pos_file_offset;
    save_to_file((uint8*) file_index_buf, sort_file_total * 2);

    ap_sort_index_offset[0] = ap_pos_file_offset;
    save_to_file((uint8*) sort_index_buf, sort_file_total * 2);

    //根据需求是否存入VRAM
    //    write_to_vram((uint8*)TITLE_SORT_INDEX_VM_ADDR,(uint8*)sort_index_buf,sort_file_total*2);

    return;
}

/******************************************************************************/
/*
 * \par  Description: 排序 album 列表
 *
 * \param[in]    str_buf--排序数据缓存地址

 * \param[out]

 * \return

 * \note
 *******************************************************************************/
//music 第二排序
//album->title
static void sort_album(char *str_buf)
{
    init_file_index();

#ifdef _VC_DEBUG
    list_printf_debug(str_buf,file_index_buf,sort_file_total,CMPSTR_MAX_LEN2);
#endif
    list_sort((char*) file_index_buf, sort_file_total, 2, str_buf, SORT_CMP_STRING, CMPSTR_MAX_LEN2); //64+8k
#ifdef _VC_DEBUG
    list_printf_debug(str_buf,file_index_buf,sort_file_total,CMPSTR_MAX_LEN2);
#endif

    plist_get_app_msg();

    tidy_same_table(tidy_buf_a, count_buf_a, file_index_buf, str_buf, CMPSTR_MAX_LEN2);//8+8+8
    son_sort_index(file_index_buf, count_buf_a, sort_index_buf, SORT_CMP_ALBUM); //8+8+8+8

    ap_son_tree_offset[1][0] = ap_pos_file_offset;
    ap_son_tree_num[1][0] = creat_plist_tree(file_index_buf, count_buf_a, NULL, NULL, PLIST_ALBUM);

    plist_get_app_msg();

    build_ex_index(sort_index_buf);//8+8+8+8

    index_parent_buf = (uint16*) COUNT_BUF_B_ADDR;//空间复用，注意
    
    creat_index_map(index_parent_buf, file_index_buf, count_buf_a);

    plist_get_app_msg();

    ap_file_index_offset[1] = ap_pos_file_offset;
    save_to_file((uint8*) file_index_buf, sort_file_total * 2);

    ap_sort_index_offset[1] = ap_pos_file_offset;
    save_to_file((uint8*) sort_index_buf, sort_file_total * 2);

    ap_index_parent_offset[1] = ap_pos_file_offset;
    save_to_file((uint8*) index_parent_buf, sort_file_total * 2);

    //根据需求是否存入VRAM
#if(GENRE_LIST_MODE==1)
    write_to_vram((uint8*)ALBUM_SORT_INDEX_VM_ADDR,(uint8*)sort_index_buf,sort_file_total*2);
#endif

    return;
}

/******************************************************************************/
/*
 * \par  Description: 排序 artist 列表
 *
 * \param[in]    str_buf--排序数据缓存地址

 * \param[out]

 * \return

 * \note
 *******************************************************************************/
//music 第三排序
//artist->album->title
static void sort_artist(char *str_buf)
{
    init_file_index();
#ifdef _VC_DEBUG
    list_printf_debug(str_buf,file_index_buf,sort_file_total,CMPSTR_MAX_LEN2);
#endif
    list_sort((char*) file_index_buf, sort_file_total, 2, str_buf, SORT_CMP_STRING, CMPSTR_MAX_LEN2); //64+8k
#ifdef _VC_DEBUG
    list_printf_debug(str_buf,file_index_buf,sort_file_total,CMPSTR_MAX_LEN2);
#endif

    plist_get_app_msg();

    tidy_same_table(tidy_buf_b, count_buf_a, file_index_buf, str_buf, CMPSTR_MAX_LEN2);//8+8+8
    son_sort_index(file_index_buf, count_buf_a, sort_index_buf, SORT_CMP_WORD);//8+8+8+8

    tidy_son_unit(count_buf_b, file_index_buf, count_buf_a, tidy_buf_a);//8+8+8+8+8

    plist_get_app_msg();

    build_ex_index(sort_index_buf);

    ap_son_tree_offset[2][0] = ap_pos_file_offset;
    ap_son_tree_num[2][0] = creat_plist_tree(file_index_buf, count_buf_a, count_buf_b, NULL, PLIST_ARTIST);

    ap_son_tree_offset[2][1] = ap_pos_file_offset;
    ap_son_tree_num[2][1] = creat_plist_tree(file_index_buf, count_buf_b, NULL, count_buf_a, PLIST_ALBUM);

    index_parent_buf = (uint16*) COUNT_BUF_A_ADDR; //空间复用，注意
    creat_index_map(index_parent_buf, file_index_buf, count_buf_b);

    ap_file_index_offset[2] = ap_pos_file_offset;
    save_to_file((uint8*) file_index_buf, sort_file_total * 2);

    ap_sort_index_offset[2] = ap_pos_file_offset;
    save_to_file((uint8*) sort_index_buf, sort_file_total * 2);

    ap_index_parent_offset[2] = ap_pos_file_offset;
    save_to_file((uint8*) index_parent_buf, sort_file_total * 2);

    //根据需求是否存入VRAM
    //    write_to_vram((uint8*)ARTIST_SORT_INDEX_VM_ADDR,(uint8*)sort_index_buf,sort_file_total*2);

    return;
}

/******************************************************************************/
/*
 * \par  Description: 排序 genre 列表
 *
 * \param[in]    str_buf--排序数据缓存地址

 * \param[out]

 * \return

 * \note
 *******************************************************************************/
//music 第四排序
//genre->artist->album->title (mode=0)
//or genre->album->title (mode=1)
static void sort_genre(char *str_buf)
{
    init_file_index();
#ifdef _VC_DEBUG
    list_printf_debug(str_buf,file_index_buf,sort_file_total,CMPSTR_MAX_LEN2);
#endif
    list_sort((char*) file_index_buf, sort_file_total, 2, str_buf, SORT_CMP_STRING, CMPSTR_MAX_LEN2); //64+8k
#ifdef _VC_DEBUG
    list_printf_debug(str_buf,file_index_buf,sort_file_total,CMPSTR_MAX_LEN2);
#endif

    plist_get_app_msg();

    tidy_same_table(NULL, count_buf_a, file_index_buf, str_buf, CMPSTR_MAX_LEN2); //8+8+8

#if(GENRE_LIST_MODE==1)
    //read album's sort_index_buf
    read_from_vram((uint8 *)sort_index_buf, (uint8 *)ALBUM_SORT_INDEX_VM_ADDR, sort_file_total*2);
#endif

    son_sort_index(file_index_buf, count_buf_a, sort_index_buf, SORT_CMP_WORD);//8+8+8+8

    plist_get_app_msg();

#if(GENRE_LIST_MODE==1)
    // tidy album
    tidy_son_unit(count_buf_b,file_index_buf,count_buf_a,tidy_buf_a);//8+8+8+8+8
#else
    //tidy artist and album
    tidy_son_unit(count_buf_b, file_index_buf, count_buf_a, tidy_buf_b);//8+8+8+8+8+8
    tidy_son_unit(count_buf_c, file_index_buf, count_buf_b, tidy_buf_a);//8+8+8+8+8
#endif

    plist_get_app_msg();

    ap_son_tree_offset[3][0] = ap_pos_file_offset;
    ap_son_tree_num[3][0] = creat_plist_tree(file_index_buf, count_buf_a, count_buf_b, NULL, PLIST_GENRE);

    index_parent_buf = (uint16*) COUNT_BUF_A_ADDR;//空间复用，注意

#if(GENRE_LIST_MODE==1)
    ap_son_tree_offset[3][1]=ap_pos_file_offset;
    ap_son_tree_num[3][1]=creat_plist_tree(file_index_buf,count_buf_b,NULL,count_buf_a,PLIST_ALBUM);
    creat_index_map(index_parent_buf,file_index_buf,count_buf_b);
#else
    ap_son_tree_offset[3][1] = ap_pos_file_offset;
    ap_son_tree_num[3][1] = creat_plist_tree(file_index_buf, count_buf_b, count_buf_c, count_buf_a, PLIST_ARTIST);
    ap_son_tree_offset[3][2] = ap_pos_file_offset;
    ap_son_tree_num[3][2] = creat_plist_tree(file_index_buf, count_buf_c, NULL, count_buf_b, PLIST_ALBUM);
    creat_index_map(index_parent_buf, file_index_buf, count_buf_c);
#endif

    plist_get_app_msg();

    build_ex_index(sort_index_buf);

    ap_file_index_offset[3] = ap_pos_file_offset;
    save_to_file((uint8*) file_index_buf, sort_file_total * 2);

    ap_sort_index_offset[3] = ap_pos_file_offset;
    save_to_file((uint8*) sort_index_buf, sort_file_total * 2);

    ap_index_parent_offset[3] = ap_pos_file_offset;
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
void sort_music_list(void)
{
    char *str_buf;

    //clear buffer
    libc_memset(file_index_buf, 0, FILE_INFO_BUF_SIZE);

    sort_file_total = ap_plist_file_total;

    plist_get_app_msg();

    if (sort_file_total > PLIST_SAVE_MAX)
    {
        str_buf = (char*) (TRACK_DATA_ADDR - TITLE_VRAM_SIZE);//64k
        read_from_vram(str_buf, (uint8 *) TITLE_VRAM_ADDR, sort_file_total * TITLE_CMP_LENGTH);
    }
    else
    {
        str_buf = (char*) TITLE_DATA_ADDR;
    }

#if 1
    sort_title(str_buf);
#endif

    plist_get_app_msg();

    if (sort_file_total > PLIST_SAVE_MAX)
    {
        str_buf = (char*) (TRACK_DATA_ADDR - ALBUM_VRAM_SIZE);//32k;
        read_from_vram(str_buf, (uint8 *) ALBUM_VRAM_ADDR, sort_file_total * ALBUM_CMP_LENGTH);
        track_buf = (uint8*) (str_buf - TRACK_VRAM_SIZE);//2k
        read_from_vram(track_buf, (uint8 *) ALBUM_VRAM_ADDR, sort_file_total * TRACK_CMP_LENGTH);
    }
    else
    {
        str_buf = (char*) ALBUM_DATA_ADDR;
        track_buf = (uint8*) TRACK_DATA_ADDR;
    }

#if 1
    sort_album(str_buf);
#endif

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

#if 1
    sort_artist(str_buf);
#endif

    plist_get_app_msg();
    if (sort_file_total > PLIST_GENRE_MAX)
    {
        str_buf = (char*) (TRACK_DATA_ADDR - GENRE_VRAM_SIZE);//32k;
        read_from_vram(str_buf, (uint8 *) GENRE_VRAM_ADDR, sort_file_total * GENRE_CMP_LENGTH);
    }
    else
    {
        str_buf = (char*) GENRE_DATA_ADDR;
    }

#if 1
    sort_genre(str_buf);
#endif

    plist_get_app_msg();

    return;
}

//**********************************************


