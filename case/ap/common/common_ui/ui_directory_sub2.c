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
 * \file     _ui_directory_sub2.c
 * \brief    commonUI 文件浏览器控件控制流实现
 * \author   lzcai
 * \par      GENERAL DESCRIPTION:
 *               实现了一种文件浏览器GUI，包括文件系统，播放列表，收藏夹等文件浏览。
 * \par      EXTERNALIZED FUNCTIONS:
 *               直接调用ui 驱动中的listbox完成控件显示流任务。
 * \version  1.0
 * \date     2011-9-18
*******************************************************************************/

#include "common_ui.h"

void write_history_item(uint8 layer_no, dir_control_t *p_dir_control, bool update);

//读取路径历史记录
void read_history_item(uint8 layer_no, dir_control_t *p_dir_control)
{
    history_index_t temp_index;
    history_item_t temp_history_item;
    
    temp_index.type = PATH_HISTORY_DIR;
    temp_index.app_id = g_this_app_info->app_id;
    temp_index.path_id = p_dir_control->dir_disk;
    temp_index.layer = layer_no;
    com_get_history_item(&temp_index, &temp_history_item);
    
    if(p_dir_control->dir_com != NULL)
    {
        if(p_dir_control->dir_com->del_back == TRUE)
        {
            uint16 tmp_bottom;
        
            //删除后更新路径记忆，如果后面没有文件替补，需要挪动列表，否则无需调整
            tmp_bottom = temp_history_item.top + p_dir_control->one_page_count - 1;
            if(tmp_bottom >= (p_dir_control->list_total + 1))//list_total为删除后的list_total，故加1
            {
                //尚未列出第一项，前面尚有文件可替补，整个列表往下移动一项
                if(temp_history_item.top > 1)
                {
                    temp_history_item.top--;
                    temp_history_item.list_no--;
                }
                else//前面没有文件替补，把后面往前挪动
                {
                    //删除最后一项，需要改变 list_no
                    if(temp_history_item.list_no == tmp_bottom)
                    {
                        temp_history_item.list_no--;
                    }
                }
            }
        
            p_dir_control->dir_com->del_back = FALSE;
        }
    }
    
    if(temp_history_item.top == V_U16_INVALID)
    {
        p_dir_control->top = 1;
        p_dir_control->list_no = 1;
    }
    else if((temp_history_item.top == 0)
          ||(temp_history_item.list_no == 0)
          ||(temp_history_item.top > p_dir_control->list_total)
          ||(temp_history_item.list_no > p_dir_control->list_total)
          ||(temp_history_item.top > temp_history_item.list_no)
          ||((temp_history_item.list_no - temp_history_item.top) > (p_dir_control->one_page_count - 1)))
    {
        //路径记忆参数出错，强制指向开头，并且重写入路径记忆，以强制把后面层级清为无效
        p_dir_control->top = 1;
        p_dir_control->list_no = 1;
        write_history_item(layer_no, p_dir_control, FALSE);
    }
    else
    {
        p_dir_control->top = temp_history_item.top;
        p_dir_control->list_no = temp_history_item.list_no;
    }
}

//保存路径历史记录
void write_history_item(uint8 layer_no, dir_control_t *p_dir_control, bool update)
{
    history_index_t temp_index;
    history_item_t temp_history_item;
    
    temp_history_item.top = p_dir_control->top;
    temp_history_item.list_no = p_dir_control->list_no;
    
    temp_index.type = PATH_HISTORY_DIR;
    temp_index.app_id = g_this_app_info->app_id;
    temp_index.path_id = p_dir_control->dir_disk;
    temp_index.layer = layer_no;
    if(update == FALSE)
    {
        com_set_history_item(&temp_index, &temp_history_item);
    }
    else
    {
        com_update_path_history(&temp_index);
    }
}

