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
 * \file     bank_b_ui_show_textbox.c
 * \brief   显示textbox的模块
 * \author   liutingting
 * \par      GENERAL DESCRIPTION:
 *               实现textbox控件数据的解析并显示textbox
 * \par      EXTERNALIZED FUNCTIONS:
 *              直接调用系统SD文件系统的接口函数解析并显示textbox
 * \version 1.0
 * \date  2011-9-05
 *******************************************************************************/
#include "display.h"
#include "ui_driver.h"

//textbox attribute bits location
#define TEXTBOX_SHOW_EN                 (0x0001 << 0)
#define TEXTBOX_BG_OPTION_SHIFT         (1)
#define TEXTBOX_BG_OPTION               (0x0001 << TEXTBOX_BG_OPTION_SHIFT)
#define BG_OPTION_COLOR                 (0x0000 << TEXTBOX_BG_OPTION_SHIFT)
#define BG_OPTION_PIC                   (0x0001 << TEXTBOX_BG_OPTION_SHIFT)
#define TEXTBOX_FONT_OPTION_SHIFT       (2)
#define TEXTBOX_FONT_OPTION             (0x0003 << TEXTBOX_FONT_OPTION_SHIFT)
#define FONT_OPTION_SMALL               (0x0000 << TEXTBOX_FONT_OPTION_SHIFT)
#define FONT_OPTION_MIDDLE              (0x0001 << TEXTBOX_FONT_OPTION_SHIFT)
#define FONT_OPTION_LARGE               (0x0002<< TEXTBOX_FONT_OPTION_SHIFT)
#define TEXTBOX_ARABIC_CANCEL_EN        (0x0001 << 4)
#define TEXTBOX_DISP_OPTION_SHIFT       (5)
#define TEXTBOX_DISP_OPTION             (0x0003 << TEXTBOX_DISP_OPTION_SHIFT)
#define DISP_OPTION_MLINE               (0x0000 << TEXTBOX_DISP_OPTION_SHIFT)
#define DISP_OPTION_SCROLL              (0x0001 << TEXTBOX_DISP_OPTION_SHIFT)
#define DISP_OPTION_NORECT              (0x0002 << TEXTBOX_DISP_OPTION_SHIFT)
#define DISP_OPTION_ELLIPSIS            (0x0003 << TEXTBOX_DISP_OPTION_SHIFT)
#define TEXTBOX_ALIGNMENT_OPTION_SHIFT  (7)
#define TEXTBOX_ALIGNMENT_OPTION        (0x0003 << TEXTBOX_ALIGNMENT_OPTION_SHIFT)
#define ALIGNMENT_OPTION_LEFT           (0x0000 << TEXTBOX_ALIGNMENT_OPTION_SHIFT)
#define ALIGNMENT_OPTION_CENTER         (0x0001 << TEXTBOX_ALIGNMENT_OPTION_SHIFT)
#define ALIGNMENT_OPTION_RIGHT          (0x0002 << TEXTBOX_ALIGNMENT_OPTION_SHIFT)

uint16 check_display_mode(uint16 attrib);

