/*******************************************************************************
 *                              US212A
 *                            Module: music ui
 *                 Copyright(c) 2003-2012 Actions Semiconductor
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *     fiona.yang   2011-09-15          1.0              create this file
 *******************************************************************************/

#include "app_music.h"

#include "app_music_playing.h"

uint8 convert_buf[Id3BufSIZE * 2] _BANK_DATA_ATTR_;

//定义const data 所有后缀数据
const uint8 support_ext[MusicExtNum][4] =
{
    "MP1", //mp3
    "MP2", //mp3
    "MP3", //mp3
    "WMA", //wma
    "APE", //ape
    "FLA", //flac
    "OGG", //ogg
    "AAC", //aac
    "AAX", //aax
    "M4A", //m4a
    "AA " //aa
};

const id3_type_e support_id3_type[] =
{
    ID3_TYPE_MP3, //mp1
    ID3_TYPE_MP3, //mp2
    ID3_TYPE_MP3, //mp3
    ID3_TYPE_WMA, //wma
    ID3_TYPE_APE, //ape
    ID3_TYPE_FLAC, //fla
    ID3_TYPE_OGG, //ogg
    ID3_TYPE_AAC, //aac
    ID3_TYPE_AAX, //aax
    ID3_TYPE_AAC, //m4a
    ID3_TYPE_AA //aa
};


/******************************************************************************/
/*!
 * \par  Description:
 * \bool music_get_playinfo(mengine_playinfo_t* play_info)
 * \获取当前播放的时间对应的专辑图片
 * \param[in]    play_info  para1
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      music_playing_getid3.c
 * \note
 */
