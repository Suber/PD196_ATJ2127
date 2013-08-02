/*******************************************************************************
 *                              US212A
 *                            Module: ui driver
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>           <time>          <version >             <desc>
 *       liutingting     2011-09-20 10:00     1.0             build this file
 *******************************************************************************/
/*!
 * \file     ui_op_entry.c
 * \brief    ui驱动总接口定义
 * \author   liutingting
 * \par      GENERAL DESCRIPTION:
 *              定义ui驱动接口数组
 * \par      EXTERNALIZED FUNCTIONS:
 none
 * \version  1.0
 * \date  2011/9/20
 *******************************************************************************/
#include "display.h"
#include "ui_driver.h"

//固化代码修复
#if (USE_MULTI_FONTLIB != 0)
uint8 fix_char_point_buffer[MAX_CHARPOINTE_SIZE];//字模缓冲区
sd_file_t fix_font_files[3];//字体字库文件描述符数组
uint8 fix_current_font;//0表示小字体字库，1表示中字体字库，2表示大字体字库
#endif
uint8 *fix_p_char_point;
uint8 fix_font_height;
uint8 fix_font_height_actual;
uint8 fix_font_height_line;//行高度，行间隔为1/4行
uint8 fix_font_height_up_margin;
uint8 fix_font_width;
uint8 fix_font_size;

//ui驱动内部字符串缓冲区
uint8 string_buf[STRING_BUFFER_LEN];

//滚屏相关变量
uint8 scroll_buf[STRING_BUFFER_LEN];
scroll_desc_t scroll_desc;
string_desc_t scroll_str;
text_background_t text_bg_desc;
bool  scroll_flag; //字符串是否超长
uint8 scrollhead_height; //滚屏的高度

//阿拉伯和希伯莱语相关变量
uint8 prev_valid_next_joining; //保存arabic字符连写变量
arabic_status_e arabic_stat;//阿拉伯和希伯莱语编码流状态
#ifndef PC
uint8 *arabic_string_buf;//该空间动态申请
#else
uint8 arabic_string_buf[STRING_BUFFER_LEN];
#endif

listbox_item_t list_item; //保存listbox item的控件数据结构
uint16 timebox_sty_id = 0; //保存上一次的timebox的style id
uint16 prev_num[2] = {0, 0}; //记录progress上一次进度
int16 slider_prev_offset = 0;//保存parambox的slider 上一次刻度值
uint16 prev_pixel = 0;   //保存listbox slider上一次的刻度值
uint8 ratio_bits = 0;   //保存ratio的数字位数
uint8 num_total_bits = 0; //保存numbox的数字位数总数
uint8 numbox_disp_prev[12];//保存numbox的上一次显示内容
uint8 param_bits = 0;    //保存parambox的数字位数总数
uint8 time_bits = 0; //保存上一次timebox显示的数字总数

uint8 lcd_buffer_type; //显示buffer类型
uint8 language_id; //当前语言类型
bool  language_2byte;//是否双字节（内码）语言，比如中日韩语
bool  rotate_flag;//屏幕旋转为横屏
uint16 lang_option;//特殊语言选项

typedef void* (*ui_op_func)(void*, void*, void*);
typedef struct
{
    ui_op_func res_open;
    ui_op_func res_close;
    ui_op_func show_picbox;
    ui_op_func show_textbox;
    ui_op_func show_timebox;
    ui_op_func show_numbox;
    ui_op_func show_progressbar;
    ui_op_func load_attributebox;
    ui_op_func show_listbox;
    ui_op_func show_dialogbox;
    ui_op_func show_parambox;
    ui_op_func show_sliderbar;
    ui_op_func get_textbox_attrb;
    ui_op_func get_picbox_attrb;
    ui_op_func get_listbox_attrb;
    ui_op_func scroll_string_ext;
    ui_op_func set_language;
    ui_op_func get_DC_status;
    ui_op_func clear_screen;
    ui_op_func set_pen_color;
    ui_op_func set_backgd_color;
    ui_op_func show_string;
    ui_op_func ellipsis_to_longstr;
    ui_op_func get_string_length;
    ui_op_func get_text_line;
    ui_op_func set_display_buf;
    ui_op_func unicode_to_char;
    ui_op_func char_to_unicode;
    ui_op_func get_unicode_data;
    ui_op_func utf8_to_unicode;
    ui_op_func set_screen_direction;
    ui_op_func show_pic;
    ui_op_func read_pic_to_buffer;
    ui_op_func flush_font_buffer;
    ui_op_func set_lang_option;
} ui_driver_operations;

ui_driver_operations ui_driver_op =
{ 
    (ui_op_func) res_open, (ui_op_func) res_close, (ui_op_func) show_picbox, 
    (ui_op_func) show_textbox, (ui_op_func) show_timebox, (ui_op_func) show_numbox, 
    (ui_op_func) show_progressbar, (ui_op_func) load_attributebox, (ui_op_func) show_listbox, 
    (ui_op_func) show_dialogbox, (ui_op_func) show_parambox, (ui_op_func) show_sliderbar, 
    (ui_op_func) get_textbox_attrb, (ui_op_func) get_picbox_attrb, (ui_op_func) get_listbox_attrb, 
    (ui_op_func) scroll_string_ext, (ui_op_func) set_language, (ui_op_func) romf_get_DC_status, 
    (ui_op_func) clear_screen, (ui_op_func) romf_set_pen_color, (ui_op_func) romf_set_backgd_color, 
    (ui_op_func) show_string, (ui_op_func) ellipsis_to_longstr, (ui_op_func) romf_get_string_length, 
    (ui_op_func) get_text_line, (ui_op_func) set_display_buf, (ui_op_func) unicode_to_char, 
    (ui_op_func) char_to_unicode, (ui_op_func) romf_get_unicode_data, (ui_op_func) utf8_to_unicode, 
    (ui_op_func) set_screen_direction, (ui_op_func) romf_show_pic, (ui_op_func)read_pic_to_buffer, 
    (ui_op_func) flush_font_buffer,(ui_op_func) set_lang_option,
};
