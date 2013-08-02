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

void init_file_index(void);
void build_ex_index(uint16 *ex_buf);
void tidy_same_table(uint16 *tidy_buf, uint16 *record_buf, uint16 *index_buf, uint8 *depend_buf, uint8 cmp_len);
void son_sort_index(uint16 *data_buf, uint16 *count_buf, uint16 *depend_buf, uint8 cmp_type);
void tidy_son_unit(uint16 *record_buf, uint16 *data_buf, uint16 *same_buf, uint16 *depend_buf);
bool creat_index_map(uint16 *map_buf, uint16 *index_buf, uint16 *count_buf);
uint16* tidy_son_table(uint16 *data_buf, uint16 *record_buf, uint16 count, uint16 *depend_buf);

extern signed char compare_string(uint16 *data1,uint16 *data2,uint8 len) __FAR__;
extern signed char compare_word(uint16 *data1,uint16 *data2,uint8 len) __FAR__;
extern signed char compare_album_track(uint16 *data1,uint16 *data2,uint8 len) __FAR__;
extern void list_sort(char *base, uint16 num, uint16 width,void *depond_buf,uint8 cmp_type,uint8 cmp_len) __FAR__;

/******************************************************************************/
/*
 * \par  Description: 初始化索引缓存
 *
 * \param[in]

 * \param[out]

 * \return

 * \note
 *******************************************************************************/
void init_file_index(void)
{
    uint16 i = 0;

    while (i < sort_file_total)
    {
        file_index_buf[i] = i;
        i++;
    }
}

/******************************************************************************/
/*
 * \par  Description:查找相同的，整理成个数表
 *

 * \param[out]   tidy_buf   --object 分类编号
 * \param[out]   record_buf --save the same object 's number
 * \param[in]    index_buf  --file_index,save file's serial
 * \param[in]    depend_buf --compare data
 * \param[in]    cmp_len  --compare data's length (unit:8bit)

 * \return

 * \note
 *******************************************************************************/
void tidy_same_table(uint16 *tidy_buf, uint16 *record_buf, uint16 *index_buf, uint8 *depend_buf, uint8 cmp_len)
{
    uint16 i;
    uint16 j, n;

    uint16 *same_p;
    //    uint16 *data_p;
    uint16 *index_p;

    index_p = index_buf;
    same_p = record_buf;
    sort_depend_buf = depend_buf;

    libc_memset(same_p, 0x00, sort_file_total * 2 + 2);

    j = 0;
    n = 1;
    *same_p = n;
    if (tidy_buf != NULL)
    {
        tidy_buf[index_p[0]] = j;
    }

    for (i = 1; i < sort_file_total; i++)
    {
        if (0 != compare_string(&index_p[i], &index_p[i - 1], cmp_len))
        {
            //不相等
            j++;
            n = 1;
            same_p++;
        }
        else
        {
            n++;
        }

        if (tidy_buf != NULL)
        {
            tidy_buf[index_p[i]] = j;
        }

        *same_p = n;
    }
}
/******************************************************************************/
/*
 * \par  Description:整理指定长度的index,统计相同的个数,返回存放个数指针位置
 *
 * \param[in]    data_buf ---排序后的file_index
 * \param[out]   record_buf ---统计个数存放表
 * \param[in]    count         ---子个数
 * \param[in]    depend_buf ---统计依赖的数据来源

 * \return       当前写入的地址

 * \note
 *******************************************************************************/
uint16* tidy_son_table(uint16 *data_buf, uint16 *record_buf, uint16 count, uint16 *depend_buf)
{
    uint16 i;
    uint16 n;

    uint16 *same_p;

    sort_depend_buf = (void*) depend_buf;
    same_p = record_buf;

    n = 1;
    *same_p = n;

    for (i = 1; i < count; i++)
    {
        if (0 != compare_word(data_buf + i, data_buf + i - 1, 2))
        {
            //不相等
            n = 1;
            same_p++;
        }
        else
        {
            n++;
        }
        *same_p = n;
    }

    same_p++;

    return same_p;

}

/******************************************************************************/
/*
 * \par  Description: 生成file_index的反向映射表
 *
 * \param[in]

 * \param[out]        ex_buf---反向影射表

 * \return

 * \note
 *******************************************************************************/
//生成存sort_index
void build_ex_index(uint16 *ex_buf)
{
    uint16 i;
    for (i = 0; i < sort_file_total; i++)
    {
        ex_buf[file_index_buf[i]] = i;
    }
}
/******************************************************************************/
/*
 * \par  Description: 排序列表的子级
 *
 * \param[in][out]   data_buf ---file_index,排序列
 * \param[in]        count_buf ---统计子级的相同个数
 * \param[in]        depend_buf ---依赖的比较数据
 * \param[in]        cmp_type  ---比较的类型

 * \return

 * \note
 *******************************************************************************/
