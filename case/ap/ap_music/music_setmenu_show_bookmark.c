/*
 ******************************************************************************
 *                               USDK
 *                            Module: app
 *                 Copyright(c) 2002-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 * File: ui_show_bookmark.c
 * History:
 *      <author>    <time>           <version >             <desc>
 *      wuyufan    2011-9-1 9:27:12      1.0             build this file
 ******************************************************************************
 */
#include "app_music.h"
#include "app_music_bookmark.h"

//.rodata
const uint8 empty_items_str[] = "hh:mm:ss";
//const uint8 ROOT_DIR_NAME[] = ":";

const key_map_t bookmark_key_map_list[] =
{
    /*! UP 按键转换为 SELECT_PREV 事件 */
    {{ KEY_PREV, 0, KEY_TYPE_DOWN | KEY_TYPE_LONG | KEY_TYPE_HOLD }, EVENT_BOOKMARK_SELECT_PREV },
    /*! DOWN 按键转换为 SELECT_NEXT 事件 */
    {{ KEY_NEXT, 0, KEY_TYPE_DOWN | KEY_TYPE_LONG | KEY_TYPE_HOLD }, EVENT_BOOKMARK_SELECT_NEXT },
    /*! 短按KEY_PLAY 按键转换为 SELECT_SURE 事件 */
    {{ KEY_PLAY, 0, KEY_TYPE_SHORT_UP }, EVENT_BOOKMARK_SELECT_SURE },
    /*! 短按KEY_MENU 按键转换为 ENTER_OPTION 事件 */
    {{ KEY_MODE, 0, KEY_TYPE_SHORT_UP }, EVENT_BOOKMARK_OPTION },
    /*! 短按KEY_RETURN 按键转换为 RETURN 事件 */
    {{ KEY_VOL, 0, KEY_TYPE_SHORT_UP }, EVENT_BOOKMARK_RETURN },
    /*! 结束标志 */
    {{ KEY_NULL, 0, KEY_TYPE_NULL }, MSG_NULL },
};

uint16 bmk_cache_sector;
//uint8  bmk_cache_attr;
music_bm_head bm_head;


//bank data
uint8 disp_buf[MAX_BOOKMARKS][BM_ITEM_LEN] _BANK_DATA_ATTR_;

listbox_private_t mbk_show_data _BANK_DATA_ATTR_;

extern void menulist_select_next(menulist_control_t *p_ml_control);
extern void menulist_select_prev(menulist_control_t *p_ml_control);

/******************************************************************************/
/*!
 * \par  Description:
 *	  将获取到的标签时间转变为时分秒显示规格
 * \param[in]
 * \param[out]
 * \return
 * \retval
 * \ingroup     music_ui
 * \note
 *******************************************************************************/
void get_bookmark_item(uint8 *dispbuf, int index, time_t *time_buf)
{
    uint8 i;
    uint8 *tmp = (uint8 *) &time_buf[index];

    libc_itoa((index + 1), dispbuf, 2);
    dispbuf[2] = ' ';

    if ((time_buf[index].hour == 0xff) && (time_buf[index].minute == 0xff) && (time_buf[index].second == 0xff))
    {
        return;
    }
    else
    {
        for (i = 1; i < 4; i++)
        {
            libc_itoa(*tmp, &dispbuf[3 * i], 2);
            dispbuf[3 * i + 2] = ':';
            tmp++;
        }
    }
    dispbuf[11] = 0x00;
    return;
}

/******************************************************************************/
/*!
 * \par  Description:
 *	  浏览书签项控件
 * \param[in]  mode 浏览模式 添加/删除/选择书签项 msg_call_back 对应的进程私有消息
 * 			   和系统消息处理函数
 * \param[out]
 * \return
 * \retval
 * \ingroup     music_ui
 * \note
 *******************************************************************************/
