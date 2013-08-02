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
 * \file     lcd_functions.c
 * \brief    实现LCD的对比度调节、standby以及背光开关的模块
 * \author   liutingting
 * \par      GENERAL DESCRIPTION:
 *               实现LCD的对比度调节、standby以及背光开关的模块
 * \par      EXTERNALIZED FUNCTIONS:
 none
 * \version  1.0
 * \date  2011/9/20
 *******************************************************************************/
#include "lcd_inner.h"

/*! \cond LCD_DRIVER_API */
/******************************************************************************/
/*!
 * \par  Description:
 *    设置对比度
 * \param[in]   contrast_value：要设置的对比度的值
 * \param[out]  none
 * \return      none
 * \ingroup     lcd_control
 * \par         exmaple code
 * \code
 *          lcd_set_contrast(15);
 * \endcode
 * \note
 *******************************************************************************/
void set_contrast(uint8 contrast_value, void *null2, void *null3)
{
//    //disable BL
//    reg_writel(reg_readl(LCMBL_GIO_EN_REG) & (~LCMBL_GIO_EN_BIT), LCMBL_GIO_EN_REG)
//    //clr BL
//    reg_writel(reg_readl(LCMBL_GIO_DATA_REG) & LCMBL_CLR_BIT, LCMBL_GIO_DATA_REG)
//    //set PWM to GPIO_A16
//    reg_writel((reg_readl(MFP_CTL1) & (~MFP_CTL1_P_GPIOA16_MASK)) | (0x00000004 << MFP_CTL1_P_GPIOA16_SHIFT), MFP_CTL1)
//    //enable PWM clock
//    reg_writel(reg_readl(CLKENCTL) | (0x00000001 << CLKENCTL_PWMCLKEN), CLKENCTL)
//    // PWM normal
//    reg_writel(reg_readl(MRCR) | (0x00000001 << MRCR_PWMReset), MRCR)
//    //PWM source clk 24M
//    reg_writel(reg_readl(PWMCLKCTL) | (0x00000001 << PWMCLKCTL_PWMCLKSEL), PWMCLKCTL)
//    //PWM CLK 24K
//    //reg_writel((reg_readl(PWMCLKCTL) & (~PWMCLKCTL_PWMCLKDIV_MASK)) | (0x3f << PWMCLKCTL_PWMCLKDIV_SHIFT), PWMCLKCTL)
//    //PWM CLK 96K
//    reg_writel((reg_readl(PWMCLKCTL) & (~PWMCLKCTL_PWMCLKDIV_MASK)) | (0x0f << PWMCLKCTL_PWMCLKDIV_SHIFT), PWMCLKCTL)
//    contrast_grade = contrast_value;
//    reg_writel((reg_readl(PWM_CTL) | (0x01 << PWM_CTL_POL_SEL)), PWM_CTL)
//    reg_writel((reg_readl(PWM_CTL) & (~PWM_CTL_DUTY_MASK)) | contrast_grade, PWM_CTL)
}

/******************************************************************************/
/*!
 * \par  Description:
 *     进入或退出 standby
 * \param[in]   standby_flag:  TRUE: 退出Standby;  FALSE: 进入 Standby
 * \param[out]  none
 * \return      none
 * \ingroup     lcd_control
 * \par         exmaple code
 * \code
 *           lcd_standby_screen(TRUE);
 * \endcode
 * \note
 *******************************************************************************/
void standby_screen(bool standby_flag, void *null2, void *null3)
{
//    uint8 contrast_grade_bk;
//    store_ce();
//    if (standby_flag == TRUE)
//    {
//        #ifdef __WELCOME__
//        welcome_delay_us(200);
//        #else
//        sys_udelay(200);
//        #endif
//        write_command(LCD_CMD_DISPLAY_ON); //电源控制命令
//        #ifdef __WELCOME__
//        welcome_delay_us(200);
//        welcome_delay_us(200);
//        #else
//        sys_udelay(200);
//        sys_udelay(200);
//        #endif
//        write_command(LCD_CMD_EXIT_SLEEP);
//        restore_ce();
//        #ifdef __WELCOME__
//        welcome_delay_ms(200);
//        #else
//        sys_mdelay(200);
//        #endif
//        //重新设置为PWM模式并设置背光亮度
//        backlight_on_off(TRUE, 0, 0);
//        set_contrast(contrast_grade, 0, 0);
//    }
//    else
//    {
//        //进入standby
//        contrast_grade_bk = contrast_grade;
//        set_contrast(0, 0, 0);
//        contrast_grade = contrast_grade_bk;
//        backlight_on_off(FALSE, 0, 0);
//        write_command(LCD_CMD_DISPLAY_OFF);
//        #ifdef __WELCOME__
//        welcome_delay_ms(10);
//        #else
//        sys_mdelay(10);
//        #endif
//        write_command(LCD_CMD_STANDBY);
//        restore_ce();
//    }
}
/******************************************************************************/
/*!
 * \par  Description:
 *     背光开关控制
 * \param[in]   blctl:  TRUE: 开背光;  FALSE: 关背光
 * \param[out]  none
 * \return      none
 * \ingroup     lcd_control
 * \par         exmaple code
 * \code
 *            //开背光
 *           lcd_backlight_on_off(TRUE);
 * \endcode
 * \note
 *******************************************************************************/
void backlight_on_off(bool blctl, void *null2, void *null3)
{
    reg_writel(reg_readl(LCMBL_GIO_EN_REG) | LCMBL_GIO_EN_BIT, LCMBL_GIO_EN_REG)
    if (blctl == TRUE) //开背光
    {
        reg_writel(reg_readl(LCMBL_GIO_DATA_REG) | LCMBL_SET_BIT, LCMBL_GIO_DATA_REG)
    }
    else //关背光
    {
        reg_writel(reg_readl(LCMBL_GIO_DATA_REG) & LCMBL_CLR_BIT, LCMBL_GIO_DATA_REG)
    }
}

/******************************************************************************/
/*!
 * \par  Description:
 *       更新屏幕显示(黑白屏接口)
 * \param[in]   update_region：要更新的屏幕区域
 * \param[out]  none
 * \return      none
 * \ingroup     lcd_write_read
 * \par         exmaple code
 * \code
 *          例子1: 更新指定区域
 *          region_t kregion;
 *          kregion.x = 10;
 *          kregion.y = 20;
 *          kregion.width = 50;
 *          kregion.height = 80;
 *          lcd_update_screen(&kregion);
 *
 *          例子2: 更新全屏
 *          lcd_update_screen(NULL);
 * \endcode
 * \note
 *******************************************************************************/
#ifndef PC
void update_screen(region_t *update_region, void*null2, void *null3)
{

}
#endif

/******************************************************************************/
/*!
 * \par  Description:
 *       将屏幕像素点反色
 * \param[in]   pix_cnt: 要转换的像素点个数
 * \param[out]  buff: 保存转换后的屏幕数据的buffer
 * \return      none
 * \ingroup     lcd_write_read
 * \par         exmaple code
 * \code
 *          uint8 *display_buffer = (uint8 *) display_buf_addr;
 *          uint16 pix_cnt = 1024;
 *          lcd_invert_buff_data_trans(display_buffer,pix_cnt);
 * \endcode
 * \note
 *******************************************************************************/
#ifndef PC
void invert_buff_data_trans(uint8 *buff, uint16 pix_cnt, void *null3)
{
    //消除warning
    *buff = 0;
    pix_cnt = 0;
    return;
}
#endif

/*! \endcond */
