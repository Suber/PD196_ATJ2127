/*******************************************************************************
 *                              US212A
 *                            Module: music ui
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *     fiona.yang   2011-09-26          1.0              create this file
 *******************************************************************************/

#include "app_music.h"

#include "app_music_favorite.h"

#include "app_music_menu_config.h"

//入口菜单组
const uint8 entry_id1[] =
{
    OPTION_FAVORLIST1_NORMAL,
    OPTION_FAVORLIST1_MPLAYING,
    OPTION_FAVORLIST1_MPAUSE,
    OPTION_FAVORLIST1_RPLAYING,
    OPTION_FAVORLIST1_RPAUSE
};

const uint8 entry_id2[] =
{
    OPTION_FAVORLIST2_NORMAL,
    OPTION_FAVORLIST2_MPLAYING,
    OPTION_FAVORLIST2_MPAUSE,
    OPTION_FAVORLIST2_RPLAYING,
    OPTION_FAVORLIST2_RPAUSE
};

const uint8 entry_id3[] =
{
    OPTION_FAVORLIST3_NORMAL,
    OPTION_FAVORLIST3_MPLAYING,
    OPTION_FAVORLIST3_MPAUSE,
    OPTION_FAVORLIST3_RPLAYING,
    OPTION_FAVORLIST3_RPAUSE
};

/******************************************************************************/
/*!
 * \par  Description:
 *	  music 选择相应的列表或歌曲播放
 * \param[in]   location 文件路径
 * \param[in]   list_no  激活项序号 
 * \param[out]  none
 * \return      app_result_e
 * \retval
 * \ingroup     music_ui
 * \note
 * \li   如果序号小于等于0，表示为ui_direcotry()控件嵌套菜单项，例如allsong的所有歌曲
 *******************************************************************************/
static app_result_e _music_listmenu_select_file(file_path_info_t* location, uint16 list_no)
{
    bool ret_val; 

    app_result_e ui_result;  
    
    plist_location_t* plist_locatp;

    plist_locatp = &(location->file_path.plist_location);

    //选择相应列表或文件
    ret_val = fsel_browser_select(list_no);

    if(ret_val == TRUE)
    {
        ret_val = fsel_browser_get_location(plist_locatp, location->file_source);
        if(ret_val == TRUE)
        {
            ui_result = RESULT_PLAYING_START;
        }
        else
        {
            //提示未发现文件
            gui_dialog_msg(DIALOG_MSG, DIALOG_INFOR_WAIT, S_NO_SONG);        
            ui_result = RESULT_REDRAW;
        }
    }
    else
    {
        //提示未发现文件
        gui_dialog_msg(DIALOG_MSG, DIALOG_INFOR_WAIT, S_NO_SONG);
        ui_result = RESULT_REDRAW;        
    }

    return ui_result;
    
}

/******************************************************************************/
/*!
 * \par  Description:
 *	  music 选择相应的列表或歌曲
 * \param[in]   location 文件路径
 * \param[in]   active   激活项序号 
 * \param[out]  none
 * \return      app_result_e
 * \retval
 * \ingroup     music_ui
 * \note
 * \li   如果序号小于等于0，表示为ui_direcotry()控件嵌套菜单项，例如allsong的所有歌曲
 *******************************************************************************/
static app_result_e _music_listmenu_play_file(file_path_info_t* location, uint16 active)
{   
    //判断层次
    dir_brow_t browser;


    browser.name_buf = NULL;
    browser.name_len = 0;
        
    fsel_browser_enter_dir(CUR_DIR, 0, &browser); 

    if((active == 0) && (browser.dir_total != 0))
    {
        //选择所有相同类型的表项播放
        active = 0xffff;
    }
    else
    {
        if(browser.dir_total != 0)
        {
             //当前的下一级    
        }
        else
        {
            if (active == 0)
            {
                active = 1;
            }          
        }
    }
    
    return _music_listmenu_select_file(location, active);
}
/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e playlist_option_result_proc(void * location, app_result_e check_type, uint8 active)
 *	  music playlist的结果处理函数
 * \param[in]
 * \param[out]
 * \return
 * \retval
 * \ingroup     music_ui
 * \note
 * \philips播放列表层级为:
 * \All song 一级
 * \Artist:  artist + album + all song
 * \album：  album + all song
 * \genre:   genre + album + all song
 *******************************************************************************/

