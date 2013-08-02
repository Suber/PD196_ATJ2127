/*******************************************************************************
 *                              US212A
 *                            Module: Config
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       zhangxs     2011-09-05 10:00     1.0             build this file
 *******************************************************************************/
/*!
 * \file     config_wav_head.c
 * \brief    config 断电恢复录音文件
 * \author   zhangxs
 * \version  1.0
 * \date     2011/9/05
 *******************************************************************************/
#include  "ap_config.h"

/*wav head读写缓冲buf*/
uint8 wav_buf[512] _BANK_DATA_ATTR_;

/******************************************************************************/
/*!
 * \par  Description:
 *  录音断电恢复
 * \param[in]    none
 * \param[out]   none
 * \return       true 成功，false 失败
 * \ingroup      config
 * \note
 *******************************************************************************/
bool write_head(void)
{
    handle fhandle;
    uint32 ret_bytes;
    fsel_param_t init_param;//文件选择器初始化参数
    int32 vfs_mount;
    uint32 file_len;
    int result;
    uint8 rtc_access_val;
    uint32 total_time, tmp_time;

    //从RTC_ACCESS寄存器读出已经录音的分钟数
    rtc_access_val = (uint8) act_readl(RTC_ACCESS);
    file_len = g_writehead.file_bytes + rtc_access_val * 60* g_writehead .bitrate / 8;

    if (file_len == 0)
    {
        return FALSE;
    }

    /*装载disk和fs驱动*/
    init_param.disk = g_writehead.disk_no;
    init_param.fsel_type = FSEL_TYPE_COMMONDIR;
    init_param.file_type_bit = ALLFILE_BITMAP; //显示所有文件和目录
    init_param.fsel_mode = FSEL_MODE_NORMAL;

    //文件选择器初始化
    vfs_mount = fsel_init(&init_param, 0);

    if (vfs_mount < 0)
    {
        return FALSE;
    }

    //获取path和file信息
    if (!vfs_file_dir_offset(vfs_mount, &g_writehead.ptr_layer, &g_writehead.ptr_file_offset, 1))
    {
        return FALSE;
    }

    fhandle = vfs_file_open(vfs_mount, g_writehead.rec_filename, OPEN_RECOVER);
    if (fhandle == 0)
    {
        return FALSE;
    }

    //通过write来设置文件size
    vfs_file_write(vfs_mount, NULL, file_len, fhandle);

    vfs_file_close(vfs_mount, fhandle);

    if ((g_writehead.rec_format == 0) || (g_writehead.rec_format == 2)) //wav or act
    {
        //重新打开文件，此时文件已被恢复
        fhandle = vfs_file_open(vfs_mount, g_writehead.rec_filename, OPEN_MODIFY);
        if (fhandle == 0)
        {
            return FALSE;
        }

        //SEEK到文件头
        result = vfs_file_seek(vfs_mount, 0, SEEK_SET, fhandle);
        if (result == FALSE)
        {
            return FALSE;
        }

        //读一PAGE数据
        ret_bytes = vfs_file_read(vfs_mount, wav_buf, 512, fhandle);
        if (ret_bytes == 0)
        {
            return FALSE;
        }

        //回写wav文件头
        if (g_writehead.rec_format == 0)
        {
            file_len = file_len - 8;
            libc_memcpy(&wav_buf[0x04], &file_len, 4);
            file_len = file_len + 8 - 512;
            libc_memcpy(&wav_buf[0x1fc], &file_len, 4);
        }
        else
        {
            //根据文件长度计算当前的总时间,ms单位
            total_time = (((file_len - 512) / 512) * 510 + ((file_len - 512) % 512));
            tmp_time = total_time / 1000;
            wav_buf[259] = (uint8)(tmp_time % 60); //秒
            tmp_time = tmp_time / 60;
            wav_buf[260] = (uint8)(tmp_time % 60); //分
            wav_buf[261] = (uint8)(tmp_time / 60); //时
        }
        //SEEK到文件头
        result = vfs_file_seek(vfs_mount, 0, SEEK_SET, fhandle);
        if (result == FALSE)
        {
            return FALSE;
        }

        ret_bytes = vfs_file_write(vfs_mount, wav_buf, 512, fhandle);
        if (ret_bytes == 0)
        {
            return FALSE;
        }

        vfs_file_close(vfs_mount, fhandle);

    }
    fsel_exit();
    return TRUE;
}
