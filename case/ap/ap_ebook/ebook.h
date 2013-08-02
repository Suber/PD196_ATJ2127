/*******************************************************************************
 *                                      US212A
 *                                  Module:Ebook
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       stevenluo     2011-11-11    11:11     1.0             build this file
 *       liaojinsong   2012-08-01                           维护此文件
 *******************************************************************************/
/*!
 * \file     
 * \brief    电子书的配置、结构体定义及变量与函数声明
 * \author   stevenluo,liaojinsong
 * \version 1.0
 * \date  2012/08/01
 *******************************************************************************/
#ifndef   _EBOOK_H
#define   _EBOOK_H

#include  "psp_includes.h"
#include  "case_include.h"
#include  "ebook_res.h"
#include  "ebook_sty.h"
#include  "mmm_id.h"

/*!定义电子书应用的任务栈和主线程优先级*/
#define     AP_EBOOK_STK_POS    AP_FRONT_LOW_STK_POS    //ebook栈
#define     AP_EBOOK_PRIO       AP_FRONT_LOW_PRIO       //ebook优先级

#define     APP_TIMER_COUNT             1   

//定义ap定时器场景ID
#define    APP_TIMER_TAG_EBOOK          0x01            

/*!定义AP VM  变量的魔数值*/
#define     MAGIC_EBOOK                 0x55aa     //写入书签文件里，用来标记页数统计完毕
#define     MAGIC_COMVAL                0x55aa
/*!定义AP的编号*/
/*!定义option入口菜单的编号*/
#define     M_NOPLAYNOREAD_NOCARD       0       //无播放，无text阅读，无卡界面入口
#define     M_NOPLAYNOREAD_CARDEXIST    1       //无播放，无text阅读，有存在界面入口
#define     M_NOPLAYREAD_NOCARD         2       //无播放，有text阅读，无卡界面入口
#define     M_NOPLAYREAD_CARDEXIST      3       //无播放，有text阅读，有卡界面入口
#define     M_NOWPLAYNOREAD_NOCARD      4       //正在播放，无text阅读，无卡界面入口
#define     M_NOWPLAYNOREAD_CARDEXIST   5       //正在播放，无text阅读，有卡界面入口
#define     M_NOWPLAYREAD_NOCARD        6       //正在播放，有text阅读，无卡界面入口
#define     M_NOWPLAYREAD_CARDEXIST     7       //正在播放，有text阅读，有卡界面入口
#define     M_LPLAYNOREAD_NOCARD        8       //上次播放，无text阅读，无卡界面入口
#define     M_LPLAYNOREAD_CARDEXIST     9       //上次播放，无text阅读，有卡界面入口
#define     M_LPLAYREAD_NOCARD          10      //上次播放，有text阅读，无卡界面入口
#define     M_LPLAYREAD_CARDEXIST       11      //上次播放，有text阅读，有卡界面入口
#define     AP_MSG_RTC                  91

#define     VM_SYSTEM                   0x0000

#define     AUTOTIME_MAX        30              //自动播放的最大间隔时间(s)
#define     AUTOTIME_MIN        2               //自动播放的最小间隔时间(s)
#define     AUTOTIME_STEP       1               //自动播放的间隔时间每次增减步进量

/* 书签标题(title)的长度 */
#define NODELEN                 0x10
#define MAX_TITLE_LEN           12              // 12 bytes
#define MAX_BKMARK_NUM          0x10            // Max is 0x20
#define FILENMAELEN             12
#define FILESIZE                0x200           // file length
#define BUFSIZE                 0x200           // sector
#define PAGE_MAGIC              0x55aaaa55      // 用作写入书签文件里标记计算页数完毕

//#define FS_OPEN_NORMAL          0x01            //可以seek
//#define FS_OPEN_NOSEEK          0x00            //不可以seek
//#define FS_OPEN_MODIFY          0x02            //支持追加写
#define ONE_PAGE_ROW_NUM        9               //一屏显示文本的实际行数
#define MAX_FILTER_COUNT        50              //一次最多过滤的字符数
#define TEXT_ROW_MAX_COUNT      10              //一屏显示文本的最大行数

#define PLAY_ICON_WIDTH         20              //自动播放图标的大小(没用到)
#define PLAY_ICON_START         0               //自动播放图标的状态(没用到)

