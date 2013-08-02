/*
 *******************************************************************************
 *                              USDK
 *                             Module: TOOLS AP
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : menu_callback_stopwatch_tools.C
 * History:
 *      <johnsen>    <2011-9-25 20:43>           <v1.0 >             <desc>
 *
 *******************************************************************************
 */
/* include file of this application, 本应用的头文件 */
#include "ap_tools.h"

////////////////////

#define      MAX_ITEMNUM    5 //秒表项总数
#define      BYCOR         27 //head项Y坐标
#define      BXCOR         12 //head项X坐标
#define      CXCOR         37 //计时项X坐标
#define      CYCOR          6 //项序号X坐标

#define      X_COLON1      52 //冒号X坐标
#define      X_COLON2      74 //冒号X坐标2
#define      X_COMMA       96 //逗号X坐标

#define  menuhead_s_pos_x_def       0 // 字符串起始X坐标
#define  menuhead_s_pos_y_def       2 // 字符串起始Y坐标
/*项序号Y坐标*/
const uint8 ItemYcoordinate[MAX_ITEMNUM] =
{ 53, 75, 97, 119, 141 };
/*计时X坐标*/
const uint8 X_coordinateA[8] =
{ 0, 7, 21, 29, 43, 51, 65, 73 };
/*head项X坐标*/
const uint8 X_coordinateB[8] =
{ 0, 11, 28, 39, 56, 67, 83, 94 };
/*计时Y坐标*/
const uint8 Ycoordinate[5] =
{ 57, 79, 101, 123, 145 };
/*背景Y坐标*/
const uint8 Y_item_bg[5] =
{ 50, 72, 94, 116, 138 };
/*当前计时时间*/
uint8 now_time[9];
//缓存计数过的item
uint8 Item_tmp[5];

/******************************************************************************/
/*!
 * \par  Description:
 *	  stopwatch的初始函数
 * \param[in]    uint8 InitialMode
 * \param[in]    uint8 ItemNum
 * \param[out]   none
 * \return
 * \retval
 * \ingroup     menu_callback_stopwatch_tools.C
 * \note        因bank空间问题，移至此处
 *******************************************************************************/
void InitialTime(uint8 InitialMode, uint8 ItemNum)
{
    sw_realtime.Hour = 0;
    sw_realtime.Minute = 0;
    sw_realtime.Second = 0;
    sw_realtime.Ms = 0;
    //if (InitialMode == 0)
    //{
    //    swtime[ItemNum].Hour = 0;
    //    swtime[ItemNum].Minute = 0;
    //    swtime[ItemNum].Second = 0;
    //    swtime[ItemNum].Ms = 0;
    //}
    //else
    {
        uint8 cnt;
        for (cnt = 0; cnt < 5; cnt++)
        {
            swtime[cnt].Hour = 0;
            swtime[cnt].Minute = 0;
            swtime[cnt].Second = 0;
            swtime[cnt].Ms = 0;
        }
    }
}

/******************************************************************************/
/*!
 * \par  Description:
 *	  将时间转换为字符串
 * \param[in]    uint8 InitialMode
 * \param[in]    uint8 ItemNum
 * \param[out]   none
 * \return
 * \retval
 * \ingroup     menu_callback_stopwatch_tools.C
 * \note
 *******************************************************************************/
void swtime_itoa(uint8 ItemNum)
{
    if ((sw_realtime.Hour >= 100) || (swtime[ItemNum].Hour >= 100))
    {
        InitialTime(1, ItemNum);
    }

    if (sw_status == RunMode)
    {
        libc_memcpy(&swtime[ItemNum], &sw_realtime, sizeof(StopWatch_time_t));
    }
    else
    {
        libc_memcpy(&sw_realtime, &swtime[ItemNum], sizeof(StopWatch_time_t));
    }

    libc_itoa(swtime[ItemNum].Hour, &now_time[0], 2);
    libc_itoa(swtime[ItemNum].Minute, &now_time[2], 2);
    libc_itoa(swtime[ItemNum].Second, &now_time[4], 2);
    libc_itoa(swtime[ItemNum].Ms, &now_time[6], 2);
}

