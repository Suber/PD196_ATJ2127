/********************************************************************************
*                              NOYA
*                            Module: soft image layer
*                 Copyright(c) 2003-2008 Actions Semiconductor,
*                            All Rights Reserved.
*
* History:
*      <author>    <time>           <version >             <desc>
*       yangrui    2009-11-25          1.0             build this file
*       shaofeng   2010-02-12          v1.1            add lcd/fm/sensor模块使用的GPIO, 并修改各对应驱动模块, 完善注释
*       shaofeng   2010-03-01          v1.2            add sensor_reset/card_detect/usb VBUS供电控制/line_in线检测等模块预留使用的GPIO
*       shaofeng   2010-03-03          v1.3            add earphone与speaker_powerdown功能详细说明
********************************************************************************/
/*!
* \file     gpio_config.h
* \brief    将驱动模块所用到的gpio分配情况统一到一个文件中，在遇到gpio分配变更时易于修改.
*           驱动中请统一使用此文件中定义的宏，新增针对gpio操作的宏请定义到此文件中
* \author   yangrui
* \version 1.0
* \date  2009-11-25
*******************************************************************************/

#ifndef __GPIO_CONFIG_H__
#define __GPIO_CONFIG_H__

#include "libdrv.h"
#include <ucos/multifunc.h>


#endif  /* end __GPIO_CONFIG_H__ */

