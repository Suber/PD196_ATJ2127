/*******************************************************************************
 *                              US212A
 *                            Module: fm driver
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>           <time>          <version >             <desc>
 *       Mikeyang     2011-09-05 10:00     1.0             build this file
 *******************************************************************************/

#include "fm_drv.h"
#include "I2C.h"

/******************************************************************************/
/*!
 * \par  Description:
 *	  FM 驱动的初始化
 * \param[in]   none
 * \param[out]  none
 * \return      0: success;    -1:  fail
 * \note  驱动初始化函数
 *******************************************************************************/
int fmdrv_init(void* null1, void* null2, void* null3)
{
    //I2C_gpio=(gpio_init_cfg_t *)gpio_i2c;
    return 0;
}

/******************************************************************************/
/*!
 * \par  Description:
 *	  FM 驱动的退出
 * \param[in]   none
 * \param[out]  none
 * \return      0: success;    -1:  fail
 * \note
 *******************************************************************************/
int fmdrv_exit(void* null1, void* null2, void* null3)
{
    return 0;
}

module_init( fmdrv_init)
module_exit(fmdrv_exit)