#define REGION_HEIGHT           16              //显示区域的高度
#define REGION_LENGTH           128             //显示区域的长度
#define BMK_NAME_LENGTH         64              //一次最多过滤的字符数


#define MAX_DEVICE_NUM          2               //定义最大可支持的盘符数目，目前只有Flash 和 SD 卡各一个 ，所以设置为2个

#define UNICODE_FLAG            0xFEFF          //UNICODE 格式编码的开头标记
#define UTF8_FLAG               0xBFBBEF        //UTF8 格式编码的开头标记

#define  SHOW_FIFLNAME          0               //显示文件名:1为显示，0为不显示。
                                                //可能需要修改:ONE_PAGE_ROW_NUM,
                                                //还需要修改UI的背景图片
#define  MAX_FILENAME_LENGTH    32              //文件名的最大长度
#define  MAX_BMK_FILE           60              //写到vram区的书签文件最多支持个数
#define  VRAM_END_ADDRESS       0x004FFFFF      //vram区可用于存储书签信息的最大地址
#define  MAX_FILE_LENGTH        0x01000000      //支持文本文件最大的长度16M

/*定义主菜单的场景*/
typedef enum
{
    ENTER_FROM_DESKTOP = 0,     //从main ap 或browser进入场景
    ENTER_FROM_READING,         //从阅读界面进入场景
    ENTER_FROM_FILELIST,        //从文件列表进入场景
    ENTER_FROM_SETTINGMENU,     //从设置菜单进入场景
    ENTER_FROM_READINGMENU,     //从阅读设置菜单进入场景
    ENTER_FROM_MENUDIR,         //从菜单目录进入场景
    ENTER_FROM_MAX
} scene_enter_mode_e;

/*定义后台播放模式*/
/*!
 *  \brief
 *  应用类型定义
 *  \li NOPLAY : 无播放记录
 *  \li PLAYING : 正在播放
 *  \li LASTPLAY : 上一次播放
 */
typedef enum
{
    NOPLAYING = 0, 
    LASTPLAY, 
    BMKMENU, 
    DESKTOP
} ebook_menu_type_e;

/*定义播放场景的ui消息事件*/
/*!
 *  \brief
 *  应用类型定义
 *  \li NEXTITEM : 下一文件
 *  \li PREVITEM : 上一文件
 *  \li OPTIONMENU : 菜单选项
 *  \li PIC_RETURNLIST : 文件列表菜单
 *  \li VOLUME: 音量调节
 */
typedef enum
{
    EBOOK_NEXTITEM = 0,
    EBOOK_PREVITEM,
    EBOOK_OPTIONMENU,
    EBOOK_RETURNLIST,
    EBOOK_VOLUME,
    EBOOK_DEFAULT,
    EBOOK_RETURN_BROWSER
} play_result_e;

typedef enum
{
    NORMAL_TYPE,    //正常模式，例如阅读界面
    BMK_SEL,        //选择书签
    BMK_DET,        //删除书签
    PAGE_SEL        //选择页数
} inlet_type_e;

/*定义主菜单应用的VM全局变量*/
/*!
 * \brief
 *  EBOOK的VM全局变量.
 */
typedef struct
{
    /*!VM 变量魔数*/
    uint16 magic;
    /*!播放模式标志，0:手动播放;1:自动播放*/
    uint8 play_flag;
    /*!自动播放的时间间隔:2-30秒*/
    uint8 autoplaytime;
    /*!当前电子书的总长度*/
    uint32 file_length;
    /*!文件退出应用时的页号*/
    uint32 page_num;
    /*电子书文件的播放路径信息*/
    file_path_info_t path;
} g_ebook_var_t;

/* 书签信息的数据结构 */
typedef struct
{
    UINT32 pagenum;                 // 书签在文本中的页号
    uint8 title[MAX_TITLE_LEN];     // 标题
} bkmark_node_t;

/* 信息类型(没用到) */
typedef enum
{
    SINGLE, DOUBLE
} info_type_e;

/* 对话框信息(没用到) */
typedef struct
{
    uint8* str_name;        // 标题
    uint8 info_type;        // 信息类型
    uint8 dialog_type;      // 对话框类型
    uint8 button_type;      // 按键类型
    uint16 option_id;       // 选项ID
    int8 language_id;       // 语音ID
} dialog_info_t;

