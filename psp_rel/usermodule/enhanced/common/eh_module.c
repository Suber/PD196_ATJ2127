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
 * \file     eh_module.c
 * \brief    文件选择器装载
 * \author   liminxian
 * \par      GENERAL DESCRIPTION:
 *               这里对文件进行描述
 * \par      EXTERNALIZED FUNCTIONS:
 *               FS,KERNEL
 * \version 1.0
 * \date  2011/9/15
 *******************************************************************************/

#include "eh_module.h"
#include "eh_rdata_ex.h"

static uint8 drv_type _BANK_DATA_ATTR_; //驱动类型
uint8 temp_module_buf[SECTOR_SIZE] _BANK_DATA_ATTR_;
//static uint8  pdir_layer_buffer[65];
//static uint8  pfile_offset_buffer[8];

extern bool mod_open_userpl(fsel_type_e type) __FAR__;
extern bool mod_open_playlist(fsel_type_e type) __FAR__;

static void init_sysdata(void);
//bool fsel_exit(void);

static const uint8 nand_drv_name[] = "nand.drv";
static const uint8 card_drv_name[2][11] =
{ "card.drv", "cards.drv" };
static const uint8 uhost_drv_name[] = "uhost.drv";

/******************************************************************************/
/*
 * \par  Description:enhanced 模块的初始化
 *
 * \param[in]    init_param 初始化参数结构体
 * \param[in]
 * \param[in]    drv_mode  存储介质驱动模式

 * \param[out]      vfs标识号

 * \return        vfs标识号(0~2有效，-1无效)

 * \note
 *******************************************************************************/
int32 fsel_init(fsel_param_t *init_param, uint8 drv_mode)
{
    //    uint8 i;
    bool ret_val;
    uint8 *drv_name;

    fsel_error = FSEL_NO_ERR;
    vfs_mount = -1;

    if (init_param == NULL)
    {
        return -1;
    }

    libc_memcpy(&fsel_init_val, init_param, sizeof(fsel_param_t));

    fsel_init_val.fsel_mode &= 0x0f;

    if ((init_param->fsel_mode & 0xf0) != 0)
    {
        shuffle_sw = TRUE;
    }
    else
    {
        shuffle_sw = FALSE;
    }

    if (init_param->disk <= DISK_C)
    {
        drv_type = DRV_GROUP_STG_BASE;
        drv_name = (uint8 *) nand_drv_name;
    }
    else if (init_param->disk == DISK_H)
    {
        drv_type = DRV_GROUP_STG_CARD;
        drv_name = (uint8 *) &card_drv_name[drv_mode & 0x01];
    }
    else if (init_param->disk == DISK_U)
    {
        drv_type = DRV_GROUP_STG_UHOST;
        drv_name = (uint8 *) uhost_drv_name;
    }
    else
    {
        fsel_error = FSEL_ERR_DISK;
        return -1;
    }

    //安装存储驱动
    if (sys_drv_install(drv_type, drv_mode, drv_name) != 0)
    {
        fsel_error = FSEL_ERR_STG;
        return -1;
    }

    //挂载FS
    vfs_mount = sys_mount_fs(drv_type, fsel_init_val.disk, 0);

    if (vfs_mount == -1)
    {
        sys_drv_uninstall(drv_type);
        fsel_error = FSEL_ERR_FS;
        return -1;
    }

    obj_handle = 0;
    init_sysdata();
    m3u_curlist_index = 0xffff;

    switch (init_param->fsel_type)
    {
        case FSEL_TYPE_PLAYLIST:
        case FSEL_TYPE_LISTAUDIBLE:
        case FSEL_TYPE_LISTVIDEO:
        case FSEL_TYPE_LISTPICTURE:
        case FSEL_TYPE_LISTEBOOK:
        case FSEL_TYPE_M3ULIST:
        {
            fsel_module_type = FSEL_MODULE_PLIST;
            ret_val = mod_open_playlist(init_param->fsel_type);
            break;
        }
        case FSEL_TYPE_LISTFAVOR1:
        case FSEL_TYPE_LISTFAVOR2:
        case FSEL_TYPE_LISTFAVOR3:
        {
            fsel_module_type = FSEL_MODULE_FAVOR;
            ret_val = mod_open_userpl(init_param->fsel_type);
            break;
        }
        case FSEL_TYPE_COMMONDIR:
        case FSEL_TYPE_DISKSEQUNCE:
        default:
        {
            fsel_module_type = FSEL_MODULE_DIR;
            ret_val = TRUE;
            break;
        }
    }

    if (ret_val == FALSE)
    {
        sys_unmount_fs(vfs_mount);
        sys_drv_uninstall(drv_type);
        fsel_error = FSEL_ERR_LIST_NOEXIST;
        vfs_mount = -1;
    }

    return vfs_mount;

}

/******************************************************************************/
/*
 * \par  Description:初始化enhanced 公共参数
 *
 * \param[in]    none

 * \param[out]      none

 * \return        none

 * \note
 *******************************************************************************/
static void init_sysdata(void)
{
    dir_layer = 0;
    root_dir = 0;
    file_no = 0;

    save_read_offset = 0xffff; //
    write_data_flag = FALSE; //

    //默认进入根目录
    vfs_cd(vfs_mount, CD_ROOT, 0);
}

/******************************************************************************/
/*
 * \par  Description: 退出enhanced
 *
 * \param[in]    none

 * \param[out]      none

 * \return        true or false

 * \note
 *******************************************************************************/
bool fsel_exit(void)
{
    int32 ret_val = 0;

    fsel_error = FSEL_NO_ERR;

    if (0 != obj_handle)
    {
        vfs_file_close(vfs_mount, obj_handle);
        obj_handle = 0;
    }

    //     vfs_mount=-1;

    //ret_val=sys_unmount_fs(disk_num);
    //
    if (vfs_mount != -1)
    {
        ret_val = sys_unmount_fs(vfs_mount);
        vfs_mount = -1;
    }
    else
    {
        return TRUE;
    }

    if (0 > ret_val)
    {
        fsel_error = FSEL_ERR_FS;
        return FALSE;
    }

    if (fsel_init_val.disk <= DISK_C)
    {
        drv_type = DRV_GROUP_STG_BASE;
    }
    else if (fsel_init_val.disk == DISK_H)
    {
        drv_type = DRV_GROUP_STG_CARD;
    }
    else if (fsel_init_val.disk == DISK_U)
    {
        drv_type = DRV_GROUP_STG_UHOST;
    }
    else
    {
        fsel_error = FSEL_ERR_DISK;
        return -1;
    }

    ret_val = sys_drv_uninstall(drv_type);

    if (0 > ret_val)
    {
        fsel_error = FSEL_ERR_STG;
        return FALSE;
    }

    return TRUE;
}

/******************************************************************************/
/*
 * \par  Description:
 *
 * \param[in]

 * \param[out]

 * \return

 * \note
 *******************************************************************************/
fsel_error_e fsel_get_error(void)
{
    return fsel_error;
}

//module_init(fsel_init);


