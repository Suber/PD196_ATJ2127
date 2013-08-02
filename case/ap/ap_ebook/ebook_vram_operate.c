/********************************************************************************
 *                              USDK
 *                             Module: ebook
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : ebook_vram_operate.c
 * History:
 *      <author>    <time>           <version >             <desc>
 *      stevenluo       2011-11-11         v1.0              create this file
 ********************************************************************************/

#include  "ebook.h"

/********************************************************************************
 * Description :书签文件的初始化
 *
 * Arguments  :
 *              status:
 * Returns     :
 *               TRUE or FALSE
 * Notes       :
 *            书签结构体:
 *             从VM_BMK_INFO        0x00100000开始的第一个sector的情况:
 *                 byte:0-47F:存储vram_bmk_info_t信息，最多支持MAX_BMK_FILE个文件信息
 *                 byte:480-508:保留
 *                 byte:508-512:0x55aaaa55表示书签的vram区已初始化过了
 *             书签信息开始存储第一个sector的情况:
 *                 byte:0-255:保存书签信息其结构参考bkmark_node_t,最多保存16个书签
 *                 byte:256-463:保留
 *                 byte:464-512:存储decode_bmk_info_t 信息
 *                 byte:504-507:最后一页所在的偏移
 *             其他sector的情况:每四个byte存储对应页数所处的偏移
 ********************************************************************************/
uint16 _vram_init_bmk_info(void)
{
    uint32 headinfo_magic;
    uint16 filename_length;
    
    uint8 i = 0;
    libc_memset(bmk_bufFile, 0x0, FILESIZE);
    cur_BufSector = -1;
    draw_page_num = page_count_flag = TRUE;
    page_totalnum = 0;
    bmk_last_pagenum = 0;
    temp_count = 0;
    bookmark_total = 0xFF;
    decode_offset = 0;
    filename_length = vfs_get_name(g_ebook_mount_id, BOOKMARK_filename, MAX_FILENAME_LENGTH);
    
    _get_filename(BOOKMARK_filename,filename_length);   //获取文件名，在界面显示

    sys_vm_read(bmk_bufFile, VM_BMK_INFO, FILESIZE);
    libc_memcpy(&headinfo_magic, &bmk_bufFile[FILESIZE - 4], 4);

    while (1)
    {
        //如果文件总数超过支持的最大数或者魔术不对的话重新初始化vram区数据
        if ( (i >= MAX_BMK_FILE ) || (headinfo_magic != PAGE_MAGIC) )
        {
            libc_memset(bmk_bufFile, 0xFF, FILESIZE);
            i = 0;
        }
        libc_memcpy(&vram_bmk_info, &bmk_bufFile[i * 12], 12);
        //如果当前的结构信息不对，则重新初始化结构信息
        if ((vram_bmk_info.cluster_no == 0x0) || (vram_bmk_info.cluster_no == 0xFFFFFFFF))
        {
            _vram_init_bmk_headinfo(i);
            return 0;
        }
        if ((vram_bmk_info.cluster_no == g_ebook_vars.path.file_path.dirlocation.cluster_no)
                && (vram_bmk_info.dir_entry == g_ebook_vars.path.file_path.dirlocation.dir_entry))
        {
            break;
        }
        else
        {
            i++;
        }
    }
    //读取书签信息，判断是否与当前的书签信息一致，如果不一致
    //需要重新需要初始化
    sys_vm_read(bmk_bufFile, VM_BMK_INFO + vram_bmk_info.startadd, FILESIZE);
    libc_memcpy(&decode_bmk_info, &bmk_bufFile[FILESIZE - 0x30], 0x30);
    if (libc_memcmp(decode_bmk_info.file_name, BOOKMARK_filename, filename_length) !=0 )
    {
        i++;
        sys_vm_read(bmk_bufFile, VM_BMK_INFO, FILESIZE);
        _vram_init_bmk_headinfo(i);
        return 0;
    }
    if (decode_bmk_info.decode_flag == PAGE_MAGIC)
    {
        page_count_flag = FALSE;
    }
    if (decode_bmk_info.totalpage == 0xFFFFFFFF)
    {
        page_totalnum = 0;
        page_count_flag = TRUE;
        cur_BufSector = 0;
        temp_count = 0;
        return 0;
    }
    decode_offset = page_count_file.file_offset = decode_bmk_info.decode_offset;
    page_totalnum = decode_bmk_info.totalpage;
    //如果页数计算还没有完成，需要将最多存储不够一个sector的数据读取出来，
    //以便继续计算以后整个sector回写
    if ((page_count_flag == TRUE) && (page_totalnum > 1))
    {
        _vram_read_bmkinfo();
        bmk_last_pagenum = page_totalnum;
    }
    return 0;
}

