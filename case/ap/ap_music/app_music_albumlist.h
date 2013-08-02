/******************************************************************************
 *                               US212A
 *                            Module: PICTURE_PREVIEW
 *                 Copyright(c) 2003-2011 Actions Semiconductor,
 *                            All Rights Reserved.
 * History:
 *      <author>         <time>             <version>             <desc>
 *      wuyufan      2011-12-9 13:57:39          1.0              build this file
 ******************************************************************************/
/*!
 * \file     picture_preview.h
 * \brief    图片缩略图模块头文件
 * \author   wuyufan
 * \version  1.0
 * \date     2011-12-9
 *******************************************************************************/
#ifndef _PICTURE_PREVIEW_H
#define _PICTURE_PREVIEW_H

#include "app_music.h"
#include  "mmm_id.h"


#define PIC_PER_ROW     3
#define PIC_PER_COLUME 3
#define PIC_ONE_SCREEN (PIC_PER_ROW * PIC_PER_COLUME)
#define PIC_PREV_VIEW_WIDTH 40
#define PIC_PREV_VIEW_HEIGHT 40
#define PIC_FRAME_WIDTH 44
#define PIC_FRAME_HEIGHT 2

#define MOVE_UP 0
#define MOVE_DOWN 1
#define MOVE_PREV 2
#define MOVE_NEXT 3

/*! \brief 传给codec的图片window */
extern wregion_t region_in _BANK_DATA_ATTR_;

/*! \brief 实际解码用的图片window */
extern wregion_t region_out _BANK_DATA_ATTR_;

extern dir_brow_t pic_dir_browser _BANK_DATA_ATTR_;

extern uint8 g_enter_dir_flag _BANK_DATA_ATTR_;

extern album_brow_t g_album_list[PIC_ONE_SCREEN] _BANK_DATA_ATTR_;

extern void init_list_para(void);

extern app_result_e pic_preview_proc_key_event(file_path_info_t *path_info, 
    dir_control_t *p_dir_control, msg_apps_type_e cur_event);
    
extern bool bs_dir_set_dirpos(uint32 object_type, uint16 dir_num) __FAR__;

extern void read_history_item(uint8 layer_no, dir_control_t *p_dir_control);

extern void write_history_item(uint8 layer_no, dir_control_t *p_dir_control, bool update);

extern app_result_e pic_decode_init(dir_control_t *p_dir_control, file_path_info_t *path_info);

extern app_result_e pic_preview(file_path_info_t *path_info, dir_com_data_t *dir_com);

extern void scroll_play_flag(void);

extern void pic_paint(uint8 active);

extern void pic_init_display_buffer(void);
#endif

/*! \endcond */


