/*******************************************************************************
 *                              US212A
 *                            Module: radio ui
 *                 Copyright(c) 2003-2012 Actions Semiconductor
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       mikeyang         2011-09-13          1.0              create this file
 *******************************************************************************/
#include "app_radio.h"

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e radioUI_scene_dispatch(app_param_e enter_mode)
 * \场景调度器
 * \param[in]    enter_mode 进入模式
 * \param[out]   none
 * \return       app_result_e the result
 * \retval       返回到何处
 * \retval
 * \note
 */
/*******************************************************************************/
app_result_e radioUI_scene_dispatch(app_param_e enter_mode)
{
    app_result_e scene_result;
    engine_type_e engine_type;

    //获取当前后台引擎状态
    engine_type = get_engine_type();

    switch (enter_mode)
    {
        //从main menu选择了radio 进入，需进入主菜单场景
        case PARAM_FROM_MAINMENU:
        g_radio_scene = FM_SCENE_MAINMENU;
        break;

        case PARAM_FROM_ALARM:
        g_radio_scene = FM_SCENE_PLAYING;
        break;

        //从main menu选择了正在播放/ 上次播放进入
        //从其他ap的option菜单选择了正在播放/上次播放
        case PARAM_BACKTO_PLAYING:
        case PARAM_BACKTO_PLAYED:
        g_radio_scene = FM_SCENE_PLAYING;
        g_playwin_mode = ENTER_PLAY_FROM_NOWPLAY;
        break;

        //从FM 录音进程返回后进入
        case PARAM_FROM_RECORD:
        g_radio_scene = FM_SCENE_PLAYING;
        g_playwin_mode = ENTER_PLAY_FROM_MAIN;
        need_reinstall = TRUE;
        break;

        //播放FM 时关机, 重新启动后直接进RADIO  播放,  需要重设频率
        case PARAM_FROM_CONFIG:
        g_radio_scene = FM_SCENE_PLAYING;
        g_playwin_mode = ENTER_PLAY_FROM_MAIN;
        break;

        //从放音返回后进入
        case PARAM_FROM_BROWSER:

        //如果是从FM  播放场景的option 进入VOICE
        if (g_radio_config.enter_voice == FROM_OPTION_MENU)
        {
            //还未进行录音文件的回放，则回option menu
            if (engine_type == ENGINE_RADIO)
            {
                g_radio_scene = FM_SCENE_OPTMENU;
                g_option_type = SUBS_OPT_PLAY;
            }

            //已经进行录音文件的回放, 后台引擎已经不是RADIO
            else
            {
                //返回播放场景，同时需开启radio 引擎
                g_radio_scene = FM_SCENE_PLAYING;
                g_playwin_mode = ENTER_PLAY_FROM_MAIN;
            }
        }

        //如果是从FM 一级主菜单进入VOICE ，返回到一级主菜单
        //其他情况( 参数异常) ， 返回到一级主菜单
        else //if(g_radio_config.enter_voice == FROM_RADIO_MAINMENU)
        {
            g_radio_scene = FM_SCENE_MAINMENU;
        }
        break;

        //默认情况，进入主菜单场景
        default:
        g_radio_scene = FM_SCENE_MAINMENU;
        break;
    }

    /* radioUI 场景循环*/
    while (g_radio_scene != FM_SCENE_EXIT)
    {
        switch (g_radio_scene)
        {
            /* 进入RadioUI 主菜单场景*/
            case FM_SCENE_MAINMENU:

            g_option_type = NO_ENTER_SUB;
            scene_result = radioUI_mainmenu();

            switch (scene_result)
            {
                //直接进入播放场景，手动调谐后
                case RESULT_RADIO_START_PLAY:
                g_radio_scene = FM_SCENE_PLAYING;
                g_playwin_mode = ENTER_PLAY_FROM_MAIN;
                break;

                //预设电台，用户电台，自动调谐后
                case RESULT_RADIO_STATION_LIST:
                case RESULT_RADIO_USER_LIST:
                g_radio_scene = FM_SCENE_STALIST;
                if (scene_result == RESULT_RADIO_USER_LIST)
                {
                    /* 进入用户电台列表*/
                    g_radio_listtype = STATIONLIST_USER;
                }
                else
                {
                    /* 进入预设电台列表*/
                    g_radio_listtype = STATIONLIST_SAVE;
                }
                break;

                //其他情况退出场景调度( 如FM 录制进入VOICE)
                default:
                g_radio_scene = FM_SCENE_EXIT;
                break;
            }
            break;

            /* 进入RADIO 播放场景*/
            case FM_SCENE_PLAYING:

            scene_result = radioUI_scene_playing(g_playwin_mode);

            switch (scene_result)
            {
                //回到主菜单界面
                case RESULT_RADIO_MAINMENU:

                g_radio_scene = FM_SCENE_MAINMENU;
                break;

                //进入预设电台列表界面
                case RESULT_RADIO_STATION_LIST:
                g_radio_scene = FM_SCENE_STALIST;
                g_radio_listtype = STATIONLIST_SAVE;
                break;

                //进入用户电台列表界面
                case RESULT_RADIO_USER_LIST:
                g_radio_scene = FM_SCENE_STALIST;
                g_radio_listtype = STATIONLIST_USER;
                break;

                //进入播放场景的option 菜单
                case RESULT_RADIO_OPTION_PLAYWIN:
                g_radio_scene = FM_SCENE_OPTMENU;
                g_option_type = SUBS_OPT_PLAY;
                break;

                //其他情况退出场景调度
                case RESULT_ALARM_QUIT:
                default:
                g_radio_scene = FM_SCENE_EXIT;
                break;
            }
            break;

            /*  进入电台列表场景( 包括预设列表和用户列表) */
            case FM_SCENE_STALIST:

            scene_result = radioUI_scene_stationlist(g_radio_listtype, g_stalist_entry, from_where);

            switch (scene_result)
            {
                //选择电台后，进入播放场景开始播放
                //删除电台后，回到播放场景播放
                //保存电台后，回到播放场景播放
                case RESULT_RADIO_START_PLAY:
                g_radio_scene = FM_SCENE_PLAYING;
                g_playwin_mode = ENTER_PLAY_FROM_LIST;
                break;

                //从列表返回Radio 主菜单
                case RESULT_RADIO_MAINMENU:
                g_radio_scene = FM_SCENE_MAINMENU;
                break;

                //进入电台列表option
                case RESULT_RADIO_OPTION_STATION:
                g_radio_scene = FM_SCENE_OPTMENU;
                g_option_type = SUBS_OPT_STATION;
                break;

                //进入播放场景option，如取消清除预设，保存到预设
                case RESULT_RADIO_OPTION_PLAYWIN:
                g_radio_scene = FM_SCENE_OPTMENU;
                g_option_type = SUBS_OPT_PLAY;
                break;

                //其他退出场景调度
                default:
                g_radio_scene = FM_SCENE_EXIT;
                break;
            }
            break;

            /*  进入option menu 场景( 包括播放界面下和电台列表下) */
            case FM_SCENE_OPTMENU:
            scene_result = radioUI_scene_optionlist(g_option_type);

            switch (scene_result)
            {
                //进入预设电台列表
                //可能是清除预设或保存到预设或普通浏览进入
                case RESULT_RADIO_STATION_LIST:
                g_radio_scene = FM_SCENE_STALIST;
                g_radio_listtype = STATIONLIST_SAVE;
                break;

                case RESULT_RADIO_START_PLAY:
                g_radio_scene = FM_SCENE_PLAYING;
                g_playwin_mode = ENTER_PLAY_FROM_MAIN;
                break;

                //其他情况退出场景调度
                default:
                g_radio_scene = FM_SCENE_EXIT;
                break;
            }
            break;

            default:
            //退出场景调度
            g_radio_scene = FM_SCENE_EXIT;
            break;
        }
    }
    return scene_result;

}

