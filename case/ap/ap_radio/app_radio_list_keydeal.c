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

/******************************************************************************/
/*!
 * \par  Description:
 *    选择电台列表中的下一项
 * \param[in]    p_ml_control：指向列表控制结构体
 * \param[out]   p_ml_control：返回更新了的列表参数
 * \return       none
 * \note
 *******************************************************************************/
void radiolist_select_next(radiolist_control_t *p_ml_control)
{
    //在页内切换，切后尚未到页底最后一项（最多到倒数第二项），更新激活项
    if ((p_ml_control->list_no + 1) < p_ml_control->bottom)
    {
        just_change_active_next: p_ml_control->list_no++;
        p_ml_control->old = p_ml_control->active;
        p_ml_control->active++;
        p_ml_control->draw_mode = LIST_DRAW_ACTIVE;
    }
    else
    {
        //在列表最后一项，跳到列表头显示
        if (p_ml_control->list_no == (p_ml_control->total - 1))
        {
            p_ml_control->list_no = 0;
            //仅有一页的情况下，只需更新激活项
            if (p_ml_control->total <= p_ml_control->one_page_count)
            {
                //总项数多余1项才需要切换
                if (p_ml_control->total > 1)
                {
                    p_ml_control->old = p_ml_control->active;
                    p_ml_control->active = 0;
                    p_ml_control->draw_mode = LIST_DRAW_ACTIVE;
                }
            }
            //从尾切换到头，更新整个列表
            else
            {
                p_ml_control->top = 0;
                p_ml_control->bottom = p_ml_control->one_page_count - 1;
                p_ml_control->old = p_ml_control->active = 0;
                p_ml_control->update_mode = LIST_UPDATE_HEAD;
                p_ml_control->draw_mode = LIST_DRAW_LIST;
            }
        }
        else
        {
            //在所有项中的倒数第二项，切到最后一项，只需更新激活项
            if (p_ml_control->list_no == (p_ml_control->total - 2))
            {
                goto just_change_active_next;
            }
            //在当前页中的倒数第二项，并且后面有足够列表项，向下切页，更新整个列表
            else
            {
                p_ml_control->list_no++;
                p_ml_control->top++;
                p_ml_control->bottom++;
                p_ml_control->old = p_ml_control->active;
                p_ml_control->update_mode = LIST_UPDATE_NEXT;
                p_ml_control->draw_mode = LIST_DRAW_LIST;
            }
        }
    }
}

/******************************************************************************/
/*!
 * \par  Description:
 *    选择电台列表中的上一项
 * \param[in]    p_ml_control：指向列表控制结构体
 * \param[out]   p_ml_control：返回更新了的列表参数
 * \return       none
 * \note
 *******************************************************************************/
void radiolist_select_prev(radiolist_control_t *p_ml_control)
{
    //在页内切换，切后尚未到页顶首项（最多到第二项），更新激活项
    if (p_ml_control->list_no > (p_ml_control->top + 1))
    {
        just_change_active_prev: p_ml_control->list_no--;
        p_ml_control->old = p_ml_control->active;
        p_ml_control->active--;
        p_ml_control->draw_mode = LIST_DRAW_ACTIVE;
    }
    else
    {
        //在列表第一项，跳到列表尾显示
        if (p_ml_control->list_no == 0)
        {
            p_ml_control->list_no = p_ml_control->total - 1;
            //仅有一页的情况下，只需更新激活项
            if (p_ml_control->total <= p_ml_control->one_page_count)
            {
                //总项数多余1项才需要切换
                if (p_ml_control->total > 1)
                {
                    p_ml_control->old = p_ml_control->active;
                    p_ml_control->active = p_ml_control->total - 1;
                    p_ml_control->draw_mode = LIST_DRAW_ACTIVE;
                }
            }
            //从头切到尾，更新整个列表
            else
            {
                p_ml_control->top = p_ml_control->total - p_ml_control->one_page_count;
                p_ml_control->bottom = p_ml_control->list_no;
                p_ml_control->old = p_ml_control->active = p_ml_control->one_page_count - 1;
                p_ml_control->update_mode = LIST_UPDATE_TAIL;
                p_ml_control->draw_mode = LIST_DRAW_LIST;
            }
        }
        else
        {
            //在所有项中的第二项，切到第一项，只需更新激活项
            if (p_ml_control->list_no == 1)
            {
                goto just_change_active_prev;
            }
            //在当前页中的第二项，并且前面有足够列表项，向上切页，更新整个列表
            else
            {
                p_ml_control->list_no--;
                p_ml_control->top--;
                p_ml_control->bottom--;
                p_ml_control->old = p_ml_control->active;
                p_ml_control->update_mode = LIST_UPDATE_PREV;
                p_ml_control->draw_mode = LIST_DRAW_LIST;
            }
        }
    }
}

/******************************************************************************/
/*!
 * \par  Description:
 *    选择电台列表中的一项进行播放
 * \param[in]    p_ml_control：指向列表控制结构体
 * \param[out]
 * \return       none
 * \note
 *******************************************************************************/
void select_current_play(radiolist_control_t *p_ml_control)
{
    uint16 *ptr = pfreq_list;
    uint16 cur_freq;
    bool is_saved = TRUE;

    cur_freq = *(ptr + p_ml_control->list_no);
    if (cur_freq == 0)
    {
        //所选择的项并无频点保存
        is_saved = FALSE;
    }

    //更新当前播放信息
    if (is_saved == TRUE)
    {
        g_radio_config.FMStatus.freq = cur_freq; //播放频率值
        g_radio_config.FMStatus.station = p_ml_control->list_no + 1; //当前电台号
    }
    else
    {
        g_radio_config.FMStatus.freq = min_freq[g_radio_config.band_mode];
        g_radio_config.FMStatus.station = 0;
    }
}

