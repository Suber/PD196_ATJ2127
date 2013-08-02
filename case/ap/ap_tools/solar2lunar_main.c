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

const uint16 stemNames[] =
{ S_STEM_1, S_STEM_2, S_STEM_3, S_STEM_4, S_STEM_5, S_STEM_6, S_STEM_7, S_STEM_8, S_STEM_9, S_STEM_10 };
const uint16 branchNames[] =
{
    S_BRANCH1, S_BRANCH2, S_BRANCH3, S_BRANCH4, S_BRANCH5, S_BRANCH6, 
    S_BRANCH7, S_BRANCH8, S_BRANCH9, S_BRANCH10,S_BRANCH11, S_BRANCH12 
};
const uint16 monthNames[] =
{
    S_DAY_1, S_DAY_2, S_DAY_3, S_DAY_4, S_DAY_5, S_DAY_6, S_DAY_7, S_DAY_8, S_DAY_9, S_DAY_10, S_DAY_11, S_DAY_12,
    S_DAY_13, S_DAY_14, S_DAY_15, S_DAY_16, S_DAY_17, S_DAY_18, S_DAY_19, S_DAY_20, S_DAY_21, S_DAY_22, S_DAY_23,
    S_DAY_24, S_DAY_25, S_DAY_26, S_DAY_27, S_DAY_28, S_DAY_29, S_DAY_30, S_DAY_31 
};
const uint16 chineseMonthNames[] =
{
    S_CHINAM_1, S_CHINAM_2, S_CHINAM_3, S_CHINAM_4, S_CHINAM_5, S_CHINAM_6, 
    S_CHINAM_7, S_CHINAM_8, S_CHINAM_9, S_CHINAM_10, S_CHINAM_11, S_CHINAM_12 
};
const uint16 principleTermNames[] =
{
    S_PRINC_1, S_PRINC_2, S_PRINC_3, S_PRINC_4, S_PRINC_5, S_PRINC_6, 
    S_PRINC_7, S_PRINC_8, S_PRINC_9, S_PRINC_10, S_PRINC_11, S_PRINC_12 
};
const uint16 sectionalTermNames[] =
{
    S_SECTI_1, S_SECTI_2, S_SECTI_3, S_SECTI_4, S_SECTI_5, S_SECTI_6, 
    S_SECTI_7, S_SECTI_8, S_SECTI_9, S_SECTI_10, S_SECTI_11, S_SECTI_12 
};
const uint16 animalNames[] =
{
    S_CHINAY_1, S_CHINAY_2, S_CHINAY_3, S_CHINAY_4, S_CHINAY_5, S_CHINAY_6, 
    S_CHINAY_7, S_CHINAY_8, S_CHINAY_9, S_CHINAY_10, S_CHINAY_11, S_CHINAY_12 
};

extern uint16 gregorianYear;
extern uint8 gregorianMonth;
extern uint8 gregorianDate;
extern uint8 isGregorianLeap;
extern uint16 chineseYear;
extern int16 chineseMonth; // 负数表示闰月
extern uint16 chineseDate;
extern uint8 sectionalTerm;
extern uint8 principleTerm;
extern void setGregorian(uint16 y, uint8 m, uint8 d);
extern uint8 computeChineseFields(void);
extern uint8 computeSolarTerms(void);
extern uint8 sectionalTerm_func(uint16 y, uint8 m);
extern uint8 principleTerm_func(uint16 y, uint8 m);
extern uint8 daysInMonth_Internal(uint16 year, uint8 month);

/*
 ********************************************************************************
 * Description : 显示阴历时间，年份，季节等信息
 *
 * Arguments   :
 *
 * Returns     :
 *
 * Notes       :
 *
 ********************************************************************************
 */
