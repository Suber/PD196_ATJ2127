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
 * \brief    播放定位总入口
 * \author   liminxian
 * \par      GENERAL DESCRIPTION:
 *               这里对文件进行描述
 * \par      EXTERNALIZED FUNCTIONS:
 *               FS
 * \version 1.0
 * \date  2011/9/15
 *******************************************************************************/

#include "../eh_fsel.h"

extern bool dir_get_next(uint8 *strfile);
extern bool dir_get_prev(uint8 *strfile);
extern bool dir_get_byno(uint8 *strfile, uint16 num);
extern bool dir_set_location(file_location_t *location, fsel_type_e type);
extern bool dir_get_location(file_location_t *location, fsel_type_e type);

extern bool dir_next_dir(uint8 *strdir);
extern bool dir_prev_dir(uint8 *strdir);
//extern bool dir_enter_dirparent(void);
//extern bool dir_enter_dirson(char *sub_name);
extern bool dir_enter_dir(uint8 mode, char *dir_name);
extern bool dir_del_one(uint16 del_num);

extern bool userpl_get_next(uint8 *strfile);
extern bool userpl_get_prev(uint8 *strfile);
extern bool userpl_get_byno(uint8 *strfile, uint16 num);
extern bool userpl_set_location(flist_location_t *location, fsel_type_e type);
extern bool userpl_get_location(flist_location_t *location, fsel_type_e type);
extern bool userpl_del_one(uint16 del_num);

extern bool plist_get_next(uint8 *strfile);
extern bool plist_get_prev(uint8 *strfile);
extern bool plist_get_byno(uint8 *strfile, uint16 num);
extern bool plist_set_location(plist_location_t *location, fsel_type_e type);
extern bool plist_get_location(plist_location_t *location, fsel_type_e type);
extern bool plist_del_one(uint16 del_num);

extern uint16 total_file_cur(uint8* dir_info, uint8 check_flag);

typedef bool (*fsel_get_next_f)(uint8 *strfile);
typedef bool (*fsel_get_prev_f)(uint8 *strfile);
typedef bool (*fsel_get_byno_f)(uint8 *strfile, uint16 num);
typedef bool (*fsel_set_location_f)(void *location, fsel_type_e type);
typedef bool (*fsel_get_location_f)(void *location, fsel_type_e type);
typedef bool (*fsel_del_one_f)(uint16 del_num);

static const fsel_get_next_f fsel_get_next_func[3] =
{ dir_get_next, plist_get_next, userpl_get_next };
static const fsel_get_prev_f fsel_get_prev_func[3] =
{ dir_get_prev, plist_get_prev, userpl_get_prev };
static const fsel_get_byno_f fsel_get_byno_func[3] =
{ dir_get_byno, plist_get_byno, userpl_get_byno };
static const fsel_set_location_f fsel_set_location_func[3] =
{ (void *) dir_set_location, (void *) plist_set_location, (void *) userpl_set_location };
static const fsel_get_location_f fsel_get_location_func[3] =
{ (void *) dir_get_location, (void *) plist_get_location, (void *) userpl_get_location };
static const fsel_del_one_f fsel_del_one_func[3] =
{ dir_del_one, plist_del_one, userpl_del_one };

uint8 fsel_temp_buf[SECTOR_SIZE] _BANK_DATA_ATTR_;//fsel 数据buffer
pdir_layer_t pdir_layer_buffer _BANK_DATA_ATTR_;
pfile_offset_t pfile_offset_buffer _BANK_DATA_ATTR_;

/******************************************************************************/
/*
 * \par  Description:获取下一个文件
 *
 * \param[in]    none

 * \param[out]      strfile 存放文件扩展名

 * \return        true or false

 * \note
 *******************************************************************************/
bool fsel_get_nextfile(char *strfile)
{
    fsel_error = FSEL_NO_ERR;
    return fsel_get_next_func[fsel_module_type](strfile);
}

/******************************************************************************/
/*
 * \par  Description:获取上一个文件
 *
 * \param[in]    none

 * \param[out]   strfile 存放文件扩展名

 * \return         true or false

 * \note
 *******************************************************************************/
bool fsel_get_prevfile(char *strfile)
{
    fsel_error = FSEL_NO_ERR;
    return fsel_get_prev_func[fsel_module_type](strfile);
}

/******************************************************************************/
/*
 * \par  Description:按指定序号获取文件
 *
 * \param[in]    num 文件排列序号(1~...)

 * \param[out]      strfile 存放文件扩展名

 * \return         true or false

 * \note
 *******************************************************************************/
bool fsel_get_byno(char *strfile, uint16 num)
{
    fsel_error = FSEL_NO_ERR;

    if (num == 0)
    {
        return FALSE;
    }
    return fsel_get_byno_func[fsel_module_type](strfile, num);
}

/******************************************************************************/
/*
 * \par  Description:当前目录或列表的文件总个数
 *
 * \param[in]    none

 * \param[out]      none

 * \return       文件总个数

 * \note
 *******************************************************************************/
