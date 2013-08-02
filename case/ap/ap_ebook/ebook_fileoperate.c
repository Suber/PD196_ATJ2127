/********************************************************************************
 *                              USDK
 *                             Module: ebook
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : ebook_fileoperate.c
 * History:
 *      <author>    <time>           <version >             <desc>
 *      stevenluo       2011-11-11         v1.0              create this file
 ********************************************************************************/

#include  "ebook.h"

/********************************************************************************
 * Description :文件信息初始化，主要包括文件location信息获取，文件打开
 *              获取文件大小，扇区数等信息
 * Arguments  :
 *              enter_mode:当前场景类型(参照ebook.h中的ebook_scene_e)
 * Returns     :
 *                  场景返回值

 * Notes       :
 *
 ********************************************************************************/

uint16 _init_file_info(void)
{
    uint32          unicode_flag;
//    int8            language;
    
    if (fsel_set_location(&g_ebook_vars.path.file_path.dirlocation, FSEL_TYPE_LISTEBOOK) == FALSE)//设置失败
    {
        if( fsel_get_byno(  g_ebook_vars.path.file_path.dirlocation.filename,
                            g_ebook_vars.path.file_path.dirlocation.file_num) == FALSE) //重新获取下一个文件
        {
            //获取当前模式下的第一个可播放文件
            if (fsel_get_nextfile(g_ebook_vars.path.file_path.dirlocation.filename) == FALSE)
            {
                //找不到可播放文件
                return S_FILE_NO_FILE;
            }        
        }

        //获取当前文件的location信息
        fsel_get_location(&g_ebook_vars.path.file_path.dirlocation, FSEL_TYPE_LISTEBOOK);
    }
    
    view_file.file_handle = vfs_file_open(g_ebook_mount_id, (void*) (0), R_NORMAL_SEEK);
    if (view_file.file_handle == (uint32) NULL)
    {
        //       _show_single_dialog(S_FILE_OPEN_ERROR);
        return S_FILE_OPEN_ERROR;
    }

    //读取文件头4个字节判别该文件的内容是内码还是unicode
    vfs_file_read(g_ebook_mount_id, &unicode_flag, 4, view_file.file_handle);
    
    //计算文件的总长度和总扇区数
    view_file.file_length = _get_file_length(view_file.file_handle);
    if (view_file.file_length == 0)
    {
        return S_FORMAT_ERROR;
    }
    _vram_limit_file_length();
    
    view_file.file_sectors = (view_file.file_length + (512 -1) ) / 512 ;  // + (512 -1) 是为了进位


    view_file.fseek = _seek_file_sector;
    view_file.fread = _read_file_sector;
    view_file.file_offset = file_offset = 0;
    
    //定位到文件头
    _seek_file_sector(0, view_file.file_handle);
    
    if ((unicode_flag & 0xFFFF) == UNICODE_FLAG)
    {
        view_file.language = UNICODEDATA;
    }
    else if ((unicode_flag & 0xFFFFFF) == UTF8_FLAG)
    {
        view_file.language = UTF_8DATA;
    }
    else
    {   
        //读取第一个扇区的内容，用来识别。复用unicode_flag来作为实际读取到的数
        unicode_flag = vfs_file_read(g_ebook_mount_id, bmk_bufFile, FILE_SECTOR, view_file.file_handle); 
        
        /*******************   需要判断是不是无BOM utf-8  ******************/
        if(Is_utf8_nobom(bmk_bufFile,FILE_SECTOR)==TRUE)
        {
            view_file.language = UTF_8DATA;
        }
        else
        {
            view_file.language = (int8) g_comval.language_id;
        } 
    }
    
    
    libc_memcpy(&page_count_file, &view_file, sizeof(view_file_t));

    //从文件长度和文件在磁盘的偏移判断该文件是否为上次打开过的文件
    if ((view_file.file_length == g_ebook_vars.file_length) && (cur_file_diroffset
            == g_ebook_vars.path.file_path.dirlocation.dir_entry))
    {
        curpagenum = g_ebook_vars.page_num;
    }
    else
    {
        curpagenum = 0;
    }
    g_ebook_vars.file_length = view_file.file_length;
    if (g_ebook_vars.path.file_path.dirlocation.disk == DISK_C)
    {
        return _vram_init_bmk_info();
    }
    
    //初始化书签文件
    return _init_bmk_info();
    
}