/*******************************************************************************/
bool music_get_audible_image(mengine_playinfo_t* play_info)
{
    bool bret = TRUE;
    msg_apps_t msg;
    //返回消息内容存放变量
    msg_reply_t reply;
    //获取当前audible的图片信息
    reply.content = play_info;
    //消息类型(即消息名称)
    msg.type = MSG_MENGINE_AUDIBLE_GETIMAGE_SYNC;
    //发送同步消息
    bret = send_sync_msg(APP_ID_MENGINE, &msg, &reply, 0);
    return bret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \id3_type_e play_scene_checktype(char * extname)
 * \获取后缀名
 * \param[in]    extname后缀名buffer  para1
 * \param[in]
 * \param[out]   none
 * \return       void the result
 * \retval       none
 * \retval       none
 * \ingroup      music_playing_getid3.c
 * \note
 */
/*******************************************************************************/
id3_type_e play_scene_checktype(uint8 * extname)
{
    uint8 cnt;
    id3_type_e ret;
    //转换当前后缀名(转成大写)
    extname[0] = ((extname[0] >= 'a') && (extname[0] <= 'z')) ? (extname[0] - 'a' + 'A') : extname[0];
    extname[1] = ((extname[1] >= 'a') && (extname[1] <= 'z')) ? (extname[1] - 'a' + 'A') : extname[1];
    extname[2] = ((extname[2] >= 'a') && (extname[2] <= 'z')) ? (extname[2] - 'a' + 'A') : extname[2];
    //比较后缀
    for (cnt = 0; cnt < MusicExtNum; cnt++)
    {
        if (libc_strncmp(extname, support_ext[cnt], sizeof(support_ext[cnt])) == 0)
        {
            ret = support_id3_type[cnt];
            break;
        }
    }
    if (cnt == MusicExtNum)
    {
        ret = ID3_TYPE_END;
    }
    return ret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \void play_scene_initid3buf(void)
 * \获取后缀名
 * \param[in]    void  para1
 * \param[in]
 * \param[out]   none
 * \return       void the result
 * \retval       none
 * \retval       none
 * \ingroup      music_playing_getid3.c
 * \note
 */
/*******************************************************************************/
void play_scene_initid3buf(void)
{
    //标题buffer
    libc_memset(g_title_buf, 0, sizeof(g_title_buf));

    g_id3_info.tit2_buffer = g_title_buf;
    //歌手buffer和长度
    libc_memset(g_artist_buf, 0, sizeof(g_artist_buf));

    g_id3_info.tpe1_buffer = g_artist_buf;
    //专辑buffer
    libc_memset(g_album_buf, 0, sizeof(g_album_buf));

    g_id3_info.talb_buffer = g_album_buf;
    //流派buffer
    g_id3_info.genre_buffer = NULL;
    //drm的buffer
    g_id3_info.drm_buffer = NULL;
    //音轨buffer
    g_id3_info.track_buffer = NULL;

    //文件标题长度
    g_id3_info.tit2_length = Id3BufSIZE;
    //作者长度
    g_id3_info.tpe1_length = ArtBufSIZE;
    //专辑长度
    g_id3_info.talb_length = ArtBufSIZE;
    //流派长度
    g_id3_info.genre_length = 0;
    //drm长度
    g_id3_info.drm_length = 0;
    //音轨长度
    g_id3_info.track_length = 0;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \void deal_id3_sub(uint8 *str_buf, uint8 type)
 * \处理id3的操作
 * \param[in]    str_buf id3信息的buffer  para1
 * \param[in]    type id3类型 para2
 * \param[out]   none
 * \return       void the result
 * \retval       none
 * \retval       none
 * \ingroup      music_playing_getid3.c
 * \note
 */
/*******************************************************************************/
void deal_id3_sub(uint8 *str_buf, uint8 type)
{
    //uint16 str_len;
    //uint16 str_width;
    uint16 convert_size;
    //uint8 unicode_flag;
    //string_desc_t desc;
    uint8 buffer_size;

    if ((str_buf[0] == 0xff) && (str_buf[1] == 0xfe))
    {
        //str_len = Id3BufSIZE;
        //unicode_flag = 0x01;
        //unicode编码
        g_scroll_content[type].language = UNICODEDATA;
    }
    else if ((str_buf[0] == 0xef) && (str_buf[1] == 0xbb) && (str_buf[2] == 0xbf))
    {
        ui_utf8_to_unicode(str_buf, convert_buf, &convert_size);
        if (type == 0)
        {
            buffer_size = Id3BufSIZE;
        }
        else
        {
            buffer_size = ArtBufSIZE;
        }
        if (convert_size > buffer_size)
        {
            convert_size = buffer_size;
            convert_buf[buffer_size - 2] = 0;
            convert_buf[buffer_size - 1] = 0;
        }
        libc_memcpy(str_buf, convert_buf, (uint32) convert_size);
        //str_len = convert_size;
        //unicode_flag = 0x01;
        //unicode编码
        g_scroll_content[type].language = UNICODEDATA;
    }
    else
    {
        //str_len = Id3BufSIZE;
        //unicode_flag = 0x00;
        g_scroll_content[type].language = ANSIDATAAUTO;
    }
    //desc.data.str = str_buf;
    //desc.length = str_len;

    //if (unicode_flag == TRUE)
    //{
    //    desc.language = UNICODEDATA;
    //}
    //else
    //{
    //    desc.language = 0;
    //}

}
/******************************************************************************/
/*!
 * \par  Description:
 * \void play_scene_dealid3(void)
 * \处理id3信息
 * \param[in]    void  para1
 * \param[in]
 * \param[out]   none
 * \return       void the result
 * \retval       none
 * \retval       none
 * \ingroup      music_playing_getid3.c
 * \note
 */
/*******************************************************************************/
void play_scene_deal_id3(void)
{
    deal_id3_sub(g_title_buf, 0);
    deal_id3_sub(g_artist_buf, 1);

    if (g_audible_now == FALSE)
    {
        deal_id3_sub(g_album_buf, 2);
    }
    else
    {
        deal_id3_sub(g_title_buf, 2);
    }
    //滚屏内容填充
    g_scroll_content[0].strbuf = g_title_buf;
    g_scroll_content[1].strbuf = g_artist_buf;
    g_scroll_content[2].strbuf = g_album_buf;

    if (g_audible_now == TRUE)
    {
        //audible没有album
        g_scroll_content[2].strbuf = g_title_buf;
    }

}

/******************************************************************************/
/*!
 * \par  Description:
 * \void void play_scene_getid3(file_path_info_t* path_info)
 * \获取id3信息
 * \param[in]    path_info文件的路径信息  para1
 * \param[in]
 * \param[out]   none
 * \return       void the result
 * \retval       none
 * \retval       none
 * \ingroup      music_playing_getid3.c
 * \note
 */
/*******************************************************************************/
void play_scene_getid3(file_path_info_t* path_info)
{
    id3_type_e music_type;
    //bool have_no_id3 = FALSE;
    uint8* nameptr = path_info->file_path.dirlocation.filename;
    mengine_playinfo_t playinfo;

    //初始化变量
    play_scene_initid3buf();

    //获取当前文件类型
    music_type = play_scene_checktype(nameptr);

    //定位到文件
    if (fsel_browser_set_file(&(path_info->file_path.plist_location), path_info->file_source) == FALSE)
    {
        return;
    }

    //是否取album art信息
    if (g_music_config.album_art_support == TRUE)
    {
        g_id3_info.apic_flag = 1;
    }
    else
    {
        g_id3_info.apic_flag = 0;
    }

    g_id3_info.apic_offset = (uint32) & (album_art_info.cluster_no);

    //获取id3信息
    get_id3_info(&g_id3_info, NULL, music_type);

    //处理id3超长等
    play_scene_deal_id3();

    //check album art是否存在
    if (g_id3_info.apic_type == 1)
    {
        //专辑图片在文件中的偏移
        album_art_info.offset = g_id3_info.apic_offset;

        album_art_info.apic_type = 0;

        g_album_art = TRUE;
    }
    else if (g_id3_info.apic_type == 2)
    {
        album_art_info.apic_type = 1;

        g_album_art = TRUE;
    }
    else
    {
        //未找到相应的专辑图片
        g_album_art = FALSE;
    }

    if ((g_audible_now == TRUE) && (g_music_status.play_status == PlaySta))
    {
        //主动获取专辑图片
        music_get_audible_image(&playinfo);
    }

    if (g_music_config.lrc_support == TRUE)
    {
        if (g_lyric_flag == TRUE)
        {
            g_lyric_flag = lyric_get_init();
        }
    }
}
