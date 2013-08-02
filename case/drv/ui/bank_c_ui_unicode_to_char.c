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
 * \file     ui_unicode_to_char.c
 * \brief    unicode转换成内码
 * \author   liutingting
 * \par      GENERAL DESCRIPTION:
 *               实现unicode字符串转换成内码字符串
 * \par      EXTERNALIZED FUNCTIONS:
 *              直接调用SD文件系统接口函数sys_sd_fread和sys_sd_fseek
 * \version  1.0
 * \date  2011/9/05
 *******************************************************************************/
#include "display.h"
#include "ui_driver.h"

const char UniToMBTab[][12] =
{
    "FTBL_GB.$$$", /*简体中文codepage table*/
    "FTBL_B5.$$$", /*繁体中文codepage table*/
    "1252L.TBL", /*英文,西欧语系 codepage table*/
    "FTBL_JP.$$$", /*日文 codepage table*/
    "FTBL_KR.$$$", /*韩文 codepage table*/
    "1251L.TBL", /*俄语语系*/
    "1254L.TBL", /*土耳其语系*/
    "1255L.TBL", /*希伯莱语*/
    "874L.TBL", /*泰语*/
    "1250L.TBL", /*北欧，东欧语系*/
    "1256L.TBL", /*阿拉伯语系*/
    "1253L.TBL" /*希腊语*/
};

bool switch_to_char(uint8 *str, int len, sd_handle fp, uint8 latin_flag);

/*! \cond UI_DRIVER_API */
/******************************************************************************/
/*!
 * \par  Description:
 *    根据语言种类，打开unicode对应的多国语言转换表，查表实现unicode至内码的转换
 * \param[in]    str：字符串首址
 * \param[in]    len：要转换的unicode字符串长度
 * \param[in]    lang_id：语言种类
 * \param[out]   str：转换后的字符串指针
 * \return       是否转换成功
 * \retval       TRUE 转换成功
 * \retval       FALSE 打开转换表失败
 * \ingroup      string
 * \note
 *******************************************************************************/
bool unicode_to_char(uint8 *str, uint16 len, uint8 lang_id)
{
    sd_handle UnitoMBTab_fp;
    uint8 result;
    uint8 table_index = 0;
    uint8 latin_flag = 1; /*拉丁语系标志符*/
    switch (lang_id)
    {
        case CHINESE_SIMPLIFIED:
        table_index = 0;
        latin_flag = 0;
        break;

        case CHINESE_TRADITIONAL:
        table_index = 1;
        latin_flag = 0;
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
        table_index = 3;
        latin_flag = 0;
        break;

        case KOREAN:
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
        latin_flag = 0;
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
    /*打开unicode to char 转换表*/
    UnitoMBTab_fp = sys_sd_fopen(UniToMBTab[table_index]);
    if (UnitoMBTab_fp == NULL)
    {
        return FALSE;
    }
    result = switch_to_char(str, len, UnitoMBTab_fp, latin_flag);
    sys_sd_fclose(UnitoMBTab_fp);
    return TRUE;
}
/*! \endcond */

/*! \cond */
/******************************************************************************/
/*!
 * \par  Description:
 *    unicode字符串转内码字符串
 * \param[in]   str：字符串首址
 * \param[in]   len：要转换的unicode字符串长度
 * \param[in]   fp：unicode多国语言转换表的句柄
 * \param[in]   latin_flag：是否拉丁语系的标志
 * \param[out]   str：转换后的字符串指针
 * \return      是否转换成功
 * \note
 *******************************************************************************/
bool switch_to_char(uint8 *str, int len, sd_handle fp, uint8 latin_flag)
{
    int s_cnt;
    int d_cnt;
    uint16 font_code;

    for (d_cnt = 0, s_cnt = 0; (s_cnt < len) && ((str[s_cnt] != 0) || (str[s_cnt + 1] != 0));s_cnt += 2)
    {
        font_code = str[s_cnt + 1];
        font_code = font_code << 8;
        font_code |= str[s_cnt];
        //非ASCII码的转换
        if (font_code > 127)
        {
            //读转换表获得目标字符集的内码表示
            sys_sd_fseek(fp, SEEK_SET, (uint32) (font_code << 1));
            sys_sd_fread(fp, &font_code, 2);

            str[d_cnt] = *((uint8*) &font_code); //低位      Z80是小端 ，51是大端
            d_cnt++;
            //非拉丁语系的内码用两个字符来表示
            if ((latin_flag == 0) && (font_code > 0x7f)) //两字节码编码转换
            {
                str[d_cnt] = *((uint8*) &font_code + 1); //高位
                d_cnt++;
            }
        }
        else
        {
            str[d_cnt] = str[s_cnt]; //只取低位,获得的是对应字符集的内码
            d_cnt++;
        }
    }
    str[d_cnt] = 0;

    return TRUE;
}
/*! \endcond */

