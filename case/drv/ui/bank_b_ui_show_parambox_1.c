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
 * \file     ui_show_parambox_1.c
 * \brief    显示parambox的参数值的模块
 * \author   liutingting
 * \par      GENERAL DESCRIPTION:
 *               实现parambox num项控件数据的解析并显示参数值
 * \par      EXTERNALIZED FUNCTIONS:
 *              直接调用系统SD文件系统的接口函数解析并显示参数值
 * \version  1.0
 * \date  2011/9/05
 *******************************************************************************/
#include "display.h"
#include "ui_driver.h"

#define PARAMBOX_LISTDIRECT_OPTION_SHIFT    (4)
#define PARAMBOX_LISTDIRECT_OPTION      (0x0001 << PARAMBOX_LISTDIRECT_OPTION_SHIFT)
#define PARAMBOX_LISTDIRECT_HORIZONTAL  (0x0000 << PARAMBOX_LISTDIRECT_OPTION_SHIFT)
#define PARAMBOX_LISTDIRECT_VERTICAL    (0x0001 << PARAMBOX_LISTDIRECT_OPTION_SHIFT)

//parambox number attribute bits location
#define NUMBER_SHOW_PLUS_EN             (0x0001 << 0)
#define NUMBER_SHOW_ZERO_EN             (0x0001 << 1)
#define NUMBER_DISP_OPTION_SHIFT        (2)
#define NUMBER_DISP_OPTION              (0x0001 << NUMBER_DISP_OPTION_SHIFT)
#define DISP_OPTION_CHAR                (0x0000 << NUMBER_DISP_OPTION_SHIFT)
#define DISP_OPTION_PIC                 (0x0001 << NUMBER_DISP_OPTION_SHIFT)
#define NUMBER_INACTIVE_BG_OPTION_SHIFT (3)
#define NUMBER_INACTIVE_BG_OPTION       (0x0003 << NUMBER_INACTIVE_BG_OPTION_SHIFT)
#define INACTIVE_BG_OPTION_COLOR        (0x0000 << NUMBER_INACTIVE_BG_OPTION_SHIFT)
#define INACTIVE_BG_OPTION_SPIC         (0x0001 << NUMBER_INACTIVE_BG_OPTION_SHIFT)
#define INACTIVE_BG_OPTION_MPIC         (0x0002 << NUMBER_INACTIVE_BG_OPTION_SHIFT)
#define NUMBER_ACTIVE_BG_OPTION_SHIFT   (5)
#define NUMBER_ACTIVE_BG_OPTION         (0x0003 << NUMBER_ACTIVE_BG_OPTION_SHIFT)
#define ACTIVE_BG_OPTION_COLOR          (0x0000 << NUMBER_ACTIVE_BG_OPTION_SHIFT)
#define ACTIVE_BG_OPTION_SPIC           (0x0001 << NUMBER_ACTIVE_BG_OPTION_SHIFT)
#define ACTIVE_BG_OPTION_MPIC           (0x0002 << NUMBER_ACTIVE_BG_OPTION_SHIFT)
#define NUMBER_ALIGNMENT_OPTION_SHIFT   (7)
#define NUMBER_ALIGNMENT_OPTION         (0x0003 << NUMBER_ALIGNMENT_OPTION_SHIFT)
#define NUMBER_ALIGNMENT_OPTION_LEFT    (0x0000 << NUMBER_ALIGNMENT_OPTION_SHIFT)
#define NUMBER_ALIGNMENT_OPTION_CENTER  (0x0001 << NUMBER_ALIGNMENT_OPTION_SHIFT)
#define NUMBER_ALIGNMENT_OPTION_RIGHT   (0x0002 << NUMBER_ALIGNMENT_OPTION_SHIFT)
#define NUMBER_FONT_OPTION_SHIFT        (9)
#define NUMBER_FONT_OPTION              (0x0003 << NUMBER_FONT_OPTION_SHIFT)
#define NUMBER_FONT_OPTION_SMALL        (0x0000 << NUMBER_FONT_OPTION_SHIFT)
#define NUMBER_FONT_OPTION_MIDDLE       (0x0001 << NUMBER_FONT_OPTION_SHIFT)
#define NUMBER_FONT_OPTION_LARGE        (0x0002 << NUMBER_FONT_OPTION_SHIFT)
/*! \cond */
/******************************************************************************/
/*!
 * \par  Description:
 *    显示parambox的参数值
 * \param[in]   parambox：parambox控件数据结构体变量
 * \param[in]   parambox_data：应用传递的parambox的私有数据结构体指针
 * \param[in]   itemnum：第几个参数项
 * \param[in]   type：当前项是否为激活项
 * \param[in]   mode：parambox的刷新模式
 * \param[in]   style_ptr：当前资源信息结构体指针
 * \param[out]  none
 * \return      none
 * \note
 *******************************************************************************/
