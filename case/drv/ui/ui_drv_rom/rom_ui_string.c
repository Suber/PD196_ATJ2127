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
 * \file     _rom_ui_string.c
 * \brief    ROM代码模块--字符串显示部分（小彩屏）
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

/*! \cond */
//说明：
//1）如果像素点数据通过 DMA 传输，那么需要低字节在前，高字节在后。
//2）如果像素点数据通过 EXTMEM 传输，那么需要高字节在前，低字节在后。

/******************************************************************************/
/*!
 * \par  Description:
 *    字符从黑白转换为彩色，保存到缓冲区中
 * \param[in]    psoure_buf：字符字模缓冲区，33 个字节（16*16，buf[32]表示宽度）
 * \param[in]    buf_region：显示缓冲区预设区域（高度小于height_max）
 *               （缓冲区被划分为宽度为width，高度为height_max + buffer_len / width的区域）
 * \param[in]    char_disp：字符（字模）显示模式
 * \param[in]    start_col：字符（字模）显示开始位置
 * \param[out]   none
 * \return       none
 * \note         start_col 不能大于等于字符宽度
 *******************************************************************************/
void romf_black_char_to_color(uint8 *psoure_buf, region_t *buf_region, char_disp_e char_disp, uint8 start_col)
{
#ifndef PC
    uint16 *display_buffer_base = (uint16 *) display_buf_addr;
#else
    uint16 *display_buffer_base = GET_REAL_ADDR(display_buf_addr);
#endif
    uint16 *display_buffer;//缓冲区为16bit，符合565小彩屏
    uint16 pen_color = (uint16) (dc.pen_color & 0xffff);
    uint8 i, j, k;
    uint8 src_data;//0表示空白点，1表示字点
    uint8 temp_width;//1表示16*8字模，2表示16×16字模
    uint8 font_width;
    uint8 real_start, real_end;

    //初始基础参数
    font_width = psoure_buf[32];
    temp_width = 1;
    if (font_width > 8)
    {
        temp_width = 2;
    }
    //确定处理区间
    if (char_disp == CHAR_DISP_NORMAL)
    {
        real_start = 0;
        real_end = font_width - 1;
    }
    else if (char_disp == CHAR_DISP_LEFT2RIGHT)
    {
        real_start = start_col;
        real_end = start_col + (uint8)(region.width - 1);
    }
    else//CHAR_DISP_RIGHT2LEFT
    {
        real_start = start_col - (uint8)(region.width - 1);
        real_end = start_col;
    }

    for (i = 0, k = (uint8)buf_region->y; i < UNICODE_HEIGHT_DEF; i++, k++)//loop for height
    {
        display_buffer = display_buffer_base + (k * buf_region->width + buf_region->x);
        for (j = real_start; j <= real_end; j++)//loop for char width
        {
            if (j > 7)
            {
                src_data = (uint8)(psoure_buf[i * temp_width + 1] & (0x80 >> (j - 8)));
            }
            else
            {
                src_data = (uint8)(psoure_buf[i * temp_width] & (0x80 >> j));
            }

            //空白点处理
            if (src_data == 0)
            {
                //跳过无需转换
                display_buffer++;
            }
            //字点处理
            else
            {
                //将画笔颜色填入
                *display_buffer = pen_color;
                display_buffer++;
            }
        }
    }
}

/******************************************************************************/
/*!
 * \par  Description:
 *    根据字符的点阵数据以及是否透明，在屏幕上画出该字符需要显示的部分。支持不完整字符显示
 * \param[in]   psoure_buf：字符字模缓冲区
 * \param[in]   char_disp：字符（字模）显示模式
 * \param[in]   start_col：字符（字模）显示开始位置
 * \param[out]  none
 * \return      none
 * \note        start_col 不能大于等于字符宽度
 *******************************************************************************/
