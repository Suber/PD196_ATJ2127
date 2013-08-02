/*******************************************************************************
 *                              us212A
 *                            Module: commonUI-menulist
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 *
 * History:         
 *      <author>    <time>           <version >             <desc>
 *       lzcai     2011-9-15 15:02     1.0             build this file 
*******************************************************************************/
/*!
 * \file     _ui_menulist2.c
 * \brief    commonUI 菜单列表控件关键路径子函数实现
 * \author   lzcai
 * \par      GENERAL DESCRIPTION:
 *               实现了一种可配置化的，带路径记忆功能的菜单列表GUI。
 * \par      EXTERNALIZED FUNCTIONS:
 *               直接调用ui 驱动中的listbox完成控件显示流任务。
 * \version 1.0
 * \date  2011-9-15
*******************************************************************************/

#include "common_ui.h"

static listbox_private_t menu_private _BANK_DATA_ATTR_;

void init_ml_control(menulist_control_t *p_ml_control, uint16 str_id);
void read_history_item_menu(menulist_control_t *p_ml_control);
void read_next_layer_history(menulist_control_t *p_ml_control, history_item_t *history_item);
void write_history_item_menu(menulist_control_t *p_ml_control, bool update);

extern void update_menulist_private(menulist_control_t *p_ml_control, listbox_private_t *menu_private, \
menu_title_action_t *title_action)__FAR__;

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

//更新 menu stack 控制变量
static void update_menu_stack(menulist_control_t *p_ml_control, menulist_stack_t *p_menu_stack, \
menu_title_action_t *title_action, uint16 ori_menu_entry, uint8 mode)
{
    if(mode == 0)//进入动态入口菜单
    {
        p_menu_stack[p_ml_control->stack_pointer].cur_entry = (uint8)title_action->menu_entry;
        p_menu_stack[p_ml_control->stack_pointer].ori_entry = (uint8)ori_menu_entry;//old菜单头ID
        p_menu_stack[p_ml_control->stack_pointer].menu_id = (uint8)title_action->menu_entry;
        p_menu_stack[p_ml_control->stack_pointer].layer = 0;
    }
    else//返回上一级时替换为动态入口菜单，只能用于首层
    {
        //动态入口菜单，进入新的入口菜单，替换掉旧的入口菜单
        p_menu_stack[p_ml_control->stack_pointer].cur_entry = (uint8)title_action->menu_entry;
        p_menu_stack[p_ml_control->stack_pointer].ori_entry = (uint8)title_action->menu_entry;
        p_menu_stack[p_ml_control->stack_pointer].menu_id = (uint8)title_action->menu_entry;
        //层级不变，无需更新
    }
}

//初始化菜单列表参数
void init_menulist_param(menulist_control_t *p_ml_control, menulist_stack_t *p_menu_stack, \
menu_title_action_t *title_action)
{
    p_ml_control->stack_pointer = 0;
    p_ml_control->path_id = p_ml_control->menu_com->path_id;
    p_ml_control->layer_no = 0;
    p_ml_control->draw_mode = LIST_DRAW_ALL;
    p_menu_stack[p_ml_control->stack_pointer].cur_entry = p_ml_control->menu_com->menu_entry;
    p_menu_stack[p_ml_control->stack_pointer].ori_entry = p_ml_control->menu_com->menu_entry;
    p_menu_stack[p_ml_control->stack_pointer].menu_id = p_ml_control->menu_com->menu_entry;
    p_menu_stack[p_ml_control->stack_pointer].layer = 0;
    __init_action_title(title_action);
    title_action->style_id = p_ml_control->style_id;
}

//加载菜单头结构体
void load_conf_title(menulist_control_t *p_ml_control, menulist_stack_t *p_menu_stack)
{     
    com_get_confmenu_title(p_menu_stack[p_ml_control->stack_pointer].menu_id, &g_menu_title);
    //入口菜单，并且是动态入口菜单，那么需要更新 father_index 和 father_active
    if((p_menu_stack[p_ml_control->stack_pointer].layer == 0) 
    && (p_menu_stack[p_ml_control->stack_pointer].menu_id != p_menu_stack[p_ml_control->stack_pointer].ori_entry))
    {
        menu_title_data_t temp_title;
        
        com_get_confmenu_title(p_menu_stack[p_ml_control->stack_pointer].ori_entry, &temp_title);
        g_menu_title.father_index = temp_title.father_index;
        g_menu_title.father_active = temp_title.father_active;
    }
}