void _vram_init_bmk_headinfo(uint8 file_index)
{
    uint32 tempadd;
    uint32 headinfo_magic;
    uint32 next_vram_bmk_startadd;
    vram_bmk_info.cluster_no = g_ebook_vars.path.file_path.dirlocation.cluster_no;
    vram_bmk_info.dir_entry = g_ebook_vars.path.file_path.dirlocation.dir_entry;
    tempadd = (view_file.file_length / 64);
    tempadd = ((tempadd / FILESIZE) + 4) * FILESIZE;
    if (file_index != 0)
    {
        libc_memcpy(&vram_bmk_info.startadd, &bmk_bufFile[file_index * 12 + 8], 4);
    }
    else
    {
        vram_bmk_info.startadd = FILESIZE;
    }
    next_vram_bmk_startadd = vram_bmk_info.startadd + tempadd;
    //如果写vram区的大小已经超过结束地址，则清除vram区，重新初始化
    if (next_vram_bmk_startadd >= VRAM_END_ADDRESS)
    {
        libc_memset(bmk_bufFile, 0xFF, FILESIZE);
        vram_bmk_info.startadd = FILESIZE;
        next_vram_bmk_startadd = vram_bmk_info.startadd + tempadd;
        file_index = 0;
    }
    libc_memcpy(&bmk_bufFile[(file_index + 1) * 12 + 8], &next_vram_bmk_startadd, 4);
    libc_memcpy(&bmk_bufFile[file_index * 12], &vram_bmk_info, sizeof(vram_bmk_info));
    headinfo_magic = PAGE_MAGIC;
    libc_memcpy(&bmk_bufFile[FILESIZE - 4], &headinfo_magic, 4);
    _vram_write_data(0);

    libc_memset(bmk_bufFile, 0xff, FILESIZE);
    libc_memcpy(decode_bmk_info.file_name, BOOKMARK_filename, 32);
    libc_memcpy(&bmk_bufFile[FILESIZE - 0x30], &decode_bmk_info, sizeof(decode_bmk_info));
    _vram_write_data(vram_bmk_info.startadd);
}

/********************************************************************************
 * Description :回写第一个盲区和最后需要保存的数据到书签文件中
 *
 * Arguments  :
 *              无
 * Returns     :
 *                  无
 * Notes       :
 *
 ********************************************************************************/

void _vram_wb_bmk_headinfo(void)
{
    uint8 need_write = FALSE;
    uint32 offset_sector;
    if (page_count_flag == TRUE)
    {
        offset_sector = vram_bmk_info.startadd + FILESIZE + ((page_totalnum - 1) / 128) * FILESIZE;
        _vram_write_data(offset_sector);
    }
    //如果页数计算还没完成，则需要回写关键信息
    sys_vm_read(bmk_bufFile, VM_BMK_INFO +vram_bmk_info.startadd, FILESIZE);
    if (page_count_flag == TRUE)
    {
        decode_bmk_info.totalpage = page_totalnum;
        decode_bmk_info.decode_offset = decode_offset;
        decode_bmk_info.decode_flag = 0xFFFFFFFF;
        libc_memcpy(&bmk_bufFile[FILESIZE - 0x30], &decode_bmk_info, 0x30);
        bmk_last_pagenum = page_totalnum;
        need_write = TRUE;
    }
    //如果书签信息有变动，则需要重新回写
    if ((bmk_modify_flag == TRUE) && (bookmark_total <= 16))
    {
        libc_memcpy(bmk_bufFile, &bkmarks, MAX_BKMARK_NUM * 16);
        need_write = TRUE;
        bmk_modify_flag = FALSE;
    }
    if (need_write == TRUE)
    {
        _vram_write_data(vram_bmk_info.startadd);
    }
}

/********************************************************************************
 * Description :将计算得到的书签信息写入VRAM区，每次都是一个sector写
 *
 * Arguments  :
 *              无
 * Returns     :
 *                  无
 * Notes       :
 *
 ********************************************************************************/
void _vram_write_bmkinfo(void)
{
    uint32 offset_sector;
    //如果到了文件尾，则将页数信息和结束符PAGE_MAGIC回写到文件中
    if (decode_offset >= page_count_file.file_length)
    {
        offset_sector = vram_bmk_info.startadd + FILESIZE + ((page_totalnum - 1) / 128) * FILESIZE;
        _vram_write_data(offset_sector);

        sys_vm_read(bmk_bufFile, VM_BMK_INFO + vram_bmk_info.startadd, FILESIZE);
        decode_bmk_info.decode_flag = PAGE_MAGIC;
        decode_bmk_info.totalpage = page_totalnum;
        libc_memcpy(&bmk_bufFile[FILESIZE - 0x30], &decode_bmk_info, sizeof(decode_bmk_info));
        _vram_write_data(vram_bmk_info.startadd);
        page_count_flag = FALSE;
        cur_BufSector = -1;
        return;
    }
    if (page_totalnum > 0)
    {
        libc_memcpy(&bmk_bufFile[((page_totalnum - 1) % 128) * 4], &decode_offset, 0x04);
    }
    //当页数信息满一个扇区时，将数据回写书签文件
    if ((page_totalnum % 128) == 0)
    {
        bmk_last_pagenum = page_totalnum;
        offset_sector = vram_bmk_info.startadd + FILESIZE + ((page_totalnum - 1) / 128) * FILESIZE;
        _vram_write_data(offset_sector);
        temp_count = 0;
        libc_memset(bmk_bufFile, 0x0, FILESIZE);
    }
}
/********************************************************************************
 * Description :从vram区的某个位置读取一个sector的数据
 *
 * Arguments  :
 *              无
 * Returns     :
 *                  无
 * Notes       :
 *
 ********************************************************************************/

void _vram_read_bmkinfo(void)
{
    uint32 offset_sector;
    offset_sector = vram_bmk_info.startadd + FILESIZE + ((page_totalnum - 1) / 128) * FILESIZE;
    sys_vm_read(bmk_bufFile, VM_BMK_INFO + offset_sector, FILESIZE);
}
/********************************************************************************
 * Description :往vram区的某个位置写入一个sector的数据
 *
 * Arguments  :
 *              无
 * Returns     :
 *                  无
 * Notes       :
 *
 ********************************************************************************/

void _vram_write_data(uint32 address_offset)
{
    sys_vm_write(bmk_bufFile, VM_BMK_INFO + address_offset);
}
void _vram_limit_file_length(void)
{
    if ((g_ebook_vars.path.file_path.dirlocation.disk == DISK_C) && (view_file.file_length > MAX_FILE_LENGTH))
    {
        view_file.file_length = MAX_FILE_LENGTH;
    }
}
