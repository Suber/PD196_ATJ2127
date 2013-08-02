/********************************************************************************
 *                              USDK
 *                             Module: ebook
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : ebook_main.c
 * History:
 *      <author>    <time>           <version >             <desc>
 *      stevenluo       2011-11-11         v1.0              create this file
 ********************************************************************************/
/* include file of this application, 本应用的头文件 */
#include "ebook.h"

//menu 内容太多，分开两个文件



/********************************************************************************
 * Description :卡目录菜单选项的执行函数
 *
 * Arguments  :
 *              param:
 * Returns     :
 *                场景返回值
 * Notes       :
 *
 ********************************************************************************/

app_result_e _menu_func_carddir(void *param)
{
    //没有卡盘
    //卡盘和FLASH都无文件
    uint16 dialog_msg = 0;
    app_result_e    result;
    uint8           disk=DISK_H;
    bool            need_create = FALSE;
    
    if ((device_status & 0x03) == 0x03)                                 //flash和card都无文件
    {
        //_show_single_dialog(S_DISK_NOFILE, DIALOG_INFOR_WAIT);
        return RESULT_MAIN_APP;                                         // 回到 MainMenu 应用 
    }
    if (_detect_device(DRV_GROUP_STG_CARD) == 0)                        
    {
        dialog_msg = S_CARD_OUT;                                        //通知SD卡不存在
    }
    else
    {
        //先切换到H盘
        if ((music_play_device != DISK_H) && (music_play_device != 0))      //music不再SD盘播放
        {
            _close_engine();                                                //通知后台引擎退出
        }

        if (g_ebook_vars.path.file_path.dirlocation.disk == DISK_C)
        {
            view_file.file_handle=0;        //切换盘，关闭文件，以便重新打开
            fsel_exit();                                                    //退出Flash
            libc_memset(&g_ebook_vars.path.file_path.dirlocation, 0x0, sizeof(file_location_t));
            g_ebook_vars.path.file_path.dirlocation.disk = disk;            //切换盘
            result =_ebook_filesel_init();                                  //初始化盘
            if (result == RESULT_CREATE_LIST_APP)
            {
                return result;
            }
            else if(result == RESULT_REDRAW)
            {
                //恢复现场环境
                g_ebook_vars.path.file_path.dirlocation.disk = DISK_C;          //切换回C盘
                _ebook_filesel_init();                                          //初始化盘
                return result;
            }
            else
            {

            }
        }

        //从C盘到H盘:初始化检测不了播放列表就回切换回C盘，C盘肯定有播放列表，可直接返回
        //           初始化检测到有播放列表，可直接返回
        //从H盘到H盘:肯定有播放列表，可直接返回
        //两种情况，所以后面不需要再判断是否存在播放列表。
        
    }
    if (dialog_msg != 0)
    {
        _show_single_dialog(dialog_msg, DIALOG_INFOR_WAIT); 
        return _deal_menu_result();
    }
    enter_mode = ENTER_FROM_DESKTOP;
    return RESULT_EBK_FILELIST;
}



