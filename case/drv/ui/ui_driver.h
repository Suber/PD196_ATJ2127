/*******************************************************************************
 *                              us211A
 *                            Module: ui驱动
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       liutingting     2011-9-9 16:23     1.0             build this file
 *******************************************************************************/
/*!
 * \file     ui_driver.h
 * \brief    ui驱动模块相关数据结构，宏定义，接口声明等
 * \author   liutinging
 * \version 1.0
 * \date  2011-9-9
 *******************************************************************************/

#ifndef _UI_DRIVER_H
#define _UI_DRIVER_H

#include "psp_includes.h"
#include "ui_driver_rom.h"
#include "display.h"

//固化代码修复
#if (USE_MULTI_FONTLIB != 0)
extern uint8 fix_char_point_buffer[MAX_CHARPOINTE_SIZE];//字模缓冲区
extern sd_file_t fix_font_files[3];//字体字库文件描述符数组
extern uint8 fix_current_font;//0表示小字体字库，1表示中字体字库，2表示大字体字库
#endif
extern uint8 *fix_p_char_point;
extern uint8 fix_font_height;
extern uint8 fix_font_height_actual;
extern uint8 fix_font_height_line;
extern uint8 fix_font_height_up_margin;
extern uint8 fix_font_width;
extern uint8 fix_font_size;

#ifndef PC
#define fix_font_cache_char_addr(n) ((uint8 *)font_cache_addr + font_cache_header + fix_font_size * (n))
#else
#define fix_font_cache_char_addr(n) (GET_REAL_ADDR(font_cache_addr) + font_cache_header + fix_font_size * (n))
#endif

//获取字体高度
static inline uint8 get_fontlib_height(uint16 disp_mode)
{
#if (USE_MULTI_FONTLIB == 0)

    return 16;

#else

#if ((USE_MULTI_FONTLIB & 0x01) != 0)
    if((disp_mode & DISP_FONT_OPTION) == DISP_FONT_SMALL)
    {
        return FONTLIB_1_HEIGHT_ACTUAL;
    }
#endif

#if ((USE_MULTI_FONTLIB & 0x04) != 0)
    if((disp_mode & DISP_FONT_OPTION) == DISP_FONT_LARGE)
    {
        return FONTLIB_3_HEIGHT_ACTUAL;
    }
#endif

    return FONTLIB_2_HEIGHT_ACTUAL;

#endif
}

#define  UI_DEBUG //ui 驱动调试模式

/*!
 * \brief
 *  控件的序号定义
 */
#define  SCENE           0
#define  PICBOX          1
#define  TEXTBOX         2
#define  NUMBOX          3
#define  TIMEBOX         4
#define  PROGRESSBAR     5
#define  LISTBOX         6
#define  PARAMBOX        7
#define  DIALOGBOX       8
#define  SLIDERBAR       9
#define  ATTRIBUTEBOX    10

/*!
 * \brief
 *  transparent_pic_t 描述透明图标/清除图标的数据结构
 */
typedef struct
{
    /*! 透明图标ID */
    uint16 pic_id;
    /*! 透明图标X坐标 */
    uint16 pic_x;
    /*! 透明图标Y坐标 */
    uint16 pic_y;
    /*! 透明图标背景颜色 */
    uint16 back_color;
    
    /*! （实时）背景图ID */
    uint16 back_id;
    /*! （实时）背景图X坐标，如果是横向图标，表示X起始值 */
    uint16 back_x;
    /*! （实时）背景图Y坐标，如果是竖向图标，表示Y起始值（最底部值） */
    uint16 back_y;
    
    /*! 是否仅仅清除痕迹 */
    bool clear;
    /*! 背景图方向，0为横向，1为竖向 */
    uint8 direct;
} transparent_pic_t;

/*!
 * \brief
 *  picbox_t 描述picbox的数据结构
 */
typedef struct
{
    /*! 如果id=-1，表示多帧pic,帧的列表在frame中，其他值表示res id */
    uint16 id;
    /*! picbox x坐标*/
    uint16 x;
    /*! picbox y坐标*/
    uint16 y;
    /*!
     * \li attrib<0>表示是否显示picbox，0为不显示，1为显示;
     * \li attrib<1>表示frame 的存储类型，0表示多帧（12帧以内）图片均存储在frame；
     *     1表示存储在frame[0],frame[1]指定的地方;
     * \li attrib<8-15>表示图片的帧数。
     */
    uint16 attrib;
    /*! 如果attrib<1>=0，多帧时，帧对应id列表;
     *   如果attrib<1>=1，frame[0],frame[1]为frame 的开始地址
     */
    uint16 frame[12];
} picbox_t;

/*!
 * \brief
 *  textbox_t 描述textbox的数据结构
 */
typedef struct
{
    /*! 固定字符串res id ，如果为不固定 id =-1 */
    uint16 id;
    /*! 显示字符x坐标 */
    uint16 x;
    /*! 显示字符y坐标 */
    uint16 y;
    /*! 显示字符的宽度 */
    uint16 width;
    /*! 显示字符的高度 */
    uint16 height;
    /*! 背景图片x坐标 */
    uint16 back_x;
    /*! 背景图片y坐标 */
    uint16 back_y;
    /*! 当attrib<1>=0时，表示背景图片res id，此时，若为-1，表示没有背景图片;
     当attrib<1>=1时，backgroundID表示背景颜色*/
    uint16 back_id;
    /*! 字体颜色 */
    uint16 text_color;
    /*!
     * \li  attrib<0>表示字符串是否显示。0表示不显示,1表示显示;
     * \li  attrib<1>表示backgroundID是背景图片,还是背景颜色。0表示背景颜色，1表示背景图片;
     * \li  attrib<2-3>表示字体大小，0表示小字体，1表示中号字体，2表示大字体，默认为中号字体;
     * \li  attrib<4>表示是否取消阿拉伯语右对齐属性，1表示取消，0表示不取消;
     * \li  attrib<5-6>表示长字符串显示模式，0表示多行显示（自动分行显示），1表示滚屏显示，
     *                 2表示截断显示（丢弃不完整字符）3表示省略号显示;
     * \li  attrib<7-8>表示字符串对齐属性，0表示左对齐，1表示居中对齐，2表示居右对齐，默认左对齐;
     * \li  attrib<9-15>为保留字段。
     */
    uint16 attrib;
} textbox_t;

/*!
 * \brief
 *  numbox_t 描述numbox的数据结构
 */
typedef struct
{
    /*! 数字显示区域X坐标 */
    uint16 x;
    /*! 数字显示区域Y坐标 */
    uint16 y;
    /*! 数字显示区域宽度 */
    uint16 width;
    /*! 数字显示区域高度 */
    uint16 height;
    /*! 数字0的res id，0-9 的res id 必须递增 */
    uint16 num0_id;
    /*! 分隔符id；-1 表示没有分隔符 */
    uint16 partition_id;
    /*! 数字图片宽度 */
    uint8 pic_width;
    /*! 分隔符图片宽度 */
    uint8 partition_width;
    /*! 数字图片背景（透明）颜色 */
    uint16 numpic_back_color;
    /*! 背景图片x坐标 */
    uint16 back_x;
    /*! 背景图片y坐标 */
    uint16 back_y;
    /*! 数字和分隔符的背景图片ID或者背景颜色值 */
    uint16 back_id;
    /*! 数字和分隔符（字符串）的画笔颜色 */
    uint16 text_color;
    /*!
     * \li  position<0-7> 数字显示位数[2]，前4位表示小数或者第一个
     *      数，后4位表示第二个数（仅在attrib<4>=1有效），为0则表
     *      示前置0隐藏，不为0则表示显示位数;
     * \li  position<8-15> 表示小数位数。
     */
    uint16 position;
    /*!
     * \li  attrib<0>表示是否显示numberbox，1表示显示，0表示不显示;
     * \li  attrib<1>表示是否显示+号，1表示显示，0表示不显示;
     * \li  attrib<2-3>表示显示对齐方式，0表示左对齐，1表示居中，2表示右对齐；3无效;
     * \li  attrib<4>表示组成模式，0表示只含一个整数或小数的numbox，1表示两个数字组成的numbox;
     * \li  attrib<5>表示显示模式，0表示字符串显示（分隔符，小数点默认为.，两个数字默认为/），1表示数字图片显示;
     * \li  attrib<6>表示背景模式，0表示背景颜色，1表示背景图片;
     * \li  attrib<7-8>表示字体字号，0表示小号字体，1表示中号字体，2表示大号字体，默认中号字体;
     * \li  attrib<9-15> 为保留字段。
     */
    uint16 attrib; //attrib<0>表示是否显示numberbox，1表示显示，0表示不显示；
} numbox_t;

