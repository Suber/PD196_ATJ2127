/********************************************************************************
 *                              USDK
 *                             Module: ebook
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : ebook_main.c
 * History:
 *      <author>    <time>           <version >             <desc>
 *      stevenluo       2011-11-11         v1.0              create this file
 ********************************************************************************/
/* include file of this application, 本应用的头文件 */
#include "ebook.h"

/********************************************************************************
 * Description :电子书阅读界面的入口函数
 *
 * Arguments  :
 * Returns     :
 *                  场景返回值
 * Notes       :
 *
 ********************************************************************************/

app_result_e _ebook_reading(scene_enter_mode_e mode)
{
    app_result_e init_result;
    int8 rtc_time__id;
    uint16 error_id;
    cur_BufSector = -1;

    //如果从阅读界面的设置菜单退回到阅读界面时，不再重新初始化文件信息
    if (view_file.file_handle == 0x0)
    {
        _close_text_file(0);                        //关闭已打开的文件文件句柄
        error_id = _init_file_info();               //文件信息初始化，主要包括文件location信息获取
                                                    //，文件打开获取文件大小，扇区数等信息
        if (error_id != 0)
        {
            _show_single_dialog(error_id, DIALOG_INFOR_WAIT);
            device_status |= 0x01;          //flash没有文件
            if (mode == ENTER_FROM_DESKTOP)
            {
                return RESULT_MAIN_APP;
            }
            else
            {
                return RESULT_EBK_FILELIST;
            }
        }
        
        g_ebook_vars.play_flag=0;                   //进入ebook，应该手动播放 。            
        draw_auto_flag = 0;
    }
    else
    {
        draw_auto_flag = g_ebook_vars.play_flag;    //从菜单进入ebook，应该保留原来播放状态
    }

    
    if (g_ebook_vars.path.file_path.dirlocation.disk == DISK_H)
    {
        change_card_state(CARD_STATE_CARD_PLAY);//改为播放SD卡
    }

    _init_decode_param();                           //文本文件解码参数初始化
    if (curpagenum >= page_totalnum)                //当前阅读文本所处的页数   >=  正在阅读文件的总页数
    {
        curpagenum = 0;
    }
    if (curpagenum != 0)                            //当前阅读文本所处的页数   !=  0
    {
        _get_page_offset(curpagenum);
    }
    else
    {
        file_offset = 0;
    }

    sys_bank_flush(AP_BANK_FRONT_ENHANCED_2);       //用了BANK2数据，需要刷新
    view_decode.param.language = view_file.language;
    page_count_decode.param.language = view_file.language;
    rtc_time__id = set_app_timer(APP_TIMER_ATTRB_UI, (uint16) (1000 * g_ebook_vars.autoplaytime), _deal_rtc_msg);
    decode_sector = 0;
    init_result = _ebook_text_read();               //初始化好后，就开始运行ebook。退出时，返回
                                                    //RESULT_CONFIRM：确认某个事件或操作返回
    kill_app_timer(rtc_time__id);                   //时间刷新的id

    /*保存当前文件路径信息*/
    if (init_result == RESULT_CANCEL)               //操作返回
    {
        if (mode == ENTER_FROM_DESKTOP)             //如果 从main ap 或browser进入场景
        {
            init_result = RESULT_MAIN_APP;          //回到 MainMenu 应用
        }
        else
        {
            init_result = RESULT_EBK_FILELIST;      //回到ebook 的文件列表界面
        }
    }
    if (init_result != RESULT_EBK_SETTINGMENU)      //如果不是返回到 ebook 的菜单界面
    {
        enter_mode = ENTER_FROM_READING;            //标记：从阅读界面进入场景
    }


    if (g_ebook_vars.path.file_path.dirlocation.disk == DISK_H)
    {
        if(get_engine_state() != ENGINE_STATE_PLAYING)  //引擎没正在播放
        {
            change_card_state(CARD_STATE_CARD_IN);
        }
        
    }
    
    return init_result;
}
/********************************************************************************
 * Description :文本文件解码参数初始化
 *
 * Arguments  :
 *              无
 * Returns     :
 *                  无
 * Notes       :
 *
 ********************************************************************************/
void _init_decode_param(void)
{
    view_decode.valid = FALSE;
    view_decode.remain = 0;
    view_decode.param.line_count_one_page = ONE_PAGE_ROW_NUM;       //一屏显示文本的实际行数
    view_decode.param.max_width_one_line = ROW_MAX_WIDTH;           //每行最多显示 ROW_MAX_WIDTH 个像素点宽度
    view_decode.param.mode = 0;                                     //显示模式，包括过滤空行，分词显示等
    view_decode.text_show_line = _show_text_line;                   //显示更新一行数据内容
    view_decode.param.language = view_file.language;                //语言类型
    libc_memcpy(&page_count_decode, &view_decode, sizeof(view_decode_t));
    page_count_decode.text_show_line = NULL;
}

/********************************************************************************
 * Description :切换到下一页
 *
 * Arguments  :

 *                无
 * Notes       :
 *
 ********************************************************************************/

void _sel_next_page(uint8 page_sel)
{
    //向下翻页
    if (curpagenum < (page_totalnum - 1))
    {
        
        if (curpagenum >= (page_totalnum - page_sel))
        {
            curpagenum = page_totalnum - 1;
        }
        else
        {
            curpagenum += page_sel;
        }
        _get_page_offset(curpagenum);                   //获取页码的文件偏移
        if (decode_sector != (file_offset / 512))       //假如文件偏移不在正在阅读的512字节扇区里
        {
            view_decode.valid = FALSE; 
            decode_sector = 0;
        }
        else if (page_sel > 1) 
        {
            view_decode.valid = FALSE;
        }
        else
        {
        }
        need_draw = TRUE;
    }
    else if (page_count_flag == FALSE)                  //如果需要继续计算页数
    {
        curpagenum = page_sel - 1;
        cur_BufSector = -1;
        _get_page_offset(curpagenum);
        view_decode.valid = FALSE;
        need_draw = TRUE;
    }
    else
    {
    }
}

/********************************************************************************
 * Description :切换到上一页
 *
 * Arguments  :
 *               无
 * Notes       :
 *
 ********************************************************************************/
void _sel_prev_page(uint8 page_sel)
{
    //向上翻页
    if (curpagenum > 0)
    {
        if (curpagenum <= page_sel)
        {
            curpagenum = 0;
        }
        else
        {
            curpagenum -= page_sel;
        }
        _get_page_offset(curpagenum);
        if (decode_sector != (file_offset / 512))
        {
            view_decode.valid = FALSE;
            decode_sector = 0;
        }
        else
        {
            view_decode.text_buf_length = (uint16) (512 - (file_offset % 512));
        }
        need_draw = TRUE;
    }
    else if (page_count_flag == FALSE)
    {
        curpagenum = page_totalnum - page_sel;
        cur_BufSector = -1;
        _get_page_offset(curpagenum);
        view_decode.valid = FALSE;
        need_draw = TRUE;
    }
    else
    {
    }
}

