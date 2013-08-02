/*******************************************************************************
 *                              us212A
 *                            Module: Common
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved. 
 *
 * History:         
 *      <author>    <time>           <version >             <desc>
 *       lzcai     2012-6-28 9:13     1.0             build this file 
*******************************************************************************/
/*!
 * \file     _ui_menulist_simple.c
 * \brief    gui 控制流：简化版菜单列表控件控制流实现。
 * \author   lzcai
 * \par      GENERAL DESCRIPTION:
 *               实现了一种可配置化的，不带路径记忆功能，不支持动态菜单功能的简化版
 *           菜单列表控件，这种菜单列表属于命令型菜单列表，它只会有一层菜单，选择了
 *           叶子菜单项后执行完立即退出。
 * \par      EXTERNALIZED FUNCTIONS:
 *               kernel，libc，ui驱动，applib，common_func，等等。
 * \version  1.0
 * \date     2012-6-28
*******************************************************************************/

#include "common_ui.h"

/*! \cond COMMON_API */

static listbox_private_t menu_private _BANK_DATA_ATTR_;
static void __menulist_scroll_handle(void);
static void __init_action_title(menu_title_action_t *title_action);
static void __load_conf_title_simple(menulist_control_t *p_ml_control, uint8 menu_id);
static void __load_conf_item_simple(menulist_control_t *p_ml_control);

#ifdef PC
extern __get_menu_func this_get_menu_func;
extern __get_callback_func this_get_callback_func;
extern __get_menu_option this_get_menu_option;
extern void load_menulist_simfunc(__get_menu_func func1, __get_callback_func func2, __get_menu_option func3);
#endif

extern void menulist_select_next(menulist_control_t *p_ml_control);
extern void menulist_select_prev(menulist_control_t *p_ml_control);
extern void update_menulist_private(menulist_control_t *p_ml_control, listbox_private_t *menu_private, \
menu_title_action_t *title_action);

//menulist gui app msg deal
app_result_e menulist_simple_msg_deal(menulist_control_t *p_ml_control, menu_title_action_t *title_action)
{
    input_gui_msg_t input_msg;      //输入型消息，即gui消息
    msg_apps_type_e gui_event;      //gui 消息映射事件
    private_msg_t private_msg;      //私有消息
    app_result_e result = RESULT_NULL;
    menu_item_data_t *active_menu;
    
#ifdef PC
    menu_cb_func menu_func;
#endif
    
    //获取gui消息
    if(get_gui_msg(&input_msg) == TRUE)//有gui消息
    {
        //进行按键映射
        if(com_key_mapping(&input_msg, &gui_event, menulist_key_map_list) == TRUE)
        {
            //GUI按键消息处理
            switch(gui_event)
            {
            case EVENT_MENULIST_SELECT_NEXT:
                //切换到下一个菜单项
                menulist_select_next(p_ml_control);
                break;
            
            case EVENT_MENULIST_SELECT_PREV:
                //切换到上一个菜单项
                menulist_select_prev(p_ml_control);
                break;
            
            case EVENT_MENULIST_ENTER_SON:
                active_menu = &g_menu_items[p_ml_control->active];
                //进入叶子菜单处理
                if((active_menu->child_index & MENU_TYPE_LEAF) == MENU_TYPE_LEAF)
                {
                    __init_action_title(title_action);
                    #ifndef PC
                    result = active_menu->menu_func(title_action);
                    #else
                    menu_func = this_get_menu_func(active_menu->str_id, active_menu->str_id_sub);
                    result = menu_func(title_action);
                    #endif
                    
                    if((result == RESULT_MENU_EXIT) ||(result == RESULT_MENU_PARENT)
                     ||(result == RESULT_MENU_CUR) ||(result == RESULT_MENU_REDRAW_PARENT)
                     ||(result == RESULT_REDRAW)||(result == RESULT_NULL))
                    {
                        result = RESULT_REDRAW;
                    }
                }
                else//只支持叶子菜单项，根菜单项不做处理，立即退出
                {
                    //退出菜单管理器
                    result = RESULT_REDRAW;
                }
                break;
                
            case EVENT_MENULIST_BACK_PARENT:
            case EVENT_MENULIST_ENTER_OPTION://option事件当作parent事件处理
                //退出菜单管理器
                result = RESULT_REDRAW;
                break;
            
            default:
                result = com_message_box(gui_event);
                if(result == RESULT_REDRAW)
                {
                    p_ml_control->draw_mode = LIST_DRAW_ALL;
                    result = RESULT_NULL;//不退出控件
                }
                break;
            }
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
                //界面显示提示，返回重绘UI
                p_ml_control->draw_mode = LIST_DRAW_ALL;
                result = RESULT_NULL;//不退出控件
            }
        }
    }
    
    return result;
}

