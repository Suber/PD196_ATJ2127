/*******************************************************************************
 *                              US212A
 *                            Module: music ui
 *                 Copyright(c) 2003-2012 Actions Semiconductor
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *     fiona.yang   2011-09-26          1.0              create this file
 *******************************************************************************/

#include "app_music.h"

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e list_scene_option_option(void)
 * \option菜单的option处理
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \ingroup      music_listmenu_optionitem.c
 * \note
 */
/*******************************************************************************/
app_result_e list_scene_option_option(void)
{
    //返回上一级
    return RESULT_MENU_PARENT;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e list_scene_musicplay_sure(void *param)
 * \music正在播放确认函数
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \ingroup      music_listmenu_optionitem.c
 * \note
 */
/*******************************************************************************/
app_result_e list_scene_musicplay_sure(void *param)
{
    //返回到播放场景
    return RESULT_PLAYING_KEEP;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e list_scene_musicpause_sure(void *param)
 * \music上一次播放确认函数
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \ingroup      music_listmenu_optionitem.c
 * \note
 */
/*******************************************************************************/
app_result_e list_scene_musicpause_sure(void *param)
{
    //返回到播放场景
    return RESULT_PLAYING_KEEP;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e list_scene_radioplay_sure(void *param)
 * \radio正在播放确认函数
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \ingroup      music_listmenu_optionitem.c
 * \note
 */
/*******************************************************************************/
app_result_e list_scene_radioplay_sure(void *param)
{
    return RESULT_NOWPLAYING;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e list_scene_radiomute_sure(void *param)
 * \radio上一次播放确认函数
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \ingroup      music_listmenu_optionitem.c
 * \note
 */
/*******************************************************************************/
app_result_e list_scene_radiomute_sure(void *param)
{
    //返回到radio播放场景
    return RESULT_LASTPLAY;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \file_path_info_t* list_scene_favorsel(fsel_type_e favor_from)
 * \选择收藏夹的第一个文件开始播放
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval       bool the result
 * \retval          1 success
 * \retval          0 fail
 * \ingroup      music_listmenu_optionitem.c
 * \note
 */
/*******************************************************************************/
bool list_scene_favorsel(file_path_info_t* select_path, fsel_type_e favor_from)
{
    uint8 cur_sel_save;
    uint16 cur_list_save;
    bool init_fsel = FALSE;
    bool init_result = TRUE;
    
    if(select_path->file_source != favor_from)
    {
        cur_sel_save = select_path->file_source;
        cur_list_save = select_path->file_path.plist_location.list_type;
        
        //退出文件选择器
        fsel_exit();
        //文件选择器初始化接口的参数设置
        select_path->file_source = favor_from;    
        init_result = music_file_init(select_path);
        init_fsel = TRUE;
    }
    
    if(init_result == TRUE)
    {
        //统计当前目录文件总数
        fsel_browser_enter_dir(CUR_DIR, 0, NULL);
        
        //获取当前模式下的第一个可播放文件
        init_result = fsel_browser_select(1);
        
        //找到可播放文件
        if (init_result == TRUE)
        {
            //获取当前文件的location信息
            init_result = fsel_browser_get_location(&g_browser_filepath.file_path.flist_location, favor_from);
        }
        else
        {
            if(init_fsel == TRUE)
            {
                //退出文件选择器
                fsel_exit();
                
                select_path->file_source = cur_sel_save;
                select_path->file_path.plist_location.list_type = cur_list_save;

                music_file_init(select_path);
            }
            
            //提示未发现文件
            gui_dialog_msg(DIALOG_MSG, DIALOG_INFOR_WAIT, S_NO_SONG);
            init_result = FALSE;
        }
    }
    return init_result;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e list_scene_favor1play_sure(void *param)
 * \第一个收藏夹选择play确认
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \ingroup      music_listmenu_optionitem.c
 * \note
 */
/*******************************************************************************/
app_result_e list_scene_favor1play_sure(void *param)
{
    bool bret;
    bret = list_scene_favorsel(&g_browser_filepath, FSEL_TYPE_LISTFAVOR1);
    if (bret == TRUE)
    {
        return RESULT_PLAYING_START;
    }
    else
    {
        return RESULT_REDRAW;
    }
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e list_scene_favor2play_sure(void *param)
 * \第二个收藏夹选择play确认
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \ingroup      music_listmenu_optionitem.c
 * \note
 */
/*******************************************************************************/
app_result_e list_scene_favor2play_sure(void *param)
{
    bool bret;
    bret = list_scene_favorsel(&g_browser_filepath, FSEL_TYPE_LISTFAVOR2);
    if (bret == TRUE)
    {
        return RESULT_PLAYING_START;
    }
    else
    {
        return RESULT_REDRAW;
    }
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e list_scene_favor3play_sure(void *param)
 * \第三个收藏夹选择play确认
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \ingroup      music_listmenu_optionitem.c
 * \note
 */
/*******************************************************************************/
app_result_e list_scene_favor3play_sure(void *param)
{
    bool bret;
    bret = list_scene_favorsel(&g_browser_filepath, FSEL_TYPE_LISTFAVOR3);
    if (bret == TRUE)
    {
        return RESULT_PLAYING_START;
    }
    else
    {
        return RESULT_REDRAW;
    }
}
