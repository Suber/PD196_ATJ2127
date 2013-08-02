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
 * \file     ui_arabic_ligature.c
 * \brief    处理Arabic连写模块
 * \author   liutingting
 * \par      GENERAL DESCRIPTION:
 *               处理Arabic连写
 * \par      EXTERNALIZED FUNCTIONS:
 *              none
 * \version  1.0
 * \date  2011/9/09
 *******************************************************************************/
#include "display.h"
#include "ui_driver.h"
//for arabic languange
/*********************************************************************
 *
 *       Types
 *
 **********************************************************************
 */
typedef struct
{
    uint16 isolated;
    uint16 final;
    uint16 initial;
    uint16 medial;
} key_info_t;

const key_info_t arabic_key_info[] =
{
    /*       Base    Isol.   final   initial medial */
    { /* 0  0x0621 */0xFE80, 0x0000, 0x0000, 0x0000 /* Arabic letter Hamza                 */},
    { /* 1  0x0622 */0xFE81, 0xFE82, 0x0000, 0x0000 /* Arabic letter Alef with Madda above */},
    { /* 2  0x0623 */0xFE83, 0xFE84, 0x0000, 0x0000 /* Arabic letter Alef with Hamza above */},
    { /* 3  0x0624 */0xFE85, 0xFE86, 0x0000, 0x0000 /* Arabic letter Waw with Hamza above  */},
    { /* 4  0x0625 */0xFE87, 0xFE88, 0x0000, 0x0000 /* Arabic letter Alef with Hamza below */},
    { /* 5  0x0626 */0xFE89, 0xFE8A, 0xFE8B, 0xFE8C /* Arabic letter Yeh with Hamza above  */},
    { /* 6  0x0627 */0xFE8D, 0xFE8E, 0x0000, 0x0000 /* Arabic letter Alef                  */},
    { /* 7  0x0628 */0xFE8F, 0xFE90, 0xFE91, 0xFE92 /* Arabic letter Beh                   */},
    { /* 8  0x0629 */0xFE93, 0xFE94, 0x0000, 0x0000 /* Arabic letter Teh Marbuta           */},
    { /* 9  0x062A */0xFE95, 0xFE96, 0xFE97, 0xFE98 /* Arabic letter Teh                   */},
    { /* 10 0x062B */0xFE99, 0xFE9A, 0xFE9B, 0xFE9C /* Arabic letter Theh                  */},
    { /* 11 0x062C */0xFE9D, 0xFE9E, 0xFE9F, 0xFEA0 /* Arabic letter Jeem                  */},
    { /* 12 0x062D */0xFEA1, 0xFEA2, 0xFEA3, 0xFEA4 /* Arabic letter Hah                   */},
    { /* 13 0x062E */0xFEA5, 0xFEA6, 0xFEA7, 0xFEA8 /* Arabic letter Khah                  */},
    { /* 14 0x062F */0xFEA9, 0xFEAA, 0x0000, 0x0000 /* Arabic letter Dal                   */},
    { /* 15 0x0630 */0xFEAB, 0xFEAC, 0x0000, 0x0000 /* Arabic letter Thal                  */},
    { /* 16 0x0631 */0xFEAD, 0xFEAE, 0x0000, 0x0000 /* Arabic letter Reh                   */},
    { /* 17 0x0632 */0xFEAF, 0xFEB0, 0x0000, 0x0000 /* Arabic letter Zain                  */},
    { /* 18 0x0633 */0xFEB1, 0xFEB2, 0xFEB3, 0xFEB4 /* Arabic letter Seen                  */},
    { /* 19 0x0634 */0xFEB5, 0xFEB6, 0xFEB7, 0xFEB8 /* Arabic letter Sheen                 */},
    { /* 20 0x0635 */0xFEB9, 0xFEBA, 0xFEBB, 0xFEBC /* Arabic letter Sad                   */},
    { /* 21 0x0636 */0xFEBD, 0xFEBE, 0xFEBF, 0xFEC0 /* Arabic letter Dad                   */},
    { /* 22 0x0637 */0xFEC1, 0xFEC2, 0xFEC3, 0xFEC4 /* Arabic letter Tah                   */},
    { /* 23 0x0638 */0xFEC5, 0xFEC6, 0xFEC7, 0xFEC8 /* Arabic letter Zah                   */},
    { /* 24 0x0639 */0xFEC9, 0xFECA, 0xFECB, 0xFECC /* Arabic letter Ain                   */},
    { /* 25 0x063A */0xFECD, 0xFECE, 0xFECF, 0xFED0 /* Arabic letter Ghain                 */},
    { /* 26 0x0641 */0xFED1, 0xFED2, 0xFED3, 0xFED4 /* Arabic letter Feh                   */},
    { /* 27 0x0642 */0xFED5, 0xFED6, 0xFED7, 0xFED8 /* Arabic letter Qaf                   */},
    { /* 28 0x0643 */0xFED9, 0xFEDA, 0xFEDB, 0xFEDC /* Arabic letter Kaf                   */},
    { /* 29 0x0644 */0xFEDD, 0xFEDE, 0xFEDF, 0xFEE0 /* Arabic letter Lam                   */},
    { /* 30 0x0645 */0xFEE1, 0xFEE2, 0xFEE3, 0xFEE4 /* Arabic letter Meem                  */},
    { /* 31 0x0646 */0xFEE5, 0xFEE6, 0xFEE7, 0xFEE8 /* Arabic letter Noon                  */},
    { /* 32 0x0647 */0xFEE9, 0xFEEA, 0xFEEB, 0xFEEC /* Arabic letter Heh                   */},
    { /* 33 0x0648 */0xFEED, 0xFEEE, 0x0000, 0x0000 /* Arabic letter Waw                   */},
    { /* 34 0x0649 */0xFEEF, 0xFEF0, 0x0000, 0x0000 /* Arabic letter Alef Maksura          */},
    { /* 35 0x064A */0xFEF1, 0xFEF2, 0xFEF3, 0xFEF4 /* Arabic letter Yeh                   */},
    { /* 36 0x067E */0xFB56, 0xFB57, 0xFB58, 0xFB59 /* Eastern arabic letter Peh           */},
    { /* 37 0x0686 */0xFB7A, 0xFB7B, 0xFB7C, 0xFB7D /* Eastern arabic letter Tcheh         */},
    { /* 38 0x0698 */0xFB8A, 0xFB8B, 0x0000, 0x0000 /* Eastern arabic letter Jeh           */},
    { /* 39 0x06A9 */0xFB8E, 0xFB8F, 0xFB90, 0xFB91 /* Eastern arabic letter Keheh         */},
    { /* 40 0x06AF */0xFB92, 0xFB93, 0xFB94, 0xFB95 /* Eastern arabic letter Gaf           */},
    { /* 41 0x06CC */0xFBFC, 0xFBFD, 0xFBFE, 0xFBFF /* Eastern arabic letter Farsi Yeh     */},
};
/*! \cond */
/******************************************************************************/
/*!
 * \par  Description:
 *    get table index
 * \param[in]   c：字符编码值
 * \param[out]  none
 * \return      返回table index
 * \note
 *******************************************************************************/
