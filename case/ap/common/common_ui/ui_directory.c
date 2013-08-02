/*******************************************************************************
 *                              us212A
 *                            Module: Common
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved. 
 *
 * History:         
 *      <author>    <time>           <version >             <desc>
 *       lzcai     2011-9-18 21:56     1.0             build this file 
*******************************************************************************/
/*!
 * \file     _ui_directory.c
 * \brief    gui 控制流：文件浏览器控件控制流实现。
 * \author   lzcai
 * \par      GENERAL DESCRIPTION:
 *               实现了一种文件浏览器控件，包括文件系统，播放列表，收藏夹等文件浏览。
 * \par      EXTERNALIZED FUNCTIONS:
 *               kernel，libc，ui驱动，applib，common_func，enhanced_bs，等等。
 * \version  1.0
 * \date     2011-9-18
*******************************************************************************/

#include "common_ui.h"

static void __directory_scroll_handle(void);
void load_dir_title_data(dir_control_t *p_dir_control);
void load_dir_item_data(dir_control_t *p_dir_control);
bool match_list_menu(uint8 layer, menu_title_data_t* title, list_menu_t *menu, uint8 count);
void get_dir_menu_item(file_brow_t *file_list_item, menu_title_data_t *p_dir_menu_title, uint16 index);
void update_decode_param(brow_decode_t *decode_param, dir_control_t *p_dir_control, uint16 lines);
app_result_e directory_msg_deal(file_path_info_t *path_info, dir_control_t *p_dir_control);

/*! \cond COMMON_API */

/*!
 *  \brief
 *  g_list_buffer：文件浏览器当前列表项缓冲区数组
 */
uint8 g_list_buffer[LIST_ITEM_BUFFER_LEN * (LIST_NUM_ONE_PAGE_MAX + 1)] _BANK_DATA_ATTR_;
listbox_private_t listbox_data _BANK_DATA_ATTR_;

extern void update_listbox_private(dir_control_t *p_dir_control, listbox_private_t *dir_private)__FAR__;
extern void read_history_item(uint8 layer_no, dir_control_t *p_dir_control)__FAR__;
extern void write_history_item(uint8 layer_no, dir_control_t *p_dir_control, bool update)__FAR__;
extern app_result_e init_directory_env(dir_control_t *p_dir_control, uint8 *list_buffer, \
file_path_info_t *path_info)__FAR__;
extern bool init_dir_control(dir_control_t *p_dir_control)__FAR__;
extern app_result_e directory_msg_deal_gui(file_path_info_t *path_info, dir_control_t *p_dir_control, \
msg_apps_type_e gui_event) __FAR__;

