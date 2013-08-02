/*******************************************************************************
 *                              us211A
 *                            Module: ui驱动
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       cailizhen     2012-1-16 9:58     1.0             build this file
 *******************************************************************************/
/*!
 * \file     ui_driver_rom.h
 * \brief    ui驱动模块相关数据结构，宏定义，接口声明等（不可改动）
 * \author   cailizhen
 * \version 1.0
 * \date  2012-1-16
 *******************************************************************************/

#ifndef _UI_DRIVER_ROM_H
#define _UI_DRIVER_ROM_H

#include "psp_includes.h"
#include "display.h"

#define RES_ID_BITS     0x7fff              //资源ID号后12bit表示有效ID，前4bit做特殊用途
#define REAL_RES_ID(id) ((id) & RES_ID_BITS)    //获取真正资源ID
#define RES_TYPE_BIT    0x8000
#define RES_TYPE_COM    0x8000
#define RES_TYPE_AP     0x0000

//获取资源文件句柄：ROM代码编译采用O0优化，不会展开内联函数，所以直接用宏定义
#define res_ptr_select(res_id) ((((res_id) & RES_TYPE_BIT) == RES_TYPE_COM)? (&g_com_res): (&g_ap_res))

//字体大小
#define FONT_LARGE      2
#define FONT_NORMAL     1
#define FONT_LITTER     0

//设置lcd显示参数，0表示BMP图片，1表示区域填充，2表示字符串，3表示字符串滚屏
#define LCD_PARAM_IMAGE     0
#define LCD_PARAM_FILL      1
#define LCD_PARAM_STRING    2
#define LCD_PARAM_SCROLL    3

//显示原子字符
typedef enum
{
    /*! 结束字符 */
    CHAR_EOF = 0,
    /*! Unicode编码标志符号0xff, 0xfe */
    CHAR_UNI_FLAG = 1,
    /*! 换行符号 */
    CHAR_LR = 2,
    /*! 普通字符 */
    CHAR_NORMAL = 3,
    /*! 未决字符，即根据当前字节流，不能确定是何种字符 */
    CHAR_NOPEND = 4,
} char_type_e;

//显示字符模式
typedef enum
{
    /*! 完整显示字符 */
    CHAR_DISP_NORMAL = 0,
    /*! 指定开始点，从左到右显示到结尾 */
    CHAR_DISP_LEFT2RIGHT = 1,
    /*! 指定开始点，从右到左显示到开头 */
    CHAR_DISP_RIGHT2LEFT = 2,
} char_disp_e;

//单行字符串显示终止原因，放在 string_desc_t.result 中返回
typedef enum
{
    /*! 字符串有效长度用完 */
    STRING_TAIL_LEN_ZERO = 0,
    /*! 遇到字符串终止符或者未决符号 */
    STRING_TAIL_EOF_NOPEND = 1,
    /*! 遇到换行符 */
    STRING_TAIL_CHAR_LR = 2,
    /*! 显示区域越界 */
    STRING_TAIL_REGION_ERR = 3,
    /*! 其他错误 */
    STRING_TAIL_OTHER_ERR = 4,
} string_tail_e;

//存放显示数据缓冲区最多为16*160*2 = 5KB，以留一部分空间用作字模点阵缓冲区
#define DISP_MAX_WIDTH  160

//字模缓存区标志，"font"的 uint32 值
#define FONT_CACHE_MAGIC    0x746E6F66
#define FONT_ONE_SIZE       33//Unicode字符字模33字节长度
#define FONT_CYCLE_INDEX    4//替代循环变量相对地址
//字模缓存区字模n 地址
#ifndef PC
#define font_cache_char_addr(n) ((uint8 *)font_cache_addr + font_cache_header + FONT_ONE_SIZE * (n))
#else
#define font_cache_char_addr(n) (GET_REAL_ADDR(font_cache_addr) + font_cache_header + FONT_ONE_SIZE * (n))
#endif

/*!
 * \brief
 *  res_head_item_t：描述res_head的数据结构
 */
typedef struct
{
    /*! 相对于文件头的偏移 */
    uint32 offset;
    /*! 资源个数 */
    uint16 count;
    /*! 资源类型 */
    uint8 item_type;
    /*! 保留字段 */
    uint8 reserve[9];
} res_head_item_t;

/*!
 * \brief
 *  res_head_t：res文件头数据结构
 */
