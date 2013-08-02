/********************************************************************************
 *                              USDK
 *                             Module: ebook
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : ebook_filelist.c
 * History:
 *      <author>    <time>           <version >             <desc>
 *      stevenluo       2011-11-11         v1.0              create this file
 ********************************************************************************/
/*!
 * \file     ebook
 * \brief    ebook的filelist函数模块，调用common_ui的控件来显示文件列表信息
 * \author   stevenluo
 * \version  1.0
 * \date  2011/11/11
 *******************************************************************************/
#include  "ebook.h"

#ifdef USE_83NAME_CREAT
const uint8 BMK_LIST_NAME[12]="EBOOK   LIB";
#else
const uint8 BMK_LIST_NAME[] = "EBOOK.LIB";
#endif

/*全局变量定义*/
/********************************************************************************
 * Description :处理文件浏览的回调函数
 *
 * Arguments  :
 *              status:
 * Returns     :
 *                场景返回值
 * Notes       :
 *
 ********************************************************************************/

app_result_e _ebook_filelist_callback(file_path_info_t *path_info, uint16 active)
{
    app_result_e result;
    
    //设置选中文件
    fsel_browser_select(active);
    //获取当前文件路径
    fsel_browser_get_location(&(path_info->file_path), path_info->file_source);
    
    libc_memcpy(&g_ebook_vars.path, path_info, sizeof(file_path_info_t));
    enter_mode = ENTER_FROM_MENUDIR;
    result = _ebook_option_menu(enter_mode);
    return result;
}

/********************************************************************************
 * Description :浏览查找电子书文件格式的文件列表
 *
 * Arguments  :
 *              status:
 * Returns     :
 *                场景返回值
 * Notes       :
 *
 ********************************************************************************/

app_result_e _ebook_filelist(scene_enter_mode_e mode)
{
    app_result_e result;
    dir_com_data_t ebook_dir_data;
    uint8 card_flag;
    uint8 temp_disk;
    ebook_dir_data.list_option_func = _ebook_filelist_callback;
    ebook_dir_data.root_layer = 0;
    ebook_dir_data.menulist = NULL;
    //从路径的第一级目录开始浏览
    ebook_dir_data.browse_mode = 0;
    //卡盘和FLASH都无文件
    gui_set_headbar_mode(HEADBAR_MODE_NORMAL, HEADBAR_ICON_BROWSER);        //显示默认状态栏
    cur_file_diroffset = g_ebook_vars.path.file_path.dirlocation.dir_entry; //当前文本文件中目录信息中的目录偏移

    /*调用目录控件显示电子书的文件列表*/
    result = gui_directory(DIRLIST, &g_ebook_vars.path, &ebook_dir_data);
    
    if (result == RESULT_REDRAW)                                            //假如按return 取消返回
    {
        if (mode == ENTER_FROM_MENUDIR)                                     //假如从菜单目录进入场景
        {
            result = _deal_menu_result();
        }
        else
        {
            result = RESULT_MAIN_APP;                                       /* 回到 MainMenu 应用 */
        }
    }
    else if (result == RESULT_EBOOK_PLAY)                                   //按play键选中播放
    {
        result = RESULT_EBK_PLAY;                                           /* 切换界面到 ebook 的播放界面*/
        _close_text_file(1);
        if (mode == ENTER_FROM_DESKTOP)
        {
            enter_mode = ENTER_FROM_FILELIST;
        }
        else
        {
            enter_mode = ENTER_FROM_MENUDIR;
        }
    }
    else if (result == RESULT_DIR_ERROR_NO_FILE)                            //错误：根目录下没有文件和文件夹
    {
        if (g_ebook_vars.path.file_path.dirlocation.disk == DISK_C)
        {
            _show_single_dialog(S_DISK_NOFILE, DIALOG_INFOR_WAIT);
            device_status |= 0x01;          //flash没有文件
            //没有卡盘
            card_flag = _detect_device(DRV_GROUP_STG_CARD);
            if (card_flag == 0)
            {
                return RESULT_MAIN_APP;
            }
//            else
//            {
//                device_status |= 0x01;          //flash没有文件
//            }

        }
        else
        {
            _show_single_dialog(S_CARD_NOFILE, DIALOG_INFOR_WAIT);
            device_status |= 0x02;
        }
        if ((device_status & 0x03) == 0x03)     //flash和card都无文件
        {
            return RESULT_MAIN_APP;
        }

        //检测card是否有文件
        
        if (engine_type != ENGINE_MUSIC)        
        {
            if (g_ebook_vars.path.file_path.dirlocation.disk == DISK_C)
            {
                libc_memset(&g_ebook_vars.path.file_path.dirlocation, 0x0, sizeof(file_location_t));
                g_ebook_vars.path.file_path.dirlocation.disk = DISK_H;  //如果原来是flash，则切换到card
            }
            else
            {
                libc_memset(&g_ebook_vars.path.file_path.dirlocation, 0x0, sizeof(file_location_t));
                g_ebook_vars.path.file_path.dirlocation.disk = DISK_C;
            }
            _close_engine();
            fsel_exit();
            temp_disk=g_ebook_vars.path.file_path.dirlocation.disk;

            if(_ebook_filesel_init() == RESULT_CREATE_LIST_APP)
            {
                return RESULT_CREATE_LIST_APP;
            }
            
            //如果初始化flash后，盘符变了，说明初始化失败
            if(temp_disk != g_ebook_vars.path.file_path.dirlocation.disk)
            {
                device_status |= 0x02;  //card没有文件
                return RESULT_MAIN_APP;
            }
            result=_deal_menu_result();
        }
        else
        {
            return RESULT_MAIN_APP;
        }
    }
    else if(result == RESULT_EBK_FILELIST)   
    {
        enter_mode = ENTER_FROM_DESKTOP;        //从main ap 或browser进入场景
    }
    else 
    {
    }
    return result;
}


