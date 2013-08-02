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

//创建收藏夹文件使用
#ifdef USE_83NAME_CREAT
static const char userpl_name[USERPL_MAX][12]=
{   "USERPL1 PL ","USERPL2 PL ","USERPL3 PL "};
#else
static const char userpl_name[USERPL_MAX][12] =
{ "USERPL1.PL", "USERPL2.PL", "USERPL3.PL" };
#endif

static bool creat_vm_userpl_table(void);
extern void reflash_userpl_table(uint8 *buf, uint32 uspl_hdl) __FAR__;

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
/*
 * \par  Description: 创建收藏夹文件
 *
 * \param[in]

 * \param[out]

 * \return

 * \note
 *******************************************************************************/
bool creat_userpl_table(void)
{
    handle ret_hdl;
    uint8 tmp_sn, temp;
    userpl_head_t *tmp_user_pl_head;
    uint32 temp32 = 0;

    if (0 == drv_sn)
    {
        //nand介质，建表放入VM区域
        return creat_vm_userpl_table();
    }

    //card,uhost分支流程

    //先清offset
    //    libc_memset(plist_lib_offset[PL_LIB_MAX], 0xff, USERPL_MAX * 8);

    tmp_user_pl_head = (userpl_head_t*) temp_plist_buf;

    for (tmp_sn = 0; tmp_sn < USERPL_MAX; tmp_sn++)
    {
        //判断文件会改变路径，必须每次回到根目录
        vfs_cd(ap_vfs_mount, CD_ROOT, 0);
        mbyte_to_wchar(temp_plist_buf, (uint8*) &userpl_name[tmp_sn]);
        ret_hdl = vfs_file_open(ap_vfs_mount, temp_plist_buf, R_NORMAL_SEEK);

        if (ret_hdl != 0)
        {
            //判断文件是否改变大小了
            vfs_file_get_size(ap_vfs_mount, &temp32, ret_hdl, 0);
            if (temp32 < (((USERPL_FILE_MAX / USERPL_PRE_SEC) + USERPL_HEAD_SEC) * SECTOR_SIZE))
            {
                //收藏夹文件变大了，需要重新创建
                vfs_file_close(ap_vfs_mount, ret_hdl);
                ret_hdl = 0;
            }
        }

        creat_file: if (ret_hdl == 0)
        {
            vfs_file_dir_remove(ap_vfs_mount, temp_plist_buf, FS_FILE);
            ret_hdl = vfs_file_create(ap_vfs_mount, temp_plist_buf, 0);

            if (0 != ret_hdl)
            {

#ifdef HIDE_FILE_OPT
                vfs_file_attralter(ap_vfs_mount, ATTR_HIDDEN, ret_hdl, 0);
#endif
                libc_memset(temp_plist_buf, 0, SECTOR_SIZE);

                libc_memcpy(tmp_user_pl_head->file_name, &userpl_name[tmp_sn], 7);
                //                tmp_user_pl_head->file_num = 0;
                //                tmp_user_pl_head->first_offset = 0;
                //                tmp_user_pl_head->last_offset = 0;

                //加入标识符
                temp_plist_buf[510] = 0x55;
                temp_plist_buf[511] = 0xAA;

                vfs_file_write(ap_vfs_mount, temp_plist_buf, SECTOR_SIZE, ret_hdl);
                libc_memset(temp_plist_buf, 0, SECTOR_SIZE);

                for (temp = 0; temp < ((USERPL_FILE_MAX / USERPL_PRE_SEC) + USERPL_HEAD_SEC); temp++)
                {
                    vfs_file_write(ap_vfs_mount, temp_plist_buf, SECTOR_SIZE, ret_hdl);
                }
                vfs_file_close(ap_vfs_mount, ret_hdl);
            }

        }
        else
        {
            vfs_file_read(ap_vfs_mount, temp_plist_buf, SECTOR_SIZE, ret_hdl);

            if ((temp_plist_buf[510] != 0x55) || (temp_plist_buf[511] != 0xAA))
            {
                //表格式不对，重建
                mbyte_to_wchar(temp_plist_buf, (uint8*) &userpl_name[tmp_sn]);
                vfs_file_close(ap_vfs_mount, ret_hdl);
                ret_hdl = 0;
                goto creat_file;
            }

            reflash_userpl_table(temp_plist_buf, ret_hdl);
            vfs_file_close(ap_vfs_mount, ret_hdl);
        }

    }
    return TRUE;
}

/******************************************************************************/
/*
 * \par  Description: 创建收藏夹文件，放在VM空间
 *
 * \param[in]

 * \param[out]

 * \return

 * \note
 *******************************************************************************/
static bool creat_vm_userpl_table(void)
{
    uint8 tmp_sn, temp;
    userpl_head_t *tmp_user_pl_head;
    uint32 uspl_vm_addr;

    tmp_user_pl_head = (userpl_head_t*) temp_plist_buf;

    for (tmp_sn = 0; tmp_sn < USERPL_MAX; tmp_sn++)
    {
        uspl_vm_addr = VM_USERPL_INFO + (uint32) tmp_sn * USERPL_VM_SIZE;
        sys_vm_read(temp_plist_buf, uspl_vm_addr, SECTOR_SIZE);

        if ((temp_plist_buf[510] != 0x55) || (temp_plist_buf[511] != 0xAA))
        {
            //表格式不对，重建
            goto creat_file;
        }

        if (0 == libc_memcmp(tmp_user_pl_head->file_name, &userpl_name[tmp_sn], 7))
        {
            //表格正确，next
            reflash_userpl_table(temp_plist_buf, uspl_vm_addr);
            continue;
        }

        creat_file:

        libc_memset(temp_plist_buf, 0, SECTOR_SIZE);
        tmp_user_pl_head = (userpl_head_t*) temp_plist_buf;
        libc_memcpy(tmp_user_pl_head->file_name, &userpl_name[tmp_sn], 7);
        //        tmp_user_pl_head->file_num = 0;
        //        tmp_user_pl_head->first_offset = 0;
        //        tmp_user_pl_head->last_offset = 0;

        //加入标识符
        temp_plist_buf[510] = 0x55;
        temp_plist_buf[511] = 0xAA;

        sys_vm_write(temp_plist_buf, uspl_vm_addr);

        libc_memset(temp_plist_buf, 0, SECTOR_SIZE);
        for (temp = 0; temp < ((USERPL_FILE_MAX / USERPL_PRE_SEC) + USERPL_HEAD_SEC); temp++)
        {
            uspl_vm_addr = uspl_vm_addr + SECTOR_SIZE;
            sys_vm_write(temp_plist_buf, uspl_vm_addr);
        }

    }

    return TRUE;

}