uint16 fsel_get_total(void)
{
    fsel_error = FSEL_NO_ERR;
    if ((file_total == 0) && (fsel_module_type == FSEL_MODULE_DIR))
    {
        file_total = total_file_cur(NULL, 0);
    }

    return file_total;
}

/******************************************************************************/
/*
 * \par  Description:获取当前选择文件的序号
 *
 * \param[in]    none

 * \param[out]      none

 * \return        序号(1~...)

 * \note
 *******************************************************************************/
uint16 fsel_get_fileno(void)
{
    fsel_error = FSEL_NO_ERR;
    return file_no;
}

/******************************************************************************/
/*
 * \par  Description:重新设计播放模式
 *
 * \param[in]    mode 播放模式

 * \param[out]      none

 * \return        ture or false

 * \note
 *******************************************************************************/
bool fsel_set_mode(fsel_mode_e mode)
{
    uint16 flag;
    fsel_error = FSEL_NO_ERR;
    if (((uint8) mode & 0xf0) > 0)//check shuffle
    {
        flag = TRUE;
    }
    else
    {
        flag = FALSE;
    }

    fsel_init_val.fsel_mode = (uint8) (mode & 0x0f);

    if (flag == TRUE)
    {
        if (shuffle_sw == FALSE)
        {
            //之前不是shuffle,要初始化
            shuffle_sw = TRUE;
            init_shuffle(file_total);
            filtrate_shuffle_no = file_no;
        }
    }
    else
    {
        shuffle_sw = FALSE;
    }

    return TRUE;
}

/******************************************************************************/
/*
 * \par  Description:重设选择的文件类型
 *
 * \param[in]    file_type_bit   文件扩展名bitmap
 reset_dir_flag 重设目录标,若ture重新初始化到根目录

 * \param[out]      none

 * \return         true or false

 * \note
 *******************************************************************************/

bool fsel_set_typebit(uint32 file_type_bit, bool reset_dir_flag)
{
    fsel_error = FSEL_NO_ERR;
    fsel_init_val.file_type_bit = file_type_bit;
    if ((reset_dir_flag == TRUE) && (fsel_module_type == FSEL_MODULE_DIR))
    {
        init_fsel_sysdata();
        dir_set_location(NULL, fsel_init_val.fsel_type);
    }
    else
    {
        file_total = total_file_cur(NULL, 0);
        file_no = 0;
    }
    return TRUE;
}

/******************************************************************************/
/*
 * \par  Description:设置文件location
 *
 * \param[in]    location
 type

 * \param[out]

 * \return         true or false

 * \note
 *******************************************************************************/
bool fsel_set_location(void *location, fsel_type_e type)
{
    fsel_error = FSEL_NO_ERR;

    if (location == NULL)
    {
        return FALSE;
    }

    return fsel_set_location_func[fsel_module_type](location, type);
}

/******************************************************************************/
/*
 * \par  Description:获取当前选择文件的location
 *
 * \param[in]    type

 * \param[out]      location

 * \return         true or false

 * \note
 *******************************************************************************/
bool fsel_get_location(void *location, fsel_type_e type)
{
    fsel_error = FSEL_NO_ERR;

    if ((file_no == 0) || (location == NULL))
    {
        return FALSE;
    }

    return fsel_get_location_func[fsel_module_type](location, type);
}

/******************************************************************************/
/*
 * \par  Description:获取下一个目录
 *
 * \param[in]        none

 * \param[out]      strfile-返回目录标识

 * \return         true or false

 * \note
 *******************************************************************************/
bool fsel_get_nextdir(char *strfile)
{
    fsel_error = FSEL_NO_ERR;
    if (fsel_module_type == FSEL_MODULE_DIR)
    {
        return dir_next_dir(strfile);
    }
    return FALSE;
}

/******************************************************************************/
/*
 * \par  Description:获取上一个目录
 *
 * \param[in]    none

 * \param[out]          strfile-返回目录标识

 * \return         true or false

 * \note
 *******************************************************************************/
bool fsel_get_prevdir(char *strfile)
{
    fsel_error = FSEL_NO_ERR;
    if (fsel_module_type == FSEL_MODULE_DIR)
    {
        return dir_prev_dir(strfile);
    }
    return FALSE;
}

/******************************************************************************/
/*
 * \par  Description:目录操作等价VFS_CD接口
 *
 * \param[in]    mode
 dir_name

 * \param[out]      none

 * \return         true or false

 * \note
 *******************************************************************************/
bool fsel_enter_dir(uint8 mode, char *dir_name)
{
    fsel_error = FSEL_NO_ERR;
    if (fsel_module_type == FSEL_MODULE_DIR)
    {
        dir_no = 0;
        return dir_enter_dir(mode, dir_name);
    }
    return FALSE;
}

