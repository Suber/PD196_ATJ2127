/*******************************************************************************
 *                              US212A
 *                            Module: lcd  driver
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>           <time>          <version >             <desc>
 *       liutingting     2011-09-20 10:00     1.0             build this file
 *******************************************************************************/
/*!
 * \file     lcd_functions_1.c
 * \brief    lcd物理控制器的操作控制模块
 * \author   liutingting
 * \version  1.0
 * \date  2011/9/20
 *******************************************************************************/
#include "lcd_inner.h"

//dma mode data
const unsigned short dma_ctl_data[3] = {0x0700, 0x2700, 0x3700};

/*! \cond */
#ifdef _FPGA_
//MFP切到EXT bus
void set_mfp_to_emif(void)
{
    unsigned int mft_ctl0_data, mft_ctl1_data, mft_ctl2_data;
    
    //set EM_RDB to GPIO_A10; set EM_WRB to GPIO_A11; set EM_RS to GPIO_A12; set EM_CEB to GPIO_A7; 
    mft_ctl0_data = reg_readl(MFP_CTL0);
    mft_ctl0_data &= ((~MFP_CTL0_P_GPIOA10_MASK) & (~MFP_CTL0_P_GPIOA11_MASK) & (~MFP_CTL0_P_GPIOA12_MASK)
                     &(~MFP_CTL0_P_GPIOA7_MASK));
    mft_ctl0_data |= ((0x00000001<<MFP_CTL0_P_GPIOA10_SHIFT) | (0x00000001 << MFP_CTL0_P_GPIOA11_SHIFT)
                     |(0x00000001<<(MFP_CTL0_P_GPIOA12_SHIFT + 1)));
    reg_writel(mft_ctl0_data, MFP_CTL0)
    
    //set EM_D7-D0 to GPIO_A30-23
    mft_ctl1_data = reg_readl(MFP_CTL1);
    mft_ctl1_data &= ((~MFP_CTL1_P_GPIOA26_MASK)&(~MFP_CTL1_P_GPIOA25_MASK)
                     &(~MFP_CTL1_P_GPIOA24_MASK)&(~MFP_CTL1_P_GPIOA23_MASK));
    mft_ctl1_data |= ((0x00000001 << MFP_CTL1_P_GPIOA26_SHIFT)|(0x00000001 << MFP_CTL1_P_GPIOA25_SHIFT)
                     |(0x00000001 << MFP_CTL1_P_GPIOA24_SHIFT)|(0x00000001 << MFP_CTL1_P_GPIOA23_SHIFT));
    reg_writel(mft_ctl1_data, MFP_CTL1)
    
    mft_ctl2_data = reg_readl(MFP_CTL2);
    mft_ctl2_data &= ((~MFP_CTL2_P_GPIOA30_MASK)&(~MFP_CTL2_P_GPIOA29_MASK)
                     &(~MFP_CTL2_P_GPIOA28_MASK)&(~MFP_CTL2_P_GPIOA27_MASK));
    mft_ctl2_data |= ((0x00000001 << MFP_CTL2_P_GPIOA30_SHIFT)|(0x00000001 << MFP_CTL2_P_GPIOA29_SHIFT)
                     |(0x00000001 << MFP_CTL2_P_GPIOA28_SHIFT)|(0x00000001 << MFP_CTL2_P_GPIOA27_SHIFT));
    reg_writel(mft_ctl2_data, MFP_CTL2)

}
//MFP切到lcd
void set_mfp_to_lcd(void)
{
    unsigned int mft_ctl0_data, mft_ctl1_data, mft_ctl2_data;
    
    //set LCD_WRB to GPIO_A11; set lCD_RS to GPIO_A12; set lCD_CEB to GPIO_A7
    mft_ctl0_data = reg_readl(MFP_CTL0);
    mft_ctl0_data &= ((~MFP_CTL0_P_GPIOA11_MASK) & (~MFP_CTL0_P_GPIOA12_MASK) & (~MFP_CTL0_P_GPIOA7_MASK));
    mft_ctl0_data |= ((0x00000001<<(MFP_CTL0_P_GPIOA11_SHIFT + 1)) | (0x00000001 << MFP_CTL0_P_GPIOA12_SHIFT)
                     |(0x00000001 << MFP_CTL0_P_GPIOA7_SHIFT));
    reg_writel(mft_ctl0_data, MFP_CTL0)
    
    //set LCD_D7-D0 to GPIO_A30-23
    mft_ctl1_data = reg_readl(MFP_CTL1);
    mft_ctl1_data &= ((~MFP_CTL1_P_GPIOA26_MASK)&(~MFP_CTL1_P_GPIOA25_MASK)
                     &(~MFP_CTL1_P_GPIOA24_MASK)&(~MFP_CTL1_P_GPIOA23_MASK));
    mft_ctl1_data |= ((0x00000001 << (MFP_CTL1_P_GPIOA26_SHIFT + 1))|(0x00000001 << (MFP_CTL1_P_GPIOA25_SHIFT + 1))
                     |(0x00000001 << (MFP_CTL1_P_GPIOA24_SHIFT + 1))|(0x00000001 << (MFP_CTL1_P_GPIOA23_SHIFT + 1)));
    reg_writel(mft_ctl1_data, MFP_CTL1)
    
    mft_ctl2_data = reg_readl(MFP_CTL2);
    mft_ctl2_data &= ((~MFP_CTL2_P_GPIOA30_MASK)&(~MFP_CTL2_P_GPIOA29_MASK)
                     &(~MFP_CTL2_P_GPIOA28_MASK)&(~MFP_CTL2_P_GPIOA27_MASK));
    mft_ctl2_data |= ((0x00000001 << (MFP_CTL2_P_GPIOA30_SHIFT + 1))|(0x00000001 << (MFP_CTL2_P_GPIOA29_SHIFT + 1))
                     |(0x00000001 << (MFP_CTL2_P_GPIOA28_SHIFT + 1))|(0x00000001 << (MFP_CTL2_P_GPIOA27_SHIFT + 1)));
    reg_writel(mft_ctl2_data, MFP_CTL2)
}
#else
//MFP切到EXT bus
void set_mfp_to_emif(void)
{
    unsigned int mft_ctl0_data, mft_ctl1_data, mft_ctl2_data;
    
    //set EM_RDB to GPIO_A10; set EM_WRB to GPIO_A11; set EM_RS to GPIO_A12
    mft_ctl0_data = reg_readl(MFP_CTL0);
    mft_ctl0_data &= ((~MFP_CTL0_P_GPIOA10_MASK) & (~MFP_CTL0_P_GPIOA11_MASK) & (~MFP_CTL0_P_GPIOA12_MASK));
    mft_ctl0_data |= ((0x00000001<<MFP_CTL0_P_GPIOA10_SHIFT) | (0x00000001 << MFP_CTL0_P_GPIOA11_SHIFT)
                     |(0x00000001<<(MFP_CTL0_P_GPIOA12_SHIFT + 1)));
    reg_writel(mft_ctl0_data, MFP_CTL0)
    //set EM_CEB to GPIO_A7
    //reg_writel(reg_readl(MFP_CTL0) & (~MFP_CTL0_P_GPIOA7_MASK), MFP_CTL0);
    
    //set EM_D7-D0 to GPIO_A30-23
    mft_ctl1_data = reg_readl(MFP_CTL1);
    mft_ctl1_data &= ((~MFP_CTL1_P_GPIOA26_MASK)&(~MFP_CTL1_P_GPIOA25_MASK)
                     &(~MFP_CTL1_P_GPIOA24_MASK)&(~MFP_CTL1_P_GPIOA23_MASK));
    mft_ctl1_data |= ((0x00000001 << MFP_CTL1_P_GPIOA26_SHIFT)|(0x00000001 << MFP_CTL1_P_GPIOA25_SHIFT)
                     |(0x00000001 << MFP_CTL1_P_GPIOA24_SHIFT)|(0x00000001 << MFP_CTL1_P_GPIOA23_SHIFT));
    reg_writel(mft_ctl1_data, MFP_CTL1)
    
    mft_ctl2_data = reg_readl(MFP_CTL2);
    mft_ctl2_data &= ((~MFP_CTL2_P_GPIOA30_MASK)&(~MFP_CTL2_P_GPIOA29_MASK)
                     &(~MFP_CTL2_P_GPIOA28_MASK)&(~MFP_CTL2_P_GPIOA27_MASK));
    mft_ctl2_data |= ((0x00000001 << MFP_CTL2_P_GPIOA30_SHIFT)|(0x00000001 << MFP_CTL2_P_GPIOA29_SHIFT)
                     |(0x00000001 << MFP_CTL2_P_GPIOA28_SHIFT)|(0x00000001 << MFP_CTL2_P_GPIOA27_SHIFT));
    reg_writel(mft_ctl2_data, MFP_CTL2)
}
//MFP切到lcd
void set_mfp_to_lcd(void)
{
    unsigned int mft_ctl0_data, mft_ctl1_data, mft_ctl2_data;
    
    //set LCD_WRB to GPIO_A11; set lCD_RS to GPIO_A12
    mft_ctl0_data = reg_readl(MFP_CTL0);
    mft_ctl0_data &= ((~MFP_CTL0_P_GPIOA11_MASK) & (~MFP_CTL0_P_GPIOA12_MASK));
    mft_ctl0_data |= ((0x00000001<<(MFP_CTL0_P_GPIOA11_SHIFT + 1)) | (0x00000001 << MFP_CTL0_P_GPIOA12_SHIFT));
    reg_writel(mft_ctl0_data, MFP_CTL0)
    //set lCD_CEB to GPIO_A7
    //reg_writel((reg_readl(MFP_CTL0) & (~MFP_CTL0_P_GPIOA7_MASK)) | (0x00000001 << MFP_CTL0_P_GPIOA7_SHIFT), MFP_CTL0)

    //set LCD_D7-D0 to GPIO_A30-23
    mft_ctl1_data = reg_readl(MFP_CTL1);
    mft_ctl1_data &= ((~MFP_CTL1_P_GPIOA26_MASK)&(~MFP_CTL1_P_GPIOA25_MASK)
                     &(~MFP_CTL1_P_GPIOA24_MASK)&(~MFP_CTL1_P_GPIOA23_MASK));
    mft_ctl1_data |= ((0x00000001 << (MFP_CTL1_P_GPIOA26_SHIFT + 1))|(0x00000001 << (MFP_CTL1_P_GPIOA25_SHIFT + 1))
                     |(0x00000001 << (MFP_CTL1_P_GPIOA24_SHIFT + 1))|(0x00000001 << (MFP_CTL1_P_GPIOA23_SHIFT + 1)));
    reg_writel(mft_ctl1_data, MFP_CTL1)
    
    mft_ctl2_data = reg_readl(MFP_CTL2);
    mft_ctl2_data &= ((~MFP_CTL2_P_GPIOA30_MASK)&(~MFP_CTL2_P_GPIOA29_MASK)
                     &(~MFP_CTL2_P_GPIOA28_MASK)&(~MFP_CTL2_P_GPIOA27_MASK));
    mft_ctl2_data |= ((0x00000001 << (MFP_CTL2_P_GPIOA30_SHIFT + 1))|(0x00000001 << (MFP_CTL2_P_GPIOA29_SHIFT + 1))
                     |(0x00000001 << (MFP_CTL2_P_GPIOA28_SHIFT + 1))|(0x00000001 << (MFP_CTL2_P_GPIOA27_SHIFT + 1)));
    reg_writel(mft_ctl2_data, MFP_CTL2)
}
#endif

