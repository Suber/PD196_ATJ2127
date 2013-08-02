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
 * \brief    文件选择器管理
 * \author   liminxian
 * \par      GENERAL DESCRIPTION:
 *               这里对文件进行描述
 * \par      EXTERNALIZED FUNCTIONS:
 *               KERNEL,FS
 * \version 1.0
 * \date  2011/9/15
 *******************************************************************************/

#include "eh_module.h"
#include "eh_rdata_ex.h"

#ifdef USE_83NAME_CREAT
static const char userpl_name[USERPL_MAX][12]=
{   "USERPL1 PL ","USERPL2 PL ","USERPL3 PL "};
static const char plist_lib_name[PL_LIB_MAX][12]=
{   "MUSIC   LIB","AUDIBLE LIB","VIDEO   LIB","PICTURE LIB","EBOOK   LIB","M3U     LIB"};

#else
static const char userpl_name[USERPL_MAX][12] =
{ "USERPL1.PL", "USERPL2.PL", "USERPL3.PL" };
static const char plist_lib_name[PL_LIB_MAX][12] =
{ "MUSIC.LIB", "AUDIBLE.LIB", "VIDEO.LIB", "PICTURE.LIB", "EBOOK.LIB", "M3U.LIB" };

#endif

extern uint8 temp_module_buf[SECTOR_SIZE];
bool mod_open_userpl(fsel_type_e type);
bool mod_open_playlist(fsel_type_e type);

static bool open_vm_userpl(uint8 sn);

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
 * \par  Description:open favorite's *pl file
 *
 * \param[in]    fsel_type_e

 * \param[out]      null

 * \return        ture or false

 * \note
 *******************************************************************************/

bool mod_open_userpl(fsel_type_e type)
{
    userpl_head_t *upl_h;
    uint32 temp;

    upl_h = (userpl_head_t*) temp_module_buf;

    if (fsel_init_val.disk == DISK_C)
    {
        return open_vm_userpl(type - FSEL_TYPE_LISTFAVOR1);
    }

    mbyte_to_wchar(temp_module_buf, (uint8 *) &userpl_name[(type - FSEL_TYPE_LISTFAVOR1) % USERPL_MAX]);
    obj_handle = vfs_file_open(vfs_mount, temp_module_buf, R_NORMAL_SEEK);

    if (0 == obj_handle)
    {

        creat_uspl: obj_handle = vfs_file_create(vfs_mount, temp_module_buf, 0);

        if (0 != obj_handle)
        {
            libc_memset(temp_module_buf, 0, SECTOR_SIZE);
            //添加标识识别
            temp_module_buf[510] = 0x55;
            temp_module_buf[511] = 0xAA;
            libc_memcpy(temp_module_buf, userpl_name[(type - FSEL_TYPE_LISTFAVOR1) % USERPL_MAX], 8);
            vfs_file_write(vfs_mount, temp_module_buf, SECTOR_SIZE, obj_handle);
            libc_memset(temp_module_buf, 0, SECTOR_SIZE);

            for (temp = 0; temp < (USERPL_FILE_MAX / USERPL_PRE_SEC) + USERPL_HEAD_SEC; temp++)
            {
                vfs_file_write(vfs_mount, temp_module_buf, SECTOR_SIZE, obj_handle);
            }

            vfs_file_close(vfs_mount, obj_handle);

            mbyte_to_wchar(temp_module_buf, (uint8 *) &userpl_name[(type - FSEL_TYPE_LISTFAVOR1) % USERPL_MAX]);
            obj_handle = vfs_file_open(vfs_mount, temp_module_buf, R_NORMAL_SEEK);

        }
    }

    if (0 != obj_handle)
    {

#ifdef NOTE_LIB_OFFSET
        vfs_file_dir_offset(vfs_mount, NULL, &lib_pfile_offset, 0);
#endif

        vfs_file_read(vfs_mount, temp_module_buf, SECTOR_SIZE, obj_handle);
        obj_open_mode = R_NORMAL_SEEK;
        vfs_file_close(vfs_mount, obj_handle);
        obj_handle = 0;

        if ((temp_module_buf[510] != 0x55) || (temp_module_buf[511] != 0xAA))
        {
            mbyte_to_wchar(temp_module_buf, (uint8 *) &userpl_name[(type - FSEL_TYPE_LISTFAVOR1) % USERPL_MAX]);
            vfs_file_dir_remove(vfs_mount, temp_module_buf, FS_FILE);
            goto creat_uspl;
        }

    }
    else
    {
        return FALSE;
    }

    file_total = upl_h->file_num;
    uspl_file_total = file_total;
    uspl_first_offset = upl_h->first_offset;
    uspl_last_offset = upl_h->last_offset;
    uspl_file_offset = uspl_first_offset;

    return TRUE;

}

