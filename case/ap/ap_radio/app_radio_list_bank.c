/*******************************************************************************
 *                              US212A
 *                            Module: radio ui
 *                 Copyright(c) 2003-2012 Actions Semiconductor
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *     mikeyang    2011-09-19          1.0              create this file
 *******************************************************************************/

#include "app_radio.h"

extern void init_list_control(radiolist_control_t *p_list_control)
__FAR__;
extern void load_onepage_item_data(radiolist_control_t *list_control)
__FAR__;
extern void update_list_private(radiolist_control_t *p_ml_control, listbox_private_t *list_private)
__FAR__;
extern void radiolist_scroll_handle(void)
__FAR__;
extern void radiolist_select_next(radiolist_control_t *p_ml_control)
__FAR__;
extern void radiolist_select_prev(radiolist_control_t *p_ml_control)
__FAR__;
extern void select_current_play(radiolist_control_t *p_ml_control)
__FAR__;
//extern uint8 get_index_from_num(uint8 num)
//__FAR__;

/* 用户列表listbox  私有结构*/
//static listbox_private_t userlist_private _BANK_DATA_ATTR_;

/*!
 * \brief
 *  userlist_key_map_list：用户电台列表按键映射表
 */
const key_map_t userlist_key_map_list[] =
{
    /*! PREV 按键转换为 SELECT_PREV 事件 */
    {{ KEY_PREV, 0, KEY_TYPE_DOWN | KEY_TYPE_LONG | KEY_TYPE_HOLD }, EVENT_RADIO_SELECT_PREV },
    /*! NEXT 按键转换为 SELECT_NEXT 事件 */
    {{ KEY_NEXT, 0, KEY_TYPE_DOWN | KEY_TYPE_LONG | KEY_TYPE_HOLD }, EVENT_RADIO_SELECT_NEXT },
    /*! 短按KEY_PLAY 按键转换为 SELECT_CUR 事件 */
    {{ KEY_PLAY, 0, KEY_TYPE_SHORT_UP }, EVENT_RADIO_SELECT_CUR },
    /*! 短按KEY_VOL 按键转换为 RETURN 事件 */
    {{ KEY_VOL, 0, KEY_TYPE_SHORT_UP }, EVENT_RADIO_RETURN },
    /*! 结束标志 */
    {{ KEY_NULL, 0, KEY_TYPE_NULL }, MSG_NULL },
};

/************************************************************************************************/
/*!
 * \par  Description:
 * \bool scene_list_init(void)
 * \param[in]    none
 * \param[out]   none
 * \return      success/fail
 * \retval
 * \retval
 * \note   电台列表场景初始化
 */
/*************************************************************************************************/
bool scene_list_init(void)
{
    //列表显示buffer 清零
    libc_memset(g_radiolist_item[0], 0, LIST_NUM_ONE_PAGE_MAX * LIST_STR_LENGTH);
    return TRUE;
}

/************************************************************************************************/
/*!
 * \par  Description:
 * \bool scene_list_exit(void)
 * \param[in]    none
 * \param[out]   none
 * \return      success/fail
 * \retval
 * \retval
 * \note   电台列表场景退出
 */
/*************************************************************************************************/
bool scene_list_exit(void)
{
    return TRUE;
}

/************************************************************************************************/
/*!
 * \par  Description:
 * \app_result_e deal_event(msg_apps_type_e event)
 * \param[in]    需处理事件
 * \param[out]   none
 * \return      app_result_e 结果消息
 * \retval
 * \retval
 * \note   avoid qac warning
 */
