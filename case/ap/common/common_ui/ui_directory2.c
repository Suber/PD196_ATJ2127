/*******************************************************************************
 *                              us212A
 *                            Module: commonUI-directory
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 *
 * History:         
 *      <author>    <time>           <version >             <desc>
 *       lzcai     2011-9-18 21:56     1.0             build this file 
*******************************************************************************/
/*!
 * \file     _ui_directory2.c
 * \brief    commonUI 文件浏览器控件 gui 消息分发处理实现
 * \author   lzcai
 * \par      GENERAL DESCRIPTION:
 *               实现了一种文件浏览器GUI，包括文件系统，播放列表，收藏夹等文件浏览。
 * \par      EXTERNALIZED FUNCTIONS:
 *               直接调用ui 驱动中的listbox完成控件显示流任务。
 * \version 1.0
 * \date  2012-2-21
*******************************************************************************/

#include "common_ui.h"


/*!
 *  \brief
 *  ext_vec_rom：ROM中的后缀名数组，最多包含 31 个后缀名
 * 
 */
const char ext_vec_rom[32][4] = 
{
    "***", "MP3", "WMA", "WAV", "AAC", "FLA", "APE", "AVI", 
    "TXT", "JPG", "JPE", "BMP", "GIF", "LRC", "ASF", "OGG", 
    "WMV", "AMV", "AAX", "AA ", "MP1", "MP2", "M4A", "POS", 
    "ACT", "** ", "** ", "** ", "** ", "**", "*f", "*d"
};

/*!
 *  \brief
 *  bitmap_vec：ROM中的后缀名数组的bitmap集合，bitmap与ROM中的后缀名对应关系是：
 *  从bit30开始，最高位对应第一个后缀名，依次类推
 */
const uint32 bitmap_vec[8] = 
{
    /*! 音乐后缀名bitmap */
    MUSIC_BITMAP,
    /*! 语音书籍后缀名bitmap */
    AUDIBLE_BITMAP,
    /*! 视频文件后缀名bitmap */
    VIDEO_BITMAP,
    /*! 图片文件后缀名bitmap */
    PICTURE_BITMAP,
    /*! 文本文件后缀名bitmap */
    TEXT_BITMAP,
    /*! 所有文件和目录bitmap */
    ALLALL_BITMAP,
    /*! 所有文件bitmap */
    ALLFILE_BITMAP,
    /*! 所有目录bitmap */
    ALLDIR_BITMAP,
};

/*!
 *  \brief
 *  result_vec：对应的结果标识集合，如音乐对应RESULT_MUSIC_PLAY等
 */
const app_result_e result_vec[6] = 
{
    /*! 音乐文件播放消息 */
    RESULT_MUSIC_PLAY,
    /*! 有声书籍文件播放消息 */
    RESULT_AUDIBLE_PLAY,
    /*! 视频文件播放消息 */
    RESULT_MOVIE_PLAY,
    /*! 图片文件播放消息 */
    RESULT_PICTURE_PLAY,
    /*! 文本文件播放消息 */
    RESULT_EBOOK_PLAY,
    /*! 空消息 */
    RESULT_NULL,
};

uint8 dir_file_type(char *ext);
void get_dir_menu_item(file_brow_t *file_list_item, menu_title_data_t *p_dir_menu_title, uint16 index);
menu_cb_func get_dir_menu_item_func(menu_title_data_t *p_dir_menu_title, uint16 index);
void update_dir_control_after_del(dir_control_t *p_dir_control);
app_result_e directory_option_deal(file_path_info_t*path_info, dir_control_t*p_dir_control);

extern void dir_select_next(file_brow_t *file_list, dir_control_t *p_dir_control)__FAR__;
extern void dir_select_prev(file_brow_t *file_list, dir_control_t *p_dir_control)__FAR__;
extern void write_history_item(uint8 layer_no, dir_control_t *p_dir_control, bool update)__FAR__;
extern void read_history_item(uint8 layer_no, dir_control_t *p_dir_control)__FAR__;

//获取后缀名对应的应用序号，从result_vec索引出引用对应的app_result_e播放消息
//其中0-4为支持的文件类型，5表示不支持的文件类型，对应RESULT_NULL消息
uint8 dir_file_type(char *ext)
{
    uint8 i, j;
    uint32 bitmap;

    //按应用匹配
    for (i = 0; i < 5; i++)
    {
        bitmap = 0x80000000;
        for(j = 0; j < 32; j++)
        {
            if((bitmap_vec[i] & bitmap) != 0x0)
            {
                if (libc_strncmp(ext, ext_vec_rom[j], 4) == 0)
                {
                    return i;
                }
            }
            bitmap = bitmap >> 1;
        }
    }
    
    return 5;
}