/*! \cond UI_DRIVER_API */
/******************************************************************************/
/*!
 * \par  Description:
 *    显示textbox
 * \param[in]    textbox_style 指向textbox的style结构体变量
 * \param[in]    textbox_data 应用传递的textbox的私有数据结构体指针
 * \param[in]    mode 显示textbox的模式
 * \param[out]   none
 * \return       ui_result_e 类型的参数，参见ui_result_e定义
 * \retval       UI_NO_ERR 正常显示textbox
 * \retval       UI_NEED_SCROLL 该textbox的字符串需要滚屏显示
 * \retval       other 显示异常
 * \ingroup      ui_controls
 * \par          exmaple code
 * \code
    例子1：显示用户指定编码串，可以是内码，也可以是Unicode码
    style_infor_t textbox_style;
    textbox_private_t textbox_param;
    
    textbox_style.style_id = TEXTBOX_ID1;
    textbox_style.type = UI_AP;
    
    textbox_param.str_id = -1;
    textbox_param.lang_id = g_comval.lang_id;
    textbox_param.str_value = "text";
    ui_show_textbox(&textbox_style, &textbox_param, TEXTBOX_DRAW_NORMAL);
    
    例子2：显示用户指定多国语言字符串
    style_infor_t textbox_style;
    textbox_private_t textbox_param;
    
    textbox_style.style_id = TEXTBOX_ID1;
    textbox_style.type = UI_AP;
    
    textbox_param.str_id = S_STRING_ID1;
    textbox_param.lang_id = UNICODEID;
    textbox_param.str_value = NULL;
    ui_show_textbox(&textbox_style, &textbox_param, TEXTBOX_DRAW_NORMAL);
    
    例子3：显示textbox中选择的多国语言字符串
    style_infor_t textbox_style;
    
    textbox_style.style_id = TEXTBOX_ID1;
    textbox_style.type = UI_AP;
    
    ui_show_textbox(&textbox_style, NULL, TEXTBOX_DRAW_NORMAL);
    
    例子4：滚屏显示字符串，该textbox的长字符串模式必须设置为“滚屏”
    初始化显示代码如下：
    style_infor_t textbox_style;
    textbox_private_t textbox_param;
    ui_result_e result;
    
    textbox_style.style_id = TEXTBOX_ID1;
    textbox_style.type = UI_AP;
    
    textbox_param.str_id = -1;
    textbox_param.lang_id = UNICODEDATA;
    textbox_param.str_value = g_filename;
    result = ui_show_textbox(&textbox_style, &textbox_param, TEXTBOX_DRAW_NORMAL);
    if(result == UI_NEED_SCROLL)
    {
        启动滚屏定时器
    }
    
    字符串滚屏显示，放在应用级定时器ISR中：
    style_infor_t textbox_style;
    
    textbox_style.style_id = TEXTBOX_ID1;
    textbox_style.type = UI_AP;
    
    ui_show_textbox(&textbox_style, NULL, TEXTBOX_DRAW_SCROLL);
 * \endcode
 * \note
 * \li  若textbox_data->str_value != NULL，则优先显示str_value指向的内容，此时，必须指定data->lang_id为内码还是unicode。
 * \li  textbox_data->str_value == NULL,若textbox_data->str_id！=-1，则优先显示str_id所指向的字符串。
 * \li  若textbox_data->str_id == -1,则显示TextBox中id指定的字符串。
 * \li  若字符串需要滚屏，返回UI_NEED_SCROLL。
 *******************************************************************************/
