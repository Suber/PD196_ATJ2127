/*******************************************************************************
 *                              us212A
 *                            Module: Common
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved. 
 *
 * History:         
 *      <author>    <time>           <version >             <desc>
 *       lzcai     2011-9-15 15:02     1.0             build this file 
*******************************************************************************/
/*!
 * \file     _ui_menulist.c
 * \brief    gui 控制流：菜单列表控件控制流实现。
 * \author   lzcai
 * \par      GENERAL DESCRIPTION:
 *               实现了一种可配置化的，带路径记忆功能的菜单列表控件。
 * \par      EXTERNALIZED FUNCTIONS:
 *               kernel，libc，ui驱动，applib，common_func，等等。
 * \version  1.0
 * \date     2011-9-15
*******************************************************************************/

#include "common_ui.h"

/*! \cond COMMON_API */

static listbox_private_t menu_private _BANK_DATA_ATTR_;
static void __menulist_scroll_handle(void);
void load_confmenu_item_data(menulist_control_t *p_ml_control);

#ifdef PC
__get_menu_func this_get_menu_func;
__get_callback_func this_get_callback_func;
__get_menu_option this_get_menu_option;

void load_menulist_simfunc(__get_menu_func func1, __get_callback_func func2, __get_menu_option func3)
{
    this_get_menu_func = func1;
    this_get_callback_func = func2;
    this_get_menu_option = func3;
}
#endif

extern void update_menulist_private(menulist_control_t *p_ml_control, listbox_private_t *menu_private, \
menu_title_action_t *title_action)__FAR__;
extern void write_history_item_menu(menulist_control_t *p_ml_control, bool update)__FAR__;
extern void load_conf_title(menulist_control_t *p_ml_control, menulist_stack_t *p_menu_stack)__FAR__;
extern void load_confmenu_title_data(menulist_control_t *p_ml_control, menulist_stack_t *p_menu_stack, \
menu_title_action_t *title_action)__FAR__;
extern void enter_newest_menulist(menulist_control_t *p_ml_control, menulist_stack_t *p_menu_stack, \
menu_title_action_t *title_action)__FAR__;
extern void re_enter_currentdir(menulist_control_t *p_ml_control, menulist_stack_t *p_menu_stack, \
menu_title_action_t *title_action, menu_cb_func menu_func)__FAR__;
extern void init_menulist_param(menulist_control_t *p_ml_control, menulist_stack_t *p_menu_stack, \
menu_title_action_t *title_action)__FAR__;

extern app_result_e menulist_msg_deal_gui(menulist_control_t *p_ml_control, menulist_stack_t *p_menu_stack, \
menu_title_action_t *title_action, msg_apps_type_e gui_event) __FAR__;
extern app_result_e menulist_msg_deal(menulist_control_t *p_ml_control, menulist_stack_t *p_menu_stack, \
menu_title_action_t *title_action)__FAR__;

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
    menu_com.browse_mode = 0;//从入口菜单开始浏览，为1表示从最新退出处开始浏览
    menu_com.menu_func = NULL;
    result = gui_menulist(MENULIST, &menu_com);
    if(result == RESULT_REDRAW)//菜单列表正常退出，重绘UI
    {
    
    }
    else//其他返回结果，需要做特殊处理
    {
    
    }
 * \endcode
 * \note 
