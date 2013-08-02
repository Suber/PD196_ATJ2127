/*******************************************************************************
 *                              US212A
 *                            Module: ui driver
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>           <time>          <version >             <desc>
 *       liutingting     2011-09-05 10:00     1.0             build this file
 *******************************************************************************/
/*!
 * \file     ui_show_dialogbox.c
 * \brief   显示dialogbox的模块
 * \author   liutingting
 * \par      GENERAL DESCRIPTION:
 *               实现dialogbox控件数据的解析并显示dialogbox
 * \par      EXTERNALIZED FUNCTIONS:
 *              直接调用系统SD文件系统的接口函数解析并显示dialogbox
 * \version 1.0
 * \date  2011-9-05
 *******************************************************************************/
#include "display.h"
#include "ui_driver.h"

//dialog attribute bits location
#define DIALOG_SHOW_EN                  (0x0001 << 0)

#define DESC_ALIGNMENT_OPTION_SHIFT     (2)
#define DESC_ALIGNMENT_OPTION           (0x0003 << DESC_ALIGNMENT_OPTION_SHIFT)
#define DESC_ALIGNMENT_OPTION_LEFT      (0x0000 << DESC_ALIGNMENT_OPTION_SHIFT)
#define DESC_ALIGNMENT_OPTION_CENTER    (0x0001 << DESC_ALIGNMENT_OPTION_SHIFT)
#define DESC_ALIGNMENT_OPTION_RIGHT     (0x0002 << DESC_ALIGNMENT_OPTION_SHIFT)

#define DESC_FONT_OPTION_SHIFT          (4)
#define DESC_FONT_OPTION                (0x0003 << DESC_FONT_OPTION_SHIFT)
#define DESC_FONT_OPTION_SMALL          (0x0000 << DESC_FONT_OPTION_SHIFT)
#define DESC_FONT_OPTION_MIDDLE         (0x0001 << DESC_FONT_OPTION_SHIFT)
#define DESC_FONT_OPTION_LARGE          (0x0002 << DESC_FONT_OPTION_SHIFT)

#define DIALOG_SHOW_ITEMS_EN            (0x0001 << 6)
#define DIALOG_SHOW_ICON_EN             (0x0001 << 7)

//dialog items attribute bits location
#define ITEMS_INACTIVE_BG_OPTION_SHIFT  (0)
#define ITEMS_INACTIVE_BG_OPTION        (0x0003 << ITEMS_INACTIVE_BG_OPTION_SHIFT)
#define INACTIVE_BG_OPTION_COLOR        (0x0000 << ITEMS_INACTIVE_BG_OPTION_SHIFT)
#define INACTIVE_BG_OPTION_SPIC         (0x0001 << ITEMS_INACTIVE_BG_OPTION_SHIFT)
#define INACTIVE_BG_OPTION_MPIC         (0x0002 << ITEMS_INACTIVE_BG_OPTION_SHIFT)
#define ITEMS_ACTIVE_BG_OPTION_SHIFT    (2)
#define ITEMS_ACTIVE_BG_OPTION          (0x0003 << ITEMS_ACTIVE_BG_OPTION_SHIFT)
#define ACTIVE_BG_OPTION_COLOR          (0x0000 << ITEMS_ACTIVE_BG_OPTION_SHIFT)
#define ACTIVE_BG_OPTION_SPIC           (0x0001 << ITEMS_ACTIVE_BG_OPTION_SHIFT)
#define ACTIVE_BG_OPTION_MPIC           (0x0002 << ITEMS_ACTIVE_BG_OPTION_SHIFT)

