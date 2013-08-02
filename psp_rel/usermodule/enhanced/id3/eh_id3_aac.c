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
 * \file     eh_id3_aac.c
 * \brief    这里填写文件的概述
 * \author   liminxian
 * \par      GENERAL DESCRIPTION:
 *               这里对文件进行描述
 * \par      EXTERNALIZED FUNCTIONS:
 *               这里描述调用到外面的模块
 * \version 1.0
 * \date  2011/9/15
 *******************************************************************************/

#include "eh_id3.h"

extern uint8 id3_temp_buf[SECTOR_SIZE];
extern uint8 key_buf[KEY_BUF_LEN];
extern uint8 check_count;
extern uint8 check_flag[8]; //查找ID3要素标记
extern uint16 id3_pos_buffer; //当前TempBuf未处理字符的索引
extern uint32 id3_pos_file; //当前文件的精确指针

extern id3_save_t *id3_save_p; //FrameID存储位置
extern id3_info_t *id3_info_p; //ap层提供的存储结构


/******************************************************************************/
/*
 * \par  Description:解析aac类型的id3信息
 *
 * \param[in]

 * \param[out]

 * \return        TRUE OR FALSE

 * \note
 *******************************************************************************/
bool get_id3_aac(void)
{
    if (FALSE != get_id3_aax())
    {
        return TRUE;
    }

    vfs_file_seek(vfs_mount, 0, SEEK_SET, id3_handle);
    if (FALSE != get_id3_mp3())
    {
        return TRUE;
    }
    vfs_file_seek(vfs_mount, 0, SEEK_SET, id3_handle);
    return get_id3_ape();
}

