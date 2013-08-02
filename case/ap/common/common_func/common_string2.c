/*******************************************************************************
 *                              us212A
 *                            Module: Common
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 *
 * History:         
 *      <author>    <time>           <version >             <desc>
 *       lzcai     2011-9-20 9:42     1.0             build this file 
*******************************************************************************/
/*!
 * \file     _common_string2.c
 * \brief    字符串公共功能接口实现。
 * \author   lzcai
 * \par      GENERAL DESCRIPTION:
 *               包括短名加点，等等。
 * \par      EXTERNALIZED FUNCTIONS:
 *               psp
 * \version  1.0
 * \date     2011-9-20
*******************************************************************************/

#include "common_func.h"

uint8 tmp_buffer[52] _BANK_DATA_ATTR_;

/*! \cond COMMON_API */

/******************************************************************************/
/*!                    
 * \par  Description:
 *    为短名添加点号，整理为 xxxxxxxx.yyy 格式。
 * \param[in]    short_name 8+3短名
 * \param[out]   short_name 返回已加点号短文件名
 * \return       none
 * \ingroup      common_func
 * \par          exmaple code
 * \code
    例子1：给短名文件名加.号
    //获取到文件名，保存在 g_filename 中
    if((g_filename[0] != 0xff) || (g_filename[1] != 0xfe))//内码，为短名文件名
    {
        com_dot_to_shortname(g_filename);
    }
    else//Unicode码，长名或真名文件名
    {
        Unicode码文件名处理
    }
 * \endcode
 * \note
 * \li  用户应该保证 short_name 空间不少于 13 字节，否则有可能产生内存溢出错误。
*******************************************************************************/
void com_dot_to_shortname(uint8 *short_name)
{
    uint8 i = 7, result;
    
    //过滤掉空格
    while(short_name[i] == ' ')
    {
        i--;
    }
    
    //拷贝并添加点号
    result = i + 1;
    libc_memcpy(tmp_buffer, short_name, result);
    tmp_buffer[result] = '.';
    result++;
    libc_memcpy(tmp_buffer + result, short_name + 8, 3);
    result += 3;
    tmp_buffer[result] = '\0';
    result++;
    
    //拷贝结果
    libc_memcpy(short_name, tmp_buffer, result);
}

/******************************************************************************/
/*!                    
 * \par  Description:
 *    将内码编码文件名转换为Unicode编码文件名。
 * \param[in]    file_name 内码编码文件名
 * \param[out]   file_name 输出Unicode编码文件名
 * \return       none
 * \ingroup      common_func
 * \par          exmaple code
 * \code
    例子1：将内码编码文件名转换为Unicode编码文件名
    const char const_rec_name[] = "rec_001.wav";
    uint8 new_rec_name[26];
    
    libc_memcpy(new_rec_name, const_rec_name, 12);
    com_ansi_to_unicode(new_rec_name);
    //接着，就可以使用 new_rec_name 来创建名字为 rec_001.wav 的文件了
 * \endcode
 * \note
 * \li  用于创建文件前把文件名转换为Unicode编码，以适应 exFat 文件系统。
 * \li  用户需自己保证 file_name 够存放Unicode编码文件名，比如 英文内码文件名
 *      rec_001.wav，转成Unicode编码文件名，需要缓冲区大小为 26 个字节，即
 *      Unicode编码标志字符0xfffe 2字节 + 11个字符 2 * 11字节 + 结束符0x0 0x0 2字节。
 * \li  受限于辅助缓冲区大小（52字节），文件名不能超过 24 个字符长度。
*******************************************************************************/
void com_ansi_to_unicode(uint8 *file_name)
{
    uint16 ansi_len = (uint16)libc_strlen(file_name);
    uint16 i;
    
    //往后移动2字节，前面添加 0xfffe
    for(i = ansi_len; i > 0; i--)
    {
        file_name[(i - 1) + 2] = file_name[i - 1];
    }
    file_name[0] = 0xff;
    file_name[1] = 0xfe;
    file_name[ansi_len + 2] = 0x0;
    
    //把文件名转换为Unicode编码 com_name_buffer
    ui_char_to_unicode(tmp_buffer, file_name + 2, ansi_len);
    
    //拷贝Unicode编码
    libc_memcpy(file_name + 2, tmp_buffer, ansi_len*2 + 2);
}

/******************************************************************************/
/*!                    
 * \par  Description:
 *    把文件名整理为"部分..."形式。
 * \param[in]    long_name 文件名缓冲区
 * \param[in]    ellipsis_len 结果文件名缓冲区长度
 * \param[out]   ellipsis_name 结果文件名缓冲区
 * \return       uint16 返回整理字符串结果字节长度
 * \ingroup      common_func
 * \par          exmaple code
 * \code
    例子1：把文件名整理为“部分...”形式
    com_ellipsis_to_longname(g_filename, com_name_buffer, 52);
 * \endcode
 * \note
 * \li  如果 longstr 为 UNICODE 编码，用户应该保证开头为 0xfffe 标志。
 * \li  整理结果“部分...”的显示长度最大为 ELLIPSIS_MAX_LEN ，即84个像素点。
*******************************************************************************/
uint16 com_ellipsis_to_longname(uint8 *long_name, uint8 *ellipsis_name, uint16 ellipsis_len)
{
    string_desc_t ellipsis_desc;
    uint16 true_bytes;
    
    libc_memcpy(ellipsis_name, long_name, ellipsis_len);
    
    ellipsis_desc.data.str = ellipsis_name;
    ellipsis_desc.length = -1;
    if((ellipsis_name[0] == 0xff) && (ellipsis_name[1] == 0xfe))
    {
        ellipsis_name[ellipsis_len - 2] = 0;
        ellipsis_name[ellipsis_len - 1] = 0;
        ellipsis_desc.language = UNICODEDATA;
        true_bytes = (uint16)(int)ui_ellipsis_to_longstr(&ellipsis_desc, ELLIPSIS_MAX_LEN, NULL);
    }
    else
    {
        ellipsis_name[ellipsis_len - 1] = 0;
        ellipsis_desc.language = ANSIDATAAUTO;
        true_bytes = (uint16)(int)ui_ellipsis_to_longstr(&ellipsis_desc, ELLIPSIS_MAX_LEN, NULL);
    }
    
    return true_bytes;
}