void ui_show_bookmark_item(menulist_control_t* mbk_list_pt)
{
    style_infor_t mbk_infor;
    uint8 i;
    //uint8 draw_mode;

    mbk_infor.style_id = FILELIST;
    mbk_infor.type = UI_COM;

    //列表项items中有效前几项
    mbk_show_data.item_valid = mbk_list_pt->bottom - mbk_list_pt->top + 1;

    for (i = 0; i < mbk_show_data.item_valid; i++)
    {

        mbk_show_data.items[i].data.str = disp_buf[i + mbk_list_pt->top];
        mbk_show_data.items[i].language = ANSIDATAAUTO;
        mbk_show_data.items[i].length = BM_ITEM_LEN;
    }

    //当前激活项
    mbk_show_data.active = mbk_list_pt->active;
    //之前激活项
    mbk_show_data.old = mbk_list_pt->old;
    //所有项总数，用于滑动杆和ratio
    mbk_show_data.list_total = mbk_list_pt->total;
    //当前激活项在所有项的位置，用于滑动杆和ratio
    mbk_show_data.list_no = mbk_list_pt->list_no;

    ui_show_listbox(&mbk_infor, &mbk_show_data, mbk_list_pt->draw_mode);
}

/******************************************************************************/
/*!
 * \par  Description:
 *	  获取一屏显示的时间
 * \param[in]    list : 最高序号
 * \param[in]    bottom : 最低序号
 * \param[in]    cur_index : 当前项的序号
 * \param[out]   ml_control：返回更新了的菜单列表参数
 * \return       none
 * \note
 *******************************************************************************/
void get_one_screen_time(uint8 top, uint8 bottom, uint16 cur_index)
{
    uint8 i;
    //uint32 mbmk_fp;
    time_t bm_items[MAX_BOOKMARKS];
    //因为要读取文件,用到两外一个bank的bank内数据
    g_bookmark_fp = open_bookmark();
    get_bookmark_time(cur_index, bm_items, g_bookmark_fp);

    //获取书签信息
    for (i = top; i <= bottom; i++)
    {
        libc_memcpy(&disp_buf[i][3], empty_items_str, 9);
        get_bookmark_item(disp_buf[i], i, bm_items);
    }

    close_bookmark(g_bookmark_fp);
}

/******************************************************************************/
/*!
 * \par  Description:
 * \uint16 ui_get_bookmark_index(void)
 * \获取当前书签所在的index
 * \param[in]
 * \param[out]
 * \return     uint16
 * \retval     当前书签的序号
 * \ingroup     music_setmenu_bookmark_deal.c
 * \note
 *******************************************************************************/
