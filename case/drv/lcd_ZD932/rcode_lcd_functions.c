/*******************************************************************************
 *                              US212A
 *                            Module: lcd driver
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>           <time>          <version >             <desc>
 *       liutingting     2011-09-20 10:00     1.0             build this file
 *******************************************************************************/
/*!
 * \file     _rcode_lcd_functions.c
 * \brief    lcd物理控制器的操作控制模块
 * \author   liutingting
 * \version  1.0
 * \date  2011/9/20
 *******************************************************************************/
#include "lcd_inner.h"
/*! \cond */


void write_command(uint8 cmd)
{
    //RS输出低电平
    reg_writel(reg_readl(EXTMEM_CTL) & (~(0x00000001 << EXTMEM_CTL_RS)), EXTMEM_CTL)
    reg_writel(cmd, EXTMEM_DATA)
    #ifdef __WELCOME__
    welcome_delay_us(3);
    #else
    sys_udelay(3);
    #endif
}
//modify--vzhan
void write_data(uint8 *data_buf, uint16 pix_cnt)
{
    //写data
    uint16 mid_data;
    uint8 merge_two_short_piexl=0;
	uint16 *data_buf_16 = (uint16 *)data_buf;
	uint16 *p_data;
    unsigned int i,j;
    unsigned int m;
    unsigned int k;
	uint8 beginpage,pagenum;
write_command(0xc8);
 	beginpage = region_setwin.y/8 + 0xb0;
	pagenum = beginpage + region_setwin.height/8;

	for(i=beginpage;i<pagenum;i++)
	{
		k=(i-beginpage)*8;
		write_command(i); //写CMD
		write_command(((region_setwin.x & 0xf0)>>4) | 0x10); //写CMD x 的高位
		write_command(region_setwin.x & 0x0f); //写CMD x的低位

		for(j=0;j<region_setwin.width;j++)
		{
		    act_writel(act_readl(EXTMEM_CTL) | (0x00000001 << EXTMEM_CTL_RS), EXTMEM_CTL);
			merge_two_short_piexl=0;

			for(m=0;m<8;m++)
			{	
				p_data = &data_buf_16[(k+m)*region_setwin.width + j];
				mid_data = *p_data;
				
				if(mid_data==0xffff)
					merge_two_short_piexl |= (0x01 << (m));
			}
			act_writel(merge_two_short_piexl, EXTMEM_DATA);	
		}
	}
}

#ifdef __WELCOME__
void welcome_delay_ms(uint8 dly_ms)
{
    uint8 i, j;
    while (dly_ms != 0)
    {
        for (i = 0; i < 20; i++)
        {
            for (j = 0; j < 255; j++)
            {
                ;//空语句，查看lst确定循环体执行周期数
            }
        }
        dly_ms--;
    }
}

void welcome_delay_us(uint8 dly_us)
{
    uint8 i;
    while (dly_us != 0)
    {
        for (i = 0; i < 5; i++)
        {
            ;//空语句，查看lst确定循环体执行周期数
        }
        dly_us--;
    }
}
#endif//__WELCOME__

/*! \endcond */

/*! \cond LCD_DRIVER_API */
/******************************************************************************/
/*!
 * \par  Description:
 *       将buffer中的数据送屏
 * \param[in]   pix_cnt: 要写的像素点个数
 * \param[in]   buff: 写数据的buffer
 * \return      none
 * \ingroup     lcd_write_read
 * \par         exmaple code
 * \code
 *          uint8 *display_buffer = (uint8 *) display_buf_addr;
 *          uint16 pix_cnt = 1024;
 *          lcd_buff_data_trans(display_buffer,pix_cnt);
 * \endcode
 * \note
 *******************************************************************************/
#ifndef PC
void buff_data_trans(uint8 *buff, uint16 pix_cnt, void *null3)
{
    //写屏，传输一个lcd buffer
    store_ce();
    write_data(buff, pix_cnt * 2);
    restore_ce();
}
#endif

/******************************************************************************/
/*!
 * \par  Description:
 *       读屏
 * \param[in]   pix_cnt: 要读取的像素点个数
 * \param[out]  buff: 保存读取的屏幕数据的buffer
 * \return      none
 * \ingroup     lcd_write_read
 * \par         exmaple code
 * \code
 *          uint8 *display_buffer = (uint8 *) display_buf_addr;
 *          uint16 pix_cnt = 1024;
 *          lcd_get_buff_data(display_buffer,pix_cnt);
 * \endcode
 * \note
 *******************************************************************************/
#ifndef PC
//该接口只能读入通过DMA传输像素点数据
//modify--vzhan
void get_buff_data(uint8 *buff, uint16 pix_cnt, void *null3)
{/*
    uint32 cpuclk_ctl, cpuclk_ctl_data; //保存memory 时钟状态
    uint16 i;

    store_ce();
    
    cpuclk_ctl = cpuclk_ctl_data = reg_readl(CPUCLKCTL);
    //CPU CLK 24M; AHB_CLK div 2
    cpuclk_ctl_data &= ((~CPUCLKCTL_CPUCLKSEL_MASK) & (~(0x00000001 << CPUCLKCTL_AHBCLKDIV)));
    cpuclk_ctl_data |= (0x02 << CPUCLKCTL_CPUCLKSEL_SHIFT);
    reg_writel(cpuclk_ctl_data, CPUCLKCTL)
    
    write_command(LCD_CMD_READ);
    reg_writel(reg_readl(EXTMEM_CTL) | (0x00000001 << EXTMEM_CTL_RS), EXTMEM_CTL)
    //先空读一次
    reg_readb(EXTMEM_DATA);
    for (i = 0; i < pix_cnt; i++)
    {
        //EXTMEM_DATA 依次是 red green blue data
        *(uint16 *)buff = ( ((uint16) (reg_readb(EXTMEM_DATA) & 0xf8) << 8) 
                          | ((uint16) (reg_readb(EXTMEM_DATA) & 0xfc) << 3) 
                          | ((uint16) (reg_readb(EXTMEM_DATA) & 0xf8) >> 3) );
        buff += 2;
    }
    reg_writel(cpuclk_ctl, CPUCLKCTL)
    restore_ce();
*/
}
#endif

/*! \endcond */
