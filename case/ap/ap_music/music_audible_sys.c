/*******************************************************************************
 *                              US212A
 *                            Module: music engine
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *      fiona.yang  2012-03-02        1.0              create this file
 *******************************************************************************/

#include "app_music.h"

#if 0
const uint8 sys_file_name[] =
{
    0xff, 0xfe, //unicode flag
    0x41, 0x00, //A
    0x75, 0x00, //u
    0x64, 0x00, //d
    0x69, 0x00, //i
    0x62, 0x00, //b
    0x6c, 0x00, //l
    0x65, 0x00, //e
    0x41, 0x00, //A
    0x63, 0x00, //c
    0x74, 0x00, //t
    0x69, 0x00, //i
    0x76, 0x00, //v
    0x61, 0x00, //a
    0x74, 0x00, //t
    0x69, 0x00, //i
    0x6f, 0x00, //o
    0x6e, 0x00, //n
    0x2e, 0x00, //.
    0x73, 0x00, //s
    0x79, 0x00, //y
    0x73, 0x00, //s
    0x00, 0x00 //end flag
};

const uint8 sys_ascfile_dir1[] =
{
    0x53,//S
    0x45,//E
    0x52,//R
    0x56,//V
    0x49,//I
    0x43,//C
    0x45,//E
    0x00 //end
};

const uint8 sys_unifile_dir1[] =
{
    0xff, 0xfe, //unicode flag
    0x53, 0x00, //S
    0x65, 0x00, //e
    0x72, 0x00, //r
    0x76, 0x00, //v
    0x69, 0x00, //i
    0x63, 0x00, //c
    0x65, 0x00, //e
    0x00, 0x00 //end
};

const uint8 sys_ascfile_dir2[] =
{
    0x41,//A
    0x55,//U
    0x44,//D
    0x49,//I
    0x42,//B
    0x4C,//L
    0x45,//E
    0x00 //end
};

const uint8 sys_unifile_dir2[] =
{
    0xff, 0xfe, //unicode flag
    0x41, 0x00, //A
    0x75, 0x00, //u
    0x64, 0x00, //d
    0x69, 0x00, //i
    0x62, 0x00, //b
    0x6c, 0x00, //l
    0x65, 0x00, //e
    0x00, 0x00 //end
};
#else
const uint8 sys_file_name[] =
{
    0xff, 0xfe, //unicode flag
    'A', 0x00, //A
    'A', 0x00, //A
    'I', 0x00, //I
    'n', 0x00, //n
    'f', 0x00, //f
    'o', 0x00, //o
    '.', 0x00, //.
    's', 0x00, //s
    'y', 0x00, //y
    's', 0x00, //s
    0x00, 0x00 //end flag
};

const uint8 sys_ascfile_dir2[] =
{
    0x41,//A
    0x55,//U
    0x44,//D
    0x49,//I
    0x42,//B
    0x4C,//L
    0x45,//E
    0x20,
    0x00 //end
};

const uint8 sys_unifile_dir2[] =
{
    0xff, 0xfe, //unicode flag
    0x41, 0x00, //A
    0x75, 0x00, //u
    0x64, 0x00, //d
    0x69, 0x00, //i
    0x62, 0x00, //b
    0x6c, 0x00, //l
    0x65, 0x00, //e
    0x00, 0x00 //end
};
#endif
static uint8 aud_sys_buffer[512] _BANK_DATA_ATTR_;

//dir layer save
static pdir_layer_t sys_layer_buf _BANK_DATA_ATTR_;

/******************************************************************************/
/*!
 * \par  Description:
 * \static bool _mengine_maindisk_init(void)
 * \将小机文件选择器重新初始化成主盘以便于读取sys文件
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      music_audible
 * \note
 */
/*******************************************************************************/
static bool _mengine_maindisk_init(void)
{
    fsel_param_t init_param;//文件选择器初始化参数

    init_param.brow_type = FSEL_BROW_ALL;
    init_param.disk = DISK_C;
    init_param.file_type_bit = AUDIBLE_BITMAP;
    init_param.fsel_mode = FSEL_MODE_NORMAL;
    init_param.fsel_type = FSEL_TYPE_COMMONDIR;

    g_music_mount_id = fsel_init(&init_param, 0);
    if (g_music_mount_id != -1)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}


/******************************************************************************/
/*!
 * \par  Description:
 * \void music_load_sysfile(void)
 * \获取audible的sys文件信息,该文件由audible工具在小机生成
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      music_audible
 * \note
 * \li   从VM读取sys文件;如果syss文件不存在或无效，会重新从主盘相应目录读取sys文件
         并保存到VM中
 */
/*******************************************************************************/
void music_load_sysfile(void)
{
   //path save
    uint8 cur_offset_save[8];

    //file handle
    handle sys_handle;

    //location ptr
    plist_location_t* location_ptr;

    bool dir_exsit = FALSE;

    sys_vm_read(aud_sys_buffer, VM_AUD_SYS + 512, 512);

    if ((aud_sys_buffer[510] == 0x55) && (aud_sys_buffer[511] == 0xaa))
    {
        //已存储sys文件
        return;
    }

    //关闭后台引擎，防止介质挂载不一致
    if(get_engine_type() == ENGINE_MUSIC)
    {
        music_close_engine();
    }

    //save current path
    vfs_file_dir_offset(g_music_mount_id, &sys_layer_buf, cur_offset_save, 0);

    location_ptr = &(g_file_path_info.file_path.plist_location);

    if (location_ptr->disk != DISK_C)
    {
        //非主盘应用，需要先切换盘符读取sys文件        
        fsel_exit();
        _mengine_maindisk_init();
    }

    //sys 文件处理
    //定位到根目录
    vfs_cd(g_music_mount_id, CD_ROOT, 0);
    //if (vfs_cd(g_music_mount_id, CD_SUB, sys_ascfile_dir1) != 0)
    //{
    if (vfs_cd(g_music_mount_id, CD_SUB, sys_ascfile_dir2) != 0)
    {
        dir_exsit = TRUE;
    }
    //}
    else
    {
        //if (vfs_cd(g_music_mount_id, CD_SUB, sys_unifile_dir1) != 0)
        //{
        if (vfs_cd(g_music_mount_id, CD_SUB, sys_unifile_dir2) != 0)
        {
            dir_exsit = TRUE;
        }
        //}

    }
    if (dir_exsit == TRUE)
    {
        //打开licence文件
        sys_handle = vfs_file_open(g_music_mount_id, sys_file_name, R_NORMAL_SEEK);
        if (sys_handle != 0)
        {
            vfs_file_read(g_music_mount_id, aud_sys_buffer, 512, sys_handle);
            sys_vm_write(aud_sys_buffer, VM_AUD_SYS);
            vfs_file_read(g_music_mount_id, aud_sys_buffer, 512, sys_handle);
            //标记该sys文件有效
            aud_sys_buffer[510] = 0x55;
            aud_sys_buffer[511] = 0xaa;
            sys_vm_write(aud_sys_buffer, VM_AUD_SYS + 512);
            vfs_file_close(g_music_mount_id, sys_handle);
        }
    }

    if (location_ptr->disk != DISK_C)
    {
        //非主盘应用，需要先切换盘符读取sys文件
        fsel_exit();
        music_file_init(&g_file_path_info);
    }

    //restore path
    vfs_file_dir_offset(g_music_mount_id, &sys_layer_buf, cur_offset_save, 1);

    return;
}
