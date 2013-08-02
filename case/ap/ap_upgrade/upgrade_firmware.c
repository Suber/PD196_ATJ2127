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

const uint8 fw_file_name[] =
{
    0xff,0xfe,
    'U', 0x00,
    'P', 0x00,
    'G', 0x00,
    'R', 0x00,
    'A', 0x00,
    'D', 0x00,
    'E', 0x00,
    '.', 0x00,
    'H', 0x00,
    'E', 0x00,
    'X', 0x00,
    0x00,0x00
};

const uint8 fw_ascfile_name[] = {"UPGRADE HEX"};

const uint8 fw_file_newname[] =
{
    0xff,0xfe,
    'U', 0x00,
    'P', 0x00,
    'G', 0x00,
    'R', 0x00,
    'A', 0x00,
    'D', 0x00,
    'E', 0x00,
    '~', 0x00,
    '.', 0x00,
    'H', 0x00,
    'E', 0x00,
    'X', 0x00,
    0x00,0x00
};

const uint8 lfi_name[] = {"FWIMAGE FW "};
const uint8 nand_id_name[] = {"FLASH_IDBIN"};

//const uint8 sys_folder_name[] = "SYSTEM     ";
/******************************************************************************/
/*!
 * \par  Description:
 *	  计算两字节校验和
 * \param[in]    buf   data buffer address
 * \param[in]    len   data length, 2byte uint
 * \param[out]   none
 * \return       the checksum
 * \retval
 * \note
 *******************************************************************************/
uint16 check_sum16(uint16 *buf, UINT32 len)
{
    uint32 index;
    uint16 sum = 0;

    for (index = 0; index < len; index++)
    {
        sum += buf[index];
    }

    return sum;
}

/******************************************************************************/
/*!
 * \par  Description:
 *	  计算四字节校验和
 * \param[in]    buf   data buffer address
 * \param[in]    len   data length, 4byte uint
 * \param[out]   none
 * \return       the checksum
 * \retval
 * \note
 *******************************************************************************/
uint32 check_sum32(uint32 *buf, uint32 len)
{
    uint32 index;
    uint32 sum = 0;

    for (index = 0; index < len; index++)
    {
        sum += buf[index];
    }

    return sum;
}

/******************************************************************************/
/*!
 * \par  Description:
 *	  读取固件升级文件并写入固件区
 * \param[in]    file_handle           文件句柄
 * \return       upgrade_status_e
 * \retval       写入操作时返回值
 * \note
 *******************************************************************************/
upgrade_status_e _firmware_file_write(uint32 file_handle)
{
    bool ret_val;
    uint16 sector_num;
    LFIHead_t *lfi_head;
    uint16 check_sum;
    uint32 write_addr;

    uint16 sector_total = (uint16)((g_fw_length + 511) / 512);

    lfi_head = (LFIHead_t *) g_data_buffer;

    //准备解析FW
    vfs_file_seek(g_file_sys_id, g_fw_offset, SEEK_SET, file_handle);

    //读取并解析FW前16K数据
    ret_val = _read_file(16 * 1024, file_handle);

    if (ret_val == FALSE)
    {
        //解密失败
        return FW_UPGRADE_FILE_ERR;
    }

    //获取USB序列号
    sys_get_fw_info(lfi_head->UsbSetupInfo, FW_INFO_USB_ID_ADDR, FW_INFO_USB_ID_LEN);

    //获取相关盘符信息
    sys_get_fw_info((uint32) & (lfi_head->CapInfo.vm_disk_cap), FW_INFO_CAPINFO_ADDR, FW_INFO_CAPINFO_LEN);

    //计算校验和
    check_sum = check_sum16((uint16*) g_data_buffer, (sizeof(LFIHead_t) - 2) / 2);

    //写入头部校验和
    lfi_head->Headchecksum = check_sum;

    write_addr = 0;

    //写入16K数据
    ret_val = ud_sector_write(g_data_buffer, write_addr, 16 * 1024);

    if (ret_val == FALSE)
    {
        return FW_UPGRADE_WRITE_ERR;
    }
    else
    {
        write_addr += 16 * 1024;
    }

    sector_total -= 32;

    while (sector_total > 0)
    {
        if (sector_total > 32)
        {
            sector_num = 32;
        }
        else
        {
            sector_num = sector_total;
        }

        ret_val = _read_file(sector_num * 512, file_handle);
        if (ret_val == FALSE)
        {
            //解密失败
            return FW_UPGRADE_FILE_ERR;
        }

        ret_val = ud_sector_write(g_data_buffer, write_addr, sector_num << 9);
        if (ret_val == FALSE)
        {
            return FW_UPGRADE_WRITE_ERR;
        }

        write_addr += sector_num << 9;
        sector_total -= sector_num;
    }
    return FW_UPGRADE_NO_ERR;
}
/******************************************************************************/
/*!
 * \par  Description:
 *	  校验写入固件区的数据是否正确
 * \param[in]    none
 * \return       upgrade_status_e
 * \retval       校验结果返回值
 * \note
 *******************************************************************************/