#define DIALOG_ALIGNMENT_OPTION_SHIFT   (4)
#define DIALOG_ALIGNMENT_OPTION         (0x0003 << DIALOG_ALIGNMENT_OPTION_SHIFT)
#define DIALOG_ALIGNMENT_OPTION_LEFT    (0x0000 << DIALOG_ALIGNMENT_OPTION_SHIFT)
#define DIALOG_ALIGNMENT_OPTION_CENTER  (0x0001 << DIALOG_ALIGNMENT_OPTION_SHIFT)
#define DIALOG_ALIGNMENT_OPTION_RIGHT   (0x0002 << DIALOG_ALIGNMENT_OPTION_SHIFT)
#define ITEMS_FONT_OPTION_SHIFT         (6)
#define ITEMS_FONT_OPTION               (0x0003 << ITEMS_FONT_OPTION_SHIFT)
#define ITEMS_FONT_OPTION_SMALL         (0x0000 << ITEMS_FONT_OPTION_SHIFT)
#define ITEMS_FONT_OPTION_MIDDLE        (0x0001 << ITEMS_FONT_OPTION_SHIFT)
#define ITEMS_FONT_OPTION_LARGE         (0x0002 << ITEMS_FONT_OPTION_SHIFT)

void show_dialogbox_desc(dialogbox_t dialogbox, dialog_private_t *dialogbox_data);
void show_dialogbox_button(uint32 addr, dialog_private_t *dialogbox_data, uint16 itemnum, uint8 type,
        res_open_infor_t *style_ptr);
/*! \cond UI_DRIVER_API */
/******************************************************************************/
/*!
 * \par  Description:
 *    显示dialogbox
 * \param[in]   dialogbox_style 指向dialogbox的style结构体变量
 * \param[in]   dialogbox_data 应用传递的dialogbox的私有数据结构体指针
 * \param[in]   mode dialogbox的刷新模式，参见dialog_draw_mode_e定义
 * \param[out]  none
 * \return      ui_result_e 类型的参数，参见ui_result_e定义
 * \ingroup     ui_controls
 * \par         exmaple code
 * \code
    例子1：显示 YES/NO 对话框，由 gui_dialog 封装，详细见 gui_dialog 说明
    style_infor_t dialogbox_style;
    dialog_private_t dialogbox_param;
    
    dialogbox_style.style_id = DIALOGBOX_ID1;//必须选择两个按钮的对话框模板
    dialogbox_style.type = UI_AP;
    dialogbox_param.icon_id = dialog_com->icon_id;
    dialogbox_param.string_desc = dialog_com->string_desc;
    dialog_private.button_cnt = 2;
    dialog_private.buttons[0] = S_BUTTON_YES;
    dialog_private.buttons[1] = S_BUTTON_NO;
    dialog_private.old = dialog_private.active = 0;//默认yes
    
    ui_show_dialogbox(&dialogbox_style, &dialogbox_param, DIALOG_DRAW_ALL);
    
    例子2：显示消息提示，由 gui_dialog_msg 封装，详细见 gui_dialog_msg 说明
    style_infor_t dialogbox_style;
    dialog_private_t dialogbox_param;
    
    dialogbox_style.style_id = DIALOGBOX_ID1;//必须选择没有按钮的对话框模板
    dialogbox_style.type = UI_AP;
    dialogbox_param.icon_id = dialog_com->icon_id;
    dialogbox_param.string_desc = dialog_com->string_desc;
    dialog_private.button_cnt = 0;
    
    ui_show_dialogbox(&dialogbox_style, &dialogbox_param, DIALOG_DRAW_ALL);
 * \endcode
 * \note
 *******************************************************************************/