//切换RAM的clock到DMA
void dma_set_RAM_clk(uint8 dma_ram)
{
    //Set DMA clock to CK24M
    //enable dma clock
    //switch (dma_ram)
    //{
    //case LCD_RAM_INDEX://设置RAM0为CPU clock
    //    reg_writel(reg_readl(MEMCLKCTL0) | (0x00000001 << MEMCLKCTL0_RAM0CLKEN), MEMCLKCTL0)
    //    break;
    //case JPEG_RAM_INDEX://设置RAM8为CPU clock
    //    reg_writel(reg_readl(MEMCLKCTL1) & (~(0x00000001 << MEMCLKCTL1_RAM8CLKSEL)), MEMCLKCTL1)
    //    reg_writel(reg_readl(MEMCLKCTL0) | (0x00000001 << MEMCLKCTL0_RAM8CLKEN), MEMCLKCTL0)
    //    break;
    //default:
    //    break;
    //}
    if (dma_ram == JPEG_RAM_INDEX)
    {
        reg_writel(reg_readl(MEMCLKCTL1) & (~(0x00000001 << MEMCLKCTL1_RAM8CLKSEL)), MEMCLKCTL1)
    }

}
/*! \endcond */

/*! \cond LCD_DRIVER_API */
/******************************************************************************/
/*!
 * \par  Description:
 *       RAM8 JRAM5 clock切换到MCU clock
 * \param[in]   none
 * \param[in]   none
 * \return      none
 * \ingroup     lcd_dma_trans
 * \par         exmaple code
 * \code
 *          lcd_mcu_set_JRAM_clk();
 * \endcode
 * \note
 *******************************************************************************/