/*!
 * \brief
 *  show_number_param_t 以数字图片方式显示数字参数传递
 */
typedef struct
{
    /*! 数字和分隔符的背景图片ID */
    uint16 back_id;
    /*! 数字显示区域X坐标 */
    uint16 x;
    /*! 数字显示区域Y坐标 */
    uint16 y;
    /*! 数字显示区域宽度 */
    uint16 width;
    /*! 数字0的res id，0-9 的res id 必须递增 */
    uint16 num0_id;
    /*! 分隔符id；-1 表示没有分隔符 */
    uint16 partition_id;
    /*! 数字图片背景（透明）颜色 */
    uint16 numpic_back_color;
    /*! 数字图片宽度 */
    uint8 num_width;
    /*! 分隔符图片宽度 */
    uint8 partition_width;
    /*! 对齐方式，0:left, 1:center, 2:right */
    uint8 align;
    /*! 分隔符位置 */
    uint8 part_pos;
} show_number_param_t;

/*!
 * \brief
 *  timebox_t 描述timebox的数据结构
 */
typedef struct
{
    /*! 时、分、秒、毫秒对应的X坐标 */
    uint16 x;
    /*! 时、分、秒、毫秒对应的Y坐标 */
    uint16 y;
    /*! 时间显示区域宽度 */
    uint16 width;
    /*! 时间显示区域高度 */
    uint16 height;
    /*! 时、分、秒、毫秒对应的数字 0 的id号，要求0-9的res id递增 */
    uint16 num0_id;
    /*! 时、分、秒、毫秒的分隔符resID，-1表示没有 */
    uint16 part_id[4];
    /*! 分隔符的宽度 */
    uint8 part_width;
    /*! 时、分、秒、毫秒资源图片的宽度*/
    uint8 pic_width;
    /*! 分别是第一个time，第一个time与第二个time，第二个time的分隔符类型ID号，ID有效值为0~3 */
    uint8 part_type[3];
    /*! 保留字节 */
    uint8 reserve;
    /*! 数字图片背景（透明）颜色 */
    uint16 numpic_back_color;
    /*! 背景图片x坐标 */
    uint16 back_x;
    /*! 背景图片y坐标 */
    uint16 back_y;
    /*! 数字和分隔符的背景图片ID或者背景颜色值 */
    uint16 back_id;
    /*! 数字和分隔符（字符串）的画笔颜色 */
    uint16 text_color;
    /*!
     * \li  attrib<0> 表示是否显示TimeBox，0为不显示，1为显示;
     * \li  attrib<1> 表示显示模式，0表示字符串显示，1表示数字图片显示;
     * \li  attrib<2-3>表示显示对齐属性，0表示左对齐，1表示居中对齐，2表示居右对齐;
     * \li  attrib<4> 表示time数目，0表示1个，1表示两个;
     * \li  attrib<5> 第一个time最高位0自动隐藏，0为隐藏，1为显示;
     * \li  attrib<6> 第二个time最高位0自动隐藏，0为隐藏，1为显示;
     * \li  attrib<7-9>第一个time第一个数字显示的位数;
     * \li  attrib<10-12>第一个time第一个数字显示的位数;
     * \li  attrib<13> 表示背景模式，0表示背景颜色，1表示背景图片;
     * \li  attrib<14-15> 表示字体字号，0表示小号，1表示中号，2表示大号，默认为中号字体;
     */
    uint16 attrib;
} timebox_t;

/*!
 * \brief
 *  progressbar_t 描述progressbar的数据结构
 */
typedef struct
{
    /*! 进度条背景图片，res id */
    uint16 back_id;
    /*! 进度条背景图片的X坐标 */
    uint16 back_x;
    /*! 进度条背景图片的Y坐标 */
    uint16 back_y;
    /*! 控件背景图片，用于清除进度游标 */
    uint16 widget_bg;
    /*! 进度填充图片，长度为1个像素点的res id，其他长度为2，4，8，16，32，等递增即可 */
    uint16 fill_id;
    /*! 进度擦除图片，长度为1个像素点的res id，其他长度为2，4，8，16，32，等递增即可 */
    uint16 unfill_id;
    /*! 竖向进度条有效：进度填充X坐标 */
    uint16 fill_x;
    /*! 横向进度条有效：进度填充Y坐标 */
    uint16 fill_y;
    /*! 游标图片ID */
    uint16 tag_id;
    /*! 清除游标的游标背景图片ID */
    uint16 tag_clear_id;
    /*! 竖向进度条有效：游标图片的X坐标 */
    uint16 tag_x;
    /*! 横向进度条有效：游标图片的Y坐标 */
    uint16 tag_y;
    /*! 横向进度条有效：进度条进度刷新起始的X坐标 */
    uint16 prog_start_x;
    /*! 竖向进度条有效：进度条进度刷新起始的Y坐标 */
    uint16 prog_start_y;
    /*! 进度条的长度 */
    uint16 length;
    /*! 进度条的每格宽度（步长） */
    uint16 scale;
    /*!
     * \li  attrib<0> 表示是否显示ProgressBar，0 表示不显示，1表示显示;
     * \li  attrib<1> 表示方向，0表示横向，1表示竖向;
     * \li  attrib<2-3> 表示是否进度条风格，0表示仅进度填充；1表示仅游标标示，2表示两者都要;
     * \li  attrib<4-15> 为保留字段。
     */
    uint16 attrib;
    /*! 保留字节 */
    uint16 reserve;
} progressbar_t;

/*!
 * \brief
 *  listbox_t 描述listbox的数据结构
 */
typedef struct
{
    /*! ListBox标题结构体的地址，相对于sty脚本文件头的偏移 */
    uint32 title_addr;
    /*! ListBox滑动条结构体地址，相对于sty脚本文件头的偏移 */
    uint32 slider_addr;
    /*! ListBox列表项说明结构体地址，相对于sty脚本文件头的偏移 */
    uint32 item_addr;
    /*! ListBox序数/总数结构体地址，相对于sty脚本文件头的偏移 */
    uint32 ratio_addr;
    /*! ListBox背景图片res_id */
    uint16 back_id;
    /*! ListBox背景图片显示X坐标 */
    uint16 back_x;
    /*! ListBox背景图片显示Y坐标 */
    uint16 back_y;
    /*! item项的间距 */
    uint8 inc_y;
    /*! item项一屏的显示行数 */
    uint8 num_one_page;
    /*!
     * \li  attrib<0>表示是否显示标题，0为不显示，1为显示;
     * \li  attrib<1>表示是否显示标题，0为不显示，1为显示;
     * \li  attrib<2>表示是否显示右侧滑动条，0为不显示，1为显示;
     * \li  attrib<3>表示是否显示序号/总项数，0为不显示，1为显示;
     * \li  attrib<4-15> 为保留字段。
     */
    uint16 attrib;
    /*! 保留字段 */
    uint16 reserve;
} listbox_t;

/*!
 * \brief
 *  listbox_title_t 描述listbox的title数据结构
 */
