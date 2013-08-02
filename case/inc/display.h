/*******************************************************************************
 *                              us212A
 *                            Module: ui驱动
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved. 
 *
 * History:         
 *      <author>    <time>           <version >             <desc>
 *       lzcai     2011-9-9 16:23     1.0             build this file 
*******************************************************************************/
/*!
 * \file     display.h
 * \brief    ui驱动模块相关数据结构，宏定义，接口声明等
 * \author   lzcai
 * \version 1.0
 * \date  2011-9-9
*******************************************************************************/

#ifndef _DISPLAY_H
#define _DISPLAY_H

/*! \cond UI_DRIVER_API */

#ifdef PC
#include "Simulator.h"
#endif
#include "psp_includes.h"
#include "lcd_driver.h"
#include "lang_id.h"

//字库格式规定
//1、字模保存必须保存在Height*Width = H*8|H*16|H*24|H*32矩阵中，H >= 字模实际高度，并且 <= 矩阵宽度。
//2、当H >= 字模实际高度时，字模点阵垂直方向上居中存放，如果实际高度为偶数，那么上下边各填充一半；
//   如果为奇数，下边比上边多1个点。
//3、字模点阵水平方向上从做到右存放，从字节最高bit开始存放；
//   如果实际宽度大于矩阵宽度W的一半，那么每行（W bits）存放1行字模点阵；
//   否则，每行存放2行字模点阵，即第 2N 和 2N + 1行（从0开始编号）字模点阵紧连着存放；
//   其中字模点阵的行数等于矩阵高度，即包括填充行。
//   注：8*8矩阵字库不支持每行存放2行字模点阵，也就是说，即使实际宽度少于等于4，也是每行存放1行字模点阵。
//4、字模点阵数据后首字节为字模点阵的实际宽度，比如，16*16矩阵字库，第32个字节（从0开始编号）即为实际宽度。

//字库定义，对应UI Editor工具中的小字体、中字体和大字体
//bit0表示小字体，其名称必须为 UNICODES.FON 
//bit1表示中字体，其名称必须为 UNICODE.FON 
//bit2表示大字体，其名称必须为 UNICODEL.FON 
//如果为0，表示使用默认标准字库，即字模实际宽高为16*16
//注：如果 lcd 屏不支持读屏，即使不需要使用多字库，也需要把 USE_MULTI_FONTLIB 定义为 0x02
#define  USE_MULTI_FONTLIB           0x0 
//字库1，对应UI Editor工具中的小字体；如果没有小字体，UI Editor工具配置的小字体用中字体显示
#define  FONTLIB_1_HEIGHT            8   //生成字模点阵最大高度
#define  FONTLIB_1_HEIGHT_ACTUAL     8   //生成字模点阵实际最大高度
#define  FONTLIB_1_WIDTH             8   //生成字模点阵最大宽度，要求为8倍数
#define  FONTLIB_1_SIZE              (FONTLIB_1_HEIGHT * FONTLIB_1_WIDTH / 8 + 1)
//字库2，对应UI Editor工具中的中字体，该字体字库必须有
#define  FONTLIB_2_HEIGHT            16  //生成字模点阵最大高度
#define  FONTLIB_2_HEIGHT_ACTUAL     16  //生成字模点阵实际最大高度
#define  FONTLIB_2_WIDTH             16  //生成字模点阵最大宽度，要求为8倍数
#define  FONTLIB_2_SIZE              (FONTLIB_2_HEIGHT * FONTLIB_2_WIDTH / 8 + 1)
//字库3，对应UI Editor工具中的大字体；如果没有大字体，UI Editor工具配置的大字体用中字体显示
#define  FONTLIB_3_HEIGHT            24  //生成字模点阵最大高度
#define  FONTLIB_3_HEIGHT_ACTUAL     24  //生成字模点阵实际最大高度
#define  FONTLIB_3_WIDTH             24  //生成字模点阵最大宽度，要求为8倍数
#define  FONTLIB_3_SIZE              (FONTLIB_3_HEIGHT * FONTLIB_3_WIDTH / 8 + 1)
//字库字模缓冲区大小，同大字体字库，用于定义字模缓冲区；如果没有大字体字库，则选用中字体字库；
#define  MAX_CHARPOINTE_SIZE         FONTLIB_3_SIZE


/*!
 * \brief
 *  LIST_NUM_ONE_PAGE_MAX 列表项目数目
 */
#define LIST_NUM_ONE_PAGE_MAX   6

/*!
 * \brief
 *  ROW_MAX_WIDTH 文本解码一行最大宽度，每行最多显示 128 个像素点宽度
 */
#define ROW_MAX_WIDTH           128
/*!
 * \brief
 *  BUFF_ONE_ROW 每行buffer字节数，每行按最多 22 个字符算，Unicode字符串需要 46个字节
 */
#define BUFF_ONE_ROW            46

//listbox 菜单项显示参数bitmap
//后置图标选项
#define ITEMS_BACK_TAG_ARGV_SHIFT   (0)
#define ITEMS_BACK_TAG_ARGV         (1 << ITEMS_BACK_TAG_ARGV_SHIFT)
#define ITEMS_BACK_TAG_ISNOT_SELECT (0 << ITEMS_BACK_TAG_ARGV_SHIFT)
#define ITEMS_BACK_TAG_IS_SELECT    (1 << ITEMS_BACK_TAG_ARGV_SHIFT)
//前置图标选项：最高bit，0不显示，1显示；对MULTY 类而言，bit8~14 表示index，0x7f表示只与位置相关，其他值表示由其值指定
#define ITEMS_FRONT_TAG_ARGV_SHIFT  (15)
#define ITEMS_FRONT_TAG_INDEX_SHIFT (8)
#define ITEMS_FRONT_TAG_INDEX       (0x7f << ITEMS_FRONT_TAG_INDEX_SHIFT)
#define ITEMS_FRONT_TAG_ARGV        (0x1 << ITEMS_FRONT_TAG_ARGV_SHIFT)
#define ITEMS_FRONT_TAG_ISNOT_DISP  (0x0 << ITEMS_FRONT_TAG_ARGV_SHIFT)
#define ITEMS_FRONT_TAG_IS_DISP     (0x1 << ITEMS_FRONT_TAG_ARGV_SHIFT)

