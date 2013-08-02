/*******************************************************************************
 *                              US212A
 *                            Module: Picture
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       zhangxs     2011-09-19 10:00     1.0             build this file
 *******************************************************************************/
/*!
 * \file     pic_play.c
 * \brief    Picture的play函数模块
 * \author   zhangxs
 * \version  1.0
 * \date  2011/9/20
 *******************************************************************************/
#include  "picture.h"

/*! \brief 物理按键与按键事件的对应关系*/
const key_map_t pic_play_key_map_list[] =
{
    /*! next事件 */
    {{ KEY_NEXT, 0, KEY_TYPE_LONG | KEY_TYPE_DOWN | KEY_TYPE_HOLD },  EVENT_PICTURE_PLAYING_NEXT },
    /*! prev事件 */
    {{ KEY_PREV, 0, KEY_TYPE_LONG | KEY_TYPE_DOWN | KEY_TYPE_HOLD },  EVENT_PICTURE_PLAYING_PREV },
    /*! 短按play事件 */
    {{ KEY_PLAY, 0, KEY_TYPE_SHORT_UP },  EVENT_PICTURE_PLAYING_PAUSE},
    /*! 短按option事件 */
    {{ KEY_MODE, 0, KEY_TYPE_SHORT_UP }, EVENT_PICTURE_OPTION },
    /*! 短按vol返回事件 */
    {{ KEY_VOL, 0, KEY_TYPE_SHORT_UP }, EVENT_PICTURE_RETURN },
    /*! 结束标志 */
    {{KEY_NULL, 0, KEY_TYPE_NULL}, MSG_NULL},
};
/******************************************************************************/
/*!
 * \par  Description:
 *    自动播放定时器回调函数
 * \param[in]    void
 * \param[out]   void
 * \return       void
 * \ingroup      pic_play
 * \note
 * \li  只设置标志位，在主循环切换文件
 *******************************************************************************/
void auto_play_flag(void)
{
    g_picture_change_file = up_picture_playnext;
    //先停止timer定时器，待文件解码结束后再开启
    stop_app_timer(timer_pic_play);
}

#if 0
static void pic_show_headbar(void)
{
    style_infor_t picture_style;
    numbox_private_t picture_numbox_param;

    picture_style.type = UI_AP;
    picture_style.style_id = STY_PIC_NUM_NUMBERBOX;

    picture_numbox_param.total = g_picture_var.path.file_path.dirlocation.file_total;
    picture_numbox_param.value = g_picture_var.path.file_path.dirlocation.file_num;
    //    gui_set_headbar_mode(HEADBAR_MODE_NORMAL, this_headbar_icon_id);
    ui_show_numbox(&picture_style, &picture_numbox_param, NUMBOX_DRAW_ALL);
}
#endif
/******************************************************************************/
/*!
 * \par  Description:
 *    图片文件切换模块
 * \param[in]    void
 * \param[out]   void
 * \return       bool  切换是否成功
 * \retval       TRUE  成功  FALSE 失败
 * \ingroup      pic_play
 * \note
 * \li   由于enhance与basal等复用，因此在文件切换之前，需要查询解码库是否存在，
 *       若存在则应关闭解码库后再调用enhance接口
 *******************************************************************************/
bool pic_change_file(void)
{
    bool result = FALSE;
    uint8 *file_name = g_picture_var.path.file_path.dirlocation.filename;

    pic_decoder_free();

    if ((g_picture_change_file & up_picture_playprev) == up_picture_playprev)
    {
        result = fsel_get_prevfile(file_name);
    }
    else
    {
        result = fsel_get_nextfile(file_name);
    }

    if (result == TRUE)
    {
        fsel_get_location((void *) &g_picture_var.path.file_path, g_picture_var.path.file_source);
    }
    g_picture_change_file = (uint8)(g_picture_change_file & clr_picture_changefile);
    return result;
}

/******************************************************************************/
/*!
 * \par  Description:
 *	  图片播放界面响应GUI事件函数
 * \param[in]   cur_event：GUI事件
 * \param[in]   auto_play_flag:当前是否为自动播放状态
 * \param[out]  none
 * \return      the result 按键事件返回值
 * \ingroup     pic_play
 * \note
 *******************************************************************************/
