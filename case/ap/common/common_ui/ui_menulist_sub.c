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
 * \file     _ui_menulist_sub.c
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

/*! \cond */

/******************************************************************************/
/*!                    
 * \par  Description:
 *    选择菜单列表中的下一项
 * \param[in]    p_ml_control：指向菜单列表控制结构体
 * \param[out]   p_ml_control：返回更新了的菜单列表参数
 * \return       none
 * \note 
*******************************************************************************/
void menulist_select_next(menulist_control_t *p_ml_control)
{
    //在页内切换，切后尚未到页底最后一项（最多到倒数第二项），更新激活项
    if((p_ml_control->list_no + 1) < p_ml_control->bottom)
    {
        p_ml_control->list_no++;
        p_ml_control->old = p_ml_control->active;
        p_ml_control->active++;
        p_ml_control->draw_mode = LIST_DRAW_ACTIVE;
    }
    else
    {
        //在列表最后一项，跳到列表头显示
        if(p_ml_control->list_no == (p_ml_control->total - 1))
        {
            p_ml_control->list_no = 0;
            //仅有一页的情况下，只需更新激活项
            if(p_ml_control->total <= p_ml_control->one_page_count)
            {
                //总项数多余1项才需要切换
                if(p_ml_control->total > 1)
                {
                    p_ml_control->old = p_ml_control->active;
                    p_ml_control->active = 0;
                    p_ml_control->draw_mode = LIST_DRAW_ACTIVE;
                }
            }
            //从尾切换到头，更新整个列表
            else
            {
                p_ml_control->top = 0;
                p_ml_control->bottom = p_ml_control->one_page_count - 1;
                p_ml_control->old = p_ml_control->active = 0;
                p_ml_control->update_mode = LIST_UPDATE_HEAD;
                p_ml_control->draw_mode = LIST_DRAW_LIST;
            }
        }
        else 
        {
            //在所有项中的倒数第二项，切到最后一项，只需更新激活项
            if(p_ml_control->list_no == (p_ml_control->total - 2))
            {
                p_ml_control->list_no++;
                p_ml_control->old = p_ml_control->active;
                p_ml_control->active++;
                p_ml_control->draw_mode = LIST_DRAW_ACTIVE;
            }
            //在当前页中的倒数第二项，并且后面有足够列表项，向下切页，更新整个列表
            else
            {
                p_ml_control->list_no++;
                p_ml_control->top++;
                p_ml_control->bottom++;
                p_ml_control->old = p_ml_control->active;
                p_ml_control->update_mode = LIST_UPDATE_NEXT;
                p_ml_control->draw_mode = LIST_DRAW_LIST;
            }
        }
    }
}

/******************************************************************************/
/*!                    
 * \par  Description:
 *    选择菜单列表中的上一项
 * \param[in]    p_ml_control：指向菜单列表控制结构体
 * \param[out]   p_ml_control：返回更新了的菜单列表参数
 * \return       none
 * \note 
*******************************************************************************/
void menulist_select_prev(menulist_control_t *p_ml_control)
{
    //在页内切换，切后尚未到页顶首项（最多到第二项），更新激活项
    if(p_ml_control->list_no > (p_ml_control->top + 1))
    {
        p_ml_control->list_no--;
        p_ml_control->old = p_ml_control->active;
        p_ml_control->active--;
        p_ml_control->draw_mode = LIST_DRAW_ACTIVE;
    }
    else
    {
        //在列表第一项，跳到列表尾显示
        if(p_ml_control->list_no == 0)
        {
            p_ml_control->list_no = p_ml_control->total - 1;
            //仅有一页的情况下，只需更新激活项
            if(p_ml_control->total <= p_ml_control->one_page_count)
            {
                //总项数多余1项才需要切换
                if(p_ml_control->total > 1)
                {
                    p_ml_control->old = p_ml_control->active;
                    p_ml_control->active = p_ml_control->total - 1;
                    p_ml_control->draw_mode = LIST_DRAW_ACTIVE;
                }
            }
            //从头切到尾，更新整个列表
            else
            {
                p_ml_control->top = p_ml_control->total - p_ml_control->one_page_count;
                p_ml_control->bottom = p_ml_control->list_no;
                p_ml_control->old = p_ml_control->active = p_ml_control->one_page_count - 1;
                p_ml_control->update_mode = LIST_UPDATE_TAIL;
                p_ml_control->draw_mode = LIST_DRAW_LIST;
            }
        }
        else 
        {
            //在所有项中的第二项，切到第一项，只需更新激活项
            if(p_ml_control->list_no == 1)
            {
                p_ml_control->list_no--;
                p_ml_control->old = p_ml_control->active;
                p_ml_control->active--;
                p_ml_control->draw_mode = LIST_DRAW_ACTIVE;
            }
            //在当前页中的第二项，并且前面有足够列表项，向上切页，更新整个列表
            else
            {
                p_ml_control->list_no--;
                p_ml_control->top--;
                p_ml_control->bottom--;
                p_ml_control->old = p_ml_control->active;
                p_ml_control->update_mode = LIST_UPDATE_PREV;
                p_ml_control->draw_mode = LIST_DRAW_LIST;
            }
        }
    }
}

