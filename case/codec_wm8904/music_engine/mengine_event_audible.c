/*******************************************************************************
 *                              US212A
 *                            Module: music engine
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *      fiona.yang  2012-05-30        1.0              create this file
 *******************************************************************************/

#include "app_mengine.h"

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e mengine_audible_getimage(void* msg_ptr)
 * \获取当前audbile文件的图片信息
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
 * \ingroup      mengine_event_audible.c
 * \note
 */
/*******************************************************************************/
app_result_e mengine_audible_getimage(void* msg_ptr)
{
    //消息指针
    private_msg_t* data_ptr = (private_msg_t*) msg_ptr;
    //当前播放时间
    uint32 cur_time = mengine_info.eg_playinfo.cur_time;
    //图片时间
    uint32 image_time_cur, image_time_next;
    //检测
    uint16 i = 0;

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
            g_image_num = i;
            mengine_info.eg_playinfo.cur_imag_flag = 1;
            mengine_info.eg_playinfo.cur_imag_pos = g_audible_info_p->imagePos_[i];
            break;
        }
    }

    //copy当前播放时间和比特率到消息指针指向的数据区
    libc_memcpy(data_ptr->reply->content, &mengine_info.eg_playinfo, sizeof(mengine_playinfo_t));

    //返回成功
    mengine_reply_msg(msg_ptr, TRUE);

    return RESULT_IGNORE;
}
/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e mengine_audible_get_section_info(void* msg_ptr)
 * \获取当前播放的audible文件章节信息
 * \param[in]    msg_ptr  输入消息指针
 * \param[out]   none
 * \return       app_result_e the result
 * \retval       none
 * \ingroup      mengine_audbile_deal.c
 * \note
 */
