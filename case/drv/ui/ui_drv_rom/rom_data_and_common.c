/*******************************************************************************
 *                              US212A
 *                            Module: ui driver
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>           <time>          <version >             <desc>
 *       cailizhen     2012-1-12 9:42     1.0             build this file
 *******************************************************************************/
/*!
 * \file     _rom_data_and_common.c
 * \brief    ROM代码模块基本功能函数和数据（小彩屏）
 * \author   cailizhen
 * \par      GENERAL DESCRIPTION:
 *
 * \par      EXTERNALIZED FUNCTIONS:
 *               none
 * \version  1.0
 * \date  2012-1-12
 *******************************************************************************/
#include "display.h"
#include "ui_driver.h"

//资源信息结构体
res_open_infor_t g_ap_res;
res_open_infor_t g_com_res;
//字库文件句柄
sd_handle FontLib_fp;
//多国语言转换表句柄
sd_handle MBtoUniTab_fp;

//dc句柄
DC dc;
//当前UI元素显示区域
region_t region;
//显示屏窗口
region_t region_win;

#ifndef PC
uint32 display_buf_addr; //显示buffer的地址
uint32 font_cache_addr; //字模缓冲区地址
#endif
uint16 display_buf_len; //显示buffer的大小
uint16 font_cache_len; //字模缓冲区大小，默认支持1KB
uint16 font_cache_header;//字模缓冲区头部大小，必须4字节对齐，默认为64B
bool font_cache_flag;//使用字模缓冲区标志

uint8 char_point_buffer[33]; //字模点阵 buffer

//各个被ROM调用的函数对应的函数指针变量
__char_fill_hook char_fill_hook;
__dma_buffer_data_hook dma_buffer_data_hook;
__read_str_bgpic read_str_bgpic;
__read_str_bgpic_hook read_str_bgpic_hook;
__get_next_char_point_hook get_next_char_point_hook;
__next_char_ansi_hook next_char_ansi_hook;
__next_char_unicode_hook next_char_unicode_hook;
__set_lcd_param_hook set_lcd_param_hook;

//ROM接口（UI驱动内的外部接口）
__put_sd_image put_sd_image;
__black_char_to_color black_char_to_color;
__char_fill char_fill;
__get_next_char_type_unicode get_next_char_type_unicode;
__get_next_char_type_ansi get_next_char_type_ansi;
__search_cache search_cache;
__put_char put_char;

/*! \cond */
/******************************************************************************/
/*!
 * \par  Description:
 *    搜索缓冲区字模，返回列表序号
 * \param[in]    uni_code：Unicode编码，只支持Unicode字库缓存
 * \param[out]   cache_index：返回列表序号
 * \return       返回字模是否已缓存
 * \note         说明：（以默认 1KB缓冲区进行说明）
 *               1）缓存空间总共 1KB，前64字节用于管理，后960字节用于缓存字模，可缓存29个不同编码字符。
 *               2）头4字节表示标志："font" = 102, 111, 110, 116 = 0x746E6F66，如果没有该标志，需初始化。
 *               3）第5字节用于控制替换字模序号；当发生冲突（找不到字模并且字模表已满）时，按照第5字节
 *                  的编码（0-28）覆盖缓存并更新列表，然后第5字节循环递增。第6字节不用无效。
 *               4）字模按Unicode编码存放，紧接着的 58B 存放当前缓冲区字模列表，用于查询。
 *               5）更新font_cache_addr，font_cache_len，font_cache_header 等变量，可能会影响到font表，
 *                  这样就需要把 FONT_CACHE_MAGIC 清除掉。
 *               6）用户自己保证实际可用字模缓冲区大小够保存 max_char 字符字模，即 960 >= max_char*33。
 *******************************************************************************/
bool romf_search_cache(uint16 uni_code, uint8 *cache_index)
{
#ifndef PC
    uint8 *font_cache_buffer = (uint8 *) font_cache_addr;
#else
    uint8 *font_cache_buffer = GET_REAL_ADDR(font_cache_addr);
#endif
    uint16 *table = (uint16 *)(font_cache_buffer + 6);//请保证 font_cache_buffer half_word 对齐
    uint8 max_char;
    uint8 i;

    //计算支持字符字模总数：减去开头6字节，每2字节保存一个字符编码
    max_char = (uint8)((font_cache_header - 6) / 2);

    //检测是否有效缓冲区
    if (*((uint32 *) font_cache_buffer) != FONT_CACHE_MAGIC)//"font" = 0x746E6F66
    {
        for (i = 0; i < max_char; i++)
        {
            table[i] = 0xfeff;
        }
        font_cache_buffer[FONT_CYCLE_INDEX] = 0;
        *cache_index = 0;
        table[0] = uni_code;//先填充列表ID，字模数据在外部填充
        *((uint32 *) font_cache_buffer) = FONT_CACHE_MAGIC;//"font"

        return FALSE;
    }

    for (i = 0; i < max_char; i++)
    {
        //找到字模，成功返回
        if (table[i] == uni_code)
        {
            *cache_index = i;
            return TRUE;
        }
        //找到无效字模，表示没有缓存，返回失败
        if (table[i] == 0xfeff)
        {
            *cache_index = i;
            table[i] = uni_code;//先填充列表ID，字模数据在外部填充
            return FALSE;
        }
    }

    //已满，获取替换序号
    *cache_index = font_cache_buffer[FONT_CYCLE_INDEX];
    table[*cache_index] = uni_code;//先填充列表ID，字模数据在外部填充
    //循环递增替换序号
    font_cache_buffer[FONT_CYCLE_INDEX]++;
    if (font_cache_buffer[FONT_CYCLE_INDEX] >= max_char)
    {
        font_cache_buffer[FONT_CYCLE_INDEX] = 0;
    }
    return FALSE;
}
/*! \endcond */

