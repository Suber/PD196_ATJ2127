/*******************************************************************************
 *                              US212A
 *                            Module: BOOKMARK
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>         <time>             <version>             <desc>
 *      wuyufan    2011-9-25 17:17:29           1.0              build this file
 *******************************************************************************/
/********************************************************************************
 * \note    书签文件构造:
 * \note最开始4字节 魔数  4bytes
 * \notelast_access_index 2bytes
 * \notetotal_index       2bytes
 * \notecluster_no        4bytes
 * \notedir_entry         4bytes
 *********************************************************************************/

#include "app_music.h"
#include "app_music_bookmark.h"

//在.xdata段，该文件.xdata段置于bank段

uint8 bm_buf[BM_BUFF_SIZE] _BANK_DATA_ATTR_;

/******************************************************************************/
/*!
 * \par  Description:
 * \根据传入的cluster_no和dir_entry,在书签索引区寻找对应的索引项序号
 * \param[in]    cluster_no 文件所在目录项簇号
 * \param[in]    dir_entry  文件偏移
 * \param[in]    bm_fp      书签句柄
 * \param[in]    mode       搜索模式 mode = 1,会搜索已删除的索引项并加以回收
 * \param[out]   none
 * \return       index      返回找到的索引项序号
 * \retval           0xffff 未找到对应的序号
 * \retval           other  索引项序号
 * \ingroup      music_bookmark
 * \note
 */
/*******************************************************************************/
uint16 find_index(uint32 cluster_no, uint32 dir_entry, uint32 bm_fp, uint8 mode)
{
    uint8 search_mode;
    uint16 i;
    uint16 file_index = 0xffff;
    uint16 loop_cnt;

    //索引数据结构
    mbmk_index_t* bmk_index_item;

    bmk_index_item = (mbmk_index_t*) (bm_buf);

    search_mode = 0;

    retry: read_sector(0, bm_fp);

    bmk_index_item += 2;

    loop_cnt = bm_head.total_index + bm_head.del_total;

    for (i = 2; i < (loop_cnt + 2); i++)
    {
        if (search_mode == 0)
        {
            if ((cluster_no == bmk_index_item->bmk_clus_no) && (dir_entry == bmk_index_item->bmk_dir_eny))
            {
                //获取数据区索引项地址
                file_index = i - 2;
                break;
            }
        }
        else
        {
            if ((0xffffffff == bmk_index_item->bmk_clus_no) && (0xffffffff == bmk_index_item->bmk_dir_eny))
            {
                //当前项处于删除状态，则回收利用该空闲项
                file_index = i - 2;
                bm_head.del_total--;
                break;
            }
        }

        bmk_index_item++;

        if ((i % BM_SECTOR_PER_SORT_INDEX) == 0)
        {
            file_read(bm_buf, SEC_SIZE_USE, bm_fp);
            bmk_index_item = (mbmk_index_t*) (bm_buf);
            bmk_cache_sector++;
        }
    }

    if ((file_index == 0xffff) && (mode == 1) && (bm_head.del_total != 0))
    {
        //search only once
        mode = 0;
        search_mode = 1;
        goto retry;
    }

    return file_index;
}
/******************************************************************************/
/*!
 * \par  Description:
 * \根据找到的序号,将索引项内容写入书签索引区
 * \param[in]    cluster_no 文件所在目录项簇号
 * \param[in]    dir_entry  文件偏移
 * \param[in]    index      书签序号
 * \param[in]    bm_fp      书签文件句柄
 * \param[out]   none
 * \return       none
 * \ingroup      music_bookmark
 * \note
 */
/*******************************************************************************/
static void write_index(uint32 cluster_no, uint32 dir_entry, uint16 index, uint32 bm_fp)
{
    //索引数据结构
    mbmk_index_t* bmk_index_item;

    //将之前最后一个索引项的后继设为该index
    if (bm_head.last_index != 0xffff)
    {
        bmk_index_item = read_bmk_index(bm_head.last_index, bm_fp);
        bmk_index_item->next = index;
        write_bmk_index(bm_head.last_index, bm_fp);
    }

    //设置该index前驱指向最后一个索引项
    bmk_index_item = read_bmk_index(index, bm_fp);
    //索引区数据
    bmk_index_item->bmk_clus_no = cluster_no;
    bmk_index_item->bmk_dir_eny = dir_entry;
    bmk_index_item->prev = bm_head.last_index;
    bmk_index_item->next = 0xffff;
    //将索引数据回写到文件
    write_bmk_index(index, bm_fp);
    bm_head.total_index++;

    //这几句话不能提到前面去
    if (bm_head.last_index == 0xffff)
    {
        //初始化头指针和尾指针
        bm_head.last_index = bm_head.first_index = index;
    }
    else
    {
        //设置当前索引到尾指针
        bm_head.last_index = index;
    }
    return;
}
/******************************************************************************/
/*!
 * \par  Description:
 * \根据找到的序号,写入书签数据项内容
 * \param[in]    index      书签序号
 * \param[in]    name_str   书签文件名
 * \param[in]    bm_fp      书签文件句柄
 * \param[out]   none
 * \return       none
 * \ingroup      music_bookmark
 * \note
 */
