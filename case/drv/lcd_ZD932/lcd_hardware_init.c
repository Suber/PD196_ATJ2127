/*******************************************************************************
 *                              US212A
 *                            Module: lcd driver
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>           <time>          <version >             <desc>
 *       liutingting     2011-09-05 10:00     1.0             build this file
 *******************************************************************************/
/*!
 * \file     lcd_hardware_init.c
 * \brief    lcd 硬件初始化模块
 * \author   liutingting
 * \par      GENERAL DESCRIPTION:
 *               实现lcd 硬件初始化
 * \par      EXTERNALIZED FUNCTIONS:
 *               none
 * \version  1.0
 * \date  2011/9/05
 *******************************************************************************/
#include "lcd_inner.h"

//8BITS LCD  
const uint8 lcd_init_buf[] =
{
    0xfd,0xe2,
    0xfd,0xa2,
    0xfe,0x78,
    0xfd,0xa0,
    0xfd,0xc8,//0xC0,
    0xfd,0xa6,
    0xfd,0x25,
    0xfd,0x81,
    0xfd,0x13,   
    0xfd,0xe5,
    0xfd,0xd2,
    0xfd,0x01,
    0xfd,0x2f,
    0xfd,0xAF,
    0xff
};


/******************************************************************************/
/*!
 * \par  Description:
 *    LCD reset
 * \param[in]   none
 * \param[out]  none
 * \return      none
 * \note
 *******************************************************************************/
void lcd_reset(void)
{
    //enable RST
    reg_writel(act_readl(LCMRST_GIO_EN_REG) | LCMRST_GIO_EN_BIT, LCMRST_GIO_EN_REG)
    //RST=1
    reg_writel(act_readl(LCMRST_GIO_DATA_REG) | LCMRST_SET_BIT, LCMRST_GIO_DATA_REG)
    #ifdef __WELCOME__
    welcome_delay_ms(10);
    #else
    sys_mdelay(10);
    #endif
    //RST=0
    reg_writel(act_readl(LCMRST_GIO_DATA_REG) & LCMRST_CLR_BIT, LCMRST_GIO_DATA_REG)
    #ifdef __WELCOME__
    welcome_delay_ms(15);
    #else
    sys_mdelay(15);
    #endif
    //RST=1
    reg_writel(act_readl(LCMRST_GIO_DATA_REG) | LCMRST_SET_BIT, LCMRST_GIO_DATA_REG)
    #ifdef __WELCOME__
    welcome_delay_ms(120);
    #else
    sys_mdelay(120);
    #endif
}

/******************************************************************************/
/*!
 * \par  Description:
 *    LCD init buf
 * \param[in]   none
 * \param[out]  none
 * \return      none
 * \note
 *******************************************************************************/
void lcd_buf_init(void)
{
    uint8 i = 0;
    store_ce();
    while ((i < sizeof(lcd_init_buf)) && (lcd_init_buf[i] != 0xff))
    {
        if (lcd_init_buf[i] == 0xfd)
        {
            i++;
            if (i < sizeof(lcd_init_buf))
            {
                write_command(lcd_init_buf[i]); //写CMD
            }
        }
        else if (lcd_init_buf[i] == 0xfe)
        {
            i++;
            if (i < sizeof(lcd_init_buf))
            {
                #ifdef __WELCOME__
                welcome_delay_ms(lcd_init_buf[i]);
                #else
                sys_mdelay(lcd_init_buf[i]);
                #endif
            }
        }
        else
        {
            write_data((uint8 *) (&lcd_init_buf[i]), 1);
        }
        i++;
    }
    restore_ce();
}
/******************************************************************************/
/*!
 * \par  Description:
 *    LCD clear screen
 * \param[in]   none
 * \param[out]  none
 * \return      none
 * \note
 *******************************************************************************/
void lcd_clear_screen(void)
{
    uint8 *databuf = (uint8*) 0xbfc28000;
    uint8 loopcnt = 16;
    uint32 everycnt;
    uint16 i;
    everycnt = (uint32) (LCD_WIDTH * LCD_HEIGHT / loopcnt);
    for (i = 0; i < (everycnt * 2); i++)
    {
        databuf[i] = 0x00;
    }
    while (loopcnt > 0)
    {
        buff_data_trans(databuf, (uint16) everycnt, 0);
        loopcnt--;
    }
}

/******************************************************************************/
/*!
 * \par  Description:
 *    LCD物理控制器初始化
 * \param[in]   lcd_mode：数据传输模式，RGB565  或 YUV420
 * \param[out]  none
 * \return      none
 * \note
 *******************************************************************************/
void lcd_clock_init(void)
{
    //enable LCD clock
    reg_writel(act_readl(CLKENCTL) | (0x00000001 << CLKENCTL_LCDCLKEN), CLKENCTL)
    //lcd reset
    reg_writel(act_readl(MRCR) | (0x00000001 << MRCR_LCDReset), MRCR)
    //LCD select CLK24M
    reg_writel(act_readl(LCDCLKCTL) & (~(0x00000001 << LCDCLKCTL_LCDCLKSEL)), LCDCLKCTL)
    //LCD clock div 24M
    reg_writel(act_readl(LCDCLKCTL) & (~LCDCLKCTL_LCDCLKDIV_MASK), LCDCLKCTL)
    //CLKHDU:0;CLKLDU:0; LCD CLK=24M/2=12M
    reg_writel(0x00000000, LCD_CLKCTL)

    //EXCLKH:0;EXCLKL:0; EXTMEM_CLK=AHB_clk/2=6M
    reg_writel(0x00000000, EXTMEM_CLKCTL)
    //enable DMA clock
    reg_writel(act_readl(CLKENCTL) | (0x00000001 << CLKENCTL_DMACLKEN), CLKENCTL)
}

/******************************************************************************/
/*!
 * \par  Description:
 *    LCD 硬件初始化
 * \param[in]   none
 * \param[out]  none
 * \return      none
 * \note
 *******************************************************************************/
void lcd_hardware_init(void)
{
    //cpu clk  24M
    lcd_clock_init();
    lcd_reset(); //LCD RESET
    lcd_buf_init(); //write lcd init buf
    lcd_clear_screen();
}



/*! \endcond */
