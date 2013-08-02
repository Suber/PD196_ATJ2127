/*******************************************************************************
 *                              US212A
 *                            Module: ui driver
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>           <time>          <version >             <desc>
 *       liutingting     2011-09-05 10:00     1.0             build this file
 *******************************************************************************/
/*!
 * \file     ui_set_language.c
 * \brief    set language语言设置模块
 * \author   liutingting
 * \par      GENERAL DESCRIPTION:
 *               设置语言类型。
 * \par      EXTERNALIZED FUNCTIONS:
 *               直接调用sd文件系统接口sys_sd_fopen
 * \version  1.0
 * \date  2011/9/05
 *******************************************************************************/
#include "display.h"
#include "ui_driver.h"

const uint8 MBtoUnicTabName[][12] =
{
    "V936GBK.TBL", /*简体中文codepage table*/
    "V950BIG.TBL", /*繁体中文codepage table*/
    "V1252.TBL", /*英文,西欧语系 codepage table*/
    "V932JIS.TBL", /*日文 codepage table*/
    "V949KOR.TBL", /*韩文 codepage table*/
    "V1251.TBL", /*俄语语系*/
    "V1254.TBL", /*土耳其语系*/
    "V1255.TBL", /*希伯莱语*/
    "V874.TBL", /*泰语*/
    "V1250.TBL", /*北欧，东欧语系*/
    "V1256.TBL", /*阿拉伯语系*/
    "V1253.TBL" /*希腊语*/
};
/*! \cond UI_DRIVER_API */
/******************************************************************************/
/*!
 * \par  Description:
 *    设置语言种类
 * \param[in]    lang_id：语言类型
 * \param[out]   none
 * \return       TRUE or FALSE，打开多国语言转换表是否成功
 * \ingroup      string
 * \retval       TRUE 打开多国语言转换表成功
 * \retval       FALSE 打开多国语言转换表失败
 * \note
 *******************************************************************************/
bool set_language(uint8 lang_id, void *null2, void *null3)
{
    uint8 table_index = 0;
    language_2byte = 0;
    switch (lang_id)
    {
        case CHINESE_SIMPLIFIED:
        language_2byte = 1;
        table_index = 0;
        break;

        case CHINESE_TRADITIONAL:
        language_2byte = 1;
        table_index = 1;
        break;

        case ENGLISH:
        case FRENCH:
        case GERMAN:
        case ITALIAN:
        case DUTCH:
        case NORWEGIAN:
        case PORTUGUESE_EUROPEAN:
        case PORTUGUESE_BRAZILIAN:
        case SPANISH:
        case SWEDISH:
        case FINNISH:
        case INDONESIAN:
        table_index = 2;
        break;

        case JAPANESE:
        language_2byte = 1;
        table_index = 3;
        break;

        case KOREAN:
        language_2byte = 1;
        table_index = 4;
        break;

        /*case CZECH:
         case POLISH:
         table_index = 5;
         break;*/

        case RUSSIAN:
        table_index = 5;
        break;

        case TURKISH:
        table_index = 6;
        break;

        case HEBREW:
        table_index = 7;
        break;

        case THAI:
        table_index = 8;
        break;

        case CZECH:
        case POLISH:
        case DANISH:
        case HUNGARIAN:
        case SLOVAK:
        case ROMANIAN:
        table_index = 9;
        break;

        case ARABIC:
        table_index = 10;
        break;
        case GREEK:
        table_index = 11;
        break;
        default:
        table_index = 0;
        break;
    }
    if (language_id != lang_id)
    {
        if (g_ap_res.res_fp != NULL)
        {
            /*读取多国语言索引的entry地址*/
            sys_sd_fseek(g_ap_res.res_fp, SEEK_SET, RESHEADITEM * 2);
            sys_sd_fread(g_ap_res.res_fp, &(g_ap_res.string_entry_offset), sizeof(g_ap_res.string_entry_offset));
            //读取当前语言类型的索引entry地址
            sys_sd_fseek(g_ap_res.res_fp, SEEK_SET, g_ap_res.string_entry_offset + (uint32) lang_id * RESHEADITEM);
            sys_sd_fread(g_ap_res.res_fp, &(g_ap_res.string_entry_offset), sizeof(g_ap_res.string_entry_offset));
        }
        if (g_com_res.res_fp != NULL)
        {
            /*读取多国语言索引的entry地址*/
            sys_sd_fseek(g_com_res.res_fp, SEEK_SET, RESHEADITEM * 2);
            sys_sd_fread(g_com_res.res_fp, &(g_com_res.string_entry_offset), sizeof(g_com_res.string_entry_offset));
            //读取当前语言类型的索引entry地址
            sys_sd_fseek(g_com_res.res_fp, SEEK_SET, g_com_res.string_entry_offset + (uint32) lang_id * RESHEADITEM);
            sys_sd_fread(g_com_res.res_fp, &(g_com_res.string_entry_offset), sizeof(g_com_res.string_entry_offset));
        }
    }
    language_id = lang_id;
    if (MBtoUniTab_fp != NULL)
    {
        sys_sd_fclose(MBtoUniTab_fp);
    }
    /*初始化多国语言到Unicode的转换表的句柄 */
    MBtoUniTab_fp = sys_sd_fopen(MBtoUnicTabName[table_index]);

    if (MBtoUniTab_fp == NULL)
    {
        return FALSE;
    }
    return TRUE;
}
/*! \endcond */
