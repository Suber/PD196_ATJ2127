/*******************************************************************************
 *                              us212A
 *                            Module: manager
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       lzcai     2011-10-8 11:21     1.0             build this file
 *******************************************************************************/
/*!
 * \file     manager_config.c
 * \brief    应用配置脚本操作接口实现
 * \author   lzcai
 * \par      GENERAL DESCRIPTION:
 *               实现了一套简单的应用配置接口，包括读取配置项等。
 * \par      EXTERNALIZED FUNCTIONS:
 *               直接调用sd 文件系统接口进行定位和读取。
 * \version 1.0
 * \date  2011-10-8
 *******************************************************************************/

#include "common_func.h"
// config 脚本文件名字
const char cfg_file[] = "config.bin";
/******************************************************************************/
/*!
 * \par  Description:
 *	  打开应用配置脚本文件，并读取解释配置文件头
 * \param[in]    config_file：应用配置脚本文件名称
 * \param[out]   none
 * \return       bool
 * \retval           返回打开成功与否，true表示成功，false表示失败
 * \note
 *******************************************************************************/
bool open_config_file(void)
{
    uint8 head[16];

    config_fp = sys_sd_fopen(cfg_file);
    if (config_fp == NULL)
    {
        return FALSE;
    }

    if (sys_sd_fread(config_fp, head, 16) == -1)
    {
        return FALSE;
    }

    if (libc_memcmp(head, "cfg", 3) != 0)
    {
        return FALSE;
    }

    return TRUE;
}

/******************************************************************************/
/*!
 * \par  Description:
 *	  读取应用配置脚本文件的配置参数默认值
 * \param[in]    config_id：配置参数ID
 * \param[out]   none
 * \return       uint16
 * \retval           返回读取到的参数默认值
 * \note         仅用于数值型参数，并由用户保证 config_id 正确
 *******************************************************************************/
uint16 get_config_default(uint16 config_id)
{
    config_index_t temp_index;
    uint16 default_value;

    //读取配置项索引结构体
    if (sys_sd_fseek(config_fp, SEEK_SET, 16 + config_id * sizeof(config_index_t)) == -1)
    {
        return 0;
    }
    if (sys_sd_fread(config_fp, &temp_index, sizeof(config_index_t)) == -1)
    {
        return 0;
    }

    //读取配置项内容
    if (sys_sd_fseek(config_fp, SEEK_SET, temp_index.offset) == -1)
    {
        return 0;
    }
    if (sys_sd_fread(config_fp, &default_value, sizeof(uint16)) == -1)
    {
        return 0;
    }

    return default_value;
}