///******************************************************************************/
///*
// * \par  Description:
// *
// * \param[in]
//
// * \param[out]
//
// * \return
//
// * \note
//*******************************************************************************/
//bool  fsel_enter_root(void)
//{
//    bool ret_val=FALSE;
//
//    switch(fsel_init_val.fsel_type)
//    {
//        case FSEL_TYPE_COMMONDIR:
//        case FSEL_TYPE_DISKSEQUNCE:
//            ret_val = dir_enter_dirparent();
//            break;
//
//        default:
//            ret_val=FALSE;
//            break;
//    }
//
//    if(FALSE!=ret_val)
//    {
//        dir_layer--;
//    }
//
//    return ret_val;
//}
///******************************************************************************/
///*
// * \par  Description:
// *
// * \param[in]
//
// * \param[out]
//
// * \return
//
// * \note
//*******************************************************************************/
//bool  fsel_enter_dirparent(void)
//{
//    bool ret_val=FALSE;
//
//    switch(fsel_init_val.fsel_type)
//    {
//        case FSEL_TYPE_COMMONDIR:
//        case FSEL_TYPE_DISKSEQUNCE:
//            ret_val = dir_enter_dirparent();
//            break;
//
//        default:
//            ret_val=FALSE;
//            break;
//    }
//
//    if(FALSE!=ret_val)
//    {
//        dir_layer--;
//    }
//
//    return ret_val;
//}
//
///******************************************************************************/
///*
// * \par  Description:
// *
// * \param[in]
//
// * \param[out]
//
// * \return
//
// * \note
//*******************************************************************************/
//bool  fsel_enter_dirson(char *sub_name)
//{
//    bool ret_val=FALSE;
//
//    if(dir_layer>DIRLAYER_MAX)
//    {
//        return FALSE;
//    }
//
//    switch(fsel_init_val.fsel_type)
//    {
//        case FSEL_TYPE_COMMONDIR:
//        case FSEL_TYPE_DISKSEQUNCE:
//            ret_val = dir_enter_dirson(sub_name);
//            break;
//
//        default:
//            ret_val=FALSE;
//            break;
//    }
//
//    if(FALSE!=ret_val)
//    {
//        dir_layer++;
//    }
//
//    return ret_val;
//}

/******************************************************************************/
/*
 * \par  Description:删除当前选择的目录或文件
 *
 * \param[in]   type 删除类型

 * \param[out]      none

 * \return         true or false

 * \note
 *******************************************************************************/
//bool fsel_delete_file(fsel_del_e type)
//{
//    fsel_error = FSEL_NO_ERR;
//    return fsel_del_one_func[fsel_module_type](file_no);
//}

/******************************************************************************/
/*
 * \par  Description:初始化公共参数
 *
 * \param[in]    none

 * \param[out]      none

 * \return        none

 * \note
 *******************************************************************************/
void init_fsel_sysdata(void)
{
    dir_layer = 0;
    root_dir = 0;
    file_no = 0;
    file_total = 0;

    save_read_offset = 0xffff; //
    write_data_flag = FALSE; //

    //默认进入根目录
    vfs_cd(vfs_mount, CD_ROOT, 0);
}

/******************************************************************************/
/*
 * \par  Description:读指定VM区数据
 *
 * \param[in]    sector_offset--扇区偏移，0~N

 * \param[out]

 * \return

 * \note
 *******************************************************************************/
static bool fsel_read_sector_vram(uint16 sector_offset)
{
    if (save_read_offset != sector_offset)
    {
        if ((FALSE != write_data_flag) && (save_read_offset != 0xffff))
        {
            //回写VRAM，obj_handle存放VM地址
            sys_vm_write(fsel_temp_buf, obj_handle + save_read_offset * SECTOR_SIZE);
        }

        sys_vm_read(fsel_temp_buf, obj_handle + sector_offset * SECTOR_SIZE, SECTOR_SIZE);
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
bool fsel_read_sector_data(uint16 sector_offset)
{
    uint16 seek_type;
    int16 seek_sector;

    if (0xee == obj_open_mode)
    {
        return fsel_read_sector_vram(sector_offset);
    }

    if (save_read_offset != sector_offset)
    {

        if ((save_read_offset == 0xffff) || (save_read_offset > sector_offset))
        { //第一次读 或 往后读时使用
            seek_type = SEEK_SET;
            seek_sector = (int16) sector_offset;
        }
        else
        {
            //往前读时使用
            seek_type = SEEK_CUR;
            //seek_sector的值可能为0，情况为连续读扇区
            seek_sector = sector_offset - save_read_offset - 1;//当文件指针其实为save_read_offset+1

        }

        if ((FALSE != write_data_flag) && (save_read_offset != 0xffff))
        {
            vfs_file_seek(vfs_mount, 0 - SECTOR_SIZE, SEEK_SET, obj_handle);
            vfs_file_write(vfs_mount, fsel_temp_buf, SECTOR_SIZE, obj_handle);
        }

        if ((seek_sector != 0) || (seek_type == SEEK_SET))//若是接着读下一个扇区就不需要seek了
        {
            vfs_file_seek(vfs_mount, seek_sector * SECTOR_SIZE, seek_type, obj_handle);
        }

        vfs_file_read(vfs_mount, fsel_temp_buf, SECTOR_SIZE, obj_handle);
        save_read_offset = sector_offset;

    }

    return TRUE;

}