/******************************************************************************/
/*!                    
 * \par  Description:
 *    文件（目录）列表控件控制流实现。
 * \param[in]    style_id ui editor工具设计的文件（目录）浏览控件模板
 * \param[in]    path_info 要进行文件（目录）浏览的路径结构体
 * \param[in]    dir_com 指向文件（目录）列表控件初始化结构体变量，是上层对文件（目录）列表的控制命令或参数
 * \param[out]   path_info 选择文件或目录后，返回文件（目录）浏览的路径结构体
 * \return       app_result_e
 * \retval           RESULT_XXX_PLAY 选择某类解码型文件进行播放返回
 * \retval           RESULT_CONFIRM 确定选择文件夹等返回
 * \retval           RESULT_REDRAW （逐级）取消返回AP，重绘UI
 * \retval           RESULT_DIR_ERROR_ENTER_DIR 错误返回，进入目录错误，包括 root
 * \retval           RESULT_DIR_ERROR_NO_FILE 错误返回，根目录下没有文件和文件夹
 * \retval           RESULT_DIR_ERROR_SETLOC 错误返回，顶层不是根目录情况下，set location 失败
 * \retval           other 收到返回值不为 RESULT_NULL 应用消息返回，见 app_result_e 定义
 * \ingroup      controls
 * \par          exmaple code
 * \code
    dir_com_data_t dir_com;
    app_result_e result;
    
    dir_com.browse_mode = 0;
    dir_com.menulist_cnt = 0;
    dir_com.root_layer = 0;
    dir_com.del_back = FALSE;
    dir_com.list_option_func = commondir_option_func;
    dir_com.menulist = NULL;
    result = gui_directory(DIRLIST, &path_info, &dir_com);
    if(result == RESULT_MUSIC_PLAY)//选择音乐进行播放
    {
        
    }
    else if(result == RESULT_REDRAW)//取消返回，重绘UI
    {
    
    }
    else if(result == RESULT_DIR_ERROR_NO_FILE)//根目录没有文件和文件夹
    {
    
    }
    else if(result == RESULT_DIR_ERROR_ENTER_DIR)//目录切换发生错误，进行错误处理
    {
    
    }
    else if(result == RESULT_DIR_ERROR_SETLOC)//顶层不是根目录情况下，set location 失败
    {
        
    }
    else//其他返回结果，需要做特殊处理
    {
    
    }
 * \endcode
 * \note 
 * \li  现在支持在文件浏览列表中添加菜单项。目前只做到支持添加 1 个不多于一页项的菜单。
 *      如果没有添加菜单项的话，必须把 menulist 置为NULL，并把 menulist_cnt 置为0。
*******************************************************************************/
app_result_e gui_directory (uint16 style_id, file_path_info_t *path_info, dir_com_data_t *dir_com)
{
    style_infor_t style_infor;
    app_result_e result = RESULT_NULL;
    
    int8 scroll_timer = -1;
    uint8 tag_backup;
    uint8 mode_backup, icon_backup;
    
    dir_history_valid = FALSE;
    style_infor.style_id = style_id;
    style_infor.type = UI_COM;
    
    tag_backup = get_app_timer_tag();
    change_app_timer_tag(APP_TIEMR_TAG_DIR);
    
    gui_get_headbar_mode(&mode_backup, &icon_backup);
    
    //count为0表示当前文件列表没有嵌入菜单
    dir_menu_title.count = 0;
    
    //获取 one_page_count
    ui_get_listbox_attrb(&style_infor, &dir_control.one_page_count, 0);
    dir_control.dir_disk = path_info->file_path.dirlocation.disk;
    dir_control.file_source = path_info->file_source;
    dir_control.dir_com = dir_com;

    //初始化 gui_directory 环境；如果初始化失败，path_info 会被初始化指向 ROOT
    result = init_directory_env(&dir_control, g_list_buffer, path_info);
    if(result != RESULT_NULL)
    {
        write_history_item(0, &dir_control, TRUE);
        change_app_timer_tag(tag_backup);
        return result;
    }

    //根目录下没有文件和文件的情况
    if((g_dir_browser.layer_no == 0)
    && ((g_dir_browser.dir_total + g_dir_browser.file_total) == 0))
    {
        //如果允许根目录没有文件项而又菜单项的浏览且有菜单项则不退出，否则退出
        if(!(((dir_com->browse_mode & 0x80) != 0) && (dir_control.dir_com->menulist_cnt != 0)))
        {
            //返回 RESULT_DIR_ERROR_NO_FILE
            write_history_item(0, &dir_control, TRUE);
            change_app_timer_tag(tag_backup);
            return RESULT_DIR_ERROR_NO_FILE;
        }
    }
    
    speed_control = 0;
    dir_control.enter_dir = TRUE;
    dir_control.draw_mode = LIST_DRAW_ALL;
    //进入文件浏览循环
    while(1)
    {
        //获取列表项
        if(dir_control.update_mode != LIST_UPDATE_NULL)
        {
            if(dir_control.enter_dir == TRUE)
            {
                //加载目录头，若当前列表有菜单项，会在此加载菜单项
                load_dir_title_data(&dir_control);
            
                dir_control.enter_dir = FALSE;
            }
            
            //加载文件记录项数据
            load_dir_item_data(&dir_control);

            dir_control.update_mode = LIST_UPDATE_NULL;
        }
              
        if((dir_control.draw_mode != LIST_DRAW_NULL) && (get_screensave_state() == FALSE))
        {
            if(dir_control.draw_mode == LIST_DRAW_ALL)
            {
                gui_set_headbar_mode(HEADBAR_MODE_NORMAL, this_headbar_icon_id);
            }
            
            update_listbox_private(&dir_control, &listbox_data);
            if(ui_show_listbox(&style_infor, &listbox_data, (uint32)dir_control.draw_mode) == UI_NEED_SCROLL)
            {
                if(scroll_timer == -1)//未创建，创建
                {
                    scroll_timer = set_app_timer(APP_TIMER_ATTRB_UI, 80, __directory_scroll_handle);
                }
            }
            else
            {
                //删除定时器，即使没创建也没问题
                kill_app_timer(scroll_timer);
                scroll_timer = -1;
            }
            
            dir_control.draw_mode = LIST_DRAW_NULL;
        }
        
        result = directory_msg_deal(path_info, &dir_control);
        if(result != RESULT_NULL)
        {
            break;
        }

        //挂起10ms，多任务调度
        sys_os_time_dly(1);  
    }
    
    write_history_item(0, &dir_control, TRUE);
    kill_app_timer(scroll_timer);
    change_app_timer_tag(tag_backup);
    return result;
}

