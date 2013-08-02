/*******************************************************************************
 *                              US212A
 *                            Module: PLAYLSIT
 *                 Copyright(c) 2003-2011 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       liminxian  2011-9-15 15:37    1.0             build this file
 *******************************************************************************/
/*!
 * \file     *.c
 * \brief    这里填写文件的概述
 * \author   liminxian
 * \par      GENERAL DESCRIPTION:
 *               这里对文件进行描述
 * \par      EXTERNALIZED FUNCTIONS:
 *               这里描述调用到外面的模块
 * \version 1.0
 * \date  2011/9/15
 *******************************************************************************/
#include "plist.h"
#include "rdata_ex.h"

const uint8 m3u_ext_buf[4] = "M3U";

/******************************************************************************/
/*
 * \par  Description: 写入文件操作，保证扇区对齐
 *
 * \param[in]     data_buf--写入数据的地址
 len--数据长度(字节)

 * \param[out]

 * \return        TRUE OR FALSE

 * \note   记录当前文件位置
 *******************************************************************************/
//写入文件操作，保证扇区对齐，记录文件位置
bool save_to_file(uint8 *data_buf, uint32 len)
{
    uint16 sectors;
    uint16 i;

    sectors = (uint16) ((len - 1) / SECTOR_SIZE) + 1;

#if 0
    //多扇区写入
    if(0==vfs_file_write(ap_vfs_mount,data_buf,SECTOR_SIZE*sectors,ap_obj_handle))
    {
        return FALSE;
    }
#else
    // 单扇区写入
    for (i = 0; i < sectors; i++)
    {
        if (0 == vfs_file_write(ap_vfs_mount, data_buf, SECTOR_SIZE, ap_obj_handle))
        {
            return FALSE;
        }
        data_buf += SECTOR_SIZE;
    }

#endif
    ap_pos_file_offset += sectors;
    return TRUE;
}

/******************************************************************************/
/*
 * \par  Description: 从VRAM读取整扇区数据
 *
 * \param[in]    dest--VM的地址
 source--数据的地址
 len--读出的字节数
 * \param[out]

 * \return       TRUE OR FALSE

 * \note
 *******************************************************************************/
bool read_from_vram(uint8 *dest, uint8 *source, uint32 len)
{
    len = (len - 1) / SECTOR_SIZE + 1;

#if 0
    //vram读受限制，每次只读512
    while (len != 0)
    {
        sys_vm_read(dest, source, SECTOR_SIZE);
        source += SECTOR_SIZE;
        dest += SECTOR_SIZE;
        len--;
    }

#else
    //多扇区读
    sys_vm_read(dest, source, len * SECTOR_SIZE);

#endif
    return TRUE;
}

/******************************************************************************/
/*
 * \par  Description: 写入整扇区数据到VRAM
 *
 * \param[in]    dest--VM的地址
 source--数据的地址
 byte--写入字节个数
 * \param[out]

 * \return       TRUE OR FALSE

 * \note
 *******************************************************************************/
bool write_to_vram(uint8 *dest, uint8 *source, uint32 len)
{
    len = (len - 1) / SECTOR_SIZE + 1;

#if 0
    //vram写受限制，每次只写512
    while (len != 0)
    {
        sys_vm_write(source, dest);
        source += SECTOR_SIZE;
        dest += SECTOR_SIZE;
        len--;
    }

#else

    //多扇区写
    sys_vm_write_multi(source, dest, len * SECTOR_SIZE);

#endif

    return TRUE;
}

/******************************************************************************/
/*
 * \par  Description: 定点debug
 *
 * \param[in]    count--检测对象
 debug_value--对象指定值debug
 * \param[out]

 * \return

 * \note
 *******************************************************************************/
void debug_count_flag(uint32 count, uint32 debug_value)
{
    if (count == debug_value)
    {
        debug_value = 0;
    }

    return;
}

/*
 void handle_timers()
 {

 }
 */

//到此，gui消息队列已经处理完毕
//获取ap私有消息和系统消息（会先转换为私有消息再返回），并处理

app_result_e plist_get_app_msg(void)
{
    private_msg_t private_msg;
    input_gui_msg_t gui_msg;
    app_result_e result = RESULT_NULL;

    if (get_gui_msg(&gui_msg) == TRUE)
    {
        if (INPUT_MSG_KEY == gui_msg.type)
        {
            if ((gui_msg.data.kmsg.val == KEY_LOCK) || (gui_msg.data.kmsg.val == KEY_UNLOCK))
            {
                gui_keylock(TRUE);
            }
        }
    }
    g_sys_counter.battery_counter = 100;//每次进入都会检测低电
    if (get_app_msg(&private_msg) == TRUE)
    {
        if (private_msg.msg.type == MSG_EARPHONE_IN)
        {
            /*耳机（天线）插入*/
            change_antenna_state(TRUE);
        }
        else if (private_msg.msg.type == MSG_EARPHONE_OUT)
        {
            /*耳机（天线）拔出*/
            change_antenna_state(FALSE);
        }
        else
        {
        }
    }

    //挂起10ms，多任务调度
    cpu_release_count++;
    if ((cpu_release_count % 5) == 00)
    {
        //暂时释放cpu
        sys_os_time_dly(1);
    }

    return result;
}
