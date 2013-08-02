/*******************************************************************************
 *                              us212A
 *                            Module: ui驱动
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved. 
 *
 * History:         
 *      <author>    <time>           <version >             <desc>
 *       lzcai     2011-9-9 16:23     1.0             build this file 
*******************************************************************************/
/*!
 * \file     lcd_driver.h
 * \brief    lcd物理驱动模块相关数据结构，宏定义，接口声明等
 * \author   liutinging
 * \version 1.0
 * \date  2011-9-9
*******************************************************************************/

#ifndef _LCD_DRIVER_H
#define _LCD_DRIVER_H

//#define _DEMO_    /* for demo, enable this macro. complile welcome and lcd_driver.  */

/*! \cond LCD_DRIVER_API */

#include "psp_includes.h"

/*! 是否支持读屏功能 */
#define SUPPORT_READ_CMD

/*!
 * \brief
 *      region_t：区域信息结构体定义
 */
typedef struct
{
    /*! 起始位置X坐标 */
    uint16 x; 
    /*! 起始位置Y坐标 */
    uint16 y; 
    /*! 区域的宽度 */
    uint16 width;
    /*! 区域的高度 */
    uint16 height;
} region_t;
/*!
 * \brief
 *  lcd_cmd_e：lcd驱动命令类型枚举类型
 */
typedef enum
{
    /*!设窗*/
    LCD_SET_WINDOW= 0,
    /*!设置对比度*/
    LCD_SET_CONTRAST,
    /*!设置显示模式*/
    LCD_SET_DRAWMODE,
    /*!屏幕standby控制*/
    LCD_STANDBY_SCREEN,
    /*!开关背光控制*/
    LCD_BACKLIGHT_ONOFF,
    /*!更新指定的屏幕区域*/
    LCD_UPDATE_SCREEN,
    /*!将buffer中的数据送屏*/
    LCD_TRANS_BUFFDATA,
    /*!获取LCD屏上的数据*/
    LCD_GET_BUFFDATA,
    /*!将显示BUFFER中的数据反色*/
    LCD_INVERT_BUFFDATA,
    /*!LCD控制初始化*/
    LCD_CONTROLLER_INIT,
    /*!设置DMA传输的数据宽度*/
    LCD_DMA_SET_COUNTER,
    /*!控制DMA开始传输*/
    LCD_DMA_START_TRANS,
    /*!设置DMA源地址*/
    LCD_DMA_SET_SRC_ADDR,
    /*!设置RAM8的CLK到MCU*/
    LCD_SET_JRAM_CLK,
    /*!恢复原来的CLK*/
    LCD_RESTORE_JRAM_CLK
} lcd_cmd_e;

/*!
 * \brief
 *  draw mode 屏幕物理原点在右下角，逻辑原点在左上角
 */
/*! 水平方向优先（竖屏左上到右下），用于竖屏显示字符串和资源图片，清屏，welcome图片，JPEG，GIF */
#define  DRAW_MODE_H_DEF         0 
/*! 垂直方向优先（横屏左上到右下），用于横屏显示字符串和资源图片，清屏，JPEG，GIF，AVI */
#define  DRAW_MODE_V_DEF         1 
/*! 水平方向优先（竖屏左下到右上），用于竖屏显示BMP */
#define  DRAW_MODE_H_PIC_DEF     2
/*! 垂直方向优先（横屏左下到右上），用于横屏显示BMP，AMV */
#define  DRAW_MODE_V_PIC_DEF     3 
/*! 垂直方向优先（竖屏左上到右下），用于竖屏显示字符串滚屏 */ 
#define  DRAW_MODE_H_SCROLL_DEF  4
/*! 水平方向优先（横屏左上到右下），用于横屏显示字符串滚屏 */ 
#define  DRAW_MODE_V_SCROLL_DEF  5
/*! AVI模式，即垂直方向优先（横屏左上到右下） */
#define  DRAW_MODE_AVI_DEF       DRAW_MODE_V_DEF  

#ifndef PC

#define JPEG_BUF_ADDR 0x32000
#define JPEG_BUF_LEN  0x1800
#define LCD_BUF_ADDR  0x18000//如果字模显示缓冲区超出1KB，请另外指定足够大的空间做缓冲区，并修改 set_display_buf 函数
#define LCD_BUF_LEN    0x400

#else
#define JPEG_BUF_ADDR 0x3000
#define JPEG_BUF_LEN  0x1800
#define LCD_BUF_ADDR  0x3000//修改为0x3000开始，空间为2KB，这样保证在 ui simulator上可以支持 32*32 字库
#define LCD_BUF_LEN    0x800

#endif

#define LCD_RAM_INDEX      0
#define JPEG_RAM_INDEX     1
#define JPEG_CODEC_RAM_INDEX     2


/*!
 * \brief
 *      res_type_e：资源类型数据结构
 */
typedef enum
{
    /*! RGB模式 */
    RGB_MODE = 0, 
    /*! YUV444模式 */
    YUV444_MODE = 1, 
    /*! YUV420模式 */
    YUV420_MODE = 2 
} lcd_mode_e;