ui_result_e show_dialogbox(style_infor_t *dialogbox_style, dialog_private_t *dialogbox_data, dialog_draw_mode_e mode)
{
    dialogbox_t dialogbox;
    res_open_infor_t *style_ptr;
    DC oldDC_sav;
    uint32 dialogbox_entry_addr;
    uint8 i;

    /*根据打开资源文件的类型，将指针指向不同的全局变量*/
    if (dialogbox_style->type == UI_AP)
    {
        style_ptr = &g_ap_res;
    }
    else
    {
        style_ptr = &g_com_res;
    }
    get_ctrl_addr(DIALOGBOX, &dialogbox_entry_addr, style_ptr);
    sys_sd_fseek(style_ptr->res_fp, SEEK_SET, dialogbox_entry_addr + (uint32) (dialogbox_style->style_id & 0x0fff)
            * sizeof(dialogbox_t));
    sys_sd_fread(style_ptr->res_fp, &dialogbox, sizeof(dialogbox_t));
    //不显示dialogbox
    if ((dialogbox.attrib & DIALOG_SHOW_EN) != DIALOG_SHOW_EN)
    {
        return UI_NO_DISPLAY;
    }
    romf_get_DC_status(&oldDC_sav, 0, 0);
    //初始化全部刷新
    if (mode == DIALOG_DRAW_ALL)
    {
        //显示dialogbox的背景图片
        if (dialogbox.back_id != (uint16) - 1)
        {
            u_show_pic(dialogbox.back_id, dialogbox.back_x, dialogbox.back_y);
        }
        //需要显示items
        if ((dialogbox.attrib & DIALOG_SHOW_ICON_EN) == DIALOG_SHOW_ICON_EN)
        {
            //显示dialogbox的Icon
            if (dialogbox_data->icon_id != (uint16) - 1)
            {
                u_show_pic(dialogbox_data->icon_id, dialogbox.icon_x, dialogbox.icon_y);
            }
            else if (dialogbox.icon_id != (uint16) - 1)
            {
                u_show_pic(dialogbox.icon_id, dialogbox.icon_x, dialogbox.icon_y);
            }
            else
            {
                //do nothing for QAC
            }
        }
        //显示对话框的描述字符串
        show_dialogbox_desc(dialogbox, dialogbox_data);
        //需要显示items
        if ((dialogbox.attrib & DIALOG_SHOW_ITEMS_EN) == DIALOG_SHOW_ITEMS_EN)
        {
            //显示button选项
            for (i = 0; i < dialogbox_data->button_cnt; i++)
            {
                if ((i == dialogbox_data->active))
                {
                    show_dialogbox_button(dialogbox.item_addr, dialogbox_data, i, 1, style_ptr);
                }
                else
                {
                    show_dialogbox_button(dialogbox.item_addr, dialogbox_data, i, 0, style_ptr);
                }
            }
        }
    }
    //只更新选中项和上一次选中项
    else
    {
        //需要显示items
        if ((dialogbox.attrib & DIALOG_SHOW_ITEMS_EN) == DIALOG_SHOW_ITEMS_EN)
        {
            show_dialogbox_button(dialogbox.item_addr, dialogbox_data, dialogbox_data->old, 0, style_ptr);
            show_dialogbox_button(dialogbox.item_addr, dialogbox_data, dialogbox_data->active, 1, style_ptr);
        }
    }
    //恢复画笔颜色
    romf_set_pen_color(oldDC_sav.pen_color, 0, 0);
    romf_set_backgd_color(oldDC_sav.backgd_color, 0, 0);
    return UI_NO_ERR;
}
/*! \endcond */

/*! \cond */
/******************************************************************************/
/*!
 * \par  Description:
 *    显示dialogbox的描述字符串
 * \param[in]   dialogbox：dialogbox结构体变量
 * \param[in]   dialogbox_data：应用传递的dialogbox的私有数据结构体指针
 * \param[out]  none
 * \return      none
 * \note
 *******************************************************************************/