/******************************************************************************/
/*!
 * \par  Description:
 *	  显示当前项时间
 * \param[in]    uint8 ItemNum
 * \param[in]    uint8 *string
 * \param[out]   none
 * \return
 * \retval
 * \ingroup     menu_callback_stopwatch_tools.C
 * \note
 *******************************************************************************/
void ui_show_active_time(uint8 ItemNum, const uint8 *string)
{
    uint8 id, i;

    swtime_itoa(ItemNum);
    if (sw_showflag == 0)
    {
        ui_show_pic(P_SW_ACT_BG, 0, Y_item_bg[ItemNum]);
    }
    for (i = 0; i < 8; i++)
    {
        id = *(string + i) - 0x30;
        ui_show_pic(P_GREENC0 + id, CXCOR + X_coordinateA[i], Ycoordinate[ItemNum]);
        ui_show_pic(P_WATCHB0 + id, BXCOR + X_coordinateB[i], BYCOR);
    }
    //if(sw_status == RunMode)
    //{
    //    return;
    //}
    if (sw_showflag == 0)
    {
        ui_show_pic(P_GREENC_11, X_COLON1, Ycoordinate[ItemNum]);
        ui_show_pic(P_GREENC_11, X_COLON2, Ycoordinate[ItemNum]);
        ui_show_pic(P_GREENC_10, X_COMMA, Ycoordinate[ItemNum]);
        ui_show_pic(P_WATCHS1 + ItemNum, CYCOR, ItemYcoordinate[ItemNum]);
        //sw_showflag = 1;
    }
}

/******************************************************************************/
/*!
 * \par  Description:
 *	  显示非当前项时间
 * \param[in]    uint8 ItemNum
 * \param[in]    uint8 *string
 * \param[out]   none
 * \return
 * \retval
 * \ingroup     menu_callback_stopwatch_tools.C
 * \note
 *******************************************************************************/
void ui_show_unactivetime(uint8 ItemNum, const uint8 *string)
{
    uint8 id, i;

    swtime_itoa(ItemNum);
    ui_show_pic(P_SW_INACT_BG, 0, Y_item_bg[ItemNum]);
    for (i = 0; i < 8; i++)
    {
        id = *(string + i) - 0x30;
        ui_show_pic(P_WATCHC0 + id, CXCOR + X_coordinateA[i], Ycoordinate[ItemNum]);
    }

    ui_show_pic(P_WATCHC_11, X_COLON1, Ycoordinate[ItemNum]);
    ui_show_pic(P_WATCHC_11, X_COLON2, Ycoordinate[ItemNum]);
    ui_show_pic(P_WATCHC_10, X_COMMA, Ycoordinate[ItemNum]);
}

void ui_show_init(uint8 ItemNum) //显示当前时间
{
    uint8 cnt;
    string_desc_t desc;
    region_t SWTitle_region;

    sw_showflag = 0;
    ui_show_pic(P_SW_BG, 0, 0);

    ui_set_pen_color(Color_WHITE_def);
    SWTitle_region.x = menuhead_s_pos_x_def;
    SWTitle_region.y = menuhead_s_pos_y_def;
    SWTitle_region.width = 128;
    SWTitle_region.height = 16;
    desc.data.id = S_STOPWATCH;
    desc.language = UNICODEID;
    ui_show_string(&desc, &SWTitle_region, DISP_ALIGN_MEDIACY);

    for (cnt = 0; cnt < MAX_ITEMNUM; cnt++)
    {
        if (cnt != ItemNum)
        {
            if ((sw_status != CloseMode) && (Item_tmp[cnt] == 1))
            {
                ui_show_unactivetime(cnt, now_time);
            }
            ui_show_pic(P_WATCH1 + cnt, CYCOR, ItemYcoordinate[cnt]);
        }
    }
    ui_show_active_time(ItemNum, now_time);
}

/******************************************************************************/
/*!
 * \par  Description:
 *	  stopwatch的main函数
 * \param[in]    void
 * \param[out]   app_result_e result
 * \return
 * \retval
 * \ingroup     menu_callback_stopwatch_tools.C
 * \note        因实时性要求，按键操作等均在一个函数中，应注意空间问题
 *******************************************************************************/