typedef struct
{
    /*! 标识，'R', 'U', '2', 0x19 */
    char magic[4];
    /*! 资源的个数 */
    uint16 counts;
    /*! 保留字段1 */
    uint8 reserve1[2];
    /*! sty信息的偏移地址 */
    uint32 style_addr;
    /*! 保留字段2 */
    uint8 reserve2[4];
    /*! 资源头items */
    res_head_item_t res_items[31];
    /*! 保留字段3 */
    char reserve3[512];
} res_head_t; //1024 bytes


/*!
 * \brief
 *  res_entry_t：资源索引表数据结构
 */
typedef struct
{
    /*! 图片数据区在文件内偏移,4 bytes */
    uint32 offset;
    /*! 资源长度, 最大 4G，4 bytes */
    uint32 length;
    /*! 资源类型,0x01--language string ,0x02--PIC */
    uint8 type;
    /*! 保留字段1 */
    uint8 reserve1[1];
    /*! 若是图片，则代表图片宽，若是字符串，则代表ID总数 */
    uint16 width;
    /*! 若是图片，则代表图片长，若是字符串，则代表该语言的ID */
    uint16 height;
    /*! 保留字段2 */
    uint8 reserve2[2];
} res_entry_t;//16 bytes


/*!
 * \brief
 *  res_open_infor_t：资源文件打开后保存在内存中的信息
 */
typedef struct
{
    /*! res文件句柄 */
    sd_handle res_fp;
    /*! 图片资源在文件内的偏移 */
    uint32 pic_entry_offset;
    /*! 字符串资源在文件内的偏移 */
    uint32 string_entry_offset;
    /*! style数据在文件内的偏移 */
    uint32 cur_style_begin_offset;
    /*! picbox数据在文件内的偏移 */
    uint32 style_picbox_offset;
} res_open_infor_t;

/*!
 * \brief
 *  res_langid_entry_t：多国语言资源ID索引表的数据结构
 */
typedef struct
{
    /*! 字符ID号对应字符串编码在文件内的偏移 */
    uint32 offset;
    /*! 字符串长度，即unicode编码字符串的字节数 */
    uint16 length;
    /*! 保留字段 */
    uint16 reserve;
} res_langid_entry_t; // 6B


extern res_open_infor_t g_ap_res; //记录ap资源信息
extern res_open_infor_t g_com_res; //记录common资源信息
extern sd_handle FontLib_fp; //字库文件句柄
extern sd_handle MBtoUniTab_fp; //多国语言转换表的句柄

extern DC dc; //DC变量
extern region_t region; //显示区域
extern region_t region_win;

#ifndef PC
extern uint32 display_buf_addr; //显示buffer地址
extern uint32 font_cache_addr;//字模缓冲区buffer
#else
extern DLL_IMP uint32 display_buf_addr;
extern DLL_IMP uint32 font_cache_addr;
#endif
extern uint16 display_buf_len; //显示buffer的长度
extern uint16 font_cache_len; //字模缓冲区大小，默认支持1KB
extern uint16 font_cache_header;//字模缓冲区头部大小，必须4字节对齐，默认为64B
extern bool font_cache_flag;//使用字模缓冲区标志

extern uint8 char_point_buffer[33]; //点阵字模 buffer

//ROM hook functions
void ramf_char_fill_hook(region_t *point_region, uint8 *trans_font);
bool ramf_dma_buffer_data_hook(uint32 pixel_cnt, uint16 data_width);
void ramf_read_str_bgpic_hook(uint8 *display_buffer, region_t *bgpic_region, char_fill_mode_e mode);
uint8 ramf_get_next_char_point_hook(uint8 *str, int8 language, uint8 *pbuffer);
char_type_e ramf_next_char_ansi_hook(uint8 *str, uint16 length, int8 language, uint8 *byte_cnt);
char_type_e ramf_next_char_unicode_hook(uint8 *str, uint16 length, int8 language, uint8 *byte_cnt);
void ramf_set_lcd_param_hook(region_t *win_region, uint8 mode);

//ROM hook functions typedef
typedef void (*__char_fill_hook)(region_t *point_region, uint8 *trans_font);
typedef bool (*__dma_buffer_data_hook)(uint32 pixel_cnt, uint16 data_width);
typedef void (*__read_str_bgpic_hook)(uint8 *display_buffer, region_t *bgpic_region, char_fill_mode_e mode);
typedef uint8 (*__get_next_char_point_hook)(uint8 *str, int8 language, uint8 *pbuffer);
typedef char_type_e (*__next_char_ansi_hook)(uint8 *str, uint16 length, int8 language, uint8 *byte_cnt);
typedef char_type_e (*__next_char_unicode_hook)(uint8 *str, uint16 length, int8 language, uint8 *byte_cnt);
typedef void (*__set_lcd_param_hook)(region_t *win_region, uint8 mode);