ui_result_e show_textbox(style_infor_t *textbox_style, textbox_private_t *textbox_data, textbox_draw_mode_e mode)
{
    textbox_t textbox;
    region_t kregion;
    DC oldDC_sav;
    res_open_infor_t *style_ptr;
    uint32 textbox_entry_addr;
    uint8 *str = NULL;
    uint16 res_id;
    uint16 disp_mode = 0;
    uint16 textout_mode = 0;
    ui_result_e result;
    bool ret_val;
    int8 language;
    string_desc_t desc;
    scroll_mode_t scroll_mode;
    
    romf_get_DC_status(&oldDC_sav, 0, 0);
    
    if ((mode == TEXTBOX_DRAW_SCROLL) || (mode == TEXTBOX_DRAW_ITEM_SCROLL))
    {
        scroll_mode.mode = FALSE;
        ret_val = scroll_string(NULL, NULL, scroll_mode);
        romf_set_pen_color(oldDC_sav.pen_color, 0, 0);
        romf_set_backgd_color(oldDC_sav.backgd_color, 0, 0);
        if(ret_val == TRUE)
        {
            return UI_NEED_SCROLL;
        }
        else
        {
            return UI_SCROLL_OVER;
        }
    }

    /*根据打开资源文件的类型，将指针指向不同的全局变量*/
    if (textbox_style->type == UI_AP)
    {
        style_ptr = &g_ap_res;
    }
    else
    {
        style_ptr = &g_com_res;
    }
    //获取AP传递的私有数据
    if (textbox_data == NULL)
    {
        language = UNICODEID;
        res_id = (uint16) - 1;
        str = NULL;
    }
    else
    {
        language = textbox_data->lang_id;
        str = textbox_data->str_value;
        res_id = textbox_data->str_id;
    }
    get_ctrl_addr(TEXTBOX, &textbox_entry_addr, style_ptr);
    //读取当前textbox的style信息
    sys_sd_fseek(style_ptr->res_fp, SEEK_SET, textbox_entry_addr + (uint32) (textbox_style->style_id & 0x0fff)
            * sizeof(textbox_t));
    sys_sd_fread(style_ptr->res_fp, &textbox, sizeof(textbox_t));
    //不显示textbox
    if ((textbox.attrib & TEXTBOX_SHOW_EN) != TEXTBOX_SHOW_EN)
    {
        return UI_NO_DISPLAY;
    }
    //保存字符串显示区域
    kregion.x = textbox.x;
    kregion.y = textbox.y;
    kregion.width = textbox.width;
    kregion.height = textbox.height;
    
    if((textbox_data != NULL) && ((textbox_data->color & 0xffff0000) == 0x5a5a0000))//用户自定义颜色
    {
        romf_set_pen_color(textbox_data->color & 0x0000ffff, 0, 0);//低16位为有效颜色值
    }
    else
    {
        romf_set_pen_color(textbox.text_color, 0, 0);
    }
    if ((textbox.attrib & TEXTBOX_BG_OPTION) == BG_OPTION_PIC)
    {
        textout_mode = CHAR_FILL_NORMAL; //有背景图片，则是透明模式
    }
    else
    {
        textout_mode = CHAR_FILL_COLOR; //背景颜色，则是非透明模式,默认为非透明快速模式
        romf_set_backgd_color(textbox.back_id, 0, 0);
    }

    //将textbox的属性转换成对应的显示模式
    disp_mode = check_display_mode(textbox.attrib);
    disp_mode |= textout_mode;
    
    //仅仅加载并初始化背景图片，已经初始化全局变量
    if (mode == TEXTBOX_DRAW_ID3_VERTICAL)
    {
        if(textout_mode != CHAR_FILL_COLOR)//透明模式，有背景图片
        {
            //加载并初始化背景图片
            load_bgpic_sd(textbox.back_id, textbox.back_x, textbox.back_y, 0);
            disp_mode &= ~DISP_CHAR_FILL_OPTION;
            disp_mode |= CHAR_FILL_SDIMAGE;
        }
        libc_memcpy(&(scroll_desc.scroll_region), &kregion, sizeof(region_t));
        scroll_desc.disp_mode = disp_mode;
        scroll_desc.pen_color = textbox.text_color;
        scrollhead_height = 0;//初始垂直ID3滚屏全局变量
        return UI_NEED_SCROLL;
    }
    //ID3水平滚屏加载，无须显示
    if (mode == TEXTBOX_DRAW_ID3_HORIZONTAL)
    {
        disp_mode |= DISP_DISPLAY_DISABLE;
    }
    else
    {
        //清除背景
        if (textout_mode == CHAR_FILL_NORMAL)
        {
            if (textbox.back_id != (uint16) - 1)
            {
                u_show_pic(textbox.back_id, textbox.back_x, textbox.back_y);
            }
        }
        else
        {
            //clear_screen(&kregion, 0, 0);
            romf_fill_rect(&kregion, (uint16) (dc.backgd_color & 0xffff));
        }
    }
    
    //优先显示用户指定的字符串
    if (str != NULL)
    {
        if (mode == TEXTBOX_DRAW_ITEM)//标题+内容模式
        {
            uint16 title_length;
            
            //先显示标题 res_id
            desc.length = -1;
            desc.language = UNICODEID;
            desc.data.id = res_id;
            title_length = get_string_length_id(res_id, disp_mode);
            show_string(&desc, &kregion, disp_mode);

            //再紧接着显示内容 str
            desc.data.str = str;
            desc.length = -1;
            if(language == UTF_8DATA)
            {
                desc.length = (uint16)libc_strlen(str);
            }
            desc.language = language;
            if (need_arabic_deal(disp_mode) == TRUE)
            {
                kregion.x = textbox.x;
                kregion.width = textbox.width - title_length;
            }
            else
            {
                kregion.x = textbox.x + title_length;
                kregion.width = textbox.width - title_length;
            }
            kregion.y = textbox.y;
            kregion.height = textbox.height;
            result = (ui_result_e) show_string(&desc, &kregion, disp_mode);
        }
        else//其他模式 TEXTBOX_DRAW_NORMAL TEXTBOX_DRAW_NOSCROLL
        {
            desc.data.str = str;
            desc.length = -1;
            if(language == UTF_8DATA)
            {
                desc.length = (uint16)libc_strlen(str);
            }
            desc.language = language;
            result = (ui_result_e) show_string(&desc, &kregion, disp_mode);
        }
    }
    //其次显示用户指定的多国语言字符串
    else if (res_id != (uint16) - 1)
    {
        desc.data.id = res_id;
        desc.language = UNICODEID;
        result = (ui_result_e) show_string(&desc, &kregion, disp_mode);
    }
    //显示textbox中指定的多国语言字符串
    else if (textbox.id != (uint16) - 1)
    {
        desc.data.id = textbox.id;
        desc.language = UNICODEID;
        result = (ui_result_e) show_string(&desc, &kregion, disp_mode);
    }
    else//这种情况可用来清除字符串
    {
        if (textbox.back_id != (uint16) - 1)
        {
            u_show_pic(textbox.back_id, textbox.back_x, textbox.back_y);
        }
        result = UI_PARAM_ERR;
    }
    //恢复画笔颜色
    romf_set_pen_color(oldDC_sav.pen_color, 0, 0);
    romf_set_backgd_color(oldDC_sav.backgd_color, 0, 0);
    disp_mode &= ~DISP_DISPLAY_DISABLE;
    
    //初始滚屏环境 TEXTBOX_DRAW_ITEM TEXTBOX_DRAW_NORMAL
    if ((result == UI_NEED_SCROLL) && (mode != TEXTBOX_DRAW_NOSCROLL))
    {
        if(textout_mode != CHAR_FILL_COLOR)//透明模式，有背景图片
        {
            //加载并初始化背景图片
            load_bgpic_sd(textbox.back_id, textbox.back_x, textbox.back_y, 1);
            disp_mode &= ~DISP_CHAR_FILL_OPTION;
            disp_mode |= CHAR_FILL_SDIMAGE;
        }
        
        //初始化字符串滚屏环境，包括给字符串末尾添加2空格
        scroll_mode.mode = TRUE;
        scroll_mode.disp_mode = disp_mode;
        scroll_desc.back_id = textbox.back_id;//仅用于记录背景颜色
        scroll_desc.back_x = textbox.back_x;//暂时没用
        scroll_desc.back_y = textbox.back_y;//暂时没用
        scroll_desc.pen_color = textbox.text_color;
        scroll_string(&scroll_str, &kregion, scroll_mode);
    }
    
    return result;
}
/*! \endcond */

