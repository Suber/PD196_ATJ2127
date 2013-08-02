/*******************************************************************************
 *                              US212A
 *                            Module: music engine
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *      fiona.yang  2011-09-05        1.0              create this file
 *******************************************************************************/

#include "app_mengine.h"

#include "audio_device.h"

//应用自己需要的软定时器个数
#define APP_TIMER_COUNT		3

//globle variable
//保存中间件句柄
void *mp_handle = NULL;

//存放文件系统id
int32 file_sys_id;

//进入模式
mengine_enter_mode_e mengine_enter_mode;

//引擎信息
mengine_info_t mengine_info;

//浏览播放起始时间
uint32 g_intro_start;

//存放music文件后缀名的bitmap配置
uint32 file_type_cfg;//

//删除当前文件标识
uint32 g_del_curfile;

//改变文件路径信息标识
uint32 g_change_path_flag = 0;

//初始化时的盘符
uint32 g_open_disk;

//标识audbile文件
uint16 g_is_audible = 0;

//audible 章节切换定时器
int8 g_chapter_timer = 0xff;

//audible 图片检测定时器
int8 g_image_timer = 0xff;

//pos file buffer
uint8 g_audible_pos[16];

//文件名buffer
uint8 g_file_name[64];

#ifndef PC
//audbile info ptr
audible_para_t *g_audible_info_p = (audible_para_t *) 0x9fc2f000;
//sent to decoder audbile info ptr
audible_para_t *g_send_audible_p = (audible_para_t *) 0x9fc37000;
#else
//audbile info ptr
audible_para_t *g_audible_info_p;
//sent to decoder audbile info ptr
audible_para_t *g_send_audible_p;
#endif

//sd 文件名
//uint8 g_sd_file_name[12];

//文件路径来源信息保存,改变路径信息时使用
fsel_type_e g_path_type;

//系统公共变量
//setting_comval_t g_setting_comval;
//comval_t g_comval;

//中间件状态检测定时器
int8 g_status_timer;

//中间件状态检测标志位
uint8 g_check_status_flag;

//music engine延时参数
uint8 g_delay_time;

//audible专辑图片数目
uint16 g_image_num;

//控件返回值
app_result_e g_mengine_result;

//引擎配置变量结构体指针
mengine_config_t *g_eg_cfg_p;

//引擎状态变量结构体指针
mengine_status_t *g_eg_status_p;

//引擎播放信息结构体指针
mengine_playinfo_t *g_eg_playinfo_p;

//私有消息全局变量
private_msg_t g_cur_msg;

//app_timer_vector：应用软定时器数组指针，指向应用空间的软定时器数组
app_timer_t mengine_app_timer_vector[APP_TIMER_COUNT];

#ifndef PC
OS_STK *ptos = (OS_STK *) AP_MENGINE_STK_POS;
#endif

INT8U prio = AP_MENGINE_PRIO;

//for audible test
#define AudibleExtNum 2

const uint8 audible_ext[AudibleExtNum][4] =
{ "AAX", "AA " };

/******************************************************************************/
/*!
 * \par  Description:
 * \void _load_cfg(void)
 * \读取配置信息
 * \param[in]    void  para1
 * \param[out]   none
 * \return       void the result
 * \retval
 * \retval
 * \ingroup      mengine_main.c
 * \note
 */