//字符串断行选项 bit10-12预留设置字体大小
//过滤空行选项
#define FILTER_BLANK_OPTION     (0x01 << 0)
#define FILTER_BLANK_DISABLE    (0x00 << 0)
#define FILTER_BLANK_ENABLE     (0x01 << 0)
//自动分词选项
#define DIVIDE_WORD_OPTION      (0x01 << 1)
#define DIVIDE_WORD_DISABLE     (0x00 << 1)
#define DIVIDE_WORD_ENABLE      (0x01 << 1)
//功能模式选项
#define FUNC_MODE_OPTION        (0x0f << 4)//bit4 ~ bit8
#define FUNC_GET_LINE_CHARS     (0x00 << 4)//获取当前行字节数
#define FUNC_GET_TOTAL_LINES    (0x01 << 4)//获取字符串总行数
#define FUNC_GET_CHAR_POINTS    (0x02 << 4)//获取当前行点阵缓冲

/*!
 * \brief
 *      textline_param_t 获取字符串点阵结构体
 */
typedef struct
{
    /*! 字符串点阵buffer */
    uint8 *point_buf;
    /*! 字符串点阵结果宽度 */
    uint16 true_width;
    /*! 字符串点阵结果高度 */
    uint16 true_height;
} textline_param_t;


/*!
 * \brief
 *  ui_error_e ui驱动处理错误类型
 */
typedef enum
{
    /*! 不显示控件 */
    UI_NO_DISPLAY           = 0x00,
    /*! 正常显示 */
    UI_NO_ERR               = 0x01,
    /*! 字符串显示需要滚屏 */
    UI_NEED_SCROLL          = 0x02,
    /*! 参数非法错误 */
    UI_PARAM_ERR            = 0x03,
    /*! 字符串显示X轴越界错误 */
    UI_PUTCHAR_X_ERR        = 0x04,
    /*! 字符串显示Y轴越界错误 */
    UI_PUTCHAR_Y_ERR        = 0x05,
    /*! 字符串显示其他错误 */
    UI_PUTCHAR_OTHER_ERR    = 0x06,
    /*! 字符串显示滚屏到末尾 */
    UI_SCROLL_OVER          = 0x07,
} ui_result_e;

/*!
 * \brief
 *  颜色值数据类型
 */
typedef uint32 COLORREF;

/*!
 * \brief
 *      DC 画图参数结构体
 */
typedef struct
{
    /*! 画笔和字体颜色*/
    COLORREF pen_color;
    /*! 背景颜色*/     
    COLORREF backgd_color;
} DC, *PDC;

/*!
 * \brief
 *      string_desc_t 字符串描述符结构体
 */
typedef struct
{
    /*! 字符串数据，或多国语言字符串ID，或code（内码或Unicode） */
    union
    {
        /*! code缓冲区指针 */
        uint8 *str;
        /*! 字符串ID */
        uint16 id;
    } data;
    /*! 字符串数据类型
     *  UNICODELANGUAGE（0x7f） 表示 Unicode 资源，只在 ui 驱动内部使用；
     *  UNICODEID（0x7e） 表示 Unicode ID；
     *  UNICODEDATA（0x7d） 表示 Unicode code data；
     *  UTF_8DATA（0x7c） 表示 utf-8 code data；
     *  ANSIDATAAUTO（0x7b） 表示 Ansi code data（自动处理为ui 驱动当前语言）；
     *  内码语言ID（如 ENGLISH） 表示 Ansi code data；
     *  说明：为了避免混乱，我们规定 UNICODELANGUAGE 只在 ui 驱动内部使用，应用用 UNICODEID 和 UNICODEDATA 代替之；
     */
    int8 language;
    uint8 reserve;
    /*! OUT：有效字节；IN：缓冲区大小 */
    uint16 length;
    /*! 任意参数，由具体场合决定其意义；如在断行中可以指定行最大像素点宽度； */
    uint16 argv;
    /*! 用于存放结果，由具体场合决定其意义；如在断行中可以表示显示行字节数； */
    uint16 result;
} string_desc_t;



/*!
 * \brief
 *      picbox显示顺序，正序显示
 */
#define DIRECTION_NORMAL    0
/*!
 * \brief
 *      picbox显示顺序，逆序显示
 */
#define DIRECTION_REVERSE   1

/*!
 * \brief
 *      picbox_private_t picbox私有数据结构
 * \note         
 *      pic_id!= -1，优先显示pic_id所指向的图片；
 *      pic_id = -1，frame_id！=-1，则显示StyleID所指向的Picbox的frame_id的图片（多帧图片中的一帧）
 */
typedef struct
{
    /*! 如果pic_id不为-1，那么就是优先用pic_id显示 */
    uint16 pic_id;
    /*! 指示显示哪一帧图片 */
    uint8 frame_id;
    /*! 显示模式，分为普通模式，透明图标，垂直滚屏 */
    uint8 reserve;
} picbox_private_t;

/*!
 * \brief
 *      textbox_private_t textbox私有数据结构
 */
typedef struct
{
    /*! 字符串资源ID */
    uint16 str_id;
    /*! str_value字符串的语言类型 */
    int8 lang_id;
    /*! 保留字节 */
    uint8 reserve;
    /*! 字符串指针，如果非NULL，用str_value显示 */
    uint8 *str_value;
    /*! 字符串显示颜色，高16bit为0x5a5a表示用户自定义颜色，低16bit为16位颜色值 */
    COLORREF color;
} textbox_private_t;

