/*******************************************************************************
 *                              us212A
 *                            Module: Common
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.  
 *
 * History:         
 *      <author>    <time>           <version >             <desc>
 *       lzcai     2011-9-19 21:47     1.0             build this file  
*******************************************************************************/
/*! 
 * \file     _ui_delete_sub.c
 * \brief    gui 控制流：文件或目录删除控件控制流实现。
 * \author   lzcai
 * \par      GENERAL DESCRIPTION:
 *               实现了文件或目录删除功能。
 * \par      EXTERNALIZED FUNCTIONS:
 *               kernel，libc，applib，enhanced_bs，gui dialog，等等。
 * \version  1.0 
 * \date     2011-9-19 
*******************************************************************************/

#include "common_ui.h"
#include "music_common.h"

bool delete_just_file(uint16 list_no, file_path_info_t *path_info, brow_del_e brow_del, bool del_playing);

/*! \cond */

/******************************************************************************/
/*!
 * \par  Description:
 *    删除目录或播放列表。
 * \param[in]    path_info 要删除目录的路径结构体
 * \param[in]    del_com 删除控制结构体指针
 * \param[out]   path_info 删除目录后更新的路径结构体
 * \return       app_result_e
 * \retval           RESULT_REDRAW 询问是否删除对话框取消返回AP，重绘UI。
 * \retval           RESULT_DELETE_FILE 删除文件成功返回。
 * \retval           RESULT_DELETE_DIRLIST 删除文件夹或列表成功返回。
 * \retval           RESULT_DELETE_DIRLIST_NOSELF 删除文件夹或列表（不删除自身）成功返回。
 * \retval           RESULT_ERROR 删除文件失败返回 。
 * \retval           dialog other 直接返回其他从 dialog 返回的消息，详细参见 app_result_e 定义。
 * \retval           delete other 收到返回值不为 RESULT_NULL 应用消息返回，见 app_result_e 定义。
 * \note
 *******************************************************************************/
app_result_e deleted_all(file_path_info_t *path_info, del_com_data_t *del_com)
{
    input_gui_msg_t input_msg;
    msg_apps_type_e gui_event;
    private_msg_t private_msg;
    app_result_e result = RESULT_NULL;
    bool info_flag = TRUE;
    bool ret;
    
    dir_brow_t browser_dir_browser; 
    scanfile_result_e browser_result;

    //先进入要删除子文件夹，即进入当前子文件夹
    browser_dir_browser.name_buf = com_str_buffer;
    browser_dir_browser.name_len = 64;
    fsel_browser_enter_dir(CUR_DIR, 0, &browser_dir_browser);

    //文件夹遍历浏览初始化
    fsel_browser_scanfile_init(&browser_dir_browser); 
    //遍历文件夹并删除文件及空文件夹
    while(1)
    {
        browser_result = fsel_browser_scanfile(path_info, &browser_dir_browser, SCANFILE_TYPE_DELETE);
        if(browser_result == SCANFILE_RESULT_FILE)//返回文件 
        {
            //删除当前文件
            if(delete_just_file(0, path_info, BROW_DEL_FILE, FALSE) == FALSE)
            {
                return RESULT_ERROR;
            }
        }
        else//返回文件夹，SONDIR 子文件夹 PARENTDIR 父文件夹 SELF 文件夹自身
        {
            //如果文件夹为空，删除之
            if (browser_result == SCANFILE_RESULT_PARENTDIR)
            {
                //返回到父目录，则该子目录可以删除
                if(fsel_browser_delete(BROW_DEL_DIR, 1, &(path_info->file_path)) == FALSE)
                {
                    return RESULT_ERROR; 
                }
                //更新当前子文件目录总数
                browser_dir_browser.dir_total--;
            }

            if (browser_result == SCANFILE_RESULT_SELF)//文件夹自身
            {
                //删除所有文件完毕，显示已删除动画，退出应用
                if(del_com->del_self == TRUE)
                {
                    if(fsel_browser_delete(BROW_DEL_DIR, del_com->del_no, &(path_info->file_path)) == FALSE)
                    {
                        return RESULT_ERROR;
                    }
                }
                
                result = RESULT_DELETE_DIRLIST; //删除文件夹或列表完成返回
                break;//正常退出
            }
        }

        //只允许第一次删除时提示信息
        if(info_flag == TRUE)
        {
            info_flag = FALSE;
        }
        
        //获取gui消息
        ret = get_gui_msg(&input_msg);
        if(ret == TRUE)//有gui消息
        {
            //进行按键映射
            if(com_key_mapping(&input_msg, &gui_event, delete_key_map_list) == TRUE)
            {
                //GUI按键消息处理
                switch(gui_event)
                {   
                case EVENT_DELETE_STOP: 
                    //取消返回
                    return RESULT_REDRAW; 
                    
                default:
                    break;
                }
            }
        } 
        else
        {
            //到此，gui消息队列已经处理完毕 
            //获取ap私有消息和系统消息（会先转换为私有消息再返回），并处理
            ret = get_app_msg(&private_msg);
            if(ret == TRUE) 
            {
                result = g_this_app_msg_dispatch(&private_msg); 
                if(result == RESULT_REDRAW) 
                {
                    info_flag = TRUE;
                }
                else if(result != RESULT_NULL)
                {
                    break;
                }
                else
                {
                    //do nothing for QAC
                }
            }
        }
        
        //挂起10ms，多任务调度
        sys_os_time_dly(1); 
    }
    
    return result;
}

/******************************************************************************/
/*!                     
 * \par  Description:
 *    删除当前文件或空文件夹
 * \param[in]    list_no 要删除的文件序号
 * \param[in]    path_info 指向当前要删除的文件的目录
 * \param[in]    brow_del 删除对象类型，BROW_DEL_FILE表示删除文件，BROW_DEL_DIR表示删除空目录
 * \param[in]    del_playing 是否删除正在播放文件
 * \param[out]   none 
 * \return       bool
 * \retval           返回是否删除文件成功，true表示成功，false表示失败
 * \note 
*******************************************************************************/
bool delete_just_file(uint16 list_no, file_path_info_t *path_info, brow_del_e brow_del, bool del_playing) 
{
    msg_apps_t msg; 
    bool ret;
    
    //1.通知音乐引擎要删除文件，如果删除当前正在播放文件，需要先停止播放
    if(g_app_info_state.type == ENGINE_MUSIC)
    {
        if(del_playing == TRUE) 
        {
            msg.type = MSG_MENGINE_DELFILE_MUSUI_SYNC;
        }
        else
        {
            msg.type = MSG_MENGINE_DELFILE_OTER_SYNC;
        }
        msg.content.addr = path_info;
        if(send_sync_msg(APP_ID_MENGINE, &msg, NULL, 0) == FALSE)
        {
            return FALSE;
        }
    }
    
    //2.删除当前文件或空文件夹
    ret = fsel_browser_delete(brow_del, list_no, &(path_info->file_path));
        
    return ret; 
}

/*! \endcond */