app_result_e stopwatch_main(void)
{
    uint8 cnt;
    uint8 ItemNum;
    uint8 OldItemNum = 0;
    //bool msg_result;
    bool need_draw_all = TRUE;
    bool need_draw = TRUE;
    bool redraw_all = FALSE;
    int32 timer_id;
    app_result_e result = RESULT_NULL;
    //消息
    input_gui_msg_t ui_msg; /* UI消息 */
    private_msg_t pri_msg; /* 进程私有消息和系统消息 */
    msg_apps_type_e gui_event;

    //初始化项
    libc_memset(&Item_tmp, 0, 5);
    //InitialCtc();
    //sw_itemnum = 0;
    sw_showflag = 0;
    sw_status = CloseMode;

    //timer_id = set_app_timer(APP_TIMER_ATTRB_UI, 10, StopWatch_timer_proc);
    timer_id = sys_set_irq_timer1(StopWatch_timer_proc, 1);
    ItemNum = 0;//sw_itemnum;
    InitialTime(1, ItemNum);

    while (1)
    {
        if (need_draw_all == TRUE)
        {
            ItemNum = 0;
            //sw_itemnum = 0;
            OldItemNum = 0;
            ui_show_init(ItemNum);

            need_draw_all = FALSE;
            need_draw = TRUE;
        }
        if (need_draw == TRUE)
        {
            sw_showflag = 0;
            //ui_show_unactivetime(OldItemNum, now_time);
            ui_show_active_time(ItemNum, now_time);
            ui_show_pic(P_WATCH1 + OldItemNum, CYCOR, ItemYcoordinate[OldItemNum]);
            //ui_show_pic(P_WATCHS1 + ItemNum, CYCOR, ItemYcoordinate[ItemNum]);

            need_draw = FALSE;
        }
        if (redraw_all == TRUE)
        {
            ui_show_init(ItemNum);
            redraw_all = FALSE;
        }

        if (get_gui_msg(&ui_msg) == TRUE)//有ui消息
        {
            key_event_t kmsg = ui_msg.data.kmsg;
            switch (kmsg.val | kmsg.type)
            {
                case KEY_PLAY | KEY_TYPE_SHORT_UP:
                {
                    if (sw_status != RunMode) //开始计时
                    {
                        sw_status = RunMode;
                        CtcCntEnable = 1;
                        change_app_state(APP_STATE_PLAYING_ALWAYS);
                    }
                    else if (sw_status == RunMode)
                    {
                        //change_app_state(APP_STATE_NO_PLAY);
                        sw_status = StopMode;
                        CtcCntEnable = 0;
                    }
                    else
                    {
                        //for QAC
                    }
                    //ui_show_active_time(ItemNum, now_time);
                    //ui_show_pic(P_WATCHS1 + ItemNum, CYCOR, ItemYcoordinate[ItemNum]);
                }
                Item_tmp[0] = 1;
                break;

                case KEY_NEXT | KEY_TYPE_SHORT_UP:
                //case KEY_UP | KEY_TYPE_SHORT_UP:
                {
                    if (ItemNum < (MAX_ITEMNUM - 1))
                    {
                        OldItemNum = ItemNum;
                        ItemNum++;
                    }
                    else
                    {
                        OldItemNum = ItemNum;
                        ItemNum = 0;
                    }

                    ui_show_unactivetime(OldItemNum, now_time);
                    if (sw_status == RunMode) //继续下项计时
                    {
                        if (kmsg.val == KEY_NEXT)
                        {
                            libc_memcpy(&swtime[ItemNum], &sw_realtime, sizeof(StopWatch_time_t));
                        }
                        //else if (kmsg.val == KEY_UP)
                        //{
                        //    InitialTime(0, ItemNum);
                        //}
                        else
                        {
                        }
                    }
                    Item_tmp[ItemNum] = 1;
                    need_draw = TRUE;
                    //ui_show_pic(P_WATCH1 + OldItemNum, CYCOR, ItemYcoordinate[OldItemNum]);
                    //ui_show_pic(P_WATCHS1 + ItemNum, CYCOR, ItemYcoordinate[ItemNum]);
                    //ui_show_active_time(ItemNum, now_time);
                }
                break;

                case KEY_PREV | KEY_TYPE_SHORT_UP:
                //case KEY_DOWN | KEY_TYPE_SHORT_UP:
                {
                    if (ItemNum > 0)
                    {
                        OldItemNum = ItemNum;
                        ItemNum--;
                    }
                    else
                    {
                        OldItemNum = ItemNum;
                        ItemNum = MAX_ITEMNUM - 1;
                    }

                    ui_show_unactivetime(OldItemNum, now_time);
                    if (sw_status == RunMode) //继续上项计时
                    {
                        if (kmsg.val == KEY_PREV)
                        {
                            libc_memcpy(&swtime[ItemNum], &sw_realtime, sizeof(StopWatch_time_t));
                        }
                        //else if (kmsg.val == KEY_DOWN)
                        //{
                        //    InitialTime(0, ItemNum);
                        //}
                        else
                        {
                        }
                    }
                    Item_tmp[ItemNum] = 1;
                    need_draw = TRUE;
                    //ui_show_pic(P_WATCH1 + OldItemNum, CYCOR, ItemYcoordinate[OldItemNum]);
                    //ui_show_pic(P_WATCHS1 + ItemNum, CYCOR, ItemYcoordinate[ItemNum]);
                    //ui_show_active_time(ItemNum, now_time);
                }
                break;

                case KEY_MODE | KEY_TYPE_SHORT_UP: //全部清零
                {
                    //if (sw_status != CloseMode)
                    {
                        sw_status = CloseMode;
                        CtcCntEnable = 0;
                        libc_memset(&Item_tmp, 0, 5);
                        //                        ui_show_pic(P_SW_BG, 0, 0);
                        //                        ui_show_string(&desc, &SWTitle_region, DISP_ALIGN_MEDIACY);
                        ItemNum = 0;
                        //                        sw_itemnum = 0;
                        OldItemNum = 0;
                        InitialTime(1, ItemNum);
                        ui_show_init(ItemNum);
                        //                        for (cnt = 0; cnt < MAX_ITEMNUM; cnt++)
                        //                        {
                        //                            if (cnt != ItemNum)
                        //                            {
                        //                                ui_show_pic(P_WATCH1 + cnt, CYCOR, ItemYcoordinate[cnt]);
                        //                            }
                        //                        }
                        change_app_state(APP_STATE_NO_PLAY);

                        //ui_show_pic(P_WATCHS1 + ItemNum, CYCOR, ItemYcoordinate[ItemNum]);
                    }
                }
                break;

                //case KEY_RETURN | KEY_TYPE_SHORT_UP:
                case KEY_VOL | KEY_TYPE_SHORT_UP:
                result = RESULT_TOOLS_MENULIST;
                break;

                default:
                {
                    if (com_key_mapping(&ui_msg, &gui_event, NULL) == TRUE)
                    {
                        result = com_message_box(gui_event);
                        if (result != RESULT_NULL)
                        {
                            CtcCntEnable = 0;
                            sw_status = StopMode;
                            if (result == RESULT_REDRAW)
                            {
                                //need_draw_all = TRUE;
                                redraw_all = TRUE;
                            }
                            break;
                        }
                    }
                }
                break;
            }
        } //gui_message
        if (get_app_msg(&pri_msg) == TRUE)
        {
            result = tool_privmsg_deal(&pri_msg);
            if (result != RESULT_NULL)
            {
                CtcCntEnable = 0;
                sw_status = StopMode;
                if (result == RESULT_REDRAW)
                {
                    //need_draw_all = TRUE;
                    redraw_all = TRUE;
                }
                //break;
            }
            //            if(result == RESULT_REDRAW)
            //            {
            //                redraw_all = TRUE;
            //                //need_draw_all = TRUE;
            //            }
            //            else if(result == RESULT_APP_QUIT)
            //            {
            //                sw_status = stopMode;
            //            }
            //            else
            //            {
            //            }
            //else if(result != RESULT_NULL)
            //{
            //    break;
            //}
        }
        //挂起10ms，多任务调度
        sys_os_time_dly(1);
        if (sw_status == RunMode)
        {
            ui_show_active_time(ItemNum, now_time);
            sw_showflag = 1;
        }
        if ((result != RESULT_NULL) && (result != RESULT_REDRAW))
        {
            break;
        }
    } //while(1)
    CtcCntEnable = 0;
    change_app_state(APP_STATE_NO_PLAY);
    //kill_app_timer(timer_id);
    sys_del_irq_timer1(timer_id);
    return result;
}
