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
 * \file     eh_bs_entry.c
 * \brief    浏览接口总入口
 * \author   liminxian
 * \par      GENERAL DESCRIPTION:
 *               从接口入口
 * \par      EXTERNALIZED FUNCTIONS:
 *               文件系统
 * \version 1.0
 * \date  2011/9/15
 *******************************************************************************/

#include "../eh_bs.h"

uint8 bs_temp_buf[SECTOR_SIZE] _BANK_DATA_ATTR_;

extern bool bs_dir_enter(brow_dir_e type, uint16 list_no, dir_brow_t *dir_brow);
extern bool bs_plist_enter(brow_dir_e type, uint16 list_no, dir_brow_t *dir_brow);
extern bool bs_uspl_enter(brow_dir_e type, uint16 list_no, dir_brow_t *dir_brow);

extern bool bs_dir_get_items(brow_decode_t *decode_param, void *file_list);
extern bool bs_plist_get_items(brow_decode_t *decode_param, void *file_list);
extern bool bs_uspl_get_items(brow_decode_t *decode_param, void *file_list);

extern bool bs_dir_get_location(file_location_t *location, fsel_type_e type);
extern bool bs_plist_get_location(plist_location_t *location, fsel_type_e type);
extern bool bs_uspl_get_location(flist_location_t*location, fsel_type_e type);

extern bool bs_dir_set_location(file_location_t *location, fsel_type_e type);
extern bool bs_plist_set_location(plist_location_t *location, fsel_type_e type);
extern bool bs_uspl_set_location(flist_location_t*location, fsel_type_e type);

extern bool bs_dir_delete(brow_del_e type, uint16 list_no, file_location_t *location);
extern bool bs_plist_delete(brow_del_e type, uint16 list_no, plist_location_t *location);
extern bool bs_uspl_delete(brow_del_e type, uint16 list_no, flist_location_t *location);

extern bool bs_dir_set_file(file_location_t *location, fsel_type_e type);
extern bool bs_plist_set_file(plist_location_t *location, fsel_type_e type);
extern bool bs_uspl_set_file(flist_location_t*location, fsel_type_e type);

extern bool bs_plist_get_albums(brow_decode_t *decode_param, void *album_list);

typedef bool (*fsel_bs_enter_f)(brow_dir_e type, uint16 list_no, dir_brow_t *dir_brow);
typedef bool (*fsel_bs_get_items_f)(brow_decode_t *decode_param, void *file_list);
typedef bool (*fsel_bs_get_location_f)(void *location, fsel_type_e type);
typedef bool (*fsel_bs_del_f)(brow_del_e type, uint16 list_no, void *location);
typedef bool (*fsel_bs_set_location_f)(void *location, fsel_type_e type);
typedef bool (*fsel_browser_set_file_f)(void *location, fsel_type_e type);

static const fsel_bs_enter_f fsel_bs_enter_func[3] =
{ bs_dir_enter, bs_plist_enter, bs_uspl_enter };
static const fsel_bs_get_items_f fsel_bs_get_items_func[3] =
{ bs_dir_get_items, bs_plist_get_items, bs_uspl_get_items };
static const fsel_bs_get_location_f fsel_bs_get_location_func[3] =
{ (void *) bs_dir_get_location, (void *) bs_plist_get_location, (void *) bs_uspl_get_location };
static const fsel_bs_del_f fsel_bs_del_func[3] =
{ (void *) bs_dir_delete, (void *) bs_plist_delete, (void *) bs_uspl_delete };
static const fsel_bs_set_location_f fsel_bs_set_location_func[3] =
{ (void *) bs_dir_set_location, (void *) bs_plist_set_location, (void *) bs_uspl_set_location };

static const fsel_browser_set_file_f fsel_browser_set_file_func[3] =
{ (void *) bs_dir_set_file, (void *) bs_plist_set_file, (void *) bs_uspl_set_file };

/******************************************************************************/
/*
 * \par  Description:操作文件选择器的目录
 *
 * \param[in]    type--操作方法
 CD_UP     改变当前目录到上一级父目录，目录项指针指向目录起始位置；
 CD_BACK   改变当前目录到上一级父目录，目录项指针指向之前CD进去的子目录；
 CD_ROOT   改变当前目录到根目录；
 CD_SUB    改变当前目录到当前目录项对应的子目录，此时参数

 list_no--列表项（1~N）

 * \param[out]   dir_brow--返回目录信息

 * \return       TRUE OR FALSE

 * \note
 *******************************************************************************/
bool fsel_browser_enter_dir(brow_dir_e type, uint16 list_no, dir_brow_t *dir_brow)
{
    fsel_error = FSEL_NO_ERR;
    return fsel_bs_enter_func[fsel_module_type](type, list_no, dir_brow);
}

/******************************************************************************/
/*
 * \par  Description:浏览时，读取列表中若干个信息项
 *
 * \param[in]    decode_param -- 浏览控制方式（向前或后浏览，信息项个数）

 * \param[out]   file_list--返回获取到的信息项内容，是file_browse_t的数组

 * \return       TRUE OR FALSE

 * \note
 *******************************************************************************/
