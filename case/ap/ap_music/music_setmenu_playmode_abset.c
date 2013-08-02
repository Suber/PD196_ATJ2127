/*******************************************************************************
 *                              US212A
 *                            Module: music ui
 *                 Copyright(c) 2003-2012 Actions Semiconductor
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *     fiona.yang   2011-09-19          1.0              create this file
 *******************************************************************************/

#include "app_music.h"

//globle data for paramter box private data

parambox_one_t ab_param_box;

parambox_private_t ab_param_private;

param_com_data_t ab_param_comdata;

/******************************************************************************/
/*!
 * \par  Description:
 * \bool music_start_ab_set(void)
 * \启动ab复读
 * \param[in]    void  para1
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      music_setmenu_playmode_abset.c
 * \note
 */
/*******************************************************************************/
bool music_start_ab_set(void)
{
    bool bret;
    bret = music_send_msg(NULL, NULL, MSG_MENGINE_START_AB_SYNC);
    return bret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e set_scene_abmode_sure(void *param)
 * \启动ab复读模式
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval       RESULT_PLAYING_SCENE 回到播放场景
 * \retval       RESULT_IGNORE 忽略
 * \ingroup      music_setmenu_playmode_abset.c
 * \note
 */
/*******************************************************************************/
app_result_e set_scene_abmode_sure(void *param)
{
    //设置ab复读次数
    music_set_ab_count(g_music_config.ab_count);

    //设置ab复读间隔
    music_set_ab_gap(g_music_config.ab_gap);

    //启动ab复读
    music_start_ab_set();

    //返回到播放场景
    return RESULT_PLAYING_SCENE;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool music_set_ab_count(uint8 count)
 * \设置ab复读次数
 * \param[in]    void  para1
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      music_control.c
 * \note
 */
/*******************************************************************************/
bool music_set_ab_count(uint16 count)
{
    bool bret;
    //发送同步消息
    bret = music_send_msg(&count, NULL, MSG_MENGINE_SET_AB_COUNT_SYNC);
    return bret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e set_scene_setabcount_sure(void *param)
 * \设置ab复读次数
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval       RESULT_PLAYING_SCENE 回到播放场景
 * \retval       RESULT_IGNORE 忽略
 * \ingroup      music_setmenu_playmode_abset.c
 * \note
 */
/*******************************************************************************/
app_result_e set_scene_setabcount_sure(void *param)
{
    app_result_e ret_vals;
    //控件参数设置
    ab_param_comdata.private_data = &ab_param_private;

    ab_param_private.back_id = -1;
    ab_param_private.param_cnt = 0x01;
    ab_param_private.active = 0;
    ab_param_private.items = &ab_param_box;
    ab_param_private.sign_flag = 0;

    ab_param_private.icon_id = -1;
    ab_param_private.title_id = -1;

    ab_param_box.min = 1;
    ab_param_box.max = 10;
    ab_param_box.step = 1;
    ab_param_box.value = (int16) g_music_config.ab_count;
    ab_param_box.cycle = TRUE;
    //参数值最大位数
    ab_param_box.max_number = 2;

    ab_param_box.adjust_func = NULL;
    ab_param_box.callback = NULL;
    ab_param_box.value_str = NULL;

    //单位
    ab_param_box.unit_id = (uint16) - 1;

    //控件调用
    ret_vals = gui_set_parameter(PARAM_SIMPLE, &ab_param_comdata);

    if (ret_vals == RESULT_CONFIRM)
    {

        g_music_config.ab_count = (uint16) ab_param_box.value;
        //设置ab复读时间
        music_set_ab_count((uint8) g_music_config.ab_count);
        //返回到播放场景
        ret_vals = RESULT_PLAYING_SCENE;
    }

    return ret_vals;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool music_set_ab_gap(uint8 gaptime)
 * \设置ab复读间隔
 * \param[in]    void  para1
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      music_control.c
 * \note
 */
/*******************************************************************************/
bool music_set_ab_gap(uint16 gaptime)
{
    bool bret;
    //发送同步消息
    bret = music_send_msg(&gaptime, NULL, MSG_MENGINE_SET_AB_GAP_SYNC);
    return bret;    
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e set_scene_setabgap_sure(void *param)
 * \设置ab复读间隔
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval       RESULT_PLAYING_SCENE 回到播放场景
 * \retval       RESULT_IGNORE 忽略
 * \ingroup      music_setmenu_playmode_abset.c
 * \note
 */
/*******************************************************************************/
app_result_e set_scene_setabgap_sure(void *param)
{
    app_result_e ret_vals;
    //控件参数设置
    ab_param_comdata.private_data = &ab_param_private;

    ab_param_private.back_id = -1;
    ab_param_private.param_cnt = 0x01;
    ab_param_private.active = 0;
    ab_param_private.items = &ab_param_box;
    ab_param_private.sign_flag = 0;

    ab_param_private.icon_id = -1;
    ab_param_private.title_id = -1;

    ab_param_box.min = 1;
    ab_param_box.max = 10;
    ab_param_box.step = 1;
    ab_param_box.value = (int16) g_music_config.ab_gap;
    ab_param_box.cycle = TRUE;
    //参数值最大位数
    ab_param_box.max_number = 2;

    ab_param_box.adjust_func = NULL;
    ab_param_box.callback = NULL;
    ab_param_box.value_str = NULL;

    //单位
    ab_param_box.unit_id = (uint16) - 1;

    //控件调用
    ret_vals = gui_set_parameter(PARAM_SIMPLE, &ab_param_comdata);

    if (ret_vals == RESULT_CONFIRM)
    {
        g_music_config.ab_gap = (uint16) ab_param_box.value;
        music_set_ab_gap(g_music_config.ab_gap);
        //返回到播放场景
        ret_vals = RESULT_PLAYING_SCENE;
    }

    return ret_vals;
}
