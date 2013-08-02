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

#include "picture.h"

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



extern void init_list_para(void);
extern app_result_e pic_preview_proc_key_event(msg_apps_type_e cur_event, dir_control_t *p_dir_control);
extern bool bs_dir_set_dirpos(uint32 object_type, uint16 dir_num) __FAR__;
extern void read_history_item(uint8 layer_no, dir_control_t *p_dir_control);
extern void write_history_item(uint8 layer_no, dir_control_t *p_dir_control, bool update);

extern app_result_e prev_view_one_screen(dir_control_t *p_dir_control);
extern app_result_e pic_preview(void);

#endif

/*! \endcond */

