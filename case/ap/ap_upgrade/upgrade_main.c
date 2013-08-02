/*                              US212A
 *                            Module: UPDATE_MAIN
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>         <time>             <version>             <desc>
 *      wuyufan    2012-10-18 9:37:12           1.0              build this file
 *******************************************************************************/
/*!
 * \file     upgrade_main.c
 * \brief    自动升级ap主模块
 * \author   wuyufan
 * \par      GENERAL DESCRIPTION:
 *               这里对文件进行描述
 * \par      EXTERNALIZED FUNCTIONS:
 *               这里描述调用到外面的模块
 * \version 1.0
 * \date    2012-10-18
 *******************************************************************************/
#include "ap_upgrade.h"
#include "audio_device.h"

#define APP_TIMER_COUNT 1

//系统变量
setting_comval_t g_setting_comval;

//文件系统具备
int32 g_file_sys_id;

//读写page缓冲区
uint8 *g_data_buffer = (uint8 *) DECRYPT_RW_BUF_ADDR;

//FW偏移
uint32 g_fw_offset;

//FW长度
uint32 g_fw_length;

//FLASH_ID偏移
uint32 g_flashid_offset;

//FLASH_ID长度
uint32 g_flashid_length;

//BREC长度
uint32 g_brec_length;

//前台频率备份
uint32 g_save_freq;

//flash id
uint8 g_flash_id[64];

//解密数据偏移
uint32 g_firmware_offset;

//百分比计数
uint8 g_precent_count;

//刷新定时器ID
int8 g_flush_timer_id;

//cpu释放计时器
uint8 cpu_release_count;

//写入扇区计数
uint32 g_write_sector;

//解密数据结构体
PCFWDecrypt_struct g_decrypt;

app_timer_t conf_app_timer_vector[COMMON_TIMER_COUNT + APP_TIMER_COUNT];

/*main中要填充这两个值，有系统人员分配*/
#ifndef PC
OS_STK *ptos = (OS_STK *) AP_FRONT_LOW_STK_POS;
#endif
INT8U prio = AP_FRONT_LOW_PRIO;

static const uint8 nand_drv_name[] = "nand.drv";
static const uint8 card_drv_name[] = "card.drv";
static const uint8 uhost_drv_name[] = "uhost.drv";

/******************************************************************************/
/*!
 * \par  Description:
 *  读取应用VM全局变量
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      upgrade
 * \note
 *******************************************************************************/
static void _upgrade_read_var(void)
{
    //读取common VM公共变量信息
    sys_vm_read(&g_setting_comval, VM_AP_SETTING, sizeof(g_setting_comval));

    //初始化系统的VM变量
    if (g_setting_comval.g_comval.magic != MAGIC_SETTING)
    {
        g_setting_comval.g_comval.magic = MAGIC_SETTING;
        com_get_default_setting(&g_setting_comval);
        sys_vm_write(&g_setting_comval, VM_AP_SETTING);
    }

    com_set_sys_comval(&g_setting_comval.g_comval);
}
/******************************************************************************/
/*!
 * \par  Description:
 *  装载物理介质和文件系统
 * \param[in]    disk_type 磁盘类型
 * \param[out]   none
 * \return       none
 * \ingroup      upgrade
 * \note
 *******************************************************************************/
int8 _upgrade_fs_init(uint8 disk_type)
{
    int32 ret_val;
    uint8 drv_type;
    uint8 *drv_name;

    if (disk_type == DISK_C)
    {
        drv_type = DRV_GROUP_STG_BASE;
        drv_name = (uint8 *) nand_drv_name;
    }
    else if (disk_type == DISK_H)
    {
        drv_type = DRV_GROUP_STG_CARD;
        drv_name = (uint8 *) card_drv_name;
    }
    else if (disk_type == DISK_U)
    {
        drv_type = DRV_GROUP_STG_UHOST;
        drv_name = (uint8 *) uhost_drv_name;
    }
    else
    {
        return -1;
    }

    //装载物理介质
    if (sys_drv_install(drv_type, 0, drv_name) != 0)
    {
        return -1;
    }

    //装载文件系统
    g_file_sys_id = sys_mount_fs(drv_type, disk_type, 0);

    if (g_file_sys_id == -1)
    {
        sys_drv_uninstall(drv_type);
        return -1;
    }

#ifndef PC
    //切换nand cache地址
    base_op_entry((void*) 1, (void*) NAND_CACHE_ADDR, 0, BASE_UPDATE);
#endif

    ret_val = sys_drv_uninstall(DRV_GROUP_AUDIO_DEVICE);

    if (ret_val == -1)
    {
        return ret_val;
    }

    ret_val = sys_drv_install(DRV_GROUP_AUDIO_DEVICE, 0, "FWDec.al");

    //提频加速
    g_save_freq = sys_adjust_clk(FREQ_108M, 0);

    return g_file_sys_id;
}
/******************************************************************************/
/*!
 * \par  Description:
 *  卸载物理介质和文件系统
 * \param[in]    disk_type 磁盘类型
 * \param[out]   none
 * \return       none
 * \ingroup      upgrade
 * \note
 *******************************************************************************/