void romf_char_fill(uint8 *psoure_buf, char_disp_e char_disp, uint8 start_col)
{
    region_t point_region;//打点窗口
    uint16 temp_data;
    uint16 temp_width;
    uint8 trans_font[2];
    uint8 i, j;
    uint8 src_data;//0表示空白点，1表示字点
    uint8 font_width;
    uint8 real_start, real_end;

    //初始基础参数
    font_width = psoure_buf[32];
    temp_width = 1;
    if (font_width > 8)
    {
        temp_width = 2;
    }
    //确定处理区间
    if (char_disp == CHAR_DISP_NORMAL)
    {
        real_start = 0;
        real_end = font_width - 1;
    }
    else if (char_disp == CHAR_DISP_LEFT2RIGHT)
    {
        real_start = start_col;
        real_end = start_col + (uint8)(region.width - 1);
    }
    else//CHAR_DISP_RIGHT2LEFT
    {
        real_start = start_col - (uint8)(region.width - 1);
        real_end = start_col;
    }

    //初始字点颜色
    temp_data = (uint16) dc.pen_color;
#ifdef PC
    trans_font[0] = (uint8) temp_data;//低字节
    trans_font[1] = (uint8) ((temp_data & 0xff00) >> 8);//高字节
#else
    trans_font[0] = (uint8) ((temp_data & 0xff00) >> 8);//通过EXTMEM传输，高字节先发送
    trans_font[1] = (uint8) temp_data;//低字节后发送
#endif

    //初始打点窗口
    point_region.x = region.x;
    point_region.y = region.y;
    point_region.width = 1;
    point_region.height = 1;

    for (i = 0; i < UNICODE_HEIGHT_DEF; i++)//loop for height
    {
        for (j = real_start; j <= real_end; j++)//loop for width
        {
            if (j > 7)
            {
                src_data = (uint8) (psoure_buf[i * temp_width + 1] & (0x80 >> (j - 8)));
            }
            else
            {
                src_data = (uint8) (psoure_buf[i * temp_width] & (0x80 >> j));
            }

            //字点，直接打点显示
            if (src_data != 0)
            {
                char_fill_hook(&point_region, trans_font);
            }

            point_region.x++;
#ifdef PC
            lcd_set_window(&point_region);
#endif
        }
        point_region.x -= (real_end - real_start + 1);
        point_region.y++;
#ifdef PC
        lcd_set_window(&point_region);
#endif
    }

    //指针指向下一个字符左上角，同时width相应减少
    region.x += (real_end - real_start + 1);
    region.width -= (real_end - real_start + 1);
}

/******************************************************************************/
/*!
 * \par  Description:
 *    读取（或构造）当前字符串段背景数据
 * \param[in]    display_buffer：字符串显示缓冲区
 * \param[in]    bgpic_region：当前字符串段显示窗口，也是字符串背景窗口
 * \param[in]    mode：字符填充模式
 * \param[out]   none
 * \return       none
 * \note
 *******************************************************************************/
void romf_read_str_bgpic(uint8 *display_buffer, region_t *bgpic_region, char_fill_mode_e mode)
{
    uint16 *display_buffer_uint16;
    uint16 backgd_color = (uint16) (dc.backgd_color & 0xffff);
    uint16 row, col;

    //使用背景颜色填充（纯逻辑代码，可固化）
    if (mode == CHAR_FILL_COLOR)
    {
        for (row = 0; row < bgpic_region->height; row++)
        {
            display_buffer_uint16 = (uint16 *) display_buffer + (row * bgpic_region->width);
            for (col = 0; col < bgpic_region->width; col++)
            {
                *(display_buffer_uint16 + col) = backgd_color;
            }
        }
    }

    //读取（或构造）背景数据 hook 函数
    read_str_bgpic_hook(display_buffer, bgpic_region, mode);
}

/******************************************************************************/
/*!
 * \par  Description:
 *    获取当前位置字符类型--Unicode 编码
 * \param[in]    str：当前字符串起始位置
 * \param[in]    length：当前有效字节数
 * \param[in]    language：字符串编码语言类型
 * \param[out]   byte_cnt：传回当前有效字符的字节数
 * \return       char_type_e：返回字符类型
 * \note         这里的字符是一个更丰富的概念，定义为完整显示的字符单元。
 *******************************************************************************/
