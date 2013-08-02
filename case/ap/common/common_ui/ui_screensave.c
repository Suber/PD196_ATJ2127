/*******************************************************************************
 *                              us212A
 *                            Module: commonUI-screensave
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 *
 * History:         
 *      <author>    <time>           <version >             <desc>
 *       lzcai     2011-9-19 17:24     1.0             build this file 
*******************************************************************************/
/*!
 * \file     _ui_screensave.c
 * \brief    commonUI 屏幕保护控制流实现
 * \author   lzcai
 * \par      GENERAL DESCRIPTION:
 *               实现了一种屏幕保护GUI，包括数字时钟，音乐专辑图片，关背光灯，演示模式等。
 * \par      EXTERNALIZED FUNCTIONS:
 *               直接调用ui 驱动中的picbox等完成控件显示流任务。
 * \version  1.0
 * \date     2011-9-19
*******************************************************************************/

#include "common_ui.h"

extern void draw_digit_clock(bool flag) __FAR__;
extern void digit_clock_handle(void) __FAR__;
extern void album_art_handle(void) __FAR__;
extern void demo_mode_handle(void) __FAR__;
extern void play_albumart_save(region_t *region);
extern void play_albumart_save_init(void);
/******************************************************************************/
/*!                    
 * \par  Description:
 *    进入屏幕保护模式。
 * \param[in]    ss_mode 屏幕保护模式，详细见screen_save_mode_e定义
 * \param[out]   none    
 * \return       app_result_e
 * \retval           RESULT_NULL 没实际意义
 * \note 
*******************************************************************************/
app_result_e gui_screen_save_enter (screen_save_mode_e ss_mode)
{
    style_infor_t style_infor;
    
    g_ss_tag_backup = get_app_timer_tag();
    change_app_timer_tag(APP_TIEMR_TAG_SCREEN);
    
    g_ss_direct_bk = com_get_gui_direction();
    com_set_gui_direction(GUI_DIRECT_NORMAL);
    
    gui_get_headbar_mode(&g_ss_mode_backup, &g_ss_icon_backup);
    gui_set_headbar_mode(HEADBAR_MODE_NULL, this_headbar_icon_id);
    
    //进入屏幕保护
    change_screensave_state(TRUE);
    
    //音乐专辑图片模式，若没有播放音乐或者暂停播放，自动切换到数字时钟模式
    if( (ss_mode == SCREEN_ALBUM_ART) 
     && (get_engine_type() != ENGINE_MUSIC) )
    {
        ss_mode = SCREEN_DIGIT_CLOCK;
    }
    
    g_ss_mode = ss_mode;
    
    gui_set_headbar_mode(HEADBAR_MODE_NULL, this_headbar_icon_id);

    //执行屏保初始化部分操作
    switch(g_ss_mode)
    {
    case SCREEN_DIGIT_CLOCK:
        //刷背景图
        style_infor.style_id = DIGIT_CLOCK_BG;
        style_infor.type = UI_COM;
        ui_show_picbox(&style_infor, NULL);
        
        //先绘制初始数字时钟，整个数字时钟都要更新
        draw_digit_clock(TRUE);
        //创建周期为 5S 应用级定时器，周期更新日期和时间显示
        g_ss_timer_id = set_app_timer(APP_TIMER_ATTRB_UI, 5000, digit_clock_handle);
        break;
        
    case SCREEN_ALBUM_ART:
        play_albumart_save_init();        
        //创建周期为 5S 应用级定时器，周期更新 Album Art 图片
        g_ss_timer_id = set_app_timer(APP_TIMER_ATTRB_UI, 5000, album_art_handle);
        break;
    
    case SCRREN_SCREEN_OFF:
        //关掉背光灯，关掉背光前先清屏，效果更好
        ui_set_backgd_color(0x0000);//清为全黑屏
        ui_clear_screen(NULL);
        lcd_backlight_on_off(FALSE);
        lcd_standby_screen(FALSE);
        change_backlight_state(BACKLIGHT_STATE_OFF);
        
        //调低频率为 3M
        g_ss_clk_backup = (uint8)sys_adjust_clk(FREQ_3M, 0);
        break;
        
    case SCREEN_DEMO_MODE:
        //创建周期为 1S 应用级定时器，周期更新应用 Demo 图片
        g_ss_timer_id = set_app_timer(APP_TIMER_ATTRB_UI, 1000, demo_mode_handle);
        g_ss_demo_mode_phase = 0;
        
        style_infor.style_id = DEMO_MODE_PIC;
        style_infor.type = UI_COM;
        ui_get_picbox_attrb(&style_infor, &g_ss_demo_frame_cnt, 1);
        
        //初始显示
        demo_mode_handle();
        break;
        
    default:
        break;
    }
    
    return RESULT_NULL;
}