int8 _upgrade_fs_deinit(uint8 disk_type)
{
    int32 ret_val;
    uint8 drv_type;

#ifndef PC
    //切换nand cache地址
    base_op_entry((void*) 1, (void*) NAND_DEFAULT_ADDR, 0, BASE_UPDATE);
#endif

    if (g_file_sys_id != -1)
    {
        ret_val = sys_unmount_fs(g_file_sys_id);
        g_file_sys_id = -1;
    }
    else
    {
        return TRUE;
    }

    if (0 > ret_val)
    {
        return FALSE;
    }

    if (disk_type == DISK_C)
    {
        drv_type = DRV_GROUP_STG_BASE;
    }
    else if (disk_type == DISK_H)
    {
        drv_type = DRV_GROUP_STG_CARD;
    }
    else if (disk_type == DISK_U)
    {
        drv_type = DRV_GROUP_STG_UHOST;
    }
    else
    {
        return -1;
    }

    ret_val = sys_drv_uninstall(drv_type);

    if (0 > ret_val)
    {
        return FALSE;
    }

    sys_drv_uninstall(DRV_GROUP_AUDIO_DEVICE);

    sys_drv_install(DRV_GROUP_AUDIO_DEVICE, 0, "aud_dev.drv");

    sys_adjust_clk(g_save_freq, 0);

    return TRUE;
}

/******************************************************************************/
/*!
 * \par  Description:
 *    upgrade applib退出
 * \param[in]    void
 * \param[out]   void
 * \return       bool
 * \retval       1    success
 * \retval       0    fail
 * \ingroup      upgrade
 * \note
 *******************************************************************************/
bool _upgrade_app_deinit(void)
{
    kill_app_timer(g_flush_timer_id);

    //卸载文件系统
    _upgrade_fs_deinit(DISK_C);

    sys_timer_exit();

    ui_res_close(UI_COM);
    ui_res_close(UI_AP);

    ui_set_display_buf(JPEG_RAM_INDEX);

    applib_quit();

    return TRUE;
}
/******************************************************************************/
/*!
 * \par  Description:
 *    upgrade显示初始化
 * \param[in]    void
 * \param[out]   void
 * \return       none
 * \retval       none
 * \ingroup      upgrade
 * \note
 *******************************************************************************/

void _upgrade_display_init(void)
{
    /*pic_box 接口变量*/
    style_infor_t upgrade_sty;
    progressbar_private_t progressbar_param;
    textbox_private_t textbox_param;

    g_precent_count = 0;

    upgrade_sty.type = UI_AP;

    /*显示底部图片*/
    upgrade_sty.style_id = UPGRADE_BACKGROUND;
    ui_show_picbox(&upgrade_sty, NULL);

    // 显示进度条
    upgrade_sty.style_id = UPGRADE_PROGRESS;
    progressbar_param.total = 120;
    progressbar_param.value = 0;
    ui_show_progressbar(&upgrade_sty, &progressbar_param, PROGRESS_DRAW_ALL);

    // 显示正在升级字符串
    textbox_param.lang_id = UNICODEID;
    textbox_param.str_id = -1;
    textbox_param.str_value = NULL;
    upgrade_sty.style_id = UPGRADE_STRING;
    ui_show_textbox(&upgrade_sty, &textbox_param, TEXTBOX_DRAW_NORMAL);

    // 显示提示字符串
    upgrade_sty.style_id = UPGRADE_STRING2;
    ui_show_textbox(&upgrade_sty, &textbox_param, TEXTBOX_DRAW_NORMAL);

    // 显示百分比
    upgrade_sty.style_id = UPGRADE_PERCENT;
    textbox_param.lang_id = ANSIDATAAUTO;
    textbox_param.str_value = "%0";
    ui_show_textbox(&upgrade_sty, &textbox_param, TEXTBOX_DRAW_NORMAL);

    //定时器2s
    g_flush_timer_id = set_app_timer(APP_TIMER_ATTRB_UI, 1000, _upgrade_proc_timer);
}