char_type_e romf_get_next_char_type_unicode(uint8 *str, uint16 length, int8 language, uint8 *byte_cnt)
{
    uint16 uni_code, next_code;

    if (length < 2)
    {
        *byte_cnt = 0;
        return CHAR_NOPEND;
    }
    else
    {
        uni_code = *str + ((uint16) (*(str + 1)) << 8);
        if (uni_code == 0x00)
        {
            *byte_cnt = 2;
            return CHAR_EOF;
        }
        else if (uni_code == 0xfeff)
        {
            *byte_cnt = 2;
            return CHAR_UNI_FLAG;
        }
        else if (uni_code == 0x0a)
        {
            *byte_cnt = 2;
            return CHAR_LR;//linux
        }
        else
        {
            //do nothing for QAC
        }

        if (length >= 4)
        {
            next_code = *(str + 2) + ((uint16) (*(str + 3)) << 8);
            if ((uni_code == 0x0d) && (next_code == 0x0a))
            {
                *byte_cnt = 4;
                return CHAR_LR;//windows
            }
            else if (uni_code == 0x0d)
            {
                *byte_cnt = 2;
                return CHAR_LR;//mac
            }
            else
            {
                //do nothing for QAC
            }
        }
        else
        {
            if (uni_code == 0x0d)
            {
                *byte_cnt = 0;
                return CHAR_NOPEND;
            }
        }
    }

    return next_char_unicode_hook(str, length, language, byte_cnt);
}

/******************************************************************************/
/*!
 * \par  Description:
 *    获取当前位置字符类型--内码编码
 * \param[in]    str：当前字符串起始位置
 * \param[in]    length：当前有效字节数
 * \param[in]    language：字符串编码语言类型
 * \param[out]   byte_cnt：传回当前有效字符的字节数
 * \return       char_type_e：返回字符类型
 * \note         这里的字符是一个更丰富的概念，定义为完整显示的字符单元。
 *******************************************************************************/
char_type_e romf_get_next_char_type_ansi(uint8 *str, uint16 length, int8 language, uint8 *byte_cnt)
{
    if (*str == 0x00)
    {
        *byte_cnt = 0;
        return CHAR_EOF;
    }
    else if (*str == 0x0a)
    {
        *byte_cnt = 1;
        return CHAR_LR;//linux
    }
    else
    {
        //do nothing for QAC
    }
    
    if (length >= 2)
    {
        if ((*str == 0x0d) && (*(str + 1) == 0x0a))
        {
            *byte_cnt = 2;
            return CHAR_LR;//windows
        }
        else if (*str == 0x0d)
        {
            *byte_cnt = 1;
            return CHAR_LR;//mac
        }
        else
        {
            //do nothing for QAC
        }
    }
    else
    {
        if (*str == 0x0d)
        {
            *byte_cnt = 0;
            return CHAR_NOPEND;
        }
    }

    return next_char_ansi_hook(str, length, language, byte_cnt);
}

/******************************************************************************/
/*!
 * \par  Description:
 *    获取指定Unicode 编码字符字模
 * \param[in]    uni_code：Unicode 字符编码
 * \param[in]    pbuffer：字符字模缓冲区
 * \param[out]   none
 * \return       none
 * \note
 *******************************************************************************/
void romf_get_next_char_point(uint16 uni_code, uint8 *pbuffer)
{
    uint8 cache_index;

    //获取字符点阵
    //有字模缓冲区，可用缓存机制减少读取字模次数
    //固化代码 for QAC：if(font_cache_flag) 等价于 if(font_cache_flag != FALSE) 而不等价于 if(font_cache_flag == TRUE)
    if (font_cache_flag != FALSE)
    {
        //找到缓存字模，直接读取
        if (search_cache(uni_code, &cache_index) == TRUE)
        {
            libc_memcpy(pbuffer, font_cache_char_addr(cache_index), 33);
        }
        //没找到缓存字模，从字库文件读取并填充到字模缓存中
        else
        {
            romf_get_font_point(uni_code, pbuffer);
            libc_memcpy(font_cache_char_addr(cache_index), pbuffer, 33);
        }
    }
    //没有字模缓冲区，每次都需要直接读取字模
    else
    {
        romf_get_font_point(uni_code, pbuffer);
    }
}

/******************************************************************************/
/*!
 * \par  Description:
 *    将内码字符转换成unicode编码
 * \param[in]    code_rename：字符码值
 * \param[out]   none
 * \return       当前字符的unicode编码值
 * \note         只需要对大于或等于0x80的内码转换（小于0x80也可以转换，浪费读flash）
 *******************************************************************************/
uint16 romf_multi_char_to_unicode(uint16 code_rename)
{
    uint16 unicode;

    /*初始化多国语言到Unicode的转换表的句柄*/
    sys_sd_fseek(MBtoUniTab_fp, SEEK_SET, (uint32) (code_rename << 1));
    sys_sd_fread(MBtoUniTab_fp, &unicode, 2);

    return unicode;
}

