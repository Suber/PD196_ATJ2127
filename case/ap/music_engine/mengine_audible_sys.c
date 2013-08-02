/*******************************************************************************
 *                              US212A
 *                            Module: music engine
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *      fiona.yang  2012-03-02        1.0              create this file
 *******************************************************************************/

#include "app_mengine.h"
#include "sys_info.h"

uint8 aud_sys_buffer[512] _BANK_DATA_ATTR_;
/******************************************************************************/
/*!
 * \par  Description:
 * \uint8 _conver_numer(uint8 num)
 * \转换小机设备ID数字，将hex字符转换为二进制数字
 * \param[in]    none
 * \return       none
 * \ingroup      mengine_audbile_deal.c
 * \note
 */
/*******************************************************************************/

static uint8 _conver_numer(uint8 num)
{
    uint8 temp = num;

    if ((temp >= '0') && (temp <= '9'))
    {
        temp -= 0x30;
    }
    else if ((temp >= 'A') && (temp <= 'Z'))
    {
        temp -= 0x37;
    }
    else
    {
        temp -= 0x57;
    }
    return temp;
}
/******************************************************************************/
/*!
 * \par  Description:
 * \void _get_firmware_info(void)
 * \获取并转换小机的设备ID
 * \param[in]    none
 * \return       none
 * \ingroup      mengine_audbile_deal.c
 * \note
 */
/*******************************************************************************/
static void _get_firmware_info(void)
{
    uint8 fw_id[16];
    uint8 i;
    uint8 j = 0;

    //device id get 为测试写固定值,usb提供获取该信息的接口
    //libc_memcpy(g_send_audible_p->DeviceId_, DeviceId, sizeof(g_send_audible_p->DeviceId_));
    for (i = 0; i < 16; i++)
    {
        fw_id[i] = 0x00;
    }

    libc_memset(g_send_audible_p->DeviceId_, 0, 20);

    sys_get_fw_info(fw_id, FW_INFO_USB_ID_ADDR, 16);

    //压缩存储
    for (i = 0; i < 8; i++)
    {
        g_send_audible_p->DeviceId_[i] = (_conver_numer(fw_id[j]) << 4);
        j++;
        g_send_audible_p->DeviceId_[i] |= _conver_numer(fw_id[j]);
        j++;
    }
}
/******************************************************************************/
/*!
 * \par  Description:
 * \void _get_audible_sysfile(void)
 * \获取audible的sys文件信息,该文件由audible工具在小机生成,并存储于VM中
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      mengine_audbile_deal.c
 * \note
 */
/*******************************************************************************/
static void _get_audible_sysfile(void)
{
    sys_vm_read(aud_sys_buffer, VM_AUD_SYS, 512);
    libc_memcpy(g_send_audible_p->ActivationRecord_, aud_sys_buffer, 512);
    sys_vm_read(aud_sys_buffer, VM_AUD_SYS + 512, 48);
    libc_memcpy(&g_send_audible_p->ActivationRecord_[512], aud_sys_buffer, 48);
}

/******************************************************************************/
/*!
 * \par  Description:
 * \void _deal_audible_sysfile(void)
 * \audible文件的sys文件的信息，以及设备号等信息的处理
 * \param[in]    void  para1
 * \param[in]    none
 * \param[out]   none
 * \return       bool the result
 * \retval
 * \retval
 * \ingroup      mengine_audbile_deal.c
 * \note
 */
/*******************************************************************************/
void _deal_audible_sysfile(void)
{
    //读取同步下来的sys文件
    _get_audible_sysfile();

    //获取小机本身的设备ID
    _get_firmware_info();
}
