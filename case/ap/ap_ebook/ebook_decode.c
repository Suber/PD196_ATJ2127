/********************************************************************************
 *                              USDK
 *                             Module: ebook
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : ebook_bookmark.c
 * History:
 *      <author>    <time>           <version >             <desc>
 *      stevenluo       2011-11-11         v1.0              create this file
 ********************************************************************************/

#include "ebook.h"
const uint8 str_partition[2] =
{ "/" };





/********************************************************************************
 * Description :电子书阅读界面的循环处理函数
 *
 * Arguments  :
 *               无
 * Notes       :
 *
 ********************************************************************************/

app_result_e _ebook_text_read(void)
{
    //打开*.txt文件
    app_result_e retval;


    need_draw = TRUE;                                   //标记需要更新画面
    gui_set_headbar_mode(HEADBAR_MODE_NORMAL, HEADBAR_ICON_NONE);
#if 0                                                   //避免自动播放图标被刷掉
    gui_headbar(HEADBAR_UPDATE_INIT);
    g_headbar_update = HEADBAR_UPDATE_CHANGE;
#endif
    change_app_state(APP_STATE_PLAYING_ALWAYS);         // 前台AP处于播放 UI 状态，且背光常亮


    draw_auto_flag = TRUE;                              //标记需要更新自动播放

    _show_filename();                                   //显示文件名
 
    /****** 进入多任务里的死循环 *****/
    while (1)                       
    {
        _count_page_num();                              //计算电子书的页数
        
        _display_text();                                //电子书阅读内容显示
        
        retval = _deal_key_msg(NULL, NORMAL_TYPE);      //处理按键和系统消息
        if (retval != RESULT_NULL)
        {
            g_ebook_vars.page_num = curpagenum;
            if (retval != RESULT_EBK_READINGMENU)
            {
                _close_text_file(1);
            }
            change_app_state(APP_STATE_NO_PLAY);
            return retval;
        }
        //多任务调度
        sys_os_time_dly(1);
    }

}

/********************************************************************************
 * Description :更新显示阅读界面下的文件总数和文件序号
 *
 * Arguments  :
 *              无
 * Returns     :
 *                  无
 * Notes       :
 *
 ********************************************************************************/
void _update_text_head(void)
{

    style_infor_t numbox_style;
    numbox_private_t numbox_param;    
    
    /*** 显示页码 ***/
    numbox_style.style_id = TRACK_NUMBER_BOX;
    numbox_style.type = UI_AP;
    numbox_param.value = (int32)(curpagenum + 1);
    numbox_param.total = (int32)page_totalnum;

    if(numboxflag == TRUE)
    {
        numboxflag=FALSE;
        _clear_and_set_head_screen(0xffff, 0x0);    //擦除头部页码
        ui_show_numbox(&numbox_style, &numbox_param, NUMBOX_DRAW_ALL);
    }
    else
    {
        ui_show_numbox(&numbox_style, &numbox_param, NUMBOX_DRAW_NUMBER);
    }

#ifdef PC
    UpdateMainWnd();        //PC上，更新窗口
#endif

}
/********************************************************************************
 * Description :显示电子书的阅读内容
 *
 * Arguments  :
 *               无
 * Notes       :
 *
 ********************************************************************************/

void _display_text(void)
{
    uint32 temp_offset;
    
    if ((draw_page_num == TRUE) || (need_draw == TRUE))
    {       
        _update_text_head();
        draw_page_num = FALSE;
    }
    if (need_draw == TRUE)
    { 
        _clear_and_set_text_screen(0xffff, 0x0);    //擦除文字显示部分的背景

#ifdef PC
        UpdateMainWnd();                            //PC上，更新窗口
#endif 

        view_file.file_offset = file_offset;
        _decode_one_page(&view_decode, &view_file, &temp_offset);   //解码文本，并显示出来
        file_offset += temp_offset;
        need_draw = FALSE;
    }
    _draw_autoplya_icon();
}

/********************************************************************************
 * Description :显示电子书的自动切页的图标
 *
 * Arguments  :
 *               无
 * Notes       :
 *
 ********************************************************************************/