/*!
 * \brief
 *  time_disp_mode_e 时间显示模式枚举类型
 */
typedef enum
{
    /*! 日期显示：YYMMDD */
    DATE_DISP_YYMMDD    = 0,
    /*! 日期显示：MMDDYY */
    DATE_DISP_MMDDYY    = 1,
    /*! 日期显示：YYMM */
    DATE_DISP_YYMM      = 2,
    /*! 日期显示：MMDD */
    DATE_DISP_MMDD      = 3,
    /*! 时间显示：HHMMSS */
    TIME_DISP_HHMMSS    = 4,
    /*! 时间显示：HHMM */
    TIME_DISP_HHMM      = 5,
    /*! 时间显示：MMSS */
    TIME_DISP_MMSS      = 6
} time_disp_mode_e;

/*!
 * \brief
 *      time_timebox_t hour 为uint16版本time_t
 */
typedef struct
{
    /*! 小时 */
    uint16 hour;
    /*! 分钟 */
    uint8  minute;
    /*! 秒钟 */
    uint8  second;
} time_timebox_t;

/*!
 * \brief
 *      timebox_private_t timebox私有数据结构
 */
typedef struct
{
    /*! 自定义time结构，时分秒或年月日的数值 */
    struct
    {
        /*! 时间联合体 */
        union
        {
            date_t date;
            time_timebox_t time;
        } union_time;
        /*! time类型：'D'表示日期，'T'表示时间，其他强制为时间 */
        char time_type;
        /*! 时间显示模式 */
        time_disp_mode_e mode;
        /*! 保留字节 */
        uint8 reserve;
    } times[2];
}timebox_private_t;

/*!
 * \brief
 *      numbox_private_t numbox私有数据结构
 */
typedef struct
{
    /*! 要显示的数值，其小数点位置由配置信息决定 */
    int32 value;
    /*! 总数，以查看当前数值所处的位置 */
    int32 total;
} numbox_private_t;

/*!
 * \brief
 *      progressbar_private_t progressbar私有数据结构
 */
typedef struct
{
    /*! 进度条当前进度（数值，由progressbar内部转换为步数） */
    uint32 value;
    /*! 进度条总进度 */
    uint32 total;
} progressbar_private_t;

/*!
 * \brief
 *      listbox_private_t listbox私有数据结构
 */
typedef struct
{
    /*! 标题字符串 */
    string_desc_t title;
    /*! 列表项字符串数组 */
    string_desc_t items[LIST_NUM_ONE_PAGE_MAX];
    /*! 非激活项前置图标列表首项ID */
    uint16 head_id;
    /*! 激活项前置图标列表首项ID */
    uint16 head_act_id;
    /*! 列表项items中有效前几项 */
    uint16 item_valid;
    /*! 当前激活项 */
    uint16 active;
    /*! 之前激活项 */
    uint16 old;
    
    /*! 应用中所有项总数，用于滑动杆和ratio */
    uint16 list_total;
    /*! 当前激活项在应用中所有项的位置，用于滑动杆和ratio */
    uint16 list_no;
} listbox_private_t;

/*!
 * \brief
 *      dialog_private_t dialog私有数据结构
 */
typedef struct
{
    /*! 详细说明字符串联合体 */
    string_desc_t *string_desc;
    /*! 对话框Icon ID */
    uint16 icon_id;
    /*! 按钮数组，最多支持3个按钮 */
    uint16 buttons[3];
    /*! 按钮数目，最多3个 */
    uint8 button_cnt;
    /*! 当前激活按钮序号 */
    uint8 active;
    /*! 之前激活按钮序号 */
    uint8 old;
} dialog_private_t;

typedef enum
{
    /*! 空结果，没有调整 */
    ADJUST_RESULT_NULL      = 0,
    /*! 调整自身参数值 */
    ADJUST_RESULT_VALUE     = 1,
    /*! 调整任意参数值，需要重新刷所有参数值 */
    ADJUST_RESULT_ALL       = 2,
} adjust_result_e;

/*!
 * \brief
 *      parambox_one_t parambox参数描述符结构体
 */
typedef struct parambox_one_struct
{
    /*! 参数单位资源字符串ID，其实也不限于单位，可以是任意辅助说明字符串 */
    uint16 unit_id;
    /*! 参数值最小值 */
    int16 min;
    /*! 参数值最大值 */
    int16 max;
    /*! 参数值步进 */
    int16 step;
    /*! 参数当前值 */
    int16 value;
    /*! 是否允许循环设置，即最大递增变为最小，最小递减变为最大 */
    uint8 cycle;
    /*! 参数值最大位数 */
    uint8 max_number;
    /*! 参数当前值字符串显示，可以通过adjust_func转换 */
    uint8 *value_str;
    /*! 参数值检测适配回调函数，比如用来限制日期设置等；这里传递进来的是
     *  参数数组，我们假设在多参数设置中，参数位置是固定的，或者是可以通
     *  过全局变量获得的；
     */
    adjust_result_e (*adjust_func)(struct parambox_one_struct *params, uint8 active, bool inc);
    /*! 设置即时回调函数，比如声音设置等 */
    bool (*callback)(int16 value);
} parambox_one_t;

/*!
 * \brief
 *      parambox_private_t parambox私有数据结构
 */
typedef struct
{
    /*! 背景图片ID，允许用户更换模板背景图片 */
    uint16 back_id;
    /*! 标志图标ID */
    uint16 icon_id;
    /*! 设置标题资源字符串ID */
    uint16 title_id;
    /*! 设置参数个数 */
    uint8 param_cnt;
    /*! 当前激活项，也作为多参数设置时默认激活项 */
    uint8 active;
    /*! 之前激活项 */
    uint8 old;
    /*! 是否显示参数值符号，0表示不显示，1表示显示 */
    uint8 sign_flag;
    /*! 刻度像素点值，仅对slider有效 */
    uint8 scale;
    uint8 reserve;
    /*! 设置参数列表 */
    parambox_one_t *items;
} parambox_private_t;