typedef struct
{
    /*! 标题显示区域X坐标 */
    uint16 str_x;
    /*! 标题显示区域Y坐标 */
    uint16 str_y;
    /*! 标题显示区域宽度 */
    uint16 str_width;
    /*! 标题显示区域高度 */
    uint16 str_height;
    /*! 标题字符串显示颜色 */
    uint16 str_color;
    /*!
     * \li   attrib<0-1>字符串对齐属性，0表示左对齐，1表示居中对齐，2表示居右对齐，默认居中对齐;
     * \li   attrib<2-3> 表示字体字号，0表示小号，1表示中号，2表示大号，默认为中号字体;
     * \li   attrib<4-15> 为保留字段
     */
    uint16 attrib;
} listbox_title_t;

/*!
 * \brief
 *  listbox_slider_t 描述listbox的slider数据结构
 */
typedef struct
{
    /*! 滑动条背景图片res_id */
    uint16 slider_back_id;
    /*! 滑动条背景图片X坐标 */
    uint16 slider_back_x;
    /*! 滑动条背景图片Y坐标 */
    uint16 slider_back_y;
    /*! 滑动点图标res_id */
    uint16 slider_point_id;
    /*! 滑动点背景图标res_id */
    uint16 slider_clear_id;
    /*! 滑动点图标X坐标 */
    uint16 slider_point_x;
    /*! 滑动点图标Y坐标 */
    uint16 slider_point_y;
    /*! 滑动条长度，要求sliderbackID对应的图片的高度等于这个值
     *   height(slider_back_id) - height(slider_point_id) - (slider_point_y - slider_back_y) * 2
     */
    uint16 slider_length;
} listbox_slider_t;

/*!
 * \brief
 *  listbox_item_t 描述listbox的item数据结构
 */
typedef struct
{
    /*! 列表项区背景图标，坐标为 back_x,back_y */
    uint16 list_back_id;
    /*! 列表项非活动项前置图标，res id，如果是multy模式，要求各图标ID递增 */
    uint16 head_id;
    /*! 列表项活动项前置图标，res id，如果是multy模式，要求各图标ID递增 */
    uint16 head_act_id;
    /*! 列表项第一项前置图标X坐标 */
    uint16 head_x;
    /*! 列表项第一项前置图标Y坐标 */
    uint16 head_y;
    /*! 如果是纯色背景的话，那么该值表示背景颜色值；如果是背景图片模式，那么表示列表项
     *   非活动项背景图片res id，如果是multy模式，要求各列表项背景图标ID递增
     */
    uint16 back_id;
    /*! 如果是纯色背景的话，那么该值表示背景颜色值；如果是背景图片模式，那么表示列表项
     *   活动项背景图片res id，如果是multy模式，要求各列表项背景图标ID递增
     */
    uint16 back_act_id;
    /*! 列表项第一项背景图片X坐标 */
    uint16 back_x;
    /*! 列表项第一项背景图片Y坐标 */
    uint16 back_y;
    /*! 列表项非活动项后置图标，res id，如果是multy模式，要求各图标ID递增 */
    uint16 tail_id;
    /*! 列表项活动（选中）项后置图标，res id，如果是multy模式，要求各图标ID递增 */
    uint16 tail_act_id;
    /*! 列表项第一项后置图标X坐标 */
    uint16 tail_x;
    /*! 列表项第一项后置图标Y坐标 */
    uint16 tail_y;
    /*! 列表项第一项显示区域X坐标 */
    uint16 data_x;
    /*! 列表项第一项显示区域Y坐标 */
    uint16 data_y;
    /*! 列表项显示区域宽度 */
    uint16 data_width;
    /*! 列表项显示区域高度*/
    uint16 data_height;
    /*! 非激活项显示字体颜色*/
    uint16 data_color;
    /*! 激活项显示字体颜色*/
    uint16 data_act_color;
    /*!
     * \li  attrib<0-1>表示选中项（非激活时）后置图标显示方式，0表示不显示，1表示显示（single），2表示显示（multy）;
     * \li  attrib<2-3>表示选中项（激活时）后置图标显示方式，0表示不显示，1表示显示（single），2表示显示（multy）;
     * \li  attrib<4-5>表示前置图标（非激活时）显示方式，0表示不显示，1表示显示（single），2表示显示（multy），3表示
     *      显示数字图标，headID[0]中存储的是数字0 的res id，这种方式只适用于列表项总数少于10的情况;
     * \li  attrib<6-7>表示前置图标（激活时）显示方式，0表示不显示，1表示显示（single），2表示显示（multy），3表示
     *      显示数字图标，headID[0]中存储的是数字0 的res id，这种方式只适用于列表项总数少于10的情况;
     * \li  attrib<8-9>表示非激活项背景显示模式，0表示背景颜色，1表示背景图片（single），2表示背景图片（multy）;
     * \li  attrib<10-11>表示激活项背景显示模式，0表示背景颜色，1表示背景图片（single），2表示背景图片（multy）
     * \li  attrib<12-15> 为保留字段。
     */
    uint16 attrib;
    /*!
     * \li  str_attrib<0-1>表示活动项列表项字符串字号，0表示小字号，1表示中字号，2表示大字号，默认为中字号;
     * \li  str_attrib<2-3>表示活动项字符串内容显示对齐方式，0表示左对齐，1表示居中，2表示右对齐;
     * \li  str_attrib<4-5>表示活动项列表项字符串的长字符串显示属性，0：多行显示，1：滚屏显示，
     *                     2：截断行显示，3：省略号显示，默认为滚屏显示;
     * \li  str_attrib<6-7>表示非活动项列表项字符串字号，0表示小字号，1表示中字号，2表示大字号，默认为中字号;
     * \li  str_attrib<8-9>表示非活动项字符串内容显示对齐方式，0表示左对齐，1表示居中，2表示右对齐;
     * \li  str_attrib<10-11>表示非活动项列表项字符串的长字符串显示属性，0：多行显示，1：滚屏显示;
     *      2：截断行显示，3：省略号显示，默认为省略号显示;
     * \li  str_attrib<12-15> 为保留字段。
     */
    uint16 str_attrib;
    /*! 保留字段 */
    uint16 reserve;
} listbox_item_t;

/*!
 * \brief
 *  listbox_ratio_t 描述listbox的ratio数据结构
 */
typedef struct
{
    /*! 数字0的res id ，0-9 的res id 必须递增 */
    uint16 num0_id;
    /*! 分隔符ID */
    uint16 partition_id;
    /*! 数字资源图片的宽度 */
    uint8 num_width;
    /*! 分隔符图片的宽度 */
    uint8 partition_width;
    /*! 数字图片透明（背景）颜色 */
    /*uint16 numpic_back_color;*/
    /*! 序号位数 */
    uint16 ratio_index_cnt;
    /*! 总数位数 */
    uint16 ratio_total_cnt;
    /*! 显示起始位置X坐标 */
    uint16 ratio_x;
    /*! 显示起始位置Y坐标 */
    uint16 ratio_y;
    /*! 显示区域总宽度 */
    uint16 ratio_width;
    /*! 显示区域总高度 */
    uint16 ratio_height;
    /*! 背景图片x坐标 */
    uint16 back_x;
    /*! 背景图片y坐标 */
    uint16 back_y;
    /*! 数字和分隔符的背景图片ID或者背景颜色值 */
    uint16 back_id;
    /*! 数字和分隔符（字符串）的画笔颜色 */
    uint16 text_color;
    /*!
     * \li  attrib<0>表示显示模式，0表示字符串显示（分隔符默认为/），1表示数字图片显示;
     * \li  attrib<1-2>表示显示对齐方式，0表示左对齐，1表示居中，2表示右对齐;
     * \li  attrib<3>表示是否序号不足位数时是否前面补0，1表示是，0表示否;
     * \li  attrib<4>表示是否总项数不足位数时是否前面补0，1表示是，0表示否;
     * \li  attrib<5>表示背景模式，0表示背景颜色，1表示背景图片;
     * \li  attrib<6-7>表示字体字号，0表示小号字体，1表示中号字体，2表示大号字体，默认为中号字体;
     * \li  attrib<8-15> 为保留字段。
     */
    uint16 attrib;
    /*! 保留字段 */
    /*uint16 reserve;*/
} listbox_ratio_t;

