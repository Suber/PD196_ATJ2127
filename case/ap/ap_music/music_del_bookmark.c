/*******************************************************************************
 *                              US212A
 *                            Module: MUSIC_CREATE_BOOKMARK
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>         <time>             <version>             <desc>
 *      wuyufan    2011-11-9 16:45:04           1.0              build this file
 *******************************************************************************/

#include "app_music.h"
#include "app_music_bookmark.h"

/******************************************************************************/
/*!
 * \par  Description:
 *  void del_bookmark_by_index(uint16 index, uint32 bm_fp)
 *	  根据书签在书签文件的索引号删除之
 * \param[in]  index 书签序号
 * \param[out] bm_fp 书签文件句柄
 * \return     none
 * \ingroup    music_bookmark
 * \note
 * \    该接口有两个地方会调用到，如果删除music文件时，会顺带删除书签文件，根据location
        得到书签序号；或者书签满时，覆盖最早创建的书签文件，也会删除
 *******************************************************************************/
void del_bookmark_by_index(uint16 index, uint32 bm_fp)
{
    //索引数据结构
    mbmk_index_t* bmk_index_item;
    mfile_bm_items_t* bm_data_ptr;

    //保存最后一个索引的内容
    //mbmk_index_t last_index_item;
    //保存待删除索引项的内容
    mbmk_index_t del_index_item;

    //uint16 last_index = bm_head.last_index;

    //定位到当前删除文件的索引项
    bmk_index_item = read_bmk_index(index, bm_fp);
    //保存待删除索引项的内容
    libc_memcpy(&del_index_item, bmk_index_item, sizeof(mbmk_index_t));
    //设置该索引项无效
    libc_memset(bmk_index_item, 0xff, sizeof(mbmk_index_t));

    if (del_index_item.prev != 0xffff)
    {
        //修改删除项前驱的后继
        bmk_index_item = read_bmk_index(del_index_item.prev, bm_fp);
        bmk_index_item->next = del_index_item.next;
        write_bmk_index(del_index_item.prev, bm_fp);
    }
    else
    {
        //修改头指针
        bm_head.first_index = del_index_item.next;
    }

    if (del_index_item.next != 0xffff)
    {
        //修改删除项后继的前驱
        bmk_index_item = read_bmk_index(del_index_item.next, bm_fp);
        bmk_index_item->prev = del_index_item.prev;
        write_bmk_index(del_index_item.next, bm_fp);
    }
    else
    {
        //修改尾指针
        bm_head.last_index = del_index_item.prev;
    }

    //清除数据区内容
    bm_data_ptr = read_bmk_item(index, bm_fp);
    libc_memset(bm_data_ptr, 0xff, sizeof(mfile_bm_items_t));
    write_sector(index / 2 + BM_HEADER_SECTOR_NUM, bm_fp);

    //更新头信息
    bm_head.total_index--;
    bm_head.del_total++;
    return;

}
/******************************************************************************/
/*!
 * \par  Description:
 *  void del_bookmark_file(file_location_t *plocation)
 *  删除文件时,删除相应的书签文件项
 * \param[in]
 * \param[out]
 * \return     void
 * \retval
 * \ingroup    music_bookmark
 * \note
 *******************************************************************************/
void music_del_bookmark(file_location_t *plocation, uint16 cur_index, uint32 bm_fp)
{
    //书签文件头信息
    music_bm_head *bm_head_ptr;
    uint16 index = 0xffff;

    bmk_cache_sector = 0xffff;

    //读取第一个扇区的值
    read_sector(0, bm_fp);

    bm_head_ptr = (music_bm_head *) bm_buf;

    libc_memcpy(&bm_head, bm_head_ptr, sizeof(music_bm_head));

    //存在书签记录项
    if (bm_head.total_index != 0xffff)
    {
        if (plocation != NULL)
        {
            //需要遍历索引项数据区
            //第一个扇区的前16字节是书签文件头
            index = find_index(plocation->cluster_no, plocation->dir_entry, g_bookmark_fp, 0);
        }
        else
        {
            index = cur_index;
        }
        //找到索引项
        if (index != 0xffff)
        {
            del_bookmark_by_index(index, bm_fp);
            read_sector(0, bm_fp);
            libc_memcpy(bm_head_ptr, &bm_head, sizeof(music_bm_head));
            write_sector(0, bm_fp);
        }
    }
}

/*! \endcond */