void mcu_set_JRAM_clk(void *null1, void *null2, void *null3)
{
    unsigned int memclk_ctl0_data, memclk_ctl1_data;
    
    memclk_ctl1 = memclk_ctl1_data = reg_readl(MEMCLKCTL1);
    memclk_ctl1_data &= ((~(0x00000001 << MEMCLKCTL1_RAM8CLKSEL)) & (~MEMCLKCTL1_JRAM5CLKSEL_MASK));
    reg_writel(memclk_ctl1_data, MEMCLKCTL1)
    
    memclk_ctl0_data = reg_readl(MEMCLKCTL0);
    memclk_ctl0_data |= ((0x00000001 << MEMCLKCTL0_RAM8CLKEN) | (0x00000001 << MEMCLKCTL0_JRAM5CLKEN));
    reg_writel(memclk_ctl0_data, MEMCLKCTL0)
}
/******************************************************************************/
/*!
 * \par  Description:
 *       恢复RAM8 JRAM5 clock
 * \param[in]   none
 * \param[in]   none
 * \return      none
 * \ingroup     lcd_dma_trans
 * \par         exmaple code
 * \code
 *          lcd_restore_JRAM_clk();
 * \endcode
 * \note
 *******************************************************************************/
void restore_JRAM_clk(void *null1, void *null2, void *null3)
{
    reg_writel(memclk_ctl1, MEMCLKCTL1)
}
/*! \endcond */

