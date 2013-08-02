/*******************************************************************************
 *                              US211A
 *                            Module: music engine
 *                 Copyright(c) 2003-2011 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>        <time>           <version >            <desc>
 *     fiona.yang     2011-11-03 15:00     1.0             build this file
 *******************************************************************************/
#include "app_mengine.h"

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e mengine_fastplay_step(void *msg_ptr)
 * \快进退步长控制
 * \param[in]    void  para1
 * \param[out]   none
 * \return       int the result
 * \retval       RESULT_IGNOR 忽略
 * \retval       Exit_App 退出app
 * \ingroup      mengine_mesasge_done.c
 * \note
 */
/*******************************************************************************/
app_result_e mengine_fastplay_step(void *msg_ptr)
{
    //消息指针
    private_msg_t* data_ptr = (private_msg_t*) msg_ptr;
    //速度参数 毫秒为单位
    uint32 *speed_ptr = (uint32 *) (data_ptr->msg.content.addr);

    //是正常播放 or 快进
    if ((g_eg_status_p->play_status == PlaySta) || (g_eg_status_p->play_status == PlayFast))
    {
        mmm_mp_cmd(mp_handle, MMM_MP_FFB_STEP, (unsigned int) (*speed_ptr));//快进
    }

    //返回成功
    mengine_reply_msg(msg_ptr, TRUE);

    return RESULT_IGNORE;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \uint16 mengine_message_done(void)
 * \引擎消息处理,处理引擎接收到的私有消息和系统消息
 * \param[in]    void  para1
 * \param[out]   none
 * \return       int the result
 * \retval       RESULT_IGNOR 忽略
 * \retval       Exit_App 退出app
 * \ingroup      mengine_mesasge_done.c
 * \note
 */
/*******************************************************************************/
app_result_e mengine_message_done_bank(private_msg_t* cur_msg)
{
    g_mengine_result = RESULT_NULL;

    switch (cur_msg->msg.type)
    {
        /*设置参数相关*/
        case MSG_MENGINE_SET_EQ_SYNC://设置eq +eq curve data
        mengine_set_eq(cur_msg);
        break;

        case MSG_MENGINE_SET_VOLUME_SYNC://设置音量
        mengine_set_volume(cur_msg);
        break;

        case MSG_MENGINE_SET_SECTION_MODE_SYNC://设置audible章节模式
        mengine_set_section_mode(cur_msg);
        break;

        case MSG_MENGINE_GET_BKINFO_SYNC://获取当前播放时间、比特率等信息
        mengine_get_bkinfo(&g_cur_msg);
        break;

        case MSG_MENGINE_SET_PLAYMODE_SYNC://设置循环模式+shuffle
        mengine_set_playmode(cur_msg);
        break;

        /*case MSG_MENGINE_SET_PLAYSPEED_SYNC://设置播放速度
         ret_value = mengine_set_playspeed(msg_sync_ptr);
         break;*/

        /*AB复读相关*/
        case MSG_MENGINE_SET_AB_COUNT_SYNC://设置AB复读次数
        mengine_set_ab_count(cur_msg);
        break;

        case MSG_MENGINE_SET_AB_GAP_SYNC://设置AB复读间隔
        mengine_set_ab_gap(cur_msg);
        break;

        case MSG_MENGINE_START_AB_SYNC://启动ab复读，等待设置a点
        mengine_start_ab(cur_msg);
        break;

        case MSG_MENGINE_SET_APOINT_SYNC://设置a点
        mengine_set_apoint(cur_msg);
        break;

        case MSG_MENGINE_SET_BPOINT_SYNC://设置b点，并开始复读
        mengine_set_bpoint(cur_msg);
        break;

        case MSG_MENGINE_CLEAR_AB_SYNC://清除ab复读
        mengine_clear_ab(cur_msg);
        break;

        /*播放音乐相关*/
        case MSG_MENGINE_PLAY_SYNC://开始播放音乐
        mengine_play(cur_msg);
        break;

        case MSG_MENGINE_STOP_SYNC://停止播放音乐
        mengine_stop(cur_msg);
        break;

        case MSG_MENGINE_PAUSE_SYNC://暂停播放音乐
        mengine_pause(cur_msg);
        break;

        case MSG_MENGINE_RESUME_SYNC://恢复播放音乐(从暂停到播放)
        mengine_resume(cur_msg);
        break;

        /*快进退相关*/
        case MSG_MENGINE_FFWD_SYNC:
        mengine_fast_forward(cur_msg);//快进
        break;

        case MSG_MENGINE_FBWD_SYNC:
        mengine_fast_backward(cur_msg);//快退
        break;

        case MSG_MENGINE_CANCEL_FFB_SYNC:
        mengine_cancel_ffb(cur_msg);//取消快进退
        break;

        case MSG_MENGINE_SET_FFBSTEP_SYNC:
        mengine_fastplay_step(cur_msg);//设置快进退步长
        break;

        /*选择文件播放*/
        case MSG_MENGINE_SET_FILEPATH_SYNC://设置播放文件的路径location
        mengine_set_filepath(cur_msg);
        break;

        case MSG_MENGINE_PLAY_NEXT_SYNC://播放下一曲
        mengine_play_next(cur_msg);
        break;

        case MSG_MENGINE_PLAY_PREV_SYNC://播放上一曲
        mengine_play_prev(cur_msg);
        break;

        case MSG_MENGINE_PLAY_BOOKMARK_SYNC://播放书签
        if (g_is_audible == TRUE)
        {
            mengine_audible_bookmark(cur_msg);
        }
        else
        {
            mengine_play_bookmark(cur_msg);
        }
        break;

        /*其他处理*/
        case MSG_MENGINE_DELFILE_MUSUI_SYNC://music播放场景删除文件
        mengine_musui_delete_file(cur_msg);
        break;

        case MSG_MENGINE_DELFILE_OTER_SYNC://其他地方删除文件
        mengine_other_delete_file(cur_msg);
        break;

        case MSG_MENGINE_CHANGE_FILEPATH_SYNC://改变文件播放路径
        mengine_change_filepath(cur_msg);
        break;

        case MSG_MENGINE_CLEAR_ERR_SYNC://清除错误号
        mengine_clear_error(cur_msg);
        break;

        case MSG_MENGINE_DEL_FAVORITE_SYNC://删除收藏夹
        mengine_del_favorite_deal(cur_msg);
        break;

        case MSG_MENGINE_ADD_FAVORITE_SYNC://添加收藏夹
        mengine_add_favorite_deal(cur_msg);
        break;

        case MSG_MENGINE_CAHPTER_NEXT_SYNC://章节切换到下一章
        mengine_chapter_next(cur_msg);
        break;

        case MSG_MENGINE_CHAPTER_PREV_SYNC://章节切换到上一章
        mengine_chapter_prev(cur_msg);
        break;

        case MSG_MENGINE_AUDIBLE_GETIMAGE_SYNC://获取当前文件的图片信息
        mengine_audible_getimage(cur_msg);
        break;

        case MSG_MENGINE_GET_SECTION_INFO_SYNC:
        mengine_audible_get_section_info(cur_msg);
        break;

        case MSG_MENGINE_STANDBY_PLAY_SYNC:
        mengine_standby_play(cur_msg);
        break;

        case MSG_APP_QUIT://退出app
        //exit_msg_loop();
        g_mengine_result = RESULT_APP_QUIT;
        break;

        default:
        break;
    }

    return g_mengine_result;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \eg_err_e _convert_error_no(mmm_mp_err_t err_no)
 * \转换解码库的错误号
 * \param[in]    err_no错误号  para1
 * \param[in]    mmm_mp_err_t:
 *							 typedef enum
 *							 {
 *							 MMM_MP_ERR_OPEN_FILE            = 0x10001,
 *							 MMM_MP_ERR_FILE_NOT_SUPPORT     = 0x10002,
 *							 MMM_MP_ERR_FILE_DAMAGED         = 0x10003,
 *							 MMM_MP_ERR_DECODER_ERROR        = 0x10004,
 *							 MMM_MP_ERR_NO_LICENSE           = 0x10005,
 *							 MMM_MP_ERR_SECURE_CLOCK         = 0x10006,
 *							 MMM_MP_ERR_LICENSE_INFO         = 0x10007,
 *							 MMM_MP_ERR_CMD_ILLEGAL          = 0x10008,
 *							 MMM_MP_ERR_CMD_NOTEXIST         = 0x10009,
 *							 MMM_MP_ERR_SEEK_BEHIND          = 0x1000A,
 *							 MMM_MP_ERR_PARSER_LOAD          = 0x1000B,
 *							 MMM_MP_ERR_DECODER_LOAD         = 0x1000C,
 *							 MMM_MP_ERR_FIFO_CREATE          = 0x1000D,
 *							 MMM_MP_ERR_TIME_OVERFLOW        = 0x1000E,
 *							 MMM_MP_ERR_IO_MISMATCH          = 0x1000F,
 *							 MMM_MP_ERR_FF_END               = 0x10010,
 *							 MMM_MP_ERR_FB_START             = 0x10011,
 *							 MMM_MP_ERR_PARSER_ERROR         = 0x10012,
 *							 MMM_MP_ERR_DEVICE_ERROR         = 0x10013,
 *							 MMM_MP_ERR_UNKNOWN              = 0x100FF
 *							 } mmm_mp_err_t;
 * \param[out]   none
 * \return       eg_err_e the result
 * \retval       typedef enum
 *							 {
 *							 EG_ERR_NONE = 0,
 *							 EG_ERR_OPEN_FILE,//打开文件失败
 *							 EG_ERR_NOT_SUPPORT,//文件格式不支持
 *							 EG_ERR_DECODER_ERROR,//解码出错
 *							 EG_ERR_NO_LICENSE,//无license，播放次数已满或是播放时间已经到期
 *							 EG_ERR_SECURE_CLOCK,//DRM时钟错误
 *							 EG_ERR_DRM_INFO//DRM信息错误
 *							 }eg_err_e;
 * \retval
 * \ingroup      mengine_mesasge_done.c
 * \note
 */
/*******************************************************************************/
eg_err_e _convert_error_no(mmm_mp_err_t err_no)
{
    eg_err_e err_status = EG_ERR_NONE; //当前错误状态

    switch (err_no)
    {
        case MMM_MP_ERR_OPEN_FILE:
        err_status = EG_ERR_OPEN_FILE;//打开文件失败
        break;

        case MMM_MP_ERR_FILE_NOT_SUPPORT://文件格式不支持
        case MMM_MP_ERR_FILE_DAMAGED://文件损毁
        err_status = EG_ERR_NOT_SUPPORT;
        break;

        case MMM_MP_ERR_DECODER_ERROR://解码出错
        err_status = EG_ERR_DECODER_ERROR;
        break;

        case MMM_MP_ERR_NO_LICENSE://无license
        err_status = EG_ERR_NO_LICENSE;
        break;

        case MMM_MP_ERR_SECURE_CLOCK://DRM时钟错误
        err_status = EG_ERR_SECURE_CLOCK;
        break;

        case MMM_MP_ERR_LICENSE_INFO://播放次数已满或是播放时间已经到期
        err_status = EG_ERR_DRM_INFO;
        break;

        default:
        err_status = EG_ERR_DECODER_ERROR;//解码出错
        break;
    }

    return err_status;
}
/******************************************************************************/
/*!
 * \par  Description:
 * \void mengine_save_errno_no(void)
 * \保存引擎错误号
 * \param[in]    void  para1
 * \param[out]   none
 * \return       none
 * \ingroup      mengine_mesasge_done.c
 * \note
 */
/*******************************************************************************/
void mengine_save_errno_no(void)
{
    eg_err_e err_status;
    mmm_mp_status_t mmm_status;

    //读取错误号
    mmm_mp_cmd(mp_handle, MMM_MP_GET_ERRORNO, (unsigned int) &mmm_status.err_no);
    //转换错误号
    err_status = _convert_error_no(mmm_status.err_no);
    if (err_status != EG_ERR_NONE)
    {
        //保存错误状态
        mengine_info.eg_status.err_status = err_status;
    }
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e mengine_play(void* msg_ptr)
 * \开始播放
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
app_result_e mengine_play(void* msg_ptr)
{
    bool ret_vals;

    //设置播放文件
    //ret_vals = _set_file();
    //if (ret_vals == TRUE)
    //{
    //设置文件播放
    ret_vals = _play(PLAY_NORMAL);//播放
    //}

    //if(g_is_audible == TRUE)
    //{
    //    g_image_num = 0xffff;
    //}

    mengine_info.eg_playinfo.cur_file_switch |= 0x01;

    mengine_reply_msg(msg_ptr, ret_vals);

    return RESULT_IGNORE;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e mengine_stop(void* msg_ptr)
 * \停止播放
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
app_result_e mengine_stop(void* msg_ptr)
{
    bool ret_vals;

    ret_vals = _stop(STOP_NORMAL);//停止播放

    change_engine_state(ENGINE_STATE_PAUSE);

    mengine_reply_msg(msg_ptr, ret_vals);

    return RESULT_IGNORE;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e mengine_pause(void* msg_ptr)
 * \暂停播放
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
app_result_e mengine_pause(void* msg_ptr)
{
    bool ret_vals;

    ret_vals = _stop(STOP_PAUSE);//暂停播放

    change_engine_state(ENGINE_STATE_PAUSE);

    mengine_reply_msg(msg_ptr, ret_vals);

    return RESULT_IGNORE;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e mengine_resume(void* msg_ptr)
 * \恢复播放(由暂停恢复)
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
app_result_e mengine_resume(void* msg_ptr)
{
    bool ret_vals;

    //重新开始播放, 设置文件
    //ret_vals = _set_file();
    //if (ret_vals == TRUE)
    //{
    ret_vals = _play(PLAY_RESUME);//播放
    //}
    //todo 暂时先这样修改
    change_engine_state(ENGINE_STATE_PLAYING);

    mengine_reply_msg(msg_ptr, ret_vals);

    return RESULT_IGNORE;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e mengine_play_switch(void *msg_ptr, uint8 mode)
 * \切换到上下曲
 * \param[in]    msg_ptr消息指针  para1
 * \param[in]    mode 切换模式  TRUE 下一曲 FALSE 上一曲
 * \param[out]   none
 * \return       int the result
 * \retval       RESULT_IGNORE 忽略
 * \retval       ......
 * \ingroup      mengine_event.c
 * \note
 */
/*******************************************************************************/
static app_result_e mengine_play_switch(void *msg_ptr, switch_mode_e switch_mode)
{
    bool bret;
    play_mode_e play_mode;//保存是否正在播放
    switch_result_e switch_ret;//切换是否成功

    if (g_eg_status_p->play_status == PlaySta)//正在播放
    {
        play_mode = PLAY_NORMAL;//正在播放标志
    }
    else
    {
        play_mode = PLAY_NO_PLAY;//未在播放
        //清除断点
        libc_memset(&(g_eg_cfg_p->bk_infor), 0, sizeof(mmm_mp_bp_info_t));
    }

    switch_ret = mengine_file_switch(STOP_NORMAL, switch_mode, play_mode);

    if (switch_ret == SWITCH_NO_ERR)
    {
        bret = TRUE;
    }
    else
    {
        bret = FALSE;
    }

    mengine_reply_msg(msg_ptr, bret);

    return RESULT_IGNORE;

}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e mengine_play_next(void* msg_ptr)
 * \切换到下一曲
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
app_result_e mengine_play_next(void* msg_ptr)
{
    return mengine_play_switch(msg_ptr, FORCE_SWITCH_NEXT);
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e mengine_play_prev(void* msg_ptr)
 * \切换到上一曲
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
app_result_e mengine_play_prev(void* msg_ptr)
{
    play_status_e play_status = g_eg_status_p->play_status;

    if (g_is_audible == FALSE)
    {
        if (g_eg_playinfo_p->cur_time > 5000)
        {
            g_eg_playinfo_p->cur_time = 0;
            
            //清除断点
            libc_memset(&(g_eg_cfg_p->bk_infor), 0, sizeof(mmm_mp_bp_info_t));
            
            _stop(STOP_NORMAL);//停止播放
            
            if (play_status == PlaySta)
            {
                _play(PLAY_NORMAL);//播放
                mengine_info.eg_playinfo.cur_file_switch = 0;
            }
            mengine_reply_msg(msg_ptr, TRUE);
            return RESULT_IGNORE;
        }
    }
    return mengine_play_switch(msg_ptr, FORCE_SWITCH_PREV);
}