//排序子级
void son_sort_index(uint16 *data_buf, uint16 *count_buf, uint16 *depend_buf, uint8 cmp_type)
{
    uint16 *same_p, *data_p;
    uint16 i, count;
    same_p = count_buf;
    data_p = data_buf;

    for (i = 0; *same_p > 0;)
    {
        if (*same_p > 1)
        {
            count = *same_p;
            list_sort((void*) (data_buf + i), count, 2, depend_buf, cmp_type, 2);
            i = i + count;
        }
        else
        {
            data_p++;
            i++;
        }
        same_p++;
    }

}
/******************************************************************************/
/*
 * \par  Description:由父级的个数，分段统计子级相同的个数
 *
 * \param[out]   record_buf   ---记录统计的子级相同个数表
 * \param[in]    data_buf   ---排序好的file_index
 * \param[in]    same_buf   ---父级相同个数表
 * \param[in]    depend_buf ---统计依赖的数据来源

 * \return

 * \note
 *******************************************************************************/
void tidy_son_unit(uint16 *record_buf, uint16 *data_buf, uint16 *same_buf, uint16 *depend_buf)
{
    uint16 *same_p, *record_p, *data_p;
    uint16 i, count;
    same_p = same_buf;
    data_p = data_buf;
    record_p = record_buf;

    libc_memset(record_p, 0x00, sort_file_total * 2 + 2);

    for (i = 0; *same_p > 0;)
    {
        if (*same_p > 1)
        {
            count = *same_p;
            record_p = tidy_son_table(data_p, record_p, count, depend_buf);
            data_p = data_p + count;
            i = i + count;
        }
        else
        {
            //            *record_p++=1;
            *record_p = 1;
            record_p++;
            i++;
            data_p++;
        }
        same_p++;
    }

}

/******************************************************************************/
/*
 * \par  Description: 创建file_index 的parent位置映射表,
 *
 * \param[out]     map_buf  ---父级的tree位置偏移
 * \param[in]      index_buf---file_index
 * \param[in]      count_buf ---父级tree统计相同个数表

 * \return         TRUE or FALSE

 * \note
 *******************************************************************************/
bool creat_index_map(uint16 *map_buf, uint16 *index_buf, uint16 *count_buf)
{
    uint16 i, count;
    uint16 son_offset;

    son_offset = 0;
    while (*count_buf > 0)
    {
        i = 0;
        count = *count_buf;
        while (i < count)
        {
            map_buf[*index_buf] = son_offset;
            i++;
            index_buf++;
        }
        son_offset++;
        count_buf++;
    }

    return TRUE;
}

/******************************************************************************/
/*
 * \par  Description: 创建树结构,返回TREE的个数
 *
 * \param[in]     index_buf  ---file_index
 * \param[in]     cur_buf  ---当前级统计子集的相同个数表
 * \param[in]     son_buf ---子级统计到子级的相同个数表
 * \param[in]     parent_buf ---父级统计到子级的相同个数表
 * \param[in]    type---创建tree的类型

 * \return        TREE的个数

 * \note
 *******************************************************************************/
uint16 creat_plist_tree(uint16 *index_buf, uint16 *cur_buf, uint16 *son_buf, uint16 *parent_buf, uint8 type)
{
    plist_tree_t *tree_p;
    uint16 *cur_p, *son_p, *pare_p;
    uint16 i, j, count, n;
    uint16 s_offset, p_offset;//子tree和父tree偏移
    uint16 list_offset, son_num;
    uint16 tree_count;

    tree_p = (plist_tree_t *) temp_plist_buf;
    cur_p = cur_buf;
    son_p = son_buf;
    pare_p = parent_buf;

    if (*cur_p == 0)
    {
        //buffer error
        return 0;
    }

    j = 0;
    s_offset = 0;
    p_offset = 0;
    list_offset = 0;
    tree_count = 0;

    libc_memset(temp_plist_buf, 0, sizeof(temp_plist_buf));

    while (*cur_p > 0)
    {
        if ((0 != tree_count) && (0 == (tree_count % (SECTOR_SIZE / sizeof(plist_tree_t)))))
        {
            //buffer is full,write to file
            save_to_file(temp_plist_buf, SECTOR_SIZE);
            libc_memset(temp_plist_buf, 0x00, SECTOR_SIZE);
            tree_p = (plist_tree_t *) temp_plist_buf;
            libc_memset(temp_plist_buf, 0, sizeof(temp_plist_buf));
        }

        son_num = 0;//每个tree的子tree个数
        count = *cur_p;
        cur_p++;
        tree_p->flag = 1;
        tree_p->type = type;
        tree_p->file_index = *index_buf;
        tree_p->file_total = count;
        tree_p->prev_offset = list_offset - 1;
        tree_p->next_offset = list_offset + 1;
        list_offset++;

        if (parent_buf == NULL)
        {
            //没有父树级
            tree_p->parent_offset = 0xffff;
        }
        else
        { //有父级
            tree_p->parent_offset = p_offset;
            j = j + count;
            if (j >= *pare_p)
            {
                j = 0;
                pare_p++;
                p_offset++;
            }
        }

        if (son_buf == NULL)
        { //没有子树级
            tree_p->son_offset = 0xffff;
        }
        else
        { //有子树级
            tree_p->son_offset = s_offset;
            i = 0;
            n = 0;
            while (i < count)
            {
                i = *son_buf + i;
                son_buf++;
                son_num++;
                s_offset++;
            }
        }

        tree_p->son_num = son_num;

        index_buf = count + index_buf;
        tree_p++;
        tree_count++;

    }

    //modify last tree's next point
    tree_p--;
    tree_p->next_offset = 0xffff;

    //save last sector
    save_to_file(temp_plist_buf, SECTOR_SIZE);

    return tree_count;

}

