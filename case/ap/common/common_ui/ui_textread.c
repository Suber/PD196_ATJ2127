/*******************************************************************************
 *                              us212A
 *                            Module: commonUI-textread
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 *
 * History:         
 *      <author>    <time>           <version >             <desc>
 *       lzcai     2011-9-19 21:49     1.0             build this file 
*******************************************************************************/
/*!
 * \file     _ui_textread.c
 * \brief    简短文本浏览控件控制流实现。
 * \author   lzcai
 * \par      GENERAL DESCRIPTION:
 *               实现了一种简短文本浏览 GUI。
 * \par      EXTERNALIZED FUNCTIONS:
 *               kernel，libc，applib，等等。
 * \version  1.0
 * \date     2011-9-19
*******************************************************************************/

#include "common_ui.h"

extern bool sd_fseek_sector (sd_handle file_handle, uint8 where, int32 sector_offset);
extern bool sd_fread_sector (sd_handle file_handle, uint8 *text_buf, uint32 sector_len);
extern void show_text_line(string_desc_t *string_infor, uint8 line);

#define HISTORY_MAX_PAGE    32//只支持32页内的简短文本文件

//文本浏览历史缓冲，最多缓冲16页的浏览结果，即offset值
uint16 g_offset_history[HISTORY_MAX_PAGE] _BANK_DATA_ATTR_;
uint16 g_file_offset _BANK_DATA_ATTR_;
uint16 g_file_offset_bk _BANK_DATA_ATTR_;
uint8 g_page_index _BANK_DATA_ATTR_;
bool g_text_end_flag _BANK_DATA_ATTR_;
bool g_need_draw _BANK_DATA_ATTR_;

//文本阅读解码缓冲区结构体全局变量，ui_textread_dec.c 需访问，不要切换出去
text_decode_t text_decode _BANK_DATA_ATTR_;

//文本阅读虚拟文件结构体指针，ui_textread_dec.c 需访问，不要切换出去
text_file_t text_file _BANK_DATA_ATTR_;

//textread gui app msg deal
app_result_e textread_msg_deal(void)
{
    input_gui_msg_t input_msg;
    msg_apps_type_e gui_event;
    private_msg_t private_msg;
    app_result_e result = RESULT_NULL;
    
    //获取gui消息
    if(get_gui_msg(&input_msg) == TRUE)//有gui消息
    {
        //进行按键映射
        if(com_key_mapping(&input_msg, &gui_event, textread_key_map_list) == TRUE)
        {
            //GUI按键消息处理
            switch(gui_event)
            {
            case EVENT_TEXTREAD_NEXT_PAGE:
                //向下翻页
                if(g_text_end_flag == TRUE)
                {
                    //已到末尾
                    break;
                }
                else
                {
                    g_page_index++;
                    g_need_draw = TRUE;
                }
                break;
                
            case EVENT_TEXTREAD_PREV_PAGE:
                //向上翻页
                if(g_page_index == 0)
                {
                    //已到首页
                    break;
                }
                else
                {
                    g_page_index--;
                    g_file_offset = g_offset_history[g_page_index];
                    g_need_draw = TRUE;

                    text_decode.valid = FALSE;
                }
                break;
                
            case EVENT_TEXTREAD_CANCEL:
                //结束文本阅读，退出
                result = RESULT_REDRAW;
                break;
                
            default:
                result = com_message_box(gui_event);
                if(result == RESULT_REDRAW)
                {
                    g_need_draw = TRUE;
                    g_file_offset = g_file_offset_bk;//重绘
                    text_decode.valid = FALSE; 
                    text_decode.remain = 0; 
                    result = RESULT_NULL;//不退出控件
                }
                break;
            }
        }
    } 
    else
    {
        //到此，gui消息队列已经处理完毕
        //获取ap私有消息和系统消息（会先转换为私有消息再返回），并处理
        if(get_app_msg(&private_msg) == TRUE)
        {
            //setting 应用只会响应默认处理的消息，可以用 com_message_box 代替
            result = com_message_box(private_msg.msg.type);
            if(result == RESULT_REDRAW)
            {
                g_need_draw = TRUE;
                g_file_offset = g_file_offset_bk;//重绘
                text_decode.valid = FALSE; 
                text_decode.remain = 0; 
                result = RESULT_NULL;//不退出控件
            }
        }
    }
    
    return result;
}

/*! \cond COMMON_API */

