/*******************************************************************************
 *                              US212A
 *                            Module: ENHANCED
 *                 Copyright(c) 2003-2011 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       liminxian  2011-9-15 15:37    1.0             build this file
 *******************************************************************************/
/*!
 * \file     eh_bs_ebook_deal.c
 * \brief    ebook处理文件
 * \author   liminxian
 * \par      GENERAL DESCRIPTION:
 *               删除ebook时，删除其对应的书签文件
 * \par      EXTERNALIZED FUNCTIONS:
 *               文件系统
 * \version 1.0
 * \date  2011/9/15
 *******************************************************************************/

#include "eh_bs.h"

static uint8 bs_ebook_buf[SECTOR_SIZE] _BANK_DATA_ATTR_;

static const uint8 hex_str[8] =
{ 0x2E, 0x0, 0x42, 0x0, 0x4D, 0x0, 0x4B, 0x0 };//.bmk字符
static const uint8 bmk_str[3] =
{ 0x42, 0x4D, 0x4B };//".bmk"字符

/********************************************************************************
 * Description :获取需要创建的书签名字
 *
 * Arguments  :
 * Returns     :
 *                无
 * Notes       :
 *			  字符名字的长度必须跟ap_play_list中创建书签的长度一样，否则
 *			  会在应用中重新创建另一个长度不同的书签文件。
 ********************************************************************************/
static bool _get_bmk_name(uint8 *name_buf, uint16 filename_length)
{
    //判断文件名是否为长名
    if ((name_buf[0] == 0xFF) && (name_buf[1] == 0xFE))
    {
        if ((filename_length > 7) && (filename_length <= FILENAME_LENGTH))
        {
            //获取需要创建书签的长名
            libc_memcpy(&name_buf[filename_length * 2 - 10], hex_str, 8);
        }
        else
        {
            return FALSE;
        }
    }
    else
    {
        if ((filename_length > 4) && (filename_length <= (FILENAME_LENGTH * 2)))
        {
            //获取需要创建书签的短名
            libc_memcpy(&name_buf[filename_length - 3], bmk_str, 3);
        }
        else
        {
            return FALSE;
        }

    }

    return TRUE;
}

/********************************************************************************
 * Description :删除ebook的书签
 *
 * Arguments  :
 * Returns    :
 *                无
 * Notes      :
 *			  字符名字的长度必须跟ap_play_list中创建书签的长度一样，否则
 *			  会在应用中重新创建另一个长度不同的书签文件
 ********************************************************************************/
bool bs_del_ebookbmk(void)
{
    bool result;
    uint8 *bmk_buf;
    uint16 name_length;

    if (fsel_init_val.disk == DISK_C)
    {
        //nand 介质，VRAM上的书签不处理
        return FALSE;
    }

    //    使用bs_ebookbmk_buf的256~511空间
    bmk_buf = (uint8 *) (bs_ebook_buf + 256);

    //根据当前位置的ebook，整理其对应书签的文件名，
    name_length = vfs_get_name(vfs_mount, bmk_buf, FILENAME_LENGTH);
    //获取书签文件名
    result = _get_bmk_name(bmk_buf, name_length);

    if (FALSE != result)
    {
        vfs_file_dir_offset(vfs_mount, bs_ebook_buf, bs_ebook_buf + 128, 0);
        //定位到根目录,BMK默认在根目录
        vfs_cd(vfs_mount, CD_ROOT, 0);
        result = vfs_file_dir_remove(vfs_mount, bmk_buf, FS_FILE);

        vfs_file_dir_offset(vfs_mount, bs_ebook_buf, bs_ebook_buf + 128, 1);

    }

    return result;

}

