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
 * \file     ui_show_numbox_1.c
 * \brief   字符串方式显示numbox的模块
 * \author   liutingting
 * \par      GENERAL DESCRIPTION:
 *              用字符串实现numbox的显示
 * \par      EXTERNALIZED FUNCTIONS:
 *              直接调用字符串显示函数showstring实现numbox的显示
 * \version 1.0
 * \date  2011-9-05
 *******************************************************************************/
#include "display.h"
#include "ui_driver.h"

//numbox attribute bits location
#define NUMBOX_PLUS_EN                  (0x0001 << 1)
#define NUMBOX_ALIGNMENT_OPTION_SHIFT   (2)
#define NUMBOX_ALIGNMENT_OPTION         (0x0003 << NUMBOX_ALIGNMENT_OPTION_SHIFT)
#define ALIGNMENT_OPTION_LEFT           (0x0000 << NUMBOX_ALIGNMENT_OPTION_SHIFT)
#define ALIGNMENT_OPTION_CENTER         (0x0001 << NUMBOX_ALIGNMENT_OPTION_SHIFT)
#define ALIGNMENT_OPTION_RIGHT          (0x0002 << NUMBOX_ALIGNMENT_OPTION_SHIFT)
#define NUMBOX_COMPOSE_OPTION_SHIFT     (4)
#define NUMBOX_COMPOSE_OPTION           (0x0001 << NUMBOX_COMPOSE_OPTION_SHIFT)
#define COMPOSE_OPTION_DECIMAL          (0x0000 << NUMBOX_COMPOSE_OPTION_SHIFT)
#define COMPOSE_OPTION_RATIO            (0x0001 << NUMBOX_COMPOSE_OPTION_SHIFT)
#define NUMBOX_BG_OPTION_SHIFT          (6)
#define NUMBOX_BG_OPTION                (0x0001 << NUMBOX_BG_OPTION_SHIFT)
#define NUMBOX_BG_OPTION_COLOR          (0x0000 << NUMBOX_BG_OPTION_SHIFT)
#define NUMBOX_BG_OPTION_PIC            (0x0001 << NUMBOX_BG_OPTION_SHIFT)
#define NUMBOX_FONT_OPTION_SHIFT        (7)
#define NUMBOX_FONT_OPTION              (0x0003 << NUMBOX_FONT_OPTION_SHIFT)
#define NUMBOX_FONT_OPTION_SMALL        (0x0000 << NUMBOX_FONT_OPTION_SHIFT)
#define NUMBOX_FONT_OPTION_MIDDLE       (0x0001 << NUMBOX_FONT_OPTION_SHIFT)
#define NUMBOX_FONT_OPTION_LARGE        (0x0002<< NUMBOX_FONT_OPTION_SHIFT)
const uint8 partition[2] = { "/" };
/*! \cond */
/******************************************************************************/
/*!
 * \par  Description:
 *    字符串显示numbox
 * \param[in]   numbox_data：应用传递的numbox的私有数据结构体指针
 * \param[in]   numbox：numbox控件的数据结构体
 * \param[in]   mode：numbox的刷新方式
 * \param[out]  none
 * \return      none
 * \note
 *******************************************************************************/