/*! LCD显示的长度 */
#define DISPLAY_LENGTH      128
/*! LCD显示的高度 */
#define DISPLAY_HEIGHT     160

//#define _FPGA_
#ifdef _FPGA_
#define LCMRST_GIO_EN_REG        GPIO_AOUTEN    //GPIO_A5 (Output)
#define LCMRST_GIO_DATA_REG      GPIO_ADAT
#define LCMRST_GIO_EN_BIT        (0x00000001<<5)
#define LCMRST_SET_BIT           (0x00000001<<5)
#define LCMRST_CLR_BIT           ~(0x00000001<<5)

#define LCMBL_GIO_EN_REG        GPIO_AOUTEN        //GPIO_A6 (Output)
#define LCMBL_GIO_DATA_REG      GPIO_ADAT
#define LCMBL_GIO_EN_BIT        (0x00000001<<6)
#define LCMBL_SET_BIT           (0x00000001<<6)
#define LCMBL_CLR_BIT           ~(0x00000001<<6)

#else

#define LCMRST_GIO_EN_REG        GPIO_AOUTEN    //GPIO_A15 (Output)
#define LCMRST_GIO_DATA_REG      GPIO_ADAT
#define CE_GIO_EN_REG           GPIO_AOUTEN        //GPIO_A18 CS pin 
#define CE_GIO_DATA_REG         GPIO_ADAT

#ifdef _DEMO_
#define LCMRST_GIO_EN_BIT        (0x00000001<<0)
#define LCMRST_SET_BIT           (0x00000001<<0)
#define LCMRST_CLR_BIT           ~(0x00000001<<0)
#define CE_EN_BIT               (0x00000001<<15)
#define CE_DN_BIT               ~(0x00000001<<15)
#define CE_SET_BIT              (0x00000001<<15)
#define CE_CLR_BIT              ~(0x00000001<<15) 

#else
#define LCMRST_GIO_EN_BIT        (0x00000001<<15)
#define LCMRST_SET_BIT           (0x00000001<<15)
#define LCMRST_CLR_BIT           ~(0x00000001<<15)
#define CE_EN_BIT               (0x00000001<<18)
#define CE_DN_BIT               ~(0x00000001<<18)
#define CE_SET_BIT              (0x00000001<<18)
#define CE_CLR_BIT              ~(0x00000001<<18) 
#endif //_DEMO_

#define LCMBL_GIO_EN_REG        GPIO_AOUTEN        //GPIO_A16 (Output)
#define LCMBL_GIO_DATA_REG      GPIO_ADAT
#define LCMBL_GIO_EN_BIT        (0x00000001<<16)
#define LCMBL_SET_BIT           (0x00000001<<16)
#define LCMBL_CLR_BIT           ~(0x00000001<<16)




#endif

#define EXTMEM_CE_SEL      0x00000005  //CE4


#define reg_writeb(val, reg)  *(volatile unsigned char *)(reg) = (val);
#define reg_writew(val, reg)  *(volatile unsigned short *)(reg) = (val);
#define reg_writel(val, reg)  *(volatile unsigned int *)(reg) = (val);
#define reg_readb(port)      (*(volatile unsigned char *)(port))
#define reg_readw(port)      (*(volatile unsigned short *)(port))
#define reg_readl(port)      (*(volatile unsigned int *)(port))




//display mode

#define RGBFLAG     0x08
#define MNONEFLAG   0x00
#define MXENDFLAG   0x01
#define MYENDFLAG   0x02
#define MVENDFLAG   0x03
//add for less mode 
#define MYREVERSE   0x04
//add for string scroll
#define SCROLLFLAG  0x05

extern uint8 lcd_draw_mode;  //LCD的显示模式
extern int8 reent_flag;    //片选标志
//extern uint32 ce_bak;    //保存片选信息
extern uint32 irq_status;
extern uint8 contrast_grade;  //背光级别
extern uint32 memclk_ctl1;    //保存memory 时钟状态
extern uint8 dma_chan;      //申请的DMA通道号
extern uint8 trans_mode;   //数据传输格式
extern uint8 pic_mode;     //刷屏模式
///对上层统一接口声明
extern void *lcd_op_entry(void *param1,void *param2,void *param3, lcd_cmd_e cmd)__FAR__;