void _draw_autoplya_icon(void)
{
    style_infor_t picbox_style;
    picbox_private_t picbox_param;

    if (draw_auto_flag == TRUE)
    {   
        picbox_style.style_id = AUTO_PLAY_PICBOX;
        picbox_style.type = UI_AP;
        picbox_param.pic_id = -1;//必须为-1

        picbox_param.frame_id = g_ebook_vars.play_flag; //播放标记与UI图层位置刚好一一对应

        ui_show_picbox(&picbox_style, &picbox_param);
#ifdef PC
        UpdateMainWnd();        //PC上，更新窗口
#endif 
        draw_auto_flag = FALSE;
    }
}

/******************************************************************************/
/*!
 * \par  Description:
 *    文本解码缺页处理，从文件中读取一个扇区的文本数据
 * \param[in]    text_decode：文本解码结构体
 * \param[in]    text_file：文本文件句柄
 * \param[out]   none
 * \return       uint16
 * \retval           读取成功返回TRUE，没有文本数据可读或者读取失败返回FALSE
 * \note
 *******************************************************************************/
bool _read_text(view_decode_t *text_decode, view_file_t *text_file)
{
    uint32 file_sector;
    uint32 pos_in_sector;
    bool file_miss = (bool) ((text_file->file_offset / 512 + 1) >= text_file->file_sectors);

    //转换为扇区和扇区内偏移地址
    file_sector = text_file->file_offset / 512;
    pos_in_sector = text_file->file_offset % 512;
    decode_sector = file_sector;
    //备份尾行数据
    libc_memcpy(text_decode->text_prev, text_decode->text_buf + 512 - BUFF_ONE_ROW, BUFF_ONE_ROW);
    libc_memset(text_decode->text_buf, 0x0, FILE_SECTOR);
    //读取扇区内容
    if (text_file->fseek(file_sector, text_file->file_handle) == FALSE)
    {
        return FALSE;
    }
    if (text_file->fread(text_decode->text_buf, text_file->file_handle) == FALSE)
    {
        return FALSE;
    }

    //计算有效数据起始位置
    if ((text_decode->remain != 0) && ((BUFF_ONE_ROW - text_decode->remain) >= 0))
    {
        text_decode->text_buf_valid = text_decode->text_prev + (BUFF_ONE_ROW - text_decode->remain);
    }
    else
    {
        text_decode->text_buf_valid = text_decode->text_buf + pos_in_sector;
    }

    if (file_miss != 0)
    {
        text_decode->text_buf_length = 
                        (uint16) (text_file->file_length - text_file->file_offset 
                        + text_decode->remain );
    }
    else
    {
        text_decode->text_buf_length = (uint16) (512 - (text_file->file_offset % 512) + text_decode->remain);
    }


    return TRUE;
}

/******************************************************************************/
/*!
 * \par  Description:
 *    解码一页文本数据，并显示出来
 * \param[in]    text_decode：文本解码结构体
 * \param[in]    text_file：文本文件句柄
 * \param[in]    display_flag：是否要显示出来
 * \return       uint16
 * \retval           返回当前文本页解码长度，返回-1表示解码失败
 * \note         ui_viewpage不会去处理文本文件数据是否已经读完，上层应该自己保证不会
 *               在文本文档末尾处解码
 *******************************************************************************/