/*!
 * \brief
 *      res_type_e 资源类型数据结构
 */
typedef enum
{
    /*! 资源类型为AP资源 */
    UI_AP = 0, 
    /*! 资源类型为COMMON资源 */ 
    UI_COM = 1  
} res_type_e;

/*!
 * \brief
 *      style_infor_t 控件的style信息
 */
typedef struct
{
    /*! 要显示的控件在资源文件中的style ID */
    uint16 style_id;   
    /*! 资源类型 */
    res_type_e type;
} style_infor_t;

/*!
 * \brief
 *      scroll_mode_t 滚屏模式
 */
typedef struct
{
    /*! 模式，TRUE or FALSE */
    uint16 mode;
    /*! 显示模式，见字符串UI显示效果定义*/
    uint16 disp_mode; 
} scroll_mode_t;

/*!
 * \brief
 *      scroll_content_t 要滚屏的字符串及图标信息（music应用ID3滚屏专用）
 */
typedef struct
{
    /*! 要滚屏的字符串指针*/
    uint8 *strbuf;
    /*! 当前字符编码类型*/
    int8  language;
    /*! 保留字节 */
    uint8 reserve[3];
} scroll_content_t;

/*!
 * \brief
 *      scroll_param_t 滚屏参数（music应用ID3滚屏专用）
 */
typedef struct
{  
    /*! 是否为初始化滚屏*/
    uint8 init;
    /*! 是否刷新显示，仅在滚屏初始化时有效 */
    uint8 update;
    /*! 当前活动项*/     
    uint8 active; 
    /*! 滚屏方向*/
    uint8 direct; 
    /*! ID3 textbox */
    style_infor_t *id3_text;
    /*! ID3 picbox multi frame */
    style_infor_t *id3_icon;
} scroll_param_t;


/*!
 * \brief
 *  str_getlen_result_t 获取字符串长度附带结果结构体，对阿拉伯和希伯莱显示意义重大
 */
typedef struct
{
    /*! 刚好未超出 max_length 字符串长度，从头到此这部分字符串当作单行字符串显示 */
    uint16 true_length;
    /*! 以上部分字符串字节数，从头到此这部分字符串当作单行字符串显示 */
    uint16 true_bytes;
} str_getlen_result_t;


/*定义USB的数据结构*/
/*!
 *  \brief
 *  资源的长宽信息
 */
typedef struct
{
    uint16 width;
    uint16 height;
}res_size_t;
/*!
 * \brief
 *  ui_cmd_e ui驱动命令类型枚举类型
 */
typedef enum
{
    /*! 打开资源文件*/
    UI_RES_OPEN = 0,
    /*! 关闭资源文件*/
    UI_RES_CLOSE,
    /*! 显示PicBox*/
    UI_SHOW_PICBOX,             
    /*! 显示TextBox*/
    UI_SHOW_TEXTBOX,            
    /*! 显示TimeBox*/
    UI_SHOW_TIMEBOX,
    /*! 显示NumBox*/
    UI_SHOW_NUMBOX,
    /*! 显示ProgressBar*/
    UI_SHOW_PROGRESSBAR,
    /*! load AttributeBox的数据*/
    UI_LOAD_ATTRIBUTEBOX,
    /*! 显示ListBox*/       
    UI_SHOW_LISTBOX,
    /*! 显示DialogBox*/ 
    UI_SHOW_DIALOG,
    /*! 显示ParamBox*/ 
    UI_SHOW_PARAMBOX,
    /*! 显示sliderbar*/
    UI_SHOW_SLIDERBAR,
    /*! 获取textbox区域参数*/
    UI_GET_TEXTBOX_ATTRB,
    /*! 获取picbox属性*/
    UI_GET_PICBOX_ATTRB,
    /*! 获取listbox属性 */
    UI_GET_LISTBOX_ATTRB,
    /*! ID3滚屏专用函数*/ 
    UI_SCROLLSTRING_EXT,
    /*! 设置语言种类*/            
    UI_SET_LANGUAGE,
    /*! 获取DC状态*/
    UI_GET_DC_STATUS,
    /*! 清屏*/
    UI_CLEARSCREEN,
    /*! 设置画笔颜色*/
    UI_SET_PEN_COLOR,
    /*! 设置背景颜色*/
    UI_SET_BACKGD_COLOR,
    /*! 显示字符串*/
    UI_SHOW_STRING,
    /*! 字符串整理为“部分...”形式*/
    UI_ELLIPSIS_TO_LONGSTR,
    /*! 获取字符串的长度*/
    UI_GET_STRING_LENGTH, 
    /*! 文本解码断行，包括过滤掉空白字符和分词显示等 */
    UI_GET_TEXT_LINE,
    /*! 设置显示BUFFER的地址及大小*/
    UI_SET_DISPLAYBUF,
    /*! Unicode转换成内码*/
    UI_UNICODE_TO_CHAR,
    /*! 内码转换成Unicode*/
    UI_CHAR_TO_UNICODE,
    /*! 获取多国语言字符串的UNICODE编码*/
    UI_GET_MULTI_STRING_UNICODE,
    /*! UTF8编码转换成UNICODE*/
    UI_UTF8_TO_UNICODE,
    /*! 设置横屏或竖屏模式*/
    UI_SET_SCREEN_DIRECTION,
    /*! 显示资源图片*/
    UI_SHOW_PIC,
    /*! 将资源图片读到buffer中*/
    UI_READ_PIC_TO_BUFFER,
    /*! 清除font缓冲区 */
    UI_FLUSH_FONT_BUFFER,
    /*! 语言特殊选项 */
    UI_SET_LANG_OPTION,
} ui_cmd_e;

