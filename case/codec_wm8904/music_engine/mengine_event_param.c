/*******************************************************************************
 *                              US212A
 *                            Module: music engine
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *      fiona.yang  2011-09-07        1.0              create this file
 *******************************************************************************/

#include "app_mengine.h"

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e mengine_start_ab(void* msg_ptr)
 * \启动ab复读,等待设置a点
 * \param[in]    msg_ptr消息指针  para1
 * \param[in]    private_msg_t
 *               typedef struct
 *               {
 *                 unsigned char sync;		//同步标志，1B
 *                 sem_id_t sem_id; 		  //信号量ID，1B
 *                 msg_apps_t msg;		    //消息内容，6B
 *               } private_msg_t;
 *               typedef struct
 *               {
 *                 msg_apps_type_e  type;	//消息类型，2B
 *                 union
 *                 {
 *	                 unsigned char data[4];
 *                   unsigned char *addr;
 *                 }content;					//消息内容或其地址，由消息自己约定，4B
 *               } msg_apps_t;//私有消息队列的消息结构体

 * \param[out]   none
 * \return       int the result
 * \retval       RESULT_IGNORE 忽略
 * \retval       ......
 * \ingroup      mengine_event.c
 * \note
 */
/*******************************************************************************/
app_result_e mengine_start_ab(void* msg_ptr)
{
    mengine_status_t *eg_status = &mengine_info.eg_status;

    if (eg_status->play_status == PlaySta)
    {
        eg_status->ab_status = PlayAB_A;//等待设置A点

        eg_status->play_status = PlayAB;

        //设置复读次数到解码器
        //mmm_mp_cmd(mp_handle, MMM_MP_AB_COUNT, (unsigned int)(g_eg_cfg_p->replaytimes));

        //设置复读间隔到解码器  10ms为单位
        //mmm_mp_cmd(mp_handle, MMM_MP_AB_WAIT_TIME, (unsigned int) (g_eg_cfg_p->replaygap * 100));

    }

    //返回成功
    mengine_reply_msg(msg_ptr, TRUE);

    return RESULT_IGNORE;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e mengine_set_apoint(void* msg_ptr)
 * \设置a点
 * \param[in]    msg_ptr消息指针  para1
 * \param[in]    private_msg_t
 *               typedef struct
 *               {
 *                 unsigned char sync;		//同步标志，1B
 *                 sem_id_t sem_id; 		  //信号量ID，1B
 *                 msg_apps_t msg;		    //消息内容，6B
 *               } private_msg_t;
 *               typedef struct
 *               {
 *                 msg_apps_type_e  type;	//消息类型，2B
 *                 union
 *                 {
 *	                 unsigned char data[4];
 *                   unsigned char *addr;
 *                 }content;					//消息内容或其地址，由消息自己约定，4B
 *               } msg_apps_t;//私有消息队列的消息结构体

 * \param[out]   none
 * \return       int the result
 * \retval       RESULT_IGNORE 忽略
 * \retval       ......
 * \ingroup      mengine_event.c
 * \note
 */
/*******************************************************************************/
app_result_e mengine_set_apoint(void* msg_ptr)
{
    mengine_status_t *eg_status = &mengine_info.eg_status;

    eg_status->ab_status = PlayAB_B;//等待设置B点

    eg_status->play_status = PlayAB;

    mmm_mp_cmd(mp_handle, MMM_MP_SET_A, (unsigned int) NULL);//发送设置A点命令

    //返回成功
    mengine_reply_msg(msg_ptr, TRUE);

    return RESULT_IGNORE;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e mengine_set_bpoint(void* msg_ptr)
 * \设置b点,开始复读
 * \param[in]    msg_ptr消息指针  para1
 * \param[in]    private_msg_t
 *               typedef struct
 *               {
 *                 unsigned char sync;		//同步标志，1B
 *                 sem_id_t sem_id; 		  //信号量ID，1B
 *                 msg_apps_t msg;		    //消息内容，6B
 *               } private_msg_t;
 *               typedef struct
 *               {
 *                 msg_apps_type_e  type;	//消息类型，2B
 *                 union
 *                 {
 *	                 unsigned char data[4];
 *                   unsigned char *addr;
 *                 }content;					//消息内容或其地址，由消息自己约定，4B
 *               } msg_apps_t;//私有消息队列的消息结构体

 * \param[out]   none
 * \return       int the result
 * \retval       RESULT_IGNORE 忽略
 * \retval       ......
 * \ingroup      mengine_event.c
 * \note
 */
/*******************************************************************************/
app_result_e mengine_set_bpoint(void* msg_ptr)
{
    mengine_status_t *eg_status = &mengine_info.eg_status;

    eg_status->ab_status = PlayAB_Do;//开始ab复读

    eg_status->play_status = PlayAB;//设置播放状态为AB复读

    mmm_mp_cmd(mp_handle, MMM_MP_SET_B, (unsigned int) NULL);//发送设置B点命令

    //返回成功
    mengine_reply_msg(msg_ptr, TRUE);

    return RESULT_IGNORE;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e mengine_clear_ab(void* msg_ptr)
 * \清除ab复读
 * \param[in]    msg_ptr消息指针  para1
 * \param[in]    private_msg_t
 *               typedef struct
 *               {
 *                 unsigned char sync;		//同步标志，1B
 *                 sem_id_t sem_id; 		  //信号量ID，1B
 *                 msg_apps_t msg;		    //消息内容，6B
 *               } private_msg_t;
 *               typedef struct
 *               {
 *                 msg_apps_type_e  type;	//消息类型，2B
 *                 union
 *                 {
 *	                 unsigned char data[4];
 *                   unsigned char *addr;
 *                 }content;					//消息内容或其地址，由消息自己约定，4B
 *               } msg_apps_t;//私有消息队列的消息结构体

 * \param[out]   none
 * \return       int the result
 * \retval       RESULT_IGNORE 忽略
 * \retval       ......
 * \ingroup      mengine_event.c
 * \note
 */
/*******************************************************************************/
app_result_e mengine_clear_ab(void* msg_ptr)
{
    mengine_status_t *eg_status = &mengine_info.eg_status;

    eg_status->ab_status = PlayAB_Null;//清除AB复读状态

    eg_status->play_status = PlaySta;//恢复正常播放状态

    mmm_mp_cmd(mp_handle, MMM_MP_CLEAR_AB, (unsigned int) NULL);//发送清除AB复读命令

    //返回成功
    mengine_reply_msg(msg_ptr, TRUE);

    return RESULT_IGNORE;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e mengine_set_section_mode(void* msg_ptr)
 * \设置audible的章节模式/标题模式
 * \param[in]    msg_ptr消息指针  para1
 * \param[in]    private_msg_t
 *               typedef struct
 *               {
 *                 unsigned char sync;		//同步标志，1B
 *                 sem_id_t sem_id; 		  //信号量ID，1B
 *                 msg_apps_t msg;		    //消息内容，6B
 *               } private_msg_t;
 *               typedef struct
 *               {
 *                 msg_apps_type_e  type;	//消息类型，2B
 *                 union
 *                 {
 *	                 unsigned char data[4];
 *                   unsigned char *addr;
 *                 }content;					//消息内容或其地址，由消息自己约定，4B
 *               } msg_apps_t;//私有消息队列的消息结构体

 * \param[out]   msg_ptr->msg.content.addr //传送eq信息的指针
 * \return       int the result
 * \retval       RESULT_IGNORE 忽略
 * \retval       ......
 * \ingroup      mengine_event.c
 * \note
 */
/*******************************************************************************/
app_result_e mengine_set_section_mode(void* msg_ptr)
{
    //设置内容指针
    private_msg_t* data_ptr = (private_msg_t*) msg_ptr;

    uint8 last_mode = mengine_info.eg_config.section_mod;

    //将消息指针指向的数据copy到引擎变量
    libc_memcpy(&mengine_info.eg_config.section_mod, data_ptr->msg.content.addr, sizeof(mmm_mp_eq_info_t));
    //待修改
    mmm_mp_cmd(mp_handle, MMM_MP_SET_CHAPTER, (unsigned int) mengine_info.eg_config.section_mod);

    //从文件模式切换到章节模式
    if ((last_mode == SWITCH_TITLE) && (mengine_info.eg_config.section_mod == SWITCH_SECTION))
    {
        _check_audible_chapter();
        if (mengine_info.eg_status.play_status == PlaySta)
        {
            _set_chapter_timer();
        }
        mengine_info.eg_playinfo.cur_file_switch |= 0x02;
    }

    //从章节模式切换到文件模式
    if ((mengine_info.eg_config.section_mod == SWITCH_TITLE) && (last_mode == SWITCH_SECTION))
    {
        //mengine_info.eg_config.location.plist_location.file_total = audbile_tracks.locat_total;
        //mengine_info.eg_config.location.plist_location.file_num = audbile_tracks.loact_num;
        _clr_chapter_timer();
    }

    //返回成功
    mengine_reply_msg(msg_ptr, TRUE);

    return RESULT_IGNORE;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e mengine_get_bkinfo(void* msg_ptr)
 * \获取当前音乐断点信息
 * \param[in]    msg_ptr消息指针  para1
 * \param[in]    private_msg_t
 *               typedef struct
 *               {
 *                 unsigned char sync;		//同步标志，1B
 *                 sem_id_t sem_id; 		  //信号量ID，1B
 *                 msg_apps_t msg;		    //消息内容，6B
 *               } private_msg_t;
 *               typedef struct
 *               {
 *                 msg_apps_type_e  type;	//消息类型，2B
 *                 union
 *                 {
 *	                 unsigned char data[4];
 *                   unsigned char *addr;
 *                 }content;					//消息内容或其地址，由消息自己约定，4B
 *               } msg_apps_t;//私有消息队列的消息结构体

 * \param[out]   msg_ptr->msg.content.addr //传送循环方式的指针
 * \return       int the result
 * \retval       RESULT_IGNORE 忽略
 * \retval       ......
 * \ingroup      mengine_event.c
 * \note
 */
/*******************************************************************************/
app_result_e mengine_get_bkinfo(void* msg_ptr)
{
    //消息指针
    private_msg_t* data_ptr = (private_msg_t*) msg_ptr;

    mengine_status_t *eg_status = &mengine_info.eg_status;
    mengine_config_t *eg_cfg = &mengine_info.eg_config;
    //正在播放
    if (eg_status->play_status == PlaySta)
    {
        //获取断点
        mmm_mp_cmd(mp_handle, MMM_MP_GET_BREAK_POINT, (unsigned int) &(eg_cfg->bk_infor));
    }
    //copy当前播放时间和比特率到消息指针指向的数据区
    libc_memcpy(data_ptr->reply->content, &(eg_cfg->bk_infor), sizeof(mmm_mp_bp_info_t));

    //返回成功
    mengine_reply_msg(msg_ptr, TRUE);

    return RESULT_IGNORE;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e mengine_play_bookmark(void* msg_ptr)
 * \设置书签播放
 * \param[in]    msg_ptr消息指针  para1
 * \param[in]    private_msg_t
 *               typedef struct
 *               {
 *                 unsigned char sync;		//同步标志，1B
 *                 sem_id_t sem_id; 		  //信号量ID，1B
 *                 msg_apps_t msg;		    //消息内容，6B
 *               } private_msg_t;
 *               typedef struct
 *               {
 *                 msg_apps_type_e  type;	//消息类型，2B
 *                 union
 *                 {
 *	                 unsigned char data[4];
 *                   unsigned char *addr;
 *                 }content;					//消息内容或其地址，由消息自己约定，4B
 *               } msg_apps_t;//私有消息队列的消息结构体

 * \param[out]   none
 * \return       int the result
 * \retval       RESULT_IGNORE 忽略
 * \retval       ......
 * \ingroup      mengine_event.c
 * \note
 */
/*******************************************************************************/
app_result_e mengine_play_bookmark(void* msg_ptr)
{
    bool ret_vals;

    //消息指针
    private_msg_t* data_ptr = (private_msg_t*) msg_ptr;

    //mengine_status_t *eg_status = &mengine_info.eg_status;
    mengine_config_t *eg_cfg = &mengine_info.eg_config;

    //void * data_ptr = msg_ptr->msg.content.addr;//存放书签断点的数据指针

    //当前是播放状态,则停止播放

    ret_vals = _stop(STOP_PAUSE);//暂停播放,由内部保证是播放状态才停止
    if (ret_vals == FALSE)
    {
        goto msg_end;
    }

    //保存断点,必须在_stop之后,因为_stop中会获取断点，放在_stop之前断点会被覆盖
    libc_memcpy(&eg_cfg->bk_infor, data_ptr->msg.content.addr, sizeof(mmm_mp_bp_info_t));

    //重新开始播放, 设置文件
    //ret_vals = _set_file();
    //if (ret_vals == FALSE)
    //{
    //    goto msg_end;
    //}

    ret_vals = _play(PLAY_RESUME);//播放

    msg_end:
    //返回成功
    mengine_reply_msg(msg_ptr, TRUE);

    return RESULT_IGNORE;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e mengine_set_playmode(void* msg_ptr)
 * \设置循环模式
 * \param[in]    msg_ptr消息指针  para1
 * \param[in]    private_msg_t
 *               typedef struct
 *               {
 *                 unsigned char sync;		//同步标志，1B
 *                 sem_id_t sem_id; 		  //信号量ID，1B
 *                 msg_apps_t msg;		    //消息内容，6B
 *               } private_msg_t;
 *               typedef struct
 *               {
 *                 msg_apps_type_e  type;	//消息类型，2B
 *                 union
 *                 {
 *	                 unsigned char data[4];
 *                   unsigned char *addr;
 *                 }content;					//消息内容或其地址，由消息自己约定，4B
 *               } msg_apps_t;//私有消息队列的消息结构体

 * \param[out]   msg_ptr->msg.content.addr //传送循环方式的指针
 * \return       int the result
 * \retval       RESULT_IGNORE 忽略
 * \retval       ......
 * \ingroup      mengine_event.c
 * \note
 */
/*******************************************************************************/
app_result_e mengine_set_playmode(void* msg_ptr)
{
    //设置循环模式的指针
    private_msg_t* data_ptr = (private_msg_t*) msg_ptr;
    uint8 cur_mode;
    uint8 old_mode;
    mengine_status_t *eg_status = &mengine_info.eg_status;

    cur_mode = *(uint8*) (data_ptr->msg.content.addr);//获取当前循环模式
    old_mode = mengine_info.eg_config.repeat_mode;

    mengine_info.eg_config.shuffle_flag = (uint8)(cur_mode & 0xf0);//保存shuffle模式
    mengine_info.eg_config.repeat_mode = (cur_mode & 0x0f);//保存循环模式

    if ((eg_status->play_status == StopSta) || (eg_status->play_status == PauseSta))
    {
        fsel_set_mode(cur_mode);//设置循环模式
    }
    else
    {
        //只有当前模式更改为浏览模式而之前模式不为浏览模式时才进行设置
        if ((mengine_info.eg_config.repeat_mode == FSEL_MODE_INTRO) && (old_mode != FSEL_MODE_INTRO))
        {
            mmm_mp_status_t mmm_status;
            mmm_mp_cmd(mp_handle, MMM_MP_GET_STATUS, (unsigned int) &mmm_status);
            g_intro_start = (uint32) mmm_status.cur_time;
        }
    }

    //返回成功
    mengine_reply_msg(msg_ptr, TRUE);

    return RESULT_IGNORE;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e mengine_set_ab_count(void* msg_ptr)
 * \设置ab复读次数
 * \param[in]    msg_ptr消息指针  para1
 * \param[in]    private_msg_t
 *               typedef struct
 *               {
 *                 unsigned char sync;		//同步标志，1B
 *                 sem_id_t sem_id; 		  //信号量ID，1B
 *                 msg_apps_t msg;		    //消息内容，6B
 *               } private_msg_t;
 *               typedef struct
 *               {
 *                 msg_apps_type_e  type;	//消息类型，2B
 *                 union
 *                 {
 *	                 unsigned char data[4];
 *                   unsigned char *addr;
 *                 }content;					//消息内容或其地址，由消息自己约定，4B
 *               } msg_apps_t;//私有消息队列的消息结构体

 * \param[out]   msg_ptr->msg.content.addr //传送ab复读次数的指针
 * \return       int the result
 * \retval       RESULT_IGNORE 忽略
 * \retval       ......
 * \ingroup      mengine_event.c
 * \note
 */
/*******************************************************************************/
app_result_e mengine_set_ab_count(void* msg_ptr)
{
    //设置复读次数的指针
    private_msg_t* data_ptr = (private_msg_t*) msg_ptr;

    uint16 rep_count;

    //获取当前复读次数
    rep_count = *(uint16*) (data_ptr->msg.content.addr);

    //mengine_info.eg_config.replaytimes = rep_count;//保存复读次数

    //设置复读次数到解码器
    mmm_mp_cmd(mp_handle, MMM_MP_AB_COUNT, (unsigned int) rep_count);

    //返回成功
    mengine_reply_msg(msg_ptr, TRUE);

    return RESULT_IGNORE;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e mengine_set_ab_count(void* msg_ptr)
 * \设置ab复读间隔时间
 * \param[in]    msg_ptr消息指针  para1
 * \param[in]    private_msg_t
 *               typedef struct
 *               {
 *                 unsigned char sync;		//同步标志，1B
 *                 sem_id_t sem_id; 		  //信号量ID，1B
 *                 msg_apps_t msg;		    //消息内容，6B
 *               } private_msg_t;
 *               typedef struct
 *               {
 *                 msg_apps_type_e  type;	//消息类型，2B
 *                 union
 *                 {
 *	                 unsigned char data[4];
 *                   unsigned char *addr;
 *                 }content;					//消息内容或其地址，由消息自己约定，4B
 *               } msg_apps_t;//私有消息队列的消息结构体

 * \param[out]   msg_ptr->msg.content.addr //传送ab复读间隔的指针
 * \return       int the result
 * \retval       RESULT_IGNORE 忽略
 * \retval       ......
 * \ingroup      mengine_event.c
 * \note
 */
/*******************************************************************************/
app_result_e mengine_set_ab_gap(void* msg_ptr)
{
    //设置复读间隔的指针
    private_msg_t* data_ptr = (private_msg_t*) msg_ptr;
    uint8 ab_gap;

    ab_gap = *(uint8*) (data_ptr->msg.content.addr);//获取当前复读间隔

    //mengine_info.eg_config.replaygap = ab_gap;///保存复读间隔到引擎变量

    //设置复读间隔到解码器  10ms为单位
    mmm_mp_cmd(mp_handle, MMM_MP_AB_WAIT_TIME, (unsigned int) ab_gap * 100);

    //返回成功
    mengine_reply_msg(msg_ptr, TRUE);

    return RESULT_IGNORE;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e mengine_clear_error(void* msg_ptr)
 * \清除错误号
 * \param[in]    msg_ptr消息指针  para1
 * \param[in]    private_msg_t
 *               typedef struct
 *               {
 *                 unsigned char sync;		//同步标志，1B
 *                 sem_id_t sem_id; 		  //信号量ID，1B
 *                 msg_apps_t msg;		    //消息内容，6B
 *               } private_msg_t;
 *               typedef struct
 *               {
 *                 msg_apps_type_e  type;	//消息类型，2B
 *                 union
 *                 {
 *	                 unsigned char data[4];
 *                   unsigned char *addr;
 *                 }content;					//消息内容或其地址，由消息自己约定，4B
 *               } msg_apps_t;//私有消息队列的消息结构体

 * \param[out]   none
 * \return       int the result
 * \retval       RESULT_IGNORE 忽略
 * \retval       ......
 * \ingroup      mengine_event.c
 * \note
 */
/*******************************************************************************/
app_result_e mengine_clear_error(void* msg_ptr)
{
    mengine_status_t *eg_status = &mengine_info.eg_status;

    eg_status->err_status = EG_ERR_NONE;//错误号清除

    //返回成功
    mengine_reply_msg(msg_ptr, TRUE);

    return RESULT_IGNORE;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \void change_locat_deal(void)
 * \文件路径修改的处理
 * \param[in]    para1
 * \param[in]    none:
 * \param[out]   none
 * \return       the result
 * \ingroup      mengine_control.c
 * \note
 */
/*******************************************************************************/
bool change_locat_deal(void)
{
    bool ret_val;

    //配置指针
    mengine_config_t *eg_cfg = &mengine_info.eg_config;

    if (g_path_type != eg_cfg->fsel_type)
    {
        fsel_exit();
        ret_val = mengine_file_init();//重新初始化
    }
    else
    {
        ret_val = fsel_set_location(&(eg_cfg->location.plist_location), eg_cfg->fsel_type);
        if (ret_val == FALSE)
        {
            fsel_exit();
            ret_val = mengine_file_init();//重新初始化
        }
    }
    g_change_path_flag = 0;

    return ret_val;

}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e mengine_musui_delete_file(void* msg_ptr)
 * \删除当前播放的文件
 * \param[in]    msg_ptr消息指针  para1
 * \param[in]    private_msg_t
 *               typedef struct
 *               {
 *                 unsigned char sync;		//同步标志，1B
 *                 sem_id_t sem_id; 		  //信号量ID，1B
 *                 msg_apps_t msg;		    //消息内容，6B
 *               } private_msg_t;
 *               typedef struct
 *               {
 *                 msg_apps_type_e  type;	//消息类型，2B
 *                 union
 *                 {
 *	                 unsigned char data[4];
 *                   unsigned char *addr;
 *                 }content;					//消息内容或其地址，由消息自己约定，4B
 *               } msg_apps_t;//私有消息队列的消息结构体

 * \param[out]   none
 * \return       int the result
 * \retval       RESULT_IGNORE 忽略
 * \retval       ......
 * \ingroup      mengine_event.c
 * \note
 */
/*******************************************************************************/
app_result_e mengine_musui_delete_file(void* msg_ptr)
{
    bool is_playing;
    bool ret_vals;
    //bool switch_ret;

    plist_location_t* cur_locat_ptr = (plist_location_t*) &(g_eg_cfg_p->location.plist_location);

    //当前播放状态
    if (g_eg_status_p->play_status == PlaySta)
    {
        is_playing = TRUE;
    }
    else
    {
        is_playing = FALSE;
    }
    ret_vals = _stop(STOP_NORMAL);//停止播放
    if (ret_vals == FALSE)
    {
        goto msg_end;
    }

    cur_locat_ptr->file_num--;

    cur_locat_ptr->file_total--;

    //设置删除当前文件标识
    g_del_curfile = 1;

    if (g_change_path_flag == 1)
    {
        ret_vals = change_locat_deal();
    }

    //清除断点
    libc_memset(&(g_eg_cfg_p->bk_infor), 0, sizeof(mmm_mp_bp_info_t));

    msg_end:
    //返回成功
    mengine_reply_msg(msg_ptr, ret_vals);

    return RESULT_IGNORE;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e mengine_other_delete_file(void* msg_ptr)
 * \删除浏览的文件
 * \param[in]    msg_ptr消息指针  para1
 * \param[in]    private_msg_t
 *               typedef struct
 *               {
 *                 unsigned char sync;		//同步标志，1B
 *                 sem_id_t sem_id; 		  //信号量ID，1B
 *                 msg_apps_t msg;		    //消息内容，6B
 *               } private_msg_t;
 *               typedef struct
 *               {
 *                 msg_apps_type_e  type;	//消息类型，2B
 *                 union
 *                 {
 *	                 unsigned char data[4];
 *                   unsigned char *addr;
 *                 }content;					//消息内容或其地址，由消息自己约定，4B
 *               } msg_apps_t;//私有消息队列的消息结构体

 * \param[out]   none
 * \return       int the result
 * \retval       RESULT_IGNORE 忽略
 * \retval       ......
 * \ingroup      mengine_event.c
 * \note
 */
/*******************************************************************************/
app_result_e mengine_other_delete_file(void* msg_ptr)
{
    bool ret_vals;

    //消息指针
    private_msg_t* data_ptr = (private_msg_t*) msg_ptr;

    //要删除的文件信息指针
    file_path_info_t* del_file_path = (file_path_info_t *) (data_ptr->msg.content.addr);

    //当前正在播放的文件指针
    file_location_t* cur_locat_ptr = (file_location_t*) &(g_eg_cfg_p->location.dirlocation);

    uint32 del_clustno = del_file_path->file_path.dirlocation.cluster_no;//删除文件的目录项所在的簇号

    uint32 del_direntry = del_file_path->file_path.dirlocation.dir_entry;//删除文件的目录项在所在簇号内的偏移

    //判断是否是删除当前文件，如果是则停止播放
    if ((del_clustno == cur_locat_ptr->cluster_no) && (del_direntry == cur_locat_ptr->dir_entry))
    {
        ret_vals = _stop(STOP_NORMAL);//停止播放

        cur_locat_ptr->file_num--;

        cur_locat_ptr->file_total--;

        //设置删除当前文件标识
        g_del_curfile = 1;

        if (g_change_path_flag == 1)
        {
            ret_vals = change_locat_deal();
        }

        //清除断点
        libc_memset(&(g_eg_cfg_p->bk_infor), 0, sizeof(mmm_mp_bp_info_t));
    }
    //返回成功
    mengine_reply_msg(msg_ptr, TRUE);

    return RESULT_IGNORE;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e mengine_del_favorite_deal(void* msg_ptr)
 * \删除当前播放的收藏夹中的文件
 * \param[in]    msg_ptr消息指针  para1
 * \param[in]    private_msg_t
 *               typedef struct
 *               {
 *                 unsigned char sync;		//同步标志，1B
 *                 sem_id_t sem_id; 		  //信号量ID，1B
 *                 msg_apps_t msg;		    //消息内容，6B
 *               } private_msg_t;
 *               typedef struct
 *               {
 *                 msg_apps_type_e  type;	//消息类型，2B
 *                 union
 *                 {
 *	                 unsigned char data[4];
 *                   unsigned char *addr;
 *                 }content;					//消息内容或其地址，由消息自己约定，4B
 *               } msg_apps_t;//私有消息队列的消息结构体

 * \param[out]   none
 * \return       int the result
 * \retval       RESULT_IGNORE 忽略
 * \retval       ......
 * \ingroup      mengine_event.c
 * \note
 */
/*******************************************************************************/
app_result_e mengine_del_favorite_deal(void* msg_ptr)
{
    //删除文件序号的指针
    private_msg_t* data_ptr = (private_msg_t*) msg_ptr;
    //删除文件序号
    uint16 del_num;

    //当前正在播放的文件指针
    flist_location_t* cur_locat_ptr = (flist_location_t*) &(mengine_info.eg_config.location.flist_location);

    del_num = *(uint16*) (data_ptr->msg.content.addr);//获取当前删除文件序号

    if (cur_locat_ptr->file_num >= del_num)
    {
        cur_locat_ptr->file_num--;
    }
    cur_locat_ptr->file_total--;
    //删除文件
    g_del_curfile = 1;

    //返回成功
    mengine_reply_msg(msg_ptr, TRUE);

    return RESULT_IGNORE;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e mengine_add_favorite_deal(void* msg_ptr)
 * \添加当前播放的收藏夹中的文件
 * \param[in]    msg_ptr消息指针  para1
 * \param[in]    private_msg_t
 *               typedef struct
 *               {
 *                 unsigned char sync;		//同步标志，1B
 *                 sem_id_t sem_id; 		  //信号量ID，1B
 *                 msg_apps_t msg;		    //消息内容，6B
 *               } private_msg_t;
 *               typedef struct
 *               {
 *                 msg_apps_type_e  type;	//消息类型，2B
 *                 union
 *                 {
 *	                 unsigned char data[4];
 *                   unsigned char *addr;
 *                 }content;					//消息内容或其地址，由消息自己约定，4B
 *               } msg_apps_t;//私有消息队列的消息结构体

 * \param[out]   none
 * \return       int the result
 * \retval       RESULT_IGNORE 忽略
 * \retval       ......
 * \ingroup      mengine_event.c
 * \note
 */
/*******************************************************************************/
app_result_e mengine_add_favorite_deal(void* msg_ptr)
{
    //当前正在播放的文件指针
    flist_location_t* cur_locat_ptr = (flist_location_t*) &(mengine_info.eg_config.location.flist_location);

    cur_locat_ptr->file_total++;
    //返回成功
    mengine_reply_msg(msg_ptr, TRUE);

    return RESULT_IGNORE;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \int _error_timeout_handle(void* param)
 * \后台music播放时出错文件自动切换下一曲函数
 * \param[in]    param  para1
 * \param[out]   none
 * \return       int the result
 * \retval       RESULT_IGNOR 忽略
 * \retval
 * \ingroup      mengine_control.c
 * \note
 * \li
 */
/*******************************************************************************/
void _error_timeout_handle(void)
{
    //mengine_config_t *cfg_ptr = &mengine_info.eg_config;
    //mengine_status_t *status = &mengine_info.eg_status;
    uint8 loop_mode;
    uint16 loopcnt;
    bool ret_val;

    //codec和enhanced复用关系，需先关闭解码中间件，再更改循环模式
    _stop(STOP_NORMAL);

    //临时设置循环模式为所有循环
    loop_mode = g_eg_cfg_p->shuffle_flag;//循环模式shuffle
    loop_mode |= FSEL_MODE_LOOPALL;
    fsel_set_mode(loop_mode);

    loopcnt = fsel_get_total();//获取当前模式的文件总数

    //循环查找,直到找到可播放文件
    while (loopcnt > 0)
    {
        loopcnt--;
        ret_val = mengine_file_switch(STOP_NORMAL, NORMAL_SWITCH_NEXT, PLAY_NORMAL);
        if (ret_val == SWITCH_NO_ERR)
        {
            break;
        }
        else if (ret_val == SWITCH_ERR_PLAY)
        {
            //因此时歌曲未播放，如果播放出错，需调用CLEAR_FILE命令关闭文件句柄
            mmm_mp_cmd(mp_handle, MMM_MP_CLEAR_FILE, (unsigned int) NULL);
        }
        else
        {
            ;//nothing
        }

        sys_os_time_dly(g_delay_time);
    }

    if (loopcnt == 0)
    {
        //未找到有效可以播放的歌曲
        g_eg_cfg_p->location.dirlocation.file_total = 0;
    }

    //恢复默认的循环模式
    loop_mode |= (uint8) g_eg_cfg_p->repeat_mode;
    fsel_set_mode(loop_mode);
}
