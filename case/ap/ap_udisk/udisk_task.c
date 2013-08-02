/*******************************************************************************
 *                              US212A
 *                            Module: udiskap
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       yliang     2011-11-07      1.0             build this file
 *******************************************************************************/
/*!
 * \file     udisk task.c
 * \brief    deal udisk message and display
 * \author   yliang
 * \version  1.0
 * \date  2011/10/07
 *******************************************************************************/
#include  "ap_udisk.h"
const uint8 batvalue[5] =
{ 0x58, 0x5d, 0x5f, 0x65, 0x76 };
void ram_clk_to_mcu(void)
{
    act_writel(act_readl(MEMCLKCTL1) & (~(0x00000001 << MEMCLKCTL1_RAM6CLKSEL)), MEMCLKCTL1);
    act_writel(act_readl(MEMCLKCTL0) | (0x00000001 << MEMCLKCTL0_RAM6CLKEN), MEMCLKCTL0);

    act_writel(act_readl(MEMCLKCTL1) & (~(0x00000001 << MEMCLKCTL1_RAM7CLKSEL)), MEMCLKCTL1);
    act_writel(act_readl(MEMCLKCTL0) | (0x00000001 << MEMCLKCTL0_RAM7CLKEN), MEMCLKCTL0);
}

/********************************************************************************
 * Description : 读battery 图片到缓存中
 *
 * Arguments  :     ramaddr
 *
 *
 * Returns     :
 *            无
 * Notes       :
 *
 ********************************************************************************/
void read_battery_pic_to_ram(void)
{
    res_size_t tmp_size;
    uint8 i;
    for (i = 0; i < 5; i++)
    {
        ui_read_pic_to_buffer(P_UDISK_BATTERY_0 + i, \
        (uint8 *) (BATTERY_PIC_BUFFER + i * BATTERY_PIC_LENGTH), &tmp_size);
    }
}

/********************************************************************************
 * Description : 读数据传输更新图片到缓存中
 *
 * Arguments  :     ramaddr
 *
 *
 * Returns     :
 *            无
 * Notes       :
 *
 ********************************************************************************/
void read_connect_pic_to_ram(void)
{
 /*   res_size_t tmp_size;
    uint8 i;
    for (i = 0; i < 10; i++)
    {
        ui_read_pic_to_buffer(P_UDISK_POINT_1A + i, \
        (uint8 *) (CONNECT_PIC_BUFFER + i * CONNECT_PIC_LENGTH), &tmp_size);
    }*/
}

/********************************************************************************
 * Description : 根据获取到的电压值显示相应的电池图标
 *
 * Arguments  :
 *            value：当前获取到的电压值
 *
 * Returns     :
 *            无
 * Notes       :
 *
 ********************************************************************************/
#if 0
void ui_show_battery(void)
{
    uint8 value;
    uint8 frame = 0;
    style_infor_t style_infor_battery;
    picbox_private_t picbox_param;
    value = act_readb(BATADC_DATA);
    if (charge_state == CHARGE_NOBATTERY)
    {
        frame = 0;
    }
    else
    {
        //分5档显示电池图标，对应关系如下：
        /*
         电池电压范围
         1.       空格电：<=3.5V  0.05V误差

         2.       一格电：3.5-3.6V  0.05V误差

         3.       两格电：3.6-3.65V   0.05V误差

         4.       三格电：3.65-3.8V    0.05V误差

         5         满格：  3 .8V以上    0.05V 误差

         */

        for (frame = 0; frame < 5; frame++)
        {
            if (value <= batvalue[frame])
            {
                break;
            }
        }
        if (frame >= 4)
        {
            frame = 4;
        }
    }
    style_infor_battery.type = UI_AP;
    style_infor_battery.style_id = UDISK_BATTERY;
    picbox_param.pic_id = -1;
    picbox_param.frame_id = frame;
    //显示界面
    ui_show_picbox(&style_infor_battery, &picbox_param);
}
#else
void ui_show_battery(void)
{
    uint8 value;
    uint8 frame = 0;
    style_infor_t style_infor_battery;
    picbox_private_t picbox_param;
    
    
    act_writel((act_readl(MFP_CTL1) & 0xffffffc7), MFP_CTL1);  //GPIOA17-TEM
		act_writel((act_readl(PMUADC_CTL) | (0x01 << 6)), PMUADC_CTL);
		value = act_readb(LRADC2_DATA) & 0xffffff7f; //干电池的电量检测用温度检测口
					
    
   // value = act_readb(BATADC_DATA);
    if (charge_state == CHARGE_NOBATTERY)
    {
        frame = 0;
    }
    else
    {
        //分5档显示电池图标，对应关系如下：
        /*
         电池电压范围
         1.       空格电：<=3.5V  0.05V误差

         2.       一格电：3.5-3.6V  0.05V误差

         3.       两格电：3.6-3.65V   0.05V误差

         4.       三格电：3.65-3.8V    0.05V误差

         5         满格：  3 .8V以上    0.05V 误差

         */

        for (frame = 0; frame < 5; frame++)
        {
            if (value <= batvalue[frame])
            {
                break;
            }
        }
        if (frame >= 4)
        {
            frame = 4;
        }
       
      if(value>0x3b)
       	 frame = 4;
      else if(value>0x33)
        frame = 3;
      else if(value>0x2b)
        frame = 2;
      else if(value>0x25)
        frame = 1;
      else
      	frame = 0;
    }
    style_infor_battery.type = UI_AP;
    style_infor_battery.style_id = UDISK_BATTERY;
    picbox_param.pic_id = -1;
    picbox_param.frame_id = frame;
    //显示界面
    ui_show_picbox(&style_infor_battery, &picbox_param);
}
#endif

