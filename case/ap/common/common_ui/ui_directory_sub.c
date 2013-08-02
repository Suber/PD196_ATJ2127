/*******************************************************************************
 *                              us212A
 *                            Module: commonUI-directory
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 *
 * History:         
 *      <author>    <time>           <version >             <desc>
 *       lzcai     2011-9-18 21:56     1.0             build this file 
*******************************************************************************/
/*!
 * \file     _ui_directory_sub.c
 * \brief    commonUI 文件浏览器控件控制流实现
 * \author   lzcai
 * \par      GENERAL DESCRIPTION:
 *               实现了一种文件浏览器GUI，包括文件系统，播放列表，收藏夹等文件浏览。
 * \par      EXTERNALIZED FUNCTIONS:
 *               直接调用ui 驱动中的listbox完成控件显示流任务。
 * \version 1.0
 * \date  2011-9-18
*******************************************************************************/

#include "common_ui.h"

///文件浏览速度控制数组
const uint16 line_cnt_vec[] = {8, 16};
void move_file_records(file_brow_t *file_list, dir_control_t *p_dir_control, uint16 lines);
bool init_dir_control(dir_control_t *p_dir_control);

//计算下翻行数
void get_line_cnt(void)
{
    uint8 list_no_max_index = sizeof(line_cnt_vec) / sizeof(uint16);
    uint8 i;

    if(speed_control <= line_cnt_vec[list_no_max_index - 1])
    {
        speed_control++;
    }

    for(i = 0; i < list_no_max_index; i++)
    {
        if(speed_control <= line_cnt_vec[i])
        {
            line_cnt = i + 1;
            return;
        }
    }

    line_cnt = list_no_max_index;
    return;
}

/******************************************************************************/
/*!                    
 * \par  Description:
 *    选择文件（目录）列表中的下几项
 * \param[in]    p_dir_control：指向文件（目录）列表控制结构体
 * \param[in]    file_list：文件浏览列表，在向上切和向下切时进行有效数据搬移
 * \param[out]   p_dir_control：返回更新了的文件（目录）列表参数
 * \return       none
 * \note 
*******************************************************************************/
void dir_select_next(file_brow_t *file_list, dir_control_t *p_dir_control)
{
    get_line_cnt();
                    
    //在页内切换，切后尚未到页底最后一项（最多到倒数第二项），更新激活项
    if((p_dir_control->list_no + line_cnt) < p_dir_control->bottom)
    {
        p_dir_control->list_no += line_cnt;
        p_dir_control->old = p_dir_control->active;
        p_dir_control->active += line_cnt;
        p_dir_control->draw_mode = LIST_DRAW_ACTIVE;
    }
    else
    {
        //在列表最后一项，跳到列表头显示
        if(p_dir_control->list_no == p_dir_control->list_total)
        {
            p_dir_control->list_no = 1;
            //仅有一页的情况下，只需更新激活项
            if(p_dir_control->list_total <= p_dir_control->one_page_count)
            {
                //总项数多余1项才需要切换
                if(p_dir_control->list_total > 1)
                {
                    p_dir_control->old = p_dir_control->active;
                    p_dir_control->active = 1;
                    p_dir_control->draw_mode = LIST_DRAW_ACTIVE;
                }
            }
            //从尾切换到头，更新整个列表
            else
            {
                p_dir_control->top = 1;
                p_dir_control->bottom = p_dir_control->one_page_count;
                p_dir_control->old = p_dir_control->active = 1;
                p_dir_control->update_mode = LIST_UPDATE_HEAD;
                p_dir_control->draw_mode = LIST_DRAW_LIST;
            }
        }
        else 
        {
            //在列表尾部，切到最后一项
            if(p_dir_control->list_no >= (p_dir_control->list_total - line_cnt))
            {
                //刚好在当前页切到最后一项，只需更新激活项
                if((p_dir_control->list_no == (p_dir_control->list_total - line_cnt))
                 &&(p_dir_control->bottom == p_dir_control->list_total))
                {
                    p_dir_control->list_no += line_cnt;
                    p_dir_control->old = p_dir_control->active;
                    p_dir_control->active += line_cnt;
                    p_dir_control->draw_mode = LIST_DRAW_ACTIVE;
                }
                //否则，更新整个列表
                else
                {
                    //计算实际切换行数
                    line_cnt = p_dir_control->list_total - p_dir_control->bottom;
                    //更新p_dir_control结构
                    p_dir_control->bottom = p_dir_control->list_no = p_dir_control->list_total;
                    if(p_dir_control->bottom >= p_dir_control->one_page_count)
                    {
                        p_dir_control->top = p_dir_control->bottom - p_dir_control->one_page_count + 1;
                    }
                    else
                    {
                        p_dir_control->top = 1;
                    }
                    p_dir_control->old = p_dir_control->active = p_dir_control->bottom - p_dir_control->top + 1;
                    p_dir_control->update_mode = LIST_UPDATE_NEXT;
                    p_dir_control->draw_mode = LIST_DRAW_LIST;
                }
            }
            //在当前页尾部，并且后面有足够列表项，向下切页，更新整个列表
            else
            {
                //当前页倒数第二项之前
                if(p_dir_control->active < (p_dir_control->one_page_count - 1))
                {
                    p_dir_control->list_no += line_cnt;
                    p_dir_control->bottom = p_dir_control->list_no + 1;
                    p_dir_control->top = p_dir_control->bottom - p_dir_control->one_page_count + 1;
                    p_dir_control->old = p_dir_control->active = p_dir_control->one_page_count - 1;
                }
                //当前页倒数第二项
                else
                {
                    p_dir_control->list_no += line_cnt;
                    p_dir_control->top += line_cnt;
                    p_dir_control->bottom += line_cnt;
                    p_dir_control->old = p_dir_control->active;
                }
                
                p_dir_control->update_mode = LIST_UPDATE_NEXT;
                p_dir_control->draw_mode = LIST_DRAW_LIST;
            }
        }
    }
    
    //移动有效列表项
    if(p_dir_control->update_mode == LIST_UPDATE_NEXT)
    {
        move_file_records(file_list, p_dir_control, line_cnt);
    }
}