/*!
 * \brief
 *  parambox_t 描述parambox的数据结构
 */
typedef struct
{
    /*! ParamBox标题结构体的存储地址 */
    uint32 title_addr;
    /*! ParamBox数字提示栏的存储地址 */
    uint32 num_addr;
    /*! ParamBox单位存储地址 */
    uint32 unit_addr;
    /*! ParamBox背景图片，res id */
    uint16 back_id;
    /*! ParamBox背景图片的X坐标 */
    uint16 back_x;
    /*! ParamBox背景图片的Y坐标 */
    uint16 back_y;
    /*! ParamBox标志图，res id */
    uint16 icon_id;
    /*! ParamBox标志图的X坐标 */
    uint16 icon_x;
    /*! ParamBox标志图的Y坐标 */
    uint16 icon_y;
    /*! ParamBox参数数目，用于UI Editor显示以及验证参数数目是否匹配 */
    uint16 param_cnt;
    /*! 参数框（外形区域）X坐标 */
    uint16 list_x[3];
    /*! 参数（外形区域）Y坐标 */
    uint16 list_y[3];
    /*! 参数框（外形区域）宽度 */
    uint16 list_width[3];
    /*! 参数（外形区域）高度 */
    uint16 list_height[3];
    /*! 参数分隔符ID号，-1表示不显示 */
    uint16 param_part_id[2];
    /*! 参数分隔符X坐标 */
    uint16 param_part_x[2];
    /*! 参数分隔符Y坐标 */
    uint16 param_part_y[2];
    /*!
     * \li  attrib<0> 是否显示ParamBox，0为不显示，1为显示;
     * \li  attrib<1> 有无title， 0为无，1为有;
     * \li  attrib<2> 是否显示数字，0为不显示，1为显示;
     * \li  attrib<3> 是否显示单位，0为不显示，1为显示;
     * \li  attrib<4> 参数列表排列方向，0表示横向，1表示竖向;
     * \li  attrib<5> 是否显示类型图标，0为不显示，1为显示;
     * \li  attrib<6-15> 为保留字段。
     */
    uint16 attrib;
} parambox_t;

/*!
 * \brief
 *  parambox_title_t 描述parambox的title数据结构
 */
typedef struct
{
    /*! 标题名称资源ID，默认显示为"参数设置" */
    uint16 title_id;
    /*! 标题名称显示位置x坐标 */
    uint16 title_x;
    /*! 标题名称显示位置y坐标 */
    uint16 title_y;
    /*! 标题名称显示区域宽度 */
    uint16 title_width;
    /*! 标题名称显示区域高度 */
    uint16 title_height;
    /*! 标题显示颜色 */
    uint16 color;
    /*!
     * \li  attrib<0-1>表示显示对齐方式，0表示左对齐，1表示居中，2表示右对齐;
     * \li  attrib<2-3> 表示字体字号，0表示小号，1表示中号，2表示大号，默认为中号字体;
     * \li  attrib<4-15> 为保留字段。
     */
    uint16 attrib;
    /*! 保留字段 */
    uint16 reserve;
} parambox_title_t;

/*!
 * \brief
 *  parambox_num_t 描述parambox的num数据结构
 */
typedef struct
{
    /*! 非激活参数值背景图片或者背景颜色，如果是图片（multy）模式，要求各背景图ID递增 */
    uint16 back_id;
    /*! 激活参数值背景图片或者背景颜色，如果是图片（multy）模式，要求各背景图ID递增 */
    uint16 back_act_id;
    /*! 多参数竖向排列时，参数数字区域Y坐标，Y坐标同外形区域；单参数必填 */
    uint16 num_x;
    /*! 多参数横向排列时，参数数字区域Y坐标，X坐标同外形区域；单参数必填 */
    uint16 num_y;
    /*! 多参数横向排列时，参数数字区域高度，宽度同外形区域；单参数必填 */
    uint16 num_height;
    /*! 多参数竖向排列时，参数数字区域宽度，高度同外形区域；单参数必填 */
    uint16 num_width;
    /*! 数字0图片资源ID（要求0-9，+，-，‘ ’图片资源ID递增）*/
    uint16 pic_id;
    /*! 数字图片宽度 */
    uint16 pic_width;
    /*! 非激活项参数值字符串颜色 */
    uint16 num_color;
    /*! 激活项参数值字符串颜色 */
    uint16 num_act_color;
    /*!
     * \li  attrib<0> 是否显示正数前面的+号，0为不显示，1为显示;
     * \li  attrib<1> 是否显示前置0，即当位数不足时是否用0补充，1为是，0为否;
     * \li  attrib<2> 是否参数值显示模式，0表示字符串显示，1表示数字图片显示;
     * \li  attrib<3-4>表示非激活项背景模式，0表示背景颜色，1表示背景图片（single），2表示背景图片（multy）;
     * \li  attrib<5-6>表示激活项背景模式，0表示背景颜色，1表示背景图片（single），2表示背景图片（multy）;
     * \li  attrib<7-8>表示显示对齐方式，0表示左对齐，1表示居中，2表示右对齐，默认为居中;
     * \li  attrib<9-10> 表示数字字体字号，0表示小号，1表示中号，2表示大号，默认为中号字体;
     * \li  attrib<11-15> 为保留字段。
     */
    uint16 attrib;
    /*! 保留字节 */
    uint16 reserve;
} parambox_num_t;

/*!
 * \brief
 *  parambox_unit_t 描述parambox的unit数据结构
 */
typedef struct
{
    /*! 多参数竖向排列时，参数单位字符串Y坐标；单参数必填 */
    uint16 unit_x;
    /*! 多参数横向排列时，参数单位字符串Y坐标；单参数必填 */
    uint16 unit_y;
    /*! 多参数横向排列时，参数单位字符串高度，宽度等于外形区域宽度；单参数必填 */
    uint16 unit_height;
    /*! 多参数竖向排列时，参数单位字符串宽度，高度等于外形区域高度；单参数必填 */
    uint16 unit_width;
    /*! 单位字符串显示颜色 */
    uint16 uint_color;
    /*!
     * \li  attrib<0-1>表示显示对齐方式，0表示左对齐，1表示居中，2表示右对齐，默认为居中;
     * \li  attrib<2-3> 表示单位字体字号，0表示小号，1表示中号，2表示大号，默认为中号字体;
     * \li  attrib<4-15> 为保留字段。
     */
    uint16 attrib;
} parambox_unit_t;

/*!
 * \brief
 *  dialogbox_t 描述dialogbox的数据结构
 */
typedef struct
{
    /*! DialogBox选项列表的存储地址 */
    uint32 item_addr;
    /*! DialogBox背景图片，res id */
    uint16 back_id;
    /*! DialogBox背景图片的X坐标 */
    uint16 back_x;
    /*! DialogBox背景图片的Y坐标 */
    uint16 back_y;
    /*! DialogBox类型图片，res id */
    uint16 icon_id;
    /*! DialogBox类型图片的X坐标 */
    uint16 icon_x;
    /*! DialogBox类型图片的Y坐标 */
    uint16 icon_y;
    /*! DialogBox说明字符串的X坐标 */
    uint16 desc_x;
    /*! DialogBox说明字符串的Y坐标 */
    uint16 desc_y;
    /*! DialogBox说明字符串的宽度 */
    uint16 desc_width;
    /*! DialogBox说明字符串的高度 */
    uint16 desc_height;
    /*! DialogBox说明字符串的字体颜色*/
    uint16 desc_color;
    /*! DialogBox按钮数目，用于UI Editor显示，以及验证按钮数目是否匹配 */
    uint16 item_cnt;
    /*!
     * \li  attrib<0> 是否显示DialogBox，0为不显示，1为显示;
     * \li  attrib<1> 保留;
     * \li  attrib<2-3>表示说明字符串对齐方式，0表示左对齐，1表示居中，2表示右对齐，默认为居中对齐;
     * \li  attrib<4-5> 表示字体字号，0表示小号，1表示中号，2表示大号，默认为中号字体;
     * \li  attrib<6> 是否显示选项列表，0为不显示，1为显示;
     * \li  attrib<7> 是否显示 icon 图标
     * \li  attrib<8-15> 为保留字段。
     */
    uint16 attrib;
    /*! 保留字段 */
    uint16 reserve;
} dialogbox_t;