app_result_e playlist_option_result_proc(file_path_info_t* location, app_result_e check_type, uint16 active)
{
    app_result_e result;

    plist_location_t* plist_locatp;

    plist_locatp = &(location->file_path.plist_location);
    switch (check_type)
    {
        //播放
        case RESULT_PLAYING_START:
        result = _music_listmenu_play_file(location, active);
        break;

        case RESULT_ADD_FAVOR1:
        //添加收藏夹1
        result = add_favorite_listmenu(plist_locatp, FSEL_TYPE_LISTFAVOR1, active);
        break;
        case RESULT_ADD_FAVOR2:
        //添加收藏夹2
        result = add_favorite_listmenu(plist_locatp, FSEL_TYPE_LISTFAVOR2, active);
        break;
        case RESULT_ADD_FAVOR3:
        //添加收藏夹3
        result = add_favorite_listmenu(plist_locatp, FSEL_TYPE_LISTFAVOR3, active);
        break;
        default:
        result = check_type;
        break;
    }
    return result;
}

/******************************************************************************/
/*!
 * \par  Description:
 *	  收藏夹列表1,2,3 gui_directory()回调函数的返回值处理函数
 * \param[in]
 * \param[out]
 * \return
 * \retval
 * \ingroup     music_ui
 * \note
 *******************************************************************************/
