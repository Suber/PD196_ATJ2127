/*
 *******************************************************************************
 *                              USDK
 *                             Module: TOOLS AP
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
  * File  :  Solar2Lunar.c
 * Purpose:   完成公历纪年到农历年，节气的转换
 * History:
 *      <johnsen>    <2011-9-25 20:43>           <v1.0 >             <desc>
 *
 *******************************************************************************
 */
/* include file of this application, 本应用的头文件 */
#include "case_include.h"
#include "ap_tools.h"


/*用到的静态变量*/
const uint8 chineseMonths[] =
{
    // 农历月份大小压缩表，两个字节表示一年。两个字节共十六个二进制位数，
    // 前四个位数表示闰月月份，后十二个位数表示十二个农历月份的大小。
    0x00, 0x04, 0xad, 0x08, 0x5a, 0x01, 0xd5, 0x54, 0xb4, 0x09, 0x64, 0x05, 0x59, 0x45, 0x95, 0x0a, 0xa6, 0x04,
    0x55, 0x24, 0xad, 0x08, 0x5a, 0x62, 0xda, 0x04, 0xb4, 0x05, 0xb4, 0x55, 0x52, 0x0d, 0x94, 0x0a, 0x4a, 0x2a,
    0x56, 0x02, 0x6d, 0x71, 0x6d, 0x01, 0xda, 0x02, 0xd2, 0x52, 0xa9, 0x05, 0x49, 0x0d, 0x2a, 0x45, 0x2b, 0x09,
    0x56, 0x01, 0xb5, 0x20, 0x6d, 0x01, 0x59, 0x69, 0xd4, 0x0a, 0xa8, 0x05, 0xa9, 0x56, 0xa5, 0x04, 0x2b, 0x09,
    0x9e, 0x38, 0xb6, 0x08, 0xec, 0x74, 0x6c, 0x05, 0xd4, 0x0a, 0xe4, 0x6a, 0x52, 0x05, 0x95, 0x0a, 0x5a, 0x42,
    0x5b, 0x04, 0xb6, 0x04, 0xb4, 0x22, 0x6a, 0x05, 0x52, 0x75, 0xc9, 0x0a, 0x52, 0x05, 0x35, 0x55, 0x4d, 0x0a,
    0x5a, 0x02, 0x5d, 0x31, 0xb5, 0x02, 0x6a, 0x8a, 0x68, 0x05, 0xa9, 0x0a, 0x8a, 0x6a, 0x2a, 0x05, 0x2d, 0x09,
    0xaa, 0x48, 0x5a, 0x01, 0xb5, 0x09, 0xb0, 0x39, 0x64, 0x05, 0x25, 0x75, 0x95, 0x0a, 0x96, 0x04, 0x4d, 0x54,
    0xad, 0x04, 0xda, 0x04, 0xd4, 0x44, 0xb4, 0x05, 0x54, 0x85, 0x52, 0x0d, 0x92, 0x0a, 0x56, 0x6a, 0x56, 0x02,
    0x6d, 0x02, 0x6a, 0x41, 0xda, 0x02, 0xb2, 0xa1, 0xa9, 0x05, 0x49, 0x0d, 0x0a, 0x6d, 0x2a, 0x09, 0x56, 0x01,
    0xad, 0x50, 0x6d, 0x01, 0xd9, 0x02, 0xd1, 0x3a, 0xa8, 0x05, 0x29, 0x85, 0xa5, 0x0c, 0x2a, 0x09, 0x96, 0x54,
    0xb6, 0x08, 0x6c, 0x09, 0x64, 0x45, 0xd4, 0x0a, 0xa4, 0x05, 0x51, 0x25, 0x95, 0x0a, 0x2a, 0x72, 0x5b, 0x04,
    0xb6, 0x04, 0xac, 0x52, 0x6a, 0x05, 0xd2, 0x0a, 0xa2, 0x4a, 0x4a, 0x05, 0x55, 0x94, 0x2d, 0x0a, 0x5a, 0x02,
    0x75, 0x61, 0xb5, 0x02, 0x6a, 0x03, 0x61, 0x45, 0xa9, 0x0a, 0x4a, 0x05, 0x25, 0x25, 0x2d, 0x09, 0x9a, 0x68,
    0xda, 0x08, 0xb4, 0x09, 0xa8, 0x59, 0x54, 0x03, 0xa5, 0x0a, 0x91, 0x3a, 0x96, 0x04, 0xad, 0xb0, 0xad, 0x04,
    0xda, 0x04, 0xf4, 0x62, 0xb4, 0x05, 0x54, 0x0b, 0x44, 0x5d, 0x52, 0x0a, 0x95, 0x04, 0x55, 0x22, 0x6d, 0x02,
    0x5a, 0x71, 0xda, 0x02, 0xaa, 0x05, 0xb2, 0x55, 0x49, 0x0b, 0x4a, 0x0a, 0x2d, 0x39, 0x36, 0x01, 0x6d, 0x80,
    0x6d, 0x01, 0xd9, 0x02, 0xe9, 0x6a, 0xa8, 0x05, 0x29, 0x0b, 0x9a, 0x4c, 0xaa, 0x08, 0xb6, 0x08, 0xb4, 0x38,
    0x6c, 0x09, 0x54, 0x75, 0xd4, 0x0a, 0xa4, 0x05, 0x45, 0x55, 0x95, 0x0a, 0x9a, 0x04, 0x55, 0x44, 0xb5, 0x04,
    0x6a, 0x82, 0x6a, 0x05, 0xd2, 0x0a, 0x92, 0x6a, 0x4a, 0x05, 0x55, 0x0a, 0x2a, 0x4a, 0x5a, 0x02, 0xb5, 0x02,
    0xb2, 0x31, 0x69, 0x03, 0x31, 0x73, 0xa9, 0x0a, 0x4a, 0x05, 0x2d, 0x55, 0x2d, 0x09, 0x5a, 0x01, 0xd5, 0x48,
    0xb4, 0x09, 0x68, 0x89, 0x54, 0x0b, 0xa4, 0x0a, 0xa5, 0x6a, 0x95, 0x04, 0xad, 0x08, 0x6a, 0x44, 0xda, 0x04,
    0x74, 0x05, 0xb0, 0x25, 0x54, 0x03 
};
const uint8 daysInGregorianMonth[] =
{ 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
const uint8 bigLeapMonthYears[20] =
{ // 大闰月的闰年年份
    6, 14, 19, 25, 33, 36, 38, 41, 44, 52, 55, 79, 117, 136, 147, 150, 155, 158, 185, 193 
};

//uint16 baseYear = 1901;
//uint8 baseMonth = 1;
//uint8 baseDate = 1;
//uint8 baseIndex = 0;
//uint16 baseChineseYear = 4598-1;
//uint8 baseChineseMonth = 11;
//uint8 baseChineseDate = 11;
uint16 gregorianYear;
uint8 gregorianMonth;
uint8 gregorianDate;
uint8 isGregorianLeap;
uint16 chineseYear;
int16 chineseMonth; // 负数表示闰月
uint16 chineseDate;
uint8 sectionalTerm;
uint8 principleTerm;

/*
 *****************************************************************************
 * \par  Description:
 *  普通说明     : 处理闰年信息
 * \param[in]    ：无
 * \param[out]   :
 * \retval       ：
 * \note: 注意事项说明
 *
 ******************************************************************************
 */
uint8 isLeapYear(uint16 y)
{
    uint8 leap_year;
    if((((y % 4) == 0) && ((y % 100) != 0)) || ((y % 400) == 0))
    {
        leap_year = 1;
    }
    else
    {
        leap_year = 0;
    }
    return leap_year;//(((y % 4) == 0) && ((y % 100) != 0) || ((y % 400) == 0));
}
//判断指定year、month中的day天数
uint8 daysInMonth_Internal(uint16 year, uint8 month)
{
    if((month == 2) && (isLeapYear(year)))
    {
        return 29;
    }
    else
    {
        if ((month < 1) || (month > 12)) //QAM
        {
            return 0;
        }
        return daysInGregorianMonth[month - 1];
    }
}

/*
uint8 isGregorianLeapYear(uint16 year)
{
    uint8 isLeap = 0;
    if((year % 4) == 0)
    {
        isLeap = 1;
    }
    if((year % 100) == 0)
    {
        isLeap = 0;
    }
    if((year % 400) == 0)
    {
        isLeap = 1;
    }
    return isLeap;
}
*/
void setGregorian(uint16 y, uint8 m, uint8 d)
{
    gregorianYear = y;
    gregorianMonth = m;
    gregorianDate = d;
    isGregorianLeap = isLeapYear(y);//isGregorianLeapYear(y);
    ///     dayOfYear = dayOfYear(y,m,d);
    ///     dayOfWeek = dayOfWeek(y,m,d);
    chineseYear = 0;
    chineseMonth = 0;
    chineseDate = 0;
    sectionalTerm = 0;
    principleTerm = 0;
}

uint8 daysInGregorianMonth_func(uint16 y, uint8 m)
{
    uint8 d;
    if ((m < 1) || (m > 12)) //QAM
    {
        return 0;
    }
    d = daysInGregorianMonth[m - 1];
    if((m == 2) && (isLeapYear(y)))
    {
        d++; // 公历闰年二月多一天
    }
    return d;
}

uint8 daysInChineseMonth(uint16 y, int16 m)
{
    // 注意：闰月 m < 0
    uint16 index = y - baseChineseYear + baseIndex;
    uint8 v = 0;
    uint8 l = 0;
    uint8 d = 30;
    uint8 i = 0;
    uint16 intex_tmp = 2 * index;
    if ((intex_tmp < 0) || (intex_tmp > 401)) //QAM
    {
        return 0;
    }

    if((1 <= m) && (m <= 8))
    {
        v = chineseMonths[intex_tmp];
        l = m - 1;
        if (((v >> l) & 0x01) == 1)
        {
            d = 29;
        }
    }
    else if((9 <= m) && (m <= 12))
    {
        v = chineseMonths[intex_tmp + 1];
        l = m - 9;
        if (((v >> l) & 0x01) == 1)
        {
            d = 29;
        }
    }
    else
    {
        v = chineseMonths[intex_tmp + 1];
        v = (v >> 4) & 0x0F;
        if (v != abs(m))
        {
            d = 0;
        }
        else
        {
            d = 29;
            for (i = 0; i < sizeof(bigLeapMonthYears); i++)
            {
                if (bigLeapMonthYears[i] == index)
                {
                    d = 30;
                    break;
                }
            }
        }
    }
    return d;
}

//返回可能为负数
int8 nextChineseMonth(uint16 y, int16 m)
{
    uint8 index;
    uint8 v;
    uint8 n = abs(m) + 1;
    if (m > 0)
    {
        index = y - baseChineseYear + baseIndex;
        v = chineseMonths[2 * index + 1];
        v = (v >> 4) & 0x0F;
        if (v == m)
        {
            n = -m;
        }
    }
    if (n == 13)
    {
        n = 1;
    }
    return n;
}

uint8 computeChineseFields(void)
{
    uint16 startYear = baseYear;
    uint8 startMonth = baseMonth;
    uint8 startDate = baseDate;
    uint16 i = 0;
    uint16 daysDiff = 0;
    uint8 lastDate;
    int8 nextMonth;
    if((gregorianYear < 1901) || (gregorianYear > 2100))
    {
        return 1;
    }

    chineseYear = baseChineseYear;
    chineseMonth = baseChineseMonth;
    chineseDate = baseChineseDate;
    // 第二个对应日，用以提高计算效率
    // 公历 2000 年 1 月 1 日，对应农历 4697 年 11 月 25 日

    if (gregorianYear >= 2000)
    {
        startYear = baseYear + 99;
        startMonth = 1;
        startDate = 1;
        chineseYear = baseChineseYear + 99;
        chineseMonth = 11;
        chineseDate = 25;
    }

    for (i = startYear; i < gregorianYear; i++)
    {
        daysDiff += 365;
        if (isLeapYear(i) == 1)
        {
            daysDiff += 1; // leap year
        }
    }
    for (i = startMonth; i < gregorianMonth; i++)
    {
        daysDiff += daysInGregorianMonth_func(gregorianYear, i);
    }
    daysDiff += gregorianDate - startDate;

    chineseDate += daysDiff;
    lastDate = daysInChineseMonth(chineseYear, chineseMonth);
    nextMonth = nextChineseMonth(chineseYear, chineseMonth);
    while (chineseDate > lastDate)
    {
        if (abs(nextMonth) < abs(chineseMonth))
        {
            chineseYear++;
        }
        chineseMonth = nextMonth;
        chineseDate -= lastDate;
        lastDate = daysInChineseMonth(chineseYear, chineseMonth);
        nextMonth = nextChineseMonth(chineseYear, chineseMonth);
    }
    return 0;
}

/*注意str必须已分配空间且str空间足够大*/

/*
 int main(int argc, char* argv[])
 {
 int indexg;
 int indexz;
 //输入阳历，计算阴历

 char str[48];
 getLunarString(str, 2006, 11, 22);
 printf("strlen (str)=%d \n", strlen(str));
 return 0;
 }

 */