//加载菜单头并初始化 ml_control 
void load_confmenu_title_data(menulist_control_t *p_ml_control, menulist_stack_t *p_menu_stack, \
menu_title_action_t *title_action)
{
    //加载菜单头结构体
    load_conf_title(p_ml_control, p_menu_stack);
    
    //初始化 ml_control 
    init_ml_control(p_ml_control, title_action->str_id);
}

//搜索并进入菜单最新退出路径，就像进入到上次退出时的子菜单一样
void enter_newest_menulist(menulist_control_t *p_ml_control, menulist_stack_t *p_menu_stack, \
menu_title_action_t *title_action)
{
    menu_item_data_t *active_menu;
    history_item_t history_item;

    //循环模拟进入下一级菜单，直到叶子菜单
    while(1)
    {
        //加载菜单头并初始化 ml_control
        p_ml_control->one_page_count = LIST_NUM_ONE_PAGE_MAX;//为默认6项即可
        load_confmenu_title_data(p_ml_control, p_menu_stack, title_action);
        
        //获取下一层路径记忆，判断是否到最底层路径记忆
        read_next_layer_history(p_ml_control, &history_item);
        if(history_item.top == 0xffff)//无效路径记忆项
        {
            break;//已到最底层路径记忆，退出
        }
        
        //获取active 项，保存到 g_menu_items[0]
        com_get_confmenu_item(&g_menu_title, p_ml_control->list_no, g_menu_items);
        active_menu = &g_menu_items[0];
        if((active_menu->child_index & MENU_TYPE_LEAF) == MENU_TYPE_LEAF)
        {
            break;//已到叶子菜单，退出
        }
        
        __init_action_title(title_action);
        //自动选择 active(对应list_no) 项进入
        if(active_menu->menu_func != NULL)
        {
            #ifdef PC
            menu_cb_func func;
            #endif

            //根据根菜单项确定函数确定，该确定函数的返回值不处理，返回 RESULT_NULL 即可
            #ifndef PC
            active_menu->menu_func(title_action);
            #else
            func = this_get_menu_func(active_menu->str_id, active_menu->str_id_sub);
            func(title_action);
            #endif
            if(title_action->menu_entry != V_U16_INVALID)
            {
                //进入新入口菜单
                p_ml_control->stack_pointer++;
                p_ml_control->layer_no++;
                
                //进入新入口菜单，更新 menu stack 控制变量
                update_menu_stack(p_ml_control, p_menu_stack, title_action, active_menu->child_index, 0);
            }
        }
        
        if(title_action->menu_entry == V_U16_INVALID)
        {
            //进入下一级菜单
            p_menu_stack[p_ml_control->stack_pointer].menu_id = (uint8)active_menu->child_index;
            p_menu_stack[p_ml_control->stack_pointer].layer++;
            p_ml_control->layer_no++;
        }
    }
}