void timer_usb_display_battery(void)
{
    uint16 pix_cnt;
    region_t res_region;
    time_count++;
    if (time_count == 5)
    {
        time_count = 0;
    }
    lcd_set_draw_mode(DRAW_MODE_H_DEF);
    /*将connect picture buffer中的图片刷到屏上*/
    res_region.x = POS_BATTERY_X_BASE;
    res_region.y = POS_BATTERY_Y_BASE;
    res_region.width = WIDTH_BATTERY;
    res_region.height = HEIGHT_BATTERY;

    lcd_set_window(&res_region); //设窗参数设置
    pix_cnt = res_region.width * res_region.height * 2;

    libc_memcpy(LCD_BUF_ADDR, BATTERY_PIC_BUFFER + time_count * BATTERY_PIC_LENGTH, pix_cnt);

    //DMA到GRAM中
    lcd_dma_set_counter(res_region.width * res_region.height, res_region.width);
    //开始DMA传输
    lcd_dma_start_trans(LCD_RAM_INDEX);
}
#if 0
void timer_usb_display_connect(void)
{
    uint8 i;
    uint8 frame;
    uint16 pix_cnt;
    region_t res_region;
    time_count++;
    if (time_count == 5)
    {
        time_count = 0;
    }
    lcd_set_draw_mode(DRAW_MODE_H_DEF);
    for (i = 0; i < 5; i++)
    {
        if (i == time_count)
        {
            frame = 2 * (4 - i);
        }
        else
        {
            frame = 2 * (4 - i) + 1;
        }
        /*将connect picture buffer中的图片刷到屏上*/
        res_region.x = POS_CONNECT_X_BASE;
        res_region.y = POS_CONNECT_Y_BASE + (4 - i) * POS_CONNECT_INC_Y;
        res_region.width = WIDTH_CONNECT;
        res_region.height = HEIGHT_CONNECT;

        lcd_set_window(&res_region); //设窗参数设置
        pix_cnt = res_region.width * res_region.height * 2;

        libc_memcpy(LCD_BUF_ADDR, CONNECT_PIC_BUFFER + frame * CONNECT_PIC_LENGTH, pix_cnt);

        //DMA到GRAM中
        lcd_dma_set_counter(res_region.width * res_region.height, res_region.width);
        //开始DMA传输
        lcd_dma_start_trans(LCD_RAM_INDEX);
    }
}
#endif
/******************************************************************************/
/*!
 * \par  Description:
 *    message mananger
 * \param[in]
 * \param[out]   none
 * \retval       none
 * \ingroup      main_udisk.c
 * \note
 *******************************************************************************/
void display(void)
{
    uint8 i;
    picbox_private_t picbox_data;
    textbox_private_t textbox_param;
    style_infor_t style_infor;
    style_infor.type = UI_AP;
    textbox_param.lang_id = UNICODEID;
    textbox_param.str_value = NULL;
    textbox_param.str_id = -1;
    if (usbstatus_bak != usbstatus) //USB 状态有变化
    {
        if (usbstatus == 0x00)  //显示任务指示 0:无显示动作  01:上传  02:下传  03:传输结束
        {
            charge_state_bak = CHARGE_NONE; 
        }        
        else if (((usbstatus == 0x10) || (usbstatus == 0x20) || (usbstatus == 0x60)) && (usbstatus_bak == 0x00))
        {
            if (timer_usb_battery_id != -1)
            {
                kill_app_timer(timer_usb_battery_id);
                timer_usb_battery_id = -1;
            }
/*
            if (timer_usb_connect_id == -1)
            {
                timer_usb_connect_id = set_app_timer(APP_TIMER_ATTRB_UI, 1000, timer_usb_display_connect);
                time_count = 0;
            }
            style_infor.style_id = UDISK_STATE_STR;
            textbox_param.str_id = S_UDISK_LINKING;
            ui_show_textbox(&style_infor, &textbox_param, TEXTBOX_DRAW_NORMAL);
            */
            ui_show_battery();
        }
        else
        {

        }
        
    }
    else
    {
    }    
    
    if (usbstatus == 0x00)  //空闲状态
    {
        if (timer_usb_connect_id != -1)
        {
            kill_app_timer(timer_usb_connect_id);
            timer_usb_connect_id = -1;
        }
        
      /*  if (usbstatus_bak != 0x00)//从非空闲状态进入到空闲状态才需要更新
        {
            for (i = 0; i < 5; i++)
            {
                style_infor.style_id = style_usb_point[i];
                picbox_data.pic_id = -1;
                picbox_data.frame_id = 0;
                //显示界面
                ui_show_picbox(&style_infor, &picbox_data);
            }
        }*/

      //  style_infor.style_id = UDISK_STATE_STR;
        charge_state = (charge_state_e) (uint32) key_chargeget(0);
        if (charge_state_bak != charge_state)
        {
            if (charge_state == CHARGE_CHARGING)
            {
                textbox_param.str_id = S_UDISK_CHARGING;
                if (timer_usb_battery_id == -1)
                {
                    timer_usb_battery_id = set_app_timer(APP_TIMER_ATTRB_UI, 500, timer_usb_display_battery);
                    time_count = 0;
                }
            }
            else if (charge_state == CHARGE_FULL)
            {
               // textbox_param.str_id = S_UDISK_FULL_CHARGED;
                if (timer_usb_battery_id != -1)
                {
                    kill_app_timer(timer_usb_battery_id);
                    timer_usb_battery_id = -1;
                }
                ui_show_battery();
            }
            else
            {

            }
            //ui_show_textbox(&style_infor, &textbox_param, TEXTBOX_DRAW_NORMAL);
        }
    }
    else
    {

    }
    charge_state_bak = charge_state;
    usbstatus_bak = usbstatus;
}