void show_numbox_string(numbox_private_t *numbox_data, numbox_t *numbox, numbox_draw_mode_e mode)
{
    region_t num_region;
    DC oldDC_sav;
    uint16 disp_mode = 0;
    string_desc_t desc;
    uint8 string[12];
    uint8 string1[6];
    uint8 string2[6];
    uint8 j;
    uint8 num_bits[2] = { 0, 0 };
    uint8 partition_pos = 0;

    libc_memset(string, 0, sizeof(string));
    //保存字符串显示区域
    num_region.x = numbox->x;
    num_region.y = numbox->y;
    num_region.width = numbox->width;
    num_region.height = numbox->height;
    num_bits[0] = (uint8) (numbox->position & 0x000f);
    num_bits[1] = ((uint8) (numbox->position & 0x00f0)) >> 4;

    if ((numbox->attrib & NUMBOX_COMPOSE_OPTION) == COMPOSE_OPTION_DECIMAL) //只有一个数字显示
    {
        partition_pos = (uint8) ((numbox->position & 0xff00) >> 8);
        itoa_n(numbox_data->value, string, (uint8) (numbox->attrib & NUMBOX_PLUS_EN), (uint16) num_bits[0], 0);
        if (partition_pos < (uint8) libc_strlen(string))
        {
            partition_pos = (uint8) libc_strlen(string) - partition_pos;
            for (j = (uint8) libc_strlen(string); j >= partition_pos; j--)//包括结束符拷贝
            {
                if ((j + 1) < 11)//for QAC
                {
                    string[j + 1] = string[j];
                }
            }
            if(partition_pos <= 11)//for QAC
            {
                string[partition_pos] = '.';
            }
        }
    }
    else //两个数字组成的numbox的显示
    {
        itoa_n(numbox_data->value, string1, (uint8) (numbox->attrib & NUMBOX_PLUS_EN), (uint16) num_bits[0], 0);
        itoa_n(numbox_data->total, string2, (uint8) (numbox->attrib & NUMBOX_PLUS_EN), (uint16) num_bits[1], 0);
        libc_strncpy(string, string1, libc_strlen(string1));
        libc_strncat(string, partition, libc_strlen(partition));
        libc_strncat(string, string2, libc_strlen(string2));
    }
    romf_get_DC_status(&oldDC_sav, 0, 0);
    if ((numbox->attrib & NUMBOX_BG_OPTION) == NUMBOX_BG_OPTION_PIC)
    {
        disp_mode |= CHAR_FILL_NORMAL; //有背景图片，则是透明模式
        if (numbox->back_id != (uint16) - 1)
        {
            u_show_pic(numbox->back_id, numbox->back_x, numbox->back_y);
        }
    }
    else
    {
        disp_mode |= CHAR_FILL_COLOR;
        romf_set_backgd_color(numbox->back_id, 0, 0);
        //clear_screen(&num_region, 0, 0);
        romf_fill_rect(&num_region, (uint16) (dc.backgd_color & 0xffff));
    }

    //atrib<7-8>0表示小字体,1表示中号字体，2表示大字体，默认中号字体
    if ((numbox->attrib & NUMBOX_FONT_OPTION) == NUMBOX_FONT_OPTION_SMALL)
    {
        disp_mode |= DISP_FONT_SMALL;
    }
    else if ((numbox->attrib & NUMBOX_FONT_OPTION) == NUMBOX_FONT_OPTION_LARGE)
    {
        disp_mode |= DISP_FONT_LARGE;
    }
    else
    {
        disp_mode |= DISP_FONT_NORMAL; //默认中号字体
    }

    //设置画笔颜色
    romf_set_pen_color(numbox->text_color, 0, 0);
    //居左显示
    if ((numbox->attrib & NUMBOX_ALIGNMENT_OPTION) == ALIGNMENT_OPTION_LEFT)
    {
        disp_mode |= DISP_ALIGN_LEFT;
    }
    //居中显示
    else if ((numbox->attrib & NUMBOX_ALIGNMENT_OPTION) == ALIGNMENT_OPTION_CENTER)
    {
        disp_mode |= DISP_ALIGN_MEDIACY;
    }
    //居右显示
    else if ((numbox->attrib & NUMBOX_ALIGNMENT_OPTION) == ALIGNMENT_OPTION_RIGHT)
    {
        disp_mode |= DISP_ALIGN_RIGHT;
    }
    else
    {
        //do nothing for QAC
    }
    disp_mode |= DISP_ARABIC_DISABLE;
    //显示数字字符串
    desc.data.str = string;
    desc.length = 12;
    desc.language = ENGLISH;
    show_string(&desc, &num_region, disp_mode);
    //恢复画笔颜色
    romf_set_pen_color(oldDC_sav.pen_color, 0, 0);
    romf_set_backgd_color(oldDC_sav.backgd_color, 0, 0);
}
/*! \endcond */