void show_dialogbox_desc(dialogbox_t dialogbox, dialog_private_t *dialogbox_data)
{
    string_desc_t str_desc;
    region_t desc_region;
    uint16 disp_mode = 0;
    uint16 height_total;
    uint16 lines_cnt;
    uint16 line_mode;

    libc_memcpy(&str_desc, dialogbox_data->string_desc, sizeof(string_desc_t));
    if (str_desc.language == UNICODEID)
    {
        uint16 str_id = str_desc.data.id;

        str_desc.data.str = scroll_buf;
        str_desc.length = STRING_BUFFER_LEN;
        str_desc.language = UNICODEDATA;
        romf_get_unicode_data(str_id, &str_desc);
    }

    //atrib<4-5>0表示小字体,1表示中号字体，2表示大字体，默认中号字体
    if ((dialogbox.attrib & DESC_FONT_OPTION) == DESC_FONT_OPTION_SMALL)
    {
        disp_mode |= DISP_FONT_SMALL;
    }
    else if ((dialogbox.attrib & DESC_FONT_OPTION) == DESC_FONT_OPTION_LARGE)
    {
        disp_mode |= DISP_FONT_LARGE;
    }
    else
    {
        disp_mode |= DISP_FONT_NORMAL; //默认中号字体
    }

    romf_set_pen_color(dialogbox.desc_color, 0, 0);

    str_desc.argv = dialogbox.desc_width;
    line_mode = FUNC_GET_TOTAL_LINES | FILTER_BLANK_DISABLE | DIVIDE_WORD_ENABLE | (disp_mode & DISP_FONT_OPTION);
    get_text_line(&str_desc, line_mode, NULL);
    lines_cnt = str_desc.result;
    height_total = fix_font_height_line * lines_cnt;
    disp_mode |= DISP_DIVIDE_WORD_ENABLE;//自动分词

    //显示区域设置
    desc_region.x = dialogbox.desc_x;
    //临时解决字符串过长的问题
    if (height_total > dialogbox.desc_height)
    {
        height_total = dialogbox.desc_height;
    }
    desc_region.y = dialogbox.desc_y + (dialogbox.desc_height - height_total) / 2;
    desc_region.width = dialogbox.desc_width;
    desc_region.height = height_total;
    if ((dialogbox.attrib & DESC_ALIGNMENT_OPTION) == DESC_ALIGNMENT_OPTION_LEFT)
    {
        disp_mode |= DISP_ALIGN_LEFT;
    }
    else if ((dialogbox.attrib & DESC_ALIGNMENT_OPTION) == DESC_ALIGNMENT_OPTION_RIGHT)
    {
        disp_mode |= DISP_ALIGN_RIGHT;
    }
    else
    {
        disp_mode |= DISP_ALIGN_MEDIACY;
    }
    disp_mode |= DISP_LONGSTR_MULTI_ROW;
    /*! 对话框描述字符串*/
    show_string(&str_desc, &desc_region, disp_mode);
}
/******************************************************************************/
/*!
 * \par  Description:
 *    显示dialogbox的items选项
 * \param[in]   addr：dialogbox的items在资源文件中的偏移地址
 * \param[in]   dialogbox_data：应用传递的dialogbox的私有数据结构体指针
 * \param[in]   itemnum：第几项
 * \param[in]   type：是否为激活项
 * \param[in]   style_ptr：当前资源信息结构体指针
 * \param[out]  none
 * \return      none
 * \note
 *******************************************************************************/