/*! \cond */
/******************************************************************************/
/*!
 * \par  Description:
 *    根据textbox的属性判断textbox的显示模式
 * \param[in]    attrib：要显示的textbox的attrib数据信息
 * \param[out]   none
 * \return       uint16类型的参数，tetxbox的显示模式
 * \ingroup      ui driver
 * \note
 *******************************************************************************/
uint16 check_display_mode(uint16 attrib)
{
    uint16 disp_mode = 0;

    //长字符串显示模式
    if ((attrib & TEXTBOX_DISP_OPTION) == DISP_OPTION_MLINE)
    {
        disp_mode |= DISP_LONGSTR_MULTI_ROW;
    }
    else if ((attrib & TEXTBOX_DISP_OPTION) == DISP_OPTION_SCROLL)
    {
        disp_mode |= DISP_LONGSTR_SCROLL;
    }
    else if ((attrib & TEXTBOX_DISP_OPTION) == DISP_OPTION_NORECT)
    {
        disp_mode |= DISP_LONGSTR_NO_RECT;
    }
    else if ((attrib & TEXTBOX_DISP_OPTION) == DISP_OPTION_ELLIPSIS)
    {
        disp_mode |= DISP_LONGSTR_ELLIPSIS;
    }
    else
    {
        disp_mode |= DISP_LONGSTR_RECT;//默认显示不完整字符
    }

    //对齐模式
    if ((attrib & TEXTBOX_ALIGNMENT_OPTION) == ALIGNMENT_OPTION_CENTER)
    {
        disp_mode |= DISP_ALIGN_MEDIACY;
    }
    else if ((attrib & TEXTBOX_ALIGNMENT_OPTION) == ALIGNMENT_OPTION_RIGHT)
    {
        disp_mode |= DISP_ALIGN_RIGHT;
    }
    else
    {
        disp_mode |= DISP_ALIGN_LEFT;//默认左对齐
    }

    //atrib<2-3>0表示小字体,1表示中号字体，2表示大字体，默认中号字体
    if ((attrib & TEXTBOX_FONT_OPTION) == FONT_OPTION_SMALL)
    {
        disp_mode |= DISP_FONT_SMALL;
    }
    else if ((attrib & TEXTBOX_FONT_OPTION) == FONT_OPTION_LARGE)
    {
        disp_mode |= DISP_FONT_LARGE;
    }
    else
    {
        disp_mode |= DISP_FONT_NORMAL; //默认中号字体
    }

    //取消阿拉伯右对齐
    if ((attrib & TEXTBOX_ARABIC_CANCEL_EN) == TEXTBOX_ARABIC_CANCEL_EN)
    {
        disp_mode |= DISP_ARABIC_DISABLE;
    }

    return disp_mode;
}
/*! \endcond */