/******************************************************************************/
/*!                    
 * \par  Description:
 *    选择文件（目录）列表中的上几项
 * \param[in]    p_dir_control：指向文件（目录）列表控制结构体
 * \param[in]    file_list：文件浏览列表，在向上切和向下切时进行有效数据搬移
 * \param[out]   p_dir_control：返回更新了的文件（目录）列表参数
 * \return       none
 * \note 
*******************************************************************************/
void dir_select_prev(file_brow_t *file_list, dir_control_t *p_dir_control)
{
    get_line_cnt();
    
    //在页内切换，切后尚未到页顶首项（最多到第二项），更新激活项
    if(p_dir_control->active > (line_cnt + 1))
    {
        p_dir_control->list_no -= line_cnt;
        p_dir_control->old = p_dir_control->active;
        p_dir_control->active -= line_cnt;
        p_dir_control->draw_mode = LIST_DRAW_ACTIVE;
    }
    else
    {
        //在列表第一项，跳到列表尾显示
        if(p_dir_control->list_no == 1)
        {
            p_dir_control->list_no = p_dir_control->list_total;
            //仅有一页的情况下，只需更新激活项
            if(p_dir_control->list_total <= p_dir_control->one_page_count)
            {
                //总项数多余1项才需要切换
                if(p_dir_control->list_total > 1)
                {
                    p_dir_control->old = p_dir_control->active;
                    p_dir_control->active = p_dir_control->list_total;
                    p_dir_control->draw_mode = LIST_DRAW_ACTIVE;
                }
            }
            //从头切到尾，更新整个列表
            else
            {
                p_dir_control->top = p_dir_control->list_total - p_dir_control->one_page_count + 1;
                p_dir_control->bottom = p_dir_control->list_total;
                p_dir_control->old = p_dir_control->active = p_dir_control->one_page_count;
                p_dir_control->update_mode = LIST_UPDATE_TAIL;
                p_dir_control->draw_mode = LIST_DRAW_LIST;
            }
        }
        else 
        {
            //在列表头部，切到第一项
            if(p_dir_control->list_no <= (1 + line_cnt))
            {
                //刚好在当前页切到第一项，只需更新激活项
                if((p_dir_control->list_no == (1 + line_cnt))
                 &&(p_dir_control->top == 1))
                {
                    p_dir_control->list_no -= line_cnt;
                    p_dir_control->old = p_dir_control->active;
                    p_dir_control->active -= line_cnt;
                    p_dir_control->draw_mode = LIST_DRAW_ACTIVE;
                }
                //否则，更新整个列表
                else
                {
                    //计算实际切换行数
                    line_cnt = p_dir_control->top - 1;
                    //更新p_dir_control结构
                    p_dir_control->top = p_dir_control->list_no = 1;
                    init_dir_control(p_dir_control);
                    p_dir_control->update_mode = LIST_UPDATE_PREV;
                    p_dir_control->draw_mode = LIST_DRAW_LIST;
                }
            }
            //在第二项，并且前面有足够列表项，向上切页，更新整个列表
            else
            {
                //当前页第二项之后
                if(p_dir_control->active > 2)
                {
                    p_dir_control->list_no -= line_cnt;
                    p_dir_control->top = p_dir_control->list_no - 1;
                    p_dir_control->bottom = p_dir_control->top + p_dir_control->one_page_count - 1;
                    p_dir_control->old = p_dir_control->active = 2;
                }
                //当前页第二项
                else
                {
                    p_dir_control->list_no -= line_cnt;
                    p_dir_control->top -= line_cnt;
                    p_dir_control->bottom -= line_cnt;
                    p_dir_control->old = p_dir_control->active;
                }
                
                p_dir_control->update_mode = LIST_UPDATE_PREV;
                p_dir_control->draw_mode = LIST_DRAW_LIST;
            }
        }
    }

    //移动有效列表项
    if(p_dir_control->update_mode == LIST_UPDATE_PREV)
    {
        move_file_records(file_list, p_dir_control, line_cnt);
    }
}


