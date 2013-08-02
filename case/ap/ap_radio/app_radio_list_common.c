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

extern bool scene_list_init(void)
__FAR__;
extern bool scene_list_exit(void)
__FAR__;

/************************************************************************************************/
/*!
 * \par  Description:
 * \void get_one_item(uint8 index, uint8 *p_item)
 * \param[in]   index-- index+1 即为电台号 p_item  存储buffer
 * \param[out]   none
 * \return
 * \retval
 * \retval
 * \note   载入一项列表项数据
 */
/*************************************************************************************************/
void get_one_item(uint8 index, uint8 *p_item)
{
    uint16 *p_cur = pfreq_list; //指向当前浏览电台列表头
    uint16 cur_freq;
    bool is_saved = TRUE;
    uint8 tmp_buf[LIST_STR_LENGTH];
    //uint8 tmp;
    uint8 fix_buf[8] = "   FM  ";

    libc_memset(tmp_buf, 0, LIST_STR_LENGTH);
    //电台列表中保存的频率值
    cur_freq = *(p_cur + index);
    if (cur_freq == 0)
    {
        is_saved = FALSE; //当前项未保存
    }

    //将频率值转为ascill 存储xx.xx  或者xxx.xx
    libc_memset(Freqtab, 0, sizeof(Freqtab));
    freq_change(Freqtab, cur_freq, 1);

    //先处理列表项序号，即电台号
    libc_itoa(index + 1, &tmp_buf[0], 2);

    libc_memcpy(&tmp_buf[2], fix_buf, 7);

    //有频点保存
    if (is_saved == TRUE)
    {
        libc_strncat(tmp_buf, Freqtab, 7);
    }
    else
    {
        libc_strncat(tmp_buf, "-/-", 4);
    }

    libc_memcpy(p_item, tmp_buf, LIST_STR_LENGTH);
    return;
}

/************************************************************************************************/
/*!
 * \par  Description:
 * \void load_onepage_item_data(radiolist_control_t *list)
 * \param[in]   p_list_control
 * \param[out]   none
 * \return
 * \retval
 * \retval
 * \note   载入一屏显示数据
 */
/*************************************************************************************************/
void load_onepage_item_data(radiolist_control_t *list)
{
    uint8 i;

    for (i = 0; i < list->one_page_count; i++)
    {
        if ((i + list->top) > list->bottom)
        {
            break;
        }
        //根据频率组织一项显示数据
        get_one_item(list->top + i, g_radiolist_item[i]);
    }
}

/************************************************************************************************/
/*!
 * \par  Description:
 * \void init_list_control(radiolist_control_t *p_list_control)
 * \param[in]   p_list_control
 * \param[out]   none
 * \return
 * \retval
 * \retval
 * \note   初始化radiolist_control_t数据结构
 */
/*************************************************************************************************/
void init_list_control(radiolist_control_t *p_list_control)
{
    uint8 num;

    //判断当前播放频率是否在需浏览的电台列表中
    num = whether_in_list(g_radio_config.FMStatus.freq, pfreq_list);
    if (num == 0xff)
    {
        //不在列表中，激活项位于第一项
        p_list_control->top = 0;
        p_list_control->list_no = 0;
    }
    else
    {
        //在列表中，激活项索引等于电台号-1
        p_list_control->list_no = num - 1;
    }

    //其他参数
    //列表总数为支持的最大电台数
    p_list_control->total = MAX_STATION_COUNT;

    //尽量让激活项处于倒数第二项
    if ((p_list_control->total > p_list_control->one_page_count) && (p_list_control->list_no
            >= (p_list_control->one_page_count - 2)))
    {
        p_list_control->top = p_list_control->list_no - (p_list_control->one_page_count - 2);
    }
    else
    {
        p_list_control->top = 0;
    }

    p_list_control->bottom = p_list_control->top + p_list_control->one_page_count - 1;
    if (p_list_control->bottom > (p_list_control->total - 1))
    {
        p_list_control->bottom = p_list_control->total - 1;
    }
    p_list_control->old = p_list_control->active = p_list_control->list_no - p_list_control->top;
}

/************************************************************************************************/
/*!
 * \par  Description:
 * \void update_list_private(radiolist_control_t *p_ml_control, listbox_private_t *list_private)
 * \param[in]   p_ml_control    list_private
 * \param[out]   none
 * \return
 * \retval
 * \retval
 * \note   更新listbox 结构
 */
/*************************************************************************************************/
void update_list_private(radiolist_control_t *p_ml_control, listbox_private_t *list_private)
{
    uint8 index;

    //电台列表Style 并不显示标题
    list_private->title.data.str = NULL;
    list_private->title.length = -1;
    list_private->title.language = (int8) g_comval.language_id;

    //本页菜单中前几项有效
    list_private->item_valid = p_ml_control->bottom - p_ml_control->top + 1;
    list_private->active = p_ml_control->active;
    list_private->old = p_ml_control->old;
    list_private->list_no = p_ml_control->list_no;
    list_private->list_total = p_ml_control->total;

    for (index = 0; index < list_private->item_valid; index++)
    {
        list_private->items[index].data.str = g_radiolist_item[index];
        list_private->items[index].length = -1;
        list_private->items[index].language = ANSIDATAAUTO;

        list_private->items[index].argv = ITEMS_BACK_TAG_ISNOT_SELECT;
    }
}

/************************************************************************************************/
/*!
 * \par  Description:
 * \void radiolist_scroll_handle(void)
 * \param[in]
 * \param[out]   none
 * \return
 * \retval
 * \retval
 * \note   如有必要，电台列表项滚屏显示
 */
/*************************************************************************************************/
void radiolist_scroll_handle(void)
{
    ui_show_listbox((void*) NULL, (void*) NULL, LIST_DRAW_ACTIVE_SCROLL);
}

/************************************************************************************************/
/*!
 * \par  Description:
 * \app_result_e radioUI_scene_stationlist(radio_listtype_e  list_type, radio_list_entry_e  entry_mode, uint8 from)
 * \
 * \param[in]    list_type     电台列表类型
 * \                 entry_mode   电台列表入口模式
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \note   用户电台列表只支持小机端浏览，不支持添加删除
 */
/*************************************************************************************************/
app_result_e radioUI_scene_stationlist(radio_listtype_e list_type, radio_list_entry_e entry_mode, uint8 from)
{
    bool result;
    app_result_e ret = RESULT_IGNORE;

    result = scene_list_init();
    if (list_type == STATIONLIST_USER)
    {
        /* 显示用户电台列表*/
        ret = show_user_station();
    }
    else
    {
        /* 显示预设电台列表*/
        ret = show_saved_station(entry_mode, from);
    }
    scene_list_exit();
    return ret;
}