/*************************************************************************************************/
app_result_e deal_event(msg_apps_type_e event)
{
    app_result_e result = RESULT_NULL;
    //GUI按键消息处理
    switch (event)
    {
        case EVENT_RADIO_SELECT_NEXT:
        //切换到下一个电台列表项
        radiolist_select_next(&list_control);
        break;

        case EVENT_RADIO_SELECT_PREV:
        //切换到上一个电台列表项
        radiolist_select_prev(&list_control);
        break;

        case EVENT_RADIO_SELECT_CUR:
        //选择了用户电台播放，更新必要信息
        g_radio_config.band_mode = Band_MAX;
        //选择当前电台播放
        select_current_play(&list_control);
        //根据电台号，获取索引
        if (g_radio_config.FMStatus.station != 0)
        {
            g_userlist_index = get_index_from_num(g_radio_config.FMStatus.station);
        }
        else
        {
            g_userlist_index = 0xff;
        }
        g_radio_config.FMStatus.station_count = g_userlist_total;
        result = RESULT_RADIO_START_PLAY;
        break;

        case EVENT_RADIO_RETURN:
        //返回radio 主菜单
        result = RESULT_RADIO_MAINMENU;
        break;

        default:
        result = com_message_box(event);
        if (result == RESULT_ALARMFM) //播放过程中遇fm alarm消息
        {
            g_radio_from_alarm = 1;
            alarmfm_in_playing();
            result = RESULT_REDRAW;
        }
        if (result == RESULT_REDRAW)
        {
            list_control.draw_mode = LIST_DRAW_ALL;
        }
        break;
    }
    return result;
}

/************************************************************************************************/
/*!
 * \par  Description:
 * \app_result_e show_user_station(void)
 * \param[in]
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \note   用户电台列表的刷新显示
 */
/*************************************************************************************************/
app_result_e show_user_station(void)
{
    style_infor_t style_infor;
    ui_result_e ui_result;
    int8 scroll_timer = -1;
    input_gui_msg_t input_msg; //输入型消息，即gui消息
    msg_apps_type_e gui_event; //gui 消息映射事件
    private_msg_t private_msg; //私有消息
    app_result_e result = RESULT_NULL;

    //电台列表的控件类型固定
    style_infor.type = UI_COM;
    style_infor.style_id = MENULIST;

    list_control.draw_mode = LIST_DRAW_ALL;
    //加载 listbox 控件属性参数，获取控件一屏显示行数
    ui_get_listbox_attrb(&style_infor, &list_control.one_page_count, 0);

    //显示headbar
    gui_set_headbar_mode(HEADBAR_MODE_NORMAL, HEADBAR_ICON_RADIO);

    //浏览为用户电台列表
    pfreq_list = &g_user_station.fmstation_user[0];

    //初始化控制结构
    init_list_control(&list_control);

    //列表循环
    while (1)
    {
        if (list_control.draw_mode != LIST_DRAW_NULL)
        {
            //加载一页显示项
            load_onepage_item_data(&list_control);
            //更新listbox 私有结构
            //update_list_private(&list_control, &userlist_private);
            update_list_private(&list_control, &radiolist_private);
            //显示
            ui_result = (ui_result_e) (uint32) ui_show_listbox(&style_infor, &radiolist_private,
                    (uint32) list_control.draw_mode);
            if (ui_result == UI_NEED_SCROLL)
            {
                if (scroll_timer == -1)//未创建，创建
                {
                    scroll_timer = set_app_timer(APP_TIMER_ATTRB_UI, 80, radiolist_scroll_handle);
                }
            }
            else
            {
                if (scroll_timer != -1)//已创建，删除
                {
                    kill_app_timer(scroll_timer);
                    scroll_timer = -1;
                }
            }
            list_control.draw_mode = LIST_DRAW_NULL;
        }

        //获取gui消息
        if (get_gui_msg(&input_msg) == TRUE)//有gui消息
        {
            //进行按键映射
            if (com_key_mapping(&input_msg, &gui_event, userlist_key_map_list) == TRUE)
            {
                result = deal_event(gui_event);
            }
        }
        else
        {
            //到此，gui消息队列已经处理完毕
            //处理ap私有消息和系统消息
            if (get_app_msg(&private_msg) == TRUE)
            {
                result = radio_app_msg_callback(&private_msg);
                if (result == RESULT_REDRAW)//界面显示提示，返回重绘UI
                {
                    list_control.draw_mode = LIST_DRAW_ALL;
                }
                else if (result != RESULT_NULL)
                {
                    goto menu_exit;
                }
                else
                {
                }
            }
        }

        //退出电台列表
        if ((result != RESULT_NULL) && (result != RESULT_REDRAW))
        {
            goto menu_exit;
        }
        //挂起10ms，多任务调度
        sys_os_time_dly(1);
    }

    menu_exit: if (scroll_timer != -1)//已创建，删除
    {
        kill_app_timer(scroll_timer);
    }
    return result;
}