/*! \endcond */

/*! \cond */

static void __directory_scroll_handle(void)
{
    ui_show_listbox(NULL, NULL, LIST_DRAW_ACTIVE_SCROLL);
}

//directory gui app msg deal
app_result_e directory_msg_deal(file_path_info_t *path_info, dir_control_t *p_dir_control)
{
    input_gui_msg_t input_msg;      //输入型消息，即gui消息
    msg_apps_type_e gui_event;      //gui 消息映射事件
    private_msg_t private_msg;      //私有消息
    app_result_e result = RESULT_NULL;
    
    //获取gui消息
    if(get_gui_msg(&input_msg) == TRUE)//有gui消息
    {
        //进行按键映射
        if(com_key_mapping(&input_msg, &gui_event, directory_key_map_list) == TRUE)
        {
            result = directory_msg_deal_gui(path_info, p_dir_control, gui_event);
        }
    } 
    else
    {
        //到此，gui消息队列已经处理完毕
        //处理ap私有消息和系统消息
        if(get_app_msg(&private_msg) == TRUE)
        {
            result = g_this_app_msg_dispatch(&private_msg);
            if(result == RESULT_REDRAW)
            {
                //返回重绘当前列表
                fsel_browser_enter_dir(CUR_DIR, 0, &g_dir_browser);
                p_dir_control->update_mode = LIST_UPDATE_REUPDATE;
                p_dir_control->draw_mode = LIST_DRAW_ALL;
                result = RESULT_NULL;//不退出控件
            }
            else if(result == RESULT_NULL)
            {
                //切换到应用control bank后需更新bank data
                p_dir_control->update_mode = LIST_UPDATE_REUPDATE;
                result = RESULT_NULL;//不退出控件
            }
            else
            {
                //do nothing for QAC
            }
        }
    }
    
    return result;
}

//加载目录头，若当前列表有菜单项，会在此加载菜单项
void load_dir_title_data(dir_control_t *p_dir_control)
{
    dir_com_data_t *t_dir_com = p_dir_control->dir_com;
    
    //备份当前目录控制结构体，这样就可以在下一级目录里重新进入该目录
    libc_memcpy(&last_dir_control, p_dir_control, sizeof(dir_control_t));

    //匹配是否嵌入菜单列表
    dir_menu_title.count = 0;
    if(t_dir_com->menulist != NULL)
    {
        t_dir_com->menulist_cnt = 1;//现在只支持1个，并且仅支持不多于一页菜单项
        match_list_menu(g_dir_browser.layer_no, &dir_menu_title, t_dir_com->menulist, t_dir_com->menulist_cnt);
    }

    //计算列表项总数并计算 dir_control 所有成员
    dir_control.list_total = g_dir_browser.dir_total + g_dir_browser.file_total + dir_menu_title.count;
    
    //读取当前层次文件浏览路径记录
    read_history_item(g_dir_browser.layer_no, p_dir_control);
    
    //计算其他 dir_control 成员
    init_dir_control(p_dir_control);
}