/******************************************************************************/
/*!
 * \par  Description:
 *    通知后台引擎退出
 * \param[in]    void
 * \param[out]   none
 * \return       none
 * \retval       none
 * \ingroup      upgrade
 * \note
 *******************************************************************************/
static void _close_engine(void)
{
    msg_apps_t msg;
    engine_type_e engine_type;

    engine_type = get_engine_type();

    //向process manager发消息要求后台退出
    msg.type = MSG_KILL_APP_SYNC;
    if (engine_type == ENGINE_RADIO)
    {
        msg.content.data[0] = APP_ID_FMENGINE;
    }
    else if (engine_type == ENGINE_MUSIC)
    {
        msg.content.data[0] = APP_ID_MENGINE;
    }
    else
    {
        ;//bug
    }
    send_sync_msg(APP_ID_MANAGER, &msg, NULL, 0);

    return;
}

/******************************************************************************/
/*!
 * \par  Description:
 *  applib初始化
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      upgrade
 * \note
 *******************************************************************************/
bool _upgrade_app_init(void)
{
    //初始化applib库，前台AP
    applib_init(APP_ID_SETTING, APP_TYPE_GUI);

    //初始化软定时器
    init_app_timers(conf_app_timer_vector, COMMON_TIMER_COUNT + APP_TIMER_COUNT);

    //初始化 applib 消息模块
    applib_message_init(NULL);

    _close_engine();

    //初始化ap资源
    ui_res_open("upgrade.sty", UI_AP);

    //初始化common资源
    ui_res_open("common.sty", UI_COM);

    //装载文件系统
    _upgrade_fs_init(DISK_C);

    //设置UI不使用picture buffer
    ui_set_display_buf(LCD_RAM_INDEX);

    _upgrade_display_init();

    _firmware_load_bank();

    g_decrypt.initusebuffer = (uint8 *) DECRYPT_INIT_BUF_ADDR;
    g_decrypt.initusebufferlen = DECRYPT_INIT_BUF_LEN;
    g_decrypt.pInOutBuffer = (uint8 *) DECRYPT_USE_BUF_ADDR;
    g_decrypt.InOutLen = DECRYPT_USR_BUF_LEN;
    g_decrypt.pGLBuffer = (uint8 *) DECRYPT_GL_BUF_ADDR;

    g_write_sector = 0;

    return TRUE;
}
/******************************************************************************/
/*!
 * \par  Description:
 *  升级成功后写看门狗寄存器等待复位
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      upgrade
 * \note
 *******************************************************************************/

void _upgrade_reboot_system(void)
{
    uint32 wd_ctl_tmp;

    //设置看门狗，176ms后复位重启
    wd_ctl_tmp = act_readl(WD_CTL);
    wd_ctl_tmp &= 0xffffffd1;//clksel = 176ms
    wd_ctl_tmp |= 0x11;//wden & clear wd timer
    act_writel(wd_ctl_tmp, WD_CTL);
    while (1)
    {
        ;//nothing
    }
}

/******************************************************************************/
/*!
 * \par  Description:
 *    upgrade应用的入口函数
 * \param[in]    0-开机，其他值：关机，低16bit代表关机时的前台应用ID，高16bit表示后台引擎的状态
 * \param[out]   none
 * \return       the result
 * \retval
 * \retval
 * \ingroup      module name
 * \par          exmaple code
 * \code
 *               list example code in here
 * \endcode
 * \note
 *******************************************************************************/
int main(int argc, const char *argv[])
{
    msg_apps_t msg;
    upgrade_status_e result;

    //应用变量初始化
    _upgrade_read_var();

    //初始化applib
    _upgrade_app_init();

    result = _upgrade_firmware();

    if (result == FW_UPGRADE_NO_ERR)
    {
        //固件升级成功
        gui_dialog_msg(DIALOG_MSG, DIALOG_INFOR_WAIT, S_UPGRADE_SUCCEED);

        //擦除固件
        ud_sector_write(g_data_buffer, 0, 0);

        //准备重启
        _upgrade_reboot_system();

    }
    else
    {
        if (result == FW_UPGRADE_FILE_ERR)
        {
            //固件文件错误
            gui_dialog_msg(DIALOG_MSG, DIALOG_INFOR_WAIT, S_FILE_OPEN_ERROR);

        }
        else
        {
            //固件升级失败
            gui_dialog_msg(DIALOG_MSG, DIALOG_INFOR_WAIT, S_UPGRADE_FAILED);
        }

        //升级失败，返回main ap
        msg.type = MSG_CREAT_APP;
        msg.content.data[0] = (uint8) APP_ID_TOOLS;
        send_async_msg(APP_ID_MANAGER, &msg);
    }

    _upgrade_app_deinit();

    return result;
}