/******************************************************************************/
/*!
 * \par  Description:
 *    从字库文件中读取字符的点阵
 * \param[in]    code_rename：字符码值
 * \param[out]   picbuf：保存字符点阵的buffer地址
 * \return       none
 * \note
 *******************************************************************************/
void romf_get_font_point(uint16 code_rename, uint8 *picbuf)
{
    sys_sd_fseek(FontLib_fp, SEEK_SET, (uint32) (code_rename * FONT_ONE_SIZE));
    sys_sd_fread(FontLib_fp, picbuf, FONT_ONE_SIZE);
}

/******************************************************************************/
/*!
 * \par  Description:
 *    显示单个字符，支持不完整字符显示
 * \param[in]    psoure_buf：字符字模缓冲区，33 个字节（16*16，buf[32]表示宽度）
 * \param[in]    mode：字符填充模式
 * \param[in]    char_disp：字符（字模）显示模式
 * \param[in]    start_col：字符（字模）显示开始位置
 * \param[out]   none
 * \return       none
 * \note         1）start_col 不能大于等于字符宽度。
 *               2）必须在外面把 char_region 拷贝到 全局 region 中。
 *******************************************************************************/
bool romf_put_char(uint8 *psoure_buf, char_fill_mode_e mode, char_disp_e char_disp, uint8 start_col)
{
#ifndef PC
    uint8 *display_buffer = (uint8 *) display_buf_addr;
#else
    uint8 *display_buffer = GET_REAL_ADDR(display_buf_addr);
#endif
    region_t buf_region;

    //直接打点模式，当LCD无法读屏时使用
    if (mode == CHAR_FILL_POINT)
    {
        char_fill(psoure_buf, char_disp, start_col);
    }
    //先把字符缓冲到显示缓冲区中，再DMA显示出来
    else
    {
        //计算缓冲区预设区域
        buf_region.x = 0;
        buf_region.y = 0;
        buf_region.height = UNICODE_HEIGHT_DEF;
        buf_region.width = region.width;

        //读取字符串背景图片，并设窗
        read_str_bgpic(display_buffer, &region, mode);

        black_char_to_color(psoure_buf, &buf_region, char_disp, start_col);
        //设置字符串显示参数
        set_lcd_param_hook(&region, LCD_PARAM_STRING);
        //传送缓冲区数据到DMA中
        if (dma_buffer_data_hook(buf_region.width * buf_region.height, buf_region.width) == FALSE)
        {
            return FALSE;//dma传输失败
        }
    }

    return TRUE;
}

/******************************************************************************/
/*!
 * \par  Description:
 *    显示单行字符串
 * \param[in]    desc：字符串描述符
 * \param[in]    str_region：单行字符串显示区域
 * \param[in]    mode：字符填充模式
 * \param[out]   none
 * \return       返回字符串显示结束指针
 * \note         1）要求传进来的字符串编码必须是最终用于显示的编码。
 *               2）显示区域是最终用于显示完整字符部分字符串的区域。
 *               3）str_region->height必须为UNICODE_HEIGHT_DEF。
 *               4）desc->result 返回单行字符串显示终止原因。
 *******************************************************************************/
