/*******************************************************************************
 *                              us212A
 *                            Module: Common
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 *
 * History:         
 *      <author>    <time>           <version >             <desc>
 *       lzcai     2011-9-19 21:49     1.0             build this file 
*******************************************************************************/
/*!
 * \file     _miscellaneous.c
 * \brief    common 杂项功能
 * \author   lzcai
 * \par      GENERAL DESCRIPTION:
 *               杂项，包括获取默认 setting_comval 值，背光亮度映射，屏幕方向设置，获取电池电量，等等
 * \par      EXTERNALIZED FUNCTIONS:
 *               
 * \version  1.0
 * \date     2011-9-19
*******************************************************************************/

#include "common_ui.h"

//低电电压 3.3V
#define BATLOW_VEL      0x51

//电池电量等级对应 ADC 参考值
const uint8 battery_grade_vel[BATTERY_GRADE_MAX+1] = 
{
    0,          //填充
    BATLOW_VEL, //低电        返回 0
    0x59,       //<= 3.5V     空格电
    0x5e,       //3.5-3.6V    一格电
    0x60,       //3.6-3.65V   两格电
    0x68,       //3.65-3.85V   三格电；3.85V以上   满格电
};

//背光亮度等级映射表
const uint8 lightness_map_data[VALUE_LIGHTNESS_MAX+1] = 
{
    1, 2, 3, 4, 5, 6, 7, 8, 9, 11, 13, 15
};
/*! \cond COMMON_API */

/******************************************************************************/
/*!                    
 * \par  Description:
 *    获取电池电量，转换为电量等级。
 * \param[in]    now 是否即时获取，即时获取会连续采样7次，然后进行计算
 * \param[out]   none 
 * \return       uint8 返回电池电量等级，1 ~ BATTERY_GRADE_MAX 为正常，0表示低电。
 * \ingroup      misc_func
 * \note 
 * \li  低电检测周期为30秒。
*******************************************************************************/
uint8 com_get_battery_grade(bool now)
{
    uint16 battery_total = 0;
    uint8 i, battery_value;
    
    if(now == TRUE)
    {
        //连续采样7次
        for(i = 0; i < BAT_SAMPLE_TIME; i++)
        {
            g_bat_sample[i] = act_readb(BATADC_DATA);//0-6bit有效
        }
        
        g_sys_counter.battery_counter = 20;//这种方式得到不稳定即时ADC，要紧接下来获取稳定平均ADC
    }
    
    for(i = 0; i < BAT_SAMPLE_TIME; i++)//连续读7次取平均值
    {
        battery_total += g_bat_sample[i];
    }
    battery_value = (uint8)(battery_total/BAT_SAMPLE_TIME);
    if((battery_total%BAT_SAMPLE_TIME) >= BAT_SAMPLE_CARRY)
    {
        battery_value++;
    }
    
    g_bat_index = 0;
    
    for(i = BATTERY_GRADE_MAX; i > 0; i--)
    {
        if(battery_value >= battery_grade_vel[i])
        {
            return i;
        }
    }
    
    return 0;
}

/******************************************************************************/
/*!                    
 * \par  Description:
 *    设置硬件背光对比度，即亮度。
 * \param[in]    lightness 系统背光亮度值
 * \param[out]   none 
 * \return       none
 * \ingroup      misc_func
 * \note 
*******************************************************************************/
void com_set_contrast(uint8 lightness)
{
    uint8 map_value;
    
    if(lightness > VALUE_LIGHTNESS_MAX)
    {
        map_value = 0;
    }
    else
    {
        map_value = lightness_map_data[lightness];
    }
    
    lcd_set_contrast(map_value);
}

/******************************************************************************/
/*!                    
 * \par  Description:
 *    设置 GUI 显示方向（小机正向竖向或旋转方向横向）。
 * \param[in]    direct UI 显示方向，见 gui_direct_e 定义
 * \param[out]   none 
 * \return       none
 * \ingroup      misc_func
 * \note 
*******************************************************************************/
void com_set_gui_direction(gui_direct_e direct)
{
    g_gui_direct = direct;
    if(g_gui_direct == GUI_DIRECT_NORMAL)
    {
        ui_set_screen_direction(0);
    }
    else
    {
        ui_set_screen_direction(1);
    }
}

/******************************************************************************/
/*!                    
 * \par  Description:
 *    获取 GUI 显示方向（小机正向竖向或旋转方向横向）。
 * \param[in]    none
 * \param[out]   none 
 * \return       gui_direct_e
 * \retval           返回 GUI 显示方向，见 gui_direct_e 定义。
 * \ingroup      misc_func
 * \note 
*******************************************************************************/
gui_direct_e com_get_gui_direction(void)
{
    return g_gui_direct;
}

/******************************************************************************/
/*!                    
 * \par  Description:
 *    设置 sys_comval 指针，以便 common 访问系统配置项。
 * \param[in]    comval 全局系统公共变量指针
 * \param[out]   none 
 * \return       none
 * \ingroup      misc_func
 * \note 
 * \li  前台应用在进入时必须调用该接口，否则 common 模块将无法正常运行。
*******************************************************************************/
void com_set_sys_comval(comval_t *comval)
{
    sys_comval = comval;
}

/*! \endcond */
