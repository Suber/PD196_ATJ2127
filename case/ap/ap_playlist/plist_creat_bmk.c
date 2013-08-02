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
 *               默认创建文件使用长名
 * \version 1.0
 * \date  2011/9/15
 *******************************************************************************/
#include "plist.h"
#include "rdata_ex.h"

//系统默认创建文件使用长名

#ifdef USE_83NAME_CREAT
static const uint8 BOOKMARK_NAME[12] = "MUSICBMKBMK";
#else
static const uint8 BOOKMARK_NAME[] = "MUSICBMK.BMK";
#endif

//创建ebook的书签使用
static const uint8 str[3] =
{ 0x42, 0x4D, 0x4B };
static const uint8 hex_str[8] =
{ 0x2E, 0x0, 0x42, 0x0, 0x4D, 0x0, 0x4B, 0x0 };

#ifdef USE_83NAME_CREAT
static const uint8 EBOOK_LIST_NAME[12]="EBOOK   LIB";
#else
static const uint8 EBOOK_LIST_NAME[] = "EBOOK.LIB";
#endif

uint32 g_bookmark_fp _BANK_DATA_ATTR_;

/******************************************************************************/
/*
 * \par  Description: mbyte to wchar
 *
 * \param[in]    dest，src

 * \param[out]

 * \return  UNICODE 个数，不包含标识符和结束符

 * \note    ASC转成UNICODE，自动加入标识符和结束符
 *******************************************************************************/
static uint16 mbyte_to_wchar(uint8 *dest, uint8 *src)
{
    uint16 num = 0;

#ifdef USE_83NAME_CREAT

    while(*src!=0x00)
    {
        *dest=*src;
        dest++;
        src++;
        num++;
    }

    dest=0x00;

#else

    *dest = 0xff;
    *(dest + 1) = 0xfe;
    dest += 2;

    while (*src != 0x00)
    {
        *dest = *src;
        *(dest + 1) = 0x00;
        dest += 2;
        src++;
        num++;
    }

    *dest = 0x00;
    *(dest + 1) = 0x00;

#endif

    return num;

}

/******************************************************************************/
/*!
 * \par  Description:
 * \void raw_write_sector(uint32 sector_num, handle fp)
 * \写一个扇区的数据到文件
 * \param[in]   sector_num 扇区序号 param1
 * \param[in]   fp 文件句柄 param2
 * \param[out]  none
 * \return      none
 * \retval      none
 * \ingroup     music_bookmark
 * \note
 *******************************************************************************/
static void raw_write_sector(uint32 sector_num, handle fp)
{
    sector_num <<= 9;
    if (drv_sn == 0)
    {
        g_bookmark_fp = VM_BOOKMARK_START + sector_num;
        sys_vm_write(temp_plist_buf, g_bookmark_fp);
    }
    else
    {
        vfs_file_write(ap_vfs_mount, temp_plist_buf, SECTOR_SIZE, fp);
    }
}

/******************************************************************************/
/*!
 * \par  Description:
 * \void raw_read_sector(uint32 sector_num, handle fp)
 * \读一个扇区的数据
 * \param[in]   sector_num 扇区序号 param1
 * \param[in]   fp 文件句柄 param2
 * \param[out]  none
 * \return      none
 * \ingroup     music_bookmark
 * \note
 *******************************************************************************/
static void raw_read_sector(uint32 sector_num, handle fp)
{
    sector_num <<= 9;
    if (drv_sn == 0)
    {
        g_bookmark_fp = VM_BOOKMARK_START + sector_num;
        sys_vm_read(temp_plist_buf, g_bookmark_fp, SECTOR_SIZE);
    }
    else
    {
        vfs_file_read(ap_vfs_mount, temp_plist_buf, SECTOR_SIZE, fp);
    }
}
/******************************************************************************/
/*!
 * \par  Description:
 * \void bool check_bookmark_exit_sub(void)
 * \检测书签文件是否存在
 * \param[in]   none
 * \param[out]  none
 * \return      none
 * \retval
 * \ingroup     music_bookmark
 * \note
 * \    对于要写入VM的书签文件，如果判断不存在，立即创建。对于写入
        卡盘的书签文件，如果文件不存在，则返回FALSE,由接下来的程序创建之 
 *******************************************************************************/