//重新进入当前层菜单，等于返回上一级后重新进入，为了更新为新style和入口
//（比如设置关机时间后返回，需要更新为新style和入口，以便查看设置结果）
void re_enter_currentdir(menulist_control_t *p_ml_control, menulist_stack_t *p_menu_stack, \
menu_title_action_t *title_action, menu_cb_func menu_func)
{
    menu_item_data_t *active_menu;
    menu_title_action_t temp_title_action;
    uint16 str_id_bk = title_action->str_id;
    
    __init_action_title(&temp_title_action);
    //已在最顶层，直接加载入口菜单
    if((p_ml_control->stack_pointer == 0) && (p_menu_stack[0].layer == 0))
    {
        //此时有可能需要更新最顶层入口菜单，要求在 menu_func 中处理
        if(menu_func != NULL)
        {
            menu_func(title_action);
            if(title_action->menu_entry != V_U16_INVALID)
            {
                //替换入口菜单，更新 menu stack 控制变量
                update_menu_stack(p_ml_control, p_menu_stack, title_action, 0, 1);
            }
        }
    }
    else//尚未到最顶层，返回上一级
    {
        //此时，有可能是返回作为动态菜单的上一级，但是因为该动态菜单节点和
        //原菜单节点在菜单树中是等价的，所以可以直接继续返回上一级
        if(p_menu_stack[p_ml_control->stack_pointer].layer == 0)
        {
            //退出当前入口菜单，返回上一个入口菜单
            p_ml_control->stack_pointer--;
            p_ml_control->layer_no--;
        }
        else
        {
            //返回上一级菜单
            p_menu_stack[p_ml_control->stack_pointer].menu_id = (uint8)g_menu_title.father_index;
            p_menu_stack[p_ml_control->stack_pointer].layer--;
            p_ml_control->layer_no--;
        }
            
        //加载菜单头并初始化 ml_control 
        p_ml_control->one_page_count = LIST_NUM_ONE_PAGE_MAX;//为默认6项即可
        load_confmenu_title_data(p_ml_control, p_menu_stack, &temp_title_action);
    
        //获取active 项，保存到 g_menu_items[0]
        com_get_confmenu_item(&g_menu_title, p_ml_control->list_no, g_menu_items);
        active_menu = &g_menu_items[0];
        
        //重新进入
        __init_action_title(title_action);
        if(active_menu->menu_func != NULL)
        {
            #ifdef PC
            menu_cb_func func;
            #endif
    
            //根据根菜单项确定函数确定，该确定函数的返回值不处理，返回 RESULT_NULL 即可
            #ifndef PC
            active_menu->menu_func(title_action);
            #else
            func = this_get_menu_func(active_menu->str_id, active_menu->str_id_sub);
            func(title_action);
            #endif
            if(title_action->menu_entry != V_U16_INVALID)
            {
                //进入新入口菜单
                p_ml_control->stack_pointer++;
                p_ml_control->layer_no++;
                
                //进入新入口菜单，更新 menu stack 控制变量
                update_menu_stack(p_ml_control, p_menu_stack, title_action, active_menu->child_index, 0);
            }
        }
        
        if(title_action->menu_entry == V_U16_INVALID)
        {
            //进入下一级菜单
            p_menu_stack[p_ml_control->stack_pointer].menu_id = (uint8)active_menu->child_index;
            p_menu_stack[p_ml_control->stack_pointer].layer++;
            p_ml_control->layer_no++;
        }
    }
    
    if(str_id_bk != V_U16_INVALID)
    {
        title_action->str_id = str_id_bk;
    }
}

//重绘当前菜单，停留0.5秒钟返回
void re_redraw_currentdir(menulist_control_t *p_ml_control, menu_title_action_t *title_action)
{
    style_infor_t style_infor;
    
    if(title_action->style_id == V_U16_INVALID)
    {
        style_infor.style_id = p_ml_control->style_id;
    }
    else
    {
        style_infor.style_id = title_action->style_id;
    }
    style_infor.type = UI_COM;
    p_ml_control->draw_mode = LIST_DRAW_ALL;
    
    //更改 select 项
    p_ml_control->select = com_get_confmenu_active(&g_menu_title, title_action->str_id);
    
    //显示菜单列表
    update_menulist_private(p_ml_control, &menu_private, title_action);
    ui_show_listbox(&style_infor, &menu_private, (uint32)p_ml_control->draw_mode);
    
    if((style_infor.style_id == MENULIST) || (style_infor.style_id == MENULIST_TITLE))
    {
        //强制刷新状态栏
        gui_headbar(HEADBAR_UPDATE_ALL);
    }
    
    //等待0.5秒钟后返回
    sys_os_time_dly(50);
}

//保存路径历史记录
void write_history_item_menu(menulist_control_t *p_ml_control, bool update)
{
    history_index_t temp_index;
    history_item_t temp_history_item;
    
    temp_history_item.top = p_ml_control->top;
    temp_history_item.list_no = p_ml_control->list_no;
    
    temp_index.type = PATH_HISTORY_MENU;
    temp_index.app_id = g_this_app_info->app_id;
    temp_index.path_id = p_ml_control->path_id;
    temp_index.layer = p_ml_control->layer_no;
    if(update == FALSE)
    {
        com_set_history_item(&temp_index, &temp_history_item);
    }
    else
    {
        com_update_path_history(&temp_index);
    }
}