/******************************************************************************/
/*!                    
 * \par  Description:
 *    菜单列表控件控制流实现。
 * \param[in]    style_id ui editor工具设计的菜单列表控件模板
 * \param[in]    menu_com 指向菜单列表控件初始化结构体变量，是上层对菜单列表的控制命令或参数
 * \param[out]   none    
 * \return       app_result_e
 * \retval           RESULT_MENU_EXIT （仅在内部使用的消息）退出菜单返回，不论在哪一级菜单，都可以直接退出到调用者
 * \retval           RESULT_MENU_PARENT （仅在内部使用的消息）退回上一级，叶子菜单项的确定函数返回
 * \retval           RESULT_MENU_CUR （仅在内部使用的消息）重新进入当前级菜单，在执行确定函数后要进入动态菜单时返回
 * \retval           RESULT_MENU_SON （仅在内部使用的消息）进入下一级，根菜单项的确定函数返回，或返回RESULT_NULL
 * \retval           RESULT_MENU_REDRAW_PARENT （仅在内部使用的消息）重绘菜单后返回上一级，比如选择风格后
 * \retval           RESULT_MENU_REDRAW （仅在内部使用的消息）重新加载菜单，用于嵌套调用 menulist 返回
 * \retval           RESULT_REDRAW （逐级）取消，或直接退出菜单等返回AP，重绘UI
 * \retval           other 收到返回值不为 RESULT_NULL 应用消息返回，见 app_result_e 定义
 * \ingroup      controls
 * \par          exmaple code
 * \code
    menu_com_data_t menu_com;
    app_result_e result;
    
    menu_com.app_id = APP_ID_MUSIC;
    menu_com.menu_entry = MENU_MUSIC_PLAY;
    menu_com.path_id = PATH_ID_MUSIC_PLAY;
    menu_com.browse_mode = 0;//只支持从入口菜单开始浏览
    menu_com.menu_func = NULL;
    result = gui_menulist_simple(MENULIST_OPTION, &menu_com);
    if(result == RESULT_REDRAW)//菜单列表正常退出，重绘UI
    {
    
    }
    else//其他返回结果，需要做特殊处理
    {
    
    }
 * \endcode
 * \note 
*******************************************************************************/
app_result_e gui_menulist_simple (uint16 style_id, menu_com_data_t *menu_com)
{
    menulist_control_t ml_control;  //菜单列表控制结构体
    menu_title_action_t title_action;
    style_infor_t style_infor;
    
    app_result_e result = RESULT_NULL;
    
    uint8 cur_menu_id;              //当前菜单id
    int8 scroll_timer = -1;
    uint8 tag_backup;
    uint8 mode_backup, icon_backup;
    
    tag_backup = get_app_timer_tag();
    change_app_timer_tag(APP_TIEMR_TAG_MENU_SIMPLE);
    
    gui_get_headbar_mode(&mode_backup, &icon_backup);
    
    //初始化菜单控制结构体
    ml_control.style_id = style_id;
    ml_control.stack_pointer = 0;
    ml_control.draw_mode = LIST_DRAW_ALL;
    __init_action_title(&title_action);
    title_action.style_id = style_id;
    
    //执行该函数，可以实现一些特殊功能，比如修改菜单标题或菜单项字符串等
    if(menu_com->menu_func != NULL)
    {
        menu_com->menu_func(&title_action);
    }
    
    cur_menu_id = menu_com->menu_entry;
    ml_control.enter_menu = TRUE;
    
    //进入菜单循环
    while(1)
    {
        if(ml_control.draw_mode != LIST_DRAW_NULL)//更新列表
        {
            //至此，要求 menu_stack 和 title_action 为当前层次菜单控制信息，否则就会出错
            style_infor.style_id = style_id;
            style_infor.type = UI_COM;
            
            //加载 listbox 控件属性参数
            ui_get_listbox_attrb(&style_infor, &ml_control.one_page_count, 0);
                
            if(ml_control.enter_menu == TRUE)
            {
                //加载菜单头并初始化 ml_control 
                __load_conf_title_simple(&ml_control, cur_menu_id);
            }
    
            //加载一页菜单项
            __load_conf_item_simple(&ml_control);
            
            //显示菜单列表
            if(get_screensave_state() == FALSE)
            {
                if(ml_control.draw_mode == LIST_DRAW_ALL)
                {
                    if((style_id == MENULIST) || (style_id == MENULIST_TITLE))
                    {
                        gui_set_headbar_mode(HEADBAR_MODE_NORMAL, this_headbar_icon_id);
                    }
                    else
                    {
                        gui_set_headbar_mode(HEADBAR_MODE_NULL, this_headbar_icon_id);
                    }
                }
                
                update_menulist_private(&ml_control, &menu_private, &title_action);
                if(ui_show_listbox(&style_infor, &menu_private, (uint32)ml_control.draw_mode) == UI_NEED_SCROLL)
                {
                    if(scroll_timer == -1)//未创建，创建
                    {
                        scroll_timer = set_app_timer(APP_TIMER_ATTRB_UI, 80, __menulist_scroll_handle);
                    }
                }
                else
                {
                    //删除定时器，即使没创建也没问题
                    kill_app_timer(scroll_timer);
                    scroll_timer = -1;
                }
            }
            
            ml_control.enter_menu = FALSE;
            ml_control.draw_mode = LIST_DRAW_NULL;
        }
        
        result = menulist_simple_msg_deal(&ml_control, &title_action);
        if(result != RESULT_NULL)
        {
            break;
        }
 
        //挂起10ms，多任务调度
        sys_os_time_dly(1);
    }

    kill_app_timer(scroll_timer);
    change_app_timer_tag(tag_backup);
    gui_set_headbar_mode(mode_backup, icon_backup);
    return result;
}

