/*
 *******************************************************************************
 *                              USDK
 *                             Module: TOOLS AP
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : menu_callback_calendar_tools.C
 * History:
 *      <johnsen>    <2011-9-25 20:43>           <v1.0 >             <desc>
 *
 *******************************************************************************
 */
/* include file of this application, 本应用的头文件 */
#include "case_include.h"
#include "ap_tools.h"

const char map0[] =
{ 7, 6, 6, 6, 6, 6, 6, 6, 6, 5, 6, 6, 6, 5, 5, 6, 6, 5, 5, 5, 5, 5, 5, 5, 5, 4, 5, 5 };
const char map1[] =
{ 5, 4, 5, 5, 5, 4, 4, 5, 5, 4, 4, 4, 4, 4, 4, 4, 4, 3, 4, 4, 4, 3, 3, 4, 4, 3, 3, 3 };
const char map2[] =
{ 6, 6, 6, 7, 6, 6, 6, 6, 5, 6, 6, 6, 5, 5, 6, 6, 5, 5, 5, 6, 5, 5, 5, 5, 4, 5, 5, 5, 5 };
const char map3[] =
{ 5, 5, 6, 6, 5, 5, 5, 6, 5, 5, 5, 5, 4, 5, 5, 5, 4, 4, 5, 5, 4, 4, 4, 5, 4, 4, 4, 4, 5 };
const char map4[] =
{ 6, 6, 6, 7, 6, 6, 6, 6, 5, 6, 6, 6, 5, 5, 6, 6, 5, 5, 5, 6, 5, 5, 5, 5, 4, 5, 5, 5, 5 };
const char map5[] =
{ 6, 6, 7, 7, 6, 6, 6, 7, 6, 6, 6, 6, 5, 6, 6, 6, 5, 5, 6, 6, 5, 5, 5, 6, 5, 5, 5, 5, 4, 5, 5, 5, 5 };
const char map6[] =
{ 7, 8, 8, 8, 7, 7, 8, 8, 7, 7, 7, 8, 7, 7, 7, 7, 6, 7, 7, 7, 6, 6, 7, 7, 6, 6, 6, 7, 7 };
const char map7[] =
{ 8, 8, 8, 9, 8, 8, 8, 8, 7, 8, 8, 8, 7, 7, 8, 8, 7, 7, 7, 8, 7, 7, 7, 7, 6, 7, 7, 7, 6, 6, 7, 7, 7 };
const char map8[] =
{ 8, 8, 8, 9, 8, 8, 8, 8, 7, 8, 8, 8, 7, 7, 8, 8, 7, 7, 7, 8, 7, 7, 7, 7, 6, 7, 7, 7, 7 };
const char map9[] =
{ 9, 9, 9, 9, 8, 9, 9, 9, 8, 8, 9, 9, 8, 8, 8, 9, 8, 8, 8, 8, 7, 8, 8, 8, 7, 7, 8, 8, 8 };
const char map10[] =
{ 8, 8, 8, 8, 7, 8, 8, 8, 7, 7, 8, 8, 7, 7, 7, 8, 7, 7, 7, 7, 6, 7, 7, 7, 6, 6, 7, 7, 7 };
const char map11[] =
{ 7, 8, 8, 8, 7, 7, 8, 8, 7, 7, 7, 8, 7, 7, 7, 7, 6, 7, 7, 7, 6, 6, 7, 7, 6, 6, 6, 7, 7 };
const char *sectionalTermMap[] =
{ map0, map1, map2, map3, map4, map5, map6, map7, map8, map9, map10, map11 };

const uint8 termyear0[] =
{ 13, 49, 85, 117, 149, 185, 201, 250, 250 };
const uint8 termyear1[] =
{ 13, 45, 81, 117, 149, 185, 201, 250, 250 };
const uint8 termyear2[] =
{ 13, 48, 84, 112, 148, 184, 200, 201, 250 };
const uint8 termyear3[] =
{ 13, 45, 76, 108, 140, 172, 200, 201, 250 };
const uint8 termyear4[] =
{ 13, 44, 72, 104, 132, 168, 200, 201, 250 };
const uint8 termyear5[] =
{ 5, 33, 68, 96, 124, 152, 188, 200, 201 };
const uint8 termyear6[] =
{ 29, 57, 85, 120, 148, 176, 200, 201, 250 };
const uint8 termyear7[] =
{ 13, 48, 76, 104, 132, 168, 196, 200, 201 };
const uint8 termyear8[] =
{ 25, 60, 88, 120, 148, 184, 200, 201, 250 };
const uint8 termyear9[] =
{ 16, 44, 76, 108, 144, 172, 200, 201, 250 };
const uint8 termyear10[] =
{ 28, 60, 92, 124, 160, 192, 200, 201, 250 };
const uint8 termyear11[] =
{ 17, 53, 85, 124, 156, 188, 200, 201, 250 };

