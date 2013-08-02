/*******************************************************************************
 *                              US212A
 *                            Module: FM driver
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>           <time>          <version >             <desc>
 *       Mikeyang     2011-09-20 10:00     1.0             build this file
 *******************************************************************************/

#include "fm_interface.h"
#include "fm_drv.h"

gpio_init_cfg_t gpio_i2c[];

/******************************************************************************
 *  FM 各模组驱动代码空间分配说明
 *  1.  驱动作为drv  加载，其非常驻代码和其他驱动一样
 *      可以分为BANKA,  BANKB 代码放在BANK 空间
 *  2.  由于FM 驱动常驻代码和数据，系统未规划固定内存，
 *       且鉴于FM  的特殊性(后台FM  播放时，后台BASAL 及音频解码
 *       等空间空闲可用)，故暂规划
 *  FM 驱动常驻代码空间 SRAM_AP_BACK_CODEC_RCODE_ADDR  0x29000，4K
 *  FM 驱动常驻数据空间 AP_BACK_DATA 1.5k 中前面0.75k 给FM 引擎AP
 *                                                                                     后面0.75K 给FM 驱动
 *******************************************************************************
 **/

/* I2C 配置指针*/
gpio_init_cfg_t *I2C_gpio = (gpio_init_cfg_t *) gpio_i2c;

/* I2C 所用模拟GPIO 信息结构*/
gpio_init_cfg_t gpio_i2c[2] =
{
     //SCL==GPIO_A15
    { GPIO_AINEN, GPIO_AOUTEN, GPIO_ADAT, GPIO_SCL_BIT },
    //SDA==GPIO_A16
    { GPIO_AINEN, GPIO_AOUTEN, GPIO_ADAT, GPIO_SDA_BIT } 
};

/* I2C 操作延时*/
uint8 delay_i2c = PARAM_DELAY_I2C;

/*FM 驱动对外接口函数*/
fm_driver_operations fm_drv_op =
{ (fm_op_func) sFM_Init, (fm_op_func) sFM_Standby, (fm_op_func) sFM_SetFreq,
        (fm_op_func) sFM_GetStatus, (fm_op_func) sFM_Mute, (fm_op_func) sFM_Search,
        (fm_op_func) sFM_HardSeek, (fm_op_func)sFM_SetBand,
        (fm_op_func)sFM_SetThrod, (fm_op_func)sFM_BreakSeek,
        (fm_op_func)sFM_GetHardSeekflag, (fm_op_func)sFM_GetBand,
        (fm_op_func)sFM_GetFreq, (fm_op_func)sFM_GetIntsity,
        (fm_op_func)sFM_GetAnten, (fm_op_func)sFM_GetStereo,
};