void show_dialogbox_button(uint32 addr, dialog_private_t *dialogbox_data, uint16 itemnum, uint8 type,
        res_open_infor_t *style_ptr)
{
    dialogbox_item_t dialogbox_item;
    region_t back_region, item_region;
    uint16 disp_mode = 0;
    uint8 actual_h;

    sys_sd_fseek(style_ptr->res_fp, SEEK_SET, addr);
    sys_sd_fread(style_ptr->res_fp, &dialogbox_item, sizeof(dialogbox_item_t));
    
    //atrib<5-6>0表示小字体,1表示中号字体，2表示大字体，默认中号字体
    if ((dialogbox_item.attrib & ITEMS_FONT_OPTION) == ITEMS_FONT_OPTION_SMALL)
    {
        disp_mode |= DISP_FONT_SMALL;
    }
    else if ((dialogbox_item.attrib & ITEMS_FONT_OPTION) == ITEMS_FONT_OPTION_LARGE)
    {
        disp_mode |= DISP_FONT_LARGE;
    }
    else
    {
        disp_mode |= DISP_FONT_NORMAL; //默认中号字体
    }
    actual_h = get_fontlib_height(disp_mode);
    
    //显示区域设置
    back_region.x = dialogbox_item.item_x[itemnum];
    back_region.y = dialogbox_item.item_x[itemnum];
    back_region.width = dialogbox_item.item_width[itemnum];
    back_region.height = dialogbox_item.item_height[itemnum];
    
    item_region.x = dialogbox_item.item_x[itemnum];
    item_region.y = dialogbox_item.item_y[itemnum] + ((dialogbox_item.item_height[itemnum] - actual_h)/2);
    item_region.width = dialogbox_item.item_width[itemnum];
    item_region.height = actual_h;
    //非激活项
    if (type == 0)
    {
        //图片方式（single）
        if ((dialogbox_item.attrib & ITEMS_INACTIVE_BG_OPTION) == INACTIVE_BG_OPTION_SPIC)
        {
            disp_mode = CHAR_FILL_NORMAL; //有背景图片，则是透明模式
            u_show_pic(dialogbox_item.backgd_id, dialogbox_item.item_x[itemnum], dialogbox_item.item_y[itemnum]);
        }
        //图片方式（multy）
        else if ((dialogbox_item.attrib & ITEMS_INACTIVE_BG_OPTION) == INACTIVE_BG_OPTION_MPIC)
        {
            disp_mode = CHAR_FILL_NORMAL; //有背景图片，则是透明模式
            u_show_pic(dialogbox_item.backgd_id + itemnum, dialogbox_item.item_x[itemnum],
                    dialogbox_item.item_y[itemnum]);
        }
        //背景颜色，填充为按钮效果
        else
        {
            disp_mode = CHAR_FILL_COLOR; //背景颜色，则是非透明模式,默认为非透明快速模式
            romf_set_backgd_color(dialogbox_item.backgd_id, 0, 0);
            //用画笔颜色进行区域填充
            romf_fill_rect(&back_region, dialogbox_item.backgd_id);
        }
    }
    else
    {
        if ((dialogbox_item.attrib & ITEMS_ACTIVE_BG_OPTION) == ACTIVE_BG_OPTION_SPIC)
        {
            disp_mode = CHAR_FILL_NORMAL; //有背景图片，则是透明模式
            u_show_pic(dialogbox_item.act_backgd_id, dialogbox_item.item_x[itemnum], dialogbox_item.item_y[itemnum]);
        }
        //图片方式（multy）
        else if ((dialogbox_item.attrib & ITEMS_ACTIVE_BG_OPTION) == ACTIVE_BG_OPTION_MPIC)
        {
            disp_mode = CHAR_FILL_NORMAL; //有背景图片，则是透明模式
            u_show_pic(dialogbox_item.act_backgd_id + itemnum, dialogbox_item.item_x[itemnum],
                    dialogbox_item.item_y[itemnum]);
        }
        //背景颜色，填充为按钮效果
        else
        {
            disp_mode = CHAR_FILL_COLOR; //背景颜色，则是非透明模式,默认为非透明快速模式
            romf_set_pen_color(dialogbox_item.act_backgd_id, 0, 0);
            //用画笔颜色进行区域填充
            romf_fill_rect(&back_region, dialogbox_item.act_backgd_id);
        }
    }

    if (type == 0)
    {
        romf_set_pen_color(dialogbox_item.text_color, 0, 0);
    }
    else
    {
        romf_set_pen_color(dialogbox_item.act_text_color, 0, 0);
    }
    if ((dialogbox_item.attrib & DIALOG_ALIGNMENT_OPTION) == DIALOG_ALIGNMENT_OPTION_LEFT)
    {
        disp_mode |= DISP_ALIGN_LEFT;
    }
    else if ((dialogbox_item.attrib & DIALOG_ALIGNMENT_OPTION) == DIALOG_ALIGNMENT_OPTION_RIGHT)
    {
        disp_mode |= DISP_ALIGN_RIGHT;
    }
    else
    {
        disp_mode |= DISP_ALIGN_MEDIACY;
    }
    if (dialogbox_data->buttons[itemnum] != (uint16) - 1)
    {
        string_desc_t desc;

        desc.data.id = dialogbox_data->buttons[itemnum];
        desc.language = UNICODEID;
        show_string(&desc, &item_region, disp_mode);//默认居中对齐
    }
}
/*! \endcond */