uint8 *romf_put_string_line(string_desc_t *desc, region_t *str_region, char_fill_mode_e mode)
{
    char_type_e (*get_next_char_type)(uint8 *str, uint16 length, int8 language, uint8 *byte_cnt);
#ifndef PC
    uint8 *display_buffer = (uint8 *) display_buf_addr;
#else
    uint8 *display_buffer = GET_REAL_ADDR(display_buf_addr);
#endif
    uint8 *str;
    uint16 length;
    int8 language;
    uint8 char_bytes;
    region_t buf_region;
    uint16 this_width;//本次实际缓冲区字符串宽度
    uint16 total_width;//字符串已显示区域长度
    char_type_e char_type;
    uint8 fontwidth;
    bool draw_over = FALSE;
    bool remain_region_flag = TRUE;

    //基本参数初始化
    total_width = 0;
    str = desc->data.str;
    length = desc->length;
    language = desc->language;
    if (language == UNICODELANGUAGE)
    {
        get_next_char_type = get_next_char_type_unicode;
    }
    else
    {
        get_next_char_type = get_next_char_type_ansi;
    }

    //直接打点模式
    if (mode == CHAR_FILL_POINT)
    {
        libc_memcpy(&region, str_region, sizeof(region_t));

        //逐个字符处理整串字符串
        while (1)
        {
            //字符串数据已经完毕
            if (length == 0)
            {
                desc->result = STRING_TAIL_LEN_ZERO;
                break;
            }
            //获取下一个字符，分为结束符，换行符和普通字符
            char_type = get_next_char_type(str, length, language, &char_bytes);
            if ((char_type == CHAR_EOF) || (char_type == CHAR_NOPEND))//结束符或者不够数据显示
            {
                desc->result = STRING_TAIL_EOF_NOPEND;
                break;
            }
            else if (char_type == CHAR_UNI_FLAG)
            {
                str += 2;
                length -= 2;
                continue;
            }
            else if (char_type == CHAR_LR)//换行符
            {
                str += char_bytes;
                desc->result = STRING_TAIL_CHAR_LR;
                break;
            }
            else
            {
                //do nothing for QAC
            }

            //考虑nor 方案裁减字库或内码字库
            char_bytes = get_next_char_point_hook(str, language, char_point_buffer);
            fontwidth = char_point_buffer[32];

            //检查显示区域（空间）是否足够显示
            if ((fontwidth > region.width)//当前字符显示超出 region 窗口
             && ((region.x + fontwidth) > (region_win.x + region_win.width)))//当前字符显示超出物理X轴
            {
                desc->result = STRING_TAIL_REGION_ERR;
                break;
            }//2012-10-30 17:04 此处有bug，导致显示越界，应把 && 改为 ||，修复为 fixf_put_string_line

            //直接打点模式，当LCD无法读屏时使用
            char_fill(char_point_buffer, CHAR_DISP_NORMAL, 0);

            //指针递增
            str += char_bytes;
            length -= char_bytes;
        }
    }
    //其他缓冲显示模式
    //根据缓冲区大小和显示模式，字符串可能会分多次显示
    //每次显示都先缓冲到显示buffer，再一次性DMA到GRAM中，提升显示效率
    else
    {
        while (1)
        {
            //读字符串背景到缓冲区中
            //计算缓冲区预设区域
            buf_region.x = 0;
            buf_region.y = 0;
            buf_region.height = UNICODE_HEIGHT_DEF;
            buf_region.width = ((display_buf_len / 2) / UNICODE_HEIGHT_DEF);
            if (buf_region.width > DISP_MAX_WIDTH)//最多160，所需缓冲区为16*160*2=5KB
            {
                buf_region.width = DISP_MAX_WIDTH;
            }
            if (buf_region.width > (str_region->width - total_width))//缓冲区最大刚好完整覆盖（剩余）region
            {
                buf_region.width = (str_region->width - total_width);
                remain_region_flag = FALSE;
            }

            //读取字符串背景图片，并设窗
            region.x = str_region->x + total_width;
            region.y = str_region->y;
            region.width = buf_region.width;
            region.height = buf_region.height;
            read_str_bgpic(display_buffer, &region, mode);

            //处理一次部分字符串
            this_width = 0;//累计每段实际缓冲区宽度
            while (1)
            {
                //字符串数据已经完毕
                if (length == 0)
                {
                    draw_over = TRUE;
                    desc->result = STRING_TAIL_LEN_ZERO;
                    break;
                }
                //获取下一个字符，分为结束符，换行符和普通字符
                char_type = get_next_char_type(str, length, language, &char_bytes);
                if ((char_type == CHAR_EOF) || (char_type == CHAR_NOPEND))//结束符或者不够数据显示
                {
                    draw_over = TRUE;
                    desc->result = STRING_TAIL_EOF_NOPEND;
                    break;
                }
                else if (char_type == CHAR_UNI_FLAG)
                {
                    str += 2;
                    length -= 2;
                    continue;
                }
                else if (char_type == CHAR_LR)//换行符
                {
                    str += char_bytes;
                    draw_over = TRUE;
                    desc->result = STRING_TAIL_CHAR_LR;
                    break;
                }
                else
                {
                    //do nothing for QAC
                }

                //考虑nor 方案裁减字库或内码字库
                char_bytes = get_next_char_point_hook(str, language, char_point_buffer);
                fontwidth = char_point_buffer[32];

                //检查显示区域（空间）是否足够显示
                if ((region.x + this_width + fontwidth) > (region_win.x + region_win.width))//超出物理X轴
                {
                    draw_over = TRUE;
                    desc->result = STRING_TAIL_REGION_ERR;
                    break;
                }

                //再检查缓冲区是否够存放（同时也检测了当前 region 窗口是否够存放）
                if ((this_width + fontwidth) > buf_region.width)
                {
                    //如果没有剩余 region，即可结束显示处理
                    if (remain_region_flag == FALSE)
                    {
                        draw_over = TRUE;
                        desc->result = STRING_TAIL_REGION_ERR;
                    }
                    break;
                }

                //先把字符缓冲到显示缓冲区中
                black_char_to_color(char_point_buffer, &buf_region, CHAR_DISP_NORMAL, 0);
                //字符宽度累计
                this_width += fontwidth;
                //坐标移到下一个字的左上角
                buf_region.x += fontwidth;

                //指针递增
                str += char_bytes;
                length -= char_bytes;
            }

            //只有在有像素点显示时才需要调用下面程序显示
            if (this_width > 0)
            {
                //显示部分字符串
                //设置字符串显示参数
                set_lcd_param_hook(&region, LCD_PARAM_STRING);
                //传送缓冲区数据到DMA中
                if (dma_buffer_data_hook(buf_region.width * buf_region.height, buf_region.width) == FALSE)
                {
                    desc->result = STRING_TAIL_OTHER_ERR;
                    break;//dma传输失败
                }

                //累加 total_width
                total_width += this_width;
            }

            //字符串显示结束
            if (draw_over == TRUE)
            {
                //更新 region为剩余显示区域
                region.x = str_region->x + total_width;
                region.y = str_region->y;
                region.width = str_region->width - total_width;
                break;
            }
        }
    }

    return str;
}
/*! \endcond */