/*!
 * \brief
 *  dialogbox_item_t 描述dialogbox_item的数据结构
 */
typedef struct
{
    /*! 选项列表区域X坐标 */
    uint16 item_x[3];
    /*! 选项列表区域Y坐标 */
    uint16 item_y[3];
    /*! 选项列表区域宽度 */
    uint16 item_width[3];
    /*! 选项列表区域高度 */
    uint16 item_height[3];
    /*! 非选中项的背景图片（在multy模式下要求背景图片ID连续），或者背景颜色（整个选项区域，制造按钮效果）*/
    uint16 backgd_id;
    /*! 非选中项的字体颜色 */
    uint16 text_color;
    /*! 选中项的背景图片（在multy模式下要求背景图片ID连续），或者背景颜色（整个选项区域，制造按钮效果）*/
    uint16 act_backgd_id;
    /*! 选中项的字体颜色 */
    uint16 act_text_color;
    /*!
     * \li  attrib<0-1>表示非选中项背景显示模式，0表示背景颜色，1表示背景图片（single），2表示背景图片（multy）;
     * \li  attrib<2-3>表示选中项背景显示模式，0表示背景颜色，1表示背景图片（single），2表示背景图片（multy）;
     * \li  attrib<4-5>表示item字符串对齐方式，0表示左对齐，1表示居中，2表示右对齐，默认为居中对齐;
     * \li  attrib<6-7> 表示字体字号，0表示小号，1表示中号，2表示大号，默认为中号字体;
     * \li  attrib<8-15> 为保留字段。
     */
    uint16 attrib;
    /*! 保留字段 */
    uint16 reserve;
} dialogbox_item_t;

/*!
 * \brief
 *  sliderbar_t 描述sliderbar的数据结构
 */
typedef struct
{
    /*! sliderbar标题结构体的存储地址 */
    uint32 title_addr;
    /*! sliderbar数字提示栏的存储地址 */
    uint32 num_addr;
    /*! sliderbar单位存储地址 */
    uint32 unit_addr;
    /*! sliderbar滑动条栏的存储地址 */
    uint32 slider_addr;
    /*! sliderbar背景图片，res id */
    uint16 back_id;
    /*! sliderbar背景图片的X坐标 */
    uint16 back_x;
    /*! sliderbar背景图片的Y坐标 */
    uint16 back_y;
    /*! sliderbar标志图，res id */
    uint16 icon_id;
    /*! sliderbar标志图的X坐标 */
    uint16 icon_x;
    /*! sliderbar标志图的Y坐标 */
    uint16 icon_y;
    /*! sliderbar参数数目，用于UI Editor显示以及验证参数数目是否匹配 */
    uint16 param_cnt;
    /*! 参数框（外形区域）X坐标 */
    uint16 list_x;
    /*! 参数（外形区域）Y坐标 */
    uint16 list_y;
    /*! 参数框（外形区域）宽度 */
    uint16 list_width;
    /*! 参数（外形区域）高度 */
    uint16 list_height;
    /*! 附加图片，用来显示对称进度条的中间分隔线等图标 */
    uint16 addition_pic;
    /*! 附加图片 X 坐标 */
    uint16 addition_x;
    /*! 附加图片 Y 坐标 */
    uint16 addition_y;
    /*!
     * \li  attrib<0> 是否显示sliderbar，0为不显示，1为显示;
     * \li  attrib<1> 有无title， 0为无，1为有;
     * \li  attrib<2> 是否显示数字，0为不显示，1为显示;
     * \li  attrib<3> 是否显示单位，0为不显示，1为显示;
     * \li  attrib<4> 是否显示滑动条，0为不显示，1为显示;
     * \li  attrib<5> 参数列表排列方向，0表示横向，1表示竖向;
     * \li  attrib<6> 是否显示类型图标，0为不显示，1为显示;
     * \li  attrib<7-15> 为保留字段。
     */
    uint16 attrib;
    /*! 保留字节 */
    uint16 reserve;
} sliderbar_t;

/*!
 * \brief
 *  sliderbar_title_t 描述sliderbar的title数据结构
 */
typedef struct
{
    /*! 标题名称资源ID，默认显示为"参数设置" */
    uint16 title_id;
    /*! 标题名称显示位置x坐标 */
    uint16 title_x;
    /*! 标题名称显示位置y坐标 */
    uint16 title_y;
    /*! 标题名称显示区域宽度 */
    uint16 title_width;
    /*! 标题名称显示区域高度 */
    uint16 title_height;
    /*! 标题显示颜色 */
    uint16 color;
    /*!
     * \li  attrib<0-1>表示显示对齐方式，0表示左对齐，1表示居中，2表示右对齐;
     * \li  attrib<2-3> 表示字体字号，0表示小号，1表示中号，2表示大号，默认为中号字体;
     * \li  attrib<4-15> 为保留字段。
     */
    uint16 attrib;
    /*! 保留字段 */
    uint16 reserve;
} sliderbar_title_t;

/*!
 * \brief
 *  sliderbar_num_t 描述sliderbar的num数据结构
 */
typedef struct
{
    /*! 参数的背景图片Y坐标，X坐标根据list_w和图片宽度居中显示方式来计算 */
    uint16 back_y;
    /*! 非激活参数值背景图片或者背景颜色，如果是图片（multy）模式，要求各背景图ID递增 */
    uint16 back_id;
    /*! 激活参数值背景图片或者背景颜色，如果是图片（multy）模式，要求各背景图ID递增 */
    uint16 back_act_id;
    /*! 多参数竖向排列时，参数数字区域Y坐标，Y坐标同外形区域；单参数必填 */
    uint16 num_x;
    /*! 多参数横向排列时，参数数字区域Y坐标，X坐标同外形区域；单参数必填 */
    uint16 num_y;
    /*! 多参数横向排列时，参数数字区域高度，宽度同外形区域；单参数必填 */
    uint16 num_height;
    /*! 多参数竖向排列时，参数数字区域宽度，高度同外形区域；单参数必填 */
    uint16 num_width;
    /*! 数字0图片资源ID（要求0-9，+，-，‘ ’图片资源ID递增）*/
    uint16 pic_id;
    /*! 数字图片宽度 */
    uint16 pic_width;
    /*! 非激活项参数值字符串颜色 */
    uint16 num_color;
    /*! 激活项参数值字符串颜色 */
    uint16 num_act_color;
    /*!
     * \li  attrib<0> 是否显示正数前面的+号，0为不显示，1为显示;
     * \li  attrib<1> 是否显示前置0，即当位数不足时是否用0补充，1为是，0为否;
     * \li  attrib<2> 是否参数值显示模式，0表示字符串显示，1表示数字图片显示;
     * \li  attrib<3-4>表示非激活项背景模式，0表示背景颜色，1表示背景图片（single），2表示背景图片（multy）;
     * \li  attrib<5-6>表示激活项背景模式，0表示背景颜色，1表示背景图片（single），2表示背景图片（multy）;
     * \li  attrib<7-8>表示显示对齐方式，0表示左对齐，1表示居中，2表示右对齐，默认为居中;
     * \li  attrib<9-10> 表示数字字体字号，0表示小号，1表示中号，2表示大号，默认为中号字体;
     * \li  attrib<11-15> 为保留字段。
     */
    uint16 attrib;
} sliderbar_num_t;

/*!
 * \brief
 *  sliderbar_unit_t 描述sliderbar的unit数据结构
 */