static bool check_bookmark_exit_sub(void)
{
    music_bm_head *bm_head_ptr;
    uint8 i;
    bool result = TRUE;

    if (drv_sn == 0)
    {
        raw_read_sector(0, g_bookmark_fp);
        bm_head_ptr = (music_bm_head *) temp_plist_buf;
        if (bm_head_ptr->magic != MAGIC_BOOKMARK)
        {
            libc_memset(temp_plist_buf, 0xff, SECTOR_SIZE);
            bm_head_ptr->magic = MAGIC_BOOKMARK;
            raw_write_sector(0, g_bookmark_fp);
            bm_head_ptr->magic = 0xffff;
            for (i = 1; i < (BM_HEADER_SECTOR_NUM + BM_DATA_SECTOR_NUM); i++)
            {
                raw_write_sector(i, g_bookmark_fp);
            }
        }
    }
    else
    {
        //定位到根目录
        vfs_cd(ap_vfs_mount, CD_ROOT, 0);
        mbyte_to_wchar(temp_plist_buf, (uint8 *) BOOKMARK_NAME);        
        //打开文件
        g_bookmark_fp = vfs_file_open(ap_vfs_mount, temp_plist_buf, R_NORMAL_SEEK);
        if (g_bookmark_fp == 0)
        {
            result = FALSE;
        }
        else
        {
            vfs_file_read(ap_vfs_mount, temp_plist_buf, SECTOR_SIZE, g_bookmark_fp);
            vfs_file_close(ap_vfs_mount, g_bookmark_fp);
            bm_head_ptr = (music_bm_head *) temp_plist_buf;
            if (bm_head_ptr->magic != MAGIC_BOOKMARK)
            {
                result = FALSE;
            }
        }
    }

    return result;
}

/******************************************************************************/
/*!
 * \par  Description:
 *  bool check_bookmark_exist(void)
 *	  检测书签文件手机否存在，如不存在，则会创建文件
 * \param[in]   handle *fp 如果打开成功，返回打开的句柄
 * \param[out] none
 * \return     bool
 * \retval     TRUE 文件存在 or 创建成功
 * \ingroup    music_bookmark
 * \note
 *******************************************************************************/
bool check_bookmark_exist(void)
{
    uint32 bm_fp;
    uint32 file_len;
    uint16 i;
    uint16 bound_val;
    music_bm_head *bm_head_ptr;
    bool retval = TRUE;

    if (FALSE == check_bookmark_exit_sub())
    {
        //打开文件失败
        vfs_file_dir_remove(ap_vfs_mount, temp_plist_buf, FS_FILE);
        bound_val = (BM_HEADER_SECTOR_NUM + BM_DATA_SECTOR_NUM);
        file_len = (bound_val << 9);
        //创建文件
        bm_fp = vfs_file_create(ap_vfs_mount, temp_plist_buf, file_len);
        if (0 == bm_fp)
        {
            //error to create file
            retval = FALSE;
        }
        else
        {
#ifdef HIDE_FILE_OPT        
            vfs_file_attralter(ap_vfs_mount, ATTR_HIDDEN, NULL, 0);
#endif            
            libc_memset(temp_plist_buf, 0xff, SECTOR_SIZE);
            bm_head_ptr = (music_bm_head *) temp_plist_buf;
            bm_head_ptr->magic = MAGIC_BOOKMARK;
            vfs_file_write(ap_vfs_mount, temp_plist_buf, SECTOR_SIZE, bm_fp);
            bm_head_ptr->magic = 0xffff;
            for (i = 1; i < (BM_HEADER_SECTOR_NUM + BM_DATA_SECTOR_NUM); i++)
            {
                vfs_file_write(ap_vfs_mount, temp_plist_buf, SECTOR_SIZE, bm_fp);
            }
            vfs_file_close(ap_vfs_mount, bm_fp);
        }
    }

    return retval;
}


/******************************************************************************/
/*
 * \par  Description: 读取ebook.lib中文件的名字，然后创建相应的书签文件
 *
 * \param[in]

 * \param[out]

 * \return

 * \note
 *******************************************************************************/