//ROM hook functions reference
extern __char_fill_hook char_fill_hook;
extern __dma_buffer_data_hook dma_buffer_data_hook;
extern __read_str_bgpic_hook read_str_bgpic_hook;
extern __get_next_char_point_hook get_next_char_point_hook;
extern __next_char_ansi_hook next_char_ansi_hook;
extern __next_char_unicode_hook next_char_unicode_hook;
extern __set_lcd_param_hook set_lcd_param_hook;

//ROM functions
bool romf_show_pic(uint16 id, uint16 x, uint16 y)__FAR__;
bool romf_put_sd_image(sd_handle *res_fp, uint32 img_addr, region_t *img_region)__FAR__;
bool romf_fill_rect(region_t *fill_region, uint16 color)__FAR__;
void romf_get_unicode_data(uint16 id, string_desc_t *desc)__FAR__;
void romf_black_char_to_color(uint8 *psoure_buf, region_t *buf_region, char_disp_e char_disp, uint8 start_col)__FAR__;
void romf_char_fill(uint8 *psoure_buf, char_disp_e char_disp, uint8 start_col)__FAR__;
void romf_read_str_bgpic(uint8 *display_buffer, region_t *bgpic_region, char_fill_mode_e mode)__FAR__;
char_type_e romf_get_next_char_type_unicode(uint8 *str, uint16 length, int8 language, uint8 *byte_cnt)__FAR__;
char_type_e romf_get_next_char_type_ansi(uint8 *str, uint16 length, int8 language, uint8 *byte_cnt)__FAR__;
uint16 romf_multi_char_to_unicode(uint16 code_rename)__FAR__;
void romf_get_font_point(uint16 code_rename, uint8 *picbuf)__FAR__;
void romf_get_next_char_point(uint16 uni_code, uint8 *pbuffer)__FAR__;
uint8 *romf_put_string_line(string_desc_t *desc, region_t *region, char_fill_mode_e mode)__FAR__;
uint16 romf_get_string_length(string_desc_t *desc, uint16 max_length, str_getlen_result_t *result)__FAR__;
bool romf_search_cache(uint16 uni_code, uint8 *cache_index)__FAR__;
void romf_get_DC_status(PDC pdc, void*null2, void *null3)__FAR__;
void romf_set_pen_color(COLORREF color, void *null2, void *null3)__FAR__;
void romf_set_backgd_color(COLORREF color, void*null2, void *null3)__FAR__;
void romf_set_text_pos(uint16 x, uint16 y)__FAR__;
void romf_get_text_pos(uint16 *x, uint16 *y)__FAR__;
bool romf_put_char(uint8 *psoure_buf, char_fill_mode_e mode, char_disp_e char_disp, uint8 start_col)__FAR__;

//ROM sub functions typedef
typedef bool (*__put_sd_image)(sd_handle *res_fp, uint32 img_addr, region_t *img_region);
typedef void (*__char_fill)(uint8 *psoure_buf, char_disp_e char_disp, uint8 start_col);
typedef void (*__black_char_to_color)(uint8 *psoure_buf, region_t *buf_region, char_disp_e char_disp, uint8 start_col);
typedef void (*__read_str_bgpic)(uint8 *display_buffer, region_t *bgpic_region, char_fill_mode_e mode);
typedef char_type_e (*__get_next_char_type_unicode)(uint8 *str, uint16 length, int8 language, uint8 *byte_cnt);
typedef char_type_e (*__get_next_char_type_ansi)(uint8 *str, uint16 length, int8 language, uint8 *byte_cnt);
typedef bool (*__search_cache)(uint16 uni_code, uint8 *cache_index);
typedef bool (*__put_char)(uint8 *psoure_buf, char_fill_mode_e mode, char_disp_e char_disp, uint8 start_col);

//ROM fix sub functions reference
extern __put_sd_image put_sd_image __FAR__;
extern __black_char_to_color black_char_to_color __FAR__;
extern __char_fill char_fill __FAR__;
extern __read_str_bgpic read_str_bgpic __FAR__;
extern __get_next_char_type_unicode get_next_char_type_unicode __FAR__;
extern __get_next_char_type_ansi get_next_char_type_ansi __FAR__;
extern __search_cache search_cache __FAR__;
extern __put_char put_char __FAR__;

#endif //_UI_DRIVER_ROM_H