//删除文件（文件夹或列表）后更新 dir_control_t 结构体
void update_dir_control_after_del(dir_control_t *p_dir_control)
{
    //尚未列出最后一项，后面还有文件替补，只需要把 list_total 减去1 即可
    if(p_dir_control->bottom < p_dir_control->list_total)
    {
        p_dir_control->list_total--;
    }
    else//后面没有文件替补，需要挪动列表
    {
        //尚未列出第一项，前面尚有文件可替补，整个列表往下移动一项
        if(p_dir_control->top > 1)
        {
            p_dir_control->list_total--;
            p_dir_control->top--;
            p_dir_control->bottom--;
            p_dir_control->list_no--;
        }
        else//前面没有文件替补，把后面往前挪动
        {
            //删除最后一项，需要改变 list_no
            if(p_dir_control->list_no == p_dir_control->bottom)
            {
                p_dir_control->list_no--;
                p_dir_control->active--;
            }
            p_dir_control->list_total--;
            p_dir_control->bottom--;
        }
    }
    p_dir_control->old = p_dir_control->active;
}

//directory gui msg--option deal
app_result_e directory_option_deal(file_path_info_t *path_info, dir_control_t *p_dir_control)
{
    app_result_e result = RESULT_NULL;
    uint16 tmp_list_no = p_dir_control->list_no - dir_menu_title.count;
                    
    //注意：该函数可能修改到 path_info，并使 fsel_browser_enter_dir(CUR_DIR, 0, &g_dir_browser); 
    //结果发生改变，用户应该清楚这种情况，已更谨慎实现 list_option_func 函数
    result = p_dir_control->dir_com->list_option_func(path_info, tmp_list_no);
    //删除当前文件夹或列表下文件，子文件夹或列表，更新列表
    if((result == RESULT_DELETE_FILE) 
    || (result == RESULT_DELETE_DIRLIST)
    || (result == RESULT_DELETE_DIRLIST_NOSELF))
    {
        if(result == RESULT_DELETE_DIRLIST_NOSELF)
        {
            result = RESULT_REDRAW; 
        }
        else
        {
            update_dir_control_after_del(p_dir_control); 
            if(p_dir_control->list_total == 0)//当前列表已无列表项（包括菜单项，如果有菜单，仍然可以进行显示）
            {
                //提示找不到文件
                //gui_dialog_msg (DIALOG_MSG, DIALOG_INFOR_WAIT, S_FILE_NO_FILE); 
                //返回上一级
                //1.如果已经处于最顶层，表示磁盘已经没有符合条件的文件，返回“无文件”
                if(g_dir_browser.layer_no == p_dir_control->dir_com->root_layer)
                {
                    //已经在浏览目录树根节点，返回
                    result = RESULT_DIR_ERROR_NO_FILE;
                }
                //2.返回上一级目录
                else
                {
                    if (fsel_browser_enter_dir(PARENT_DIR, 0, &g_dir_browser) == TRUE)
                    {
                        p_dir_control->enter_dir = TRUE;
                        p_dir_control->update_mode = LIST_UPDATE_HEAD;
                        p_dir_control->draw_mode = LIST_DRAW_ALL;
                        result = RESULT_NULL;//不退出控件
                    }
                    else
                    {
                        result = RESULT_DIR_ERROR_ENTER_DIR;
                    }
                }
            }
            else//尚有文件，重绘当前列表
            {
                result = RESULT_REDRAW; 
            }
        }
        
    }
    
    if(result == RESULT_REDRAW)
    {
        //返回重绘当前列表
        fsel_browser_enter_dir(CUR_DIR, 0, &g_dir_browser);
        p_dir_control->update_mode = LIST_UPDATE_REUPDATE;
        p_dir_control->draw_mode = LIST_DRAW_ALL;
        result = RESULT_NULL;//不退出控件
    }
    
    return result;
}

