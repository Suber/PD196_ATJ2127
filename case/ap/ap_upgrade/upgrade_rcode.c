/*******************************************************************************
 *                              US212A
 *                            Module: UPGRADE_FIRMWARE
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>         <time>             <version>             <desc>
 *      wuyufan    2012-10-19 14:00:41           1.0              build this file
 *******************************************************************************/
/*!
 * \file     upgrade_firmware.c
 * \brief    固件升级模块
 * \author   wuyufan
 * \par      GENERAL DESCRIPTION:
 *               这里对文件进行描述
 * \par      EXTERNALIZED FUNCTIONS:
 *               这里描述调用到外面的模块
 * \version 1.0
 * \date    2012-10-19
 *******************************************************************************/
#include "ap_upgrade.h"
#include "audio_device.h"

const uint8 upgrade_percent_str[11][5] =
{ "0%", "10%", "20%", "30%", "40%", "50%", "60%", "70%", "80%", "90%", "100%" };

/******************************************************************************/
/*!
 * \par  Description:
 *	  获取LFI数据在文件中偏移
 * \param[in]    buf   data buffer address
 * \param[in]    buffer_len   buffer length
 * \param[in]    key_word 关键字字符串
 * \param[in]    key_len  关键字字符串长度
 * \param[in]    offset  偏移
 * \return       AFI_DIR_t* 目录项指针
 * \retval
 * \note
 *******************************************************************************/
AFI_DIR_t *check_key_offset_exist(char *buffer, uint32 buffer_len, const uint8 *key_word, uint8 key_len, uint8 offset)
{
    uint32 str_len = 0;

    while (str_len < buffer_len)
    {
        if (libc_memcmp(&buffer[str_len + offset], key_word, (uint32) key_len) == 0)
        {
            return (AFI_DIR_t *) (&buffer[str_len]);
        }
        else
        {
            str_len += 32;
        }
    }

    return NULL;
}

/******************************************************************************/
/*!
 * \par  Description:
 *	  读取文件数据并解密
 * \param[in]    mode           0 初始化 1正常解密
 * \param[in]    decrypt_rw     数据结构体
 * \param[in]    read_len       读取的数据长度
 * \param[in]    fp                     文件句柄
 * \return       the read len
 * \note
 *******************************************************************************/
uint32 _read_file_translate(uint8 mode, uint32 fp, PCFWDecrypt_struct *decrypt_rw, uint32 read_len)
{
    uint32 ret_val = 0;

    //读取文件
    read_len = vfs_file_read(g_file_sys_id, decrypt_rw->pInOutBuffer, read_len, fp);

    if (mode == 0)
    {
        ret_val = (uint32) fw_decrypt_init(decrypt_rw);
    }
    else
    {
        fw_decrypt_run(decrypt_rw->pInOutBuffer, read_len, decrypt_rw->pGLBuffer);
    }

    if (ret_val != 0)
    {
        return 0;
    }
    else
    {
        return read_len;
    }
}

/******************************************************************************/
/*!
 * \par  Description:
 *	  读取文件数据并解密
 * \param[in]    read_len       读取的数据长度
 * \param[in]    fp             文件句柄
 * \return       the result
 * \note
 *******************************************************************************/
bool _read_file(uint32 read_len, uint32 fp)
{
    uint8 i, read_cnt;
    uint32 read_bytes;
    uint8 *data_buffer = g_data_buffer;

    read_cnt = (uint8)((read_len + 2047) / 2048);

    upgrade_get_app_msg();

    for (i = 0; i < read_cnt; i++)
    {
        if (read_len >= 2048)
        {
            read_bytes = 2048;
        }
        else
        {
            read_bytes = read_len;
        }

        read_bytes = _read_file_translate(1, fp, &g_decrypt, read_bytes);
        if (read_bytes == 0)
        {
            return FALSE;
        }

        read_len -= read_bytes;

        libc_memcpy(data_buffer, g_decrypt.pInOutBuffer, read_bytes);

        data_buffer += read_bytes;
    }

    return TRUE;
}

/******************************************************************************/
/*!
 * \par  Description:
 *	  封装底层写逻辑扇区接口
 * \param[in]    pbuf           写入数据buffer
 * \param[in]    addr           写入逻辑区地址
 * \param[in]    len            写入长度
 * \return       the result
 * \retval       TRUE succeed  FALSE fail
 * \note
 *******************************************************************************/