/*! \cond */
#ifdef _FPGA_
//select CE4
void store_ce_gio(void)
{
    reg_writel((reg_readl(EXTMEM_CTL)&(~EXTMEM_CTL_CESEL_MASK))|(EXTMEM_CE_SEL << EXTMEM_CTL_CESEL_SHIFT), EXTMEM_CTL)
}

void restore_ce_gio(void)
{
    //将LCD_CE切到LCD_SEG3,段码屏才会用到该功能
    //reg_writel((reg_readl(MFP_CTL0) & (~MFP_CTL0_P_GPIOA7_MASK)) | (0x00000003 << MFP_CTL0_P_GPIOA7_SHIFT), MFP_CTL0)
    //将外扩总线的CE选为CE3
    reg_writel((reg_readl(EXTMEM_CTL) & (~EXTMEM_CTL_CESEL_MASK)) | (0x00000004 << EXTMEM_CTL_CESEL_SHIFT), EXTMEM_CTL)
}
#else
void store_ce_gio(void)
{
    reg_writel(reg_readl(CE_GIO_EN_REG) | CE_EN_BIT, CE_GIO_EN_REG)
    reg_writel((reg_readl(EXTMEM_CTL)&(~EXTMEM_CTL_CESEL_MASK))|(EXTMEM_CE_SEL << EXTMEM_CTL_CESEL_SHIFT),EXTMEM_CTL)
    reg_writel(reg_readl(CE_GIO_DATA_REG) & CE_CLR_BIT, CE_GIO_DATA_REG)
}

