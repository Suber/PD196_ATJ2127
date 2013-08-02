/********************************************************************************
 *                              USDK
 *                             Module: ebook
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : ebook_bookmark.c
 * History:
 *      <author>    <time>           <version >             <desc>
 *      stevenluo		2011-11-11         v1.0              create this file
 ********************************************************************************/
/* include file of this application, 本应用的头文件 */
#include "ebook.h"

//按键事件转换表
const key_map_t _key_map_list[] =
{
    /*! down事件 */
    { KEY_NEXT, 0, KEY_TYPE_LONG | KEY_TYPE_HOLD, EVENT_TEXTREAD_NEXT_PAGE },
    /*! up事件 */
    { KEY_PREV, 0, KEY_TYPE_LONG | KEY_TYPE_HOLD, EVENT_TEXTREAD_PREV_PAGE },
    /*! next事件 */
    { KEY_NEXT, 0, KEY_TYPE_SHORT_UP, EVENT_TEXTREAD_KEY_DOWN },
    /*! prev事件 */
    { KEY_PREV, 0, KEY_TYPE_SHORT_UP, EVENT_TEXTREAD_KEY_UP },
    
    { KEY_VOL, 0, KEY_TYPE_SHORT_UP, EVENT_TEXTREAD_RETURN },
    
    { KEY_MODE, 0, KEY_TYPE_LONG_UP, EVENT_TEXTREAD_BASE },
    
    { KEY_MODE, 0, KEY_TYPE_SHORT_UP, EVENT_TEXTREAD_OPTION },
    
    { KEY_PLAY, 0, KEY_TYPE_SHORT_UP, EVENT_TEXTREAD_PLAY },
    /*! 结束标志 */
    {{KEY_NULL, 0, KEY_TYPE_NULL}, MSG_NULL},
};

/****************************************************************************/
/*									代码段												       */
/****************************************************************************/

/********************************************************************************
 * Description :书签列表信息(包括页数列表)函数
 *
 * Arguments  :
 *				msg_callback:菜单列表回调函数。
 *				inlet_type:功能函数的入口类型(参照ebook.h中的inlet_type_e)
 *                    (inlet_type)0-阅读界面;1-书签选择菜单，1-书签删除菜单,3-页数选择菜单
 * Returns     :
 *                无
 * Notes       :
 *
 ********************************************************************************/

app_result_e _bmk_list_enter(msg_cb_func msg_callback, inlet_type_e inlet_type)
{
    uint16 bookmark_totalnum;
    dir_control_t mlst_ctl;
    app_result_e retval;
    style_infor_t style_infor;
    uint8 tag_backup;
    style_infor.style_id = FILELIST;
    style_infor.type = UI_COM;
    bookmark_totalnum = _get_bookmark_nums(inlet_type);		//获取书签总页数
    if (bookmark_totalnum == 0)
    {
        _show_single_dialog(S_NOBMK, DIALOG_INFOR_WAIT);
        return RESULT_REDRAW;
    }
    need_draw = TRUE;
    _clear_and_set_screen(0xffff, 0x0);
    gui_set_headbar_mode(HEADBAR_MODE_NORMAL, HEADBAR_ICON_BROWSER);
    _bmklist_init(&mlst_ctl, bookmark_totalnum);
    
    //书签可看做独立的子场景，该场景可能被common的ui_menulist调用，而这些场景的
    //定时器在该场景下不应该起作用，通过change_app_timer_tag,可以暂停这些定时器，待退出书签子场景
    //后再做恢复。需要注意这里设置的定时器TAG不能和applib.h定义的重复
    tag_backup = get_app_timer_tag();
    change_app_timer_tag(APP_TIMER_TAG_EBOOK);   
        
    while (1)
    {
    //    _count_page_num();
        if (need_draw == TRUE)
        {
            _update_list_info(&mlst_ctl, &bmklist_data, inlet_type);
            ui_show_listbox(&style_infor, &bmklist_data, mlst_ctl.draw_mode);
#ifdef PC
		UpdateMainWnd();		//PC上，更新窗口
#endif 		
            need_draw = FALSE;
        }
        retval = _deal_key_msg(&mlst_ctl, inlet_type);
        //注销软定时器
        if (retval != RESULT_NULL)
        {
            enter_mode = ENTER_FROM_READINGMENU;
            break;
        }
        //多任务调度
        sys_os_time_dly(1);
    }
    change_app_timer_tag(tag_backup);       
    return retval;
}
/********************************************************************************
 * Description :处理映射后的按键实现功能
 *
 * Arguments  :
 *				ui_msg:获取到的消息结构体
 *				mlst_ctl:菜单列表控制结构体(参照common_ui.h中的dir_control_t)
 *				inlet_type:功能函数的入口类型(参照ebook.h中的inlet_type_e)
 *                    (inlet_type)0-阅读界面;1-书签选择菜单，1-书签删除菜单,3-页数选择菜单
 * Returns     :
 *                无
 * Notes       :
 *
 ********************************************************************************/

