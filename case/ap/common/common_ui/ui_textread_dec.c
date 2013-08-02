/*******************************************************************************
 *                              us212A
 *                            Module: commonUI-textread_view
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 *
 * History:         
 *      <author>    <time>           <version >             <desc>
 *       lzcai     2011-9-20 10:47     1.0             build this file 
*******************************************************************************/
/*!
 * \file     _ui_textread_dec.c
 * \brief    文本文件解码及显示实现。
 * \author   lzcai
 * \par      GENERAL DESCRIPTION:
 *               实现了文本文件解码和显示。
 * \par      EXTERNALIZED FUNCTIONS:
 *               kernel，libc，applib，等等。
 * \version  1.0
 * \date     2011-9-19
*******************************************************************************/

#include "common_ui.h"

/*! \cond */

//文本解码缺页处理，从文件中读取一个扇区的文本数据
static bool __read_text(text_decode_t *text_decode, text_file_t *text_file)
{
    int32 file_sector;
    uint32 pos_in_sector;
    bool file_miss = (bool)(((text_file->file_offset / 512) + 1) >= text_file->file_sectors);

    //转换为扇区和扇区内偏移地址
    file_sector = (int32)(text_file->file_offset / 512);
    pos_in_sector = text_file->file_offset % 512;
    
    //备份尾行数据
    libc_memcpy(text_decode->text_prev, text_decode->text_buf + 512 - BUFF_ONE_ROW, BUFF_ONE_ROW);
    
    //读取扇区内容
    if(text_file->fseek(text_file->file_handle, SEEK_SET, file_sector) == FALSE)
    {
        return FALSE;
    }
    if(text_file->fread(text_file->file_handle, text_decode->text_buf, 1) == FALSE)
    {
        return FALSE;
    }
    
    //计算有效数据起始位置
    if(text_decode->remain != 0)
    {
        if(BUFF_ONE_ROW < text_decode->remain)//for QAM
        {
            return FALSE;
        }
        text_decode->text_buf_valid = text_decode->text_prev + (BUFF_ONE_ROW - text_decode->remain);
    }
    else
    {
        text_decode->text_buf_valid = text_decode->text_buf + pos_in_sector;
    }
    
    if (file_miss == TRUE)
    {
        text_decode->text_buf_length = (uint16)(text_file->file_length-text_file->file_offset + text_decode->remain);
    }
    else
    {
        text_decode->text_buf_length = (uint16)(512 - (text_file->file_offset % 512) + text_decode->remain);
    }
    
    return TRUE;
}

//判断当前符号类型是否换行符
static bool __get_lr_char(uint8 *tmp_str, uint16 tmp_length, int8 language, uint8 *col)
{
    uint16 tmp_code, next_code;
    
    if(language == UNICODEDATA)
    {
        if(tmp_length < 2)
        {
            *col = 0;
            return FALSE;
        }
        tmp_code = *tmp_str + ((uint16) (*(tmp_str + 1)) << 8);
        if (tmp_code == 0xfeff)
        {
            *col = 2;
            return TRUE;
        }
        else if (tmp_code == 0x0a)//linux
        {
            *col = 2;
            return TRUE;
        }
        else
        {
            //do nothing for QAC
        }

        if (tmp_length >= 4)
        {
            next_code = *(tmp_str + 2) + ((uint16) (*(tmp_str + 3)) << 8);
            if ((tmp_code == 0x0d) && (next_code == 0x0a))//windows
            {
                *col = 4;
                return TRUE;
            }
            else if (tmp_code == 0x0d)//mac
            {
                *col = 2;
                return TRUE;
            }
            else
            {
                //do nothing for QAC
            }
        }
        else
        {
            if (tmp_code == 0x0d)
            {
                *col = 0;
                return FALSE;
            }
        }
    }
    else//UTF-8 & ANSI
    {
        if(tmp_length < 1)
        {
            *col = 0;
            return FALSE;
        }

        if(tmp_length >= 3)
        {
            if( (*tmp_str == 0xef)
             && (*(tmp_str + 1) == 0xbb)
             && (*(tmp_str + 2) == 0xbf) )
            {
                *col = 0;
                return TRUE;
            }
        }
        
        if (*tmp_str == 0x0a)//linux
        {
            *col = 1;
            return TRUE;
        }
        if (tmp_length >= 2)
        {
            if ((*tmp_str == 0x0d) && (*(tmp_str + 1) == 0x0a))//windows
            {
                *col = 2;
                return TRUE;
            }
            else if (*tmp_str == 0x0d)//mac
            {
                *col = 1;
                return TRUE;
            }
            else
            {
                //do nothing for QAC
            }
        }
        else
        {
            if (*tmp_str == 0x0d)
            {
                *col = 0;
                return FALSE;
            }
        }
    }
    
    *col = 0xff;
    return FALSE;
}

/*! \endcond */

/*! \cond COMMON_API */