/******************************************************************************/
/*!                    
 * \par  Description:
 *    简短文本浏览控件控制流实现，该接口只支持 SD 文件系统中的文本文件。
 * \param[in]    style_id ui editor 工具设计的文本浏览控件模板
 * \param[in]    file_name 阅读文件名字
 * \param[out]   none    
 * \return       app_result_e
 * \retval           RESULT_REDRAW 正常返回AP，重绘UI
 * \retval           RESULT_ERROR 无法打开文件等，返回错误（除非系统bug，否则不可能返回）
 * \retval           other 收到返回值不为 RESULT_NULL 应用消息返回，见 app_result_e 定义
 * \ingroup      controls
 * \par          exmaple code
 * \code
    例子1：显示样机法律说明书
    const char law_file_name[12] = "legal.txt";
    app_result_e result;
    
    result = gui_text_read(TEXT_READ, law_file_name);
 * \endcode
 * \note 
 * \li  该接口只支持 SD 文件系统中的文本文件，并且文件页数不能超过 32 页。
 * \li  支持 Unicode-16，UTF-8，内码编码（参见 lang_id.h 中定义语言列表）格式。
*******************************************************************************/
app_result_e gui_text_read (uint16 style_id, const char *file_name)
{
    app_result_e result = RESULT_NULL;
    style_infor_t style_infor;
    
    sd_handle sd_file_handle;
    uint32 sd_file_length;
    uint16 page_bytes;
    text_end_mode_e end_mode;
    uint8 tag_backup;
    uint8 mode_backup, icon_backup;

    tag_backup = get_app_timer_tag();
    change_app_timer_tag(APP_TIEMR_TAG_TEXTRD);
    
    gui_get_headbar_mode(&mode_backup, &icon_backup);
    gui_set_headbar_mode(HEADBAR_MODE_NULL, this_headbar_icon_id);
    
    style_infor.style_id = style_id;
    style_infor.type = UI_COM;
    
    //初始化文本文件，完善text_file_t数据结构
    sd_file_handle = sys_sd_fopen(file_name);
    if(sd_file_handle == NULL)
    {
        return RESULT_ERROR;
    }
    
    sys_sd_fread(sd_file_handle, text_decode.text_buf, 512);
    if( (text_decode.text_buf[0] == 0xff)
     && (text_decode.text_buf[1] == 0xfe) )
    {
        text_decode.param.language = UNICODEDATA;
    }
    else if( (text_decode.text_buf[0] == 0xef)
          && (text_decode.text_buf[1] == 0xbb)
          && (text_decode.text_buf[2] == 0xbf) )
    {
        text_decode.param.language = UTF_8DATA;
    }
    else
    {
        text_decode.param.language = (int8)sys_comval->language_id;
    }
    
    sys_sd_fseek(sd_file_handle, SEEK_END, 0);
    sd_file_length = sys_sd_ftell(sd_file_handle);
    sys_sd_fseek(sd_file_handle, SEEK_SET, 0);
    sd_file_length = (sd_file_length - sys_sd_ftell(sd_file_handle) + 1);//首位指针相减加1为文件长度
    
    text_file.file_handle = sd_file_handle;
    text_file.file_length = sd_file_length;
    text_file.file_sectors = sd_file_length / 512;
    if((sd_file_length % 512) != 0)
    {
        text_file.file_sectors++;
    }
    text_file.fseek = sd_fseek_sector;
    text_file.fread = sd_fread_sector;
                
    //初始化历史记录        
    libc_memset(g_offset_history, 0xff, sizeof(g_offset_history));
    g_file_offset = 0;
    g_file_offset_bk = 0;
    g_page_index = 0;
    g_text_end_flag = FALSE;
    g_need_draw = TRUE;
    
    text_file.file_offset = 0;       
    text_decode.valid = FALSE; 
    text_decode.remain = 0; 
    
    text_decode.param.line_count_one_page = 10;
    text_decode.param.max_width_one_line = ROW_MAX_WIDTH;
    text_decode.param.mode = FILTER_BLANK_DISABLE | DIVIDE_WORD_ENABLE;

    text_decode.text_show_line = show_text_line;

    gui_set_headbar_mode(HEADBAR_MODE_NULL, this_headbar_icon_id);
    
    //进入文本浏览循环
    while (1)
    {
        //显示一页文本
        if(g_need_draw == TRUE)
        {
            g_offset_history[g_page_index] = g_file_offset;
            ui_show_picbox(&style_infor, NULL);
            text_file.file_offset = g_file_offset;
            g_file_offset_bk = g_file_offset;
            end_mode = text_decode_one_page(&text_decode, &text_file, &page_bytes);
            if(end_mode == TEXT_END_NUL)//遇到结束符，终止继续往下解码
            {
                g_text_end_flag = TRUE;
            }
            else if(end_mode == TEXT_END_READ_ERR)//读取数据失败，退出
            {
                result = RESULT_ERROR;
                break;//退出控件
            }
            else
            {
                g_file_offset += page_bytes;
                if(g_file_offset >= text_file.file_length)
                {
                    g_text_end_flag = TRUE;
                }
                else
                {
                    g_text_end_flag = FALSE;
                }
            }
            
            text_decode.valid = TRUE;
            g_need_draw = FALSE;
        }
        
        result = textread_msg_deal();
        if(result != RESULT_NULL)
        {
            break;
        }

        //挂起10ms，多任务调度
        sys_os_time_dly(1);
    }
    
    //关闭文本文件
    sys_sd_fclose(sd_file_handle);
    change_app_timer_tag(tag_backup);
    gui_set_headbar_mode(mode_backup, icon_backup);
    return result;
}

/*! \endcond */