const uint8 *sectionalTermYear[] =
{
    termyear0, termyear1, termyear2, termyear3, termyear4, termyear5, 
    termyear6, termyear7, termyear8, termyear9, termyear10, termyear11 
};
const uint8 termmap0[] =
{ 21, 21, 21, 21, 21, 20, 21, 21, 21, 20, 20, 21, 21, 20, 20, 20, 20, 20, 20, 20, 20, 19, 20, 20, 20, 19, 19, 20 };
const uint8 termmap1[] =
{ 20, 19, 19, 20, 20, 19, 19, 19, 19, 19, 19, 19, 19, 18, 19, 19, 19, 18, 18, 19, 19, 18, 18, 18, 18, 18, 18, 18 };
const uint8 termmap2[] =
{ 21, 21, 21, 22, 21, 21, 21, 21, 20, 21, 21, 21, 20, 20, 21, 21, 20, 20, 20, 21, 20, 20, 20, 20, 19, 20, 20, 20, 20 };
const uint8 termmap3[] =
{ 20, 21, 21, 21, 20, 20, 21, 21, 20, 20, 20, 21, 20, 20, 20, 20, 19, 20, 20, 20, 19, 19, 20, 20, 19, 19, 19, 20, 20 };
const uint8 termmap4[] =
{ 21, 22, 22, 22, 21, 21, 22, 22, 21, 21, 21, 22, 21, 21, 21, 21, 20, 21, 21, 21, 20, 20, 21, 21, 20, 20, 20, 21, 21 };
const uint8 termmap5[] =
{ 22, 22, 22, 22, 21, 22, 22, 22, 21, 21, 22, 22, 21, 21, 21, 22, 21, 21, 21, 21, 20, 21, 21, 21, 20, 20, 21, 21, 21 };
const uint8 termmap6[] =
{ 23, 23, 24, 24, 23, 23, 23, 24, 23, 23, 23, 23, 22, 23, 23, 23, 22, 22, 23, 23, 22, 22, 22, 23, 22, 22, 22, 22, 23 };
const uint8 termmap7[] =
{ 23, 24, 24, 24, 23, 23, 24, 24, 23, 23, 23, 24, 23, 23, 23, 23, 22, 23, 23, 23, 22, 22, 23, 23, 22, 22, 22, 23, 23 };
const uint8 termmap8[] =
{ 23, 24, 24, 24, 23, 23, 24, 24, 23, 23, 23, 24, 23, 23, 23, 23, 22, 23, 23, 23, 22, 22, 23, 23, 22, 22, 22, 23, 23 };
const uint8 termmap9[] =
{ 24, 24, 24, 24, 23, 24, 24, 24, 23, 23, 24, 24, 23, 23, 23, 24, 23, 23, 23, 23, 22, 23, 23, 23, 22, 22, 23, 23, 23 };
const uint8 termmap10[] =
{ 23, 23, 23, 23, 22, 23, 23, 23, 22, 22, 23, 23, 22, 22, 22, 23, 22, 22, 22, 22, 21, 22, 22, 22, 21, 21, 22, 22, 22 };
const uint8 termmap11[] =
{ 22, 22, 23, 23, 22, 22, 22, 23, 22, 22, 22, 22, 21, 22, 22, 22, 21, 21, 22, 22, 21, 21, 21, 22, 21, 21, 21, 21, 22 };

const unsigned char * principleTermMap[] =
{ (unsigned char *) termmap0, (unsigned char *) termmap1, (unsigned char *) termmap2, (unsigned char *) termmap3,
        (unsigned char *) termmap4, (unsigned char *) termmap5, (unsigned char *) termmap6, (unsigned char *) termmap7,
        (unsigned char *) termmap8, (unsigned char *) termmap9, (unsigned char *) termmap10,
        (unsigned char *) termmap11 
};

const uint8 ptyear0[] =
{ 13, 45, 81, 113, 149, 185, 201 };
const uint8 ptyear1[] =
{ 21, 57, 93, 125, 161, 193, 201 };
const uint8 ptyear2[] =
{ 21, 56, 88, 120, 152, 188, 200, 201 };
const uint8 ptyear3[] =
{ 21, 49, 81, 116, 144, 176, 200, 201 };
const uint8 ptyear4[] =
{ 17, 49, 77, 112, 140, 168, 200, 201 };
const uint8 ptyear5[] =
{ 28, 60, 88, 116, 148, 180, 200, 201 };
const uint8 ptyear6[] =
{ 25, 53, 84, 112, 144, 172, 200, 201 };
const uint8 ptyear7[] =
{ 29, 57, 89, 120, 148, 180, 200, 201 };
const uint8 ptyear8[] =
{ 17, 45, 73, 108, 140, 168, 200, 201 };
const uint8 ptyear9[] =
{ 28, 60, 92, 124, 160, 192, 200, 201 };
const uint8 ptyear10[] =
{ 16, 44, 80, 112, 148, 180, 200, 201 };
const uint8 ptyear11[] =
{ 17, 53, 88, 120, 156, 188, 200, 201 };
const uint8 *principleTermYear[] =
{ ptyear0, ptyear1, ptyear2, ptyear3, ptyear4, ptyear5, ptyear6, ptyear7, ptyear8, ptyear9, ptyear10, ptyear11 };
//extern uint16 baseYear;