/******************************************************************************/
/*!                    
 * \par  Description:
 *    解码一页文本数据，并显示出来（按照用户自定义的方式显示）。
 * \param[in]    text_decode 文本解码结构体
 * \param[in]    text_file 文本文件句柄
 * \param[out]   page_bytes 当前页字节数，读取数据失败则返回 -1 
 * \return       text_end_mode_e
 * \retval           返回当前文本页解码结束原因，见 text_end_mode_e 定义。
 * \ingroup      controls
 * \par          exmaple code
 * \code
    例子1：SD文件系统文本阅读示例
    extern void show_text_line(string_desc_t *string_infor, uint8 line);
    text_decode_t text_decode;
    text_file_t text_file;
    uint16 page_bytes;
    text_end_mode_e end_mode;
    
    text_file.file_handle = sd_file_handle;//已经打开的SD文本文件
    text_file.file_length = sd_file_length;//获取到的SD文本文件长度
    text_file.file_sectors = sd_file_length / 512;
    if(sd_file_length % 512 != 0)
    {
       text_file.file_sectors++;
    }
    text_file.fseek = sd_fseek_sector;
    text_file.fread = sd_fread_sector;
    
    text_file.file_offset = 0;       
    text_decode.valid = FALSE; 
    text_decode.remain = 0; 
    
    text_decode.param.language = UNICODEDATA;
    text_decode.param.line_count_one_page = 10;
    text_decode.param.max_width_one_line = ROW_MAX_WIDTH;
    text_decode.param.mode = FILTER_NONE_BLANK | DIVIDE_WORD_ENABLE;
    
    text_decode.text_show_line = show_text_line;
    
    text_file.file_offset = g_file_offset;
    end_mode = text_decode_one_page(&text_decode, &text_file, &page_bytes);
    g_file_offset += page_bytes;
    text_decode.valid = TRUE;
 * \endcode
 * \note
 * \li  text_decode_one_page 不会去处理文本文件数据是否读完，上层应自己判断处理。
 * \li  text_file->file_offset 不会自动被更新，同样上层应自己处理。
*******************************************************************************/
text_end_mode_e text_decode_one_page(text_decode_t *text_decode, text_file_t *text_file, uint16 *page_bytes)
{
    uint16 text_buf_index;
    uint16 view_total_bytes;
    string_desc_t str_desc;
    text_show_param_t *param = &(text_decode->param);
    bool file_miss; //文件是否已读取完毕
    text_end_mode_e end_mode = TEXT_END_INIT;
    uint8 row = 0;
    
    str_desc.argv = param->max_width_one_line;
    str_desc.language = param->language;
    
    view_total_bytes = 0;
    text_buf_index = 0;
    
    if(text_decode->valid == TRUE)
    {
        text_decode->text_buf_valid = text_decode->text_buf + (text_file->file_offset % 512);
    }
    
    while (1)
    {
        if(row >= param->line_count_one_page)//已显示整页，退出
        {
            break;
        }
        
        if (text_decode->valid == FALSE)
        {
            //读回数据
            if(__read_text(text_decode, text_file) == FALSE)
            {
                *page_bytes = V_U16_INVALID;
                return TEXT_END_READ_ERR;
            }
            
            text_decode->valid = TRUE;
            text_decode->remain = 0;
            text_buf_index = 0;
        }
        
        // 初始化参数
        str_desc.data.str = text_decode->text_buf_valid + text_buf_index;
        str_desc.length = text_decode->text_buf_length;
        file_miss = (bool)(((text_file->file_offset / 512) + 1) >= text_file->file_sectors);
        
        end_mode = TEXT_END_INIT;
        // 过滤行开头换行符
        if(((param->mode) & FILTER_BLANK_OPTION) == FILTER_BLANK_ENABLE)
        {
            uint8 tmp_col;
            
            while(__get_lr_char(str_desc.data.str, str_desc.length, param->language, &tmp_col) == TRUE)
            {
                str_desc.data.str += tmp_col;
                str_desc.length -= tmp_col;
                //缓冲区数据长度更新
                text_decode->text_buf_length -= tmp_col;
                text_buf_index += tmp_col;
            }
            if(tmp_col == 0)//缺页
            {
                end_mode = TEXT_END_PAGE_MISS;
            }
        }
        
        //初始状态，没有断行结束
        if(end_mode == TEXT_END_INIT)
        {
            // 自动断行
            end_mode = (text_end_mode_e)(uint32)ui_get_text_line(&str_desc, param->mode, NULL);
        }
        
        //缺页并且尚未到文件末尾
        if((end_mode == TEXT_END_PAGE_MISS) && (file_miss != TRUE))//缺页，且后面尚有数据
        {
            view_total_bytes += text_buf_index;
            text_decode->remain = (uint8)text_decode->text_buf_length;     
            text_file->file_offset = ((text_file->file_offset / 512) + 1) * FILE_SECTOR;
            text_decode->valid = FALSE;
            continue; //读回后续扇区内容后继续断行
        }
        //遇到结束符或换行符，或字符串超长，或缺页且文件已读取完
        else
        {
            //累加当前行显示字节数
            text_decode->text_buf_length -= str_desc.result;
            text_buf_index += str_desc.result;

            if (text_decode->text_show_line != NULL)//提供显示函数，显示当前行字符串
            {
                str_desc.length = str_desc.result;
                text_decode->text_show_line(&str_desc, row);
            }

            if(end_mode == TEXT_END_NUL)//读到结束符，应该终止解码
            {
                break;
            }

            //缺页且文件已读取完，应该终止解码
            if((end_mode == TEXT_END_PAGE_MISS) && (file_miss == TRUE))
            {
                break;
            }
        }
        
        row++;//至此才能确定处理完一行
    }

    //累加最后一次读回的文本数据显示了的字节数
    view_total_bytes += text_buf_index;
    //如果最后剩余有效数据大于 1 个扇区，表示缓冲区有效数据指针超出 text_decode->text_buf；
    //为了简化流程，直接丢掉有效数据，下次进来再读回。（该情况概率非常小，不会影响效率）
    if(text_decode->text_buf_length > 512)
    {
        text_decode->valid = FALSE;
    }
    
    *page_bytes = view_total_bytes;
    return end_mode;
}

/*! \endcond */