typedef struct
{
    /*! 多参数竖向排列时，参数单位字符串Y坐标；单参数必填 */
    uint16 unit_x;
    /*! 多参数横向排列时，参数单位字符串Y坐标；单参数必填 */
    uint16 unit_y;
    /*! 多参数横向排列时，参数单位字符串高度，宽度等于外形区域宽度；单参数必填 */
    uint16 unit_height;
    /*! 多参数竖向排列时，参数单位字符串宽度，高度等于外形区域高度；单参数必填 */
    uint16 unit_width;
    /*! 单位字符串显示颜色 */
    uint16 uint_color;
    /*!
     * \li  attrib<0-1>表示显示对齐方式，0表示左对齐，1表示居中，2表示右对齐，默认为居中;
     * \li  attrib<2-3> 表示单位字体字号，0表示小号，1表示中号，2表示大号，默认为中号字体;
     * \li  attrib<4-15> 为保留字段。
     */
    uint16 attrib;
} sliderbar_unit_t;

/*!
 * \brief
 *  sliderbar_slider_t 描述sliderbar的slider数据结构
 */
typedef struct
{
    /*! 滑动条背景图片ID，如果为multy，要求背景图片ID递增 */
    uint16 slider_back_id;
    /*! 滑动条背景图片ID，如果为multy，要求背景图片ID递增，如果只有一个滑动条，可以为-1 */
    uint16 slider_back_act_id;
    /*! 进度条横向时，滑动条背景图片X坐标；进度条竖向时，由进度条居中计算所得 */
    uint16 slider_back_x;
    /*! 进度条竖向时，滑动条背景图片Y坐标；进度条横向时，由进度条居中计算所得 */
    uint16 slider_back_y;
    /*! 滑动条填充图标，长度为1个像素点的res id；
     *  如果只有一个滑动条并且没有刻度，可以为-1；如果只有一个滑动条但有刻度，需要填一个像素点宽度的图标来作为刻度
     */
    uint16 slider_fill_id;
    /*! 激活滑动条填充图标，长度为1个像素点的res id */
    uint16 slider_fill_act_id;
    /*! 滑动条背景填充图标，长度为1个像素点的res id */
    uint16 slider_unfill_id;
    /*! 进度条竖向时有效：进度填充图标 起始X坐标 */
    uint16 slider_fill_x;
    /*! 进度条横向时有效：进度填充图标 起始Y坐标 */
    uint16  slider_fill_y;
    /*! 滑动条游标图片，res id */
    uint16 prog_id;
    /*! 与滑动条背景一样的滑动条游标图片，res id */
    uint16 unprog_id;
    /*! 滑动条游标图片起始X坐标 */
    /*! 进度条竖向时有效：滑动条游标图片 起始X坐标 */
    uint16  prog_x;
    /*! 进度条横向时有效：滑动条游标图片 起始Y坐标（位于进度条的最底部） */
    uint16  prog_y;
    /*! 进度条竖向时有效：进度条进度起始Y坐标 */
    uint16  slider_start_y;
    /*! 进度条横向时有效：进度条进度起始 X坐标 */
    uint16  slider_start_x;
    /*!  滑动条可滑动范围，单位为像素点，计算公式为：
     * \li  带游标竖屏进度条： = slider_back_length - slider_prog_length - (slider_back_y_end - slider_start_y) * 2;
     * \li  无游标竖屏进度条： = slider_back_length - (slider_back_y_end - slider_start_y) * 2;
     * \li  带游标横屏进度条： = slider_back_length - slider_prog_length - (slider_start_x - slider_back_x) * 2;
     * \li  无游标横屏进度条： = slider_back_length - (slider_start_x - slider_back_x) * 2;
     */
    uint16 length;
    /*! 刻度，表示每个刻度等于多少像素点宽度，默认为1（暂时不支持其他值） */
    uint16 scale;
    /*!
     * \li  attrib<0> 表示方向，0为横向，1为竖向;
     * \li  attrib<1-2> 表示风格，0表示仅进度填充；1表示仅游标标示，2表示两者都要;
     * \li  attrib<3> 表示是否刻度进度条，0表示否，1表示是，这种情况下，清除时要注意保留刻度;
     * \li  attrib<4> 表示是否对称正负值模式，即进度条中间刻度为0，向上为正，向下为负，正如将两个进度条合并为一个，
     *      0表示一般模式，1表示正负值模式（这时length应取一半）;
     * \li  attrib<5>表示非激活滑动条背景图片模式，0表示single，1表示multy;
     * \li  attrib<6>表示激活滑动条背景图片模式，0表示single，1表示multy;
     * \li  attrib<7-15> 为保留字段。
     */
    uint16 attrib;
} sliderbar_slider_t;

/*!
 * \brief
 *  style_t 描述style的数据结构
 */
typedef struct
{
    /*! 当前风格在sty文件中的存储起始地址 */
    uint32 style_begin_addr;
    /*! LCD屏幕的高度 */
    uint16 lcd_height;
    /*! LCD屏幕的宽度 */
    uint16 lcd_width;
} style_t;

/*!
 * \brief
 *  scene_t 描述scene的数据结构
 */
typedef struct
{
    /*! 背景图片 res id，或者背景颜色值 */
    uint16 back_id;
    /*! 场景区域X坐标 */
    uint16 x;
    /*! 场景区域Y坐标 */
    uint16 y;
    /*! 场景区域宽度 */
    uint16 width;
    /*! 场景区域高度 */
    uint16 height;
    /*! 场景中控件的数目 */
    uint16 ctrl_count;
    /*! 控件索引列表的开始位置 */
    uint32 ctrl_list_addr;
    /*!
     * \li  atrrib<0>表示背景模式，0为背景图，1为背景颜色;
     * \li  attrib<1-15> 为保留字段。
     */
    uint16 attrib;
    /*! 保留字段 */
    uint16 reserve;
} scene_t;


/*!
 * \brief
 *  scroll_desc_t 描述dialogbox_item的数据结构
 */
typedef struct
{
    /*! 滚屏字符串首地址 */
    uint8 *scrollhead_pt;
    /*! 滚屏字符串首地址 */
    uint8 *strhead_pt;
    /*! 滚屏区域 */
    region_t scroll_region; //滚屏区域
    /*! 滚屏的宽度*/
    uint8 scroll_width; //滚屏的宽度
    /*! 语言类型 */
    int8 language;
    /*! 滚屏字符串显示模式 */
    uint16 disp_mode;
    /*! 滚屏字符串背景图片id */
    uint16 back_id;
    /*! 滚屏字符串背景图片x坐标 */
    uint16 back_x;
    /*! 滚屏字符串背景图片y坐标 */
    uint16 back_y;
    /*! 滚屏字符串画笔颜色 */
    uint16 pen_color;
} scroll_desc_t;

/*!
 * \brief
 *      arabic_status_e 阿拉伯语和希伯来语编码流状态枚举类型
 */
typedef enum
{
    /*! 初始状态，由 string_desc_t 描述 */
    ARABIC_INIT       = 0,
    /*! UNICODE 连写状态 */
    ARABIC_UNI_JOIN   = 1,
    /*! 西文翻转状态 */
    ARABIC_ASCII_TURN = 2,
    /*! 中间完整单行翻转状态 */
    ARABIC_LINE_TURN  = 3
} arabic_status_e;

/*!
 * \brief
 *      text_background_t 字符串背景描述结构体
 */
typedef struct
{
    /*!字符串背景图片id */
    uint16 back_id;
    /*!字符串背景图片x坐标 */
    uint16 back_x;
    /*!字符串背景图片y坐标 */
    uint16 back_y;
    /*!字符串背景图片宽度*/
    uint16 width;
    /*!字符串背景图片高度 */
    uint16 height;
    /*!按列优先存储图片数据起始位置*/
    uint32 offset_col;
} text_background_t;

/*!
 * \brief
 *      fill_dump_direction_e 填充方向枚举类型
 */
