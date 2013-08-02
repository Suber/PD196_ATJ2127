/*******************************************************************************
 *                              US212A
 *                            Module: radio ui
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *     mikeyang   2011-09-23          1.0              create this file
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
extern app_result_e deal_station_del(radiolist_control_t *p_ml_control)
__FAR__;

/* 字串处理最大长度定义宏*/
#define  MAX_STRING   50

/* 预设列表listbox  私有结构*/
//static listbox_private_t savelist_private _BANK_DATA_ATTR_;

/* 字串处理buffer */
uint8 string_buf[MAX_STRING] _BANK_DATA_ATTR_;

/*!
 * \brief
 *  savelist_key_map_list：预设电台列表按键映射表
 */
const key_map_t savelist_key_map_list[] =
{
    /*! PREV 按键转换为 SELECT_PREV 事件 */
    {{ KEY_PREV, 0, KEY_TYPE_DOWN | KEY_TYPE_LONG | KEY_TYPE_HOLD }, EVENT_RADIO_SELECT_PREV },
    /*! NEXT 按键转换为 SELECT_NEXT 事件 */
    {{ KEY_NEXT, 0, KEY_TYPE_DOWN | KEY_TYPE_LONG | KEY_TYPE_HOLD }, EVENT_RADIO_SELECT_NEXT },
    /*! 短按KEY_PLAY 按键转换为 SELECT_CUR 事件 */
    {{ KEY_PLAY, 0, KEY_TYPE_SHORT_UP }, EVENT_RADIO_SELECT_CUR },
    /*! 短按KEY_VOL 按键转换为 RETURN 事件 */
    {{ KEY_VOL, 0, KEY_TYPE_SHORT_UP }, EVENT_RADIO_RETURN },
    /*! 短按KEY_MODE 按键转换为 OPTION 事件 */
    {{ KEY_MODE, 0, KEY_TYPE_SHORT_UP }, EVENT_RADIO_OPTION },
    /*! 结束标志 */
    {{ KEY_NULL, 0, KEY_TYPE_NULL }, MSG_NULL },
};

/************************************************************************************************/
/*!
 * \par  Description:
 * \app_result_e  string_joint_save(uint8 station_num, uint8 mode)
 * \param[in]    station_num  电台号  mode   询问还是成功后提示
 * \param[out]   none
 * \return      none
 * \retval
 * \retval
 * \note   保存电台时字符串拼接处理
 */
/*************************************************************************************************/
app_result_e string_joint_save(uint8 station_num, uint8 mode)
{
    string_desc_t string_desc;
    string_desc_t string_source[2];
    uint8 tmp_buf[7];
    //uint8 tmp;
    dialog_com_data_t dialog_com;
    app_result_e ret;
    uint16 style_id;

    libc_memset(tmp_buf, 0, sizeof(tmp_buf));
    libc_memset(string_buf, 0, sizeof(string_buf));

    //电台号转成ascill
    libc_itoa(station_num, &tmp_buf[0], 2);

#if 0    //itoa 已经做了转换
    if(station_num <10)
    {
        //只有一位时，转换后需交换顺序
        tmp = tmp_buf[0];
        tmp_buf[0] = tmp_buf[1];
        tmp_buf[1] = tmp;
    }
#endif

    //源字串--- 电台号填充
    string_source[1].data.str = tmp_buf;
    string_source[1].length = 3;
    string_source[1].language = ANSIDATAAUTO;

    //当前需保存频点--- 即当前播放频率转为ascill
    libc_memset(Freqtab, 0, sizeof(Freqtab));
    freq_change(Freqtab, g_radio_config.FMStatus.freq, 1);
    //源字串--- 频率值填充
    string_source[0].data.str = Freqtab;
    string_source[0].length = 7;
    string_source[0].language = ANSIDATAAUTO;

    //目标字串
    string_desc.data.str = string_buf;
    string_desc.length = MAX_STRING;
    if (mode == 0)
    {
        //是否保存
        string_desc.argv = CON_SAVE;
    }
    else
    {
        //成功保存
        string_desc.argv = SUC_SAVE;
    }
    string_desc.language = (int8) g_comval.language_id;

    com_string_format(&string_desc, &string_source[0]);

    //询问，含有按键的dialog
    if (mode == 0)
    {
        dialog_com.dialog_type = DIALOG_BUTTON_INCLUDE;
        dialog_com.button_type = BUTTON_YESNO;
        dialog_com.active_default = BUTTON_YESNO_YES;
        style_id = DIALOG_ASK;
    }
    //成功后提示，不含按键
    else
    {
        dialog_com.dialog_type = DIALOG_INFOR_WAIT;
        dialog_com.button_type = BUTTON_NO_BUTTON;
        style_id = DIALOG_MSG;
    }
    dialog_com.icon_id = V_U16_INVALID;
    dialog_com.string_desc = &string_desc;
    ret = gui_dialog(style_id, &dialog_com);
    return ret;
}

/************************************************************************************************/
/*!
 * \par  Description:
 * \app_result_e do_save_process(uint8 num)
 * \param[in]    num  电台号
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \note   电台保存处理，将当前播放频点保存到菜单波段指定列表
 */