/*!
 * \brief
 *      textbox_draw_mode_e 文本框绘制模式枚举类型
 */
typedef enum
{
    /*! 无需绘制 */
    TEXTBOX_DRAW_NULL         = 0,
    /*! 以普通方式显示TEXTBOX */
    TEXTBOX_DRAW_NORMAL       = 1,
    /*! 以普通方式显示TEXTBOX，但不进行滚屏初始化 */
    TEXTBOX_DRAW_NOSCROLL     = 2,
    /*! 标题+内容 */
    TEXTBOX_DRAW_ITEM         = 3,
    /*! 滚屏显示：普通模式 */
    TEXTBOX_DRAW_SCROLL       = 4,
    /*! 滚屏显示：标题+内容，只滚内容 */
    TEXTBOX_DRAW_ITEM_SCROLL  = 5,
    /*! ID3初始化水平滚屏专用 */
    TEXTBOX_DRAW_ID3_HORIZONTAL    = 6,
    /*! ID3初始化垂直滚屏专用 */
    TEXTBOX_DRAW_ID3_VERTICAL      = 7,
} textbox_draw_mode_e;

/*!
 * \brief
 *      list_draw_mode_e 列表控件绘制模式枚举类型
 */
typedef enum
{
    /*! 无需绘制 */
    LIST_DRAW_NULL      = 0,
    /*! 重绘激活项 */
    LIST_DRAW_ACTIVE    = 1,
    /*! 重绘整个列表 */
    LIST_DRAW_LIST      = 2,
    /*! 重绘全部 */
    LIST_DRAW_ALL       = 3,
    /*! 滚动列表头 */
    LIST_DRAW_TITLE_SCROLL = 4,
    /*! 滚动激活项 */
    LIST_DRAW_ACTIVE_SCROLL = 5,
} list_draw_mode_e;
/*!
 * \brief
 *      dialog_draw_mode_e 对话框控件绘制模式枚举类型
 */
typedef enum
{
    /*! 无需绘制 */
    DIALOG_DRAW_NULL        = 0,
    /*! 重绘按钮 */
    DIALOG_DRAW_BUTTON      = 1,
    /*! 重绘对话框 */
    DIALOG_DRAW_ALL         = 2,
} dialog_draw_mode_e;

/*!
 * \brief
 *      parambox_draw_mode_e 参数设置控件绘制模式枚举类型
 */
typedef enum
{
    /*! 无需绘制 */
    PARAMBOX_DRAW_NULL      = 0,
    /*! 重绘参数值 */
    PARAMBOX_DRAW_VALUE     = 1,
    /*! 重绘参数（更换参数） */
    PARAMBOX_DRAW_PARAM     = 2,
    /*! 重绘参数设置框 */
    PARAMBOX_DRAW_ALL       = 3,
} parambox_draw_mode_e;

/*!
 * \brief
 *      progress_draw_mode_e 进度条控件绘制模式枚举类型
 */
typedef enum
{
    /*! 无需绘制 */
    PROGRESS_DRAW_NULL           = 0,
    /*! 重绘进度 */
    PROGRESS_DRAW_PROGRESS       = 1,
    /*! 音量条专用重绘进度，左箭头初始化 */
    PROGRESS_DRAW_LEFT_INIT      = 2,
    /*! 音量条专用重绘进度，左箭头更新 */
    PROGRESS_DRAW_PROGRESS_LEFT  = 3,
    /*! 音量条专用重绘进度，右箭头初始化 */
    PROGRESS_DRAW_RIGHT_INIT     = 4,
    /*! 音量条专用重绘进度，右箭头 */
    PROGRESS_DRAW_PROGRESS_RIGHT = 5,
    /*! 重绘全部 */
    PROGRESS_DRAW_ALL            = 6,
} progress_draw_mode_e;

/*!
 * \brief
 *      numbox_draw_mode_e 数字控件绘制模式枚举类型
 */
typedef enum
{
    /*! 无需绘制 */
    NUMBOX_DRAW_NULL        = 0,
    /*! 重绘数字 */
    NUMBOX_DRAW_NUMBER      = 1,
    /*! 重绘全部 */
    NUMBOX_DRAW_ALL         = 2,
} numbox_draw_mode_e;

/*!
 * \brief
 *      timebox_draw_mode_e 时间控件绘制模式枚举类型
 */
typedef enum
{
    /*! 无需绘制 */
    TIMEBOX_DRAW_NULL       = 0,
    /*! 重绘时间或日期 */
    TIMEBOX_DRAW_TIME       = 1,
    /*! 重绘全部 */
    TIMEBOX_DRAW_ALL        = 2,
} timebox_draw_mode_e;


/*!
 * \brief
 *  16bit 字符串显示模式格式：
 *  0-1      | 2-3      | 4-7          | 8-9      | 10-11 | 12       | 13       | 14           | 15
 *  填充模式 | 对齐方式 | 多行显示模式 | 滚屏属性 | 字号  | 自动分词 | 取消显示 | 语言列表菜单 | 取消阿拉伯属性
 */
#define DISP_ALIGN_OPTION_SHIFT     (2)
#define DISP_ALIGN_OPTION           (0x3 << DISP_ALIGN_OPTION_SHIFT)
#define DISP_ALIGN_LEFT             (0 << DISP_ALIGN_OPTION_SHIFT)//居左（默认左对齐输出）
#define DISP_ALIGN_MEDIACY          (1 << DISP_ALIGN_OPTION_SHIFT)//居中
#define DISP_ALIGN_RIGHT            (2 << DISP_ALIGN_OPTION_SHIFT)//居右

