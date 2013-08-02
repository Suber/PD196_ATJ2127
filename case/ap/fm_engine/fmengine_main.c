/*******************************************************************************
 *                              US212A
 *                            Module: radio engine
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *      mikeyang  2011-09-05        1.0              create this file
 *******************************************************************************/

#include "App_fmengine.h"

//globle variable
//FM 引擎状态信息
Engine_Status_t g_fmengine_status;

//引擎配置信息
FMEngine_cfg_t g_fmengine_cfg;

//FM 模组所处状态
Module_Status_e g_module_status;

//FM 引擎任务栈
#ifndef PC
OS_STK *ptos = (OS_STK *) AP_FMENGINE_STK_POS;
#endif

//FM 引擎主线程优先级
INT8U prio = AP_FMENGINE_PRIO;

/******************************************************************************/
/*!
 * \par  Description:
 * \void fmengine_load_cfg(void)
 * \读取配置信息
 * \param[in]    void  
 * \param[out]   none
 * \return       void 
 * \retval
 * \retval
 * \note
 */
/*******************************************************************************/
void fmengine_load_cfg(void)
{
    //获取FM 引擎配置信息
    sys_vm_read(&g_fmengine_cfg, VM_AP_FMENGINE, sizeof(FMEngine_cfg_t));
    if (g_fmengine_cfg.magic != MAGIC_USER1) //0x55AA
    {
        g_fmengine_cfg.magic = MAGIC_USER1;
        g_fmengine_cfg.fm_threshold = (uint8) com_get_config_default(FMENGINE_AP_ID_SEEKTH); //THRESHOLD_DEFAULT;
        g_fmengine_cfg.paddv_mode = (PA_DDV_Mode_e) com_get_config_default(FMENGINE_AP_ID_PAMODE); //PA_MODE;
        /*保存VM变量*/
        sys_vm_write(&g_fmengine_cfg, VM_AP_FMENGINE);
    }
}

/******************************************************************************/
/*!
 * \par  Description:
 * \void fmengine_save_cfg(void)
 * \保存配置信息
 * \param[in]    void  
 * \param[out]   none
 * \return       void 
 * \retval
 * \retval
 * \note
 */
/*******************************************************************************/
void fmengine_save_cfg(void)
{
    /*保存VM变量*/
    sys_vm_write(&g_fmengine_cfg, VM_AP_FMENGINE);
}

/******************************************************************************/
/*!
 * \par  Description:
 * \int _app_init(void)
 * \读取配置信息，安装FM  驱动
 * \param[in]    void  
 * \param[out]   none
 * \return       int the result
 * \retval           0 sucess
 * \retval          -1 failed
 * \note
 */
/*******************************************************************************/
int fmengine_init(void)
{
    int app_init_ret = 0;

    //读取配置信息 (包括vm信息)
    fmengine_load_cfg();

    //初始化applib库，后台AP
    applib_init(APP_ID_FMENGINE, APP_TYPE_CONSOLE);

    //初始化 applib 消息模块
    applib_message_init(NULL);

#ifndef PC
    //安装fm  驱动
    app_init_ret = sys_drv_install(DRV_GROUP_FM, 0, "drv_fm.drv");
#else
    app_init_ret = 0;
#endif
    //驱动安装成功
    if (app_init_ret == 0)
    {
        //模组尚未初始化，需处理MSG_FMENGINE_INIT  消息后进行初始化
        g_module_status = MODULE_NO_INIT;
    }

    return app_init_ret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool fmengine_deinit(void)
 * \退出app的功能函数,保存配置信息
 * \param[in]    void 
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \note
 */
/*******************************************************************************/
bool fmengine_deinit(void)
{
    //执行applib库的注销操作
    applib_quit();

    //save config 写vram
    fmengine_save_cfg();

#ifndef PC
    //卸载fm 驱动
    sys_drv_uninstall(DRV_GROUP_FM);
#endif
    return TRUE;
}

/******************************************************************************/
/*!
 * \Description: FM Engine ap entry function
 * \int main(int argc, const char *argv[])
 * \app入口函数
 * \param[in]    argc  para1
 * \param[in]    argc  para2
 * \param[out]   none
 * \return       int the result
 * \retval           1 sucess
 * \retval           0 failed
 * \note
 */
/*******************************************************************************/
int main(int argc, const char *argv[])
{
    app_result_e result = RESULT_NULL;
    //初始化
    if (fmengine_init() == 0)
    {
        //引擎刚安装时，默认为播放
        change_engine_state(ENGINE_STATE_PLAYING);
        result = fmengine_control_block();
    }
    fmengine_deinit();//退出

    return result;
}