/*! \cond */

/******************************************************************************/
/*!
 * \par  Description:
 *    获取多国语言字符串资源的字符串长度
 * \param[in]   str_id 多国语言字符串资源ID
 * \param[in]   disp_mode 字符串显示模式
 * \param[out]  none
 * \return      uint16 字符串长度
 * \note
 *******************************************************************************/
uint16 get_string_length_id(uint16 str_id, uint16 disp_mode)
{
    string_desc_t string_infor;
    uint16 str_len;
    
#if (USE_MULTI_FONTLIB != 0)
    //切换字体字库
    fix_switch_fontlib(disp_mode);
#endif

    libc_memset(string_buf, 0, STRING_BUFFER_LEN);
    
    string_infor.data.str = string_buf;
    string_infor.length = STRING_BUFFER_LEN;
    string_infor.language = UNICODEDATA;
    romf_get_unicode_data(str_id, &string_infor);
    
    //阿拉伯和希伯来语特殊处理
    if(need_arabic_deal(0) == TRUE)
    {
        //阿拉伯和希伯莱需要进行 UNICODE 连写处理
        string_infor.argv = STRING_BUFFER_LEN;
        arabic_uni_join(&string_infor);
    }
    
    string_infor.language = UNICODELANGUAGE;

#if (USE_MULTI_FONTLIB == 0)    
    str_len = romf_get_string_length(&string_infor, 128, NULL);
#else
    str_len = fixf_get_string_length(&string_infor, 128, NULL);
#endif

    return str_len;
}

/*! \endcond */
