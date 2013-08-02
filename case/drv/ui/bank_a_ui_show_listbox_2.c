/*******************************************************************************
 *                              US212A
 *                            Module: ui driver
 *                 Copyright(c) 2003-2011 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>       <time>           <version >             <desc>
 *       betty     2011-09-07 16:00     1.0             build this file
 *******************************************************************************/
/*!
 * \file     ui_showlistbox_2.c
 * \brief   显示listbox的items
 * \author   betty
 * \par      GENERAL DESCRIPTION:
 *              解析listbox的控件数据结构，并按照刷新模式，显示listbox items
 * \par      EXTERNALIZED FUNCTIONS:
 *              none
 * \version 1.0
 * \date  2011/09/07
 *******************************************************************************/
#include "display.h"
#include "ui_driver.h"

//listbox items attribute bits location
#define ITEMS_BACK_TAG_INA_OPTION_SHIFT     (0)
#define ITEMS_BACK_TAG_INA_OPTION       (0x0003 << ITEMS_BACK_TAG_INA_OPTION_SHIFT)
#define BACK_TAG_INA_OPTION_NONE        (0x0000 << ITEMS_BACK_TAG_INA_OPTION_SHIFT)
#define BACK_TAG_INA_OPTION_SINGLE      (0x0001 << ITEMS_BACK_TAG_INA_OPTION_SHIFT)
#define BACK_TAG_INA_OPTION_MULTY       (0x0002 << ITEMS_BACK_TAG_INA_OPTION_SHIFT)
#define ITEMS_BACK_TAG_OPTION_SHIFT         (2)
#define ITEMS_BACK_TAG_OPTION           (0x0003 << ITEMS_BACK_TAG_OPTION_SHIFT)
#define BACK_TAG_OPTION_NONE            (0x0000 << ITEMS_BACK_TAG_OPTION_SHIFT)
#define BACK_TAG_OPTION_SINGLE          (0x0001 << ITEMS_BACK_TAG_OPTION_SHIFT)
#define BACK_TAG_OPTION_MULTY           (0x0002 << ITEMS_BACK_TAG_OPTION_SHIFT)
#define ITEMS_FRONT_TAG_INA_OPTION_SHIFT    (4)
#define ITEMS_FRONT_TAG_INA_OPTION      (0x0003 << ITEMS_FRONT_TAG_INA_OPTION_SHIFT)
#define FRONT_TAG_INA_OPTION_NONE       (0x0000 << ITEMS_FRONT_TAG_INA_OPTION_SHIFT)
#define FRONT_TAG_INA_OPTION_SINGLE     (0x0001 << ITEMS_FRONT_TAG_INA_OPTION_SHIFT)
#define FRONT_TAG_INA_OPTION_MULTY      (0x0002 << ITEMS_FRONT_TAG_INA_OPTION_SHIFT)
#define FRONT_TAG_INA_OPTION_NUMBER     (0x0003 << ITEMS_FRONT_TAG_INA_OPTION_SHIFT)
#define ITEMS_FRONT_TAG_OPTION_SHIFT        (6)
#define ITEMS_FRONT_TAG_OPTION          (0x0003 << ITEMS_FRONT_TAG_OPTION_SHIFT)
#define FRONT_TAG_OPTION_NONE           (0x0000 << ITEMS_FRONT_TAG_OPTION_SHIFT)
#define FRONT_TAG_OPTION_SINGLE         (0x0001 << ITEMS_FRONT_TAG_OPTION_SHIFT)
#define FRONT_TAG_OPTION_MULTY          (0x0002 << ITEMS_FRONT_TAG_OPTION_SHIFT)
#define FRONT_TAG_OPTION_NUMBER         (0x0003 << ITEMS_FRONT_TAG_OPTION_SHIFT)
#define ITEMS_INACTIVE_BG_OPTION_SHIFT      (8)
#define ITEMS_INACTIVE_BG_OPTION        (0x0003 << ITEMS_INACTIVE_BG_OPTION_SHIFT)
#define INACTIVE_BG_OPTION_COLOR        (0x0000 << ITEMS_INACTIVE_BG_OPTION_SHIFT)
#define INACTIVE_BG_OPTION_SPIC         (0x0001 << ITEMS_INACTIVE_BG_OPTION_SHIFT)
#define INACTIVE_BG_OPTION_MPIC         (0x0002 << ITEMS_INACTIVE_BG_OPTION_SHIFT)
#define ITEMS_ACTIVE_BG_OPTION_SHIFT        (10)
#define ITEMS_ACTIVE_BG_OPTION          (0x0003 << ITEMS_ACTIVE_BG_OPTION_SHIFT)
#define ACTIVE_BG_OPTION_COLOR          (0x0000 << ITEMS_ACTIVE_BG_OPTION_SHIFT)
#define ACTIVE_BG_OPTION_SPIC           (0x0001 << ITEMS_ACTIVE_BG_OPTION_SHIFT)
#define ACTIVE_BG_OPTION_MPIC           (0x0002 << ITEMS_ACTIVE_BG_OPTION_SHIFT)
#define ITEMS_ACTIVE_FONT_OPTION_SHIFT      (0)
#define ITEMS_ACTIVE_FONT_OPTION        (0x0003 << ITEMS_ACTIVE_FONT_OPTION_SHIFT)
#define ITEMS_ACTIVE_FONT_OPTION_SMALL  (0x0000 << ITEMS_ACTIVE_FONT_OPTION_SHIFT)
#define ITEMS_ACTIVE_FONT_OPTION_MIDDLE (0x0001 << ITEMS_ACTIVE_FONT_OPTION_SHIFT)
#define ITEMS_ACTIVE_FONT_OPTION_LARGE  (0x0002 << ITEMS_ACTIVE_FONT_OPTION_SHIFT)
#define ITEMS_ACTIVE_ALIGNMENT_OPTION_SHIFT (2)
#define ITEMS_ACTIVE_ALIGNMENT_OPTION       (0x0003 << ITEMS_ACTIVE_ALIGNMENT_OPTION_SHIFT)
#define ACTIVE_ALIGNMENT_OPTION_LEFT        (0x0000 << ITEMS_ACTIVE_ALIGNMENT_OPTION_SHIFT)
#define ACTIVE_ALIGNMENT_OPTION_CENTER      (0x0001 << ITEMS_ACTIVE_ALIGNMENT_OPTION_SHIFT)
#define ACTIVE_ALIGNMENT_OPTION_RIGHT       (0x0002 << ITEMS_ACTIVE_ALIGNMENT_OPTION_SHIFT)
#define ITEMS_ACTIVE_DISP_OPTION_SHIFT      (4)
#define ITEMS_ACTIVE_DISP_OPTION            (0x0003 << ITEMS_ACTIVE_DISP_OPTION_SHIFT)
#define ITEMS_ACTIVE_DISP_OPTION_MLINE      (0x0000 << ITEMS_ACTIVE_DISP_OPTION_SHIFT)
#define ITEMS_ACTIVE_DISP_OPTION_SCROLL     (0x0001 << ITEMS_ACTIVE_DISP_OPTION_SHIFT)
#define ITEMS_ACTIVE_DISP_OPTION_NORECT     (0x0002 << ITEMS_ACTIVE_DISP_OPTION_SHIFT)
#define ITEMS_ACTIVE_DISP_OPTION_ELLIPSIS   (0x0003 << ITEMS_ACTIVE_DISP_OPTION_SHIFT)
#define ITEMS_INACTIVE_FONT_OPTION_SHIFT    (6)
#define ITEMS_INACTIVE_FONT_OPTION          (0x0003 << ITEMS_INACTIVE_FONT_OPTION_SHIFT)
#define ITEMS_INACTIVE_FONT_OPTION_SMALL    (0x0000 << ITEMS_INACTIVE_FONT_OPTION_SHIFT)
#define ITEMS_INACTIVE_FONT_OPTION_MIDDLE   (0x0001 << ITEMS_INACTIVE_FONT_OPTION_SHIFT)
#define ITEMS_INACTIVE_FONT_OPTION_LARGE    (0x0002 << ITEMS_INACTIVE_FONT_OPTION_SHIFT)
#define ITEMS_INACTIVE_ALIGNMENT_OPTION_SHIFT   (8)
#define ITEMS_INACTIVE_ALIGNMENT_OPTION     (0x0003 << ITEMS_INACTIVE_ALIGNMENT_OPTION_SHIFT)
#define INACTIVE_ALIGNMENT_OPTION_LEFT      (0x0000 << ITEMS_INACTIVE_ALIGNMENT_OPTION_SHIFT)
#define INACTIVE_ALIGNMENT_OPTION_CENTER    (0x0001 << ITEMS_INACTIVE_ALIGNMENT_OPTION_SHIFT)
#define INACTIVE_ALIGNMENT_OPTION_RIGHT     (0x0002 << ITEMS_INACTIVE_ALIGNMENT_OPTION_SHIFT)
#define ITEMS_INACTIVE_DISP_OPTION_SHIFT    (10)
#define ITEMS_INACTIVE_DISP_OPTION          (0x0003 << ITEMS_INACTIVE_DISP_OPTION_SHIFT)
#define ITEMS_INACTIVE_DISP_OPTION_MLINE    (0x0000 << ITEMS_INACTIVE_DISP_OPTION_SHIFT)
#define ITEMS_INACTIVE_DISP_OPTION_SCROLL   (0x0001 << ITEMS_INACTIVE_DISP_OPTION_SHIFT)
#define ITEMS_INACTIVE_DISP_OPTION_NORECT   (0x0002 << ITEMS_INACTIVE_DISP_OPTION_SHIFT)
#define ITEMS_INACTIVE_DISP_OPTION_ELLIPSIS (0x0003 << ITEMS_INACTIVE_DISP_OPTION_SHIFT)
/*! \cond */
/******************************************************************************/
/*!
 * \par  Description:
 *    显示listbox items
 * \param[in]   listbox：listbox结构体变量指针
 * \param[in]   listbox_data：应用传递的listbox的私有数据结构体指针
 * \param[in]   itemnum：当前项是第几项
 * \param[in]   type：当前项是否为激活项
 * \param[in]   style_ptr：当前资源信息结构体指针
 * \param[out]  none
 * \return      uint8类型的参数，是否需要滚屏
 * \note
 *******************************************************************************/
