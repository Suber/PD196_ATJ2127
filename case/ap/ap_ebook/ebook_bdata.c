/********************************************************************************
 *                              USDK
 *                             Module: ebook
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : ebook_main.c
 * History:
 *      <author>    <time>           <version >             <desc>
 *      stevenluo		2011-11-11         v1.0              create this file
 ********************************************************************************/
/* include file of this application, 本应用的头文件 */
#include "ebook.h"

/****************************************************************************/

/* bank段 变量定义 */
//文本阅读数据存储和解码缓冲区结构体，主要用于阅读时使用
view_decode_t view_decode _BANK_DATA_ATTR_;
view_decode_t page_count_decode _BANK_DATA_ATTR_;

//书签信息存储结构体
bkmark_node_t bkmarks[MAX_BKMARK_NUM]_BANK_DATA_ATTR_;
//列表框信息结构体
listbox_private_t bmklist_data _BANK_DATA_ATTR_;

//存储列表显示信息的数组
uint8 file_list[LIST_NUM_ONE_PAGE_MAX][FILENMAELEN]_BANK_DATA_ATTR_;