/*! \endcond */

/*! \cond */

//加载菜单头并初始化 ml_control 
static void __load_conf_title_simple(menulist_control_t *p_ml_control, uint8 menu_id)
{
    //加载菜单头结构体
    com_get_confmenu_title(menu_id, &g_menu_title);
    
    //初始化 ml_control 
    p_ml_control->select = 0;
    p_ml_control->list_no = 0;
    p_ml_control->top = 0;
    p_ml_control->total = g_menu_title.count;
    p_ml_control->bottom = p_ml_control->top + p_ml_control->one_page_count - 1;
    if(p_ml_control->bottom > (p_ml_control->total - 1))
    {
        p_ml_control->bottom = p_ml_control->total - 1;
    }
    p_ml_control->old = p_ml_control->active = 0;
}

//加载一页菜单项
static void __load_conf_item_simple(menulist_control_t *p_ml_control)
{
    uint8 i;
    
    for(i = 0; i < p_ml_control->one_page_count; i++)
    {
        if((i + p_ml_control->top) > p_ml_control->bottom)
        {
            break;
        }
        
        //获取菜单项字符串
        com_get_confmenu_item(&g_menu_title, p_ml_control->top + i, g_menu_items + i);
    }
}

static void __init_action_title(menu_title_action_t *title_action)
{
    libc_memset(title_action, 0xff, sizeof(menu_title_action_t));
    //title_action->style_id = V_U16_INVALID;
    //title_action->title_id = V_U16_INVALID;
    //title_action->str_id = V_U16_INVALID;
    //title_action->menu_entry = V_U16_INVALID;//强制menu_entry条件不满足
    //title_action->source_id = V_U16_INVALID;
    title_action->ret_str = NULL;
}

static void __menulist_scroll_handle(void)
{
    ui_show_listbox(NULL, NULL, LIST_DRAW_ACTIVE_SCROLL);
}

/*! \endcond */