/********************************************************************************
 * Description :获取书签文件的第一个扇区的信息
 *
 * Arguments   :
 *             :
 * Returns     :
 *               TRUE or FALSE
 * Notes       :
 *            书签结构体:
 *             sector 0的情况:
 *                 byte:0-256:保存书签信息其结构参考bkmark_node_t,最多保存16个书签
 *                 byte:596-599:页数计算完毕的魔术标记;
 *                 byte:500-503:当前文件的总页数
 *                 byte:504-507:最后一页所在的偏移
 *             其他sector的情况:每四个byte存储对应页数所处的偏移
 ********************************************************************************/

bool _get_bmk_firstsec_info(void)
{
    uint32 cur_direction, file_length;
    file_length = _get_file_length(bmk_modify_handle);
    //如果读取的书签文件长度不大于一个扇区，说明没有存储到
    //页数信息，所以还得继续初始化书签文件
    if (file_length <= 512)
    {
        curpagenum = 0;
        libc_memset(bmk_bufFile, 0xFF, FILESIZE);
        _seek_and_write(0, FILESIZE, bmk_bufFile);
        cur_BufSector = 0;
        return 1;
    }
    _seek_and_read(0, FILESIZE, bmk_bufFile);
    libc_memcpy(&cur_direction, &bmk_bufFile[512 - 16], 4);
    //如果第一个扇区最后四个字节为0x55aaaa55的话，说明文件页数已经计算完毕
    //否则的话则将从最后的位置继续计算页数
    if (cur_direction == PAGE_MAGIC)
    {
        page_count_flag = FALSE;
    }
    libc_memcpy(&page_totalnum, &bmk_bufFile[512 - 12], 4);
    if (page_totalnum == 0xFFFFFFFF)
    {
        page_totalnum = 0;
        page_count_flag = TRUE;
        cur_BufSector = 0;
        temp_count = 0;
        return 1;
    }
    if (page_count_flag == TRUE)
    {
        libc_memcpy(&page_count_file.file_offset, &bmk_bufFile[512 - 8], 4);
        bmk_last_pagenum = page_totalnum;
    }
    return 1;
}
/********************************************************************************
 * Description :书签文件的初始化
 *
 * Arguments  :
 *              status:
 * Returns     :
 *               TRUE or FALSE
 * Notes       :
 *            书签结构体:
 *             sector 0的情况:
 *                 byte:0-256:保存书签信息其结构参考bkmark_node_t,最多保存16个书签
 *                 byte:596-599:页数计算完毕的魔术标记;
 *                 byte:500-503:当前文件的总页数
 *                 byte:504-507:最后一页所在的偏移
 *             其他sector的情况:每四个byte存储对应页数所处的偏移
 ********************************************************************************/
uint16 _init_bmk_info(void)
{
    uint16 filename_length;
    uint32 tmp_page_total;
    libc_memset(bmk_bufFile, 0x0, FILESIZE);
    
    cur_BufSector = -1;
    draw_page_num = page_count_flag = TRUE;
    page_totalnum = 0;
    bmk_last_pagenum = 0;
    temp_count = 0;
    bookmark_total = 0xFF;
    filename_length = vfs_get_name(g_ebook_mount_id, BOOKMARK_filename, MAX_FILENAME_LENGTH);

    _get_filename(BOOKMARK_filename,filename_length);   //获取文件名，在界面显示
    
    //获取书签文件名
    if (_get_bmk_name(filename_length) == 0)
    {
        return S_FILE_OPEN_ERROR;
    }
    bmk_modify_handle = vfs_file_open(g_ebook_mount_id, BOOKMARK_filename, OPEN_MODIFY);
    if ((uint32) NULL == bmk_modify_handle)
    {
        //书签文件的长度计算为扇区倍数
        tmp_page_total = view_file.file_length / 128;
        if((view_file.file_length % 128) != 0)
        {
            tmp_page_total++;
        }
        if( (tmp_page_total % 128) != 0)
        {
            tmp_page_total = (tmp_page_total & 0xffffff00) + 128;
        }
        // 书签文件不存在，创建书签文件
        _show_single_dialog(S_WAITING, DIALOG_INFOR_IMMEDIATE);
        bmk_modify_handle = vfs_file_create(g_ebook_mount_id, BOOKMARK_filename, 512 + tmp_page_total * 4);
        if (bmk_modify_handle != (uint32) NULL)
        {
            //将文件属性设置为隐藏
            //      vfs_file_attralter(g_ebook_mount_id, ATTR_HIDDEN, bmk_modify_handle, 0);
            libc_memset(bmk_bufFile, 0xFF, FILESIZE);
            _seek_and_write(0, FILESIZE, bmk_bufFile);
            curpagenum = 0;
            cur_BufSector = 0;
        }
        else
        {
            return S_FILE_OPEN_ERROR;
        }
    }
    else
    {
        _get_bmk_firstsec_info();
    }
    decode_offset = page_count_file.file_offset;
    return 0;
}

