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

static const uint8 audible_ext[2][4] =
{ "AAX", "AA " };

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e mengine_get_config(void* msg_ptr)
 * \获取引擎当前的配置信息
 * \param[in]    msg_ptr消息指针  para1
 * \param[in]    private_msg_t
 *               typedef struct
 *               {
 *                 unsigned char sync;		//同步标志，1B
 *                 sem_id_t sem_id; 		  //信号量ID，1B
 *                 msg_reply_t *reply;    //同步消息回执指针
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

 * \param[out]   msg_ptr->msg.content.addr //存放引擎配置信息
 * \return       int the result
 * \retval       RESULT_IGNORE 忽略
 * \retval       ......
 * \ingroup      mengine_event.c
 * \note
 */
/*******************************************************************************/
/*app_result_e mengine_get_config(void* msg_ptr)
 {
 //消息指针
 private_msg_t* data_ptr = (private_msg_t*) msg_ptr;

 //copy引擎信息到消息指针指向的数据区
 libc_memcpy((data_ptr->reply->content), &mengine_info.eg_config, sizeof(mengine_config_t));

 //返回成功
 data_ptr->reply->type = MSG_REPLY_SUCCESS;

 //回复同步消息(发送信号量)
 reply_sync_msg(msg_ptr);

 return RESULT_IGNORE;
 }*/

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e mengine_get_filepath(void* msg_ptr)
 * \获取当前播放文件的location信息
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

 * \param[out]   msg_ptr->msg.content.addr //存放文件路径信息
 * \return       int the result
 * \retval       RESULT_IGNORE 忽略
 * \retval       ......
 * \ingroup      mengine_event.c
 * \note
 */
/*******************************************************************************/
app_result_e mengine_get_filepath(void* msg_ptr)
{
    uint8 location_size;
    //消息指针
    private_msg_t* data_ptr = (private_msg_t*) msg_ptr;

    //eg_config
    mengine_config_t* egcfg_ptr = (mengine_config_t*) &(mengine_info.eg_config);

    //返回路径的指针
    file_path_info_t* locat_info = (file_path_info_t*) (data_ptr->reply->content);

    //当前文件类型
    fsel_type_e file_source = egcfg_ptr->fsel_type;

    if ((file_source == FSEL_TYPE_PLAYLIST) || (file_source == FSEL_TYPE_LISTAUDIBLE) || (file_source
            == FSEL_TYPE_M3ULIST))
    {
        //是播放列表模式
        location_size = sizeof(plist_location_t);
    }
    else if ((file_source == FSEL_TYPE_COMMONDIR) || (file_source == FSEL_TYPE_DISKSEQUNCE))
    {
        //是目录模式
        location_size = sizeof(file_location_t);
    }
    else
    {
        //是收藏夹模式
        location_size = sizeof(flist_location_t);
    }
    libc_memcpy(&(locat_info->file_path), &(egcfg_ptr->location), (int) location_size);

    locat_info->file_source = egcfg_ptr->fsel_type;//当前文件模式

    mengine_reply_msg(msg_ptr, TRUE);

    return RESULT_IGNORE;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e mengine_get_fileinfo(void* msg_ptr)
 * \获取当前播放的文件格式、总时间、比特率等信息
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

 * \param[out]   msg_ptr->msg.content.addr //存放文件信息
 * \return       int the result
 * \retval       RESULT_IGNORE 忽略
 * \retval       ......
 * \ingroup      mengine_event.c
 * \note
 */
/*******************************************************************************/
app_result_e mengine_get_fileinfo(void* msg_ptr)
{
    //消息指针
    private_msg_t* data_ptr = (private_msg_t*) msg_ptr;

    //copy当前文件信息到消息指针指向的数据区
    libc_memcpy((data_ptr->reply->content), &mengine_info.eg_file_info, sizeof(mmm_mp_file_info_t));

    mengine_reply_msg(msg_ptr, TRUE);

    return RESULT_IGNORE;
}
/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e mengine_get_status(void* msg_ptr)
 * \获取当前播放状态
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

 * \param[out]   msg_ptr->msg.content.addr //存放状态信息
 * \return       int the result
 * \retval       RESULT_IGNORE 忽略
 * \retval       ......
 * \ingroup      mengine_event.c
 * \note
 */
/*******************************************************************************/
app_result_e mengine_get_status(void* msg_ptr)
{
    //消息指针
    private_msg_t* data_ptr = (private_msg_t*) msg_ptr;

    //copy当前播放状态到消息指针指向的数据区
    libc_memcpy(data_ptr->reply->content, &mengine_info.eg_status, sizeof(mengine_status_t));

    mengine_reply_msg(msg_ptr, TRUE);

    return RESULT_IGNORE;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e mengine_get_playmode(void* msg_ptr)
 * \获取循环模式
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
app_result_e mengine_get_playmode(void* msg_ptr)
{
    //消息指针
    private_msg_t* data_ptr = (private_msg_t*) msg_ptr;
    //保存shuffle模式+循环模式
    *(uint8*) (data_ptr->reply->content) = (uint8)(mengine_info.eg_config.shuffle_flag
            | mengine_info.eg_config.repeat_mode);

    //返回成功
    mengine_reply_msg(msg_ptr, TRUE);

    return RESULT_IGNORE;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e mengine_get_playinfo(void* msg_ptr)
 * \获取当前播放时间和比特率
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

 * \param[out]   msg_ptr->msg.content.addr //存放播放时间和比特率
 * \return       int the result
 * \retval       RESULT_IGNORE 忽略
 * \retval       ......
 * \ingroup      mengine_event.c
 * \note
 */
/*******************************************************************************/
app_result_e mengine_get_playinfo(void* msg_ptr)
{
    //消息指针
    private_msg_t* data_ptr = (private_msg_t*) msg_ptr;

    //copy当前播放时间和比特率到消息指针指向的数据区
    libc_memcpy(data_ptr->reply->content, &mengine_info.eg_playinfo, sizeof(mengine_playinfo_t));

    //if ((mengine_info.eg_status.play_status == PauseSta) || ((mengine_info.eg_status.play_status == StopSta)))
    {
        mengine_info.eg_playinfo.cur_imag_flag = 0;
    }

    //清除文件切换标志
    mengine_info.eg_playinfo.cur_file_switch &= (uint8)(0xfe);

    mengine_reply_msg(msg_ptr, TRUE);

    return RESULT_IGNORE;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e mengine_fast_forward(void* msg_ptr)
 * \快进
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
app_result_e mengine_fast_forward(void* msg_ptr)
{
    //当前播放状态
    mengine_status_t *eg_status = &mengine_info.eg_status;
    //当前不是AB复读
    //if (eg_status->ab_status == PlayAB_Null)
    //{
    //是正常播放 or 快进
    if ((eg_status->play_status == PlaySta) || (eg_status->play_status == PlayFast))
    {
        mmm_mp_cmd(mp_handle, MMM_MP_SET_FFB, (unsigned int) 4);//快进
        eg_status->play_status = PlayFast;//修改状态
        eg_status->fast_status = FFPlaySta;
    }
    //}
    //返回成功
    mengine_reply_msg(msg_ptr, TRUE);

    return RESULT_IGNORE;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e mengine_fast_backward(void* msg_ptr)
 * \快退
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
app_result_e mengine_fast_backward(void* msg_ptr)
{
    mengine_status_t *eg_status = &mengine_info.eg_status;
    //当前不是AB复读
    //if (eg_status->ab_status == PlayAB_Null)
    //{
    //当前是正常播放 or 快退
    if ((eg_status->play_status == PlaySta) || (eg_status->play_status == PlayFast))
    {
        mmm_mp_cmd(mp_handle, MMM_MP_SET_FFB, (unsigned int) -4);//快退
        eg_status->play_status = PlayFast;//修改状态
        eg_status->fast_status = FBPlaySta;
    }
    //}

    //返回成功
    mengine_reply_msg(msg_ptr, TRUE);

    return RESULT_IGNORE;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e mengine_cancel_ffb(void* msg_ptr)
 * \取消快进退
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
app_result_e mengine_cancel_ffb(void* msg_ptr)
{
    mengine_status_t *eg_status = &mengine_info.eg_status;
    //当前是快进退状态
    if ((eg_status->play_status == PlayFast) && (eg_status->fast_status != FBPlay_Null))
    {
        mmm_mp_cmd(mp_handle, MMM_MP_SET_FFB, (unsigned int) 0);//取消快进退
        eg_status->play_status = PlaySta;//修改状态
        eg_status->fast_status = FBPlay_Null;
    }

    //返回成功
    mengine_reply_msg(msg_ptr, TRUE);

    return RESULT_IGNORE;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e mengine_set_filepath(void* msg_ptr)
 * \设置播放文件路径信息
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
app_result_e mengine_filepath_deal(void* msg_ptr)
{
    uint8 location_size;
    //void *data_ptr = msg_ptr->msg.content.addr;//消息数据指针
    //消息指针
    private_msg_t* data_ptr = (private_msg_t*) msg_ptr;

    file_path_info_t *locat_info = (file_path_info_t*) data_ptr->msg.content.addr;//路径信息指针

    fsel_type_e file_source = locat_info->file_source;

    if ((file_source == FSEL_TYPE_PLAYLIST) || (file_source == FSEL_TYPE_LISTAUDIBLE) || (file_source
            == FSEL_TYPE_M3ULIST))
    {
        //是播放列表模式
        location_size = sizeof(plist_location_t);
    }
    else if ((file_source == FSEL_TYPE_COMMONDIR) || (file_source == FSEL_TYPE_DISKSEQUNCE))
    {
        //是目录模式
        location_size = sizeof(file_location_t);
    }
    else
    {
        //是收藏夹模式
        location_size = sizeof(flist_location_t);
    }
    libc_memcpy(&(mengine_info.eg_config.location), &(locat_info->file_path), (int) location_size);

    //mengine_info.eg_config.fsel_type = cur_type;
    /*_set_file();//设置播放文件*/
    return RESULT_IGNORE;

}

/******************************************************************************/
/*!
 * \par  Description:
 * \uinapp_result_e mengine_set_filepath(void* msg_ptr)
 * \设置播放文件路径信息
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
app_result_e mengine_set_filepath(void* msg_ptr)
{
    bool ret_vals;
    uint8 loop_cnt;
    bool need_fsel_init = FALSE;

    //盘符
    uint8 disk_new, disk_old;
    //消息指针
    private_msg_t* data_ptr = (private_msg_t*) msg_ptr;

    file_path_info_t *locat_info = (file_path_info_t*) data_ptr->msg.content.addr;//路径信息指针

    fsel_type_e cur_type = locat_info->file_source;

    if (locat_info->file_source == FSEL_TYPE_SDFILE)
    {
        ret_vals = _stop(STOP_NORMAL);//停止播放
        mengine_info.eg_config.fsel_type = locat_info->file_source;//更改引擎保存的文件选择类型
        libc_memcpy(g_file_name, locat_info->file_path.plist_location.filename, 12);
    }
    else
    {
        //注意_stop中有引用g_is_audible,因此要先停止歌曲播放
        ret_vals = _stop(STOP_NORMAL);//停止播放
        if (ret_vals == FALSE)
        {
            goto msg_end;
        }

        //判断是否从audible切换到music或从music切换到audible
        for (loop_cnt = 0; loop_cnt < 2; loop_cnt++)
        {
            if (libc_strncmp(&(locat_info->file_path.dirlocation.filename), audible_ext[loop_cnt], 4) == 0)
            {
                //是否从music切换到audible
                if (g_is_audible == FALSE)
                {
                    g_is_audible = TRUE;
                    need_fsel_init = TRUE;
                }
                break;
            }
        }

        if (loop_cnt == 2)
        {
            //是否从audible切换到music
            if (g_is_audible == TRUE)
            {
                mengine_save_audible_breakinfo();
                g_is_audible = FALSE;
                need_fsel_init = TRUE;
            }
        }

        //文件选择类型修改，需要重新初始化文件选择器(盘符修改也需要重新初始化)
        disk_new = locat_info->file_path.plist_location.disk;
        disk_old = mengine_info.eg_config.location.plist_location.disk;

        mengine_filepath_deal(msg_ptr);

        if ((mengine_info.eg_config.fsel_type != cur_type) || (disk_new != disk_old))
        {
            need_fsel_init = TRUE;
        }

        if (need_fsel_init == TRUE)
        {
            //退出文件选择器
            fsel_exit();
            mengine_info.eg_config.fsel_type = cur_type;//更改引擎保存的文件选择类型
            ret_vals = mengine_file_init();//重新初始化
            if (ret_vals == FALSE)
            {
                goto msg_end;
            }
        }
        //else
        {
            ret_vals = fsel_set_location(&mengine_info.eg_config.location.plist_location, cur_type);
            if (ret_vals == FALSE)
            {
                fsel_exit();
                ret_vals = mengine_file_init();//重新初始化
                if (ret_vals == FALSE)
                {
                    goto msg_end;
                }
            }
        }
        if ((mengine_info.eg_config.shuffle_flag != 0) && (file_sys_id != -1))
        {
            ret_vals = fsel_get_location(&(mengine_info.eg_config.location.plist_location),
                    mengine_info.eg_config.fsel_type);
        }
        //for audible file
        if (g_is_audible == TRUE)
        {
            _deal_audible_posfile(0);
        }
    }
    msg_end: mengine_reply_msg(msg_ptr, ret_vals);

    return RESULT_IGNORE;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e mengine_change_filepath(void* msg_ptr)
 * \改变播放文件路径
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
app_result_e mengine_change_filepath(void* msg_ptr)
{

    //消息指针
    private_msg_t* data_ptr = (private_msg_t*) msg_ptr;

    file_path_info_t *locat_info = (file_path_info_t*) data_ptr->msg.content.addr;//路径信息指针

    g_path_type = mengine_info.eg_config.fsel_type;

    mengine_filepath_deal(msg_ptr);

    mengine_info.eg_config.fsel_type = locat_info->file_source;
    //重新开始播放, 设置文件
    /*if (_set_file() == TRUE)
     {
     _play(PLAY_RESUME);//播放
     }*/
    g_change_path_flag = 1;
    //返回成功
    mengine_reply_msg(msg_ptr, TRUE);
    return RESULT_IGNORE;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \void _check_audible_chapter(void)
 * \设置章节播放
 * \param[in]    void  para1
 * \param[out]   none
 * \return       bool the result
 * \retval       1 success
 * \retval       0 failed
 * \ingroup      mengine_control.c
 * \note
 */
/*******************************************************************************/
void _check_audible_chapter(void)
{
    //检测
    uint8 i = 0;

    uint16 old_num = (mengine_info.eg_config.file_cnt.num - 1);

    uint16 old_total = mengine_info.eg_config.file_cnt.total;

    //当前播放时间
    uint32 cur_time = mengine_info.eg_playinfo.cur_time;

    //audible章节时间
    uint32 cha_time_cur, cha_time_next;

    if (g_audible_info_p->m_cChapters_ > 200)
    {
        g_audible_info_p->m_cChapters_ = 200;
    }

    if (g_audible_info_p->m_cChapters_ == 1)
    {
        //小于两个章节，不需要使用定时器检测
        stop_app_timer(g_chapter_timer);
    }

    for (i = 0; i < 200; i++)
    {
        if (i >= g_audible_info_p->m_cChapters_)
        {
            break;
        }
        else
        {
            cha_time_cur = g_audible_info_p->SongChapters_[i];
            if (i == (g_audible_info_p->m_cChapters_ - 1))
            {
                cha_time_next = 0xffffffff;
            }
            else
            {
                cha_time_next = g_audible_info_p->SongChapters_[i + 1];
            }
        }

        if ((cha_time_next > cur_time) && (cha_time_cur <= cur_time))
        {
            //文件总数或文件序号有变更，说明切换章节
            if ((old_total != g_audible_info_p->m_cChapters_) || (old_num != i))
            {
                //章节序号
                mengine_info.eg_config.file_cnt.num = (i + 1);

                //章节总数
                mengine_info.eg_config.file_cnt.total = (uint16) g_audible_info_p->m_cChapters_;

                mengine_info.eg_playinfo.cur_file_switch |= 0x02;
            }
        }
    }
}

/******************************************************************************/
/*!
 * \par  Description:
 * \void _check_audible_image(void)
 * \设置章节播放
 * \param[in]    void  para1
 * \param[out]   none
 * \return       bool the result
 * \retval       1 success
 * \retval       0 failed
 * \ingroup      mengine_control.c
 * \note
 */
/*******************************************************************************/
void _check_audible_image(void)
{
    mengine_playinfo_t *eg_play_info = &mengine_info.eg_playinfo;
    mengine_status_t *eg_status = &mengine_info.eg_status;
    //图片时间
    uint32 image_time_cur, image_time_next;

    //当前时间
    uint32 cur_time = eg_play_info->cur_time;
    //检测
    uint16 i = 0;

    if (eg_status->play_status == PlayFast)
    {
        //快进退状态不检测专辑图片，加快快进退速度
        return;
    }

    if (g_audible_info_p->m_cImages_ > 200)
    {
        g_audible_info_p->m_cImages_ = 200;
    }

    for (i = 0; i < 200; i++)
    {
        if (i >= g_audible_info_p->m_cImages_)
        {
            break;
        }
        else
        {
            image_time_cur = g_audible_info_p->imageTime_[i];
            if (i == (g_audible_info_p->m_cImages_ - 1))
            {
                image_time_next = 0xffffffff;
            }
            else
            {
                image_time_next = g_audible_info_p->imageTime_[i + 1];
            }
        }

        if ((cur_time >= image_time_cur) && (cur_time < image_time_next))
        {
            if (g_image_num != i)
            {
                g_image_num = i;
                mengine_info.eg_playinfo.cur_imag_flag = 1;
                mengine_info.eg_playinfo.cur_imag_pos = g_audible_info_p->imagePos_[i];
                break;
            }
        }
    }
}
/******************************************************************************/
/*!
 * \par  Description:
 * \断点续播处理
 * \param[in]    void  para1
 * \param[out]   none
 * \return       bool the result
 * \retval       1 success
 * \retval       0 failed
 * \ingroup      mengine_event.c
 * \note
 */
/*******************************************************************************/
app_result_e mengine_standby_play(void* msg_ptr)
{
    bool ret_vals = TRUE;
    
    if (mengine_enter_mode == ENTER_RESUME)
    {
        //开启音频输入    
        com_set_sound_out(TRUE, SOUND_OUT_START, NULL);
        
        //恢复音乐播放,播放
        ret_vals = _play(PLAY_RESUME);
    }
    else if(mengine_enter_mode == ENTER_RESUME_PAUSE)
    {
        //开启音频输入
        com_set_sound_out(TRUE, SOUND_OUT_START, NULL);
        
        //关闭音频输出
        com_set_sound_out(FALSE, SOUND_OUT_PAUSE, NULL);
    }
    else
    {
        ;//nothing for QAC
    }

    //返回成功
    mengine_reply_msg(msg_ptr, ret_vals); 

    return RESULT_IGNORE;

}