text_end_mode_e _decode_one_page(view_decode_t *text_decode, view_file_t *text_file, uint32 *page_bytes)
{
    uint16 text_buf_index=0;
    uint32 view_total_bytes=0;
    uint32 temp_offset;
    string_desc_t str_desc;                             //字符串描述符结构体
    text_show_param_t *param = &(text_decode->param);   //文本显示参数结构体
    text_end_mode_e end_mode;                           //电子书断行结束类型
    bool file_miss;                                     //文件已读取完毕标记
    uint8 row;                                          //行
    uint8 filter_count = 0;

    str_desc.argv = param->max_width_one_line;
    str_desc.language = param->language;

    temp_offset = text_file->file_offset;
    if (text_decode->valid != 0)
    {
        text_decode->text_buf_valid = text_decode->text_buf + (text_file->file_offset % 512);
    }

    /************************************************************************/
    /*              这个for循环用于每行每行读取，并显示出来                 */ 
    /************************************************************************/
    for (row = 0; row < param->line_count_one_page; row++)
    {
        read_text: if (text_decode->valid == FALSE)
        {
            //读回数据
            if (_read_text(text_decode, text_file) == FALSE)
            {
                view_total_bytes = V_U32_INVALID;
                end_mode = TEXT_END_READ_ERR;
                goto viewpage_exit;
            }

            text_decode->valid = TRUE;
            text_decode->remain = 0;
            text_buf_index = 0;
        }

        // 自动断行
        str_desc.data.str = text_decode->text_buf_valid + text_buf_index;
        str_desc.length = (text_decode->text_buf_length >= 256) ? 255 : text_decode->text_buf_length;
        //过滤回车换行
        filter_count = _filter_enter_line(&str_desc);
        if (filter_count != 0)
        {
            str_desc.data.str += filter_count;
            str_desc.length -= filter_count;
            text_decode->text_buf_length -= filter_count;
            text_buf_index += filter_count;
            if ((temp_offset + text_buf_index) >= text_file->file_length)
            {
                //过滤的是第一行，并且已经到了文件尾了，则总页数减1，
                //并返回0xFFFFFFFF做为标记
                view_total_bytes = V_U32_INVALID;
                goto viewpage_exit;
            }
        }
           
        file_miss = (bool) ((text_file->file_offset / 512 + 1) >= text_file->file_sectors);

        end_mode = (text_end_mode_e) (uint32) ui_get_text_line(&str_desc, (uint32) param->mode, NULL);
        // 如果缺页并且尚未到文件末尾
        if ((end_mode == TEXT_END_PAGE_MISS) && (file_miss == 0))
        {
            view_total_bytes += text_buf_index;
            text_decode->remain = (uint8) text_decode->text_buf_length;
            text_file->file_offset = (text_file->file_offset / 512 + 1) * FILE_SECTOR;
            text_decode->valid = FALSE;
            goto read_text;
            //读回后续扇区内容后继续断行
        }
        else
        {
            //累加当前行显示字节数
            text_decode->text_buf_length -= (uint8) (str_desc.result);
            text_buf_index += (uint8) (str_desc.result);

            if (text_decode->text_show_line != NULL)
            {
                str_desc.length = str_desc.result;
                text_decode->text_show_line(&str_desc, row);    //text 显示行回调函数：显示更新一行数据内容
                sys_os_time_dly(1);
            }


            if (end_mode == TEXT_END_NUL)           //读到结束符，应该终止解码
            {
                view_total_bytes = text_file->file_length - decode_offset;
                break;                              //直接让其指先结尾
            }   

            //缺页且文件已读取完，应该终止解码
            if ( (end_mode == TEXT_END_PAGE_MISS )&& (file_miss == TRUE) )
            {
                break;
            }
        }

        if ((row == 0) && (text_decode->text_show_line != NULL))
        {
            libc_memcpy(BOOKMARK_filename, str_desc.data.str, BMK_NAME_LENGTH);
        }
    }



    //累加最后一次读回的文本数据显示了的字节数
    view_total_bytes += text_buf_index;
    if (text_decode->text_buf_length > 512)
    {
        text_decode->valid = FALSE;
    }
    viewpage_exit: 
    *page_bytes = view_total_bytes;
    return TEXT_END_PAGE_MISS;
}


/********************************************************************************
 * Description :过滤回车换行符
 *
 * Arguments  :
 *               str_desc:需过滤的字符串信息
 * Returns     :
 *                  已经过滤掉的字符个数
 * Notes       :
 *
 ********************************************************************************/