/*! \cond UI_DRIVER_API */
/******************************************************************************/
/*!
 * \par  Description:
 *    获取字符串长度
 * \param[in]    desc：字符串描述符
 * \param[in]    max_length：字符串最大长度，当扫描到字符串长度大于该长度则终止返回
 * \param[out]   result：附带结果指针，可以为空
 * \return       字符串长度（像素点）
 * \ingroup      string
 * \note         1）阿拉伯必须连写但无需逆序。
 *               2）desc->result 返回计算字符串长度终止原因。
 *******************************************************************************/
uint16 romf_get_string_length(string_desc_t *desc, uint16 max_length, str_getlen_result_t *result)
{
    char_type_e (*get_next_char_type)(uint8 *str, uint16 length, int8 language, uint8 *byte_cnt);
    uint8 *str;
    uint16 length;
    int8 language;
    uint8 char_bytes;
    uint16 str_width = 0;//字符串长度（像素点）
    char_type_e char_type;

    //基本参数初始化
    str = desc->data.str;
    length = desc->length;
    language = desc->language;
    if (language == UNICODELANGUAGE)
    {
        get_next_char_type = get_next_char_type_unicode;
    }
    else
    {
        get_next_char_type = get_next_char_type_ansi;
    }
    if (result != NULL)
    {
        result->true_length = 0;
        result->true_bytes = 0;
    }

    //单行字符串获取字符串长度
    while (1)
    {
        //字符串数据已经完毕
        if (length == 0)
        {
            desc->result = STRING_TAIL_LEN_ZERO;
            break;
        }
        //获取下一个字符，分为结束符，换行符和普通字符
        char_type = get_next_char_type(str, length, language, &char_bytes);
        if ((char_type == CHAR_EOF) || (char_type == CHAR_NOPEND))//结束符或者不够数据显示
        {
            desc->result = STRING_TAIL_EOF_NOPEND;
            break;
        }
        else if (char_type == CHAR_UNI_FLAG)
        {
            str += 2;
            length -= 2;
            if (result != NULL)
            {
                result->true_bytes += 2;
            }
            continue;
        }
        else if (char_type == CHAR_LR)//换行符
        {
            desc->result = STRING_TAIL_CHAR_LR;
            break;
        }
        else
        {
            //do nothing for QAC
        }

        //考虑nor 方案裁减字库或内码字库
        char_bytes = get_next_char_point_hook(str, language, char_point_buffer);

        //字符串宽度累计
        str_width += char_point_buffer[32];
        if (str_width > max_length)//已经超过最大长度，无需再计算
        {
            desc->result = STRING_TAIL_REGION_ERR;
            break;
        }

        //指针递增
        str += char_bytes;
        length -= char_bytes;

        if (result != NULL)
        {
            result->true_length += char_point_buffer[32];
            result->true_bytes += char_bytes;
        }
    }

    return str_width;
}
/*! \endcond */
