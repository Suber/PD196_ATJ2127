/*******************************************************************************
 *                              US212A
 *                            Module: PIC_PLAY_SUB
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>         <time>             <version>             <desc>
 *      wuyufan    2011-12-9 15:04:19           1.0              build this file
 *******************************************************************************/
/*!
 * \file     pic_play_sub.c
 * \brief    图片解码子模块
 * \author   wuyufan
 * \par      GENERAL DESCRIPTION:
 *               该模块完成图片解码功能，实现解码初始化，与图片中间件的交互等
 * \par      EXTERNALIZED FUNCTIONS:
 *               需要调用图片中间件实现解码功能
 * \version 1.0
 * \date    2011-12-9
 *******************************************************************************/
#include "picture.h"
#include "picture_preview.h"

/******************************************************************************/
/*!
 * \par  Description:
 确定图片旋转时的坐标
 * \param[in]    region1 应用传递给中间件的窗坐标
 * \param[in]    region2 中间件返回给ap的图片实际坐标，需要对该坐标进行变换
 * \param[out]   none
 * \return       none
 * \ingroup      pic_play
 * \note
 * \li   由于底层未对图片旋转模式进行坐标变化，需要ap完成这一工作
 * \li   对于bmp图片,坐标原点在左下角，因此坐标只需xy交换位置
 * \li   对于jpg,bmp图片，坐标原点在左上角，需要在交换完xy坐标后再调整y坐标
 *******************************************************************************/
static void _format_region(wregion_t *region1, wregion_t *region2)
{
    uint16 temp_x = region2->x;
    uint16 region_width;
    uint16 region_height;

    region_width = region1->w;
    region_height = region1->h;

    if ((g_picture_userinfor.returnflag & RETURNFLAG_ROTATION) != 0)
    {
        //旋转坐标变换
        region2->x = region2->y;
        if (g_picture_userinfor.file_format == IMAGE_BMP)
        {
            region2->y = temp_x;
        }
        else
        {
            region2->y = DISPLAY_LENGTH - temp_x - region1->w;
        }

        //居中处理
        region2->x += (region_height - region2->w) / 2;
        region2->y += (region_width - region2->h) / 2;
    }
    else
    {
        //居中处理
        region2->x += (region_width - region2->w) / 2;
        region2->y += (region_height - region2->h) / 2;
    }

    return;

}
/******************************************************************************/
/*!
 * \par  Description:
 *    设置图片刷屏模式及设定图片窗口坐标
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      pic_play
 * \note
 * \li   注意清屏函数会更改刷屏模式，因此一定要在清屏后设置刷屏模式
 *******************************************************************************/
static void _pic_set_window(void)
{
    uint8 draw_mode;

    //计算窗坐标
    _format_region(&region_in, &region_out);

    switch (g_picture_userinfor.file_format)
    {

        case IMAGE_GIF:
        case IMAGE_JPG:
        if ((g_picture_userinfor.returnflag & RETURNFLAG_ROTATION) != 0)
        {
            //横屏
            draw_mode = DRAW_MODE_V_DEF;
        }
        else
        {
            //竖屏 左上到右下
            draw_mode = DRAW_MODE_H_DEF;
        }
        break;
        case IMAGE_BMP:
        if ((g_picture_userinfor.returnflag & RETURNFLAG_ROTATION) != 0)
        {
            //横屏
            draw_mode = DRAW_MODE_V_PIC_DEF;
        }
        else
        {
            //竖屏
            draw_mode = DRAW_MODE_H_PIC_DEF;
        }
        break;

        default:
        draw_mode = DRAW_MODE_H_DEF;
        break;
    }

    lcd_set_draw_mode(draw_mode);

    //设置图片显示窗口
    lcd_set_window(&region_out);

}

/******************************************************************************/
/*!
 * \par  Description:
 *      设置待播放图片子函数
 * \param[in]    none
 * \param[out]   none
 * \return       decode_status_e 返回解码状态
 * \ingroup      pic_play
 * \note
 * \li  设置图片待播放文件，需要完成加载图片中间件，打开中间件句柄，发送SET_FILE命令等工作
 * \li  如果设置失败，会返回相应的错误号，应用据此进行相应处理
 *******************************************************************************/
decode_status_e _pic_set_file(void)
{
    int32 dec_result;

    sys_bank_flush(AP_BANK_FRONT_ENHANCED_1);
    sys_bank_flush(AP_BANK_FRONT_ENHANCED_2);

    //装载图片中间件
    if (-1 == sys_load_mmm("mmm_id.al", FALSE))
    {
        //错误
        return DECODE_LOAD_ERR;
    }
    else
    {
        g_mmm_id_free = FALSE;
    }

    //打开线程句柄
    dec_result = mmm_id_cmd(&picture_handle, MMM_ID_OPEN, 0);
    if ((picture_handle == NULL) || (dec_result == MMM_ID_ENGINE_ERROR))
    {
        return DECODE_OPEN_ERR;
    }

    //发送SET_FILE命令
    dec_result = mmm_id_cmd(picture_handle, MMM_ID_SET_FILE, (uint32) & g_picture_userinfor);
    if (dec_result != 0)
    {
        //文件格式不对，或者无法解指定窗大小的文件
        //需要切换文件
        g_picture_change_file |= up_picture_changefile;
        return DECODE_FILE_ERR;
    }
    return DECODE_NO_ERR;

}
/******************************************************************************/
/*!
 * \par  Description:
 *    图片解码函数
 * \param[in]    none
 * \param[out]   none
 * \return       decode_status_e 解码状态
 * \ingroup      pic_play
 * \note
 * \li  根据待播放文件，发送解码命令，同时会对文件格式进行检查。如果此时后台正在播放
        music，而前台为软解bmp/gif,则给出提示语进行选择
 * \li  该函数可能被图片解码和缩略图解码共同调用
 *******************************************************************************/
