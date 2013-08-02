/*******************************************************************************
 *                              US212A
 *                            Module: ENHANCED
 *                 Copyright(c) 2003-2011 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       liminxian  2011-9-15 15:37    1.0             build this file
 *******************************************************************************/
/*!
 * \file     eh_bs_rdata.c
 * \brief    浏览模块常驻数据
 * \author   liminxian
 * \par      GENERAL DESCRIPTION:
 *               这里对文件进行描述
 * \par      EXTERNALIZED FUNCTIONS:
 *               这里描述调用到外面的模块
 * \version 1.0
 * \date  2011/9/15
 *******************************************************************************/

#include "eh_module.h"

/*********************************
 *** rdata  define
 *********************************/

// common
//uint8 dir_layer;
uint16 bs_select_listno; // 选择的list序号
uint16 bs_select_fileno; // 选择的文件序号
// disk mode
//uint8  bs_file_type;     //浏览的文件类型
uint16 bs_dir_total; //当前目录下文件夹的总数
uint16 bs_file_total; //当前目录下文件的总数
// favorite mode
uint16 bs_uspl_cur;
// playlist mode
uint16 list_layer_no[4];
plist_tree_t plist_cur_tree;
bs_dir_location_t g_bs_dir_loc;