typedef struct
{
    /*! 当前显示项目列表信息 */
    /*! 项目列表页号，仅在页模式下有效 */
    uint32 group;
    /*! 项目列表有效区域top项序号，相对整个列表的序号，值从1开始 */
    uint32 top;
    /*! 项目列表有效区域bottom项序号，相对整个列表的序号，值从1开始 */
    uint32 bottom;
    /*!项目列表的项目总数*/
    uint32 total;

    /*! 当前激活文件信息 */
    /*! 当前文件序号，相对整个列表的序号，值从0开始 */
    uint32 list_no;
    /*! 当前列表的激活项，值从1 ~ one_page_count*/
    uint32 active;
    /*! 当前列表旧的激活项，值从1 ~ one_page_count */
    uint32 old;

    /*! 文件列表更新模式，参见 list_update_mode_e 定义 */
    list_update_mode_e update_mode;
    /*! 列表控件绘制模式，参见 list_draw_mode_e 定义 */
    list_draw_mode_e draw_mode;
} pagenum_control_t;

/*!
 * \brief
 *      text_buf_t：文本阅读解码缓冲区结构体
 */
typedef struct
{
    /*! 一行字符串缓冲区 */
    /*! 缓冲区中有效数据长度 */
    uint16 text_buf_length;
    uint8 text_prev[BUFF_ONE_ROW];
    /*! 一个扇区缓冲区，512个字节 */
    uint8 text_buf[FILE_SECTOR];
    /*! 缓冲区中有效数据指针 */
    uint8 *text_buf_valid;

    /*! 缓冲区中剩余字节数 */
    uint8 remain;
    /*! buffer 数据是否有效 */
    bool valid;
    /*! 文本显示参数结构体 */
    text_show_param_t param;
    /*! text 显示行回调函数 */
    void (*text_show_line)(string_desc_t *string_desc, uint8 index);
} view_decode_t;

/*!
 * \brief
 *      text_file_t：文本阅读虚拟文件结构体，为了兼容SD文件和文件系统文件
 */
typedef struct
{
    /*! 打开的文件句柄 */
    uint32 file_handle;
    /*! 文件总扇区数 */
    uint32 file_sectors;
    /*! 文件总字节数 */
    uint32 file_length;
    /*！文件偏移地址，以字节为单位 */
    uint32 file_offset;
    int8 language;
    /*! 文件定位接口函数指针，以扇区为单位 */
    bool (*fseek)(uint32 sector_offset, uint32 file_handle);
    /*! 文件数据读接口函数指针，以扇区为单位 */
    bool (*fread)(uint8 *text_buf, uint32 file_handle);
} view_file_t;

typedef struct
{
    //文件首簇号
    uint32 cluster_no;
   //文件在该簇内的偏移
    uint32 dir_entry;
   //存储书签信息的开始地址
    uint32 startadd;
} vram_bmk_info_t;
typedef struct
{
    //书签文件名
    uint8 file_name[32];
   //页数计算是否完成标记
    uint32 decode_flag;
   //当前文件总页数
    uint32 totalpage;
   //当前文件计算页数的偏移
    uint32 decode_offset;
    uint32 reserve;
} decode_bmk_info_t;

extern bool numboxflag;                         //是否全刷numbox标记
extern g_ebook_var_t g_ebook_vars;              //应用存储变量结构体
extern comval_t g_comval;                       //全局系统变量
extern uint8 need_draw;                         //界面是否刷新标志，0-不需要刷新,1-需要刷新
extern uint8 auto_play_count;                   //自动播放时间统计
extern uint8 page_count_flag;                   //是否继续计算页数标记。0-继续计算，1-计算完毕
extern uint32 curpagenum;                       //当前阅读文本所处的页数
extern uint32 decode_sector;                    //当前正在阅读的页面在对应的文件中所处的扇区数
extern uint32 cur_file_diroffset;               //当前文本文件中目录信息中的目录偏移
extern view_decode_t view_decode _BANK_DATA_ATTR_;//文本阅读解码缓冲区结构体，主要用于阅读时使用
extern vram_bmk_info_t vram_bmk_info;
extern decode_bmk_info_t decode_bmk_info;
extern view_file_t view_file;