/*! \cond UI_DRIVER_API */
/******************************************************************************/
/*!
 * \par  Description:
 *    获取dc信息
 * \param[in]    none
 * \param[out]   pdc：保存DC信息的结构体指针
 * \return       none
 * \ingroup      settings
 * \note
 *******************************************************************************/
void romf_get_DC_status(PDC pdc, void*null2, void *null3)
{
    pdc->pen_color = dc.pen_color;
    pdc->backgd_color = dc.backgd_color;
}

/******************************************************************************/
/*!
 * \par  Description:
 *    设置画笔颜色
 * \param[in]    color：要设置的画笔颜色值
 * \param[out]   none
 * \return       none
 * \ingroup      settings
 * \note
 *******************************************************************************/
void romf_set_pen_color(COLORREF color, void *null2, void *null3)
{
    dc.pen_color = color;
}

/******************************************************************************/
/*!
 * \par  Description:
 *    设置背景颜色
 * \param[in]    color：要设置的背景颜色值
 * \param[out]   none
 * \return       none
 * \ingroup      settings
 * \note
 *******************************************************************************/
void romf_set_backgd_color(COLORREF color, void*null2, void *null3)
{
    dc.backgd_color = color;
}
/*! \endcond */

/*! \cond */
/******************************************************************************/
/*!
 * \par  Description:
 *    获取当前屏幕坐标
 * \param[in]    none
 * \param[out]   x：获取的X坐标
 * \param[out]   y：获取的Y坐标
 * \return       none
 * \note
 *******************************************************************************/
void romf_get_text_pos(uint16 *x, uint16 *y)
{
    *x = region.x;
    *y = region.y;
}

/******************************************************************************/
/*!
 * \par  Description:
 *    设置字符输出当前坐标
 * \param[in]    x：要设置的X坐标
 * \param[in]    y：要设置的Y坐标
 * \param[out]   none
 * \return       none
 * \note
 *******************************************************************************/
void romf_set_text_pos(uint16 x, uint16 y)
{
    region.x = x;
    region.y = y;
}
/*! \endcond */

/*! \cond UI_DRIVER_API */
/******************************************************************************/
/*!
 * \par  Description:
 *    获取多国语言字符串的unicode编码
 * \param[in]    id：多国语言字符串ID
 * \param[in]    type：资源类型
 * \param[in]    string_code_infor：指向多国语言字符串结构体变量
 * \return       none
 * \ingroup      string
 * \note         desc->result 和 desc->length 都返回有效字节数。
 *******************************************************************************/
void romf_get_unicode_data(uint16 id, string_desc_t *desc)
{
    res_langid_entry_t res_langid_entry;
    res_open_infor_t *res_ptr;
    uint16 real_len;

    /*根据打开资源文件的类型，将指针指向不同的全局变量*/
    res_ptr = res_ptr_select(id);
    // 指针指向字符在文件中的位置
    sys_sd_fseek(res_ptr->res_fp, SEEK_SET, res_ptr->string_entry_offset
            + (uint32) (REAL_RES_ID( id)) *sizeof(res_langid_entry_t));
    sys_sd_fread(res_ptr->res_fp, &res_langid_entry, sizeof(res_langid_entry_t)); //res_fp->&res_entry
    /*读取字符串内容，unicode编码*/
    //注意：res_langid_entry.length 已经包含了结束符在内。
    if (res_langid_entry.length > desc->length)
    {
        //注意：desc->length 需为偶数
        desc->result = desc->length = real_len = desc->length - 2;
        //自己添加结束符
        desc->data.str[desc->length] = 0;
        desc->data.str[desc->length + 1] = 0;
    }
    else
    {
        real_len = res_langid_entry.length;
        desc->result = desc->length = real_len - 2;//除去结束符
    }
    //读取Unicode编码
    sys_sd_fseek(res_ptr->res_fp, SEEK_SET, res_langid_entry.offset);
    sys_sd_fread(res_ptr->res_fp, desc->data.str, (uint32) real_len);
}
/*! \endcond */