static void creat_bmk(void)
{
    uint32 bmk_modify_handle, file_length;
    uint8 i, j, filename_length;//count
    uint32 tmp_page_total;
    for (i = 0; i < 2; i++)
    {
        libc_memcpy(&file_buffer[(i + 1) * 0x50], &temp_plist_buf[i * PLIST_FILE_SIZE], 0x10 * 5);
    }
    for (i = 0; i < 2; i++)
    {
        temp_count++;
        if (temp_count > ebook_total)
        {
            return;
        }
        if (file_buffer[(i + 1) * 0x50] != 0)
        {
            libc_memset(file_buffer, 0x0, 0x10 * 5);
            //byte 0-63存储文件名
            libc_memcpy(file_buffer, &file_buffer[(i + 1) * 0x50], 64);
            //byte 68存储文件名的长度
            filename_length = file_buffer[((i + 1) * 0x50) + 68];
            if ((file_buffer[0] == 0xFF) && (file_buffer[1] == 0xFE))
            {
                if ((filename_length > 5) && (filename_length <= FILENAME_LENGTH))
                {
                    libc_memcpy(&file_buffer[(filename_length * 2) - 10], hex_str, 8);
                }
                else
                {
                    return;
                }
            }
            else
            {
                if (filename_length > 3)
                {
                    for (j = 8; j > 0; j--)
                    {
                        if (file_buffer[j] == 0x2e)
                        {
                            break;
                        }
                    }
                    libc_memset(&file_buffer[j], 0x20, 9 - j);
                    libc_memcpy(&file_buffer[8], str, 3);
                    file_buffer[11] = 0x0;
                }
                else
                {
                    return;
                }
            }
            //此处不用修改转为长名，因为是根据取到的文件名创建对应的书签
            bmk_modify_handle = vfs_file_dir_exist(ap_vfs_mount, file_buffer, 0x01);
            if (bmk_modify_handle == (uint32) NULL)
            {
                //byte 64-67存储文件长度
                libc_memcpy(&file_length, &file_buffer[(i + 1) * 0x50 + 64], 0x04);

                //书签文件的长度计算为扇区倍数
                tmp_page_total = file_length / 128;
                if ((file_length % 128) != 0)
                {
                    tmp_page_total++;
                }
                if ((tmp_page_total % 128) != 0)
                {
                    tmp_page_total = (tmp_page_total & 0xffffff00) + 128;
                }

                //创建一定长度的书签文件，并将第一个扇区的数据全初始化为0xFF
                bmk_modify_handle = vfs_file_create(ap_vfs_mount, file_buffer, 512 + tmp_page_total * 4);

                if (0 != bmk_modify_handle)
                {
                    libc_memset(temp_plist_buf, 0xFF, 0x200);
                    vfs_file_seek(ap_vfs_mount, 0, SEEK_SET, bmk_modify_handle);
                    vfs_file_write(ap_vfs_mount, temp_plist_buf, 0x200, bmk_modify_handle);
                    vfs_file_close(ap_vfs_mount, bmk_modify_handle);
                }
            }

        }
    }
}
/******************************************************************************/
/*
 * \par  Description: 在根目录下创建所有电子书的书签文件，每个文件对应一个书签文件
 *
 * \param[in]

 * \param[out]

 * \return

 * \note
 *******************************************************************************/
void creat_ebook_bmk(void)
{
    uint16 i;
    uint32 ebook_lib;
    //    uint8 read_sector;

    if (0 == drv_sn)
    {
        //nand介质，建表放入VM区域,此处不处理
        return;
    }

    vfs_cd(ap_vfs_mount, CD_ROOT, 0);
    mbyte_to_wchar(temp_plist_buf, (uint8 *) EBOOK_LIST_NAME);
    ebook_lib = vfs_file_open(ap_vfs_mount, temp_plist_buf, R_NORMAL_SEEK);
    if (0x00 == ebook_lib)
    {
        return;
    }
    vfs_file_read(ap_vfs_mount, temp_plist_buf, 512, ebook_lib);
    libc_memcpy(&ebook_total, &temp_plist_buf[8], 2);
    temp_count = 0;
    if (ebook_total != 0)
    {
        for (i = 0; i < ((ebook_total - 1) / 2 + 1); i++)
        {
            libc_memset(temp_plist_buf, 0x0, 512);
            vfs_file_seek(ap_vfs_mount, (i + 1) * 512, SEEK_SET, ebook_lib);
            vfs_file_read(ap_vfs_mount, temp_plist_buf, 512, ebook_lib);
            creat_bmk();

            if (0 == (i & 0x03))
            {
                plist_get_app_msg();
            }
        }
    }

    vfs_file_close(ap_vfs_mount, ebook_lib);
    return;
}