/******************************************************************************/
/*!                    
 * \par  Description:
 *    退出屏幕保护模式。
 * \param[in]    ss_mode 屏幕保护模式，详细见screen_save_mode_e定义
 * \param[out]   none    
 * \return       app_result_e
 * \retval           RESULT_REDRAW 退出屏保，需要重绘UI
 * \retval           RESULT_NULL 延迟退出屏保，无需处理
 * \note 
*******************************************************************************/
app_result_e gui_screen_save_exit (void)
{
    //如果背光关闭，打开背光
    if(get_backlight_state() == BACKLIGHT_STATE_OFF)
    {
        //判断是否需要回到正在播放界面
        if((g_ss_vip_event != TRUE)//不是因为高优先级事件退出屏幕保护
        && (g_ss_delay_flag == FALSE)//不是delay退出屏保
        && (get_engine_state() == ENGINE_STATE_PLAYING)//后台引擎不处于播放状态
        && (get_app_state() == APP_STATE_NO_PLAY))//前台应用不处于播放 UI 状态
        {
            msg_apps_t msg;
            
            //给当前AP发送 back_to_playing 异步消息
            msg.type = MSG_APP_BACKTO_PLAYING;
            send_async_msg(g_this_app_info->app_id, &msg);
            
            g_ss_delay_flag = TRUE;//delay退出屏保
            return RESULT_NULL;
        }
        
        //恢复到黑屏前频率
        sys_adjust_clk(g_ss_clk_backup, 0);
        
        //点亮背光
        lcd_standby_screen(TRUE);
        lcd_backlight_on_off(TRUE);
        com_set_contrast(sys_comval->lightness);
        change_backlight_state(BACKLIGHT_STATE_NORMAL);
    }
    
    //执行屏保退出处理
    switch(g_ss_mode)
    {
    case SCREEN_DIGIT_CLOCK:
    case SCREEN_ALBUM_ART:
    case SCREEN_DEMO_MODE:
        //删除定时器
        kill_app_timer(g_ss_timer_id);
        break;
        
    default:
        break;
    }
    
    //退出屏幕保护
    change_screensave_state(FALSE);
    com_set_gui_direction(g_ss_direct_bk);
    change_app_timer_tag(g_ss_tag_backup);
    gui_set_headbar_mode(g_ss_mode_backup, g_ss_icon_backup);
    g_ss_mode = SCREEN_NULL;
    
    g_ss_vip_event = FALSE;
    return RESULT_REDRAW;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \void play_albumart_save(region_t pic_region)
 * \保存专辑封面album art
 * \param[in]    void  para1
 * \param[in]
 * \param[out]   none
 * \return       void the result
 * \retval       none
 * \retval       none
 * \ingroup      ui_screensave.c
 * \note
 */
/*******************************************************************************/
void play_albumart_save(region_t *pic_region)
{
    region_t tmp_region;
#ifndef PC
    uint8* save_buf = (uint8*) 0x9fc3fc00;
#else
    uint8* save_buf = GET_REAL_ADDR(0x9fc3fc00);
#endif
    uint32 pic_cnt;
    uint8 i, loop_cnt;
    
    loop_cnt = (uint8)(pic_region->height / 2);

    //设置竖屏模式
    lcd_set_draw_mode(DRAW_MODE_H_DEF);
    //设窗,读取背景
    tmp_region.x = pic_region->x;
    tmp_region.y = pic_region->y;
    tmp_region.width = pic_region->width;
    tmp_region.height = 2;

    pic_cnt = pic_region->width * 2;

    for (i = 0; i < loop_cnt; i++)
    {
        //设窗
        lcd_set_window(&tmp_region);
        //读取GRAM点阵
        lcd_get_buff_data(save_buf, pic_cnt);
        sys_vm_write(save_buf, VM_ALBUMART_INFO + i * 512);
        tmp_region.y += 2;
    }
#if 0//height 为偶数，没有余量；如果为奇数，需打开
    pic_cnt = (pic_region.height % 2) * pic_region.width;
    if (pic_cnt != 0)
    {
        //设窗
        lcd_set_window(&tmp_region);
        lcd_get_buff_data(save_buf, pic_cnt);
        sys_vm_write(save_buf, VM_ALBUMART_INFO + i * 512);
    }
#endif
}
/******************************************************************************/
/*!
 * \par  Description:
 * \void play_albumart_save_init(void)
 * \初始化进入屏保显示专辑相册
 * \param[in]    void  para1
 * \param[in]
 * \param[out]   none
 * \return       void the result
 * \retval       none
 * \retval       none
 * \ingroup      ui_screensave.c
 * \note
 */
/*******************************************************************************/
void play_albumart_save_init(void)
{
    if (g_com_paint_cb.func != NULL)
    {
        ui_clear_screen(NULL);
        g_com_paint_cb.func(g_com_paint_cb.arg);
    }
    
    //保存 albumart 到 VRAM 去，以便屏保读取
    play_albumart_save(&(g_com_paint_cb.arg->region));

    g_ss_album_art_phase = 0;
    
    //初始显示
    //album_art_handle();

}