/********************************************************************************
 * Description :获取当前打开的文件总长度
 * Arguments  :
 *              mount_id:盘符识别号，主要是获取fsel_init的返回值
 *              file_handle:
 * Returns     :
 *                文件的总长度
 * Notes       :
 *
 ********************************************************************************/
uint32 _get_file_length(uint32 file_handle)
{
    uint32 file_length;
    vfs_file_get_size(g_ebook_mount_id, &file_length, file_handle, 0);
    return file_length;
}
/********************************************************************************
 * Description :定位要读取的文件位置，以扇区为单位
 * Arguments  :
 *              sector_offset:扇区序号
 * Returns     :
 *                TRUE or FALSE
 * Notes       :
 *
 ********************************************************************************/
bool _seek_file_sector(uint32 sector_offset, uint32 file_handle)
{
    return vfs_file_seek(g_ebook_mount_id, sector_offset * 512, SEEK_SET, file_handle);
}
/********************************************************************************
 * Description :读取一个扇区到数据到某一个缓存区中
 * Arguments  :
 *              viewfilebuf:缓存读取数据的buf
 *              file_handle:需要读取的文件的句柄
 * Returns     :
 *                TRUE or FALSE
 * Notes       :
 *
 ********************************************************************************/
bool _read_file_sector(uint8 *viewfilebuf, uint32 file_handle)
{
    if (vfs_file_read(g_ebook_mount_id, viewfilebuf, FILE_SECTOR, file_handle) == 0)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}
/********************************************************************************
 * Description :关闭已打开的文件文件句柄
 *
 * Arguments  :
 *              无
 * Returns     :
 *                  无
 * Notes       :
 *
 ********************************************************************************/

void _close_text_file(uint8 update_flag)
{
    if (bmk_modify_handle != 0)
    {
        if (update_flag == TRUE)
        {
            _writeback_bmk_info();
        }
        vfs_file_close(g_ebook_mount_id, bmk_modify_handle);
        bmk_modify_handle = 0x0;
        g_ebook_vars.play_flag = 0;
    }
    if (view_file.file_handle != 0)
    {
        if (g_ebook_vars.path.file_path.dirlocation.disk == DISK_C)
        {
            _vram_wb_bmk_headinfo();
        }
        vfs_file_close(g_ebook_mount_id, view_file.file_handle);

        view_file.file_handle = 0x0;
    }
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

void _writeback_bmk_info(void)
{
    uint8 need_write = FALSE;
    if (page_count_flag == TRUE)
    {
        _seek_and_write(FILE_SECTOR + (page_totalnum - temp_count) * 4, temp_count * 4, bmk_bufFile);
    }
    _seek_and_read(0, FILE_SECTOR, bmk_bufFile);
    //如果页数计算还没完成，则需要回写关键信息
    if (page_count_flag == TRUE)
    {
        libc_memcpy(&bmk_bufFile[500], &page_totalnum, 0x04);
        libc_memcpy(&bmk_bufFile[504], &decode_offset, 0x04);
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
        _seek_and_write(0, FILE_SECTOR, bmk_bufFile);
    }
}

