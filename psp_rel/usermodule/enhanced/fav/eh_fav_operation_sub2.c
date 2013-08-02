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
 * \file     *.c
 * \brief    收藏夹添加，删除和清空操作
 * \author   liminxian
 * \par      GENERAL DESCRIPTION:
 *               这里对文件进行描述
 * \par      EXTERNALIZED FUNCTIONS:
 *               这里描述调用到外面的模块
 * \version 1.0
 * \date  2011/9/15
 *******************************************************************************/

#include "eh_module.h"
#include "eh_rdata_ex.h"

#ifdef USE_83NAME_CREAT
static const char userpl_name[USERPL_MAX][12]=
{   "USERPL1 PL ","USERPL2 PL ","USERPL3 PL "};
#else
static const char userpl_name[USERPL_MAX][12] =
{ "USERPL1.PL", "USERPL2.PL", "USERPL3.PL" };
#endif

extern userpl_file_t *fav_opt_user_pl_p _BANK_DATA_ATTR_;
extern userpl_head_t fav_opt_user_pl_head _BANK_DATA_ATTR_;
extern uint16 fav_opt_existfile _BANK_DATA_ATTR_;
extern uint8 fav_obj_open_mode _BANK_DATA_ATTR_;

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
 * \par  Description: 打开VRAM收藏夹
 *
 * \param[in]    file_name--收藏夹的文件名
 buf--使用的缓存buffer
 creat_flag--创建收藏夹文件标志
 * \param[out]

 * \return       文件句柄 ：非0--成功，0--失败

 * \note         读入fav_opt_user_pl_head信息
 *******************************************************************************/
handle fav_opt_open_userpl_vram(uint8 sn, uint8 *buf, uint8 creat_flag)
{
    uint8 temp;
    uint32 uspl_vm_addr;
    userpl_head_t *upl_h;

    upl_h = (userpl_head_t *) buf;
    uspl_vm_addr = VM_USERPL_INFO + sn * USERPL_VM_SIZE;
    sys_vm_read(buf, uspl_vm_addr, SECTOR_SIZE);

    if ((buf[510] != 0x55) || (buf[511] != 0xAA))
    {
        //表格式不对，重建
        goto creat_file;
    }

    if (0 != libc_memcmp(upl_h->file_name, userpl_name[sn], 7))
    {
        //表格式不对，重建
        creat_file:

        if (0 == creat_flag)
        {
            return 0;
        }

        libc_memset(buf, 0, SECTOR_SIZE);
        upl_h = (userpl_head_t*) buf;
        libc_memcpy(upl_h->file_name, userpl_name[sn], 7);
        upl_h->file_num = 0;
        upl_h->first_offset = 0;
        upl_h->last_offset = 0;

        //加入标识符
        buf[510] = 0x55;
        buf[511] = 0xAA;

        sys_vm_write(buf, uspl_vm_addr);

        libc_memset(buf, 0, SECTOR_SIZE);
        for (temp = 0; temp < (USERPL_FILE_MAX / USERPL_PRE_SEC) + USERPL_HEAD_SEC; temp++)
        {
            uspl_vm_addr = uspl_vm_addr + SECTOR_SIZE;
            sys_vm_write(buf, uspl_vm_addr);
        }

        //读出头信息
        sys_vm_read(buf, VM_USERPL_INFO + sn * USERPL_VM_SIZE, SECTOR_SIZE);
    }

    fav_obj_open_mode = 0xee;//标记是VM的句柄

    libc_memcpy(&fav_opt_user_pl_head, upl_h, sizeof(userpl_head_t));

    return (VM_USERPL_INFO + sn * USERPL_VM_SIZE);

}

/******************************************************************************/
/*
 * \par  Description: 打开收藏夹
 *
 * \param[in]    file_name--收藏夹的文件名
 buf--使用的缓存buffer
 creat_flag--创建收藏夹文件标志
 * \param[out]

 * \return       文件句柄 ：非0--成功，0--失败

 * \note         读入fav_opt_user_pl_head信息
 *******************************************************************************/
handle fav_opt_open_userpl(uint8 sn, uint8 *buf, uint8 creat_flag)
{
    handle t_handle;
    userpl_head_t *upl_h;
    uint32 temp;
    uint8 *file_name;

    if (fsel_init_val.disk == DISK_C)
    {
        return fav_opt_open_userpl_vram(sn, buf, creat_flag);
    }

    //card or host 分支

    upl_h = (userpl_head_t*) buf;
    file_name = (uint8 *) userpl_name[sn];
    fav_obj_open_mode = OPEN_MODIFY;

    open_uspl:

    //保存入口路径
    vfs_file_dir_offset(vfs_mount, buf, buf + 128, 0);
    //默认进入根目录
    vfs_cd(vfs_mount, CD_ROOT, 0);

    mbyte_to_wchar(buf + 256, (uint8 *) file_name);
    t_handle = vfs_file_open(vfs_mount, buf + 256, OPEN_MODIFY);

    if (t_handle == 0)
    {
        if (0 == creat_flag)
        {
            vfs_file_dir_offset(vfs_mount, buf, buf + 128, 1);
            return t_handle;
        }

        t_handle = vfs_file_create(vfs_mount, buf + 256, 0);

        vfs_file_dir_offset(vfs_mount, buf, buf + 128, 1);

        if (0 == t_handle)
        {
            return t_handle;
        }

        libc_memset(buf, 0, SECTOR_SIZE);
        libc_memset(&fav_opt_user_pl_head, 0, sizeof(userpl_head_t));

        libc_memcpy(fav_opt_user_pl_head.file_name, file_name, 8);
        fav_opt_user_pl_head.file_num = 0;
        fav_opt_user_pl_head.first_offset = 0;
        fav_opt_user_pl_head.last_offset = 0;

        libc_memcpy(buf, &fav_opt_user_pl_head, sizeof(userpl_head_t));

        //添加标识识别
        buf[510] = 0x55;
        buf[511] = 0xAA;

        //写入1个扇区
        vfs_file_write(vfs_mount, buf, SECTOR_SIZE, t_handle);

        libc_memset(buf, 0, SECTOR_SIZE);

        for (temp = 0; temp < (USERPL_FILE_MAX / USERPL_PRE_SEC) + USERPL_HEAD_SEC; temp++)
        {
            vfs_file_write(vfs_mount, buf, SECTOR_SIZE, t_handle);
        }

        vfs_file_close(vfs_mount, t_handle);

        vfs_file_dir_offset(vfs_mount, buf, buf + 128, 0);
        //默认进入根目录
        vfs_cd(vfs_mount, CD_ROOT, 0);
        mbyte_to_wchar(buf + 256, (uint8 *) file_name);
        t_handle = vfs_file_open(vfs_mount, buf + 256, OPEN_MODIFY);

        vfs_file_dir_offset(vfs_mount, buf, buf + 128, 1);

        if (0 == t_handle)
        {
            return 0;
        }
    }
    else
    {
        //恢复路径
        vfs_file_dir_offset(vfs_mount, buf, buf + 128, 1);

        vfs_file_read(vfs_mount, buf, SECTOR_SIZE, t_handle);

        if ((buf[510] != 0x55) || (buf[511] != 0xAA))
        {
            vfs_file_close(vfs_mount, t_handle);
            mbyte_to_wchar(buf + 256, (uint8 *) file_name);
            vfs_file_dir_remove(vfs_mount, buf + 256, FS_FILE);
            t_handle = 0;
            goto open_uspl;
        }

        libc_memcpy(&fav_opt_user_pl_head, upl_h, sizeof(userpl_head_t));
    }

    return t_handle;

}