/*******************************************************************************/
void _load_cfg(void)
{

    //系统变量获取
    //sys_vm_read(&g_setting_comval, VM_AP_SETTING, sizeof(setting_comval_t));
    //sys_vm_read(&g_comval, VM_AP_SETTING, sizeof(comval_t));

    //music engine 变量获取
    sys_vm_read(&mengine_info.eg_config, VM_AP_MENGINE, sizeof(mengine_config_t));
    if (mengine_info.eg_config.magic != MAGIC_MENGINE)
    {
        mengine_info.eg_config.magic = MAGIC_MENGINE;
        //文件循环模式
        mengine_info.eg_config.repeat_mode = FSEL_MODE_LOOPALL;
        //shuffle flag
        mengine_info.eg_config.shuffle_flag = 0;
        //播放通道模式，0：正常，1：只输出左声道，2：只输出右声道，3：左右声道互换
        mengine_info.eg_config.chanel_select_mode = 0;
        //文件选择类型 列表/收藏夹/audible/目录
        //mengine_info.eg_config.fsel_type = FSEL_TYPE_PLAYLIST;
        mengine_info.eg_config.fsel_type = FSEL_TYPE_COMMONDIR;
        //淡出时间单位毫秒
        mengine_info.eg_config.fade_out_time = 0;
        //淡入时间单位毫秒
        mengine_info.eg_config.fade_in_time = 0;
        //快进步长
        mengine_info.eg_config.fff_step = 2;
        //audible章节模式
        mengine_info.eg_config.section_mod = 0;
        //当前文件序号
        mengine_info.eg_config.file_cnt.num = 0;
        //当前文件章节总数
        mengine_info.eg_config.file_cnt.total = 0;
        //(记录当前文件的路径信息/列表文件的层级关系)
        libc_memset(&mengine_info.eg_config.location.dirlocation, 0, sizeof(musfile_location_u));

        //断点信息
        libc_memset(&mengine_info.eg_config.bk_infor, 0, sizeof(mmm_mp_bp_info_t));
    }

    if (mengine_enter_mode == ENTER_START)
    {
        mengine_info.eg_config.fsel_type = FSEL_TYPE_COMMONDIR;
        //(记录当前文件的路径信息/列表文件的层级关系)
        libc_memset(&mengine_info.eg_config.location.dirlocation, 0, sizeof(musfile_location_u));
        mengine_info.eg_config.location.dirlocation.disk = (uint8) g_open_disk;
    }
    else
    {
        //检测文件有效性
        if (mengine_info.eg_config.location.dirlocation.disk == DISK_H)
        {
            if (sys_detect_disk(DRV_GROUP_STG_CARD) == -1)
            {
                mengine_info.eg_config.fsel_type = FSEL_TYPE_COMMONDIR;
                libc_memset(&mengine_info.eg_config.location.dirlocation, 0, sizeof(musfile_location_u));
                mengine_info.eg_config.location.dirlocation.disk = DISK_C;
            }
        }

        //需要主动获取文件信息
        mengine_info.eg_playinfo.cur_file_switch |= 0x01;
    }
    //mengine_info.eg_config.volume = g_comval.volume_current;
    g_image_num = 0xffff;

#ifdef PC
    g_audible_info_p = (audible_para_t *) (GET_REAL_ADDR(0x2f000));
    g_send_audible_p = (audible_para_t *) (GET_REAL_ADDR(0x37000));
#endif
}

/******************************************************************************/
/*!
 * \par  Description:
 * \void _save_cfg(void)
 * \保存配置信息
 * \param[in]    void  para1
 * \param[out]   none
 * \return       void the result
 * \retval
 * \retval
 * \ingroup      mengine_main.c
 * \note
 */