//数字转换为十六进制字符串itoa_hex
uint8 itoa_hex(uint32 value, uint8 *tmp_buf)
{
    uint8 zero_flag = TRUE;
    uint8 byte_value, byte_high, byte_low;
    uint8 ret_len, j;
    
    //添加前导符0x
    ret_len = 0;
    tmp_buf[ret_len] = '0';
    tmp_buf[ret_len+1] = 'x';
    ret_len += 2;
    
    //4个字节转换为双十六进制字符
    for(j = 0; j < 4; j++)
    {
        byte_value = *((uint8 *)(&value) + (3-j));
        byte_high = (byte_value >> 4);
        byte_low = (byte_value & 0x0f);
        if((zero_flag == TRUE) && (byte_value == 0) && (j < 3))//不显示前置0
        {
            continue;
        }
        
        tmp_buf[ret_len] = (byte_high >= 10)? (byte_high - 10 + 'a'): (byte_high + '0');
        tmp_buf[ret_len+1] = (byte_low >= 10)? (byte_low - 10 + 'a'): (byte_low + '0');
        ret_len += 2;
        zero_flag = FALSE;
    }
    
    return ret_len;
}

/******************************************************************************/
/*!                    
 * \par  Description:
 *    显示打印调试信息到UI上，调试信息显示为黑底白字。
 * \param[in]    x 打印信息X坐标
 * \param[in]    y 打印信息Y坐标
 * \param[in]    format 打印格式
 * \param[in]    value 打印整型数
 * \param[out]   none
 * \return       none
 * \ingroup      common_func
 * \par          exmaple code
 * \code
    例子1：打印格式为 errno = value，value 为 10 进制
    com_dump_debug(0, 40, "errno = %d", fsel_errno);
 * \endcode
 * \note
 * \li  format 中支持一个 %x 或 %d，前者表示16进制，后者表示10进制。
 * \li  format 长度最长为 32 字节，并且只支持 ANSI 编码。
*******************************************************************************/
void com_dump_debug(uint16 x, uint16 y, uint8 *format, uint32 value)
{
    string_desc_t debug_str;
    region_t debug_region;
    uint16 format_len, remain_len;
    uint16 ret_len, i;
    uint8  ch_prev, ch_now;
    uint8  value_len;
    
    format_len = (uint16)libc_strlen(format);
    if(format_len > 32)
    {
        format_len = 32;
    }
    
    //转换为debug
    ret_len = 0;
    ch_prev = 0x0;
    for(i = 0; i < format_len; i++)
    {
        ch_now = format[i];
        if(ch_prev == '%')
        {
            switch(ch_now)
            {
            case 'x':
            case 'd':
                if(ch_now == 'x')
                {
                    value_len = itoa_hex(value, tmp_buffer + ret_len);
                }
                else
                {
                    value_len = libc_itoa(value, tmp_buffer + ret_len, 0);
                }
                ret_len += value_len;
                                
                remain_len = format_len - (i+1);
                if((remain_len > 0) && ((ret_len + remain_len) <= 52))//for QAM
                {
                    libc_memcpy(tmp_buffer + ret_len, format + (i+1), remain_len);
                    ret_len += remain_len;
                }
                break;
                
            default:
                if(ret_len >= 52)
                {
                    return ;//for QAM
                }
                tmp_buffer[ret_len] = ch_prev;
                ret_len++;
                ch_prev = 0x0;
                break;
            }
            
            if((ch_now == 'x') || (ch_now == 'd'))
            {
                break;//要求format为以%x或%d结束，故至此可退出
            }
        }
        else if(ch_now == '%')
        {
            ch_prev = ch_now;
            continue;
        }
        else
        {
            ch_prev = 0x0;
        }
        if(ret_len >= 52)
        {
            return ;//for QAM
        }
        tmp_buffer[ret_len] = ch_now;
        ret_len++;
    }
    
    if(ret_len >= 52)
    {
        return ;//for QAM
    }
    tmp_buffer[ret_len] = 0x0;
    
    //打印debug信息
    debug_str.data.str = tmp_buffer;
    debug_str.language = ANSIDATAAUTO;
    debug_str.length = ret_len;
    
    debug_region.x = x;
    debug_region.y = y;
    debug_region.height = 16;
    debug_region.width = (DISPLAY_LENGTH - x);
    
    ui_set_pen_color(0xffff);
    ui_set_backgd_color(0x0000);
    //非透明模式，丢弃不完整字符，取消阿拉伯右对齐属性
    ui_show_string(&debug_str, &debug_region, CHAR_FILL_COLOR | DISP_LONGSTR_NO_RECT | DISP_ARABIC_DISABLE);
}

/*! \endcond */