//同步更新listbox私有数据结构
void update_menulist_private(menulist_control_t *p_ml_control, listbox_private_t *menu_private, \
menu_title_action_t *title_action)
{
    uint8 index;
    
    if((title_action->ret_str != NULL) 
    && (title_action->source_id == V_U16_INVALID))
    {
        menu_private->title.data.str = title_action->ret_str;
        menu_private->title.length = -1;
        menu_private->title.language = (int8)sys_comval->language_id;
    }
    else
    {
        if(title_action->title_id != V_U16_INVALID)
        {
            menu_private->title.data.id = title_action->title_id;
            menu_private->title.language = UNICODEID;
        }
        else
        {
            menu_private->title.data.id = g_menu_title.str_id;
            menu_private->title.language = UNICODEID;
        }
    }
    
    menu_private->item_valid = p_ml_control->bottom - p_ml_control->top + 1;
    menu_private->active = p_ml_control->active;
    menu_private->old = p_ml_control->old;
    menu_private->list_no = p_ml_control->list_no;
    menu_private->list_total = p_ml_control->total;
    
    for(index = 0; index < menu_private->item_valid; index++)
    {
        if((title_action->ret_str != NULL)
        && (title_action->source_id != V_U16_INVALID)
        && (com_get_confmenu_active(&g_menu_title, title_action->source_id) == (p_ml_control->top + index)))
        {
            menu_private->items[index].data.str = title_action->ret_str;
            menu_private->items[index].length = -1;
            menu_private->items[index].language = (int8)sys_comval->language_id;
        }
        else
        {
            menu_private->items[index].data.id = g_menu_items[index].str_id;
            menu_private->items[index].language = UNICODEID;
        }
        
        if( ((g_menu_items[index].child_index & 0x8000) == 0x8000)//RADIO 菜单
          &&(p_ml_control->select != V_U16_INVALID)//该条件保证在没有指定 select 项时不会乱显示 RADIO 项而造成用户误解
          &&((p_ml_control->top + index) == p_ml_control->select) )
        {
            menu_private->items[index].argv = ITEMS_BACK_TAG_IS_SELECT;
        }
        else
        {
            menu_private->items[index].argv = ITEMS_BACK_TAG_ISNOT_SELECT;
        }
        //前置图标与具体菜单项绑定的菜单模板
        if (title_action->style_id == MENULIST_HEAD)
        {
            menu_private->items[index].argv |= ITEMS_FRONT_TAG_IS_DISP;
            menu_private->items[index].argv |= ((uint16)(g_menu_items[index].pic_index) << ITEMS_FRONT_TAG_INDEX_SHIFT);
            menu_private->head_act_id = title_action->head_act_id;
            menu_private->head_id = title_action->head_id;
        }
    }
}

/*! \endcond */