app_result_e pic_play_proc_key_event(msg_apps_type_e gui_event)
{
    app_result_e ui_result = RESULT_NULL;

    switch (gui_event)
    {
        case EVENT_PICTURE_PLAYING_NEXT:
        g_picture_change_file = up_picture_playnext;
        if (g_picture_auto_play == TRUE)
        {
            //先停止timer定时器，待文件解码结束后决定是否开启
            stop_app_timer(timer_pic_play);
        }
        break;

        case EVENT_PICTURE_PLAYING_PREV:
        g_picture_change_file = up_picture_playprev;
        if (g_picture_auto_play == TRUE)
        {
            //先停止timer定时器，待文件解码结束后决定是否开启
            stop_app_timer(timer_pic_play);
        }
        break;

        case EVENT_PICTURE_RETURN:
        ui_result = RESULT_PIC_RETURN;
        break;

        case EVENT_PICTURE_OPTION:
        if (g_picture_auto_play == FALSE)
        {
            ui_result = RESULT_PIC_OPTIONMENU;
        }
        else
        {
            /*自动播放不响应mode按键*/
            ui_result = RESULT_NULL;
        }
        break;

        case EVENT_PICTURE_PLAYING_PAUSE:
        g_picture_auto_play ^= 0x01;
        if (g_picture_auto_play == TRUE)
        {
            //开启自动切换图片的定时器
            restart_app_timer(timer_pic_play);
            change_app_state(APP_STATE_PLAYING_ALWAYS);
        }
        else
        {
            stop_app_timer(timer_pic_play);
            change_app_state(APP_STATE_NO_PLAY);
        }
        break;

        default:
        //如果当前线程未关闭，相应热键消息前先关闭线程，随后根据
        //返回值确定是否需要重新刷图
        if(g_mmm_id_free == FALSE)
        {
            pic_decoder_free(); 
            ui_result = com_message_box(gui_event);
            if(ui_result == RESULT_NULL)
            {
                //重新进行图片解码
                ui_result = RESULT_REDRAW;
            }           
        }
        else
        {
            ui_result = com_message_box(gui_event);    
        }        
        break;
    }
    return ui_result;
}
/******************************************************************************/
/*!
 * \par  Description:
 *    检测解码状态
 * \param[in]    dec_status 解码状态
 * \param[out]   none
 * \return       the result
 * \retval       0 sucess   需要退出解码
 * \retval       1 failed   继续解码
 * \ingroup      pic_play
 * \note
 * \li  实现图片手动/自动播放，文件切换等功能
 *******************************************************************************/
static bool _deal_decode_status(decode_status_e dec_status)
{
    bool ret_val = TRUE;

    switch (dec_status)
    {
        case DECODE_NO_ERR:
        g_file_err = 0;
        break;

        case DECODE_FILE_ERR:
        //非自动播放则直接返回列表界面
        //自动播放错误文件超出一定限制也会返回列表界面
        if ((g_picture_auto_play == FALSE) || (g_file_err >= g_file_total))
        {
            gui_dialog_msg(DIALOG_MSG, DIALOG_INFOR_WAIT, S_FILE_NO_SUPPORT);
            ret_val = FALSE;
        }
        else
        {
            //切换歌曲
            g_file_err++;
        }
        break;

        case DECODE_UNSUPPORT_ERR:
        //此场景不支持该种播放模式
        //如果为非自动播放模式，则直接返回
        if (g_picture_auto_play == FALSE)
        {
            ret_val = FALSE;
        }
        else
        {
            //切下一曲
            g_picture_change_file = up_picture_playnext;
        }
        break;

        default:
        ret_val = FALSE;
        break;
    }
    return ret_val;
}
/******************************************************************************/
/*!
 * \par  Description:
 *	  picture播放场景函数
 * \param[in]    none
 * \param[out]   none
 * \return       the result
 * \retval       0 sucess
 * \retval       1 failed
 * \ingroup      module name
 * \par          exmaple code
 * \note
 * \li  实现图片手动/自动播放，文件切换等功能
 *******************************************************************************/