decode_status_e pic_decode(void)
{
    decode_status_e dec_status;
    engine_type_e engine_type = get_engine_type();
    engine_state_e engine_state = get_engine_state();

    if ((engine_type == ENGINE_MUSIC) && (engine_state == ENGINE_STATE_PLAYING))
    {
        //后台有music播放，则只支持整数倍缩放
        g_picture_userinfor.playflag |= PLAYFLAG_BYPASS_MODE;
    }
    else
    {
        g_picture_userinfor.playflag &= PLAYFLAG_FULL_MODE;
    }

    //无效font cache
    ui_flush_font_buffer();
    dec_status = _pic_set_file();
    if (dec_status != DECODE_NO_ERR)
    {
        return dec_status;
    }

    if ((engine_type == ENGINE_MUSIC) && (engine_state == ENGINE_STATE_PLAYING))
    {
        if(g_picture_userinfor.file_format != IMAGE_JPG)
        {
            //关闭图片解码
            pic_decoder_free();

            //检测文件格式在当前场景是否支持
            if (pic_check_engine(g_picture_userinfor.file_format) == FALSE)
            {
                //确认不进行picture播放
                return DECODE_UNSUPPORT_ERR;
            }
            else
            {
                //重新开始解码
                g_picture_userinfor.playflag &= 0xfb;

                //清屏 开始解码
                ui_clear_screen(NULL);

                //开启图片解码
                _pic_set_file();
            }
        }
    }

    //设窗
    _pic_set_window();

    //开始图片解码
    mmm_id_cmd(picture_handle, MMM_ID_DECODE, 0);
    return DECODE_NO_ERR;
}

/******************************************************************************/
/*!
 * \par  Description:
 *    图片解码初始化
 * \param[in]  mode 初始化模式 0：播放模式 1：缩略图模式
 * \param[out] none
 * \return     the result
 * \retval     TRUE  初始化成功
 * \retval     FALSE 初始化失败
 * \ingroup    pic_play
 * \note
 * \li  初始化包括打开解码线程句柄，初始化文件设定
 * \li  该函数可能被图片解码和缩略图解码共同调用
 *******************************************************************************/
bool pic_decode_init(uint8 mode)
{
    bool ret_val = TRUE;
    //engine_type_e engine_type = get_engine_type();

    g_mmm_id_free = TRUE;
    ui_clear_screen(NULL);

    //每次进入之前如果选择不是关闭music，则进行一次询问
    gui_set_headbar_mode(HEADBAR_MODE_NULL, this_headbar_icon_id);
    if (mode == 0)
    {
        //播放界面设置图片解码窗口为全屏模式
        region_in.x = 0;
        region_in.y = 0;
        region_in.w = DISPLAY_LENGTH;
        region_in.h = DISPLAY_HEIGHT;

        g_picture_change_file = 0;

        //进入为非自动播放模式
        g_picture_auto_play = FALSE;

        g_picture_userinfor.playflag &= PLAYFLAG_NORMAL_DECODE;

        //需要延时
        g_picture_userinfor.playflag |= PLAYFLAG_DELAY_MODE;

        //申请用于自动播放的定时器
        timer_pic_play = set_app_timer(APP_TIMER_ATTRB_UI, (uint16)(g_comval.slide_time * 1000), auto_play_flag);
        stop_app_timer(timer_pic_play);
    }
    else
    {
        //缩略图模式窗口这里先不设置
        gui_set_headbar_mode(HEADBAR_MODE_NORMAL, this_headbar_icon_id);
        g_picture_userinfor.playflag |= PLAYFLAG_PREVIEW_DECODE;

        //不需要延时
        g_picture_userinfor.playflag &= PLAYFLAG_NODELAY_MODE;

        //申请用于80ms滚动播放的定时器
        timer_pic_play = set_app_timer(APP_TIMER_ATTRB_UI, 80, scroll_play_flag);
        stop_app_timer(timer_pic_play);
    }

    //设置要播放文件路径
    ret_val = fsel_set_location(&g_picture_var.path.file_path, g_picture_var.path.file_source);

    if (ret_val == FALSE)
    {
        ret_val = fsel_get_nextfile(g_picture_var.path.file_path.dirlocation.filename);
        if (ret_val == TRUE)
        {
            ret_val = fsel_get_location(&g_picture_var.path.file_path, g_picture_var.path.file_source);
        }

    }
    return ret_val;
}