bool fsel_browser_get_items(brow_decode_t *decode_param, file_brow_t *file_list)
{
    fsel_error = FSEL_NO_ERR;

    if ((decode_param == NULL) || (file_list == NULL))
    {
        return FALSE;
    }

    if ((decode_param->top == 0) || (decode_param->num == 0))
    {
        return FALSE;
    }

    return fsel_bs_get_items_func[fsel_module_type](decode_param, file_list);
}

/******************************************************************************/
/*
 * \par  Description:播放列表专辑图片浏览时，读取列表中若干个信息项
 *
 * \param[in]    decode_param -- 浏览控制方式（向前或后浏览，信息项个数）

 * \param[out]   album_list--返回获取到的信息项内容，是file_browse_t的数组

 * \return       TRUE OR FALSE

 * \note
 *******************************************************************************/
bool fsel_browser_get_albums(brow_decode_t *decode_param, album_brow_t *album_list)
{
    fsel_error = FSEL_NO_ERR;

    if (fsel_module_type == FSEL_MODULE_PLIST)
    {
        return bs_plist_get_albums(decode_param, album_list);
    }

    return FALSE;
}

/******************************************************************************/
/*
 * \par  Description: 浏览时，选定当前列表某个文件（列表或目录）
 *
 * \param[in]    list_no--列表项（1~N）

 * \param[out]

 * \return       TRUE OR FALSE

 * \note         当要调fsel_browser_get_location，必须先调该接口，使文件选择器先定位
 当list_no=0xffff，选择当前列表下所有文件播放
 *******************************************************************************/
bool fsel_browser_select(uint16 list_no)
{
    fsel_error = FSEL_NO_ERR;
    if ((list_no == 0xffff) || (list_no <= (bs_file_total + bs_dir_total)))
    {
        bs_select_listno = list_no;
    }
    else
    {
        return FALSE;
    }

    /*
     if(fsel_module_type==FSEL_MODULE_PLIST)
     {
     list_layer_no[dir_layer]=list_no;

     }
     */
    return TRUE;
}

/******************************************************************************/
/*
 * \par  Description: 浏览时，根据列表序号或location删除当前列表下的文件
 *
 * \param[in]    type     -- 取值为  FSEL_BROW_DEL_FILE  //删除指定的文件
 list_no  -- 列表项（1~N）
 location -- 文件的location结构（没有文件序号时即list_no=0，删除中使用）

 * \param[out]

 * \return

 * \note
 *******************************************************************************/
bool fsel_browser_delete(brow_del_e type, uint16 list_no, void *location)
{
    fsel_error = FSEL_NO_ERR;
    if ((list_no == 0) && (location == NULL))
    {
        return FALSE;
    }

    return fsel_bs_del_func[fsel_module_type](type, list_no, location);
}

/******************************************************************************/
/*
 * \par  Description:  取当前选择文件或列表位置
 *
 * \param[in]    location -- 存储当前选择文件的位置信息
 type --location类型（no used）
 * \param[out]

 * \return       TRUE OR FALSE

 * \note   调此接口前必须先调fsel_browser_select(uint16 list_no)，确定对象
 *******************************************************************************/
bool fsel_browser_get_location(void *location, fsel_type_e type)
{
    fsel_error = FSEL_NO_ERR;

    if ((0x00 == bs_select_listno) || (NULL == location))
    {
        return FALSE;
    }

    return fsel_bs_get_location_func[fsel_module_type](location, type);
}

/******************************************************************************/
/*
 * \par  Description: 设置当前的浏览文件选择位置
 *
 * \param[in]    location -- 浏览文件的位置信息
 type --location类型（no used）

 * \param[out]

 * \return       TRUE OR FALSE

 * \note
 *******************************************************************************/
bool fsel_browser_set_location(void *location, fsel_type_e type)
{
    fsel_error = FSEL_NO_ERR;

    if (location == NULL)
    {
        return FALSE;
    }

    return fsel_bs_set_location_func[fsel_module_type](location, type);
}

/******************************************************************************/
/*
 * \par  Description: 根据bitmap重新统计当前目录 修改location的file_total和file_num值
 *
 * \param[in]    location -- 选择文件的位置信息
 bitmap  --  选择文件的类型bitmap

 * \param[out]

 * \return       TRUE OR FALSE

 * \note
 *******************************************************************************/
bool fsel_browser_modify_location(file_location_t *location, uint32 bitmap)
{
    uint16 t_file_no = 0;
    uint16 t_total = 0;
    uint8 dir_type = DIR_HEAD;

    fsel_error = FSEL_NO_ERR;
    //借bs_temp_buf缓存

    if (location == NULL)
    {
        return FALSE;
    }

    vfs_file_dir_offset(vfs_mount, &bs_temp_buf[128], &bs_temp_buf[16], 0);

    while (0 != vfs_dir(vfs_mount, dir_type, NULL, bitmap))
    {
        t_total++;
        if (0 == t_file_no)
        {
            //借bs_temp_buf缓存 8字节
            vfs_file_dir_offset(vfs_mount, NULL, &bs_temp_buf, 0);
            if (libc_memcmp(&location->cluster_no, bs_temp_buf, 8) == 0)
            {
                t_file_no = t_total;
            }
        }
        dir_type = DIR_NEXT;
    }

    vfs_file_dir_offset(vfs_mount, &bs_temp_buf[128], &bs_temp_buf[16], 1);

    if ((t_file_no != 0) && (t_total != 0))
    {
        location->file_total = t_total;
        location->file_num = t_file_no;
        return TRUE;
    }
    else
    {
        return FALSE;
    }

}