#define DISP_LONGSTR_OPTION_SHIFT   (4)
#define DISP_LONGSTR_OPTION         (0xf << DISP_LONGSTR_OPTION_SHIFT)
#define DISP_LONGSTR_RECT           (0 << DISP_LONGSTR_OPTION_SHIFT)//默认模式，显示不完整字符
#define DISP_LONGSTR_NO_RECT        (1 << DISP_LONGSTR_OPTION_SHIFT)//丢弃不完整字符
#define DISP_LONGSTR_MULTI_ROW      (2 << DISP_LONGSTR_OPTION_SHIFT)//多行显示
#define DISP_LONGSTR_ELLIPSIS       (3 << DISP_LONGSTR_OPTION_SHIFT)//加省略号显示
#define DISP_LONGSTR_SCROLL         (4 << DISP_LONGSTR_OPTION_SHIFT)//滚屏显示

#define DISP_SCROLL_OPTION_SHIFT    (8)
#define DISP_SCROLL_OPTION          (0x3 << DISP_SCROLL_OPTION_SHIFT)
#define DISP_SCROLL_HORIZONTAL      (0 << DISP_SCROLL_OPTION_SHIFT)//默认属性，水平滚屏
#define DISP_SCROLL_VERTICAL        (1 << DISP_SCROLL_OPTION_SHIFT)//向上滚屏
#define DISP_SCROLL_REPEAT          (2 << DISP_SCROLL_OPTION_SHIFT)//循环滚屏，也是水平滚屏，首尾相接，中间插入2空格符

//ID3滚屏类型
#define ID3_SCROLL_HORIZONTAL       (0)//ID3水平滚屏
#define ID3_SCROLL_VERTICAL         (1)//ID3垂直滚屏

#define DISP_FONT_OPTION_SHIFT      (10)
#define DISP_FONT_OPTION            (0x3 << DISP_FONT_OPTION_SHIFT)
#define DISP_FONT_NORMAL            (0 << DISP_FONT_OPTION_SHIFT)//默认字体，中号字体
#define DISP_FONT_LARGE             (1 << DISP_FONT_OPTION_SHIFT)//大号字体
#define DISP_FONT_SMALL             (2 << DISP_FONT_OPTION_SHIFT)//小号字体

#define DISP_DIVIDE_WORD_SHIFT      (12)
#define DISP_DIVIDE_WORD_ENABLE     (1 << DISP_DIVIDE_WORD_SHIFT)//自动分词

#define DISP_DISPLAY_DISABLE_SHIFT  (13)
#define DISP_DISPLAY_DISABLE        (1 << DISP_DISPLAY_DISABLE_SHIFT)//取消显示，仅处理

#define DISP_LANG_LIST_MENU_SHIFT   (14)
#define DISP_LANG_LIST_MENU         (1 << DISP_LANG_LIST_MENU_SHIFT)//语言列表菜单选项，需要特殊处理

#define DISP_ARABIC_DISABLE_SHIFT   (15)
#define DISP_ARABIC_DISABLE         (1 << DISP_ARABIC_DISABLE_SHIFT)//取消阿拉伯属性

#define DISP_CHAR_FILL_OPTION       (0x3 << 0)

/*!
 * \brief
 *  字符填充模式，透明模式优先级为 CHAR_FILL_NORMAL > CHAR_FILL_POINT，滚屏模式下转为 CHAR_FILL_SDIMAGE；
 *  如果lcd不支持GRAM读命令，请把ui 驱动中模式赋值 CHAR_FILL_NORMAL 全部换成 CHAR_FILL_POINT
 */
typedef enum
{
    /*! 普通模式，从GRAM读出字符串背景，再打点叠上去 */
    CHAR_FILL_NORMAL   = 0,
    /*! 非透明模式，显示区域内，字点填前景色，空白点填背景色 */
    CHAR_FILL_COLOR    = 1,
    /*! 直接读图模式，要求字符串背景是列优先存储，可以从SD读回
     *  任意字符区域背景图片；读回后同 NORMAL 一样处理。
     */
    CHAR_FILL_SDIMAGE  = 2,
    /*! 直接打点模式（透明），不读回背景图片，字点直接打前景色，空白点跳过 */
    CHAR_FILL_POINT    = 3,
} char_fill_mode_e;

/*!
 * \brief
 *  电子书断行结束类型
 */
typedef enum
{
    /*! 初始断行结束类型 */
    TEXT_END_INIT         = 0,
    /*! 缺页结束 */
    TEXT_END_PAGE_MISS    = 1,
    /*! 遇到 '\0' 字符结束 */
    TEXT_END_NUL          = 2,
    /*! 遇到换行符结束 */
    TEXT_END_LR           = 3,
    /*! 长度已经超长结束 */
    TEXT_END_OVER         = 4,
    /*! 读取文本数据失败 */
    TEXT_END_READ_ERR     = 5,
} text_end_mode_e;

/*!
 * \brief
 *  特殊语言选项
 */
/*! 语言列表菜单选项，需要根据语言类型进行特殊处理，以保证任何语言都能看到一致效果 */
#define  LANG_LIST_MENU_OPTION  (0x01 << 0)
#define  LANG_LIST_MENU_ENTER   (0x01 << 0)
#define  LANG_LIST_MENU_EXIT    (0x00 << 0)

/*!
 * \brief
 *  字符编码类型定义
 */
/*! 字符资源为unicode资源，泛指unicode data 和 unicode ID */ 
#define  UNICODELANGUAGE  0x7f    
/*! 字符资源为unicode资源ID */ 
#define  UNICODEID        0x7e   
/*! 字符资源为unicode data */ 
#define  UNICODEDATA      0x7d 
/*! 字符资源为utf-8 data */ 
#define  UTF_8DATA        0x7c  
/*! 字符资源为ansi data，语言类型自动处理为ui 驱动当前语言 */ 
#define  ANSIDATAAUTO     0x7b    
                               

