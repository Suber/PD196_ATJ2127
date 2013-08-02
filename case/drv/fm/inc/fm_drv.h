/*
 ********************************************************************************
 *                       US212A
 *                  fm drv head file
 *
 *                (c) Copyright 2002-2003, Actions Co,Ld.
 *                        All Right Reserved
 *
 * File   : Fm_drv.h
 * By     : Mikeyang
 * Version: 1> v1.00     first version     2003-11-20 9:14
 ********************************************************************************
 ********************************************************************************
 */
#ifndef _FM_DRV_H
#define _FM_DRV_H

#include "psp_includes.h"
#include "fmengine.h"
#include "I2C.h"

/* FM 电源GPIO 定义，与硬件相关，待定*/
#define REGFM_POWER_CTRL      GPIO_ADAT
#define OpenFMPower()   act_writel((act_readl(REGFM_POWER_CTRL)|0x00000040), REGFM_POWER_CTRL)
#define CloseFMPower()   act_writel((act_readl(REGFM_POWER_CTRL)&0xffffffbf), REGFM_POWER_CTRL)

//模拟I2C  延时参数，与主频相关，需传参调整
#define PARAM_DELAY_I2C   20

//清看门狗
#define CLEAR_WATCHDOG()    act_writel((0x00000001|act_readl(WD_CTL)),WD_CTL)

/*驱动公用全局变量声明*/
extern gpio_init_cfg_t *I2C_gpio;
extern uint8 delay_i2c;

/*FM 驱动内部函数声明*/
extern uint8 I2C_Trans_Bytes(uint8 *buf, uint8 length, gpio_init_cfg_t* gpio, uint8* delay);
extern uint8 I2C_Recev_Bytes(uint8 *buf, uint8 address, uint8 length, gpio_init_cfg_t* gpio, uint8* delay);
extern void IIC_Start(gpio_init_cfg_t* gpio, uint8* delay);
extern void IIC_Stop(gpio_init_cfg_t* gpio, uint8* delay);
extern void IIC_WriteByte(uint8 dat, gpio_init_cfg_t* gpio, uint8* delay);
extern uint8 IIC_ReadByte(gpio_init_cfg_t* gpio, uint8* delay);
extern void IIC_Init(gpio_init_cfg_t* gpio, uint8* delay);
extern void IIC_Exit(gpio_init_cfg_t* gpio);
extern void IIC_SendAck(uint8 ack, gpio_init_cfg_t* gpio, uint8* delay);
extern uint8 IIC_GetAck(gpio_init_cfg_t* gpio, uint8* delay);
extern void IIC_SDA_Input_Dis(gpio_init_cfg_t* gpio);
extern void IIC_SCL_Output_Dis(gpio_init_cfg_t* gpio);
extern void IIC_SCL_Input_Dis(gpio_init_cfg_t* gpio);
extern void IIC_SDA_Output_Dis(gpio_init_cfg_t* gpio);

/*FM 驱动对外接口函数声明*/
extern int sFM_Init(uint8 band, uint8 level, uint16 freq)
__FAR__;
extern int sFM_Standby(void* null1, void* null2, void* null3)
__FAR__;
extern int sFM_SetFreq(uint16 freq, void* null2, void* null3)
__FAR__;
extern int sFM_GetStatus(void * pstruct_buf, uint8 mode, void* null3)
__FAR__;
extern int sFM_Mute(FM_MUTE_e mode, void* null2, void* null3)
__FAR__;
extern int sFM_Search(uint16 freq, uint8 direct, void* null3)
__FAR__;
extern int sFM_HardSeek(uint16 freq, uint8 direct, void* null3)
__FAR__;
extern int sFM_SetBand(radio_band_e band, void* null2, void* null3)
__FAR__;
extern int sFM_SetThrod(uint8 level, void* null2, void* null3)
__FAR__;
extern int sFM_BreakSeek(void* null1, void* null2, void* null3)
__FAR__;
extern int sFM_GetHardSeekflag(void* flag, void* null2, void* null3)
__FAR__;
extern int sFM_GetBand(void* band, void* null2, void* null3)
__FAR__;
extern int sFM_GetFreq(void* freq, void* null2, void* null3)
__FAR__;
extern int sFM_GetIntsity(void* intensity, void* null2, void* null3)
__FAR__;
extern int sFM_GetAnten(void* antenna, void* null2, void* null3)
__FAR__;
extern int sFM_GetStereo(void* stereo, void* null2, void* null3)
__FAR__;

#endif /*_FM_DRV_H */

