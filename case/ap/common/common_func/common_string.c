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
 * \file     _common_string.c
 * \brief    字符串公共功能接口实现。
 * \author   lzcai
 * \par      GENERAL DESCRIPTION:
 *               包括格式化字符串组合，等等。
 * \par      EXTERNALIZED FUNCTIONS:
 *               psp
 * \version  1.0
 * \date     2011-9-20
*******************************************************************************/

#include "common_func.h"

static bool __symbol_search(uint8 *str, uint16 len, int8 language, uint16 *scan_len);

//最终有用数据长度为124字节，同 UI 驱动内部缓冲区一致
//这里设置为192字节，是为了用ANSI编码来保存结果时，可以转换更多的UNICODE字符
#define PATTERN_BUF_LEN  192
#define SUB_BUF_LEN      128
//字符串整合缓冲区
uint8 pattern_buf[PATTERN_BUF_LEN] _BANK_DATA_ATTR_;
uint8 sub_buf[SUB_BUF_LEN] _BANK_DATA_ATTR_;

/*! \cond COMMON_API */

/******************************************************************************/
/*!                    
 * \par  Description:
 *    字符串格式化处理，包括拼接处理等
 * \param[in]    pattern 字符串模式，以字符串 ID 存在，通过 pattern->argv 传递，
 *                        其中 [%s] 会替换为后续字符串指针参数指向的字符串。
 * \param[in]    strings 用来替换的字符串描述符数组，如果是code，必须包含结束符。
 * \param[out]   pattern 返回格式化处理结果，结果缓冲区在 pattern->data.str，语言
                         类型由 pattern->lang_id 指定。
 * \return       bool
 * \retval           返回是否处理成功，TRUE表示成功，FALSE表示失败。
 * \ingroup      common_func
 * \par          exmaple code
 * \code
    例子1：把文件名插入到模板字符串中，构成字符串内容：正在删除“文件名”...
    string_desc_t pattern, input_str;
    uint8 pattern_buffer[PATTERN_BUF_LEN];
    
    pattern.argv = S_DELETING;
    pattern.data.str = pattern_buffer;
    pattern.length = PATTERN_BUF_LEN;
    pattern.language = UNICODEDATA;
    
    input_str.data.str = file_name;
    input_str.language = UNICODEDATA;
    input_str.length = -1;//Unicode字符串长度因为难求取，不填写，但要求字符串末尾有结束符\0\0
    
    com_string_format(&pattern, &input_str);
 * \endcode
 * \note
 * \li  字符串整合结果长度受限于 pattern 缓冲区大小（要预留结束符），超出截掉。
 * \li  如果 strings 传进来的字符串为 UNICODEDATA ，那么会被截断为 128 字节。
 * \li  如果 strings 传进来的字符串均为内码编码，则pattern->language 可以指定为
 *      当前语言类型，也可以指定为UNICODEDATA。
 * \li  如果 strings 传进来的字符串有 UNICODEDATA ，那么必须把 pattern->language
 *      指定为UNICODEDATA，以保证多国语言能够正常显示。
*******************************************************************************/
bool com_string_format(string_desc_t *pattern, string_desc_t *strings)
{
    string_desc_t parrern_str;
    uint8 *copy_str;
    uint16 copy_len;
    uint16 max_len;
    uint16 len;
    uint16 total, index, result_index;
    uint16 str_index = 0;
    uint16 result;
    uint8  pt_len;
    bool   ret = FALSE;
    uint8 *result_str = pattern->data.str;
    int8   result_lang = pattern->language;
    bool   is_ansi;
    
    parrern_str.data.str = pattern_buf;
    parrern_str.length = PATTERN_BUF_LEN;
    ui_get_multi_string_unicode(pattern->argv, &parrern_str);
    if(result_lang == UNICODEDATA)
    {
        total = len = (uint16)(int)libc_strlenuni(pattern_buf);
        max_len = pattern->length - 2;
        pt_len = 8;
    }
    else
    {
        ui_unicode_to_char(pattern_buf, parrern_str.length, result_lang);
        total = len = (uint16)libc_strlen(pattern_buf);
        max_len = pattern->length - 1;
        pt_len = 4;
    }
    
    index = 0;
    result_index = 0;
    while(1)
    {
        if(__symbol_search(pattern_buf + index, len, result_lang, &result) == FALSE)
        {
            if((result_index + result) >= max_len)
            {
                libc_memcpy(result_str + result_index, pattern_buf + index, max_len - result_index);
                result_index = max_len;
            }
            else
            {
                libc_memcpy(result_str + result_index, pattern_buf + index, result);
                result_index += result;
            }
            
            break;//扫描完毕，退出
        }
        
        //拷贝 pattern 部分字符串
        if((result_index + result) >= max_len)
        {
            libc_memcpy(result_str + result_index, pattern_buf + index, max_len - result_index);
            result_index = max_len;
            break;//buffer已满，终止处理
        }
        libc_memcpy(result_str + result_index, pattern_buf + index, result);
        result_index += result;
        
        //输入字符串转换
        is_ansi = FALSE;
        if(strings[str_index].language == UNICODEID)
        {
            //多国语言字符串转为Unicode 编码
            parrern_str.data.str = sub_buf;
            parrern_str.length = SUB_BUF_LEN;
            ui_get_multi_string_unicode(strings[str_index].data.id, &parrern_str);
        }
        else if(strings[str_index].language == UNICODEDATA)
        {
            //拷贝SUB_BUF_LEN字节，如果 strings[str_index].data.str 有效长度大于SUB_BUF_LEN-2，那么就会被截掉部分
            libc_memcpy(sub_buf, strings[str_index].data.str, SUB_BUF_LEN);
        }
        else if(strings[str_index].language == UTF_8DATA)
        {
            //UTF-8编码转换为UNICODE编码，末尾已添加结束符
            ui_utf8_to_unicode(strings[str_index].data.str, sub_buf, &copy_len);
        }
        else
        {
            is_ansi = TRUE;
        }
        
        //拷贝输入字符串
        if(result_lang == UNICODEDATA)
        {
            //合并字符串源为内码编码字节流，此种类型可能会导致码流不匹配显示乱码
            if(is_ansi == TRUE)
            {
                copy_len = (uint16)libc_strlen(strings[str_index].data.str);
                ui_char_to_unicode(sub_buf, strings[str_index].data.str, copy_len);
            }
            copy_str = sub_buf;
            copy_len = (uint16)(int)libc_strlenuni(copy_str);
        }
        else
        {
            //合并字符串源为内码编码字节流，此种类型可能会导致码流不匹配显示乱码
            if(is_ansi == TRUE)
            {
                copy_str = strings[str_index].data.str;
            }
            else
            {
                ui_unicode_to_char(sub_buf, SUB_BUF_LEN, result_lang);
                copy_str = sub_buf;
            }
            copy_len = (uint16)libc_strlen(copy_str);
        }
        
        if((result_index + copy_len) >= max_len)
        {
            libc_memcpy(result_str + result_index, copy_str, max_len - result_index);
            result_index = max_len;
            break;//buffer已满，终止处理
        }
        libc_memcpy(result_str + result_index, copy_str, copy_len);
        result_index += copy_len;
        
        len = len - result - pt_len;
        index = total - len;
        str_index++;
    }
    
    result_str[result_index] = 0;
    if(result_lang == UNICODEDATA)
    {
        result_str[result_index + 1] = 0;
    }
    pattern->length = result_index;

    return ret;
}

/*! \endcond */

/*! \cond */

//搜索指代符号[%s]
static bool __symbol_search(uint8 *str, uint16 len, int8 language, uint16 *scan_len)
{
    uint16 i;
    uint16 *str_16;
    
    if(language == UNICODEDATA)
    {
        if(len < 8)
        {
            *scan_len = len;
            return FALSE;
        }
        str_16 = (uint16 *)str;
        for(i = 0; i <= ((len/2) - 4); i++)
        {   
            if(  (str_16[i] == '[')
               &&(str_16[i + 1] == '%')
               &&(str_16[i + 2] == 's')
               &&(str_16[i + 3] == ']') )
            {
                *scan_len = i*2;
                return TRUE;
            }
        }
    }
    else
    {
        if(len < 4)
        {
            *scan_len = len;
            return FALSE;
        }
        for(i = 0; i <= (len - 4); i++)
        {   
            if(  (str[i] == '[')
               &&(str[i + 1] == '%')
               &&(str[i + 2] == 's')
               &&(str[i + 3] == ']') )
            {
                *scan_len = i;
                return TRUE;
            }
        }
    }
    
    *scan_len = len;
    return FALSE;
}

/*! \endcond */