extern void set_window(region_t *rgn,void *null2,void *null3)__FAR__;  //设窗
extern void set_contrast(uint8 contrast_value,void *null2,void *null3);  //设置对比度
extern void set_draw_mode(uint8 mode,void *null2,void *null3)__FAR__;    //设置模式
extern void standby_screen(bool standby_flag,void *null2,void *null3);   //控制LCD的standby
extern void backlight_on_off(bool on_off,void *null2,void *null3)__FAR__;     //开关背光控制
extern void update_screen(region_t *rgn,void*null2,void *null3);   //更新屏幕显示
extern void buff_data_trans(uint8 *buff,uint16 pix_cnt,void *null3)__FAR__;  //将显示buffer中的数据送屏
extern void get_buff_data(uint8 *buff,uint16 pix_cnt,void *null3);   //获取屏上的数据，存放到buffer中
extern void invert_buff_data_trans(uint8 *buff,uint16 pix_cnt,void *null3);  //将buffer中的数据反色
extern void controller_init(lcd_mode_e lcd_mode,void *null2,void *null3)__FAR__;     //LCD控制器初始化
extern void dma_set_counter(uint32 pix_cnt,uint16 data_width,void *null3);   //设置DMA传输的数据长度
extern bool dma_start_trans(uint8 dma_ram, void *null2,void *null3);    //启动DMA传输
extern void dma_set_src_addr(uint32 addr0,uint32 addr1,uint32 addr2)__FAR__;
extern void mcu_set_JRAM_clk(void *null1, void *null2,void *null3);   //将RAM8的clk设为MCU clk
extern void restore_JRAM_clk(void *null1, void *null2,void *null3);  //恢复RAM8的clk

/*!
 * \brief
 *   设窗
 */
#define lcd_set_window(rgn)                          \
lcd_op_entry((void *)(rgn), (void *)(0), (void *)(0), LCD_SET_WINDOW)
/*!
 * \brief
 *   设置对比度
 */
#define lcd_set_contrast(value)                      \
lcd_op_entry((void *)(uint32)(value), (void *)(0), (void *)(0), LCD_SET_CONTRAST)
/*!
 * \brief
 *   设置显示模式
 */
#define lcd_set_draw_mode(mode)                      \
lcd_op_entry((void *)(uint32)(mode), (void *)(0), (void *)(0), LCD_SET_DRAWMODE)
/*!
 * \brief
 *   屏幕standby控制
 */
#define lcd_standby_screen(flag)                     \
lcd_op_entry((void *)(uint32)(flag), (void *)(0), (void *)(0), LCD_STANDBY_SCREEN)
/*!
 * \brief
 *   屏幕背光控制
 */
#define lcd_backlight_on_off(on_off)                 \
lcd_op_entry((void *)(uint32)(on_off), (void *)(0), (void *)(0), LCD_BACKLIGHT_ONOFF)
/*!
 * \brief
 *   更新屏幕区域
 */
#define lcd_update_screen(rgn)                       \
lcd_op_entry((void *)(rgn), (void *)(0), (void *)(0), LCD_UPDATE_SCREEN)
/*!
 * \brief
 *   将buffer中的数据送屏
 */
#define lcd_buff_data_trans(buff,pix_cnt)            \
lcd_op_entry((void *)(buff), (void *)(uint32)(pix_cnt), (void *)(0), LCD_TRANS_BUFFDATA)
/*!
 * \brief
 *   获取LCD屏上的数据
 */
#define lcd_get_buff_data(buff,pix_cnt)              \
lcd_op_entry((void *)(buff), (void *)(uint32)(pix_cnt), (void *)(0), LCD_GET_BUFFDATA)
/*!
 * \brief
 *   将显示BUFFER中的数据反色
 */
#define lcd_invert_buff_data_trans(buff,pix_cnt)     \
lcd_op_entry((void *)(buff), (void *)(uint32)(pix_cnt), (void *)(0), LCD_INVERT_BUFFDATA)
/*!
 * \brief
 *   LCD控制初始化
 */
#define lcd_controller_init(mode)                    \
lcd_op_entry((void *)(uint32)(mode), (void *)(0), (void *)(0), LCD_CONTROLLER_INIT)
/*!
 * \brief
 *   设置DMA传输的数据宽度
 */
#define lcd_dma_set_counter(pix_cnt,data_width)      \
lcd_op_entry((void *)(uint32)(pix_cnt), (void *)(uint32)(data_width), (void *)(0), LCD_DMA_SET_COUNTER)
/*!
 * \brief
 *   控制DMA开始传输
 */
#define lcd_dma_start_trans(dma_ram)                 \
lcd_op_entry((void *)(uint32)(dma_ram), (void *)(0), (void *)(0), LCD_DMA_START_TRANS)
/*!
 * \brief
 *   设置DMA源地址
 */
#define lcd_dma_set_src_addr(addr0,addr1,addr2)      \
lcd_op_entry((void *)(uint32)(addr0), (void *)(uint32)(addr1), (void *)(uint32)(addr2), LCD_DMA_SET_SRC_ADDR)
/*!
 * \brief
 *   设置RAM8的CLK到MCU
 */
#define lcd_mcu_set_JRAM_clk()                       \
lcd_op_entry((void *)(0), (void *)(0), (void *)(0), LCD_SET_JRAM_CLK)
/*!
 * \brief
 *   恢复原来的CLK
 */
#define lcd_restore_JRAM_clk()                       \
lcd_op_entry((void *)(0), (void *)(0), (void *)(0), LCD_RESTORE_JRAM_CLK)


/*! \endcond */

#endif //_LCD_DRIVER_H