*******************************************************************************/
app_result_e gui_menulist (uint16 style_id, menu_com_data_t *menu_com)
{
    menulist_control_t ml_control;  //菜单列表控制结构体
    menulist_stack_t menu_stack[3]; //菜单嵌套控制
    menu_title_action_t title_action;
    style_infor_t style_infor;
    
#ifdef PC
    menu_cb_leaf callback_func;
#endif
    
    app_result_e result = RESULT_NULL;
    
    uint16 last_style;              //上一个菜单 style id
    int8 scroll_timer = -1;
    uint8 tag_backup;
    uint8 mode_backup, icon_backup;
    
    tag_backup = get_app_timer_tag();
    change_app_timer_tag(APP_TIEMR_TAG_MENU);
    
    gui_get_headbar_mode(&mode_backup, &icon_backup);
    
    //读取菜单记忆路径，初始化菜单控制结构体
    menu_history_valid = FALSE;
    last_style = style_id;
    ml_control.style_id = style_id;
    ml_control.menu_com = menu_com;
    init_menulist_param(&ml_control, menu_stack, &title_action);
    
    //获取菜单头结构体，用来在 re_enter_currentdir 中判断是否到达菜单最顶层
    load_conf_title(&ml_control, menu_stack);
    //允许进入菜单时进行某些特殊处理（等同于重新进入入口菜单，title_action.str_id指向select项）
    re_enter_currentdir(&ml_control, menu_stack, &title_action, menu_com->menu_func);//此时title_action.str_id为-1
    //需要进入最新菜单退出路径
    if(menu_com->browse_mode == 1)
    {
        //搜索并进入最新菜单退出路径
        enter_newest_menulist(&ml_control, menu_stack, &title_action);
    }
    ml_control.enter_menu = TRUE;
    
    //进入菜单循环
    while(1)
    {
        if(ml_control.draw_mode != LIST_DRAW_NULL)//更新列表
        {
            //至此，要求 menu_stack 和 title_action 为当前层次菜单控制信息，否则就会出错
            if(title_action.style_id == V_U16_INVALID)
            {
                style_infor.style_id = style_id;
            }
            else
            {
                style_infor.style_id = title_action.style_id;
            }
            style_infor.type = UI_COM;
            if(style_infor.style_id != last_style)
            {
                ml_control.draw_mode = LIST_DRAW_ALL;
            }
            last_style = style_infor.style_id;
            
            //加载 listbox 控件属性参数
            ui_get_listbox_attrb(&style_infor, &ml_control.one_page_count, 0);
                
            if(ml_control.enter_menu == TRUE)
            {
                //加载菜单头并初始化 ml_control 
                load_confmenu_title_data(&ml_control, menu_stack, &title_action);
            }
    
            //加载一页菜单项
            load_confmenu_item_data(&ml_control);
            
            //显示菜单列表
            if(get_screensave_state() == FALSE)
            {
                if(ml_control.draw_mode == LIST_DRAW_ALL)
                {
                    if((last_style == MENULIST) || (last_style == MENULIST_TITLE))
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
            
            //即时叶子菜单响应
            if((ml_control.enter_menu == FALSE) && (g_menu_items[ml_control.active].callback != NULL))
            {
                #ifndef PC
                g_menu_items[ml_control.active].callback();
                #else
                callback_func = this_get_callback_func(g_menu_items[ml_control.active].str_id, g_menu_items[ml_control.active].str_id_sub);
                callback_func();
                #endif
            }
            
            ml_control.enter_menu = FALSE;
            ml_control.draw_mode = LIST_DRAW_NULL;
        }
        
        result = menulist_msg_deal(&ml_control, menu_stack, &title_action);
        if(result != RESULT_NULL)
        {
            break;
        }
 
        //挂起10ms，多任务调度
        sys_os_time_dly(1);
    }

    write_history_item_menu(&ml_control, TRUE);
    kill_app_timer(scroll_timer);
    change_app_timer_tag(tag_backup);
    gui_set_headbar_mode(mode_backup, icon_backup);
    return result;
}

/*! \endcond */

/*! \cond */

static void __menulist_scroll_handle(void)
{
    ui_show_listbox(NULL, NULL, LIST_DRAW_ACTIVE_SCROLL);
}

//menulist gui app msg deal
app_result_e menulist_msg_deal(menulist_control_t *p_ml_control, menulist_stack_t *p_menu_stack, \
menu_title_action_t *title_action)
{
    input_gui_msg_t input_msg;      //输入型消息，即gui消息
    msg_apps_type_e gui_event;      //gui 消息映射事件
    private_msg_t private_msg;      //私有消息
    app_result_e result = RESULT_NULL;
    
    //获取gui消息
    if(get_gui_msg(&input_msg) == TRUE)//有gui消息
    {
        //进行按键映射
        if(com_key_mapping(&input_msg, &gui_event, menulist_key_map_list) == TRUE)
        {
            result = menulist_msg_deal_gui(p_ml_control, p_menu_stack, title_action, gui_event);
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
            else if(result != RESULT_NULL)
            {
                //执行返回父菜单回调函数
                if((g_back_to_parant_arg != NULL) && (*g_back_to_parant_arg != 0))
                {
                    g_back_to_parent_func();
                    g_back_to_parant_arg = NULL;
                }
            }
            else
            {
            }
        }
    }
    
    return result;
}

//加载一页菜单项
void load_confmenu_item_data(menulist_control_t *p_ml_control)
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

/*! \endcond */