/******************************************************************************/
/*
 * \par  Description: 定位到文件在磁盘的目录项位置
 *
 * \param[in]    location -- 文件的位置信息
 type --location类型（no used）

 * \param[out]

 * \return       TRUE OR FALSE

 * \note
 *******************************************************************************/
bool fsel_browser_set_file(void *location, fsel_type_e type)
{
    if (NULL == location)
    {
        return FALSE;
    }
    return fsel_browser_set_file_func[fsel_module_type](location, type);

}

/******************************************************************************/
/*
 * \par  Description: dir 文件和文件夹
 *
 * \param[in]    dir_type --dir操作
 object_type --对象类型

 * \param[out]

 * \return       TRUE OR FALSE

 * \note
 *******************************************************************************/
//dir 接口封装
bool bs_dir_function(uint8 dir_type, uint32 object_type)
{
    //   uint8 name[4];

    //若要切换目录，请在接口前准备好
    cur_ext_name = vfs_dir(vfs_mount, (uint32) dir_type, NULL, object_type);
    if (0 == cur_ext_name)
    {
        fsel_error = FSEL_ERR_OUTOF;
        return FALSE;
    }
    return TRUE;
}

/******************************************************************************/
/*
 * \par  Description: 定位到指定的dir位置
 *
 * \param[in]    dir_num--from (1~N)
 object_type --对象类型
 * \param[out]

 * \return       TRUE OR FALSE

 * \note
 *******************************************************************************/
//定位到指定的目录项位置
bool bs_dir_set_dirpos(uint32 object_type, uint16 dir_num)
{
    //    uint8 name[4];
    uint8 t_dir_type;

    //若要切换目录，请在接口前准备好

    if (dir_num == 0)
    {
        return FALSE;
    }

    t_dir_type = DIR_HEAD;

    for (; dir_num > 0; dir_num--)
    {
        if (FALSE == bs_dir_function(t_dir_type, object_type))
        {
            fsel_error = FSEL_ERR_OUTOF;
            return FALSE;
        }
        t_dir_type = DIR_NEXT;
    }

    return TRUE;
}

/******************************************************************************/
/*
 * \par  Description: 读VM指定的扇区
 *
 * \param[in]    sector_offset--扇区偏移，0~N

 * \param[out]   NULL

 * \return       NULL

 * \note 打开的文件不支持往回seek
 *******************************************************************************/
bool bs_read_sector_vram(uint16 sector_offset)
{
    if (save_read_offset != sector_offset)
    {
        if ((FALSE != write_data_flag) && (save_read_offset != 0xffff))
        {
            //回写VRAM，obj_handle存放VM地址
            sys_vm_write(bs_temp_buf, obj_handle + save_read_offset * SECTOR_SIZE);
        }

        sys_vm_read(bs_temp_buf, obj_handle + sector_offset * SECTOR_SIZE, SECTOR_SIZE);
        save_read_offset = sector_offset;

    }

    return TRUE;
}

/******************************************************************************/
/*
 * \par  Description: 读文件指定的扇区
 *
 * \param[in]    sector_offset--扇区偏移，0~N

 * \param[out]   NULL

 * \return       NULL

 * \note 打开的文件不支持往回seek
 *******************************************************************************/
bool bs_read_sector_data(uint16 sector_offset)
{
    uint16 seek_type;
    int16 seek_sector;

    if (0xee == obj_open_mode)
    {
        return bs_read_sector_vram(sector_offset);
    }

    if (save_read_offset != sector_offset)
    {
        if ((save_read_offset == 0xffff) || (save_read_offset > sector_offset))
        { //第一次读 或 往后读时使用
            seek_type = SEEK_SET;
            seek_sector = sector_offset;
        }
        else
        {
            //往前读时使用
            seek_type = SEEK_CUR;
            //seek_sector的值可能为0，情况为连续读扇区
            seek_sector = sector_offset - save_read_offset - 1;//当文件指针其实为save_read_offset+1
        }

        if (write_data_flag && (save_read_offset != 0xffff))
        {
            vfs_file_seek(vfs_mount, save_read_offset * SECTOR_SIZE, SEEK_SET, obj_handle);
            vfs_file_write(vfs_mount, bs_temp_buf, SECTOR_SIZE, obj_handle);
        }

        if ((seek_sector != 0) || (seek_type == SEEK_SET))//若是接着读下一个扇区就不需要seek了
        {
            vfs_file_seek(vfs_mount, seek_sector * SECTOR_SIZE, seek_type, obj_handle);
        }
        vfs_file_read(vfs_mount, bs_temp_buf, SECTOR_SIZE, obj_handle);
        save_read_offset = sector_offset;

    }

    return TRUE;
}