/*******************************************************************************/
void _save_cfg(void)
{
    /*保存VM变量*/
    //setting_comval_t temp_val;
    if (mengine_enter_mode != ENTER_ALARM)
    {
        //闹钟应用不保存任何变量
        sys_vm_write(&mengine_info.eg_config, VM_AP_MENGINE);
        //sys_vm_read(&temp_val, VM_AP_SETTING, sizeof(setting_comval_t));
        //注意只更新引擎需要的音量值，其它内容均不改变
        //否则有可能前台更新的内容被引擎覆盖
        //temp_val.g_comval.volume_current = g_comval.volume_current;
        //sys_vm_write(&temp_val, VM_AP_SETTING);
    }
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool mengine_file_init(void)
 * \初始化文件选择器,定位到设定文件or当前模式第一个可播放文件
 * \param[in]    void  para1
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      mengine_main.c
 * \note
 */
/*******************************************************************************/
bool mengine_file_init(void)
{
    fsel_param_t init_param;//文件选择器初始化参数
    uint8 exname[4];

    plist_location_t* loaction_ptr;
    fsel_type_e file_source;
    bool init_result = FALSE;
    uint8 loop_cnt = 0;
    mengine_config_t *cfg_ptr = &mengine_info.eg_config;

    //获取后缀名配置
    //get_config(CFG_MUSIC_FILE_EXT,init_param.file_type_bit);

    loaction_ptr = &(cfg_ptr->location.plist_location);

    //文件选择器初始化接口的参数设置

    //盘符信息
    init_param.disk = loaction_ptr->disk;

    file_source = cfg_ptr->fsel_type;

    init_param.fsel_type = file_source;

    init_param.fsel_mode = (uint8)(cfg_ptr->repeat_mode | cfg_ptr->shuffle_flag);

    //判断是否是audible
    for (loop_cnt = 0; loop_cnt < AudibleExtNum; loop_cnt++)
    {
        if (libc_strncmp(loaction_ptr->filename, audible_ext[loop_cnt], sizeof(loaction_ptr->filename)) == 0)
        {
            g_is_audible = TRUE;
            break;
        }
    }
    if (loop_cnt == AudibleExtNum)
    {
        g_is_audible = FALSE;
    }

#ifdef PC
    //模拟器不支持真正的audible文件播放
    //因此所有的引擎均走普通文件分支
    g_is_audible = FALSE;
#endif

    //bitmap 设置
    if (g_is_audible == TRUE)
    {
        init_param.file_type_bit = AUDIBLE_BITMAP;
        init_param.fsel_mode = FSEL_MODE_NORMAL;
        cfg_ptr->repeat_mode = FSEL_MODE_NORMAL;
        cfg_ptr->shuffle_flag = 0;
    }
    else
    {
        init_param.file_type_bit = MUSIC_BITMAP;
    }

    file_type_cfg = init_param.file_type_bit;

    //文件选择器初始化
    file_sys_id = fsel_init(&init_param, 0);
    if (file_sys_id != -1)
    {
        //设置上次退出的文件
        init_result = fsel_set_location(loaction_ptr, file_source);

        if (init_result == TRUE)
        {
            //比较后缀名
            vfs_get_name(file_sys_id, exname, 0);

            if (libc_strncmp(exname, loaction_ptr->filename, sizeof(exname)) != 0)
            {
                init_result = FALSE;
            }
        }

        //设置失败
        if (init_result == FALSE)
        {
            //清除断点信息
            libc_memset(&cfg_ptr->bk_infor, 0, sizeof(mmm_mp_bp_info_t));

            //获取当前模式下的第一个可播放文件
            init_result = fsel_get_nextfile(loaction_ptr->filename);
            //找到可播放文件
            if (init_result == TRUE)
            {
                //获取当前文件的location信息
                init_result = fsel_get_location(loaction_ptr, file_source);
            }
            else
            {
                //没有可播放文件
                loaction_ptr->file_total = 0;
            }
        }
    }

    return init_result;

}

/******************************************************************************/
/*!
 * \par  Description:
 * \int _app_init(void)
 * \初始化引擎、读取配置信息、初始化文件选择器、中间件
 * \param[in]    void  para1
 * \param[out]   none
 * \return       int the result
 * \retval           1 sucess
 * \retval          -1 failed
 * \ingroup      mengine_main.c
 * \note
 */
/*******************************************************************************/
int _app_init(void)
{
    int app_init_ret = 0;
    bool init_fsel_ret;
    char* mmm_name =
    { "mmm_mp.al" };

    //读取配置信息 (包括vm信息)
    _load_cfg();

    //初始化applib库，后台AP
    applib_init(APP_ID_MENGINE, APP_TYPE_CONSOLE);

    //初始化 applib 消息模块
    applib_message_init(NULL);

    //初始化软定时器
    init_app_timers(mengine_app_timer_vector, APP_TIMER_COUNT);

    //加载中间件驱动
    sys_load_mmm(mmm_name, TRUE);//待完善
    //初始化文件获取可播放文件/断点
    init_fsel_ret = mengine_file_init();

    //初始化中间件库
    app_init_ret = mmm_mp_cmd(&mp_handle, MMM_MP_OPEN, (unsigned int) NULL);
    if (app_init_ret == 0)
    {
        if (init_fsel_ret == TRUE)
        {
            _file_switch_info();
        }

        if (mengine_enter_mode == ENTER_START)
        {
            //ENTER_START模式立即开启PA,其它模式等到播放界面刷出
            //之后再开启PA
            com_set_sound_out(TRUE, SOUND_OUT_START, NULL);
            g_eg_playinfo_p->cur_time = 0;
        }
        else
        {
            g_eg_playinfo_p->cur_time = (uint32)g_eg_cfg_p->bk_infor.bp_time_offset;
        }
    }

    g_delay_time = 2;
    //第一次进入立即查询状态，以更新mengine内容
    g_check_status_flag = TRUE;

    return app_init_ret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool _app_deinit(void)
 * \退出app的功能函数,保存配置信息
 * \param[in]    void  para1
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      mengine_main.c
 * \note
 */
/*******************************************************************************/
bool _app_deinit(void)
{
    //在引擎退出前修改卡状态
    if (mengine_info.eg_config.location.plist_location.disk == DISK_H)
    {
        if (sys_detect_disk(DRV_GROUP_STG_CARD) != -1)
        {
            //存在卡盘
            //当前盘符是卡盘,设置状态为卡未播放状态
            change_card_state(CARD_STATE_CARD_IN);
        }
        else
        {
            change_card_state(CARD_STATE_CARD_NULL);
        }

    }

    //关闭文件选择器
    fsel_exit();

    //关闭声音设备
    //mmm_mp_cmd(mp_handle, MMM_MP_AOUT_CLOSE, (unsigned int) NULL);
    //关闭中间件设备
    mmm_mp_cmd(mp_handle, MMM_MP_CLOSE, (unsigned int) NULL);

    //disable_dac();
    //disable_pa();
    com_set_sound_out(FALSE, SOUND_OUT_STOP, NULL);

    //执行applib库的注销操作
    applib_quit();

    //save config 写vram
    _save_cfg();
    //卸载解码驱动
    sys_free_mmm(TRUE);
    return TRUE;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \int main(int argc, const char *argv[])
 * \app入口函数
 * \param[in]    argc  para1
 * \param[in]    argc  para2
 * \param[out]   none
 * \return       int the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      mengine_main.c
 * \note
 */
/*******************************************************************************/
int main(int argc, const char *argv[])
{
    mengine_enter_mode = (mengine_enter_mode_e)(argc & 0xc0);

    //从alarm进入，则重新修改为ENTER_START
    //alarm歌曲播放不需要支持断点播放，不需要保存location
    if (mengine_enter_mode == ENTER_ALARM)
    {
        mengine_enter_mode = ENTER_START;
    }

    g_open_disk = (uint8)(argc & 0x3f);

    g_eg_cfg_p = &mengine_info.eg_config;

    g_eg_status_p = &mengine_info.eg_status;

    g_eg_playinfo_p = &mengine_info.eg_playinfo;

    //初始化
    //if (_app_init() == 1)
    _app_init();
    {
        g_mengine_result = mengine_control_block();//app功能处理z
    }
    //重新恢复enter_mode
    mengine_enter_mode = (mengine_enter_mode_e)(argc & 0xc0);
    _app_deinit();//退出

    return g_mengine_result;
}