//初始化dir_control_t数据结构
bool init_dir_control(dir_control_t *p_dir_control)
{
    p_dir_control->bottom = p_dir_control->top + p_dir_control->one_page_count - 1;
    if(p_dir_control->bottom > p_dir_control->list_total)
    {
        p_dir_control->bottom = p_dir_control->list_total;
    }
    p_dir_control->old = p_dir_control->active = p_dir_control->list_no - p_dir_control->top + 1;
    
    return TRUE;
}

//移动有效文件记录项，仅对 LIST_UPDATE_NEXT 和 LIST_UPDATE_PREV 有效
void move_file_records(file_brow_t *file_list, dir_control_t *p_dir_control, uint16 lines)
{
    file_brow_t *from, *to;
    uint16 begin, end, i;

    switch(p_dir_control->update_mode)
    {
    case LIST_UPDATE_NEXT:
        begin = lines;
        end = p_dir_control->one_page_count;
        for(i= begin; i < end; i++)
        {
            from = file_list + i;
            to = file_list + i - lines;
            libc_memcpy(to->ext, from->ext, sizeof(to->ext));
            libc_memcpy(to->name_buf, from->name_buf, (uint32)(to->name_len));
        }
        file_list += p_dir_control->one_page_count - 1;
        break;
    case LIST_UPDATE_PREV:
        begin = 0;
        end = p_dir_control->one_page_count - lines;
        for(i= end; i > begin; i--)
        {
            from = file_list + i - 1;
            to = file_list + i + lines - 1;
            libc_memcpy(to->ext, from->ext, sizeof(to->ext));
            libc_memcpy(to->name_buf, from->name_buf, (uint32)(to->name_len));
        }
        break;
    default:
        break;
    }
}