app_result_e _deal_msg_result(input_gui_msg_t *ui_msg, dir_control_t *mlst_ctl, inlet_type_e inlet_type)
{
    app_result_e result = RESULT_NULL;
    uint8 line_num = 1;
    msg_apps_type_e gui_event; //gui 消息映射事件
    //进行按键映射
    if (com_key_mapping(ui_msg, &gui_event, _key_map_list) == TRUE)
    {

    }
    switch (gui_event)
    {
        case EVENT_TEXTREAD_NEXT_PAGE:
        if (inlet_type == PAGE_SEL)
        {
            line_num = 6;
        }
        if (inlet_type == NORMAL_TYPE)
        {
            line_num = 10;
        }
        case EVENT_TEXTREAD_KEY_DOWN:
        if (inlet_type != NORMAL_TYPE)
        {
            _select_next_item(mlst_ctl, line_num);
        }
        else
        {
            _sel_next_page(line_num);
        }
        break;
        case EVENT_TEXTREAD_PREV_PAGE:
        if (inlet_type == PAGE_SEL)
        {
            line_num = 6;
        }
        if (inlet_type == NORMAL_TYPE)
        {
            line_num = 10;
        }
        case EVENT_TEXTREAD_KEY_UP:
        if (inlet_type != NORMAL_TYPE)
        {
            _select_prev_item(mlst_ctl, line_num);
        }
        else
        {
            _sel_prev_page(line_num);
        }
        break;
        case EVENT_TEXTREAD_RETURN:
        if (inlet_type != NORMAL_TYPE)
        {
            result = RESULT_REDRAW;
        }
        else
        {
            result = RESULT_CANCEL;
        }
        break;
        case EVENT_TEXTREAD_BASE:
        result = RESULT_MAIN_APP;
        break;
        case EVENT_TEXTREAD_OPTION:
        if (inlet_type != NORMAL_TYPE)
        {
            result = RESULT_REDRAW;
        }
        else
        {
            result = RESULT_EBK_READINGMENU; //EBOOK_SCENE_READINGMENU;
            is_reading_menu = TRUE;
        }
        break;

        case EVENT_TEXTREAD_PLAY:
        if (inlet_type != NORMAL_TYPE)
        {
            result = _deal_confirm_item(mlst_ctl, inlet_type);
        }
        else
        {
            g_ebook_vars.play_flag = !g_ebook_vars.play_flag;
            draw_auto_flag = TRUE;
        }
        break;
        default:
        result = com_message_box(gui_event);
        if ((result == RESULT_REDRAW) || (result == RESULT_CONFIRM))
        {
            need_draw = TRUE;
            numboxflag = TRUE;
            if (inlet_type == NORMAL_TYPE)
            {
                _get_page_offset(curpagenum);
                view_decode.valid = FALSE;
                draw_auto_flag = TRUE;
            }
            result = RESULT_NULL;
        }
        break;
    }
    return result;
}