/*!
 * \brief
 *  基本颜色表
 */
#define  Color_BLACK_def        0x00000000//黑      --作为通用的背景颜色
#define  Color_WHITE_def        0x0000ffff//白      --作为通用的画笔颜色
#define  Color_GRAY_def         0x0000bdf7//灰191
#define  Color_RED_def          0x0000f800//红
#define  Color_YELLOW_def       0x0000e748//黄
#define  Color_GREEN_def        0x000007e0//绿
#define  Color_BULE_def         0x0000001f//蓝
#define  Color_BULE_Dark_def    0x0000753E//深蓝
#define  Color_BULE_Light_def   0x0000335f//浅蓝
#define  Color_ORANGE_def       0x0000fcc0//橙
#define  Color_CYAN_def         0x00008cc1//青
#define  Color_Gold_def         0x0000f488//金黄色
#define  Color_Red_Dark_def     0x00003800//暗红色
/*!
 * \brief
 *   可参照 windows 画笔的颜色表
 */
#define  Color_tab0101_def   Color_BLACK_def
#define  Color_tab0102_def   0x00008410
#define  Color_tab0103_def   0x00008000
#define  Color_tab0104_def   0x00008400
#define  Color_tab0105_def   0x00000400
#define  Color_tab0106_def   0x00000410
#define  Color_tab0107_def   0x00000010
#define  Color_tab0108_def   0x00008010
#define  Color_tab0109_def   0x00008408
#define  Color_tab0110_def   0x00000208
#define  Color_tab0111_def   0x0000041f
#define  Color_tab0112_def   0x00000210
#define  Color_tab0113_def   0x0000401f
#define  Color_tab0114_def   0x00008200
#define  Color_tab0201_def   Color_WHITE_def
#define  Color_tab0202_def   0x0000c618
#define  Color_tab0203_def   Color_RED_def
#define  Color_tab0204_def   0x0000ffe0
#define  Color_tab0205_def   Color_GREEN_def
#define  Color_tab0206_def   0x000007ff
#define  Color_tab0207_def   Color_BULE_def
#define  Color_tab0208_def   0x0000f81f
#define  Color_tab0209_def   0x0000fff0
#define  Color_tab0210_def   0x000007f0
#define  Color_tab0211_def   0x000087ff
#define  Color_tab0212_def   0x0000841f
#define  Color_tab0213_def   0x0000f810
#define  Color_tab0214_def   0x0000fc08


//ui显示接口函数声明
extern void *ui_op_entry(void *param1, void *param2, void *param3,ui_cmd_e cmd) __FAR__;

/*!
 * \brief
 *   打开资源文件
 */
#define ui_res_open(filename,type)                     \
ui_op_entry((void*)(filename), (void*)(type), (void*)(0), UI_RES_OPEN)

/*!
 * \brief
 *   关闭资源文件
 */
#define ui_res_close(type)                             \
ui_op_entry((void*)(type), (void*)(0), (void*)(0), UI_RES_CLOSE)

/*!
 * \brief
 *   ID3滚屏
 */
#define ui_scroll_string_ext(infor,param)  \
ui_op_entry((void*)(infor), (void*)(param), (void*)(0), UI_SCROLLSTRING_EXT)

/*!
 * \brief
 *   显示picbox
 */
#define ui_show_picbox(style,data)                     \
(ui_result_e)(int)ui_op_entry((void*)(style), (void*)(data), (void*)(0), UI_SHOW_PICBOX)

/*!
 * \brief
 *   显示textbox
 */
#define ui_show_textbox(style,data,mode)               \
(ui_result_e)(int)ui_op_entry((void*)(style), (void*)(data), (void*)(uint32)(mode), UI_SHOW_TEXTBOX)

/*!
 * \brief
 *   显示timebox
 */
#define ui_show_timebox(style,data,mode)               \
(ui_result_e)(int)ui_op_entry((void*)(style), (void*)(data), (void*)(uint32)(mode), UI_SHOW_TIMEBOX)

/*!
 * \brief
 *   显示numbox
 */
#define ui_show_numbox(style,data,mode)                \
(ui_result_e)(int)ui_op_entry((void*)(style), (void*)(data), (void*)(uint32)(mode), UI_SHOW_NUMBOX)

/*!
 * \brief
 *   显示progressbar
 */
#define ui_show_progressbar(style,data, mode)          \
(ui_result_e)(int)ui_op_entry((void*)(style), (void*)(data), (void*)(uint32)(mode), UI_SHOW_PROGRESSBAR)

/*!
 * \brief
 *   获取attributebox的数据信息
 */
#define ui_load_attributebox(style,data, count)        \
(ui_result_e)(int)ui_op_entry((void*)(style), (void*)(data), (void*)(uint32)(count), UI_LOAD_ATTRIBUTEBOX)

/*!
 * \brief
 *   显示listbox
 */
#define ui_show_listbox(style,data,mode)               \
(ui_result_e)(int)(ui_result_e)(int)ui_op_entry((void*)(style), (void*)(data), (void*)(uint32)(mode), UI_SHOW_LISTBOX)

/*!
 * \brief
 *   显示dialogbox
 */
#define ui_show_dialogbox(style,data,mode)             \
(ui_result_e)(int)ui_op_entry((void*)(style), (void*)(data), (void*)(uint32)(mode), UI_SHOW_DIALOG)

/*!
 * \brief
 *   显示parambox
 */
#define ui_show_parambox(style,data,mode)              \
(ui_result_e)(int)ui_op_entry((void*)(style), (void*)(data), (void*)(uint32)(mode), UI_SHOW_PARAMBOX)

/*!
 * \brief
 *   显示sliderbar
 */