/*******************************************************************************/
app_result_e mengine_audible_get_section_info(void* msg_ptr)
{
    //消息指针
    private_msg_t* data_ptr = (private_msg_t*) msg_ptr;

    //章节切换标志
    mengine_playinfo_t *play_info = &mengine_info.eg_playinfo;

    libc_memcpy(data_ptr->reply->content, &mengine_info.eg_config.file_cnt, sizeof(musfile_cnt_t));

    //清除章节切换标志
    play_info->cur_file_switch &= (uint8)(0xfd);

    //返回成功
    mengine_reply_msg(msg_ptr, TRUE);

    return RESULT_IGNORE;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \void _check_time_switch(uint32 time_sel)
 * \根据时间跳转播放
 * \param[in]    time_sel  para1
 * \param[in]    time_sel  目标时间
 * \param[out]   none
 * \return       bool the result
 * \retval       1 success
 * \retval       0 failed
 * \ingroup      mengine_mesasge_done.c
 * \note
 */
/*******************************************************************************/
void _check_time_switch(uint32 time_sel)
{
    //停止 or 暂停时 发送播放命令
    if ((mengine_info.eg_status.play_status == PauseSta) || (mengine_info.eg_status.play_status == StopSta))
    {
        uint32 checksum = 0;

        uint8 i;
        //计算pos文件内容
        // 毫秒位置
        //*(uint32*)(g_audible_info_p->playbackPosFilebuffer[8]) = time_sel;
        _adjust_big_little_endian((uint8*) &time_sel, 4);
        libc_memcpy(&g_audible_info_p->playbackPosFilebuffer[8], &time_sel, 4);

        // 结束标志
        g_audible_info_p->playbackPosFilebuffer[15] = 0;

        for (i = 8; i < 16; i++)
        {
            checksum += g_audible_info_p->playbackPosFilebuffer[i];
        }
        //校验和
        //*(uint32*)(g_audible_info_p->playbackPosFilebuffer[4]) = checksum;
        _adjust_big_little_endian((uint8*) &checksum, 4);
        libc_memcpy(&g_audible_info_p->playbackPosFilebuffer[4], &checksum, 4);
        //保存pos文件内容
        _deal_audible_posfile(1);

    }
    else
    {
        //seek命令
        mmm_mp_cmd(mp_handle, MMM_MP_SEEK, (unsigned int) time_sel);
    }
}

/******************************************************************************/
/*!
 * \par  Description:
 * \void _chapter_switch_force(bool next_prev)
 * \手动章节切换
 * \param[in]    next_prev  para1
 * \param[in]    next_prev  1--next;0--prev
 * \param[out]   none
 * \return       bool the result
 * \retval       1 success
 * \retval       0 failed
 * \ingroup      mengine_mesasge_done.c
 * \note
 */
/*******************************************************************************/
void _chapter_switch_force(bool next_prev)
{
    //audible章节时间
    uint32 cha_time_cur;

    uint16 file_num = mengine_info.eg_config.file_cnt.num;

    //状态信息
    bool stop_flag = FALSE;

    if (next_prev == 0)
    {
        //播放时向前切换
        if (file_num > 1)
        {
            file_num--;
        }
        else
        {
            stop_flag = TRUE;
        }
    }
    else
    {
        //播放时向后切换
        if (file_num < g_audible_info_p->m_cChapters_)
        {
            file_num++;
        }
        else
        {
            stop_flag = TRUE;
        }
    }
    //手动切换需要停止
    if (stop_flag == TRUE)
    {
        _stop(STOP_PAUSE);
    }
    else
    {
        if (file_num >= 200)
        {
            file_num = 200;
        }
        if (file_num < 1)
        {
            file_num = 1;
        }
        cha_time_cur = g_audible_info_p->SongChapters_[(file_num - 1)];
        _check_time_switch(cha_time_cur);
        //当前播放时间
        mengine_info.eg_playinfo.cur_time = cha_time_cur;
    }
    //判断图片是否需要刷新
    _check_audible_image();

    //章节序号
    mengine_info.eg_config.file_cnt.num = file_num;

    //章节总数
    mengine_info.eg_config.file_cnt.total = (uint16) g_audible_info_p->m_cChapters_;

    mengine_info.eg_playinfo.cur_file_switch |= 0x02;

    //章节号
    //eg_cfg->location.plist_location.file_num = eg_cfg->file_num;

}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e mengine_chapter_next(void* msg_ptr)
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
 * \ingroup      mengine_mesasge_done.c
 * \note
 */
/*******************************************************************************/
app_result_e mengine_chapter_next(void* msg_ptr)
{
    _chapter_switch_force(1);

    //返回成功
    mengine_reply_msg(msg_ptr, TRUE);

    return RESULT_IGNORE;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e mengine_chapter_prev(void* msg_ptr)
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
 * \ingroup      mengine_mesasge_done.c
 * \note
 */
/*******************************************************************************/
app_result_e mengine_chapter_prev(void* msg_ptr)
{
    _chapter_switch_force(0);

    //返回成功
    mengine_reply_msg(msg_ptr, TRUE);

    return RESULT_IGNORE;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e mengine_audible_bookmark(void* msg_ptr)
 * \设置audible书签播放
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
 * \ingroup      mengine_event_audible.c
 * \note
 */
/*******************************************************************************/
app_result_e mengine_audible_bookmark(void* msg_ptr)
{
    //消息指针
    bool bret = TRUE;
    private_msg_t* data_ptr = (private_msg_t*) msg_ptr;

    mengine_config_t *eg_cfg = &mengine_info.eg_config;

    uint32 bk_time;

    //保存断点,必须在_stop之后,因为_stop中会获取断点，放在_stop之前断点会被覆盖
    libc_memcpy(&eg_cfg->bk_infor, data_ptr->msg.content.addr, sizeof(mmm_mp_bp_info_t));

    bk_time = (uint32) eg_cfg->bk_infor.bp_time_offset;

    //if(g_eg_status_p->play_status == PlaySta)
    //{
    //    _stop(STOP_NORMAL);
    //}

    _check_time_switch(bk_time);

    if ((g_eg_status_p->play_status == PauseSta) || (g_eg_status_p->play_status == StopSta))
    {
        //重新开始播放, 设置文件
        //if (_set_file() == TRUE)
        //{
        bret = _play(PLAY_RESUME);//播放
        //
    }

    g_image_num = 0xffff;

    mengine_info.eg_playinfo.cur_file_switch |= 0x01;

    //返回成功
    mengine_reply_msg(msg_ptr, bret);

    return RESULT_IGNORE;
}

/******************************************************************************/
/*!
 * \par  Description:
 *     adjust big or little endian.
 * \param[in]    data_address   Address of Variable.
 * \param[in]    data_length    Length of Variable.
 * \param[out]   none
 * \return       none
 * \ingroup      module name
 * \par          exmaple code
 * \code
 *               list example code in here
 * \endcode
 *******************************************************************************/
void _adjust_big_little_endian(uint8 *data_address, uint8 data_length)
{
    uint8 tmp;
    uint8 *head_p, *end_p;
    head_p = data_address;
    end_p = data_address + data_length - 1;

    for (; head_p < end_p; ++head_p, --end_p)
    {
        tmp = *end_p;
        *end_p = *head_p;
        *head_p = tmp;
    }
}

/******************************************************************************/
/*!
 * \par  Description:
 * \void _set_chapter_timer(void)
 * \判断是否设置audible timer
 * \param[in]    void  para1
 * \param[out]   none
 * \return       bool the result
 * \retval       none
 * \ingroup      mengine_audbile_deal.c
 * \note
 */
/*******************************************************************************/
void _set_chapter_timer(void)
{
    if (mengine_info.eg_config.section_mod == SWITCH_SECTION)
    {
        if (g_chapter_timer == -1)
        {
            g_chapter_timer = set_app_timer(APP_TIMER_ATTRB_UI, 1000, _check_audible_chapter);
        }
    }
}

/******************************************************************************/
/*!
 * \par  Description:
 * \void _set_imag_timer(void)
 * \判断是否设置audible timer
 * \param[in]    void  para1
 * \param[out]   none
 * \return       bool the result
 * \retval       none
 * \ingroup      mengine_audbile_deal.c
 * \note
 */
/*******************************************************************************/
void _set_imag_timer(void)
{

    if (g_image_timer == -1)
    {
        g_image_timer = set_app_timer(APP_TIMER_ATTRB_UI, 1000, _check_audible_image);
    }

}

/******************************************************************************/
/*!
 * \par  Description:
 * \void _clr_chapter_timer(void)
 * \清除audible timer
 * \param[in]    void  para1
 * \param[out]   none
 * \return       bool the result
 * \retval       none
 * \ingroup      mengine_audbile_deal.c
 * \note
 */
/*******************************************************************************/
void _clr_chapter_timer(void)
{
    if (g_chapter_timer != -1)
    {
        kill_app_timer(g_chapter_timer);
        g_chapter_timer = -1;
    }
}

/******************************************************************************/
/*!
 * \par  Description:
 * \void _clr_imag_timer(void)
 * \清除audible timer
 * \param[in]    void  para1
 * \param[out]   none
 * \return       bool the result
 * \retval       none
 * \ingroup      mengine_audbile_deal.c
 * \note
 */
/*******************************************************************************/
void _clr_imag_timer(void)
{
    if (g_image_timer != -1)
    {
        kill_app_timer(g_image_timer);
        g_image_timer = -1;
    }
}