uint8 show_list_item(listbox_t *listbox, listbox_private_t *listbox_data, uint16 itemnum, uint8 type,
        res_open_infor_t *style_ptr)
{
    string_desc_t *cur_item = &(listbox_data->items[itemnum]);
    region_t back_region, item_region;
    scroll_mode_t scroll_mode;
    uint16 tmp = 0;
    uint16 disp_mode = 0, textout_mode;
    uint16 head_y, back_y, data_y, tail_y;
    uint16 back_act_id = -1;
    uint8 result;
    uint8 actual_h;
    
    tmp = itemnum % listbox->num_one_page;

    //计算当前菜单项的Y坐标
    head_y = list_item.head_y + tmp * listbox->inc_y;
    back_y = list_item.back_y + tmp * listbox->inc_y;
    if (type == 1)
    {
        //str_atrib<0-1>0表示小字体,1表示中号字体，2表示大字体，默认中号字体
        if ((list_item.str_attrib & ITEMS_ACTIVE_FONT_OPTION) == ITEMS_ACTIVE_FONT_OPTION_SMALL)
        {
            disp_mode |= DISP_FONT_SMALL;
        }
        else if ((list_item.str_attrib & ITEMS_ACTIVE_FONT_OPTION) == ITEMS_ACTIVE_FONT_OPTION_LARGE)
        {
            disp_mode |= DISP_FONT_LARGE;
        }
        else
        {
            disp_mode |= DISP_FONT_NORMAL; //默认中号字体
        }
    }
    else
    {
        //str_atrib<6-7>0表示小字体,1表示中号字体，2表示大字体，默认中号字体
        if ((list_item.str_attrib & ITEMS_INACTIVE_FONT_OPTION) == ITEMS_INACTIVE_FONT_OPTION_SMALL)
        {
            disp_mode |= DISP_FONT_SMALL;
        }
        else if ((list_item.str_attrib & ITEMS_INACTIVE_FONT_OPTION) == ITEMS_INACTIVE_FONT_OPTION_LARGE)
        {
            disp_mode |= DISP_FONT_LARGE;
        }
        else
        {
            disp_mode |= DISP_FONT_NORMAL; //默认中号字体
        }
    }
    actual_h = get_fontlib_height(disp_mode);
    data_y = list_item.back_y + ((listbox->inc_y - actual_h)/2) + tmp * listbox->inc_y;
    tail_y = list_item.tail_y + tmp * listbox->inc_y;

    //显示区域设置
    back_region.x = list_item.data_x;
    back_region.y = list_item.back_y + tmp * listbox->inc_y;
    back_region.width = list_item.data_width;
    back_region.height = listbox->inc_y;
    
    item_region.x = list_item.data_x;
    item_region.y = data_y;
    item_region.width = list_item.data_width;
    item_region.height = actual_h;
    //显示活动项
    if (type == 1)
    {
        if ((list_item.attrib & ITEMS_ACTIVE_BG_OPTION) == ACTIVE_BG_OPTION_SPIC)
        {
            textout_mode = CHAR_FILL_NORMAL; //有背景图片，则是透明模式
            if (list_item.back_act_id != (uint16) - 1)
            {
                back_act_id = list_item.back_act_id;
                u_show_pic(list_item.back_act_id, list_item.back_x, back_y);
            }
        }
        else if ((list_item.attrib & ITEMS_ACTIVE_BG_OPTION) == ACTIVE_BG_OPTION_MPIC)
        {
            textout_mode = CHAR_FILL_NORMAL; //有背景图片，则是透明模式
            if (list_item.back_act_id != (uint16) - 1)
            {
                back_act_id = list_item.back_act_id + tmp;
                u_show_pic(list_item.back_act_id + tmp, list_item.back_x, back_y);
            }
        }
        else
        {
            textout_mode = CHAR_FILL_COLOR; //背景颜色，则是非透明模式,默认为非透明快速模式
            back_act_id = list_item.back_act_id;
            romf_set_backgd_color(list_item.back_act_id, 0, 0);
            //clear_screen(&back_region, 0, 0);
            romf_fill_rect(&back_region, (uint16) (dc.backgd_color & 0xffff));
        }
        disp_mode |= textout_mode;

        if (((list_item.attrib & ITEMS_FRONT_TAG_OPTION) != FRONT_TAG_OPTION_NONE) 
         && ((cur_item->argv & ITEMS_FRONT_TAG_ARGV) == ITEMS_FRONT_TAG_IS_DISP))
        {
            if ((list_item.attrib & ITEMS_FRONT_TAG_OPTION) == FRONT_TAG_OPTION_SINGLE)
            {
                u_show_pic(list_item.head_act_id, list_item.head_x, head_y);
            }
            else
            {
                if ((cur_item->argv & ITEMS_FRONT_TAG_INDEX) != ITEMS_FRONT_TAG_INDEX)
                {
                    u_show_pic(listbox_data->head_act_id + \
                    ((cur_item->argv & ITEMS_FRONT_TAG_INDEX) >> ITEMS_FRONT_TAG_INDEX_SHIFT), list_item.head_x, head_y);
                }
                else
                {
                u_show_pic(list_item.head_act_id + itemnum, list_item.head_x, head_y);
                }
            }
        }

        if ((cur_item->argv & ITEMS_BACK_TAG_ARGV) == ITEMS_BACK_TAG_IS_SELECT)
        {
            if ((list_item.attrib & ITEMS_BACK_TAG_OPTION) == BACK_TAG_OPTION_SINGLE)
            {
                u_show_pic(list_item.tail_act_id, list_item.tail_x, tail_y);
            }
            else if ((list_item.attrib & ITEMS_BACK_TAG_OPTION) == BACK_TAG_OPTION_MULTY)
            {
                u_show_pic(list_item.tail_act_id + itemnum, list_item.tail_x, tail_y);
            }
            else
            {
                //do nothing for QAC
            }
        }
        
        romf_set_pen_color(list_item.data_act_color, 0, 0);
        
        //str_attrib<2-3>表示活动项字符串对齐属性，0：左对齐，1：居中对齐，2：右对齐，默认为左对齐显示。
        if ((list_item.str_attrib & ITEMS_ACTIVE_ALIGNMENT_OPTION) == ACTIVE_ALIGNMENT_OPTION_CENTER)
        {
            disp_mode |= DISP_ALIGN_MEDIACY;
        }
        //else if ((list_item.str_attrib & ITEMS_ACTIVE_ALIGNMENT_OPTION) == ACTIVE_ALIGNMENT_OPTION_RIGHT)
        //{
        //    disp_mode |= DISP_ALIGN_RIGHT;
        //}
        else
        {
            disp_mode |= DISP_ALIGN_LEFT; //默认左对齐显示
        }
        //str_attrib<4-5>表示活动项长字符串显示属性，0：多行显示，1：滚屏显示，2：截断行显示
        //3：省略号显示，默认为滚屏显示。
        if ((list_item.str_attrib & ITEMS_ACTIVE_DISP_OPTION) == ITEMS_ACTIVE_DISP_OPTION_MLINE)
        {
            disp_mode |= DISP_LONGSTR_MULTI_ROW;
        }
        else if ((list_item.str_attrib & ITEMS_ACTIVE_DISP_OPTION) == ITEMS_ACTIVE_DISP_OPTION_NORECT)
        {
            disp_mode |= DISP_LONGSTR_NO_RECT;
        }
        else if ((list_item.str_attrib & ITEMS_ACTIVE_DISP_OPTION) == ITEMS_ACTIVE_DISP_OPTION_ELLIPSIS)
        {
            disp_mode |= DISP_LONGSTR_ELLIPSIS;
        }
        else if ((list_item.str_attrib & ITEMS_ACTIVE_DISP_OPTION) == ITEMS_ACTIVE_DISP_OPTION_SCROLL)
        {
            disp_mode |= DISP_LONGSTR_SCROLL;//默认滚屏显示
        }
        else
        {
            disp_mode |= DISP_LONGSTR_RECT;
        }
    }
    //非活动项
    else
    {
        if ((list_item.attrib & ITEMS_INACTIVE_BG_OPTION) == INACTIVE_BG_OPTION_SPIC)
        {
            textout_mode = CHAR_FILL_NORMAL; //有背景图片，则是透明模式
            if (list_item.back_id != (uint16) - 1)
            {
                u_show_pic(list_item.back_id, list_item.back_x, back_y);
            }
        }
        else if ((list_item.attrib & ITEMS_INACTIVE_BG_OPTION) == INACTIVE_BG_OPTION_MPIC)
        {
            textout_mode = CHAR_FILL_NORMAL; //有背景图片，则是透明模式
            if (list_item.back_id != (uint16) - 1)
            {
                u_show_pic(list_item.back_id + tmp, list_item.back_x, back_y);
            }
        }
        else
        {
            textout_mode = CHAR_FILL_COLOR; //背景颜色，则是非透明模式,默认为非透明快速模式
            romf_set_backgd_color(list_item.back_id, 0, 0);
            //clear_screen(&back_region, 0, 0);
            romf_fill_rect(&back_region, (uint16) (dc.backgd_color & 0xffff));
        }
        disp_mode |= textout_mode;

        if (type == 0xff)//仅清除背景
        {
            return UI_NO_ERR;
        }
        
        if (((list_item.attrib & ITEMS_FRONT_TAG_INA_OPTION) != FRONT_TAG_INA_OPTION_NONE) 
         && ((cur_item->argv & ITEMS_FRONT_TAG_ARGV) == ITEMS_FRONT_TAG_IS_DISP))
        {
            if ((list_item.attrib & ITEMS_FRONT_TAG_INA_OPTION) == FRONT_TAG_INA_OPTION_SINGLE)
            {
                u_show_pic(list_item.head_id, list_item.head_x, head_y);
            }
            else
            {
                if ((cur_item->argv & ITEMS_FRONT_TAG_INDEX) != ITEMS_FRONT_TAG_INDEX)
                {
                    u_show_pic(listbox_data->head_id + \
                    ((cur_item->argv & ITEMS_FRONT_TAG_INDEX) >> ITEMS_FRONT_TAG_INDEX_SHIFT), list_item.head_x, head_y);
                }
                else
                {
                    u_show_pic(list_item.head_id + itemnum, list_item.head_x, head_y);
                }
            }
        }

        if ((cur_item->argv & ITEMS_BACK_TAG_ARGV) == ITEMS_BACK_TAG_IS_SELECT)
        {
            if ((list_item.attrib & ITEMS_BACK_TAG_INA_OPTION) == BACK_TAG_INA_OPTION_SINGLE)
            {
                u_show_pic(list_item.tail_id, list_item.tail_x, tail_y);
            }
            else if ((list_item.attrib & ITEMS_BACK_TAG_INA_OPTION) == BACK_TAG_INA_OPTION_MULTY)
            {
                u_show_pic(list_item.tail_id + itemnum, list_item.tail_x, tail_y);
            }
            else
            {
                //do nothing for QAC
            }
        }
        romf_set_pen_color(list_item.data_color, 0, 0);
        
        //str_attrib<8-9>表示非活动项字符串对齐属性，0：左对齐，1：居中对齐，2：右对齐，默认为左对齐显示。
        if ((list_item.str_attrib & ITEMS_INACTIVE_ALIGNMENT_OPTION) == INACTIVE_ALIGNMENT_OPTION_CENTER)
        {
            disp_mode |= DISP_ALIGN_MEDIACY;
        }
        //else if ((list_item.str_attrib & ITEMS_INACTIVE_ALIGNMENT_OPTION) == INACTIVE_ALIGNMENT_OPTION_RIGHT)
        //{
        //    disp_mode |= DISP_ALIGN_RIGHT;
        //}
        else
        {
            disp_mode |= DISP_ALIGN_LEFT; //默认左对齐显示
        }

        //str_attrib<10-11>表示非活动项长字符串显示属性，0：多行显示，1：滚屏显示，2：截断行显示
        //3：省略号显示，默认为滚屏显示。
        if ((list_item.str_attrib & ITEMS_INACTIVE_DISP_OPTION) == ITEMS_INACTIVE_DISP_OPTION_MLINE)
        {
            disp_mode |= DISP_LONGSTR_MULTI_ROW;
        }
        else if ((list_item.str_attrib & ITEMS_INACTIVE_DISP_OPTION) == ITEMS_INACTIVE_DISP_OPTION_NORECT)
        {
            disp_mode |= DISP_LONGSTR_NO_RECT;
        }
        else if ((list_item.str_attrib & ITEMS_INACTIVE_DISP_OPTION) == ITEMS_INACTIVE_DISP_OPTION_SCROLL)
        {
            disp_mode |= DISP_LONGSTR_SCROLL;
        }
        else if ((list_item.str_attrib & ITEMS_INACTIVE_DISP_OPTION) == ITEMS_INACTIVE_DISP_OPTION_ELLIPSIS)
        {
            disp_mode |= DISP_LONGSTR_ELLIPSIS; //默认省略号显示
        }
        else
        {
            disp_mode |= DISP_LONGSTR_RECT;
        }
    }
    
    //语言选择菜单中，菜单项字符串特殊处理
    if((lang_option & LANG_LIST_MENU_OPTION) == LANG_LIST_MENU_ENTER)
    {
        disp_mode |= DISP_LANG_LIST_MENU;
    }
    
    result = show_string(cur_item, &item_region, disp_mode);
    if ((result == UI_NEED_SCROLL) && (type == 1))
    {
        if(textout_mode != CHAR_FILL_COLOR)//透明模式，有背景图片
        {
            //加载并初始化背景图片
            load_bgpic_sd(back_act_id, list_item.back_x, back_y, 1);
            disp_mode &= ~DISP_CHAR_FILL_OPTION;
            disp_mode |= CHAR_FILL_SDIMAGE;
        }
        
        //初始化字符串滚屏环境，包括给字符串末尾添加2空格
        scroll_mode.mode = TRUE;
        scroll_mode.disp_mode = disp_mode;
        scroll_desc.back_id = back_act_id;//仅用于记录背景颜色
        //scroll_desc.back_x = list_item.back_x;//暂时没用
        //scroll_desc.back_y = back_y;//暂时没用
        scroll_desc.pen_color = list_item.data_act_color;
        scroll_string(&scroll_str, &item_region, scroll_mode);
    }
    return result;
}
/*! \endcond */