/*******************************************************************************/

static bool write_bmk_item(uint16 index, uint8 *name_str, uint32 bm_fp)
{
    uint32 i;
    uint16 sector_no;
    bool is_write_bmk = FALSE;
    uint8 data_offset = (uint8)(index % 2);

    //数据区结构体
    mfile_bm_items_t* bm_data_ptr = (mfile_bm_items_t*) (bm_buf);

    //查看当前的index所在数据项是否匹配

    //未找到或者找到了但文件名不匹配时需要将文件名写入
    //计算数据区的扇区号

    sector_no = BM_HEADER_SECTOR_NUM + (index >> 1);
    //定位到数据区
    read_sector(sector_no, bm_fp);
    bm_data_ptr += data_offset;

    //比较文件名,未找到时文件名同样不匹配
    i = (uint32) libc_strlen(name_str);
    if (libc_memcmp(bm_data_ptr->mfile_name, name_str, i) != 0)
    {
        //数据区清空
        libc_memset(bm_data_ptr->mfile_bmk_items, 0xff, MAX_BOOKMARKS * sizeof(music_bm_item));
        //写入文件名
        libc_memcpy(bm_data_ptr->mfile_name, name_str, i);
        write_sector(sector_no, bm_fp);
#if 0
        //TODO:我觉得这里还有问题，假如后面一个扇区文件真的有效呢?
        sector_no++;
        if (sector_no < (BM_HEADER_SECTOR_NUM + BM_DATA_SECTOR_NUM))
        {
            libc_memset(bm_buf, 0xff, BM_BUFF_SIZE);
            write_sector(sector_no, bm_fp);
        }
#endif
        is_write_bmk = TRUE;
    }
    return is_write_bmk;
}
/******************************************************************************/
/*!
 * \par  Description:
 * \uint16 get_bookmark_index(uint32 cluster_no, uint32 dir_entry,uint8* name_str)
 * \获取当前书签所在的index
 * \param[in]  bm_fp书签句柄 param1
 * \param[in]  cluster_no书签输在的簇号 param2
 * \param[in]  dir_entry书签所在的目录项 param3
 * \param[out]
 * \return     uint16
 * \retval     书签序号
 * \ingroup    music_bookmark
 *******************************************************************************/
uint16 get_bookmark_index(uint32 cluster_no, uint32 dir_entry, uint8* name_str, uint32 bm_fp)
{
    music_bm_head *bm_head_ptr;
    bool is_write_bmk = FALSE;

    uint16 index;

    bmk_cache_sector = 0xffff;

    read_sector(0, bm_fp);

    bm_head_ptr = (music_bm_head *) bm_buf;

    //获取当前记录项总数
    libc_memcpy(&bm_head, bm_head_ptr, sizeof(music_bm_head));

    if (bm_head_ptr->total_index != 0xffff)
    {
        //需要遍历索引项数据区
        index = find_index(cluster_no, dir_entry, bm_fp, 1);
    }
    else
    {
        //记录项为0
        bm_head.total_index = 0;
        bm_head.del_total = 0;
        index = 0xffff;
        is_write_bmk = TRUE;
    }

    //未找到对应的索引项
    if (index == 0xffff)
    {
        if (bm_head.total_index >= MAX_MUSIC_FILE_NUMS)
        {
            //设置当前书签项索引为被删除书签项的索引
            index = bm_head.first_index;

            //书签项已满，删除最早添加进的书签项
            music_del_bookmark(NULL, bm_head.first_index, bm_fp);

            bmk_cache_sector = 0xffff;

            //回收一个删除项，计数减1
            bm_head.del_total--;
        }
        else
        {
            index = bm_head.total_index;
        }

        is_write_bmk = TRUE;

        //计算当前索引项在索引区的地址
        write_index(cluster_no, dir_entry, index, bm_fp);
    }

    //未找到或者找到了但文件名不匹配时需要将文件名写入
    //计算数据区的扇区号
    if (write_bmk_item(index, name_str, bm_fp) == TRUE)
    {
        is_write_bmk = TRUE;
    }

    if (is_write_bmk == TRUE)
    {
        //需要重新回写头信息
        //切换到第一扇区读内容
        read_sector(0, bm_fp);
        //更新书签文件头信息
        libc_memcpy(bm_head_ptr, &bm_head, sizeof(music_bm_head));
        //将书签文件头信息重新写回
        write_sector(0, bm_fp);
    }
    return index;

}

/*! \endcond */