app_result_e favorlist_option_result_proc(uint8 file_source, app_result_e check_type, uint16 index)
{
    app_result_e result = RESULT_IGNORE;
    switch (check_type)
    {
        case RESULT_PLAYING_START:
        //播放某个收藏夹列表某个文件
        //fsel_get_location((void *) &g_browser_filepath.file_path.flist_location, file_source);
        //g_browser_filepath.file_source = file_source;
        //cpy到播放使用的全局变量中
        //libc_memcpy(&g_file_path_info, &g_browser_filepath, sizeof(file_path_info_t));
        fsel_browser_select(index);
        fsel_browser_get_location(&g_browser_filepath.file_path.flist_location, file_source);
        result = check_type;
        break;
        case RESULT_DEL_FAVOR:
        //删除收藏夹文件
        result = del_favorite_listmenu(file_source, index);
        break;
        case RESULT_CLR_FAVOR:
        //清空收藏夹文件
        result = clr_favorite_menu(file_source);
        break;
        default:
        result = check_type;
        break;
    }
    return result;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e list_scene_list_option(void)
 * \列表的option函数
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \ingroup      music_listmenu_listoption.c
 * \note
 */
/*******************************************************************************/
app_result_e list_scene_list_option(void)
{
    menu_com_data_t menu;
    app_result_e result_value;
    engine_type_e engine_type;
    engine_state_e engine_state;
    //for temp complie
    //bool sound_on = TRUE;

    menu.app_id = APP_ID_MUSIC;
    menu.menu_func = NULL;
    //引擎是否存在/是music or fm
    engine_type = get_engine_type();
    engine_state = get_engine_state();
    if (engine_type == ENGINE_MUSIC)
    {
        //正在播放
        if (engine_state == ENGINE_STATE_PLAYING)
        {
            //进入入口菜单m playnow & play & add to favorite
            menu.menu_entry = OPTION_PLIST_MPLAING;
        }
        else
        {
            //进入入口菜单n lastplayed & paly & add to favorite
            menu.menu_entry = OPTION_PLIST_MPAUSE;
        }
    }
    else if (engine_type == ENGINE_RADIO)
    {
        //判断是否静音
        if (engine_state == ENGINE_STATE_PLAYING)
        {
            //进入入口菜单j playnow & play & add to favorite
            menu.menu_entry = OPTION_PLIST_RPLAYING;
        }
        else
        {
            //进入入口菜单k lastplayed & paly & add to favorite
            menu.menu_entry = OPTION_PLIST_RPAUSE;
        }
    }
    else
    {
        //进入入口菜单 i paly & add to favorite
        menu.menu_entry = OPTION_PLIST_NORMAL;
    }
    //从跟菜单进入
    menu.browse_mode = 0;
    menu.path_id = MUSIC_PATH_ID_LIST_PLAYLIST;
    menu.menu_func = NULL; //有什么用
    result_value = gui_menulist(MENULIST_OPTION, &menu);
    return result_value;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e list_scene_favorlist_option(void)
 * \列表的option函数
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \ingroup      music_listmenu_listoption.c
 * \note
 */
/*******************************************************************************/
app_result_e list_scene_favorlist_check(const uint8* entryid_buf)
{
    menu_com_data_t menu;
    app_result_e result_value;
    engine_type_e engine_type;
    engine_state_e engine_state;

    //for temp complie

    menu.app_id = APP_ID_MUSIC;
    menu.menu_func = NULL;

    //引擎是否存在/是music or fm
    engine_type = get_engine_type();
    engine_state = get_engine_state();

    if (engine_type == ENGINE_MUSIC)
    {
        //正在播放
        if (engine_state == ENGINE_STATE_PLAYING)
        {
            //进入入口菜单entryid[1] playnow & play & del form favorite & clear favorite
            menu.menu_entry = entryid_buf[1];
        }
        else
        {
            //进入入口菜单entryid[2] lastplayed & paly & del form favorite & clear favorite
            menu.menu_entry = entryid_buf[2];
        }
    }
    else if (engine_type == ENGINE_RADIO)
    {
        //判断是否静音
        if (engine_state == ENGINE_STATE_PLAYING)
        {
            //进入入口菜单entryid[3] playnow & play & del form favorite & clear favorite
            menu.menu_entry = entryid_buf[3];
        }
        else
        {
            //进入入口菜单entryid[4] lastplayed & paly & del form favorite & clear favorite
            menu.menu_entry = entryid_buf[4];
        }
    }
    else
    {
        //进入入口菜单 entryid[0] paly & & del form favorite & clear favorite
        menu.menu_entry = entryid_buf[0];
    }
    //从跟菜单进入
    menu.browse_mode = 0;
    menu.path_id = MUSIC_PATH_ID_LIST_FAVORLIST;
    menu.menu_func = NULL; //有什么用
    result_value = gui_menulist(MENULIST_OPTION, &menu);
    return result_value;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e list_scene_favorlist1_option(void)
 * \收藏夹1列表的option函数
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \ingroup      music_listmenu_listoption.c
 * \note
 */
/*******************************************************************************/
app_result_e list_scene_favorlist1_option(void)
{
    return list_scene_favorlist_check(entry_id1);
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e list_scene_favorlist2_option(void)
 * \收藏夹2列表的option函数
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \ingroup      music_listmenu_listoption.c
 * \note
 */
/*******************************************************************************/
app_result_e list_scene_favorlist2_option(void)
{
    return list_scene_favorlist_check(entry_id2);
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e list_scene_favorlist3_option(void)
 * \收藏夹3列表的option函数
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \ingroup      music_listmenu_listoption.c
 * \note
 */
/*******************************************************************************/
app_result_e list_scene_favorlist3_option(void)
{
    return list_scene_favorlist_check(entry_id3);
}
/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e list_scene_m3ulist_option(void)
 * \m3u列表的option函数
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \ingroup      music_listmenu_listoption.c
 * \note
 */
/*******************************************************************************/
app_result_e list_scene_m3ulist_option(void)
{
    menu_com_data_t menu;
    app_result_e result_value;
    engine_type_e engine_type;
    engine_state_e engine_state;

    menu.app_id = APP_ID_MUSIC;
    menu.menu_func = NULL;
    //引擎是否存在/是music or fm
    engine_type = get_engine_type();
    engine_state = get_engine_state();
    if (engine_type == ENGINE_MUSIC)
    {
        //正在播放
        if (engine_state == ENGINE_STATE_PLAYING)
        {
            //进入入口菜单m playnow & play 
            menu.menu_entry = OPTION_M3ULIST_MPLAYING;
        }
        else
        {
            //进入入口菜单n lastplayed & paly 
            menu.menu_entry = OPTION_M3ULIST_MPAUSE;
        }
    }
    else if (engine_type == ENGINE_RADIO)
    {
        //判断是否静音
        if (engine_state == ENGINE_STATE_PLAYING)
        {
            //进入入口菜单j playnow & play 
            menu.menu_entry = OPTION_M3ULIST_RPLAYING;
        }
        else
        {
            //进入入口菜单k lastplayed & paly
            menu.menu_entry = OPTION_M3ULIST_RPAUSE;
        }
    }
    else
    {
        //进入入口菜单 i paly 
        menu.menu_entry = OPTION_M3ULIST_NORMAL;
    }
    //从跟菜单进入
    menu.browse_mode = 0;
    menu.path_id = MUSIC_PATH_ID_LIST_M3ULIST;
    menu.menu_func = NULL; //有什么用
    result_value = gui_menulist(MENULIST_OPTION, &menu);
    return result_value;            
}
/******************************************************************************/
/*!
 * \par  Description:
 * \m3u列表的option回调函数结果处理
 * \param[in]    location 文件路径
 * \param[in]    check_type 叶子菜单返回值 
 * \param[in]    active     激活项序号
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \ingroup      music_listmenu_listoption.c
 * \note
 */
/*******************************************************************************/
app_result_e m3ulist_option_result_proc(file_path_info_t* location, app_result_e check_type, uint16 active)
{
    app_result_e result;     
    int16 real_active = (int16)active;

    plist_location_t* plist_locatp;

    plist_locatp = &(location->file_path.plist_location);

    if(check_type == RESULT_PLAYING_START)
    {   
        if(real_active > 0)
        {
            //统计当前文件总数
            fsel_browser_enter_dir(CUR_DIR, 0, NULL);
            
            result =  _music_listmenu_select_file(location, active);
        }
        else if(real_active == 0)
        {
            result = list_scene_favor3play_sure(NULL);
        }
        else if(real_active == -1)
        {
            result = list_scene_favor2play_sure(NULL);
        }
        else
        {
            result = list_scene_favor1play_sure(NULL);
        }
    }
    else
    {
        result = check_type;
    }

    return result;
}
/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e list_scene_list_option_calback(fsel_type_e fsorce)
 * \列表的option回调函数
 * \param[in]    fsorce文件来源  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \ingroup      music_listmenu_listoption.c
 * \note
 */
/*******************************************************************************/
app_result_e list_scene_list_option_callback(file_path_info_t *path_info, uint16 active)
{
    app_result_e result = RESULT_IGNORE;

    //取列表类型
    fsel_type_e file_source = path_info->file_source;

    switch(file_source)
    {
        case FSEL_TYPE_PLAYLIST:   
        //播放列表
        result = list_scene_list_option();
        result = playlist_option_result_proc(path_info, result, active);
        break;

        case FSEL_TYPE_LISTAUDIBLE:
        //audible列表 只存在上一次播放/正在播放/提示语 因此直接返回处理结果
        result = list_scene_menu_option();
        if (result == RESULT_MENU_REDRAW)
        {
            result = RESULT_REDRAW;
        }
        break;

        case FSEL_TYPE_LISTFAVOR1:
        result = list_scene_favorlist1_option();
        //对各级收藏夹列表返回值的处理函数
        result = favorlist_option_result_proc(file_source, result, active);
        break;
        
        case FSEL_TYPE_LISTFAVOR2:
        result = list_scene_favorlist2_option();
        //对各级收藏夹列表返回值的处理函数
        result = favorlist_option_result_proc(file_source, result, active);
        break;

        case FSEL_TYPE_LISTFAVOR3:
        result = list_scene_favorlist3_option();
        //对各级收藏夹列表返回值的处理函数
        result = favorlist_option_result_proc(file_source, result, active);
        break;

        case FSEL_TYPE_M3ULIST:
        result = list_scene_m3ulist_option();
        result = m3ulist_option_result_proc(path_info, result, active);
        break;

        default:
        break;
    }
    return result;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e list_scene_listplay_sure(void *param)
 * \列表播放选项的确认函数
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \ingroup      music_listmenu_listoption.c
 * \note
 */
/*******************************************************************************/
app_result_e list_scene_listplay_sure(void *param)
{
    //返回选择文件播放
    return RESULT_PLAYING_START;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e list_scene_addfavor_sure(void *param)
 * \添加到收藏夹的确认函数
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \ingroup      music_listmenu_listoption.c
 * \note
 */
/*******************************************************************************/
app_result_e list_scene_addfavor_sure(void *param)
{
    //返回参数
    menu_title_action_t* paramptr = (menu_title_action_t*) param;

    paramptr->style_id = MENULIST_TITLE;

    paramptr->title_id = S_ADD_TO_WHICH;
    //进入下一级菜单
    return RESULT_MENU_SON;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e list_scene_addfavor1_sure(void *param)
 * \添加到收藏夹1的确认函数
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \ingroup      music_listmenu_listoption.c
 * \note
 */
/*******************************************************************************/
app_result_e list_scene_addfavor1_sure(void *param)
{
    return RESULT_ADD_FAVOR1;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e list_scene_addfavor2_sure(void)
 * \添加到收藏夹2的确认函数
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \ingroup      music_listmenu_listoption.c
 * \note
 */
/*******************************************************************************/
app_result_e list_scene_addfavor2_sure(void *param)
{
    return RESULT_ADD_FAVOR2;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e list_scene_addfavor3_sure(void *param)
 * \添加到收藏夹3的确认函数
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \ingroup      music_listmenu_listoption.c
 * \note
 */
/*******************************************************************************/
app_result_e list_scene_addfavor3_sure(void *param)
{
    return RESULT_ADD_FAVOR3;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e list_scene_delfavor_sure(void *param)
 * \从收藏夹删除的确认函数(已经定位到某个收藏夹)
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \ingroup      music_listmenu_listoption.c
 * \note
 */
/*******************************************************************************/
app_result_e list_scene_delfavor_sure(void *param)
{
    return RESULT_DEL_FAVOR;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e list_scene_clrfavor_sure(void *param)
 * \清除收藏夹的确认函数(已经定位到某个收藏夹)
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \ingroup      music_listmenu_listoption.c
 * \note
 */
/*******************************************************************************/
app_result_e list_scene_clrfavor_sure(void *param)
{
    return RESULT_CLR_FAVOR;
}
