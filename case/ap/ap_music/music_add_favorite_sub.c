/*******************************************************************************
 *                              US212A
 *                            Module: MUSIC_ADD_FAVORITE
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>         <time>             <version>             <desc>
 *      wuyufan    2011-11-9 16:17:43           1.0              build this file
 *******************************************************************************/

#include "app_music.h"

#include "app_music_playing.h"

/*!
 * \brief
 *  delete_key_map_list：批量添加文件到收藏夹控件按键映射表
 */
const key_map_t add_favor_key_map_list[] =
{
    /*! 短按 VOL 键，终止添加后续文件 */
    {{KEY_VOL, 0, KEY_TYPE_SHORT_UP}, EVENT_PLAYING_RETURN},

    /*! 不响应以下快捷键 */
    {{KEY_LOCKHOLD, 0, KEY_TYPE_ALL}, MSG_NULL},
    {{KEY_VOL, 0, KEY_TYPE_LONG}, MSG_NULL},

    /*! 结束标志 */
    {{KEY_NULL, 0, KEY_TYPE_NULL}, MSG_NULL},
};

/******************************************************************************/
/*!
 * \par  Description:
 * \bool music_set_ab_count(uint8 count)
 * \通知后台添加当前播放的收藏夹文件
 * \param[in]    void  para1
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      music_favorite
 * \note
 */