typedef enum
{
    /*!从左到右填充 */
    DUMP_LEFT2RIGHT = 0,
    /*!从右到左填充 */
    DUMP_RIGHT2LEFT = 1,
    /*!从上到下填充 */
    DUMP_UP2DOWN = 2,
    /*!从下到上填充 */
    DUMP_DOWN2UP = 3,
} fill_dump_direction_e;

/*! 资源头字节数 */
#define  RESHEADITEM     16

/*! Unicode字符的默认高度 */
#define UNICODE_HEIGHT_DEF     16
/*! Unicode字符的默认宽度 */
#define UNICODE_WIDTH_DEF      16

/*! 内码字符和ASCII码的分界值 */
#define  ASCIIDISCODE          0x80

/*!
 * \brief
 *    3D效果
 */
#define  D3_NORMAL_DEF         0 //只加边框
#define  D3_KEYUP_DEF          1 //突出效果
#define  D3_KEYDOWN_DEF        2 //凹进效果
/*!
 * \brief
 *      3D图标效果颜色
 */
#define  FaceLightColor_def  Color_tab0202_def   //向光边的颜色
#define  BackLightColor_def  Color_tab0102_def   //背光边的颜色
#define  SelectedColor_def   Color_BLACK_def     //被选中图片的边框颜色

#define  ROW_PRIORITY_DEF    0 //行优先
#define  COL_PRIORITY_DEF    1 //列优先

//字符串缓冲区长度，统一为124字节，存放60个字符（UNICODE情况下，包括前置标志和结束符）
#define  STRING_BUFFER_LEN   124

//ui驱动内部字符串缓冲区
extern uint8 string_buf[STRING_BUFFER_LEN];

//滚屏相关变量
extern uint8 scroll_buf[STRING_BUFFER_LEN];
extern scroll_desc_t scroll_desc;
extern string_desc_t scroll_str;
extern text_background_t text_bg_desc;
extern bool  scroll_flag;//字符串是否超长
extern uint8 scrollhead_height; //滚屏高度

//阿拉伯和希伯莱语相关变量
extern uint8 prev_valid_next_joining; //保存arabic字符连写变量
extern arabic_status_e arabic_stat;//阿拉伯和希伯莱语编码流状态
#ifndef PC
extern uint8 *arabic_string_buf;
#else
extern uint8 arabic_string_buf[STRING_BUFFER_LEN];
#endif

extern listbox_item_t list_item; //保存listbox item的控件数据结构
extern uint16 timebox_sty_id;
extern uint16 prev_num[2]; //记录progress上一次进度
extern int16 slider_prev_offset;//保存parambox的slider 上一次刻度值
extern uint16 prev_pixel;   //保存listbox slider上一次的刻度值
extern uint8 ratio_bits;   //保存ratio的数字位数
extern uint8 num_total_bits; //保存numbox的数字位数总数
extern uint8 numbox_disp_prev[12];//保存numbox的上一次显示内容
extern uint8 param_bits;    //保存parambox的数字位数总数
extern uint8 time_bits; //保存上一次timebox显示的数字总数

extern uint8 lcd_buffer_type; //buffer类型
extern uint8 language_id; //当前语言种类
extern bool  language_2byte;//是否双字节（内码）语言，比如中日韩语
extern bool  rotate_flag;//屏幕旋转为横屏
extern uint16 lang_option;//特殊语言选项

