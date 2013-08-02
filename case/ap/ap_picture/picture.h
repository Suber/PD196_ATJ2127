/*******************************************************************************
 *                              这里填写项目名
 *                            Module: 这里填写模块名
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       zhangxs     2011-09-06 15:00     1.0             build this file
 *******************************************************************************/
/*
 * \file     这里请真实填写文件名字
 * \brief    这里填写文件的概述
 * \author   这里填写文件的作者
 * \version 1.0
 * \date  2009/5/4
 *******************************************************************************/
#ifndef   _PICTURE_H
#define   _PICTURE_H

#include  "psp_includes.h"
#include  "case_include.h"
#include  "picture_res.h"
#include  "picture_sty.h"
#include  "mmm_id.h"

#define FOR_TEST
/*!定义图片应用的任务栈和主线程优先级*/
#define    AP_PICTURE_STK_POS  AP_FRONT_LOW_STK_POS
#define    AP_PICTURE_PRIO     AP_FRONT_LOW_PRIO

/*! \brief 应用自己需要的软定时器个数 */
#define APP_TIMER_COUNT		1
#define APP_TIMER_TOTAL     (COMMON_TIMER_COUNT + APP_TIMER_COUNT)

#define DISK_BASE           DISK_C

//#define AUTO_CHANGE_DISK    //是否自动切换盘符

//playflag定义
/*!图片旋转播放*/
#define    PLAYFLAG_AUTO_ROTATION              0x00000001
/*!图片缩略图播放*/
#define    PLAYFLAG_PREVIEW_DECODE             0x00000002
/*!图片整数倍播放*/
#define    PLAYFLAG_BYPASS_MODE                0x00000004
/*!图片解码支持延时播放*/
#define    PLAYFLAG_DELAY_MODE                 0x00000008

/*!图片非旋转播放*/
#define    PLAYFLAG_NO_ROTATION                0xfffffffe
/*!图片大图解码*/
#define    PLAYFLAG_NORMAL_DECODE              0xfffffffd
/*!图片小数倍播放*/
#define    PLAYFLAG_FULL_MODE                  0xfffffffb
/*!图片无延时播放*/
#define    PLAYFLAG_NODELAY_MODE               0xfffffff7

//returnflag定义
/*!图片是否选择*/
#define    RETURNFLAG_ROTATION                 0x00000001
/*!图片是否多帧*/
#define    RETURNFLAG_MULTIFRAME               0x00000002



#define VALUE_PREVIEW_MODULE_ON 0x03      //查询缩略图是否开  
#define VALUE_PREVIEW_ON        0x02      //置缩略图开
#define VALUE_PREVIEW_OFF       0xfd      //置缩略图关  
#define VALUE_PREVIEW_SUPPORT   0x01      //是否支持缩略图

//图片切换文件标志位
#define up_picture_changenext   0x00
#define up_picture_changeprev   0x01
#define up_picture_changefile   0x02
#define up_picture_playnext     (up_picture_changefile | up_picture_changenext)
#define up_picture_playprev     (up_picture_changefile | up_picture_changeprev)
#define clr_picture_changefile 0xfd


/*定义picture应用的VM全局变量*/
/*!
 * \brief
 *  picture的VM全局变量.
 */
typedef struct
{
    /*!VM 变量魔数*/
    uint16 magic;
    /*!图片缩略图功能开关 bit 0:是否支持缩略图 bit1:缩略图开关*/
    uint8 thumbnail_flag;
    /*!图片自动旋转功能开关 0:关 1:开*/
    uint8 revolve_flag;
    /*!图片扩展名的bit map*/
    uint32 file_ext_bitmap;
    /*图片文件的播放路径信息*/
    file_path_info_t path;
    /*上一次进入图片的应用*/
    uint8 prev_enter_mode;
} g_picture_var_t;

/*定义主菜单的场景*/
/*!
 *  \brief
 *  scene type
 *  \li SCENE_EXIT : 退出
 *  \li SCENE_FILELIST : picture文件列表
 *  \li SCENE_OPTIONMENU : 菜单选项列表场景
 *  \li SCENE_PLAY : 播放场景
 */
