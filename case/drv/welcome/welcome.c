/*
 ********************************************************************************
 *                       ACTOS
 *                 show logo when boot
 *
 *                (c) Copyright 2003-2009, Actions Co,Ld.
 *                        All Right Reserved
 *
 * File   : welcome.c
 * By     : fiona.yang
 ********************************************************************************
 */
#include "lcd_inner.h"

void mcu_enter_sleep(void);

void disp_starting(void(*adfu_launch)(void), void(*read_res)(uint8*, uint16, uint16))
{
    uint8 loopcnt;
    uint8 dpdm_status = 0;
    //for low power
    uint8 batval;
    uint8 *buffer_addr = (uint8*) 0xbfc28000;
    uint16 pixcnt;
//    uint16 sec_cnt = 0;
//    region_t picreg;

    dpdm_status = act_readb(DPDMCTRL) & 0x40;
    dpdm_status |= (uint8)(act_readb(SYSTEM_VOL) & 0x80);
    batval = act_readb(BATADC_DATA);
    //for low power and adfu check
    if ((dpdm_status & 0x000000c0) == 0)
    {
        if (batval < 0x51)
        {
            mcu_enter_sleep();
        }
    }

    if (((act_readb(DPDMCTRL)) & 0x40) != 0x00)
    {
        loopcnt = 5;
        //USB插入时，检测Play键进入ADFU
        while (loopcnt != 0)
        {
            if ((act_readl(SYSTEM_CTL_RTCVDD) & 0x80000000) == 0)
            {
                goto show_logo_start;
            }
            delay_ms(1);
            loopcnt--;
        }
        //clear watch dog
        act_writel(0x00000001 | (act_readl(WD_CTL)), WD_CTL);
        adfu_launch();
    }
    show_logo_start:
	
    lcd_hardware_init();

    read_res(buffer_addr,0, 40);

    pixcnt = 0x2000;//0x0c00;  //128*64
   
    buff_data_trans(buffer_addr, pixcnt, 0);

    backlight_on_off(TRUE, 0, 0); //enable backlight
    act_writel(act_readl(GPIO_AOUTEN) & 0xfffffffd, GPIO_AOUTEN);//disable speak control??
    act_writel(act_readl(GPIO_ADAT) & 0xfffffffd, GPIO_ADAT);//enable speak control mute
    /*if(((act_readb(DPDMCTRL)) & 0x40) != 0x00)
     {
     act_writel((act_readl(PMUADC_CTL) | (0x01 << 5)), PMUADC_CTL);
     delay_ms(20);
     loopcnt = 5;
     //USB插入时，检测Play键进入ADFU
     while (loopcnt != 0)
     {
     if ((act_readl(SYSTEM_CTL_RTCVDD) & 0x80000000) == 0)
     {
     goto exit_welcome;
     }
     delay_ms(1);
     loopcnt--;
     }
     //disable all intrupt
     //IE0 &= 0x7f;
     act_writel(0x00000001 | (act_readl(WD_CTL)), WD_CTL);
     adfu_launch();
     }
     exit_welcome: return;*/
}

//mcu enter low frequent
void mcu_enter_sleep(void)
{
    act_writel(act_readl(SYSTEM_CTL_RTCVDD) & 0xfffffffe, SYSTEM_CTL_RTCVDD);
    act_writel(act_readl(WD_CTL)|0x08, WD_CTL);
    while(1)
    {
        ;
    }
}

