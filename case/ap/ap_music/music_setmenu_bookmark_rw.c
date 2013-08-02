#include "app_music.h"
#include "app_music_bookmark.h"

//.rodata
//const uint8 BM_FILENAME[12] = "MUSICBMKBMK";
static const uint8 BM_FILENAME[] =
{
    0xff, 0xfe, //unicode flag
    0x4D, 0x00, //M
    0x55, 0x00, //U
    0x53, 0x00, //S
    0x49, 0x00, //I
    0x43, 0x00, //C
    0x42, 0x00, //B
    0x4D, 0x00, //M
    0x4B, 0x00, //K
    0x2E, 0x00, //.
    0x42, 0x00, //B
    0x4D, 0x00, //M
    0x4B, 0x00, //K
    0x00, 0x00 //end flag
};

//nandflash只支持SEEK_SET
static void file_seek(uint32 seek_num, uint8 seek_type, uint32 fp)
{
    seek_num <<= 9;
    if (g_browser_filepath.file_path.plist_location.disk == DISK_C)
    {
        g_bookmark_fp = VM_BOOKMARK_START + seek_num;
    }
    else
    {
        vfs_file_seek(g_music_mount_id, seek_num, seek_type, fp);
    }
}

//封装文件读函数
void file_read(void *buffer, uint32 len, uint32 fp)
{
    if (g_browser_filepath.file_path.plist_location.disk == DISK_C)
    {
        //注意这里不能使用fp,因为fp的值有可能没有更新
        sys_vm_read(buffer, g_bookmark_fp, len);
        g_bookmark_fp += len;
    }
    else
    {
        vfs_file_read(g_music_mount_id, buffer, len, fp);
    }
}

//封装文件写函数
static void file_write(void *buffer, uint32 len, uint32 fp)
{
    if (g_browser_filepath.file_path.plist_location.disk == DISK_C)
    {
        //注意这里不能使用fp,因为fp的值有可能没有更新
        sys_vm_write(buffer, g_bookmark_fp);
        g_bookmark_fp += len;
    }
    else
    {
        vfs_file_write(g_music_mount_id, buffer, len, fp);
    }
}

/******************************************************************************/
/*!
 * \par  Description:
 * \void write_sector(uint32 sector_num, handle fp)
 * \写一个扇区的数据到文件
 * \param[in]   sector_num 扇区序号 param1
 * \param[in]   fp 文件句柄 param2
 * \param[out]
 * \return     void
 * \retval
 * \ingroup     music_bookmark
 * \note
 *******************************************************************************/
void write_sector(uint32 sector_num, uint32 fp)
{
    file_seek(sector_num, SEEK_SET, fp);
    file_write(bm_buf, BM_BUFF_SIZE, fp);
}

/******************************************************************************/
/*!
 * \par  Description:
 * \void read_sector(uint32 sector_num, handle fp)
 * \读一个扇区的数据
 * \param[in]   sector_num 扇区序号 param1
 * \param[in]   fp 文件句柄 param2
 * \param[out]
 * \return     void
 * \retval
 * \ingroup    music_bookmark
 * \note
 *******************************************************************************/
void read_sector(uint32 sector_num, uint32 fp)
{
    if (sector_num != bmk_cache_sector)
    {
        bmk_cache_sector = (uint16)sector_num;
        file_seek(sector_num, SEEK_SET, fp);
        file_read(bm_buf, BM_BUFF_SIZE, fp);
    }
}
/******************************************************************************/
/*!
 * \par  Description:
 * \根据索引号读取索引区内容
 * \param[in]   data_index 序号
 * \param[in]   bm_fp 书签句柄
 * \param[out]  none
 * \return      mbmk_index_t * 索引项内容指针
 * \retval
 * \ingroup    music_bookmark
 * \note
 *******************************************************************************/

mbmk_index_t *read_bmk_index(uint16 data_index, uint32 bm_fp)
{
    uint8 sector_num;
    uint8 item_offset;
    //索引数据结构
    mbmk_index_t* bmk_index_item;

    uint16 index = data_index + 2;

    sector_num = (uint8)(index / BM_SECTOR_PER_SORT_INDEX);
    item_offset = (uint8)(index % BM_SECTOR_PER_SORT_INDEX);

    read_sector(sector_num, bm_fp);

    bmk_index_item = (mbmk_index_t*) (bm_buf);

    bmk_index_item += item_offset;

    return bmk_index_item;
}
/******************************************************************************/
/*!
 * \par  Description:
 * \根据索引号写入索引区内容
 * \param[in]   data_index 序号
 * \param[in]   bm_fp 书签句柄
 * \param[out]  none
 * \return      mbmk_index_t * 索引项内容指针
 * \retval
 * \ingroup    music_bookmark
 * \note
 *******************************************************************************/

void write_bmk_index(uint16 data_index, uint32 bm_fp)
{
    uint8 sector_num;
    uint16 index = data_index + 2;

    sector_num = (uint8)(index / BM_SECTOR_PER_SORT_INDEX);
    write_sector(sector_num, bm_fp);
}
/******************************************************************************/
/*!
 * \par  Description:
 * \根据索引号读取数据区内容
 * \param[in]   data_index 序号
 * \param[in]   bm_fp 书签句柄
 * \param[out]  none
 * \return      mfile_bm_items_t * 数据区内容指针
 * \retval
 * \ingroup    music_bookmark
 * \note
 *******************************************************************************/

mfile_bm_items_t *read_bmk_item(uint16 index, uint32 bm_fp)
{
    uint16 sector_no;
    uint8 sector_offset;
    //数据区结构体
    mfile_bm_items_t* bm_data_ptr = (mfile_bm_items_t*) (bm_buf);

    sector_no = BM_HEADER_SECTOR_NUM;
    sector_no += index / 2;

    sector_offset = (uint8)(index % 2);
    //读取数据内容
    //bmk_read_sector(sector_no, bm_fp);
    read_sector(sector_no, bm_fp);

    bm_data_ptr += sector_offset;

    return bm_data_ptr;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \uint32 open_bookmark(void)
 * \打开书签文件
 * \param[in]   none
 * \param[out]  none
 * \return      uint32 书签句柄
 * \retval      打开成功或创建成功,返回打开的句柄，
 * \ingroup     music_bookmark
 * \note
 *******************************************************************************/
uint32 open_bookmark(void)
{
    uint32 bm_fp;

    bmk_cache_sector = 0xffff;

    if (g_browser_filepath.file_path.plist_location.disk == DISK_C)
    {
        bm_fp = VM_BOOKMARK_START;
    }
    else
    {
        bm_fp = vfs_file_open(g_music_mount_id, BM_FILENAME, OPEN_MODIFY);
    }

    return bm_fp;
}
/******************************************************************************/
/*!
 * \par  Description:
 * \void close_bookmark(uint32 bm_fp)
 * \关闭书签文件
 * \param[in]   bm_fp 书签句柄
 * \param[out]  none
 * \return      none
 * \ingroup     music_bookmark
 * \note
 *******************************************************************************/
void close_bookmark(uint32 bm_fp)
{
    if (g_browser_filepath.file_path.plist_location.disk != DISK_C)
    {
        vfs_file_close(g_music_mount_id, bm_fp);
    }
}