/*************************************************************************************************/
app_result_e do_save_process(uint8 num)
{
    //保存后回到播放场景
    app_result_e result = RESULT_RADIO_START_PLAY;

    if ((num > MAX_STATION_COUNT) || (num == 0))
    {
        //电台号超范围
        return result;
    }

    if (g_menu_band == Band_Japan)
    {
        g_radio_config.fmstation_jp[num - 1] = g_radio_config.FMStatus.freq;
    }
    else if (g_menu_band == Band_Europe)
    {
        g_radio_config.fmstation_eu[num - 1] = g_radio_config.FMStatus.freq;
    }
    else
    {
        g_radio_config.fmstation_us[num - 1] = g_radio_config.FMStatus.freq;
    }

    //如果是用户模式，切换到预设模式
    g_radio_config.band_mode = g_menu_band;
    //更新列表指针
    init_point_freq();
    //更新电台号
    g_radio_config.FMStatus.station = num;
    //更新保存后的电台总数
    g_radio_config.FMStatus.station_count = get_station_count();

    //保存成功提示
    string_joint_save(num, 1);
    return result;
}

/************************************************************************************************/
/*!
 * \par  Description:
 * \app_result_e deal_station_save(radiolist_control_t *p_ml_control)
 * \param[in]    p_ml_control
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \note   保存到预设处理
 */
/*************************************************************************************************/
app_result_e deal_station_save(radiolist_control_t *p_ml_control)
{
    uint8 station_num;
    app_result_e ret = RESULT_NULL;

    //保存位置，电台号
    station_num = p_ml_control->list_no + 1;

    ret = string_joint_save(station_num, 0);

    if (ret == RESULT_DIALOG_YES)
    {
        //确认保存
        ret = do_save_process(station_num);
    }
#if 0	
    else
    {
        //取消保存，重绘列表
        ret = RESULT_REDRAW;
    }
#endif	
    return ret;
}

/************************************************************************************************/
/*!
 * \par  Description:
 * \app_result_e gui_event_process(msg_apps_type_e event, radio_list_entry_e entry, uint8 index)
 * \param[in]    entry   电台列表入口模式，影响按play 后响应
 * \param[in]    index   从哪个清除预设的菜单进入，影响按return 后响应
 * \param[in]    event   需处理的事件
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \note   avoid qac warning
 */
/*************************************************************************************************/
app_result_e gui_event_process(msg_apps_type_e event, radio_list_entry_e entry, uint8 index)
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
        //正常显示进入，选择电台播放
        if (entry == ENTER_NORMAL)
        {
            //选择了预设电台播放，更新播放模式
            g_radio_config.band_mode = g_menu_band;
            select_current_play(&list_control);
            result = RESULT_RADIO_START_PLAY;
        }
        //保存到预设进入，保存操作
        else if (entry == ENTER_ADD_STATION)
        {
            result = deal_station_save(&list_control);
            if (result == RESULT_REDRAW)
            {
                list_control.draw_mode = LIST_DRAW_ALL;
            }
        }
        //清除预设进入，清除操作
        else //if (entry_mode == ENTER_DEL_STATION)
        {
            result = deal_station_del(&list_control);
            if (result == RESULT_REDRAW)
            {
                list_control.draw_mode = LIST_DRAW_ALL;
            }
        }
        //else
        //{
        //    ;
        //}
        break;

        case EVENT_RADIO_RETURN:
        if (entry == ENTER_NORMAL)
        {
            //返回radio 主菜单
            result = RESULT_RADIO_MAINMENU;
        }
        else if (entry == ENTER_ADD_STATION)
        {
            //返回播放场景的option
            result = RESULT_RADIO_OPTION_PLAYWIN;
        }
        else //if (entry_mode == ENTER_DEL_STATION)
        {
            //从预设列表的option 中选择清除进来
            if (index == 1)
            {
                result = RESULT_RADIO_OPTION_STATION;
            }
            //从播放场景的option 中选择清除进来
            else if (index == 2)
            {
                result = RESULT_RADIO_OPTION_PLAYWIN;
            }
            //其他返回radio 主菜单
            else
            {
                result = RESULT_RADIO_MAINMENU;
            }
        }
        //else
        //{
        //    ;
        //}
        break;

        case EVENT_RADIO_OPTION:
        if (entry == ENTER_NORMAL)
        {
            //进入电台列表option 场景
            result = RESULT_RADIO_OPTION_STATION;
        }
        //删除预设或保存预设进来，不响应option 键
        break;

        default:
        result = com_message_box(event);
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
 * \app_result_e show_saved_station(radio_list_entry_e  entry_mode, uint8 from)
 * \param[in]    entry_mode   电台列表入口模式，影响按play 后响应
 * \param[in]    from    从哪个清除预设的菜单进入，影响按return 后响应
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \note   预设电台列表的刷新显示
 */
/*************************************************************************************************/
app_result_e show_saved_station(radio_list_entry_e entry_mode, uint8 from)
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

    //初始化所浏览的列表指针
    if (g_menu_band == Band_China_US)
    {
        pfreq_list = &g_radio_config.fmstation_us[0];
    }
    else if (g_menu_band == Band_Japan)
    {
        pfreq_list = &g_radio_config.fmstation_jp[0];
    }
    else
    {
        pfreq_list = &g_radio_config.fmstation_eu[0];
    }

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
            if (com_key_mapping(&input_msg, &gui_event, savelist_key_map_list) == TRUE)
            {
                result = gui_event_process(gui_event, entry_mode, from);
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

