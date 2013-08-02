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

#include "app_music_menu_config.h"

const uint8 audible_ext_name[][4] =
{ "AAX", "AA " };

const id3_type_e audible_id3_type[] =
{
    //AAX
    ID3_TYPE_AAX,
    //AA
    ID3_TYPE_AA
};
//bank data for audible resume play
audible_resume_info_t audible_resume_data _BANK_DATA_ATTR_;

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e list_scene_allsong_sure(void* param)
 * \所有歌曲确认函数
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \ingroup      music_listmenu_menusure.c
 * \note
 */
/*******************************************************************************/
app_result_e list_scene_allsong_sure(void* param)
{
    //返回到file list
    return RESULT_ENTER_TITLE_LIST;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e list_scene_artist_sure(void* param)
 * \歌手确认函数
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \ingroup      music_listmenu_menusure.c
 * \note
 */
/*******************************************************************************/
app_result_e list_scene_artist_sure(void* param)
{
    //返回到file list
    return RESULT_ENTER_ARTIST_LIST;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e list_scene_album_sure(void* param)
 * \专辑确认函数
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \ingroup      music_listmenu_menusure.c
 * \note
 */
/*******************************************************************************/
app_result_e list_scene_album_sure(void* param)
{
    //返回到file list
    return RESULT_ENTER_ALBUM_LIST;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e list_scene_genre_sure(void)
 * \流派确认函数
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \ingroup      music_listmenu_menusure.c
 * \note
 */
/*******************************************************************************/
app_result_e list_scene_genre_sure(void* param)
{
    //返回到file list
    return RESULT_ENTER_GENRE_LIST;
}

/******************************************************************************/
/*!
 * \par  Description:
 进入专辑图片列表确认执行函数
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval       RESULT_ENTER_ALBUM_THUMBNAIL
 * \ingroup      music_listmenu_menusure.c
 * \note
 */
/*******************************************************************************/

app_result_e list_scene_albumlist_sure(void *param)
{
    return RESULT_ENTER_ALBUM_THUMBNAIL;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool list_scene_favor_total(file_path_info_t* select_path fsel_type_e favor_from)
 * \检查收藏夹是否存在文件
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval       bool the result
 * \retval          1 success
 * \retval          0 fail
 * \ingroup      music_listmenu_menusure.c
 * \note
 */
/*******************************************************************************/
bool list_scene_favor_total(file_path_info_t* select_path, fsel_type_e favor_from)
{
    bool retval;

    uint8 cur_sel_save;
    uint16 cur_list_save;

    dir_brow_t browser_plist;

    //不获取文件名
    browser_plist.name_len = 0;

    cur_sel_save = select_path->file_source;

    cur_list_save = select_path->file_path.plist_location.list_type;

    //退出文件选择器
    fsel_exit();

    //文件选择器初始化
    select_path->file_source = favor_from;

    retval = music_file_init(select_path);

    if (retval == TRUE)
    {
        fsel_browser_set_location(&select_path->file_path.flist_location, favor_from);

        //获取当前收藏夹文件总数
        fsel_browser_enter_dir(CUR_DIR, 0, &browser_plist);

        if (browser_plist.file_total == 0)
        {
            fsel_exit();

            select_path->file_source = cur_sel_save;
            select_path->file_path.plist_location.list_type = cur_list_save;

            music_file_init(select_path);

            //提示未发现文件
            gui_dialog_msg(DIALOG_MSG, DIALOG_INFOR_WAIT, S_NO_SONG);
            retval = FALSE;
        }
    }
    return retval;
}
/******************************************************************************/
/*!
 * \par  Description:
 进入收藏夹叶子菜单回调函数
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \ingroup      music_listmenu_menusure.c
 * \note
 */
/*******************************************************************************/
app_result_e list_scene_favor_sure(void *param)
{
    //返回filelist
    return RESULT_ENTER_FAVOR_LIST;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e list_scene_favor1_sure(void)
 * \收藏夹1进入确认函数
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \ingroup      music_listmenu_menusure.c
 * \note
 */
/*******************************************************************************/
app_result_e list_scene_favor1_sure(void* param)
{
    bool bret;
    //check收藏夹是否存在文件
    bret = list_scene_favor_total(&g_browser_filepath, FSEL_TYPE_LISTFAVOR1);
    if (bret == TRUE)
    {
        //返回到file list
        return RESULT_ENTER_FAVOR1_LIST;
    }
    else
    {
        //当前菜单
        return RESULT_REDRAW;
    }
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e list_scene_favor2_sure(void)
 * \收藏夹2进入确认函数
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \ingroup      music_listmenu_menusure.c
 * \note
 */
/*******************************************************************************/
app_result_e list_scene_favor2_sure(void* param)
{
    bool bret;
    //check收藏夹是否存在文件
    bret = list_scene_favor_total(&g_browser_filepath, FSEL_TYPE_LISTFAVOR2);
    if (bret == TRUE)
    {
        //返回到file list
        return RESULT_ENTER_FAVOR2_LIST;
    }
    else
    {
        //当前菜单
        return RESULT_REDRAW;
    }
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e list_scene_favor3_sure(void)
 * \收藏夹3进入确认函数
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \ingroup      music_listmenu_menusure.c
 * \note
 */
/*******************************************************************************/
app_result_e list_scene_favor3_sure(void* param)
{
    bool bret;
    //check收藏夹是否存在文件
    bret = list_scene_favor_total(&g_browser_filepath, FSEL_TYPE_LISTFAVOR3);
    if (bret == TRUE)
    {
        //返回到file list
        return RESULT_ENTER_FAVOR3_LIST;
    }
    else
    {
        //当前菜单
        return RESULT_REDRAW;
    }
}

/******************************************************************************/
/*!
 * \par  Description:
 * \获取ID3信息 获取文件名字段
 * \param[in]  buffer 存放id3的buffer param1
 * \param[in]  ext_name 该文件的后缀名 param2
 * \param[out]
 * \return
 * \retval
 * \ingroup     music_add_favorite.c
 * \note
 *******************************************************************************/
void get_audible_id3_title(uint8 *buffer, uint8 *ext_name)
{
    id3_info_t id3_infor;
    id3_type_e cur_type;
    uint8 i;
    libc_memset(&id3_infor, 0, sizeof(id3_info_t));
    id3_infor.tit2_buffer = buffer;
    id3_infor.tit2_length = 32;

    //cur_type = check_music_type(ext_name);
    for (i = 0; i < 2; i++)
    {
        if (libc_strncmp(&audible_ext_name[i], ext_name, 4) == 0)
        {
            break;
        }
    }
    if (i < 2)
    {
        cur_type = audible_id3_type[i];
        get_id3_info(&id3_infor, NULL, cur_type);
    }
    return;
}

/******************************************************************************/
/*!
 * \par  Description:
 获取当前的audible播放断点
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      music_listmenu_menusure.c
 * \note
 * \li   该函数只是获取当前audible的播放断点，并不存储断点。断点的存储由music engine完成
         由于audible_resume_data为bank data，因此在根菜单和叶子菜单都需要读取
 * \li   引擎对断点的保存存在两种情形:从audible切换到music，或者music engine退出，这样可以
         减少VRAM的读写次数
 */
/*******************************************************************************/
void load_breakinfo(void)
{
    if ((g_audible_now == FALSE) && (g_music_config.audible_bk_flag == TRUE))
    {
        //读取audible的断点信息
        sys_vm_read(&audible_resume_data, VM_AP_MUSIC_AUDBK, sizeof(audible_resume_info_t));
        if (audible_resume_data.magic != MAGIC_AUDBK)
        {
            //断点无效
            g_music_config.audible_bk_flag = FALSE;
        }
    }
    else
    {
        if (g_audible_now == TRUE)
        {
            //获取断点信息
            music_get_bkinfo(&g_audible_bkinfo);

            //获取当前播放文件的路径信息
            music_get_filepath(&g_file_path_info);

            libc_memcpy(&(audible_resume_data.bk_infor), &g_audible_bkinfo, sizeof(g_audible_bkinfo));
            libc_memcpy(&(audible_resume_data.locat_info), &g_file_path_info, sizeof(file_path_info_t));
        }
    }
    return;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e list_scene_audible_sure(void *param)
 * \语音书籍确认函数
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \ingroup      music_listmenu_menusure.c
 * \note
 */
/*******************************************************************************/
app_result_e list_scene_audible_sure(void *param)
{
    file_path_info_t temp_file_path;

    plist_location_t* locat_ptr;

    //获取当前播放文件的路径信息
    locat_ptr = &temp_file_path.file_path.plist_location;

    load_breakinfo();

    //是否曾经播放过audible歌曲
    if (g_music_config.audible_bk_flag == TRUE)
    {
        //调用动态入口菜单 续播/书籍/作者
        string_desc_t aud_resume_desc;//目的字符串结构
        string_desc_t aud_resume_name;//源字符串结构
        //返回参数
        menu_title_action_t* retptr = (menu_title_action_t*) param;

        libc_memcpy(&temp_file_path, &audible_resume_data.locat_info, sizeof(file_path_info_t));

        if (temp_file_path.file_path.plist_location.disk != locat_ptr->disk)
        {
            //二者盘符不一致，则直接进入书籍和作者列表
            return RESULT_MENU_SON;
        }

        //定位到文件
        if (fsel_browser_set_file(locat_ptr, temp_file_path.file_source) == TRUE)
        {
            get_audible_id3_title(g_song_filename, ((plist_location_t*) locat_ptr)->filename);
        }
        else
        {
            //文件无法找到，证明该断点无效 清断点标志
            g_music_config.audible_bk_flag = FALSE;
            return RESULT_MENU_SON;
        }

        //另外一个入口菜单styid
        retptr->menu_entry = SLEEP_TIMER_SETMENU;

        //aud_resume_desc.data.str = g_insert_dialog;
        //aud_resume_desc.length = 80;
        aud_resume_desc.data.str = g_temp_buffer_p;
        aud_resume_desc.length = 128;       
        aud_resume_desc.argv = S_PLAY_RESUME;
        //菜单项组织的字符串当前只支持内码显示
        aud_resume_desc.language = (int8) g_setting_comval.g_comval.language_id;
        //aud_resume_desc.language = UNICODEDATA;
        aud_resume_name.data.str = g_song_filename;
        aud_resume_name.length = 32;
        if (*(uint16*) g_song_filename == 0xfeff)
        {
            aud_resume_name.language = UNICODEDATA;
        }
        else if ((g_song_filename[0] == 0xef) && (g_song_filename[1] == 0xbb) && (g_song_filename[2] == 0xbf))
        {
            aud_resume_name.language = UTF_8DATA;
        }
        else
        {
            aud_resume_name.language = (int8) g_setting_comval.g_comval.language_id;
        }

        com_string_format(&aud_resume_desc, &aud_resume_name);

        retptr->menu_entry = LISTMENU_AUDIBLE_RESUME;//动态菜单入口ID
        retptr->str_id = -1;//激活项多国语言字符串ID，-1表示采用默认激活项
        //retptr->ret_str = g_insert_dialog;//动态菜单项字符串指针
        retptr->ret_str = g_temp_buffer_p;//动态菜单项字符串指针
        retptr->style_id = -1;//本级菜单style_id，可以更换ui_menulist传进来的默认style_id，-1表示沿用默认style_id
        retptr->source_id = S_PLAY_RESUME;
    }
    //进入下一级
    return RESULT_MENU_SON;

}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e list_scene_audresume_sure(void *param)
 * \audible续播
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \ingroup      music_listmenu_menusure.c
 * \note
 */
/*******************************************************************************/
app_result_e list_scene_audresume_sure(void *param)
{
    bool ret_val = TRUE;
    fsel_type_e cur_sel_save = g_file_path_info.file_source;

    //读取audible的断点信息
    //sys_vm_read(&audible_resume_data, VM_AP_MUSIC_AUDBK, sizeof(audible_resume_info_t));
    load_breakinfo();

    libc_memcpy(&g_file_path_info, &audible_resume_data.locat_info, sizeof(file_path_info_t));

    //断点信息
    libc_memcpy(&g_audible_bkinfo, &(audible_resume_data.bk_infor), sizeof(g_audible_bkinfo));

    if (g_file_path_info.file_source != cur_sel_save)
    {
        fsel_exit();
        ret_val = music_file_init(&g_file_path_info);
    }

    if (ret_val == TRUE)
    {
        //返回到audible续播
        return RESULT_AUDIBLE_RESUME;
    }
    else
    {
        //初始化文件选择器失败
        return RESULT_LISTMENU_MENU;
    }
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e list_scene_books_sure(void *param)
 * \audible的书籍列表确认函数
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \ingroup      music_listmenu_menusure.c
 * \note
 */
/*******************************************************************************/
app_result_e list_scene_books_sure(void *param)
{
    //返回到file list
    return RESULT_ENTER_BOOKS_LIST;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e list_scene_authors_sure(void)
 * \audible的作者列表确认函数
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \ingroup      music_listmenu_menusure.c
 * \note
 */
/*******************************************************************************/
app_result_e list_scene_authors_sure(void *param)
{
    //返回到file list
    return RESULT_ENTER_AUTHORS_LIST;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e list_scene_create_sure(void)
 * \创建播放列表确认函数
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \ingroup      music_listmenu_menusure.c
 * \note
 */
/*******************************************************************************/
app_result_e list_scene_create_sure(void *param)
{
    //进入创建播放列表app
    return RESULT_CREATE_LIST_APP;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool list_scene_check_change(uint8 disk_flag)
 * \进入主盘目录确认函数
 * \param[in]    disk_flag 当前盘符  para1
 * \param[out]   none
 * \return       bool the result
 * \retval       TRUE 需要更换盘且更换成功
 * \retval       FALSE 不需要更换盘
 * \ingroup      music_listmenu_menusure.c
 * \note
 */
/*******************************************************************************/
app_result_e list_scene_check_change(uint8 disk_flag)
{
    uint8 cur_disk;
    //fsel_error_e get_erro;
    bool retval = TRUE;

    music_get_filepath(&g_file_path_info);
    cur_disk = g_file_path_info.file_path.dirlocation.disk;
    if (cur_disk != disk_flag)
    {
        //是否后台存在music
        engine_type_e engine_type = get_engine_type();

        if (engine_type == ENGINE_MUSIC)
        {
            sys_vm_write(&g_setting_comval, VM_AP_SETTING);
            //引擎关闭
            music_close_engine();
        }

        //退出文件选择器
        fsel_exit();

        g_file_path_info.file_path.dirlocation.disk = disk_flag;
        g_browser_filepath.file_path.plist_location.disk = disk_flag;
        retval = music_file_init(&g_file_path_info);
        //切换失败,回到原盘
        if (retval == FALSE)
        {
            //退出文件选择器
            fsel_exit();
            //切换回主盘
            g_file_path_info.file_path.dirlocation.disk = cur_disk;
            g_browser_filepath.file_path.plist_location.disk = cur_disk;
            music_file_init(&g_file_path_info);
        }
    }

    if (retval == TRUE)
    {
        return RESULT_ENTER_TITLE_LIST;
    }
    else
    {
#if 0
        get_erro = fsel_get_error();
        //提示
        if ((get_erro == FSEL_ERR_STG) || (get_erro == FSEL_ERR_FS))
        {
            gui_dialog_msg(DIALOG_MSG, DIALOG_INFOR_WAIT, S_DISK_ERROR);
        }
        else if(get_erro == FSEL_ERR_LIST_NOEXIST)
        {
            gui_dialog_msg(DIALOG_MSG, DIALOG_INFOR_WAIT, S_NO_PLAYLIST);
        }
#endif
        return RESULT_REDRAW;
    }
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e list_scene_maindisk_sure(void)
 * \进入主盘目录确认函数
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \ingroup      music_listmenu_menusure.c
 * \note
 */
/*******************************************************************************/
app_result_e list_scene_maindisk_sure(void *param)
{
    app_result_e change_ret;
    change_ret = list_scene_check_change(DISK_C);
    return change_ret;

}

static app_result_e _ask_for_change_disk(void)
{
    app_result_e result;
    dialog_com_data_t ask_dialog_data;
    string_desc_t ask_create_str;

    uint8 old_disk = g_browser_filepath.file_path.plist_location.disk;

    //为了响应卡拔出消息，先临时更改盘符
    g_browser_filepath.file_path.plist_location.disk = DISK_H;

    //对话框是否创建播放列表
    ask_create_str.data.id = S_IS_CREATE_PLAYLIST;
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

    //恢复之前的盘符
    g_browser_filepath.file_path.plist_location.disk = old_disk;

    return result;

}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e list_scene_carddisk_sure(void)
 * \进入卡盘目录确认函数
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \ingroup      music_listmenu_menusure.c
 * \note
 */
/*******************************************************************************/
app_result_e list_scene_carddisk_sure(void *param)
{
    //bool ret;
    app_result_e result;

    //首先检查卡是否存在
    if (sys_detect_disk(DRV_GROUP_STG_CARD) != -1)
    {
        //card还没有更新播放列表
        if (get_card_plist() == FALSE)
        {
            //询问是否创建播放列表
            result = _ask_for_change_disk();
            if (result == RESULT_DIALOG_YES)
            {
                //确认的情况，以后不再创建播放列表
                //只更改盘符，不重新初始化文件选择器
                //以传递给plist建表
                g_browser_filepath.file_path.plist_location.disk = DISK_H;
                g_file_path_info.file_path.plist_location.disk = DISK_H;
                result = RESULT_CREATE_LIST_APP;
            }
            else if (result == RESULT_DIALOG_NO)
            {
                result = list_scene_check_change(DISK_H);
            }
            else
            {
                ;//nothing 返回热键消息
            }
        }
        else
        {
            result = list_scene_check_change(DISK_H);
        }
    }
    else
    {
        //提示语
        gui_dialog_msg(DIALOG_MSG, DIALOG_INFOR_WAIT, S_CARD_OUT);
        //重新进入menu菜单
        result = RESULT_CARD_OUT_ERROR;
    }
    return result;
}