uint16 ui_get_bookmark_index(void)
{
    uint16 index = 0xffff;

    file_location_t *plocation = (file_location_t *) &g_file_path_info.file_path;

    music_get_filepath(&g_file_path_info);

    //定位到文件
    //fsel_set_location(&(g_file_path_info.file_path.plist_location), g_file_path_info.file_source);
    fsel_browser_set_file(&(g_file_path_info.file_path.plist_location), g_file_path_info.file_source);
    //取出长名
    vfs_get_name(g_music_mount_id, g_song_filename, 32);

    // 更新文件列表
    vfs_cd(g_music_mount_id, CD_ROOT, 0);

    g_bookmark_fp = open_bookmark();

    if (g_bookmark_fp == 0)
    {
        gui_dialog_msg(DIALOG_MSG, DIALOG_INFOR_WAIT, S_NO_BOOKMARK);
    }
    else
    {
        //通过给出的cluster_no和dir_entry找到对应书签的index
        index = get_bookmark_index(plocation->cluster_no, plocation->dir_entry, g_song_filename, g_bookmark_fp);
        close_bookmark(g_bookmark_fp);
    }
    return index;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \uint16 ui_show_bookmark(uint8 mode, msg_cb_func msg_call_back)
 * \显示书签项
 * \param[in]    mode : 绘制模式
 * \param[in]    msg_call_back: 消息处理回调函数
 * \param[in]
 * \param[out]
 * \return       uint16
 * \note
 *******************************************************************************/
uint16 ui_show_bookmark(uint8 mode, msg_cb_func msg_call_back)
{
    bool ret;
    uint8 tag_backup;

    bool need_read_bmk = TRUE;

    uint16 cur_index, result;

    menulist_control_t ml_control_deal;

    input_gui_msg_t input_msg; //输入型消息，即gui消息

    //gui 消息对应gui 事件
    msg_apps_type_e gui_event;

    private_msg_t private_msg; //私有消息

    libc_memset(&ml_control_deal, 0, sizeof(ml_control_deal));
    ml_control_deal.total = MAX_BOOKMARKS;
    ml_control_deal.bottom = LIST_NUM_ONE_PAGE - 1;
    ml_control_deal.draw_mode = LIST_DRAW_ALL;
    ml_control_deal.one_page_count = LIST_NUM_ONE_PAGE;

    // 更新文件列表
    //vfs_cd(g_music_mount_id, 0, ROOT_DIR_NAME);

    cur_index = ui_get_bookmark_index();
    if (cur_index == 0xffff)
    {
        return RESULT_PLAYING_SCENE;
    }
    //书签可看做独立的子场景，该场景可能被common的ui_menulist调用，而这些场景的
    //定时器在该场景下不应该起作用，通过change_app_timer_tag,可以暂停这些定时器，待退出书签子场景
    //后再做恢复。需要注意这里设置的定时器TAG不能和applib.h定义的重复
    tag_backup = get_app_timer_tag();
    change_app_timer_tag(APP_TIMER_TAG_BOOKMARK);    

    while (1)
    {
        if (need_read_bmk == TRUE)
        {
            need_read_bmk = FALSE;
            get_one_screen_time(0, 9, cur_index);
        }
        if (ml_control_deal.draw_mode != LIST_DRAW_NULL)
        {
            ui_show_bookmark_item(&ml_control_deal);
            ml_control_deal.draw_mode = LIST_DRAW_NULL;
        }

        //获取gui消息
        ret = get_gui_msg(&input_msg);
        if (ret == TRUE)//有gui消息
        {
            ////进行按键映射
            if (com_key_mapping(&input_msg, &gui_event, bookmark_key_map_list) == TRUE)
            {
                result = RESULT_NULL;
                //GUI按键消息处理
                switch (gui_event)
                {
                    case EVENT_BOOKMARK_SELECT_NEXT:
                    //bmlist_select_next(&ml_control_deal);
                    menulist_select_next(&ml_control_deal);
                    //ml_control_deal.draw_mode = 0;
                    break;

                    case EVENT_BOOKMARK_SELECT_PREV:
                    //bmlist_select_prev(&ml_control_deal);
                    menulist_select_prev(&ml_control_deal);
                    //ml_control_deal.draw_mode = 0;
                    break;

                    case EVENT_BOOKMARK_OPTION:
                    result = RESULT_REDRAW;
                    break;

                    case EVENT_BOOKMARK_SELECT_SURE:
                    //按确认键，需要添加，删除或转入书签
                    handle_bookmark((uint8) ml_control_deal.list_no, mode, cur_index);
                    result = RESULT_PLAYING_SCENE;
                    break;
                    //返回按键
                    case EVENT_BOOKMARK_RETURN:
                    result = RESULT_REDRAW;
                    break;
                    default:
                    result = com_message_box(gui_event);
                    break;
                }
                if ((result != RESULT_NULL) && (result != RESULT_IGNORE))
                {
                    break;
                }
            }
        }
        else
        {
            //到此，gui消息队列已经处理完毕
            //处理ap私有消息和系统消息
            ret = get_app_msg(&private_msg);
            if (ret == TRUE)
            {
                result = msg_call_back(&private_msg);
                if ((result == RESULT_REDRAW) || (result == RESULT_NULL))
                {
                    need_read_bmk = TRUE;
                    ml_control_deal.draw_mode = LIST_DRAW_ALL;
                }
                else
                {
                    //goto menu_exit;
                    break;
                }
            }
        }
        sys_os_time_dly(2);

    }//end of while(1)
    change_app_timer_tag(tag_backup);    
    return result;
}
/******************************************************************************/
/*!
 * \par  Description:
 * \void del_bookmark_file(file_location_t *plocation)
 * \删除书签文件
 * \param[in]    plocation 文件路径
 * \param[out]
 * \return       none
 * \note
 *******************************************************************************/
void del_bookmark_file(file_location_t *plocation)
{
    //到根目录打开书签文件
    vfs_cd(g_music_mount_id, CD_ROOT, 0);

    g_bookmark_fp = open_bookmark();
    if (g_bookmark_fp != 0)
    {
        music_del_bookmark(plocation, 0, g_bookmark_fp);
        //vfs_file_close(g_music_mount_id, bm_fp);
        close_bookmark(g_bookmark_fp);
    }
    return;
}