int16 get_table_index(uint16 c)
{
    if (c < 0x621)
    {
        return 0;
    }
    if (c > 0x6cc)
    {
        return 0;
    }
    if (c <= 0x63a)
    {
        return c - 0x621;
    }
    if ((c >= 0x641) && (c <= 0x64a))
    {
        return c - 0x641 + 26;
    }
    if (c == 0x67e)
    {
        return 36;
    }
    if (c == 0x686)
    {
        return 37;
    }
    if (c == 0x698)
    {
        return 38;
    }
    if (c == 0x6a9)
    {
        return 39;
    }
    if (c == 0x6af)
    {
        return 40;
    }
    if (c == 0x6cc)
    {
        return 41;
    }
    return 0;
}

/******************************************************************************/
/*!
 * \par  Description:
 *    get ligature
 * \param[in]   code：当前字符编码值
 * \param[in]   next：下一个字符编码值
 * \param[in]   prev_valid_joining：上一个是否连写
 * \param[out]  none
 * \return      返回当前字符的连写格式
 * \note
 *******************************************************************************/
int16 get_ligature(uint16 code, uint16 next, int16 prev_valid_joining)
{
    if (((next != 0x622) && (next != 0x623) && (next != 0x625) && (next != 0x627)) || (code != 0x644))
    {
        return 0;
    }
    if (prev_valid_joining != 0)
    {
        switch (next)
        {
            case 0x622:
            return 0xfef6;
            case 0x623:
            return 0xfef8;
            case 0x625:
            return 0xfefa;
            case 0x627:
            return 0xfefc;
            default:
            break;
        }
    }
    else
    {
        switch (next)
        {
            case 0x622:
            return 0xfef5;
            case 0x623:
            return 0xfef7;
            case 0x625:
            return 0xfef9;
            case 0x627:
            return 0xfefb;
            default:
            break;
        }
    }
    return 0;
}


/*********************************************************************
 *
 *       get_presentation_form
 *
 * Purpose:
 *   Returns the Arabic presentation form of the given character(s).
 *   Depending on their position in the text the presentation form of
 *   Arabic characters differr from their character codes. So the function
 *   needs the previous and the next character code to calculate the right
 *   representation form.
 *   Some sequences of 2 characters are formed to one ligature. In this
 *   case the routine uses the pointer pIgnore_next_character to inform the
 *   calling routine that the next character should be ignored.
 *
 * Parameters:
 *   Char                 - Current character
 *   next                 - Previous character
 *   Prev                 - next character following the current character
 *   pIgnore_next_character - Pointer to inform calling routine that the
 *                          next character should be ignored.
 *   s                    - Pointer for decoding text data. Need if 'next' is a Non Spacing Mark.
 **************************************************************************************************/