//读取路径历史记录
void read_history_item_menu(menulist_control_t *p_ml_control)
{
    history_index_t temp_index;
    history_item_t temp_history_item;
    
    temp_index.type = PATH_HISTORY_MENU;
    temp_index.app_id = g_this_app_info->app_id;
    temp_index.path_id = p_ml_control->path_id;
    temp_index.layer = p_ml_control->layer_no;
    com_get_history_item(&temp_index, &temp_history_item);
    
    if(temp_history_item.top == V_U16_INVALID)
    {
        p_ml_control->top = 0;
        p_ml_control->list_no = 0;
    }
    else if((temp_history_item.top >= p_ml_control->total)
          ||(temp_history_item.list_no >= p_ml_control->total)
          ||(temp_history_item.top > temp_history_item.list_no)
          ||((temp_history_item.list_no - temp_history_item.top) > (p_ml_control->one_page_count - 1)))
    {
        //路径记忆参数出错，强制指向开头，并且重写入路径记忆，以强制把后面层级清为无效
        p_ml_control->top = 0;
        p_ml_control->list_no = 0;
        write_history_item_menu(p_ml_control, FALSE);
    }
    else
    {
        p_ml_control->top = temp_history_item.top;
        p_ml_control->list_no = temp_history_item.list_no;
    }
}

//读取下一层路径历史记录
void read_next_layer_history(menulist_control_t *p_ml_control, history_item_t *history_item)
{
    history_index_t temp_index;
    
    temp_index.type = PATH_HISTORY_MENU;
    temp_index.app_id = g_this_app_info->app_id;
    temp_index.path_id = p_ml_control->path_id;
    temp_index.layer = p_ml_control->layer_no + 1;
    com_get_history_item(&temp_index, history_item);
}

//初始化menulist_control_t数据结构
void init_ml_control(menulist_control_t *p_ml_control, uint16 str_id)
{
    uint16 select_no;
    
    //用来校验菜单路径记忆是否有效
    p_ml_control->total = g_menu_title.count;
    
    //初始化 p_ml_control total top list_no select 参数
    if(str_id != V_U16_INVALID)//通过字符串ID匹配菜单激活项
    {
        //匹配菜单项，决定激活项，优先放在倒数第二个
        select_no = com_get_confmenu_active(&g_menu_title, str_id);
        if(select_no != V_U16_INVALID)
        {
            //匹配到激活项，根据匹配结果初始化
            p_ml_control->select = select_no;
            p_ml_control->list_no = select_no;
            p_ml_control->top = 0;
        }
        else
        {
            //不能匹配到激活项，读取当前层次菜单记录记忆
            read_history_item_menu(p_ml_control);
            p_ml_control->select = V_U16_INVALID;
        }
    }
    else
    {
        //读取当前层次菜单记录记忆
        read_history_item_menu(p_ml_control);
        p_ml_control->select = V_U16_INVALID;
    }
    
    //尽量让激活项处于倒数第二项
    if(p_ml_control->total > p_ml_control->one_page_count)
    {
        if(p_ml_control->list_no == (p_ml_control->total - 1))//最后一项，只能停在当前页最后一项
        {
            p_ml_control->top =  p_ml_control->list_no - (p_ml_control->one_page_count - 1);
        }
        else if(p_ml_control->list_no >= (p_ml_control->one_page_count - 2))//不在最后一项，可以停在当前页倒数第二项
        {
            p_ml_control->top = p_ml_control->list_no - (p_ml_control->one_page_count - 2);
        }
        else
        {
            p_ml_control->top = 0;
        }
    }
    else
    {
        p_ml_control->top = 0;
    }
    
    //初始化 p_ml_control 其他参数
    p_ml_control->bottom = p_ml_control->top + p_ml_control->one_page_count - 1;
    if(p_ml_control->bottom > (p_ml_control->total - 1))
    {
        p_ml_control->bottom = p_ml_control->total - 1;
    }
    p_ml_control->old = p_ml_control->active = p_ml_control->list_no - p_ml_control->top;
}