void show_LunarString(uint16 id_stem, uint16 id_branch, uint16 id_animal, uint16 id_chineseMonth, uint16 id_day,
        uint16 id_Term)
{
    region_t day_region;
    string_desc_t str_info;
    string_desc_t desc;

    str_info.language = (int8)g_comval.language_id;

    day_region.y = 20;
    day_region.width = 16;
    day_region.height = 16;

    //	ResShowPic(CLDBBA,0,139);
    //	ui_set_textout_mode(TEXT_OUT_NORMAL);
    //	ResShowMultiString(id_stem,g_comval.langid,0,140);
    day_region.x = 0;
    desc.data.id = id_stem;
    desc.language = UNICODEID;
    ui_show_string(&desc, &day_region, DISP_ALIGN_MEDIACY);
    //	ResShowMultiString(id_branch,g_comval.langid,2*8,140);
    day_region.x = 1 * 16;
    desc.data.id = id_branch;
    desc.language = UNICODEID;
    ui_show_string(&desc, &day_region, DISP_ALIGN_MEDIACY);
    //	SetTextPos(4*8,140);
    //	PutS("(", 1);
    day_region.x = 1 * 16 + 8;
    str_info.data.str = "(";
    str_info.length = 1;
    ui_show_string(&str_info, &day_region, DISP_ALIGN_MEDIACY);
    //ui_show_string("(", &day_region, DISP_RIGHT);
    //	SetTextPos(7*8,140);
    //	PutS(")", 1);
    day_region.x = 2 * 16 + 8;
    day_region.y = 20;
    str_info.data.str = ")";
    str_info.length = 1;
    ui_show_string(&str_info, &day_region, DISP_ALIGN_MEDIACY);

    //	ResShowMultiString(id_animal,g_comval.langid,5*8,140);
    day_region.x = 2 * 16;
    desc.data.id = id_animal;
    desc.language = UNICODEID;
    ui_show_string(&desc, &day_region, DISP_ALIGN_MEDIACY);

    //	ResShowMultiString(S_YEAR,g_comval.langid,8*8,140);
    day_region.x = 3 * 16;
    desc.data.id = S_CAL_YR;
    desc.language = UNICODEID;
    ui_show_string(&desc, &day_region, DISP_ALIGN_MEDIACY);

    //	ResShowMultiString(id_chineseMonth,g_comval.langid,10*8,140);
    day_region.x = 4 * 16;
    desc.data.id = id_chineseMonth;
    desc.language = UNICODEID;
    ui_show_string(&desc, &day_region, DISP_ALIGN_MEDIACY);

    //	ResShowMultiString(S_MONTH,g_comval.langid,12*8,140);
    day_region.x = 5 * 16;
    desc.data.id = S_CAL_MN;
    desc.language = UNICODEID;
    ui_show_string(&desc, &day_region, DISP_ALIGN_MEDIACY);
    //	ResShowMultiString(id_day,g_comval.langid,14*8,140);
    day_region.width = 32;

    day_region.x = 6 * 16;
    desc.data.id = id_day;
    desc.language = UNICODEID;
    ui_show_string(&desc, &day_region, DISP_ALIGN_MEDIACY);
    //if (id_Term != 0)
    //{
    //		ResShowMultiString(id_Term,g_comval.language_id,20*8,140);
    //}
}

uint8 computeSolarTerms(void)
{
    if((gregorianYear < 1901) || (gregorianYear > 2100))
    {
        return 1;
    }
    sectionalTerm = (uint8)sectionalTerm_func(gregorianYear, gregorianMonth);
    principleTerm = (uint8)principleTerm_func(gregorianYear, gregorianMonth);
    return 0;
}
/*
 ********************************************************************************
 * Description : 获取当前阴历时间，年份，季节等信息
 *
 * Arguments   :
 *
 * Returns     :
 *
 * Notes       :
 *
 ********************************************************************************
 */
