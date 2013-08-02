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
 * \file     _ui_menulist_sub2.c
 * \brief    commonUI 菜单列表控件控制流实现
 * \author   lzcai
 * \par      GENERAL DESCRIPTION:
 *               实现了一种可配置化的，带路径记忆功能的菜单列表GUI。
 * \par      EXTERNALIZED FUNCTIONS:
 *               直接调用ui 驱动中的listbox完成控件显示流任务。
 * \version 1.0
 * \date  2011-9-15
*******************************************************************************/

#include "common_ui.h"

#ifdef PC
extern __get_menu_func this_get_menu_func;
extern __get_callback_func this_get_callback_func;
extern __get_menu_option this_get_menu_option;
extern void load_menulist_simfunc(__get_menu_func func1, __get_callback_func func2, __get_menu_option func3);
#endif

extern void menulist_select_next(menulist_control_t *p_ml_control)__FAR__;
extern void menulist_select_prev(menulist_control_t *p_ml_control)__FAR__;
extern void write_history_item_menu(menulist_control_t *p_ml_control, bool update)__FAR__;
extern void load_conf_title(menulist_control_t *p_ml_control, menulist_stack_t *p_menu_stack)__FAR__;
extern void load_confmenu_title_data(menulist_control_t *p_ml_control, menulist_stack_t *p_menu_stack, \
menu_title_action_t *title_action)__FAR__;
extern void re_enter_currentdir(menulist_control_t *p_ml_control, menulist_stack_t *p_menu_stack, \
menu_title_action_t *title_action, menu_cb_func menu_func)__FAR__;
extern void re_redraw_currentdir(menulist_control_t *p_ml_control, menu_title_action_t *title_action)__FAR__;

/*! \cond */

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