void show_parambox_num(parambox_t *parambox, parambox_private_t *parambox_data, uint16 itemnum, uint8 type,
        parambox_draw_mode_e mode, res_open_infor_t *style_ptr)
{
    parambox_num_t parambox_num;
    res_entry_t res_entry;
    region_t disp_region;
    res_open_infor_t *res_ptr;
    uint16 x, bak_x;
    uint16 x_left = 0;
    uint16 x_right = 0;
    uint16 back_x, back_y;
    uint16 length;
    uint16 textout_mode = 0;
    uint16 num_back_id = -1;
    uint8 string[10];
    uint8 num_bits = 0;
    int8 n = 0;
    uint8 plus_flag = 0;
    uint8 j;
    string_desc_t desc;

    sys_sd_fseek(style_ptr->res_fp, SEEK_SET, parambox->num_addr);
    sys_sd_fread(style_ptr->res_fp, &parambox_num, sizeof(parambox_num_t));
    if ((mode == PARAMBOX_DRAW_ALL) || (mode == PARAMBOX_DRAW_PARAM)) //初始化或变换参数时需要重画背景
    {
        param_bits = 0;
    }

    //非激活项背景处理
    if (type == 0)
    {
        if ((parambox_num.attrib & NUMBER_INACTIVE_BG_OPTION) == INACTIVE_BG_OPTION_COLOR)
        {
            romf_set_backgd_color(parambox_num.back_id, 0, 0);
            textout_mode |= CHAR_FILL_COLOR;
            //clear_screen(&disp_region,0,0);
            //romf_fill_rect(&disp_region, (uint16) (dc.backgd_color & 0xffff));
        }
        else
        {
            textout_mode |= CHAR_FILL_NORMAL;
            if ((parambox_num.attrib & NUMBER_INACTIVE_BG_OPTION) == INACTIVE_BG_OPTION_SPIC)
            {
                num_back_id = parambox_num.back_id;
            }
            else if ((parambox_num.attrib & NUMBER_INACTIVE_BG_OPTION) == INACTIVE_BG_OPTION_MPIC)
            {
                num_back_id = parambox_num.back_id + itemnum;
            }
            else
            {
                //do nothing for QAC
            }
            res_ptr = res_ptr_select(num_back_id);
            //获取图片宽高信息
            sys_sd_fseek(res_ptr->res_fp, SEEK_SET, res_ptr->pic_entry_offset + (uint32) (num_back_id & 0x7fff)
                    * RESHEADITEM);
            sys_sd_fread(res_ptr->res_fp, &res_entry, sizeof(res_entry_t));
            back_x = parambox->list_x[itemnum] + (parambox->list_width[itemnum] - res_entry.width) / 2;
            back_y = parambox->list_y[itemnum];
            u_show_pic(num_back_id, back_x, back_y);
        }
    }
    //激活项背景处理
    else
    {
        if ((parambox_num.attrib & NUMBER_ACTIVE_BG_OPTION) == ACTIVE_BG_OPTION_COLOR)
        {
            romf_set_backgd_color(parambox_num.back_act_id, 0, 0);
            textout_mode |= CHAR_FILL_COLOR;
            //clear_screen(&disp_region,0,0);
            //romf_fill_rect(&disp_region, (uint16) (dc.backgd_color & 0xffff));
        }
        else
        {
            textout_mode |= CHAR_FILL_NORMAL;
            if ((parambox_num.attrib & NUMBER_ACTIVE_BG_OPTION) == ACTIVE_BG_OPTION_SPIC)
            {
                num_back_id = parambox_num.back_act_id;
            }
            else if ((parambox_num.attrib & NUMBER_ACTIVE_BG_OPTION) == ACTIVE_BG_OPTION_MPIC)
            {
                num_back_id = parambox_num.back_act_id + itemnum;
            }
            else
            {
                //do nothing for QAC
            }

            res_ptr = res_ptr_select(num_back_id);
            //获取图片宽高信息
            sys_sd_fseek(res_ptr->res_fp, SEEK_SET, res_ptr->pic_entry_offset + (uint32) (num_back_id & 0x7fff)
                    * RESHEADITEM);
            sys_sd_fread(res_ptr->res_fp, &res_entry, sizeof(res_entry_t));
            if ((parambox_num.attrib & NUMBER_ALIGNMENT_OPTION) == NUMBER_ALIGNMENT_OPTION_CENTER)
            {
                back_x = parambox->list_x[itemnum] + (parambox->list_width[itemnum] - res_entry.width) / 2;
            }
            else if ((parambox_num.attrib & NUMBER_ALIGNMENT_OPTION) == NUMBER_ALIGNMENT_OPTION_RIGHT)
            {
                back_x = parambox->list_x[itemnum] + parambox->list_width[itemnum] - res_entry.width;
            }
            else
            {
                back_x = parambox->list_x[itemnum];
            }
            
            back_y = parambox->list_y[itemnum];
            u_show_pic(num_back_id, back_x, back_y);  //modify--vzhan
        }
    }
    
    //设置显示区域
    if(parambox->param_cnt > 1)//多参数
    {
        if((parambox->attrib & PARAMBOX_LISTDIRECT_OPTION) == PARAMBOX_LISTDIRECT_HORIZONTAL)//横向排列
        {
            disp_region.x = parambox->list_x[itemnum];
            disp_region.y = parambox_num.num_y;
            disp_region.width = parambox->list_width[itemnum];
            disp_region.height = parambox_num.num_height;
        }
        else//竖向排列
        {
            disp_region.x = parambox_num.num_x;
            disp_region.y = parambox->list_y[itemnum];
            disp_region.width = parambox_num.num_width;
            disp_region.height = parambox->list_height[itemnum];
        }
    }
    else//单参数
    {
        disp_region.x = parambox_num.num_x;
        disp_region.y = parambox_num.num_y;
        disp_region.width = parambox_num.num_width;
        disp_region.height = parambox_num.num_height;
    }

    if (parambox_data->items[itemnum].value_str == NULL)
    {
        //显示参数值数字
        if ((parambox_num.attrib & NUMBER_SHOW_ZERO_EN) != NUMBER_SHOW_ZERO_EN)
        {
            num_bits = 0; //隐藏高位数字0
        }
        else
        {
            num_bits = parambox_data->items[itemnum].max_number;
        }

        if((parambox_data->sign_flag == 1)
        && ((parambox_num.attrib & NUMBER_SHOW_PLUS_EN) == NUMBER_SHOW_PLUS_EN))
        {
            plus_flag = 1; //需要显示正号
        }
        else
        {
            plus_flag = 0;//不显示符号
        }
        
        if ((parambox_num.attrib & NUMBER_DISP_OPTION) == DISP_OPTION_PIC)
        {
            itoa_n((int32) parambox_data->items[itemnum].value, string, plus_flag, num_bits, 1);
        }
        else
        {
            itoa_n((int32) parambox_data->items[itemnum].value, string, plus_flag, num_bits, 0);
        }
    }
    //数字图片显示模式
    if ((parambox_num.attrib & NUMBER_DISP_OPTION) == DISP_OPTION_PIC)
    {
        if (parambox_data->items[itemnum].value_str == NULL)
        {
            res_ptr = res_ptr_select(parambox_num.pic_id);
            sys_sd_fseek(res_ptr->res_fp, SEEK_SET, res_ptr->pic_entry_offset + (uint32) (parambox_num.pic_id & 0x7fff)
                    * RESHEADITEM);
            sys_sd_fread(res_ptr->res_fp, &res_entry, sizeof(res_entry_t));
            length = ((uint8) libc_strlen(string)) * parambox_num.pic_width;
            //居中显示的X坐标
            if ((parambox_num.attrib & NUMBER_ALIGNMENT_OPTION) == NUMBER_ALIGNMENT_OPTION_LEFT)
            {
                //居左显示
                bak_x = x = disp_region.x;
            }
            //居右显示的X坐标
            else if ((parambox_num.attrib & NUMBER_ALIGNMENT_OPTION) == NUMBER_ALIGNMENT_OPTION_RIGHT)
            {
                bak_x = x = disp_region.x + disp_region.width - length;
            }
            else
            {
                //默认居中显示
                bak_x = x = disp_region.x + (disp_region.width - length) / 2;
            }
            for (j = 0; j < (uint8) libc_strlen(string); j++)
            {
                show_num_pic(string[j] - 0x30, x, disp_region.y, &res_entry, res_ptr);
                x += parambox_num.pic_width;
            }
            if (mode == PARAMBOX_DRAW_VALUE)
            {
                n = (int8) (param_bits - (uint8) libc_strlen(string));
                if (n > 0)//计算清除痕迹坐标
                {
                    //for QAC 把 if else if 换成switch，减少1层控制嵌套
                    switch (parambox_num.attrib & NUMBER_ALIGNMENT_OPTION)
                    {
                    //居中显示时，数字位数变少时，两边均需用背景图清除原来的数字
                    case NUMBER_ALIGNMENT_OPTION_CENTER:
                        n = (n/2) + (n%2);//for QAC 意为取一半，且如有余数则加多1
                        x_left = bak_x - n * parambox_num.pic_width;
                        x_right = bak_x + length;
                        break;
                        
                    //居右显示的X坐标
                    case NUMBER_ALIGNMENT_OPTION_RIGHT:
                        x = bak_x - n * parambox_num.pic_width;
                        break;
                    
                    default:
                        //do nothing for QAC
                        break;
                    }
                }
                while (n > 0)//清除痕迹
                {
                    if ((parambox_num.attrib & NUMBER_ALIGNMENT_OPTION) == NUMBER_ALIGNMENT_OPTION_CENTER)
                    {
                        show_num_pic(12, x_left, disp_region.y, &res_entry, res_ptr);
                        show_num_pic(12, x_right, disp_region.y, &res_entry, res_ptr);
                        x_left += parambox_num.pic_width;
                        x_right += parambox_num.pic_width;
                    }
                    else
                    {
                        show_num_pic(12, x, disp_region.y, &res_entry, res_ptr);
                        x += parambox_num.pic_width;
                    }
                    n--;
                }
            }
            param_bits = (uint8) libc_strlen(string);
        }
    }
    else //字符串显示模式
    {
        //atrib<9-10>0表示小字体,1表示中号字体，2表示大字体，默认中号字体
        if ((parambox_num.attrib & NUMBER_FONT_OPTION) == NUMBER_FONT_OPTION_SMALL)
        {
            textout_mode |= DISP_FONT_SMALL;
        }
        else if ((parambox_num.attrib & NUMBER_FONT_OPTION) == NUMBER_FONT_OPTION_LARGE)
        {
            textout_mode |= DISP_FONT_LARGE;
        }
        else
        {
            textout_mode |= DISP_FONT_NORMAL; //默认中号字体
        }

        //设置画笔颜色
        if (type == 0)
        {
            romf_set_pen_color(parambox_num.num_color, 0, 0);
        }
        else
        {
            romf_set_pen_color(parambox_num.num_act_color, 0, 0);
        }
        
        //clear_screen(&disp_region, 0, 0);
        //romf_fill_rect(&disp_region, (uint16) (dc.backgd_color & 0xffff));
        //居左显示
        if ((parambox_num.attrib & NUMBER_ALIGNMENT_OPTION) == NUMBER_ALIGNMENT_OPTION_LEFT)
        {
            textout_mode |= DISP_ALIGN_LEFT;
        }
        //居右显示
        else if ((parambox_num.attrib & NUMBER_ALIGNMENT_OPTION) == NUMBER_ALIGNMENT_OPTION_RIGHT)
        {
            textout_mode |= DISP_ALIGN_RIGHT;
        }
        //居中显示
        else
        {
            textout_mode |= DISP_ALIGN_MEDIACY;
        }
        textout_mode |= DISP_ARABIC_DISABLE;
        if (parambox_data->items[itemnum].value_str == NULL)
        {
            desc.data.str = string;
            desc.length = (uint16) libc_strlen(string);
            desc.language = 0;
            show_string(&desc, &disp_region, textout_mode);
        }
        else
        {
            desc.data.str = parambox_data->items[itemnum].value_str;
            desc.length = (uint16) libc_strlen(parambox_data->items[itemnum].value_str);
            desc.language = 0;
            show_string(&desc, &disp_region, textout_mode);
        }
    }
}
/*! \endcond */