/******************************************************************************/
/*
 * \par  Description: mbyte to wchar
 *
 * \param[in]    dest，src

 * \param[out]

 * \return  UNICODE 个数，不包含标识符和结束符

 * \note    ASC转成UNICODE，自动加入标识符和结束符
 *******************************************************************************/
uint16 _char_to_unicode(uint8 *dest, uint8 *src)
{
    uint16 num = 0;
    
    #ifdef USE_83NAME_CREAT         //假如宏定义USE_83NAME_CREAT，即 创建文件使用短名定义
    
    while(*src!=0x00)
    {
        *dest=*src;
        dest++;
        src++;
        num++;
    }
    
    dest=0x00;
    
    #else                           //创建文件使用长名定义                  
            
    *dest = 0xff;                     //加入标识符  
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
    
    *dest       = 0x00;                 //填充0x00表示结束
    *(dest + 1) = 0x00;            
    
    #endif

    return num;

}




/******************************************************************************/
/*
 * \par  Description: 检测书签文件EBOOK.LIB 是否存在
 *
 * \return  1:书签文件EBOOK.LIB 不存在 ，0表示存在

 * \note    
 *******************************************************************************/
uint16 _check_bmk_list(void)
{
    uint16 total_num;
    uint32 ebook_lib;
    
    vfs_cd(g_ebook_mount_id, CD_ROOT, 0);
    _char_to_unicode(bmk_bufFile, (uint8 *) BMK_LIST_NAME);                 //ASC转成UNICODE，自动加入标识符和结束符
    ebook_lib = vfs_file_open(g_ebook_mount_id, bmk_bufFile, R_NORMAL_SEEK);
    if (ebook_lib != 0)                                                     //如果打开文件成功
    {
        vfs_file_read(g_ebook_mount_id, bmk_bufFile, 512, ebook_lib);       //读取书签信息512字节到bmk_bufFile
        libc_memcpy(&total_num, &bmk_bufFile[8], 2);                        //提取总页码total_num
        vfs_file_close(g_ebook_mount_id, ebook_lib);
        if (total_num != 0)
        {
            device_status &= 0x1;                                           //标记SD卡有文件
            return 0;
        }
    }
    device_status |= 0x02;                                                  //标记SD卡无文件
    return 1;
}

void _close_text_handle(void)
{
    if (bmk_modify_handle != 0)
    {
        _writeback_bmk_info();
        vfs_file_close(g_ebook_mount_id, bmk_modify_handle);
        bmk_modify_handle = 0x0;
    }
    if (view_file.file_handle != 0)
    {
        vfs_file_close(g_ebook_mount_id, view_file.file_handle);
        view_file.file_handle = 0x0;
    }
}