//directory gui app msg deal--gui
app_result_e directory_msg_deal_gui(file_path_info_t *path_info,dir_control_t *p_dir_control,msg_apps_type_e gui_event)
{
    file_brow_t *cur_file;//当前选择的文件结构体
    app_result_e result = RESULT_NULL;
    bool redraw_all = FALSE;
    
    //GUI按键消息处理
    switch(gui_event)
    {
    case EVENT_DIRECTORY_SELECT_NEXT:
        //向下切更新列表项控制结构，并移动有效文件记录项
        dir_select_next(g_file_list, p_dir_control);
        break;
        
    case EVENT_DIRECTORY_SELECT_PREV:
        //向上切更新列表项控制结构，并移动有效文件记录项
        dir_select_prev(g_file_list, p_dir_control);
        break;
        
    case EVENT_DIRECTORY_SELECT_STOP:
        //清除速度控制变量，从头开始计算
        speed_control = 0;
        break;

    case EVENT_DIRECTORY_BACK_PARENT:
        //保存当前层次文件浏览记录
        write_history_item(g_dir_browser.layer_no, p_dir_control, FALSE);

        if(g_dir_browser.layer_no == p_dir_control->dir_com->root_layer)
        {
            //已经在浏览目录树根节点，返回
            result = RESULT_REDRAW;
        }
        else
        {
            //返回上一级目录
            if (fsel_browser_enter_dir(PARENT_DIR, 0, &g_dir_browser) == TRUE)
            {
                p_dir_control->enter_dir = TRUE;
                p_dir_control->update_mode = LIST_UPDATE_HEAD;
                p_dir_control->draw_mode = LIST_DRAW_ALL;
            }
            else
            {
                result = RESULT_DIR_ERROR_ENTER_DIR;
            }
        }
        break;
    
    case EVENT_DIRECTORY_ENTER_SON:
        //保存当前层次文件浏览记录
        write_history_item(g_dir_browser.layer_no, p_dir_control, FALSE);
        if((p_dir_control->active < 1) || (p_dir_control->active > LIST_NUM_ONE_PAGE_MAX))//for QAM
        {
            result = RESULT_DIR_ERROR_ENTER_DIR;
            break;
        }
        cur_file = &(g_file_list[p_dir_control->active - 1]);
        
        //选择菜单项
        if(p_dir_control->list_no <= dir_menu_title.count)
        {
            menu_cb_func func;
            //从菜单中读取当前项p_dir_control->list_no，然后执行回调函数
            func = get_dir_menu_item_func(&dir_menu_title, p_dir_control->list_no - 1);
            if(func != NULL)
            {
                result = func(NULL);
                if(result == RESULT_REDRAW)
                {
                    //返回重绘当前列表
                    redraw_all = TRUE;
                }
            }
        }
        //选择文件夹
        else if(p_dir_control->list_no <= (g_dir_browser.dir_total + dir_menu_title.count))
        {
            uint16 tmp_active;
            
            if(g_dir_browser.layer_no >= (MAX_DIR_LAYER - 1))
            {
                //超出8层子目录不返回错误，不进行任何反应
                break;
            }
            
            //如果处于子目录上，进入子目录
            tmp_active = p_dir_control->list_no - dir_menu_title.count;
            if (fsel_browser_enter_dir(SON_DIR, tmp_active, &g_dir_browser) == TRUE)
            {
                //空子文件夹，不进入，提示找不到文件，重绘父目录
                if((g_dir_browser.dir_total == 0) && (g_dir_browser.file_total == 0))
                {
                    fsel_browser_enter_dir(PARENT_DIR, 0, &g_dir_browser);
                    
                    //提示找不到文件
                    gui_dialog_msg (DIALOG_MSG, DIALOG_INFOR_WAIT, S_FILE_NO_FILE);
                    //返回重绘当前列表
                    redraw_all = TRUE;
                }
                else//进入子文件夹
                {
                    p_dir_control->enter_dir = TRUE;
                    p_dir_control->update_mode = LIST_UPDATE_HEAD;
                    p_dir_control->draw_mode = LIST_DRAW_ALL;
                    result = RESULT_NULL;//不退出控件
                }
            }
            else
            {
                result = RESULT_DIR_ERROR_ENTER_DIR;
            }
        }
        //选择文件
        else
        {
            //选择文件播放
            result = result_vec[dir_file_type(cur_file->ext)];
            if (result != RESULT_NULL)//只对所支持文件类型操作
            {
                //设置选中文件
                fsel_browser_select(p_dir_control->list_no - dir_menu_title.count);
                //获取当前文件路径
                fsel_browser_get_location(&(path_info->file_path), path_info->file_source);
            }
            else
            {
                //提示文件格式不支持
                gui_dialog_msg (DIALOG_MSG, DIALOG_INFOR_WAIT, S_FILE_NO_SUPPORT);
                //返回重绘当前列表
                redraw_all = TRUE;
            }
        }
        break;
    
    case EVENT_DIRECTORY_ENTER_OPTION:
        // 进入弹出式 option menu
        if(p_dir_control->dir_com->list_option_func != NULL)
        {
            result = directory_option_deal(path_info, p_dir_control);
            if(result != RESULT_NULL)
            {
                //保存当前层次文件浏览记录
                write_history_item(g_dir_browser.layer_no, p_dir_control, FALSE);
            }
        }
        break;
    
    default:
        result = com_message_box(gui_event);
        if(result == RESULT_REDRAW)
        {
            //返回重绘当前列表
            redraw_all = TRUE;
        }
        break;
    }
    
    if(redraw_all == TRUE)
    {
        fsel_browser_enter_dir(CUR_DIR, 0, &g_dir_browser);
        p_dir_control->update_mode = LIST_UPDATE_REUPDATE;
        p_dir_control->draw_mode = LIST_DRAW_ALL;
        result = RESULT_NULL;//不退出控件
    }

    return result;
}

//获取菜单项确定执行函数 menu_func
menu_cb_func get_dir_menu_item_func(menu_title_data_t *p_dir_menu_title, uint16 index)
{
    menu_item_data_t dir_menu_item;
    menu_cb_func func;

    //读取菜单项
    com_get_confmenu_item(p_dir_menu_title, index, &dir_menu_item);
#ifndef PC
    func = dir_menu_item.menu_func;
#else
    func = this_get_menu_func(dir_menu_item.str_id, dir_menu_item.str_id_sub);
#endif
    
    return func;
}
