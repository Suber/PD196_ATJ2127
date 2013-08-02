/*******************************************************************************
 *                              US212A
 *                            Module: ui driver
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>           <time>          <version >             <desc>
 *       liutingting     2011-09-09 10:00     1.0             build this file
 *******************************************************************************/
/*!
 * \file     ui_arabic_nsm.c
 * \brief    arabic 符号的判断查找处理模块
 * \author   liutingting
 * \par      GENERAL DESCRIPTION:
 *               arabic 符号的判断查找处理
 * \par      EXTERNALIZED FUNCTIONS:
 *               none
 * \version  1.0
 * \date  2011/9/09
 *******************************************************************************/
#include "display.h"
#include "ui_driver.h"

const uint16 nsm_list[] =
{
    0x0610, /* ARABIC SIGN SALLALLAHOU ALAYHE WASSALLAM */
    0x0611, /* ARABIC SIGN ALAYHE ASSALLAM */
    0x0612, /* ARABIC SIGN RAHMATULLAH ALAYHE */
    0x0613, /* ARABIC SIGN RADI ALLAHOU ANHU */
    0x0614, /* ARABIC SIGN TAKHALLUS */
    0x064B, /* ARABIC FATHATAN */
    0x064C, /* ARABIC DAMMATAN */
    0x064D, /* ARABIC KASRATAN */
    0x064E, /* ARABIC FATHA */
    0x064F, /* ARABIC DAMMA */
    0x0650, /* ARABIC KASRA */
    0x0651, /* ARABIC SHADDA */
    0x0652, /* ARABIC SUKUN */
    0x0653, /* ARABIC MADDAH ABOVE */
    0x0654, /* ARABIC HAMZA ABOVE */
    0x0655, /* ARABIC HAMZA BELOW */
    0x0656, /* ARABIC SUBSCRIPT ALEF */
    0x0657, /* ARABIC INVERTED DAMMA */
    0x0658, /* ARABIC MARK NOON GHUNNA */
    0x0659, /* ARABIC ZWARAKAY */
    0x065A, /* ARABIC VOWEL SIGN SMALL V ABOVE */
    0x065B, /* ARABIC VOWEL SIGN INVERTED SMALL V ABOVE */
    0x065C, /* ARABIC VOWEL SIGN DOT BELOW */
    0x065D, /* ARABIC REVERSED DAMMA */
    0x065E, /* ARABIC FATHA WITH TWO DOTS */
    0x0615, /* ARABIC SMALL HIGH TAH */
    0x0670, /* ARABIC LETTER SUPERSCRIPT ALEF */
    0x06D6, /* ARABIC SMALL HIGH LIGATURE SAD WITH LAM WITH ALEF MAKSURA */
    0x06D7, /* ARABIC SMALL HIGH LIGATURE QAF WITH LAM WITH ALEF MAKSURA */
    0x06D8, /* ARABIC SMALL HIGH MEEM INITIAL FORM */
    0x06D9, /* ARABIC SMALL HIGH LAM ALEF */
    0x06DA, /* ARABIC SMALL HIGH JEEM */
    0x06DB, /* ARABIC SMALL HIGH THREE DOTS */
    0x06DC, /* ARABIC SMALL HIGH SEEN */
    0x06DE, /* ARABIC START OF RUB EL HIZB */
    0x06DF, /* ARABIC SMALL HIGH ROUNDED ZERO */
    0x06E0, /* ARABIC SMALL HIGH UPRIGHT RECTANGULAR ZERO */
    0x06E1, /* ARABIC SMALL HIGH DOTLESS HEAD OF KHAH */
    0x06E2, /* ARABIC SMALL HIGH MEEM ISOLATED FORM */
    0x06E3, /* ARABIC SMALL LOW SEEN */
    0x06E4, /* ARABIC SMALL HIGH MADDA */
    0x06E7, /* ARABIC SMALL HIGH YEH */
    0x06E8, /* ARABIC SMALL HIGH NOON */
    0x06EA, /* ARABIC EMPTY CENTRE LOW STOP */
    0x06EB, /* ARABIC EMPTY CENTRE HIGH STOP */
    0x06EC, /* ARABIC ROUNDED HIGH STOP WITH FILLED CENTRE */
    0x06ED, /* ARABIC SMALL LOW MEEM */
};
/*! \cond */
/******************************************************************************/
/*!
 * \par  Description:
 *    arabic_is_nsm
 * \param[in]    c：要查找的arabic 符号
 * \param[out]   none
 * \return       是否是arabic符号，1：查找成功，0：查找失败
 * \note
 *******************************************************************************/
bool arabic_is_nsm(uint16 c)
{
    int16 low = 0;
    int16 high = sizeof(nsm_list) - 1;
    int16 mid;
    mid = (low + high) / 2;
    while (low <= high)
    {
        if (c > nsm_list[mid])
        {
            low = mid + 1;
        }
        else if (c < nsm_list[mid])
        {
            high = mid - 1;
        }
        else
        {
            return 1;
        }
        mid = (low + high) / 2;
    }
    return 0;
}
/*! \endcond */
