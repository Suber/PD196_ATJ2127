/*******************************************************************************
 *                              US212A
 *                            Module: music engine
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *      fiona.yang  2012-05-30        1.0              create this file
 *******************************************************************************/

#include "app_mengine.h"

//first 32*2 is normal volume, last 32*2 is eq volume
const uint8 VolumeTbl[32 * 2][2] =
{
    //normal volume
    { 40, 64 },
    { 40, 60 },
    { 39, 64 },
    { 39, 60 },
    { 38, 64 },
    { 38, 60 },
    { 37, 60 },
    { 36, 60 },
    { 35, 60 },
    { 34, 60 },
    { 33, 60 },
    { 32, 60 },
    { 31, 60 },
    { 30, 58 },
    { 28, 60 },
    { 27, 56 },
    { 26, 56 },
    { 25, 56 },
    { 24, 56 },
    { 23, 56 },
    { 22, 54 },
    { 20, 62 },
    { 19, 59 },
    { 17, 62 },
    { 16, 53 },
    { 14, 51 },
    { 12, 51 },
    { 10, 46 },
    { 8, 41 },
    { 6, 33 },
    { 4, 24 },
    { 0, 24 },
    //eq volume
    //normal volume
    { 40, 24 },
    { 40, 24 },
    { 40, 24 },
    { 40, 24 },
    { 40, 24 },
    { 40, 24 },
    { 40, 24 },
    { 40, 24 },
    { 40, 24 },
    { 40, 24 },
    { 40, 24 },
    { 40, 24 },
    { 39, 24 },
    { 38, 23 },
    { 37, 22 },
    { 35, 23 },
    { 33, 24 },
    { 32, 22 },
    { 30, 24 },
    { 28, 25 },
    { 27, 22 },
    { 25, 25 },
    { 24, 24 },
    { 22, 24 },
    { 20, 25 },
    { 18, 23 },
    { 16, 22 },
    { 13, 23 },
    { 10, 26 },
    { 8, 20 },
    { 4, 24 },
    { 0, 24 },
};

//支持变速播放文件类型
const uint8 music_speed_ext[5][4] =
{
    "MP1", //mp3
    "MP2", //mp3
    "MP3", //mp3
    "WMA", //wma
    "WAV"  //wav
};

static uint8 eq_volume_type[3] =
{
    EQ_EQ_USR_MODE,
    EQ_TYPE_USR_MODE,
    EQ_FULLSOUND_MODE
};

/******************************************************************************/
/*!
 * \par  Description:
 * \void mengine_set_real_volume(uint8 volume, uint8 eq_type)
 * \音量转换
 * \param[in]    volume  para1 输入音量值
 * \param[in]    eq_type  param2 eq类型
 * \param[out]   none
 * \return       uint32 the result
 * \retval
 * \retval
 * \ingroup      mengine_event_volume.c
 * \note
 */
/*******************************************************************************/
void mengine_set_real_volume(uint8 volume, uint8 eq_type)
{
    uint8 i;
    uint8 eq_volume = FALSE;

    //设置下去的音量值
    uint32 volum_val;

    uint32 volum_val_high;

    uint8 tab_index;

    //判断EQ类型
    for (i = 0; i < 3; i++)
    {
        if (eq_type == eq_volume_type[i])
        {
            eq_volume = TRUE;
        }
    }

    if (eq_volume == FALSE)
    {
        tab_index = (31 - volume);
    }
    else
    {
        tab_index = (31 - volume) + 32;
    }

    volum_val = (VolumeTbl[tab_index][0] & 0x3f);

    volum_val_high = VolumeTbl[tab_index][1];

    volum_val_high <<= 16;

    volum_val |= volum_val_high;

    //设置音量
    mmm_mp_cmd(mp_handle, MMM_MP_SET_VOLUME, volum_val);

    return;

}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e mengine_set_volume(void* msg_ptr)
 * \设置音量
 * \param[in]    msg_ptr消息指针  para1
 * \param[in]    private_msg_t
 *               typedef struct
 *               {
 *                 unsigned char sync;		//同步标志，1B
 *                 sem_id_t sem_id; 		  //信号量ID，1B
 *                 msg_apps_t msg;		    //消息内容，6B
 *               } private_msg_t;
 *               typedef struct
 *               {
 *                 msg_apps_type_e  type;	//消息类型，2B
 *                 union
 *                 {
 *	                 unsigned char data[4];
 *                   unsigned char *addr;
 *                 }content;					//消息内容或其地址，由消息自己约定，4B
 *               } msg_apps_t;//私有消息队列的消息结构体

 * \param[out]   msg_ptr->msg.content.addr //传送eq信息的指针
 * \return       int the result
 * \retval       RESULT_IGNORE 忽略
 * \retval       ......
 * \ingroup      mengine_event_volume.c
 * \note
 */