/*******************************************************************************/
static bool music_add_favorite_deal(void)
{
    bool bret = TRUE;
    msg_apps_t msg;
    //消息类型(即消息名称)
    msg.type = MSG_MENGINE_ADD_FAVORITE_SYNC;
    //发送同步消息
    bret = send_sync_msg(APP_ID_MENGINE, &msg, NULL, 0);
    return bret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool ask_for_override_favorlist(void)
 * \查询当收藏夹数目超过最大数目时是否覆盖
 * \param[in]    void  para1
 * \param[out]   none
 * \return       bool the result
 * \retval           1 覆盖
 * \retval           0 不覆盖
 * \ingroup      music_favorite
 * \note
 */
/*******************************************************************************/
static app_result_e _ask_for_override_favorlist(void)
{
    app_result_e result;
    dialog_com_data_t ask_dialog_data;
    string_desc_t ask_create_str;

    //对话框是否创建播放列表
    ask_create_str.data.id = S_IS_OVERRIDE_FAVLIST;
    ask_create_str.language = UNICODEID;
    //对话框类型
    ask_dialog_data.dialog_type = DIALOG_BUTTON_INCLUDE;
    ask_dialog_data.icon_id = -1;
    //字符串
    ask_dialog_data.string_desc = &ask_create_str;

    //按键类型
    ask_dialog_data.button_type = BUTTON_YESNOCANCEL;
    /*! 默认激活按键项 */
    ask_dialog_data.active_default = 0x01;

    result = gui_dialog(DIALOG_ASK, &ask_dialog_data);

    if (result == RESULT_DIALOG_NO)
    {
        result = RESULT_IGNORE;
    }
    return result;
}
/******************************************************************************/
/*!
 * \par  Description:
 *   批量添加文件到收藏夹子场景函数
 * \param[in]  plocation 当前文件的路径信息
 * \param[in]  target_type 加入哪个收藏夹
 * \param[in]  fav_file_total 当前收藏夹文件总数
 * \param[in]  browser
 * \param[out]   none
 * \return       app_result_e the result
 * \retval           RESULT_REDRAW
 * \retval           0 不覆盖
 * \ingroup      music_favorite
 * \note
 */
/*******************************************************************************/
app_result_e add_favorlist_all(plist_location_t * plocation, uint8 target_type, uint16 fav_file_total,
        dir_brow_t *browser)
{
    uint8 tag_backup;
    flist_location_t flocation;
    scanfile_result_e browser_result;
    input_gui_msg_t input_msg;
    msg_apps_type_e gui_event;
    private_msg_t private_msg;
    fsel_error_e fsel_error;
    bool ret = TRUE;
    bool is_override_favlist = FALSE;
    app_result_e result = RESULT_NULL;

    //批量添加收藏夹可看做独立的子场景，该场景可能被common的ui_direcotry或ap的ablumlist调用，而这些场景的
    //定时器在该场景下不应该起作用，通过change_app_timer_tag,可以暂停这些定时器，待退出批量添加收藏夹子场景
    //后再做恢复。需要注意这里设置的定时器TAG不能和applib.h定义的重复
    tag_backup = get_app_timer_tag();
    change_app_timer_tag(APP_TIMER_TAG_FAVOR);

    //提示"请稍后"
    gui_dialog_msg(DIALOG_MSG, DIALOG_INFOR_IMMEDIATE, S_WAITING);

    //文件夹遍历浏览初始化
    fsel_browser_scanfile_init(browser);

    //遍历文件夹并删除文件及空文件夹
    while (1)
    {
        browser_result = fsel_browser_scanfile(&(g_browser_filepath), browser, SCANFILE_TYPE_DXCH);
        if (browser_result == SCANFILE_RESULT_FILE)//返回文件
        {
            if ((fav_file_total == USERPL_FILE_MAX) && (is_override_favlist == FALSE))
            {
                result = _ask_for_override_favorlist();
                if (result != RESULT_DIALOG_YES)
                {
                    //超过最大数目后终止当前添加操作
                    break;
                }
                else
                {
                    //开始覆盖当前的收藏夹文件
                    is_override_favlist = TRUE;
                }
            }

            fsel_change_fav_location(&(g_browser_filepath.file_path.plist_location), &flocation, FSEL_TYPE_PLAYLIST,
                    &g_id3_info);

            ret = fsel_favorite_add(&flocation, target_type);

            //未超过总数时更新总数并更新后台文件总数
            if (ret == TRUE)
            {
                if (fav_file_total != USERPL_FILE_MAX)
                {
                    fav_file_total++;
                    if (g_file_path_info.file_source == target_type)
                    {
                        music_add_favorite_deal();
                    }
                }
            }
            else
            {
                fsel_error = fsel_get_error();

                //批量添加模式，如果该文件已添加过�
                //则忽略该错误
                if (fsel_error != FSEL_ERR_EXIST)
                {
                    //出错终止当前添加操作
                    result = RESULT_ERROR;
                    break;
                }
            }

        }
        else//返回文件夹
        {
            //如果文件夹为空，忽略
            if ((browser->file_total == 0) && (browser->dir_total == 0))
            {
                ;//nothing
            }

            //文件夹自身或当前已回到playlist的最顶层，结束添加操作
            if ((browser_result == SCANFILE_RESULT_SELF) || (browser->layer_no == 0))
            {
                fsel_browser_enter_dir(PARENT_DIR, 0, browser);
                result = RESULT_REDRAW;
                break;
            }
        }

        //获取gui消息
        ret = get_gui_msg(&input_msg);
        if (ret == TRUE)//有gui消息
        {
            //进行按键映射
            if (com_key_mapping(&input_msg, &gui_event, add_favor_key_map_list) == TRUE)
            {
                //GUI按键消息处理
                switch (gui_event)
                {
                    case EVENT_PLAYING_RETURN:
                    //取消返回
                    return RESULT_IGNORE;

                    default:
                    break;
                }
            }
        }
        else
        {
            //到此，gui消息队列已经处理完毕
            //获取ap私有消息和系统消息（会先转换为私有消息再返回），并处理
            ret = get_app_msg(&private_msg);
            if (ret == TRUE)
            {
                result = music_message_deal(&private_msg);
                if ((result == RESULT_REDRAW) || (result == RESULT_NULL))
                {
                    //提示"请稍后"
                    gui_dialog_msg(DIALOG_MSG, DIALOG_INFOR_IMMEDIATE, S_WAITING);
                }
                else
                {
                    break;
                }
            }
        }

        //挂起10ms，多任务调度
        sys_os_time_dly(1);
    }
    change_app_timer_tag(tag_backup);
    return result;
}