bool ud_sector_write(void *pbuf, uint32 addr, uint32 len)
{
    int32 ret_val;
    ud_sector_param_t ud_sec_param;

    if ((addr & 0x1ff) != 0)
    {
        //非512字节对齐
        return FALSE;
    }

    ud_sec_param.file_offset = (addr >> 9);
    ud_sec_param.sec_num = (len + 0x1ff) >> 9;
    ud_sec_param.sram_addr = (uint32) pbuf;

    ret_val = (int32) base_fw_write(ud_sec_param.file_offset, ud_sec_param.sram_addr, ud_sec_param.sec_num);
    if (ret_val == -1)
    {
        return FALSE;
    }
    else
    {
        g_write_sector += ud_sec_param.sec_num;
        return TRUE;
    }
}
/******************************************************************************/
/*!
 * \par  Description:
 *	  封装底层读逻辑扇区接口
 * \param[in]    pbuf           读入数据buffer
 * \param[in]    addr           读入逻辑区地址
 * \param[in]    len            读入长度
 * \return       the result
 * \retval       0 succeed  -1 fail
 * \note
 *******************************************************************************/
bool ud_sector_read(void *pbuf, uint32 addr, uint32 len)
{
    int32 ret_val;
    ud_sector_param_t ud_sec_param;

    if ((addr & 0x1ff) != 0)
    {
        //非512字节对齐
        return FALSE;
    }

    ud_sec_param.file_offset = addr >> 9;
    ud_sec_param.sec_num = (len + 0x1ff) >> 9;
    ud_sec_param.sram_addr = (uint32) pbuf;

    ret_val = (int32) base_fw_read(ud_sec_param.file_offset, ud_sec_param.sram_addr, ud_sec_param.sec_num);
    if (ret_val == -1)
    {
        return FALSE;
    }
    return TRUE;
}

/******************************************************************************/
/*!
 * \par  Description:
 *	  封装底层brec逻辑扇区接口
 * \param[in]    pbuf           读入数据buffer
 * \param[in]    addr           读入逻辑区地址
 * \param[in]    len            读入长度
 * \return       the result
 * \retval       0 succeed  -1 fail
 * \note
 *******************************************************************************/
bool brec_sector_write(void *pbuf, uint32 addr, uint32 len)
{
    int32 ret_val;
    ud_sector_param_t ud_sec_param;

    if ((addr & 0x1ff) != 0)
    {
        //非512字节对齐
        return FALSE;
    }

    ud_sec_param.file_offset = addr >> 0x9;
    ud_sec_param.sec_num = (len + 0x1ff) >> 9;
    ud_sec_param.sram_addr = (uint32) pbuf;

    ret_val = (int32) base_brec_write(ud_sec_param.file_offset, ud_sec_param.sram_addr, ud_sec_param.sec_num);
    if (ret_val == -1)
    {
        return FALSE;
    }
    else
    {
        g_write_sector += ud_sec_param.sec_num;
        return TRUE;
    }
}

/******************************************************************************/
/*!
 * \par  Description:
 *	  更新固件升级过程中的显示
 * \param[in]    value     更新显示的值
 * \return       none
 * \note
 *******************************************************************************/
void _upgrade_refreash_progress(uint8 value)
{
    style_infor_t upgrade_sty;
    progressbar_private_t progressbar_param;
    textbox_private_t textbox_param;

    upgrade_sty.type = UI_AP;

    // 显示百分比
    upgrade_sty.style_id = UPGRADE_PERCENT;
    textbox_param.lang_id = ANSIDATAAUTO;
    textbox_param.str_value = (uint8 *) &upgrade_percent_str[value];
    ui_show_textbox(&upgrade_sty, &textbox_param, TEXTBOX_DRAW_NORMAL);

    // 显示进度条
    upgrade_sty.style_id = UPGRADE_PROGRESS;
    progressbar_param.total = 120;
    progressbar_param.value = value * 12;
    ui_show_progressbar(&upgrade_sty, &progressbar_param, PROGRESS_DRAW_PROGRESS);
}

/******************************************************************************/
/*!
 * \par  Description:
 *	  定时器显示函数，用于更新固件升级过程中的显示
 * \param[in]    none
 * \return       none
 * \note
 *******************************************************************************/
void _upgrade_proc_timer(void)
{
    uint8 old_precent_count;
    uint32 sector_total;

    old_precent_count = g_precent_count;

    sector_total = (uint32)((g_fw_length + g_brec_length) >> 9);

    if (sector_total == 0)
    {
        return;
    }

    g_precent_count = (uint8)((9 * g_write_sector) / sector_total);

    if (g_precent_count == 0)
    {
        //初始化读取阶段设置为10%
        g_precent_count = 1;
    }

    if (old_precent_count != g_precent_count)
    {
        _upgrade_refreash_progress(g_precent_count);
    }
}

/******************************************************************************/
/*!
 * \par  Description:
 *	 固件升级过程中获取消息接口
 * \param[in]    none
 * \return       app_result_e 会被忽略
 * \note
 *******************************************************************************/
app_result_e upgrade_get_app_msg(void)
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
    if ((cpu_release_count % 5) == 0)
    {
        //暂时释放cpu
        sys_os_time_dly(1);
    }

    return result;
}