//是否需要进行阿拉伯和希伯莱特殊处理
static inline bool need_arabic_deal(uint16 disp_mode)
{
    if (((language_id == ARABIC) || (language_id == HEBREW)) && ((disp_mode & DISP_ARABIC_DISABLE) == 0))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

//是否阿拉伯语言字符
static inline bool is_arabic_char(uint16 ch)
{
    if (((ch >= 0x600) && (ch <= 0x6ff)) || ((ch >= 0x750) && (ch <= 0x77f)))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

//是否希伯来语言字符
static inline bool is_hebrew_char(uint16 ch)
{
    if ((ch >= 0x590) && (ch <= 0x5ff))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

//获取控件在文件中的起始地址
static inline void get_ctrl_addr(uint8 ctrl_type, uint32 *ctrl_begin_addr, res_open_infor_t *style_ptr)
{
    sys_sd_fseek(style_ptr->res_fp, SEEK_SET, (uint32) (style_ptr->cur_style_begin_offset + ctrl_type * 8));
    sys_sd_fread(style_ptr->res_fp, ctrl_begin_addr, sizeof(uint32));
}
//打开资源文件
bool res_open(char *filename, res_type_e type, void *null3);
//关闭资源文件
void res_close(res_type_e type, void *null2, void *null3);
//music ID3滚屏专用函数
bool scroll_string_ext(scroll_content_t *scroll_string_infor, scroll_param_t *scroll_param, void *null3);
//显示picbox
ui_result_e show_picbox(style_infor_t *picbox_style,picbox_private_t *picbox_data, void *null3)__FAR__;
//显示textbox
ui_result_e show_textbox(style_infor_t *textbox_style,textbox_private_t *textbox_data,textbox_draw_mode_e mode)__FAR__;
//显示timebox
ui_result_e show_timebox(style_infor_t *timebox_style, timebox_private_t *timebox_data, timebox_draw_mode_e mode);
//显示numbox
ui_result_e show_numbox(style_infor_t *numbox_style, numbox_private_t *numbox_data, numbox_draw_mode_e mode);
//显示progressbar
ui_result_e show_progressbar(style_infor_t *progressbar_style, progressbar_private_t *progressbar_data,
        progress_draw_mode_e mode);
//load attributebox
ui_result_e load_attributebox(style_infor_t *attributebox_style, void*attributebox_data, uint16 count);
//显示listbox
ui_result_e show_listbox(style_infor_t *listbox_style, listbox_private_t *listbox_data, list_draw_mode_e mode);
//显示dialogbox
ui_result_e show_dialogbox(style_infor_t *dialogbox_style, dialog_private_t *dialogbox_data, dialog_draw_mode_e mode);
//显示parambox
ui_result_e show_parambox(style_infor_t *parambox_style, parambox_private_t *parambox_data, parambox_draw_mode_e mode);
//显示sliderbar
ui_result_e show_sliderbar(style_infor_t *parambox_style, parambox_private_t *parambox_data, parambox_draw_mode_e mode);
//获取textbox的区域参数
ui_result_e get_textbox_attrb(style_infor_t *textbox_style, void *attrb, uint8 type);
ui_result_e get_picbox_attrb(style_infor_t *picbox_style, void *attrb, uint8 type);
ui_result_e get_listbox_attrb(style_infor_t *listbox_style, void *attrb, uint8 type);
void read_pic_to_buffer(uint16 id, uint8 *buffer, res_size_t *res_size);
//设置语言
bool set_language(uint8 lang_id,void *null2, void *null3)__FAR__;
//清屏
void clear_screen(region_t *clrregion,void *null2,void *null3)__FAR__;
void flush_font_buffer(void *null1, void *null2, void *null3)__FAR__;
//显示字符串
uint8 show_string(string_desc_t *string_infor, region_t *str_region,uint16 mode)__FAR__;
//字符串整理为“部分...”形式
uint16 ellipsis_to_longstr(string_desc_t *desc, uint16 max_len, str_getlen_result_t *getlen_result)__FAR__;
//字符串断行，也可用于统计行数
text_end_mode_e get_text_line(string_desc_t *str_desc, uint16 mode, void *null3)__FAR__;
//选择buffer的地址及大小
void set_display_buf(uint8 mode,void *null2,void *null3)__FAR__;
//设置语言特殊选项
void set_lang_option(uint16 mode, void *null2, void *null3)__FAR__;
//unicode转内码
bool unicode_to_char(uint8 *str,uint16 len,uint8 lang_id)__FAR__;
//内码转unicode
bool char_to_unicode(uint8 *dsc,uint8 *src,uint16 len)__FAR__;
//UTF8转unicode
void utf8_to_unicode(uint8* src,uint16*dest,int16* size)__FAR__;
//设置屏幕模式，横屏或竖屏
void set_screen_direction(uint8 screen_mode, void *null2, void *null3)__FAR__;
void display_buffer_deal(void)__FAR__;
uint8 *put_string(string_desc_t *string_infor, region_t *str_region, uint16 mode);
bool scroll_string(string_desc_t *scroll_infor, region_t *scroll_region, scroll_mode_t scroll_mode)__FAR__;
void show_num_pic(uint16 num, uint16 x, uint16 y, res_entry_t *res_entry, res_open_infor_t *res_ptr)__FAR__;
uint8 *itoa_n(int32 num, uint8 *str, uint8 radix, uint16 counts, uint16 disp_mode)__FAR__;
void show_timebox_pic(timebox_private_t *time, timebox_t *timebox, timebox_draw_mode_e mode)__FAR__;
void show_timebox_string(timebox_private_t *time, timebox_t *timebox, timebox_draw_mode_e mode)__FAR__;
void show_numbox_pic(numbox_private_t *numbox_data, numbox_t *numbox, numbox_draw_mode_e mode)__FAR__;
void show_number_pic(uint8 *num_string, show_number_param_t *number_param, numbox_draw_mode_e mode)__FAR__;
void show_numbox_string(numbox_private_t *numbox_data, numbox_t *numbox, numbox_draw_mode_e mode)__FAR__;
uint8 TimeConvert(timebox_private_t *time, timebox_t *timebox, uint16 *time_buffer, uint8 *disp_bits)__FAR__;
void show_list_ratio(uint32 addr, listbox_private_t *listbox_data, uint8 mode, res_open_infor_t *style_ptr)__FAR__;
uint8 show_list_item(listbox_t *listbox, listbox_private_t *listbox_data, uint16 itemnum, uint8 type,\
res_open_infor_t *style_ptr)__FAR__;
void show_list_title(uint32 addr, string_desc_t *title_str, list_draw_mode_e mode, res_open_infor_t *style_ptr)__FAR__;
void show_list_slider(uint32 addr, listbox_private_t *listbox_data, list_draw_mode_e mode, \
res_open_infor_t *style_ptr)__FAR__;
void show_parambox_num(parambox_t *parambox, parambox_private_t *parambox_data, uint16 itemnum, uint8 type,\
parambox_draw_mode_e mode,res_open_infor_t *style_ptr)__FAR__;
void show_parambox_title(uint32 addr, uint16 title_id, res_open_infor_t *style_ptr)__FAR__;
void show_parambox_unit(parambox_t *parambox, parambox_private_t *parambox_data, uint16 itemnum,\
res_open_infor_t *style_ptr)__FAR__;
void show_sliderbar_num(sliderbar_t *sliderbar, parambox_private_t *parambox_data, uint16 itemnum, uint8 type,\
parambox_draw_mode_e mode,res_open_infor_t *style_ptr)__FAR__;
void show_sliderbar_title(uint32 addr, uint16 title_id, res_open_infor_t *style_ptr)__FAR__;
void show_sliderbar_unit(sliderbar_t *sliderbar,parambox_private_t *parambox_data, uint16 itemnum,\
res_open_infor_t *style_ptr)__FAR__;
void show_sliderbar_slider(sliderbar_t *sliderbar, parambox_private_t *parambox_data, uint16 itemnum, uint8 type,\
parambox_draw_mode_e mode,res_open_infor_t *style_ptr)__FAR__;

void char_out_rect(uint8 *str, int8 language, region_t *char_region, uint16 disp_mode, uint8 start_col, \
uint8 mode)__FAR__;
bool fill_rect_dump(region_t *fill_region, uint16 pic_id, fill_dump_direction_e direction)__FAR__;
uint8 *put_string_line_sdimage(string_desc_t *desc, region_t *str_region, uint8 mode)__FAR__;
void clear_remain_pixel_sdimage(region_t *remain_region)__FAR__;
#if 0//2012-10-30 17:27 修复版本
uint8 *fixf_put_string_line(string_desc_t *desc, region_t *str_region, char_fill_mode_e mode)__FAR__;
#endif

uint8 thai_point(uint8 *str, int8 language, uint16 source_code, uint8* char_buf, uint8 *pt_step)__FAR__;
uint16 arabic_char_to_unicode(string_desc_t *desc)__FAR__;
uint8 is_arabic_hebrew_string(string_desc_t *desc)__FAR__;
bool arabic_is_nsm(uint16 c)__FAR__;
uint8 arabic_do_join(string_desc_t *desc)__FAR__;
void arabic_uni_join(string_desc_t *desc)__FAR__;
void arabic_do_turn(uint8 *str, uint16 start, uint16 end) __FAR__;
void arabic_ascii_turn(string_desc_t *desc)__FAR__;
void bankaf_put_string_line_arabic_hook(string_desc_t *string_infor, uint16 disp_mode, uint16 param)__FAR__;

void ramf_put_string_line_hook(string_desc_t *string_infor, uint16 disp_mode, uint16 param)__FAR__;
uint8 verti_scroll_string(region_t *scroll, scroll_content_t *scroll_string_infor, uint8 index)__FAR__;
uint8 *put_string(string_desc_t *string_infor, region_t *str_region, uint16 mode)__FAR__;
uint16 get_string_length_id(uint16 str_id, uint16 disp_mode)__FAR__;

//透明图片显示，比如透明数字图片
void show_pic_transparent(transparent_pic_t *p_trs_pic)__FAR__;

//CHAR_FILL_SDIMAGE 支持，直接从SD读取背景再叠加字符串
void put_char_sd(uint8 *psoure_buf, region_t *char_region, char_disp_e char_disp, uint8 start_col, uint8 mode)__FAR__;
void black_char_to_color_sd(uint8 *psoure_buf, region_t *buf_region, char_disp_e char_disp, \
uint8 start_col, uint8 start_row, uint8 end_row, uint8 mode)__FAR__;
void load_bgpic_sd(uint16 id, uint16 x, uint16 y, uint8 mode)__FAR__;
bool u_put_sd_image(sd_handle *res_fp, uint32 img_addr, region_t *img_region)__FAR__;
ui_result_e show_picbox_id3(style_infor_t *picbox_style, picbox_private_t *picbox_data, uint16 start_row)__FAR__;

//图形处理接口
void draw_rect_frame(region_t* frame_region)__FAR__;
void draw_3D_rect(region_t* D3region, uint8 type)__FAR__;

#if (USE_MULTI_FONTLIB != 0)
//固化代码fix
void fixf_get_next_char_point(uint16 uni_code, uint8 *pbuffer)__FAR__;
void fixf_get_font_point(uint16 code_rename, uint8 *picbuf)__FAR__;

void fix_switch_fontlib(uint16 disp_mode)__FAR__;
uint16 fixf_get_string_length(string_desc_t *desc, uint16 max_length, str_getlen_result_t *result)__FAR__;
void fixf_black_char_to_color(uint8 *psoure_buf, region_t *buf_region, char_disp_e char_disp, uint8 start_col)__FAR__;
void fixf_char_fill(uint8 *psoure_buf, char_disp_e char_disp, uint8 start_col)__FAR__;
bool fixf_put_char(uint8 *psoure_buf, char_fill_mode_e mode, char_disp_e char_disp, uint8 start_col)__FAR__;
uint8 *fixf_put_string_line(string_desc_t *desc, region_t *str_region, char_fill_mode_e mode)__FAR__;
#endif
bool u_show_pic(uint16 id, uint16 x, uint16 y)__FAR__;
bool u_put_sd_image(sd_handle *res_fp, uint32 img_addr, region_t *img_region)__FAR__;
uint8 *fixft_put_string_line(string_desc_t *desc, region_t *str_region, char_fill_mode_e mode)__FAR__;
bool fix_fill_rect(region_t *fill_region, uint16 color);

#endif //_UI_DRIVER_H