app_result_e pic_play(void)
{
    bool result;
    decode_status_e dec_status;
    input_gui_msg_t ui_msg;
    private_msg_t private_msg;
    msg_apps_type_e gui_event;

    bool need_play = TRUE;
    bool is_scene_exit = FALSE;
    app_result_e ui_result = RESULT_NULL;

    //解码初始化
    if (pic_decode_init(0) == FALSE)
    {
        ui_result = RESULT_PIC_RETURN;
        is_scene_exit = TRUE;
    }

    while (!is_scene_exit)
    {
        if ((g_picture_change_file & up_picture_changefile) != 0)
        {
            if (pic_change_file() == TRUE)
            {
                //注意在切换文件之前不要调用显示驱动函数
                ui_clear_screen(NULL);
                //gui_set_headbar_mode(HEADBAR_MODE_NULL, this_headbar_icon_id);
                need_play = TRUE;
            }
            else
            {
                //切换歌曲失败
                ui_result = RESULT_PIC_RETURN;
                is_scene_exit = TRUE;
                continue;
            }
        }

        if (need_play == TRUE)
        {
            need_play = FALSE;

            g_file_total = g_picture_var.path.file_path.dirlocation.file_total;

            dec_status = pic_decode();

            if (_deal_decode_status(dec_status) == FALSE)
            {
                if ((g_picture_var.path.file_path.dirlocation.disk == DISK_H) && (sys_detect_disk(DRV_GROUP_STG_CARD)
                        == -1))
                {
                    //检测不到卡盘
                    gui_dialog_msg(DIALOG_MSG, DIALOG_INFOR_WAIT, S_CARD_OUT);
                    ui_result = RESULT_CARD_OUT_ERROR;
                }
                else
                {
                    ui_result = RESULT_PIC_RETURN;
                }
                is_scene_exit = TRUE;
                continue;
            }
        }

        //解码库还未释放但线程解码已结束
        if ((g_mmm_id_free == FALSE) && (pic_check_decode() == TRUE))
        {
            //释放该解码中间件资源
            pic_decoder_free();

            if (g_picture_auto_play == FALSE)
            {
                //图片为多帧且没有背光
                if (((g_picture_userinfor.returnflag & RETURNFLAG_MULTIFRAME) != 0)
                        && (get_screensave_state() == FALSE))
                {
                    //重复显示该多帧图片
                    need_play = TRUE;
                }
                else
                {
                    //非自动播放状态显示headbar
                    //pic_show_headbar();
                }
            }
            else
            {
                restart_app_timer(timer_pic_play);
            }
        }

        result = get_gui_msg(&ui_msg);
        if (result == TRUE)//有ui消息
        {
            //gui_set_headbar_mode(HEADBAR_MODE_NULL, this_headbar_icon_id);
            if (com_key_mapping(&ui_msg, &gui_event, pic_play_key_map_list) == TRUE)
            {
                ui_result = pic_play_proc_key_event(gui_event);
            }
        }
        else
        {
            //处理ap私有消息
            result = get_app_msg(&private_msg);
            if (result == TRUE)
            {
                //gui_set_headbar_mode(HEADBAR_MODE_NULL, this_headbar_icon_id);
                ui_result = pic_msg_callback(&private_msg);                
            }

        }

        switch (ui_result)
        {
            case RESULT_NULL:
            break;

            case RESULT_REDRAW:
            //重绘当前图片
            need_play = TRUE;
            ui_result = RESULT_NULL;
            ui_clear_screen(NULL);
            break;

            default:
            if (g_picture_auto_play == TRUE)
            {
                change_app_state(APP_STATE_NO_PLAY);
            }
            is_scene_exit = TRUE;
            break;

        }

        //挂起20ms，多任务调度
        sys_os_time_dly(2);
    }

    pic_decoder_free();

    kill_app_timer(timer_pic_play);
    change_app_state(APP_STATE_NO_PLAY);

    if (ui_result == RESULT_PIC_RETURN)
    {
        if (g_picture_enter_mode == PARAM_FROM_MAINMENU)
        {
            ui_result = RESULT_PIC_FILELIST;
        }
        else
        {
            ui_result = RESULT_RETURN_BROWSER;
        }
    }

    if ((g_need_resume_engine == TRUE) && (ui_result != RESULT_APP_QUIT))
    {
        g_need_resume_engine = FALSE;
        music_resume_engine();
    }

    return ui_result;
}