extern view_decode_t page_count_decode _BANK_DATA_ATTR_;

extern view_file_t page_count_file;
extern listbox_private_t bmklist_data _BANK_DATA_ATTR_;

//存储列表显示信息的数组
extern uint8 file_list[LIST_NUM_ONE_PAGE_MAX][FILENMAELEN]_BANK_DATA_ATTR_;

extern bkmark_node_t bkmarks[MAX_BKMARK_NUM] _BANK_DATA_ATTR_;
extern uint8 bmk_bufFile[FILE_SECTOR];
extern uint8 BOOKMARK_filename[BMK_NAME_LENGTH];

#if SHOW_FIFLNAME                                       //显示文件名
    extern uint8 BOOK_filename[MAX_FILENAME_LENGTH];
#endif

extern app_timer_t ebook_app_timer_vector[COMMON_TIMER_COUNT + APP_TIMER_COUNT];
extern uint8 page_count_flag;                   //是否计算继续计算页数标记。0-继续计算，1-不计算
extern uint32 page_totalnum;                    //总页数
extern int32 g_ebook_mount_id;                  //正在应用文件系统的索引号
extern uint16 bookmark_total;
extern uint32 file_offset;
extern uint32 decode_offset;
extern int32 cur_BufSector;
extern uint16 page_multiple_sel;
extern uint16 temp_count;
extern uint8 draw_page_num;
extern uint32 bmk_modify_handle;
extern uint8 draw_auto_flag;
extern uint8 bmk_modify_flag;
extern scene_enter_mode_e enter_mode;
extern engine_type_e engine_type;
extern uint8 is_reading_menu;
extern uint8 device_status;
extern uint8 music_play_device;
extern uint32 bmk_last_pagenum;
extern uint8 support_card_flag;
extern uint8 need_creatlist;
//ebook_main.c
app_result_e _ebook_scene_msg_dispatch(private_msg_t *private_msg)__FAR__;
int _ebook_scene_dispatch(int param);
app_result_e _ebook_init(void);
void _ebook_exit(void);
void _ebook_read_var(void);
app_result_e _ebook_filesel_init(void) __FAR__;
void _ebook_param_init(void);
void _deal_exit_result(app_result_e result);
bool _deal_return_val(app_result_e retval);
void _check_play_device(void);

//ebook_menu.c
app_result_e _ebook_option_menu(scene_enter_mode_e mode);
app_result_e _menu_func_nowplaying(void *param);
app_result_e _menu_func_lastplay(void *param);
app_result_e _menu_func_deleteebook(void *param);
app_result_e _menu_func_selbookmark(void *param);
app_result_e _menu_func_detbookmark(void *param);
app_result_e _menu_func_selbookmark(void *param);
app_result_e _menu_func_addbookmark(void * param);
app_result_e _menu_func_selbookpage(void *param);
app_result_e _menu_func_carddir(void *param);
app_result_e _menu_func_flashdir(void *param);
uint16 _check_bmk_list(void);
void _close_text_handle(void);

app_result_e _menu_func_autoplay(void *param);
//app_result_e _option_callback(dialog_info_t *dialog_info) __FAR__;
bool _set_autotime_callback(uint16 value);
uint8 _filterwhitespace(uint8 *str, int8 language);
app_result_e _show_double_dialog(uint16 option_id,uint8 *str_name);

//ebook_filelist.c
app_result_e _ebook_filelist_callback(file_path_info_t *path_info, uint16 active);
app_result_e _ebook_filelist(scene_enter_mode_e mode);


//ebook_reading.c
app_result_e _ebook_reading(scene_enter_mode_e mode);
app_result_e _ebook_text_read(void);
void _display_text(void);
void _sel_next_page(uint8 page_sel);
void _sel_prev_page(uint8 page_sel);
void _deal_rtc_msg(void);
void _update_text_head(void);
void _init_decode_param(void);
void _draw_autoplya_icon(void);
#if     SHOW_FIFLNAME
void _show_filename(void);
void _get_filename(uint8 *data,uint32 datalength);
#else
#define _show_filename()
#define _get_filename(data,datalength)
#endif