void restore_ce_gio(void)
{
    //将外扩总线的CE选为CE3
    reg_writel((reg_readl(EXTMEM_CTL) & (~EXTMEM_CTL_CESEL_MASK)) | (0x00000004 << EXTMEM_CTL_CESEL_SHIFT), EXTMEM_CTL)
    reg_writel(reg_readl(CE_GIO_DATA_REG) | CE_SET_BIT, CE_GIO_DATA_REG)
    //reg_writel(reg_readl(CE_GIO_EN_REG) & CE_DN_BIT, CE_GIO_EN_REG)
}
#endif
/*! \endcond */

/*! \cond LCD_DRIVER_API */
/******************************************************************************/
/*!
 * \par  Description:
 *       LCD物理控制器初始化
 * \param[in]   lcd_mode：数据传输模式，参见lcd_mode_e定义
 * \param[out]  none
 * \return      none
 * \ingroup     lcd_dma_trans
 * \par         exmaple code
 * \code
 *      例子1: 设置成RGB模式
 *      lcd_controller_init(RGB_MODE);
 *
 *      例子2: 设置成YUV420模式
 *      lcd_controller_init(YUV420_MODE);  
 * \endcode
 * \note
 *******************************************************************************/
void controller_init(lcd_mode_e lcd_mode, void *null2, void *null3)
{
    unsigned int lcd_ctl_data;
    
    trans_mode = lcd_mode;
    //关中断
    //irq_status=sys_local_irq_save();
    
    lcd_ctl_data = reg_readl(LCD_CTL);
    //set source data
    if (lcd_mode == RGB_MODE)
    {
        //reg_writel(reg_readl(LCD_CTL) | (0x00000001 << LCD_CTL_SDT), LCD_CTL)
        lcd_ctl_data |= (0x00000001 << LCD_CTL_SDT);
        //set_dma_ctl(dma_chan, 0x00000700); //normal mode
        //reg_writel(0x00000700, DMA_CHANNEL_BASE + dma_chan * DMA_CHANNEL_LEN + DMA_CTL_OFF)
        //reg_writel(0x00000700, DMA1CTL) //用reg_writel(dma_ctl_data[(uint8)lcd_mode], DMA1CTL)代替
    }
    else //YUV444_MODE & YUV420_MODE
    {
        lcd_ctl_data &= (~(0x00000001 << LCD_CTL_SDT));
    }
    
    //set dma mode
    reg_writel((unsigned int)dma_ctl_data[(uint8)lcd_mode], DMA1CTL)

    //RGB sequence; 8bit; enable DRQ
    lcd_ctl_data &= ((~(0x00000001 << LCD_CTL_SEQ)) & (~(0x00000001 << LCD_CTL_FORMATS)));
    lcd_ctl_data |= ((0x00000001 << LCD_CTL_EMDE));
    reg_writel(lcd_ctl_data, LCD_CTL)

    //clear DMA irq pending bit
    reg_writel((1 << DMAIP_DMA1TCIP) | (1 << DMAIP_DMA1HFIP), DMAIP)
    //sys_local_irq_restore(irq_status); //开中断
}
/*! \endcond */