/******************************************************************************/
/*
 * \par  Description:open favorite's *pl file
 *
 * \param[in]    收藏夹编号 0，1，2

 * \param[out]      null

 * \return        ture or false

 * \note
 *******************************************************************************/

static bool open_vm_userpl(uint8 sn)
{
    uint8 temp;
    userpl_head_t *upl_h;
    uint32 uspl_vm_addr;

    upl_h = (userpl_head_t*) temp_module_buf;
    uspl_vm_addr = VM_USERPL_INFO + sn * USERPL_VM_SIZE;
    sys_vm_read(temp_module_buf, uspl_vm_addr, SECTOR_SIZE);

    if ((temp_module_buf[510] != 0x55) || (temp_module_buf[511] != 0xAA))
    {
        //表格式不对，重建
        goto creat_file;
    }

    if (0 != libc_memcmp(upl_h->file_name, &userpl_name[sn], 7))
    {
        //表格式不对，重建
        creat_file:

        libc_memset(temp_module_buf, 0, SECTOR_SIZE);
        libc_memcpy(upl_h->file_name, &userpl_name[sn], 7);
        //        upl_h->file_num = 0;
        //        upl_h->first_offset = 0;
        //        upl_h->last_offset = 0;

        //加入标识符
        temp_module_buf[510] = 0x55;
        temp_module_buf[511] = 0xAA;

        sys_vm_write(temp_module_buf, uspl_vm_addr);

        libc_memset(temp_module_buf, 0, SECTOR_SIZE);
        for (temp = 0; temp < (USERPL_FILE_MAX / USERPL_PRE_SEC) + USERPL_HEAD_SEC; temp++)
        {
            uspl_vm_addr = uspl_vm_addr + SECTOR_SIZE;
            sys_vm_write(temp_module_buf, uspl_vm_addr);
        }

        //读出头信息
        sys_vm_read(temp_module_buf, VM_USERPL_INFO + sn * USERPL_VM_SIZE, SECTOR_SIZE);
    }

    file_total = upl_h->file_num;
    uspl_file_total = file_total;
    uspl_first_offset = upl_h->first_offset;
    uspl_last_offset = upl_h->last_offset;
    uspl_file_offset = uspl_first_offset;

    obj_open_mode = 0xee;//标记是VM的句柄

    return TRUE;
}
/******************************************************************************/
/*
 * \par  Description:open playlist's *pl file
 *
 * \param[in]    favorite type :

 * \param[out]      none

 * \return         ture or false

 * \note
 *******************************************************************************/
bool mod_open_playlist(fsel_type_e type)
{
    plist_head_t *plist_h_p;
    plist_item_t * item_p;
    bool ret_val = TRUE;

    mbyte_to_wchar(temp_module_buf, (uint8 *) &plist_lib_name[(type - FSEL_TYPE_PLAYLIST) % PL_LIB_MAX]);
    obj_handle = vfs_file_open(vfs_mount, temp_module_buf, R_NORMAL_SEEK);

    if (0 != obj_handle)
    {

#ifdef NOTE_LIB_OFFSET
        vfs_file_dir_offset(vfs_mount, NULL, &lib_pfile_offset, 0);
#endif

        vfs_file_read(vfs_mount, temp_module_buf, SECTOR_SIZE, obj_handle);

        if ((temp_module_buf[510] == 0x55) && (temp_module_buf[511] == 0xAA))
        {
            plist_h_p = (plist_head_t *) temp_module_buf;

            plist_file_total = plist_h_p->file_total;
            plist_info_offset = plist_h_p->file_info_offset;

            //            if(fsel_init_val.fsel_type>FSEL_TYPE_LISTAUDIBLE )
            //            {
            //music,audible,VIDEO,picture,ebook

            //默认为ALLSONG
            dir_layer = 0;
            //            plist_list_layerno[0]=0;
            plist_layer_offset = 0;
            file_total = plist_file_total;
            plist_start_sort_index = 0;

            item_p = (plist_item_t *) (temp_module_buf + sizeof(plist_head_t));
            libc_memset(&plist_item_info, 0x00, sizeof(plist_item_info_t));
            plist_item_info.file_index_offset = item_p->file_index_offset;
            plist_item_info.sort_index_offset = item_p->sort_index_offset;

            //            }
            obj_open_mode = R_NORMAL_SEEK;
        }
        else
        {
            ret_val = FALSE;
        }

        vfs_file_close(vfs_mount, obj_handle);
        obj_handle = 0;

    }
    else
    {
        ret_val = FALSE;
    }

    return ret_val;
}

//module_init(fsel_init);