uint8 sectionalTerm_func(uint16 y, uint8 m)
{
    uint8 index;
    uint8 ry;
    uint8 term;
    uint8 array_tmp;
    if((y < 1901) || (y > 2100))
    {
        return 0;
    }
    index = 0;
    ry = y - baseYear + 1;
    if ((m < 1) || (m > 12)) //QAM
    {
        return 0;
    }
    //if ((index < 0) || (index > 8))
    //{
    //    return 0;
    //}
    while (ry >= sectionalTermYear[m - 1][index])
    {
        index++;
    }
    array_tmp = (4 * index) + (ry % 4);
    if ((array_tmp < 0) || (array_tmp > 27)) //QAM
    {
        return 0;
    }
    term = sectionalTermMap[m - 1][array_tmp];
    if ((ry == 121) && (m == 4))
    {
        term = 5;
    }
    if ((ry == 132) && (m == 4))
    {
        term = 5;
    }
    if ((ry == 194) && (m == 6))
    {
        term = 6;
    }
    return term;
}

uint8 principleTerm_func(uint16 y, uint8 m)
{
    uint8 index;
    uint8 ry;
    uint8 term;
    uint8 array_tmp;
    if((y < 1901) || (y > 2100))
    {
        return 0;
    }
    index = 0;
    ry = y - baseYear + 1;
    if ((m < 1) || (m > 12)) //QAM
    {
        return 0;
    }
    //if ((index < 0) || (index > 8))
    //{
    //    return 0;
    //}
    while (ry >= principleTermYear[m - 1][index])
    {
        index++;
    }
    array_tmp = (4 * index) + (ry % 4);
    if ((array_tmp < 0) || (array_tmp > 7)) //QAM
    {
        return 0;
    }
    term = principleTermMap[m - 1][array_tmp];

    if ((ry == 171) && (m == 3))
    {
        term = 21;
    }
    if ((ry == 181) && (m == 5))
    {
        term = 21;
    }
    return term;
}
/*
 ********************************************************************************
 * Description : 显示当前公历的时间
 *
 * Arguments   :
 *
 * Returns     :
 *
 * Notes       :
 *
 ********************************************************************************
 */
/*
 void show_headinfo_line1(date_t *date)
 {
 region_t day_region;
 string_desc_t str_info;

 uint8 strchar[1],str1[30];
 uint8 x_point,y_point;
 char x_offset=0,y_offset=0;

 str_info.language = g_comval.language_id;
 if(g_comval.language_id!=CHINESE_SIMPLIFIED)//>1)
 {
 x_offset=38;
 y_offset=18;
 }
 libc_memcpy(strchar, "-", 1);
 x_point=48-x_offset;
 y_point=158-y_offset;
 day_region.x = x_point;
 day_region.y = y_point;
 //	SetTextPos(x_point,y_point);
 //	itoa_4(date->year, str1);
 libc_itoa(date->year, str1,4);
 //	PutS(str1, sizeof(str1));

 str_info.data.str = str1;
 //str_info.argv = ISNOT_SELECT;
 str_info.length = sizeof(str1);
 ui_show_string(&str_info, &day_region, DISP_MEDIACY);


 x_point=x_point+libc_strlen(str1)*8;
 //	SetTextPos(x_point,y_point);
 //	PutS(strchar, sizeof(strchar));
 day_region.x = x_point;
 //    day_region.y = y_point;

 str_info.data.str = strchar;
 str_info.length = sizeof(strchar);
 ui_show_string(&str_info, &day_region, DISP_MEDIACY);

 x_point=x_point+8;
 //	SetTextPos(x_point,y_point);
 day_region.x = x_point;
 //    day_region.y = y_point;

 if(date->month<=9)
 {
 libc_itoa(date->month, str1,1);
 //		itoa_1(date->month, str1);
 }
 else
 {
 libc_itoa(date->month, str1,2);
 //		itoa_2(date->month, str1);
 }
 //	PutS(str1, sizeof(str1));
 str_info.data.str = str1;
 str_info.length = sizeof(str1);
 ui_show_string(&str_info, &day_region, DISP_MEDIACY);

 x_point=x_point+libc_strlen(str1)*8;
 //	SetTextPos(x_point,y_point);
 //	PutS(strchar, sizeof(strchar));
 day_region.x = x_point;
 //    day_region.y = y_point;
 str_info.data.str = strchar;
 str_info.length = sizeof(strchar);
 ui_show_string(&str_info, &day_region, DISP_MEDIACY);

 x_point=x_point+8;
 //	SetTextPos(x_point,y_point);
 if(date->day<=9)
 {
 libc_itoa(date->day, str1,1);
 //		itoa_1(date->day, str1);
 }
 else
 {
 libc_itoa(date->day, str1,2);
 //		itoa_2(date->day, str1);//廿
 }
 //	PutS(str1, sizeof(str1));
 day_region.x = x_point;
 //    day_region.y = y_point;
 str_info.data.str = str1;
 str_info.length = sizeof(str1);
 ui_show_string(&str_info, &day_region, DISP_MEDIACY);
 }
 */