//ebook_message.c
extern app_result_e _ebook_msg_callback(private_msg_t *pri_msg) __FAR__;

//ebook_bmklist.c
app_result_e _bmk_list_enter(msg_cb_func msg_callback, inlet_type_e inlet_type);
app_result_e _deal_msg_result(input_gui_msg_t *ui_msg,dir_control_t *mlst_ctl,inlet_type_e inlet_type)__FAR__;

//ebook_bookmark.c
uint16 _get_bookmark_nums(inlet_type_e inlet_type);
void _del_bookmark(uint16 id);
bool _add_bookmark(void);
uint32 _get_bmk_page(uint16 index);
uint8 _count_num_bytes(uint32 num);

//ebook_bmklist_sub.c
app_result_e _deal_confirm_item(dir_control_t *mlst_ctl, inlet_type_e inlet_type);
void _select_next_item(dir_control_t *mlst_ctl, uint8 line_cnt);
void _select_prev_item(dir_control_t *mlst_ctl, uint8 line_cnt);
void _update_delete_item(dir_control_t *mlst_ctl);
void _bmklist_init(dir_control_t *mlst_ctl, uint16 total);
void _update_list_info(dir_control_t *dir_control, listbox_private_t *dir_private, inlet_type_e inlet_type);
uint8 *_get_bmk_title(uint8 index);

//ebook_fileoperate.c
bool _seek_file_sector(uint32 sector_offset, uint32 file_handle);
bool _read_file_sector(uint8 *viewfilebuf, uint32 file_handle);
uint32 _get_file_length(uint32 file_handle);
void _close_text_file(uint8 update_flag) __FAR__;
uint16 _init_bmk_info(void);
uint16 _init_file_info(void);
bool _get_bmk_firstsec_info(void);
void _writeback_bmk_info(void);

//ebook_fileoperate2.c
bool Is_utf8_nobom(uint8 * str,uint16 len);

//ebook_bmklist_sub.c
uint8 *_get_page_title(uint16 top, uint8 index);

//ebook_decode.c
text_end_mode_e _decode_one_page(view_decode_t *text_decode, view_file_t *text_file, uint32 *page_bytes) __FAR__;
bool _read_text(view_decode_t *text_decode, view_file_t *text_file);
void _show_text_line(string_desc_t *string_infor, uint8 line);
uint8 _filter_enter_line(string_desc_t *str_desc);

//下面的代码段存放在常驻代码段

//ebook_comfunc.c
void _clear_and_set_screen(COLORREF pencolor, COLORREF backcolor)__FAR__;   //全屏清屏
void _clear_and_set_text_screen(COLORREF pencolor, COLORREF backcolor)__FAR__;     //清屏文字显示部分
void _clear_and_set_head_screen(COLORREF pencolor, COLORREF backcolor)__FAR__;
void _show_single_dialog(uint16 option_id,uint8 diglog_type) __FAR__;
uint8 _get_bmk_name(uint16 filename_length) __FAR__;
app_result_e _deal_menu_result(void) __FAR__;


//ebook_residentcode.c
void _get_page_offset(uint32 pagenum)__FAR__;
void _count_page_num(void)__FAR__;
app_result_e _deal_key_msg(dir_control_t *mlst_ctl, inlet_type_e inlet_type) __FAR__;
bool _seek_and_read(uint32 seek_offset, uint32 read_size,uint8 *databuf) __FAR__;
bool _seek_and_write(uint32 seek_offset, uint32 write_size,uint8 *databuf) __FAR__;
bool _detect_device(uint8 device_type) __FAR__;
void _close_engine(void) __FAR__;

uint16 _vram_init_bmk_info(void);
void _vram_init_bmk_headinfo(uint8 file_index);
void _vram_wb_bmk_headinfo(void);
void _vram_write_bmkinfo(void)__FAR__;
void _vram_read_bmkinfo(void);
void _vram_write_data(uint32 address_offset);
void _vram_limit_file_length(void);


#ifdef PC
menu_cb_option _get_menu_option(uint16 str_id, uint16 str_id_sub);
menu_cb_func _get_menu_func(uint16 str_id, uint16 str_id_sub);
menu_cb_leaf _get_callback_func(uint16 str_id, uint16 str_id_sub);

#endif

#endif