upgrade_status_e _firmware_file_check(void)
{
    LFIHead_t *lfi_head;
    LFIHead_dir_t *lfi_dir;
    uint32 check_sum;
    uint32 index;
    uint8 *buf;
    uint16 sector_num;
    uint16 sector_total;
    uint32 addr;

    lfi_head = (LFIHead_t *) g_data_buffer;
    lfi_dir = (LFIHead_dir_t *) g_data_buffer;

    //读入前8K
    ud_sector_read(g_data_buffer, 0, 0x2000);

    //计算头部校验和
    check_sum = check_sum16((uint16*) g_data_buffer, (sizeof(LFIHead_t) - 2) / 2);
    if (check_sum != lfi_head->Headchecksum)
    {
        return FW_UPGRADE_CHECK_ERR;
    }

    //SD区文件校验用另外8K空间
    buf = (uint8 *) (g_data_buffer + 0x2000);

    _upgrade_refreash_progress(9);

    for (index = 0; index < 240; index++)
    {
        if (0 == lfi_dir->diritem[index].filename[0])
        {
            break;
        }

        sector_total = (uint16)(lfi_dir->diritem[index].length / 0x200);
        addr = (lfi_dir->diritem[index].offset << 9);
        check_sum = 0;

        while (sector_total != 0)
        {
            if (sector_total > 16)
            {
                sector_num = 16;
            }
            else
            {
                sector_num = sector_total;
            }

            if (TRUE != ud_sector_read(buf, addr, sector_num << 9))
            {
                return FW_UPGRADE_CHECK_ERR;
            }

            check_sum = check_sum + check_sum32((uint32*) buf, sector_num * 512 / 4);

            addr += (sector_num << 9);
            sector_total -= sector_num;
        }

        if (lfi_dir->diritem[index].checksum != check_sum)
        {
            return FW_UPGRADE_CHECK_ERR;
        }

    }

    return FW_UPGRADE_NO_ERR;
}

/******************************************************************************/
/*!
 * \par  Description:
 *	  固件升级程序
 * \param[in]    none
 * \return       upgrade_status_e
 * \retval       结果返回值
 * \note
 *******************************************************************************/
upgrade_status_e _upgrade_firmware(void)
{
    uint32 file_len;
    uint32 ret_val;
    AFI_DIR_t *lfi_dir;
    AFI_DIR_t *nandid_dir;
    //file handle
    handle file_handle;
    upgrade_status_e upgrade_result;

    vfs_cd(g_file_sys_id, CD_ROOT, 0);

    file_handle = vfs_file_open(g_file_sys_id, fw_file_name, R_NORMAL_SEEK);

    if (file_handle == 0)
    {
        file_handle = vfs_file_open(g_file_sys_id, fw_ascfile_name, R_NORMAL_SEEK);
        if (file_handle == 0)
        {
            return FW_UPGRADE_FILE_ERR;
        }
    }

    vfs_file_get_size(g_file_sys_id, &file_len, file_handle, 0);

    g_decrypt.FileLength = (long) file_len;

    while (1)
    {
        //读取第一个扇区数据
        ret_val = _read_file_translate(0, file_handle, &g_decrypt, (uint32) g_decrypt.InOutLen);
        if (ret_val == FALSE)
        {
            //解密失败
            return FW_UPGRADE_FILE_ERR;
        }

        //计算解密数据总偏移
        g_firmware_offset = (uint32)(DECRYPT_USR_BUF_LEN - g_decrypt.InOutLen);

        //获取LFI在文件内偏移
        lfi_dir = check_key_offset_exist(g_decrypt.pInOutBuffer, (uint32) g_decrypt.InOutLen, lfi_name, 11, 0);
        nandid_dir = check_key_offset_exist(g_decrypt.pInOutBuffer, (uint32) g_decrypt.InOutLen, nand_id_name, 11, 0);

        if ((lfi_dir != NULL) && (nandid_dir != NULL))
        {
            g_fw_offset = lfi_dir->offset + g_firmware_offset;
            g_fw_length = lfi_dir->length;
            g_flashid_offset = nandid_dir->offset + g_firmware_offset;
            g_flashid_length = nandid_dir->length;
            break;
        }
    }

    //写brec
    upgrade_result = _firmware_brec_write(g_flashid_offset, g_flashid_length, file_handle);
    if (upgrade_result != FW_UPGRADE_NO_ERR)
    {
        return upgrade_result;
    }

    //写固件
    upgrade_result = _firmware_file_write(file_handle);
    if (upgrade_result != FW_UPGRADE_NO_ERR)
    {
        return upgrade_result;
    }

    upgrade_result = _firmware_file_check();
    if (upgrade_result != FW_UPGRADE_NO_ERR)
    {
        return upgrade_result;
    }

    //最后一次写不会进行刷新，因此这里主动刷新
    _upgrade_refreash_progress(10);

    vfs_file_close(g_file_sys_id, file_handle);

    //重命名固件文件
    vfs_file_dir_remove(g_file_sys_id, fw_file_newname, FS_FILE);

    file_handle = vfs_file_open(g_file_sys_id, fw_file_name, OPEN_MODIFY);
    if (file_handle == 0)
    {
        file_handle = vfs_file_open(g_file_sys_id, fw_ascfile_name, OPEN_MODIFY);
        if (file_handle == 0)
        {
            return FW_UPGRADE_FILE_ERR;
        }
    }

    vfs_file_rename(g_file_sys_id, fw_file_newname, file_handle, 0);
    vfs_file_close(g_file_sys_id, file_handle);

    return upgrade_result;
}