//初始化 gui_directory 环境，包括初始化缓冲区，进入目录
app_result_e init_directory_env(dir_control_t *p_dir_control, uint8 *list_buffer, file_path_info_t *path_info)
{
    uint8 i;
    
    //初始列表项缓冲区空间   
    g_dir_browser.name_buf = list_buffer + 0;
    g_dir_browser.name_len = LIST_ITEM_BUFFER_LEN;
    for(i = 1; i <= LIST_NUM_ONE_PAGE_MAX; i++)
    {
        g_file_list[i - 1].name_buf = list_buffer + (LIST_ITEM_BUFFER_LEN * i);
        g_file_list[i - 1].name_len = LIST_ITEM_BUFFER_LEN;
    }
    
    //从路径的第一级目录浏览
    if((p_dir_control->dir_com->browse_mode & 0x7f) == 0)
    {
        if(p_dir_control->dir_com->root_layer == 0)
        {
            //进入并获取根目录的目录结构体
            if(fsel_browser_enter_dir(ROOT_DIR, 0, &g_dir_browser) == FALSE)
            {
                return RESULT_DIR_ERROR_ENTER_DIR;
            }
        }
        else
        {
            //进入并获取当前目录的目录结构体，用户调用 gui_directory 之前必须保证已经定位到指定目录
            if(fsel_browser_enter_dir(CUR_DIR, 0, &g_dir_browser) == FALSE)
            {
                return RESULT_DIR_ERROR_ENTER_DIR;
            }
        }
        dir_control.update_mode = LIST_UPDATE_HEAD;
    }
    else//直接跳到路径指向目录浏览
    {
        if(fsel_browser_set_location(&(path_info->file_path), path_info->file_source) == TRUE)
        {
            //获取当前路径的目录结构体
            if(fsel_browser_enter_dir(CUR_DIR, 0, &g_dir_browser) == FALSE)
            {
                return RESULT_DIR_ERROR_ENTER_DIR;
            }
            dir_control.update_mode = LIST_UPDATE_REUPDATE;
        }
        else
        {
            if(p_dir_control->dir_com->root_layer == 0)
            {
                //进入并获取根目录的目录结构体
                if(fsel_browser_enter_dir(ROOT_DIR, 0, &g_dir_browser) == FALSE)
                {
                    return RESULT_DIR_ERROR_ENTER_DIR;
                }
                dir_control.update_mode = LIST_UPDATE_HEAD;
                
                //获取 ROOT 文件路径，确保下次进来时设置成功
                fsel_browser_get_location(&(path_info->file_path), path_info->file_source);
                
                //找不到路径，清除该应用文件浏览记忆
                com_clear_app_history(PATH_HISTORY_DIR, g_this_app_info->app_id, p_dir_control->dir_disk);
            }
            else
            {
                //顶层不是根目录，错误返回，由上层自己处理
                return RESULT_DIR_ERROR_SETLOC;
            }
        }
    }

    return RESULT_NULL;
}

//同步更新listbox私有数据结构
void update_listbox_private(dir_control_t *p_dir_control, listbox_private_t *dir_private)
{
    uint8 index;

    dir_private->title.data.str = g_dir_browser.name_buf;
    dir_private->title.length = LIST_ITEM_BUFFER_LEN;
    if( (dir_private->title.data.str[0] == 0xff)
      &&(dir_private->title.data.str[1] == 0xfe) )
    {
        dir_private->title.language = UNICODEDATA;
    }
    else
    {
        //短名后面添加结束符
        dir_private->title.data.str[11] = '\0';
        dir_private->title.language = (int8)sys_comval->language_id;
    }
    
    dir_private->item_valid = p_dir_control->bottom - p_dir_control->top + 1;
    dir_private->active = p_dir_control->active - 1;
    dir_private->old = p_dir_control->old - 1;
    dir_private->list_no = p_dir_control->list_no - 1;
    dir_private->list_total = p_dir_control->list_total;
    
    for(index = 0; index < dir_private->item_valid; index++)
    {
        dir_private->items[index].data.str = g_file_list[index].name_buf;
        dir_private->items[index].length = LIST_ITEM_BUFFER_LEN;
        if( (dir_private->items[index].data.str[0] == 0xff)
          &&(dir_private->items[index].data.str[1] == 0xfe) )
        {
            dir_private->items[index].language = UNICODEDATA;
        }
        else
        {
            dir_private->items[index].language = (int8)sys_comval->language_id;
        }
        
        if((p_dir_control->top + index) <= dir_menu_title.count)
        {
            dir_private->items[index].argv = ITEMS_FRONT_TAG_ISNOT_DISP;
        }
        else if((p_dir_control->top + index) <= (g_dir_browser.dir_total + dir_menu_title.count))
        {
            dir_private->items[index].argv = ITEMS_FRONT_TAG_IS_DISP | ITEMS_FRONT_TAG_INDEX;
        }
        else
        {
            dir_private->items[index].argv = ITEMS_FRONT_TAG_ISNOT_DISP;
        }
    }
}