#define ui_show_sliderbar(style,data,mode)              \
(ui_result_e)(int)ui_op_entry((void*)(style), (void*)(data), (void*)(uint32)(mode), UI_SHOW_SLIDERBAR)

/*!
 * \brief
 *   获取textbox的属性
 */
#define ui_get_textbox_attrb(style,attrb,type)         \
(ui_result_e)(int)ui_op_entry((void*)(style), (void*)(attrb), (void*)(type), UI_GET_TEXTBOX_ATTRB)

/*!
 * \brief
 *   获取picbox的属性
 */
#define ui_get_picbox_attrb(style,attrb,type)          \
(ui_result_e)(int)ui_op_entry((void*)(style), (void*)(attrb), (void*)(type), UI_GET_PICBOX_ATTRB)

/*!
 * \brief
 *   获取listbox的属性
 */
#define ui_get_listbox_attrb(style,attrb,type)         \
(ui_result_e)(int)ui_op_entry((void*)(style), (void*)(attrb), (void*)(type), UI_GET_LISTBOX_ATTRB)

/*!
 * \brief
 *   语言设置
 */
#define ui_set_language(lang_id)                       \
ui_op_entry((void*)(uint32)(lang_id), (void*)(0), (void*)(0), UI_SET_LANGUAGE)

/*!
 * \brief
 *   获取DC状态
 */
#define ui_get_DC_status(pdc)                          \
ui_op_entry((void*)(pdc), (void*)(0), (void*)(0), UI_GET_DC_STATUS)

/*!
 * \brief
 *   清屏
 */
#define ui_clear_screen(clrregion)                     \
ui_op_entry((void*)(clrregion), (void*)(0), (void*)(0), UI_CLEARSCREEN)

/*!
 * \brief
 *   设置画笔颜色
 */
#define ui_set_pen_color(color)                        \
ui_op_entry((void*)(color), (void*)(0), (void*)(0), UI_SET_PEN_COLOR)

/*!
 * \brief
 *   设置背景颜色
 */
#define ui_set_backgd_color(color)                     \
ui_op_entry((void*)(color), (void*)(0), (void*)(0), UI_SET_BACKGD_COLOR)

/*!
 * \brief
 *   在指定区域显示字符串
 */
#define ui_show_string(infor, region, mode)             \
ui_op_entry((void*)(infor), (void*)(region), (void*)(uint32)(mode), UI_SHOW_STRING)

/*!
 * \brief
 *   字符串整理为“部分...”形式
 */
#define ui_ellipsis_to_longstr(desc, max, result)      \
ui_op_entry((void*)(desc), (void*)(uint32)(max), (void*)(uint32)(result), UI_ELLIPSIS_TO_LONGSTR)

/*!
 * \brief
 *   获取字符串的长度，Unicode编码字符串必须使用 UNICODELANGUAGE 作为语言类型
 */
#define ui_get_string_length(desc, max, result)        \
ui_op_entry((void*)(desc), (void*)(max), (void*)(result), UI_GET_STRING_LENGTH)

/*!
 * \brief
 *  分析字符串，专用于文本阅读
 */
#define ui_get_text_line(desc, mode, param)            \
ui_op_entry((void*)(desc), (void*)(uint32)(mode), (void*)(param), UI_GET_TEXT_LINE)

/*!
 * \brief
 *  设置显示buffer
 */
#define ui_set_display_buf(mode)                       \
ui_op_entry((void*)(uint32)(mode), (void*)(0), (void*)(0), UI_SET_DISPLAYBUF)

/*!
 * \brief
 *  unicode转内码
 */
#define ui_unicode_to_char(str,len,lang_id)            \
ui_op_entry((void*)(str), (void*)(uint32)(len), (void*)(int32)(lang_id), UI_UNICODE_TO_CHAR)

/*!
 * \brief
 *  内码转unicode
 */
#define ui_char_to_unicode(dest,src,len)               \
ui_op_entry((void*)(dest), (void*)(src), (void*)(uint32)(len), UI_CHAR_TO_UNICODE)

/*!
 * \brief
 * 获取多国语言字符串资源的unicode编码
 */
#define ui_get_multi_string_unicode(id,infor)          \
ui_op_entry((void*)(uint32)(id), (void*)(infor), (void*)(0), UI_GET_MULTI_STRING_UNICODE)

/*!
 * \brief
 * utf8转unicode
 */
#define ui_utf8_to_unicode(src,dest,size)              \
ui_op_entry((void*)(src), (void*)(dest), (void*)(size), UI_UTF8_TO_UNICODE)

/*!
 * \brief
 *  设置屏幕模式
 */
#define ui_set_screen_direction(screen_mode)           \
ui_op_entry((void*)(uint32)(screen_mode), (void*)(0), (void*)(0), UI_SET_SCREEN_DIRECTION)

/*!
 * \brief
 * 显示图片
 */
#define ui_show_pic(id,x,y)                            \
ui_op_entry((void*)(uint32)(id), (void*)(uint32)(x), (void*)(uint32)(y), UI_SHOW_PIC)

/*!
 * \brief
 * 读取图片信息保存到buffer中
 */
#define ui_read_pic_to_buffer(id,buffer,res_size)      \
ui_op_entry((void*)(uint32)(id), (void*)(buffer), (void*)(res_size), UI_READ_PIC_TO_BUFFER)

/*!
 * \brief
 * 清理字符点阵字模缓冲区
 */
#define ui_flush_font_buffer()                         \
ui_op_entry((void*)(0), (void*)(0), (void*)(0), UI_FLUSH_FONT_BUFFER)

/*!
 * \brief
 * 设置语言特殊选项
 */
#define ui_set_lang_option(mode)                       \
ui_op_entry((void*)(uint32)(mode), (void*)(0), (void*)(0),UI_SET_LANG_OPTION)

/*! \endcond */

#endif //_DISPLAY_H