//加载文件记录项数据
void load_dir_item_data(dir_control_t *p_dir_control)
{
    file_brow_t *cur_file_list;
    brow_decode_t decode_param;
    
    //计算 decode_param，跳过前面有效部分列表项
    update_decode_param(&decode_param, p_dir_control, line_cnt);

    //获取新文件记录项
    //1.看看是否当前层级需要添加列表菜单，菜单所有情况下都会读回更新
    if((p_dir_control->dir_com->menulist != NULL) && (decode_param.top <= dir_menu_title.count))
    {
        //先从菜单中读取若干项
        while((decode_param.num > 0)
        && (decode_param.top <= dir_menu_title.count)
        && (decode_param.top <= p_dir_control->bottom) )
        {
            cur_file_list = g_file_list + (decode_param.top - p_dir_control->top);
            get_dir_menu_item(cur_file_list, &dir_menu_title, decode_param.top - 1);
            decode_param.top++;
            decode_param.num--;
        }
    }
    
    //2.看看是否需要添加新文件记录项
    if(decode_param.num > 0)
    {
        cur_file_list = g_file_list + (decode_param.top - p_dir_control->top);
        //读取更新文件记录项，跳过前面有效部分列表项
        decode_param.top = decode_param.top - dir_menu_title.count;//减去菜单项
        fsel_browser_get_items(&decode_param, cur_file_list);
    }
}

//计算 brow_decode_t 结构体，list_cnt仅对 LIST_UPDATE_NEXT 和 LIST_UPDATE_PREV 有效
void update_decode_param(brow_decode_t *decode_param, dir_control_t *p_dir_control, uint16 lines)
{
    decode_param->option = (uint8)p_dir_control->update_mode;
    switch(p_dir_control->update_mode)
    {
    case LIST_UPDATE_NEXT:
        decode_param->top = p_dir_control->bottom - (lines - 1);
        decode_param->num = (uint8)lines;
        break;
    case LIST_UPDATE_PREV:
        decode_param->top = p_dir_control->top;
        decode_param->num = (uint8)lines;
        break;
    default://全部更新
        decode_param->top = p_dir_control->top;
        decode_param->num = (uint8)(p_dir_control->bottom - p_dir_control->top + 1);
        break;
    }
}

//匹配文件浏览器菜单列表数组，仅支持 1 个菜单列表
bool match_list_menu(uint8 layer, menu_title_data_t* title, list_menu_t *menu, uint8 count)
{
    //无需匹配层级，或者匹配成功
    if((menu->layer == V_U8_INVALID) || (menu->layer == layer))
    {
        //读回菜单头数据
        com_get_confmenu_title((uint8)menu->list_menu, title);
        return TRUE;
    }
    
    return FALSE;
}

//获取菜单项字符串编码
void get_dir_menu_item(file_brow_t *file_list_item, menu_title_data_t *p_dir_menu_title, uint16 index)
{
    menu_item_data_t dir_menu_item;
    string_desc_t str_desc;
    
    file_list_item->name_buf[0] = 0xff;
    file_list_item->name_buf[1] = 0xfe;
    str_desc.data.str = file_list_item->name_buf + 2;
    str_desc.length = LIST_ITEM_BUFFER_LEN - 2;

    //读取菜单项
    com_get_confmenu_item(p_dir_menu_title, index, &dir_menu_item);
    //获取字符串ID code
    ui_get_multi_string_unicode(dir_menu_item.str_id, &str_desc);
}

/*! \endcond */
