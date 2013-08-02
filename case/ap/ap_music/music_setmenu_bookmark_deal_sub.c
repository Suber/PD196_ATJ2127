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


/******************************************************************************/
/*!
 * \par  Description:
 * \uint8 handle_bookmark(uint8 active, uint8 mode, uint16 cur_index)
 * \添加书签项或者删除书签项，删除书签项就将对应内容写0xffffff:wq
 * \param[in]  active 当前激活项 param1
 * \param[in]  mode 添加 or 删除 param2
 * \param[in]  cur_index 当前项的序号 param3
 * \param[out]
 * \return     uint8
 * \retval     TRUE 成功
 * \retval     FALSE 失败
 * \ingroup     music_bookmark
 * \note
 *******************************************************************************/
uint8 handle_bookmark(uint8 active, uint8 mode, uint16 cur_index)
{
    uint32 time;
    mengine_playinfo_t play_info;
    music_bm_item bm_item;
    //uint16 sec_no, offset;

    //索引数据结构
    mbmk_index_t* bmk_index_ptr;
    //数据区结构
    mfile_bm_items_t* bm_data_ptr;

    file_location_t *plocation = (file_location_t *) &g_file_path_info.file_path;

    bmk_cache_sector = 0xffff;

    if (mode == MODE_ADD_BOOKMARK)
    {
        music_get_playinfo(&play_info);
        //播放时间转换为秒为单位
        time = (play_info.cur_time / 1000);

        bm_item.break_time.hour = (uint8)(time / 3600);
        bm_item.break_time.minute = (uint8)((time % 3600) / 60);
        bm_item.break_time.second = (uint8)(time % 60);

        //获取断点信息
        music_get_bkinfo(&bm_item.break_param);
    }
    else if (mode == MODE_DEL_BOOKMARK) //删除标签就是将某项置为0xffffff
    {
        libc_memset(&bm_item.break_time, 0xff, sizeof(time_t));
    }
    else //select bookmark
    {
        ;//do nothing
    }

    g_bookmark_fp = open_bookmark();

    //检测当前播放的location和书签的location是否还匹配
    //定位到索引区读取索引数据
    bmk_index_ptr = read_bmk_index(cur_index, g_bookmark_fp);

    music_get_filepath(&g_file_path_info);

    if ((plocation->cluster_no != bmk_index_ptr->bmk_clus_no) || (plocation->dir_entry != bmk_index_ptr->bmk_dir_eny))
    {
        //不对书签进行任何操作
        close_bookmark(g_bookmark_fp);
        return FALSE;
    }

    //定位到数据区
    bm_data_ptr = read_bmk_item(cur_index, g_bookmark_fp);

    if (mode == MODE_SELECT_BOOKMARK)
    {
        //先关闭书签文件，因为断点播放对于audible需要操作pos文件
        //文件系统只支持一个文件modify
        close_bookmark(g_bookmark_fp);
        
        libc_memcpy(&bm_item, &(bm_data_ptr->mfile_bmk_items[active]), sizeof(music_bm_item));

        if ((bm_item.break_time.hour == 0xff) && (bm_item.break_time.minute == 0xff) && (bm_item.break_time.second
                == 0xff))
        {
            return FALSE;
        }

        music_play_bookmark(&bm_item.break_param);
    }
    else
    {
        libc_memcpy(&(bm_data_ptr->mfile_bmk_items[active]), &bm_item, sizeof(music_bm_item));
        write_sector(cur_index / 2 + BM_HEADER_SECTOR_NUM, g_bookmark_fp);
        close_bookmark(g_bookmark_fp);
    }
    return TRUE;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \uint8 get_bookmark_time(uint16 index, time_t *bm_item_buf)
 * \获取书签的时间标签
 * \param[in]  index 当前项的序号 param1
 * \param[in]  bm_item_buf 保存书签时间的buffer param2
 * \param[out]
 * \return     uint8
 * \retval     TRUE 成功
 * \retval     FALSE 失败
 * \ingroup    music_bookmark
 * \note
 *******************************************************************************/
uint8 get_bookmark_time(uint16 index, time_t *bm_item_buf, uint32 bm_fp)
{
    uint8 i;

    mfile_bm_items_t* bm_data_ptr = (mfile_bm_items_t*) (bm_buf);

    bmk_cache_sector = 0xffff;

    bm_data_ptr = read_bmk_item(index, bm_fp);

    // 读取书签内容
    for (i = 0; i < MAX_BOOKMARKS; i++)
    {
        libc_memcpy(&bm_item_buf[i], &(bm_data_ptr->mfile_bmk_items[i].break_time), sizeof(time_t));
    }
    return TRUE;
}

