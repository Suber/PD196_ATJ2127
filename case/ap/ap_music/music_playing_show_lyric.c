/*******************************************************************************
 *                              US212A
 *                            Module: SHOW_LYRIC
 *                 Copyright(c) 2003-2009 Actions Semiconductor
 *                            All Rights Reserved.
 *
 * History:
 *      <author>         <time>             <version>             <desc>
 *      wuyufan    2011-9-25 19:48:17           1.0              build this file
 *******************************************************************************/

#include "app_music.h"
#include "app_music_show_lyric.h"

#define MAX_LRC_TIME  0xff000000

lyric_decode_t g_lyric_decode _LCDBUF_DATA_ATTR_;

/* 读取一个时间标签歌词内容缓冲区 */
uint8 lrc_buf[LRC_BUF_LEN] _LCDBUF_DATA_ATTR_;
/* 一个时间标签歌词显示内容缓冲区 */
uint8 display_buf[LRC_BUF_LEN] _LCDBUF_DATA_ATTR_;

/******************************************************************************/
/*!
 * \par  Description:
 *	  判断当前符号类型是否换行符
 * \param[in]
 * \param[out]
 * \return
 * \retval
 * ingroupmusic_playing_show_lyric .c
 * note
 *******************************************************************************/
static bool _filter_lr_char(uint8 *tmp_str, uint16 tmp_length, uint8 *col)
{
    //uint16 tmp_code, next_code;

    if (tmp_length < 1)
    {
        *col = 0;
        return FALSE;
    }

    if (*tmp_str == ' ')
    {
        *col = 1;
        return TRUE;
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

    *col = 0xff;
    return FALSE;
}

/******************************************************************************/
/*!
 * \par  Description:
 *	  处理当前歌词，还是上一句歌词或下一句歌词
 * \param[in]
 * \param[out]
 * \return
 * \retval
 * ingroupmusic_playing_show_lyric .c
 * note
 *******************************************************************************/

void parse_lyric(lyric_decode_t *lyric_decode)
{
    text_end_mode_e end_mode;

    //行计数
    uint8 row, i;

    //分屏计数
    uint8 screen_counter = 0;
    uint32 cur_time;
    uint32 next_time;
    uint32 between_time = 0;

    bool file_end = FALSE;

    //歌词解析配置属性
    lyric_show_param_t *param = &(lyric_decode->param);

    string_desc_t str_desc;

    str_desc.argv = param->max_width_one_line;
    str_desc.language = param->language;

    deal_lryic: lyric_decode->output_start[screen_counter] = lyric_decode->output_length;
    for (row = 0; row < param->line_count_one_page; row++)
    {
        //初始化参数
        str_desc.data.str = lyric_decode->input_buffer + lyric_decode->input_length;
        str_desc.length = lyric_decode->input_remain;

        // 过滤行开头换行符和空格符
        if (((param->mode) & LRC_FILTER_BLANK_OPTION) == LRC_FILTER_BLANK_ENABLE)
        {
            uint8 tmp_col;

            while (_filter_lr_char(str_desc.data.str, str_desc.length, &tmp_col) == TRUE)
            {
                str_desc.data.str += tmp_col;
                str_desc.length -= tmp_col;
                //缓冲区数据长度更新
                lyric_decode->input_length++;
                lyric_decode->input_remain--;
            }
        }

        // 自动断行
        end_mode = (text_end_mode_e) (uint32) ui_get_text_line(&str_desc, param->mode, NULL);

        //防止输出buffer越界
        if ((lyric_decode->output_length + str_desc.result + 1) >= LRC_BUF_LEN)
        {
            str_desc.result = LRC_BUF_LEN - lyric_decode->output_length - 1;
            end_mode = TEXT_END_NUL;
        }

        //更新数据到输出缓冲区
        libc_memcpy(lyric_decode->output_buffer + lyric_decode->output_length, lyric_decode->input_buffer
                + lyric_decode->input_length, (int32)str_desc.result);

        lyric_decode->output_length += str_desc.result;

        //累加当前行显示字节数
        lyric_decode->input_remain -= str_desc.result;
        lyric_decode->input_length += str_desc.result;

        //读到结束符，应该终止解码
        if ((end_mode == TEXT_END_NUL) || (end_mode == TEXT_END_PAGE_MISS))
        {
            //末尾追加换屏符
            lyric_decode->output_buffer[lyric_decode->output_length] = NEXT_SCREEN;
            file_end = TRUE;
            break;
        }
        else
        {
            if (row < (param->line_count_one_page - 1))
            {
                //末尾追加换行符
                lyric_decode->output_buffer[lyric_decode->output_length] = NEXT_LINE;
            }
            else
            {
                //末尾追加换屏符
                lyric_decode->output_buffer[lyric_decode->output_length] = NEXT_SCREEN;
            }

        }

        lyric_decode->output_length++;

    }
    //检测是否允许分频显示
    if ((param->mode & LRC_DIVIDE_SCREEN_OPTION) == LRC_DIVIDE_SCREEN_ENABLE)
    {
        //当前歌词tag是否解析完毕
        if (file_end == FALSE)
        {
            screen_counter++;
            if (screen_counter < LYRIC_MAX_SCREEN)
            {
                g_lyric_multi_screen = TRUE;
                goto deal_lryic;
            }
        }
        else
        {
            //至此歌词解析完毕，划分分频时间点
            if (screen_counter > 0)
            {
                lyric_get_time(LYRIC_CUR_TIME, &cur_time);
                lyric_get_time(LYRIC_NEXT_TIME, &next_time);

                if (next_time == MAX_LRC_TIME)
                {
                    //最后一条歌词超长，则设置时间戳为4s
                    next_time = cur_time + 4000;
                }

                between_time = next_time - cur_time;

                //计算平均间隔时间，以毫秒为单位
                between_time /= (screen_counter + 1);

                for (i = 0; i < LYRIC_MAX_SCREEN; i++)
                {
                    if (i < (screen_counter + 1))
                    {
                        lyric_decode->output_time[i] = g_cur_time + i * between_time;
                    }
                    else
                    {
                        lyric_decode->output_time[i] = 0xffffffff;
                    }
                }
            }

        }
    }

    return;
}

/******************************************************************************/
/*!
 * \par  Description:
 * 初始化歌词显示参数
 * param[in] null
 * param[out] null
 * return
 * retval
 * ingroup music_playing_show_lyric.c
 * note
 *******************************************************************************/

void lyric_param_init(void)
{
    g_lyric_decode.input_buffer = lrc_buf;
    g_lyric_decode.output_buffer = display_buf;
    g_lyric_decode.input_length = 0;
    g_lyric_decode.input_remain = (uint16) libc_strlen(lrc_buf);
    g_lyric_decode.output_length = 0;

    g_lyric_decode.param.line_count_one_page = LYRIC_LINE_COUNT_ONE_PAGE;
    g_lyric_decode.param.max_width_one_line = LYRIC_MAX_WIDTH_ONE_LINE;
    g_lyric_decode.param.mode = (LRC_FILTER_BLANK_ENABLE | LRC_DIVIDE_WORD_ENABLE | LRC_DIVIDE_SCREEN_ENABLE);
    g_lyric_decode.param.language = (int8) g_setting_comval.g_comval.language_id;

}