void getLunarString(const date_t *scolar, date_t *lunar, uint8 languageId)
{
    uint16 year = scolar->year;
    uint8 month = scolar->month;
    uint8 day = scolar->day;

    setGregorian(year, month, day);
    computeChineseFields();
    computeSolarTerms();

    lunar->year = chineseYear;
    lunar->month = abs(chineseMonth);
    lunar->day = (uint8)chineseDate;

    switch (languageId)
    {
        case CHINESE_SIMPLIFIED: //简体
        if ((gregorianMonth < 1) || (gregorianMonth > 12)) //QAM
        {
            return;
        }
        if ((abs(chineseMonth) < 1) || (abs(chineseMonth) > 12)) //QAM
        {
            return;
        }
        if ((chineseDate < 1) || (chineseDate > 31)) //QAM
        {
            return;
        }

        if (gregorianDate == principleTerm)
        {
            show_LunarString(stemNames[(chineseYear - 1) % 10], branchNames[(chineseYear - 1) % 12],
                    animalNames[(chineseYear - 1) % 12], chineseMonthNames[abs(chineseMonth) - 1],
                    monthNames[chineseDate - 1], principleTermNames[gregorianMonth - 1]);
        }
        else if (gregorianDate == sectionalTerm)
        {
            show_LunarString(stemNames[(chineseYear - 1) % 10], branchNames[(chineseYear - 1) % 12],
                    animalNames[(chineseYear - 1) % 12], chineseMonthNames[abs(chineseMonth) - 1],
                    monthNames[chineseDate - 1], sectionalTermNames[gregorianMonth - 1]);
        }
        else
        {
            show_LunarString(stemNames[(chineseYear - 1) % 10], branchNames[(chineseYear - 1) % 12],
                    animalNames[(chineseYear - 1) % 12], chineseMonthNames[abs(chineseMonth) - 1],
                    monthNames[chineseDate - 1], 0);
        }
        break;
        default:
        break;
    }
}

/********************************************************************************/
/*!
 * \par  Description: 其他bank代码空间问题移到此处
 * \
 * \get the week of cur year and month and 1st day
 * \param[in]    date_year  data_month
 * \param[out]   none
 * \return       cur_weekday
 * \retval       none
 * \ingroup
 * \note    
 */
/********************************************************************************/
uint8 get_weekofday(uint16 y, uint8 m, uint8 d)
{
    int8 wk; //wk：最后得到的星期
    uint8 ic, ir; //ic：年份的前两位数字; ir年份的后两位

    if (m < 3)
    {
        m += 12;
        y--;
    }
    ic = (uint8) (y / 100);
    ir = (uint8) (y % 100);
    // 使用泰勒公式
    wk = (ic / 4) + ir + (ir / 4) + (26 * (m + 1)) / 10 - 2 * ic + d - 1; // 避免负数
    while (wk >= 7) // 如果求得的数大于7，就减去7的倍数，直到余数小于7为止。
    {
        wk -= 7;
    }
    while (wk < 0)
    {
        wk += 7;
    }
    return wk;
}

/********************************************************************************/
/*!
 * \par  Description:
 * \
 * \循环显示所有日期
 * \param[in]    date  col
 * \param[out]   none
 * \return       none//cur_weekday
 * \retval       none
 * \ingroup
 * \note
 */
/********************************************************************************/
void paintCalendarTable(date_t date, uint8 col)
{
    uint8 i;
    uint8 days;
    uint8 zero_posx = 2, zero_posy = 56;
    uint8 monChar[3];
    uint8 startCol;
    uint8 startRow = 0;
    //    uint8 cur_weekday=0;
    region_t day_region;
    string_desc_t str_info;

    days = (uint8)daysInMonth_Internal(date.year, date.month);
    startCol = col;

    day_region.width = 16;
    day_region.height = 16;

    for (i = 0; i < days; i++)
    {
        //		ui_set_textout_mode(TEXT_OUT_NORMAL);
        if (date.day == (i + 1))
        {
            ui_set_pen_color(Color_BULE_def);
            //			cur_weekday=startCol;
        }
        else
        {
            ui_set_pen_color(Color_WHITE_def);
        }

        //		set_text_pos(8+startCol*31,104-startRow*20);
        day_region.x = zero_posx + startCol * (16 + 2);
        day_region.y = zero_posy + startRow * (16 + 1);
        startCol++;
        if (startCol >= 7)
        {
            startCol = 0;
            startRow++;
        }
        libc_itoa(i + 1, monChar, 2);

        str_info.data.str = monChar;
        //str_info.argv = ISNOT_SELECT;
        str_info.language = (int8)g_comval.language_id;
        str_info.length = 2;

        ui_show_string(&str_info, &day_region, DISP_ALIGN_MEDIACY);
    }
    //	return cur_weekday;
}