uint8 _filter_enter_line(string_desc_t *str_desc)
{
    uint8 str_count = 0;
    uint8 add_offset = 1;
    uint16 test_str;
    uint8 *tmp_str = str_desc->data.str;
    uint16 tmp_length = str_desc->length;
    while (1)
    {
        if (tmp_length == 0)
        {
            break;
        }
        if (str_desc->language == UNICODEDATA)
        {
            test_str = (uint16) ((*(tmp_str + 1)) * 0x100 + *tmp_str);
            add_offset = 2;
        }
        else
        {
            test_str = *tmp_str;
        }
        if ((test_str == 0x0d) || (test_str == 0x0a))
        {
            str_count += add_offset;
            tmp_str += add_offset;
            tmp_length -= add_offset;
        }
        else
        {
            break;
        }
    }
    return str_count;
}


/********************************************************************************
 * Description :显示更新一行数据内容
 *
 * Arguments  :
 *               无
 * Notes       :
 *
 ********************************************************************************/

void _show_text_line(string_desc_t *string_infor, uint8 line)
{
    region_t row_region;
    row_region.x = 0;
    row_region.y = REGION_HEIGHT * line + REGION_HEIGHT;
    row_region.width = REGION_LENGTH;
    row_region.height = REGION_HEIGHT;
    ui_show_string(string_infor, &row_region, DISP_LONGSTR_NO_RECT);
#ifdef PC
    UpdateMainWnd();        //PC上，更新窗口
#endif
}


/********************************************************************************
 * Description :RTC消息处理函数
 *
 * Arguments  :
 *               无
 * Notes       :
 *
 ********************************************************************************/

void _deal_rtc_msg(void)
{
    if (g_ebook_vars.play_flag != 0)
    {
        _sel_next_page(1);
    }
}

#if SHOW_FIFLNAME
/********************************************************************************
 * Description :显示文件名
 *
 * Arguments  :
 *               无
 * Notes       :
 *
 ********************************************************************************/
void _show_filename(void)
{
    style_infor_t picbox_style;                         //背景图片结构
    style_infor_t textbox_style;
    textbox_private_t textbox_param;

    /****** 刷屏文件名区域 *****/
    picbox_style.style_id = FILE_NAME_BACKGD;       
    picbox_style.type = UI_AP;
    ui_show_picbox(&picbox_style, NULL);                //显示背景  

    /****** 显示文件名 *****/
    textbox_style.style_id = FILE_NAME;
    textbox_style.type = UI_AP;
    textbox_param.str_id = -1;
    if((BOOK_filename[0] == 0xff) && (BOOK_filename[1] == 0xfe))   //unicode内容开头是0xff、oxfe
    {
        textbox_param.lang_id = UNICODEDATA ;
    }
    else
    {
        textbox_param.lang_id = (int8)g_comval.language_id;
    } 
    textbox_param.str_value = BOOK_filename;

    ui_show_textbox(&textbox_style, &textbox_param, TEXTBOX_DRAW_NORMAL);
#ifdef PC
    UpdateMainWnd();        //PC上，更新窗口
#endif
}

/********************************************************************************
 * Description :获取文件名
 *
 * Arguments  :
 *               无
 * Notes       :需要先vfs_get_name到data缓存区
 *
 ********************************************************************************/
void _get_filename(uint8 *data,uint32 datalength)
{
    uint8   clean_count=0;                              //用来计数清空".txt"
    //提取文件名（不带 .txt）
    if((BOOKMARK_filename[0] == 0xff) && (BOOKMARK_filename[1] == 0xfe))
    {
        datalength <<= 1;
    }
    
    if (datalength > MAX_FILENAME_LENGTH)
    {
        datalength = MAX_FILENAME_LENGTH;
    } 
    
    while( (data[clean_count++] != '.') && (clean_count < datalength) )  
    {       
        //寻找'.'或者两个'\0'
        if( (data[clean_count]==0 ) && (data[clean_count-1]==0 ))   
        {
            break;                                              //跳出while
        }
        if((data[clean_count]==0x20) && (data[++clean_count]==0x20) )
        {
            
            break;                                              //跳出while
        }
    }
    libc_memcpy(BOOK_filename,data,clean_count);   //复制文件名
    BOOK_filename[clean_count-1]=0;                             //删掉".txt"
    if(clean_count < datalength)
    {
        BOOK_filename[clean_count]=0;                           //删掉".txt"
    } 
}

#endif  //#if SHOW_FIFLNAME