typedef enum
{
    SCENE_EXIT = 0,
    SCENE_FILELIST,
    SCENE_OPTIONMENU,
    SCENE_PLAY,
    SCENE_PREVIEW,
    SCENE_MENU
} scene_picture_e;

/*!
 *  \brief
 *  定义解码初始化错误状态
 */
typedef enum
{
    DECODE_NO_ERR = 0,
    DECODE_LOAD_ERR,
    DECODE_OPEN_ERR,
    DECODE_FILE_ERR,
    DECODE_UNSUPPORT_ERR
}decode_status_e;


/*定义picture配置项*/
/*!
 *  \brief
 */
typedef struct
{
    /*! 是否支持缩略图显示 */
    bool thumbnail_support;
}picture_config_t;

/* 全局变量声明 */

/*! \brief picture应用VM变量*/
extern g_picture_var_t g_picture_var;

/*! \brief 通用VM变量*/
extern comval_t g_comval;

/*! \brief picture 挂载文件系统的mount ID*/
extern int32 picture_mount;

/*! \brief 传给codec的图片window */
extern wregion_t region_in;

/*! \brief 实际解码用的图片window */
extern wregion_t region_out;

/*! \brief 图片解码线程句柄 */
extern void *picture_handle;

/*! \brief 传给codec的图片解码信息 */
extern ColorLcmInfor g_picture_userinfor;

/*! \brief 应用进入模式*/
extern app_param_e g_picture_enter_mode;

/*! \brief 自动播放或滚动显示文件名的定时器ID */
extern int8 timer_pic_play;

/*! \brief 自动播放切换文件标志 */
extern uint8 g_picture_change_file;

/*! \brief 自动播放标志 */
extern bool  g_picture_auto_play;

/*! \brief 文件名需要滚动显示标志 */
extern bool  g_picture_scroll_flag;

/*! \brief picture下一个场景 */
extern scene_picture_e g_picture_scene_next;

/*! \brief picture上一个场景*/
extern scene_picture_e g_picture_scene_prev;

/*! \brief 需要重新启动music引擎标志 */
extern bool g_need_resume_engine;

/*! \brief 图片解码库是否空闲标志 */
extern bool g_mmm_id_free;

/*! \brief 文件名缓冲区 */
extern uint8 g_picture_filename[LIST_ITEM_BUFFER_LEN];

/*! \brief 文件选择器是否初始化 */
extern bool is_fsel_init;

/*! \brief 是否关闭music后台 */
extern uint8 g_stop_music;

/*! \brief 文件总数 */
extern uint16 g_file_total;

/*! \brief 错误文件总数*/
extern uint16 g_file_err;

/*! \brief 是否删除文件*/
extern uint8  g_del_file;


extern app_result_e pic_filelist(void);

extern app_result_e pic_option_menulist(void);

extern app_result_e pic_play(void);

extern app_result_e pic_menu(void);

extern app_result_e pic_msg_callback(private_msg_t *msg)
__FAR__;

extern bool pic_get_file_information(void);

extern bool pic_change_file(void);

extern bool pic_decode_init(uint8 mode) __FAR__;

extern app_result_e pic_play_proc_key_event(msg_apps_type_e gui_event);

extern decode_status_e pic_decode(void) __FAR__;

extern void pic_decoder_free(void) __FAR__;

extern bool pic_check_decode(void) __FAR__;

extern void music_close_engine(void) __FAR__;

extern bool music_resume_engine(void);

extern bool music_pause_engine(void);

extern fsel_error_e pic_fsel_initial(uint8 disk_flag);

extern app_result_e pic_disk_initial(void);

extern void auto_play_flag(void);

extern void scroll_play_flag(void);

extern app_result_e pic_data_init(void);

extern bool pic_check_engine(image_type_t image_type) __FAR__;

extern  decode_status_e _pic_set_file(void);

#endif