/*******************************************************************************/
app_result_e mengine_set_volume(void* msg_ptr)
{
    //设置音量内容的指针
    private_msg_t* data_ptr = (private_msg_t*) msg_ptr;

    //将消息指针指向的数据copy到引擎变量
    g_eg_cfg_p->volume = *(uint8*) (data_ptr->msg.content.addr);

    mengine_set_real_volume(g_eg_cfg_p->volume, (uint8) g_eg_cfg_p->eq_info.eq_type);

    //返回成功
    mengine_reply_msg(msg_ptr, TRUE);

    return RESULT_IGNORE;

}
/******************************************************************************/
/*!
 * \par  Description:
 * \bool check_music_file_speed_type(uint8 * extname)
 * \检测当前文件是否支持变速播放
 * \param[in]    extname  待播放文件后缀名
 * \param[out]   none
 * \return       bool
 * \retval       TRUE 支持变速播放  FALSE 不支持变速播放
 * \retval
 * \ingroup      mengine_main.c
 * \note
 */
/*******************************************************************************/
bool _check_music_file_speed_type(uint8 * extname)
{
    uint8 cnt;
    bool ret_val = FALSE;
    //转换当前后缀名(转成大写)
    extname[0] = ((extname[0] >= 'a') && (extname[0] <= 'z')) ? (extname[0] - 'a' + 'A') : extname[0];
    extname[1] = ((extname[1] >= 'a') && (extname[1] <= 'z')) ? (extname[1] - 'a' + 'A') : extname[1];
    extname[2] = ((extname[2] >= 'a') && (extname[2] <= 'z')) ? (extname[2] - 'a' + 'A') : extname[2];
    //比较后缀
    for (cnt = 0; cnt < 5; cnt++)
    {
        if (libc_strncmp(extname, music_speed_ext[cnt], sizeof(music_speed_ext[cnt])) == 0)
        {
            ret_val = TRUE;
        }
    }
    return ret_val;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \void mengine_set_real_eq(uint8 mode)
 * \向解码中间件发送设置EQ命令
 * \param[in]    mode TRUE 正常切歌设置EQ FALSE 通过菜单设置EQ
 * \param[out]   none
 * \return       none
 * \ingroup      mengine_main.c
 * \note
 */
/*******************************************************************************/
void mengine_set_real_eq(uint8 mode)
{
    int32 eq_type;
    uint8 *ext_name;
    bool is_real_eq = TRUE;

    eq_type = mengine_info.eg_config.eq_info.eq_type;

    //当前是否为变速播放
    if (eq_type == EQ_VPS_USR_MODE)
    {
        ext_name = mengine_info.eg_config.location.dirlocation.filename;

        if (_check_music_file_speed_type(ext_name) == FALSE)
        {
            //非支持变速播放文件类型，不改变当前实际EQ
            is_real_eq = FALSE;
            if (mode == FALSE)
            {
                //非支持变速播放文件调节变速播放不需要设置硬件EQ
                return;
            }
        }
    }

    if (is_real_eq == TRUE)
    {
        //当前为非变速播放模式或支持变速播放模式的文件，设置当前eq到解码器
        mmm_mp_cmd(mp_handle, MMM_MP_SET_EQ, (unsigned int) &mengine_info.eg_config.eq_info);
    }
    else
    {
        //当前为变速播放模式但文件不支持变速播放, 设置上一次保存的EQ到解码器
        mmm_mp_cmd(mp_handle, MMM_MP_SET_EQ, (unsigned int) &mengine_info.eg_config.eq_info_other);
    }

    if (mode == TRUE)
    {
        //设置音量
        mengine_set_real_volume(g_eg_cfg_p->volume, (uint8)eq_type);
    }
    return;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e mengine_set_eq(void* msg_ptr)
 * \设置eq
 * \param[in]    msg_ptr消息指针  para1
 * \param[in]    private_msg_t
 *               typedef struct
 *               {
 *                 unsigned char sync;		//同步标志，1B
 *                 sem_id_t sem_id; 		  //信号量ID，1B
 *                 msg_apps_t msg;		    //消息内容，6B
 *               } private_msg_t;
 *               typedef struct
 *               {
 *                 msg_apps_type_e  type;	//消息类型，2B
 *                 union
 *                 {
 *	                 unsigned char data[4];
 *                   unsigned char *addr;
 *                 }content;					//消息内容或其地址，由消息自己约定，4B
 *               } msg_apps_t;//私有消息队列的消息结构体

 * \param[out]   msg_ptr->msg.content.addr //传送eq信息的指针
 * \return       int the result
 * \retval       RESULT_IGNORE 忽略
 * \retval       ......
 * \ingroup      mengine_event.c
 * \note
 */
/*******************************************************************************/
app_result_e mengine_set_eq(void* msg_ptr)
{
    uint8 i;
    uint8 eq_type;
    uint8 normal_volume = TRUE;
    bool need_set_volume = FALSE;

    //设置eq数据内容的指针
    private_msg_t * data_ptr = (private_msg_t*) msg_ptr;

    mmm_mp_eq_info_t *eq_info_p = data_ptr->msg.content.addr;

    eq_type = (uint8)(eq_info_p->eq_type);

    for (i = 0; i < 3; i++)
    {
        if (eq_type == eq_volume_type[i])
        {
            //需要设置EQ音量
            normal_volume = FALSE;
        }
    }

    //要设置的音效类型和引擎记录的音效类型不一致
    if (eq_type != g_eg_cfg_p->eq_info.eq_type)
    {
        //从其他音效切换到类EQ音效，如EQ音效，FullSound等
        if (normal_volume == TRUE)
        {
            //只有音效类型不一致时才需要设置,变速播放需要备份之前设置的EQ值，因为对于不支持变速播放的文件
            //还需要设置之前的音效
            if (eq_type == EQ_VPS_USR_MODE)
            {
                //属于变速播放，则需要保存之前设置的EQ
                libc_memcpy(&(mengine_info.eg_config.eq_info_other), &(g_eg_cfg_p->eq_info), sizeof(mmm_mp_eq_info_t));
            }

            //从fullsound音效切换到其它音效，先设置为NORMAL音效
            g_eg_cfg_p->eq_info.eq_type = EQ_NORMAL;
            mengine_set_real_eq(0);
        }

        need_set_volume = TRUE;
    }

    //将消息指针指向的数据copy到引擎变量
    libc_memcpy(&(g_eg_cfg_p->eq_info), (data_ptr->msg.content.addr), sizeof(mmm_mp_eq_info_t));

    //设置当前的EQ
    mengine_set_real_eq(0);

    if (need_set_volume == TRUE)
    {
        mengine_set_real_volume(g_eg_cfg_p->volume, eq_type);
    }

    //返回成功
    mengine_reply_msg(msg_ptr, TRUE);

    return RESULT_IGNORE;
}
