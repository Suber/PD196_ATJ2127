/*******************************************************************************
 *                              us212A
 *                            Module: Common
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved. 
 *
 * History:         
 *      <author>    <time>           <version >             <desc>
 *       lzcai     2011-9-16 9:47     1.0             build this file 
*******************************************************************************/
/*!
 * \file     _common_confmenu.c
 * \brief    可配置菜单脚本文件操作接口实现。
 * \author   lzcai
 * \par      GENERAL DESCRIPTION:
 *               实现了一套可配置菜单脚本文件操作接口，包括打开，关闭，读取菜单头，读取菜单项等。
 * \par      EXTERNALIZED FUNCTIONS:
 *               kernel sd_filesystem，等等。
 * \version  1.0
 * \date     2011-9-16
*******************************************************************************/

#include "common_func.h"

/*! \cond COMMON_API */

/******************************************************************************/
/*!                    
 * \par  Description:
 *    打开可配置菜单脚本文件。
 * \param[in]    file_name 可配置菜单脚本文件名字
 * \param[out]   none
 * \return       bool
 * \retval           TRUE 打开文件成功。
 * \retval           FALSE 打开文件失败。
 * \ingroup      common_func
 * \note 
 * \li  该接口在进入前台AP时调用，然后在退出前台AP时再调用 com_close_confmenu_file
 *      关闭，在AP的整个生命周期，该脚本都是可以直接访问的。
*******************************************************************************/
bool com_open_confmenu_file(const char* file_name)
{
    uint8 header[16];
    
    confmenu_fp = sys_sd_fopen(file_name);
    if(confmenu_fp == NULL)
    {
        return FALSE;
    }
    
    if(sys_sd_fread(confmenu_fp, header, sizeof(header)) == -1)
    {
        return FALSE;
    }
    confmenu_offset = ((uint16)header[13] << 8) + header[12];   //低字节在前
    
    return TRUE;
}

/******************************************************************************/
/*!                    
 * \par  Description:
 *    关闭可配置菜单脚本文件。
 * \param[in]    none
 * \param[out]   none    
 * \return       bool
 * \retval           TRUE 关闭文件成功。
 * \retval           FALSE 关闭文件失败。
 * \ingroup      common_func
 * \note 
 * \li  该接口应该在退出前台AP时调用。
*******************************************************************************/
bool com_close_confmenu_file(void)
{
    if(sys_sd_fclose(confmenu_fp) == -1)
    {
        return FALSE;
    }
    
    return TRUE;
}

/******************************************************************************/
/*!                    
 * \par  Description:
 *    读取菜单头数据。
 * \param[in]    menu_id 菜单头ID
 * \param[out]   confmenu_title 菜单头结构体数据缓冲区指针
 * \return       bool
 * \retval           TRUE 读取菜单头成功。
 * \retval           FALSE 读取菜单头失败。
 * \ingroup      common_func
 * \note 
*******************************************************************************/
bool com_get_confmenu_title(uint8 menu_id, menu_title_data_t* confmenu_title)
{
    sys_sd_fseek(confmenu_fp, SEEK_SET, confmenu_offset + menu_id * sizeof(menu_title_data_t));
    return (bool)(sys_sd_fread(confmenu_fp, confmenu_title, sizeof(menu_title_data_t)) != -1);
}

/******************************************************************************/
/*!                    
 * \par  Description:
 *    读取菜单项数据。
 * \param[in]    confmenu_title 菜单头数据结构指针
 * \param[in]    item_index 要读取的当前菜单项序号
 * \param[out]   confmenu_item 菜单项结构体缓冲区指针
 * \return       bool
 * \retval           TRUE 读取菜单项成功。
 * \retval           FALSe 读取菜单项失败。
 * \ingroup      common_func
 * \note 
*******************************************************************************/
bool com_get_confmenu_item(menu_title_data_t* confmenu_title, uint16 item_index, menu_item_data_t *confmenu_item)
{
    sys_sd_fseek(confmenu_fp, SEEK_SET, confmenu_title->offset + sizeof(menu_item_data_t) * item_index);
    return (bool)(sys_sd_fread(confmenu_fp, confmenu_item, sizeof(menu_item_data_t)) != -1);
}

/******************************************************************************/
/*!                    
 * \par  Description:
 *    获取菜单项在当前级菜单中对应的索引号。
 * \param[in]    confmenu_title 菜单头数据结构指针
 * \param[in]    item_str 要检索的菜单项的字符串ID
 * \param[out]   none
 * \return       uint8
 * \retval           返回菜单项在菜单中的索引号，-1表示索引失败。
 * \ingroup      common_func
 * \note 
 * \li  该接口用于确定 RADIO 菜单的 select 项。因为RADIO 菜单要求进入菜单的 active
 *      项为 select 项，而可配置菜单模式下，菜单项的序号是可变的，根本不可能在程序
 *      中指定，所以只能间接的指定菜单项的字符串ID，通过字符串ID去匹配序号。
*******************************************************************************/
uint16 com_get_confmenu_active(menu_title_data_t* confmenu_title, uint16 key_str)
{
    menu_item_data_t temp_data;
    uint16 active;
    
    if(key_str == V_U16_INVALID)
    {
        return V_U16_INVALID;
    }
    
    for(active = 0; active < confmenu_title->count; active++)
    {
        sys_sd_fseek(confmenu_fp, SEEK_SET, confmenu_title->offset + sizeof(menu_item_data_t) * active);
        if(sys_sd_fread(confmenu_fp, &temp_data, sizeof(menu_item_data_t)) == -1)
        {
            return V_U16_INVALID;
        }
        
        if(temp_data.str_id == key_str)
        {
            return active;
        }
    }
    
    return V_U16_INVALID;
}

/******************************************************************************/
/*!                    
 * \par  Description:
 *    获取菜单项在当前级菜单中对应的索引号。
 * \param[in]    confmenu_title 菜单头数据结构指针
 * \param[in]    item_str 要检索的菜单项的字符串ID
 * \param[out]   none
 * \return       uint8
 * \retval           返回菜单项在菜单中的索引号，-1表示索引失败。
 * \ingroup      common_func
 * \note 
 * \li  该接口用于确定 RADIO 菜单的 select 项。因为RADIO 菜单要求进入菜单的 active
 *      项为 select 项，而可配置菜单模式下，菜单项的序号是可变的，根本不可能在程序
 *      中指定，所以只能间接的指定菜单项的字符串ID，通过字符串ID去匹配序号。
*******************************************************************************/
uint16 com_get_menu_active(uint8 menu_id, uint16 key_str)
{
    menu_title_data_t temp_title;
    menu_item_data_t temp_data;
    uint16 active;
    
    com_get_confmenu_title(menu_id, &temp_title);
    
    if(key_str == V_U16_INVALID)
    {
        return V_U16_INVALID;
    }
    
    for(active = 0; active < temp_title.count; active++)
    {
        sys_sd_fseek(confmenu_fp, SEEK_SET, temp_title.offset + sizeof(menu_item_data_t) * active);
        if(sys_sd_fread(confmenu_fp, &temp_data, sizeof(menu_item_data_t)) == -1)
        {
            return V_U16_INVALID;
        }
        
        if(temp_data.str_id == key_str)
        {
            return active;
        }
    }
    
    return V_U16_INVALID;
}

/*! \endcond */