uint16 get_presentation_form(uint16 code, uint16 next, uint16 prev, int16* pIgnore_next, uint16 n_next)
{
    int16 code_index, next_valid_joining, prev_valid_joining, final, initial, medial, ligature;
    code_index = get_table_index(code);
    if (!code_index)
    {
        if (!arabic_is_nsm(code))
        {
            prev_valid_next_joining = 0;
        }
        return code;
    }
    prev_valid_joining = prev_valid_next_joining;
    ligature = get_ligature(code, next, prev_valid_joining);
    if (!ligature)
    {
        ligature = get_ligature(prev, code, prev_valid_joining);
    }
    if (ligature != 0)
    {
        if (pIgnore_next != 0)
        {
            *pIgnore_next = 1;
        }
        prev_valid_next_joining = 0;
        return ligature;
    }
    else
    {
        if (pIgnore_next != 0)
        {
            *pIgnore_next = 0;
        }
    }
    if (arabic_is_nsm(next) != 0)
    {
        if (code != 0)
        {
            next = n_next;
        }
    }

    next_valid_joining = (int16) ((get_table_index(next)) && (arabic_key_info[code_index].medial));
    code = arabic_key_info[code_index].isolated;
    prev_valid_next_joining = (uint8) next_valid_joining;
    if ((!prev_valid_joining) && (!next_valid_joining))
    {
        code = (uint16) arabic_key_info[code_index].isolated;
        //prev_valid_next_joining = 0;
    }
    else if ((!prev_valid_joining) && (next_valid_joining))
    {
        initial = (int16) arabic_key_info[code_index].initial;
        //prev_valid_next_joining = 1;
        if (initial != 0)
        {
            code = (uint16) initial;
        }
        /*else
         {
         code = arabic_key_info[code_index].isolated;
         }*/

    }
    else if ((prev_valid_joining) && (next_valid_joining))
    {
        medial = (int16) arabic_key_info[code_index].medial;
        //prev_valid_next_joining = 1;
        if (medial != 0)
        {
            code = (uint16) medial;
        }
        /*else
         {
         code = arabic_key_info[code_index].isolated;
         }*/
    }
    else if ((prev_valid_joining) && (!next_valid_joining))
    {
        final = (int16) arabic_key_info[code_index].final;
        //prev_valid_next_joining = 0;
        if (final != 0)
        {
            code = (uint16) final;
        }
        /*else
         {
         code = arabic_key_info[code_index].isolated;
         }*/
    }
    else
    {
        //do nothing for QAC
    }
    return code;
}

/******************************************************************************/
/*!
 * \par  Description:
 *    unicode to arabic
 * \param[in]   str：字符串指针
 * \param[in]   length：字符串长度
 * \param[out]  none
 * \return      返回Arabic连写长度
 * \note
 *******************************************************************************/
uint8 arabic_do_join(string_desc_t *desc)
{
    int16 ignore_next_character = 0;
    uint16 cur_char, next_char = 0, prev_char = 0, n_next_char = 0;
    uint16 gly_ph;
    uint8 i = 0, j = 0;
    uint8 result_length = 0;
    uint8 link_length = 0;
    uint8 *str = desc->data.str;
    uint16 length = desc->length;

    while (i < (length * 2))
    {
        cur_char = str[i] + ((uint16) (str[i + 1]) << 8);
        if (cur_char == 0)
        {
            link_length = result_length;
            break;
        }
        next_char = str[i + 2] + ((uint16) (str[i + 3]) << 8);
        n_next_char = str[i + 4] + ((uint16) (str[i + 5]) << 8);

        if (ignore_next_character != 0)
        {
            ignore_next_character = 0;
            i = i + 2;
        }
        else
        {
            if (is_arabic_char(cur_char) == TRUE)
            {
                gly_ph = get_presentation_form(cur_char, next_char, prev_char, &ignore_next_character, n_next_char);
                prev_char = cur_char;
            }
            else
            {
                gly_ph = cur_char;
                prev_char = 0;
                link_length = result_length + 1;
            }

            str[j] = (uint8) gly_ph;
            str[j + 1] = (uint8) (gly_ph >> 8);
            i = i + 2;
            j = j + 2;
            result_length++;
        }
    }
    if (j != 0)
    {
        str[j] = 0;
        str[j + 1] = 0;
    }
    return link_length;
}
/*! \endcond */