//menulist gui app msg deal--gui
app_result_e menulist_msg_deal_gui(menulist_control_t *p_ml_control, menulist_stack_t *p_menu_stack, \
menu_title_action_t *title_action, msg_apps_type_e gui_event)
{
    app_result_e result = RESULT_NULL;
    menu_item_data_t *active_menu;
    menu_title_action_t temp_title_action;//临时变量
    bool deal_menu_result, deal_return_parent;
    
#ifdef PC
    menu_cb_func menu_func;
    menu_cb_option option_func;
#endif
    
    deal_menu_result = FALSE;
    deal_return_parent = FALSE;
    
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
        //保存当前层次菜单记录记忆
        write_history_item_menu(p_ml_control, FALSE);
        active_menu = &g_menu_items[p_ml_control->active];
        //进入叶子菜单处理
        if((active_menu->child_index & MENU_TYPE_LEAF) == MENU_TYPE_LEAF)
        {
            __init_action_title(&temp_title_action);//清空，这样才不会影响menu_func结果
            #ifndef PC
            result = active_menu->menu_func(&temp_title_action);
            #else
            menu_func = this_get_menu_func(active_menu->str_id, active_menu->str_id_sub);
            result = menu_func(&temp_title_action);
            #endif
            //叶子菜单执行返回值处理
            deal_menu_result = TRUE;
        }
        //进入根菜单处理
        else
        {
            //获取下级菜单的菜单头和活动项值
            if(active_menu->menu_func != NULL)
            {
                __init_action_title(&temp_title_action);//清空，这样才不会影响menu_func结果
                //根据根菜单项确定函数确定，该确定函数的返回值不处理，返回 RESULT_NULL 即可
                #ifndef PC
                result = active_menu->menu_func(&temp_title_action);
                #else
                menu_func = this_get_menu_func(active_menu->str_id, active_menu->str_id_sub);
                result = menu_func(&temp_title_action);
                #endif
                //非叶子菜单项除了进入下一级外，还可以当作叶子菜单执行，
                //如果返回值不是 RESULE_MENU_SON，就认为是当作叶子菜单执行
                if(result != RESULT_MENU_SON)
                {
                    deal_menu_result = TRUE;
                    
                    break;//case 分支从此处退出
                }
                
                libc_memcpy(title_action, &temp_title_action, sizeof(menu_title_action_t));
                if(title_action->menu_entry != V_U16_INVALID)
                {
                    //进入新入口菜单
                    p_ml_control->stack_pointer++;
                    p_ml_control->layer_no++;
                    
                    //进入新入口菜单，更新 menu stack 控制变量
                    update_menu_stack(p_ml_control, p_menu_stack, title_action, active_menu->child_index, 0);
                }
                else
                {
                    //进入下一级菜单
                    p_menu_stack[p_ml_control->stack_pointer].menu_id = (uint8)active_menu->child_index;
                    p_menu_stack[p_ml_control->stack_pointer].layer++;
                    p_ml_control->layer_no++;
                }
            }
            else
            {
                __init_action_title(title_action);//清空，正常进入下一级菜单
                if(title_action->menu_entry == V_U16_INVALID)
                {
                    //进入下一级菜单
                    p_menu_stack[p_ml_control->stack_pointer].menu_id = (uint8)active_menu->child_index;
                    p_menu_stack[p_ml_control->stack_pointer].layer++;
                    p_ml_control->layer_no++;
                }
            }
            
            //重新加载菜单列表
            p_ml_control->enter_menu = TRUE;
            p_ml_control->draw_mode = LIST_DRAW_LIST;
            result = RESULT_NULL;//不退出控件
        }
        break;
         
    case EVENT_MENULIST_BACK_PARENT:
        //执行返回父菜单回调函数
        if((g_back_to_parant_arg != NULL) && (*g_back_to_parant_arg != 0))
        {
            g_back_to_parent_func();
            g_back_to_parant_arg = NULL;
        }
        
        //保存当前层次菜单记录记忆
        write_history_item_menu(p_ml_control, FALSE);
        __init_action_title(title_action);//清空，这样才不会影响下次处理
        result = RESULT_NULL;
        deal_return_parent = TRUE;
        break;
        
    case EVENT_MENULIST_ENTER_OPTION:
        //保存当前层次菜单记录记忆
        write_history_item_menu(p_ml_control, FALSE);
        active_menu = &g_menu_items[p_ml_control->active];
        if(active_menu->menu_option != NULL)
        {
            #ifndef PC
            result = active_menu->menu_option();
            #else
            option_func = this_get_menu_option(active_menu->str_id, active_menu->str_id_sub);
            result = option_func();
            #endif
            switch(result)
            {
            case RESULT_MENU_REDRAW://重新加载菜单，用于嵌套调用 menulist 返回
                p_ml_control->enter_menu = TRUE;
                p_ml_control->draw_mode = LIST_DRAW_ALL;
                result = RESULT_NULL;//不退出控件
                break;
            
            case RESULT_MENU_PARENT:
                __init_action_title(title_action);//清空，这样就只会按照最普通方式返回上一级
                deal_return_parent = TRUE;
                break;
            
            case RESULT_REDRAW:
                p_ml_control->draw_mode = LIST_DRAW_ALL;
                result = RESULT_NULL;//不退出控件
                break;
            
            default:
                //do nothing
                break;
            }
        }
        break;
    
    default:
        result = com_message_box(gui_event);
        if(result == RESULT_REDRAW)
        {
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
        break;
    }
    
    //菜单回调函数返回值处理
    if(deal_menu_result == TRUE)
    {
        switch(result)
        {
        case RESULT_MENU_EXIT:
            result = RESULT_REDRAW;
            break;
        
        case RESULT_MENU_PARENT:
            libc_memcpy(title_action, &temp_title_action, sizeof(menu_title_action_t));
            deal_return_parent = TRUE;
            break;
        
        case RESULT_MENU_CUR:
            //重新进入当前层菜单，为了更新为新style和入口
            libc_memcpy(title_action, &temp_title_action, sizeof(menu_title_action_t));
            re_enter_currentdir(p_ml_control, p_menu_stack, title_action, p_ml_control->menu_com->menu_func);
            //重新加载菜单列表
            p_ml_control->enter_menu = TRUE;
            p_ml_control->draw_mode = LIST_DRAW_ALL;
            result = RESULT_NULL;//不退出控件
            break;
        
        case RESULT_MENU_REDRAW_PARENT:
            //重画后返回上一级
            re_redraw_currentdir(p_ml_control, &temp_title_action);
            __init_action_title(title_action);//清空，以默认方式返回上一级
            deal_return_parent = TRUE;
            break;
        
        case RESULT_REDRAW:
            p_ml_control->draw_mode = LIST_DRAW_ALL;
            result = RESULT_NULL;//不退出控件
            break;
        
        default:
            //do nothing
            break;
        }
    }
    
    //返回上一级菜单处理
    if(deal_return_parent == TRUE)
    {
        if(g_menu_title.father_index != MENU_TYPE_ENTRY) 
        {
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
            
            //获取菜单头结构体，用来在 re_enter_currentdir 中判断是否到达菜单最顶层
            load_conf_title(p_ml_control, p_menu_stack);
            
            //重新进入当前层菜单，也就是说，返回上一级等同于先简单返回上一级再重新进入当前层菜单
            re_enter_currentdir(p_ml_control, p_menu_stack, title_action, p_ml_control->menu_com->menu_func);
            //重新加载菜单列表
            p_ml_control->enter_menu = TRUE;
            if(result == RESULT_NULL)
            {
                p_ml_control->draw_mode = LIST_DRAW_LIST;
            }
            else//其他方式返回上一级，需要全部重绘
            {
                p_ml_control->draw_mode = LIST_DRAW_ALL;
            }
            
            result = RESULT_NULL;//不退出控件
        }
        else
        {
            //从入口菜单退出菜单管理器
            result = RESULT_REDRAW;
        }
    }
    
    return result;
}

/*! \endcond */
