/********************************************************************************
 *                              USDK
 *                             Module: ebook
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : ebook_bookmark_sub.c
 * History:
 *      <author>    <time>           <version >             <desc>
 *      stevenluo		2011-11-11         v1.0              create this file
 ********************************************************************************/

#include "ebook.h"

//字符"P_"
const uint8 page_str[2] =
{ "p_" };
//字符"*"
const uint8 page_multiply[1] =
{ "*" };
/****************************************************************************/
/*									代码段												       */
/****************************************************************************/

/********************************************************************************
 * Description :切换到下一个书签文件
 *
 * Arguments  :
 *			  	mlst_ctl:菜单列表控制结构体((参照common_ui.h中的dir_control_t))
 *			 	 line_num:切换的行数;0:切换1行;1:切换2行，其他以此类推
 * Returns     :
 *           	    无
 * Notes       :
 *
 ********************************************************************************/
void _select_next_item(dir_control_t *mlst_ctl, uint8 line_num)
{
    //如果列表总数不大于切换的行数，则不进行更新
    if (mlst_ctl->list_total <= line_num)
    {
        return;
    }
    if (line_num < 1)
    {
        line_num = 1;
    }
    //在页内切换，尚未到页底最后一项，更新激活项
    if ((mlst_ctl->list_no + line_num) < mlst_ctl->bottom)
    {
        just_change_active_next: mlst_ctl->list_no += line_num;
        mlst_ctl->old = mlst_ctl->active;
        mlst_ctl->active += line_num;
        mlst_ctl->draw_mode = LIST_DRAW_ACTIVE;
    }
    else
    {
        //在列表最后一项，跳到列表头显示
        if (mlst_ctl->list_no == (mlst_ctl->list_total - 1))
        {
            mlst_ctl->list_no = 0;
            //仅有一页的情况下，只需更新激活项
            if (mlst_ctl->list_total <= LIST_NUM_ONE_PAGE_MAX)
            {
                mlst_ctl->old = mlst_ctl->active;
                mlst_ctl->active = 0;
                mlst_ctl->draw_mode = LIST_DRAW_ACTIVE;
            }
            //从尾切换到头，更新整个列表
            else
            {
                mlst_ctl->top = 0;
                mlst_ctl->bottom = LIST_NUM_ONE_PAGE_MAX - 1;
                mlst_ctl->old = mlst_ctl->active = 0;
                mlst_ctl->update_mode = LIST_UPDATE_HEAD;
                mlst_ctl->draw_mode = LIST_DRAW_LIST;
            }
        }
        //在列表倒数第line_num项或更靠后，切到最后一项
        else if (mlst_ctl->list_no >= (mlst_ctl->list_total - 1 - line_num))
        {
            //在所有项中的倒数第二项，切1项到最后一项，只需更新激活项
            if (line_num == 1)
            {
                goto just_change_active_next;
            }
            //切多项（2项或3项）到最后一项，更新整个列表
            else
            {
                mlst_ctl->list_no = mlst_ctl->list_total - 1;
                mlst_ctl->top = mlst_ctl->list_total - LIST_NUM_ONE_PAGE_MAX;
                mlst_ctl->bottom = mlst_ctl->list_no;
                mlst_ctl->old = mlst_ctl->active = LIST_NUM_ONE_PAGE_MAX - 1;
                mlst_ctl->draw_mode = LIST_DRAW_LIST;
            }
        }
        //在倒数第二项，并且后面有足够列表项，向下切页，更新整个列表
        else
        {
            mlst_ctl->list_no += line_num;
            mlst_ctl->top += line_num;
            mlst_ctl->bottom += line_num;
            mlst_ctl->old = mlst_ctl->active;
            mlst_ctl->update_mode = LIST_UPDATE_NEXT;
            mlst_ctl->draw_mode = LIST_DRAW_LIST;
        }
    }
    need_draw = TRUE;
}
/********************************************************************************
 * Description :切换到上一个书签文件
 *
 * Arguments  :
 *			  	mlst_ctl:菜单列表控制结构体((参照common_ui.h中的dir_control_t))
 *			 	 line_num:切换的行数;0:切换1行;1:切换2行，其他以此类推
 * Returns     :
 *               	无
 * Notes       :
 *
 ********************************************************************************/
void _select_prev_item(dir_control_t *mlst_ctl, uint8 line_num)
{
    //如果列表总数不大于切换的行数，则不进行更新
    if (mlst_ctl->list_total <= line_num)
    {
        return;
    }
    if (line_num < 1)
    {
        line_num = 1;
    }
    if (mlst_ctl->active >= (1 + line_num))
    {
        //在当前页内切换到上一个文件
        just_change_active_prev: mlst_ctl->list_no -= line_num;
        mlst_ctl->old = mlst_ctl->active;
        mlst_ctl->active -= line_num;
        mlst_ctl->draw_mode = LIST_DRAW_ACTIVE; //更新激活项
    }
    else
    {
        if (mlst_ctl->list_no == 0)
        {
            mlst_ctl->list_no = (mlst_ctl->list_total - 1);
            if (mlst_ctl->list_total <= LIST_NUM_ONE_PAGE_MAX)
            {
                mlst_ctl->old = mlst_ctl->active;
                mlst_ctl->active = (mlst_ctl->list_total - 1);
                mlst_ctl->draw_mode = LIST_DRAW_ACTIVE; //更新激活项
            }
            else
            {
                //从头切到尾
                mlst_ctl->top = mlst_ctl->list_total - LIST_NUM_ONE_PAGE_MAX;
                mlst_ctl->bottom = mlst_ctl->list_no;
                mlst_ctl->old = mlst_ctl->active = LIST_NUM_ONE_PAGE_MAX - 1;
                mlst_ctl->update_mode = LIST_UPDATE_TAIL; //从头切到尾
                mlst_ctl->draw_mode = LIST_DRAW_LIST; //更新整个文件列表
            }
        }
        //在列表倒数第line_num项或更靠后，切到最后一项
        else if (mlst_ctl->list_no <= line_num)
        {
            //在所有项中的第二项，切1项到第一项，只需更新激活项
            if (line_num == 1)
            {
                goto just_change_active_prev;
            }
            //切多项（2项或3项）到第一项，更新整个列表
            else
            {
                mlst_ctl->list_no = 0;
                mlst_ctl->top = 0;
                mlst_ctl->old = mlst_ctl->active;
                mlst_ctl->active = 0;
                if (mlst_ctl->list_total <= LIST_NUM_ONE_PAGE_MAX)
                {
                    mlst_ctl->bottom = mlst_ctl->list_total - 1;
                }
                else
                {
                    mlst_ctl->bottom = LIST_NUM_ONE_PAGE_MAX - 1;
                }
                mlst_ctl->draw_mode = LIST_DRAW_LIST;
            }
        }
        else
        {
            //向上切页
            mlst_ctl->list_no -= line_num;
            mlst_ctl->top -= line_num;
            mlst_ctl->bottom -= line_num;
            mlst_ctl->old = mlst_ctl->active;
            mlst_ctl->update_mode = LIST_UPDATE_PREV; //向上切页
            mlst_ctl->draw_mode = LIST_DRAW_LIST; //更新整个文件列表
        }
    }
    need_draw = TRUE;
}
/********************************************************************************
 * Description :菜单列表初始化
 *
 * Arguments  :
 *				mlst_ctl:菜单列表控制结构体(参照common_ui.h中的dir_control_t)
 *				total:显示的文件总数
 * Returns     :
 *                无
 * Notes       :
 *
 ********************************************************************************/
void _bmklist_init(dir_control_t *mlst_ctl, uint16 total)
{
    mlst_ctl->list_no = 0;
    mlst_ctl->top = 0;
    mlst_ctl->list_total = total;
    mlst_ctl->bottom = mlst_ctl->top + LIST_NUM_ONE_PAGE_MAX - 1;
    mlst_ctl->draw_mode = LIST_DRAW_ALL;
    if (mlst_ctl->bottom > (mlst_ctl->list_total - 1))
    {
        mlst_ctl->bottom = mlst_ctl->list_total - 1;
    }
    mlst_ctl->old = mlst_ctl->active = mlst_ctl->list_no - mlst_ctl->top;
}
/********************************************************************************
 * Description :切换到下一个书签文件
 *
 * Arguments  :
 *			  	mlst_ctl:菜单列表控制结构体((参照common_ui.h中的dir_control_t))
 *			 	 line_num:切换的行数;0:切换1行;1:切换2行，其他以此类推
 * Returns     :
 *           	    无
 * Notes       :
 *
 ********************************************************************************/
void _update_delete_item(dir_control_t *mlst_ctl)
{
    mlst_ctl->list_total--;
    if (mlst_ctl->list_total == 0)
    {
        return;
    }
    if (mlst_ctl->bottom > (mlst_ctl->list_total - 1))
    {
        mlst_ctl->bottom = mlst_ctl->list_total - 1;
        if (mlst_ctl->top > 0)
        {
            mlst_ctl->top--;
            mlst_ctl->list_no--;
        }
    }
    if (mlst_ctl->list_no > (mlst_ctl->list_total - 1))
    {
        mlst_ctl->list_no = mlst_ctl->list_total - 1;
        if (mlst_ctl->active > mlst_ctl->list_no)
        {
            mlst_ctl->active = mlst_ctl->list_no;
        }
    }
    else if ((mlst_ctl->list_no == mlst_ctl->list_total) && (mlst_ctl->list_no > 1))
    {
        mlst_ctl->list_no--;
    }
    else
    {
    }
    if (mlst_ctl->active > (mlst_ctl->list_total - 1))
    {
        mlst_ctl->active = mlst_ctl->list_total - 1;
    }
    mlst_ctl->draw_mode = LIST_DRAW_ALL;
}

/********************************************************************************
 * Description :更新列表显示信息的内容
 *
 * Arguments  :
 *			 	 dir_private:listbox 私有结构体(参照display.h中的listbox_private_t)
 *			 	 dir_control:菜单列表控制结构体(参照common_ui.h中的dir_control_t)
 *			 	 iinlet_type:功能函数的入口类型(参照ebook.h中的inlet_type_e)
 *                    (inlet_type)0-阅读界面;1-书签选择菜单，1-书签删除菜单,3-页数选择菜单
 * Returns     :
 *                无
 * Notes       :
 *
 ********************************************************************************/
void _update_list_info(dir_control_t *menlist, listbox_private_t *dir_private, inlet_type_e inlet_type)
{
    uint16 index;
    dir_private->title.data.str = BOOKMARK_filename;
    dir_private->title.length = LIST_ITEM_BUFFER_LEN;
    if (*(uint16*) (dir_private->title.data.str) == 0xfeff)
    {
        dir_private->title.language = UNICODEDATA;
    }
    else
    {
        dir_private->title.language = (int8) g_comval.language_id;
    }
    dir_private->item_valid = (uint8) (menlist->bottom - menlist->top + 1);
    dir_private->active = menlist->active;
    dir_private->old = menlist->old;
    dir_private->list_no = menlist->list_no;
    dir_private->list_total = menlist->list_total;

    for (index = 0; index < dir_private->item_valid; index++)
    {
        if (inlet_type == PAGE_SEL)
        {
            dir_private->items[index].data.str = _get_page_title(menlist->top, (uint8) index);
            dir_private->items[index].language = (int8) g_comval.language_id;
        }
        else
        {
            dir_private->items[index].data.str = _get_bmk_title((uint8) (menlist->top + index));
            dir_private->items[index].language = (int8) view_file.language;
        }
        dir_private->items[index].length = LIST_ITEM_BUFFER_LEN;
        dir_private->items[index].argv = 0;
    }
}
/********************************************************************************
 * Description :处理书签列表下的确定键功能
 *
 * Arguments  :
 *			 	 mlst_ctl:菜单列表控制结构体(参照common_ui.h中的dir_control_t)
 *			 	inlet_type:功能函数的入口类型(参照ebook.h中的inlet_type_e)
 *                    (inlet_type)0-阅读界面;1-书签选择菜单，1-书签删除菜单,3-页数选择菜单
 * Returns     :
 *               	 返回相应的处理结果
 * Notes       :
 *
 ********************************************************************************/
app_result_e _deal_confirm_item(dir_control_t *mlst_ctl, inlet_type_e inlet_type)
{
    app_result_e retval = RESULT_NULL;
    switch (inlet_type)
    {
        case BMK_SEL:
        curpagenum = _get_bmk_page(mlst_ctl->list_no);
        cur_BufSector = -1;
        retval = RESULT_EBK_PLAY;
        break;
        case BMK_DET:
        retval = _show_double_dialog(S_DELETEOPTION, _get_bmk_title((uint8) mlst_ctl->list_no));
        if (retval == RESULT_DIALOG_YES)                //如果按下是，就删掉电子书
        {
            _del_bookmark(mlst_ctl->list_no);
            _update_delete_item(mlst_ctl);
            if (mlst_ctl->list_total == 0)
            {
                _show_single_dialog(S_NOBMK, DIALOG_INFOR_WAIT);
                return RESULT_REDRAW;
            }
            retval = RESULT_NULL;
            need_draw = TRUE;
        }
        else
        {
            need_draw = TRUE;
            mlst_ctl->draw_mode = LIST_DRAW_ALL;
            retval = RESULT_NULL;
        }

        break;
        case PAGE_SEL:
        curpagenum = mlst_ctl->list_no * page_multiple_sel;
        if ((page_multiple_sel > 1) && (mlst_ctl->list_no > 0))
        {
            curpagenum--;
        }
        retval = RESULT_EBK_PLAY;
        break;
        default:
        break;
    }
    return retval;
}
/********************************************************************************
 * Description :获取页数标题信息
 *
 * Arguments  :
 *			 	 top:菜单列表项中的顶端项
 *			 	 index:菜单列表项中的索引号
 *
 *               	 返回列表项指针，里面存储页数字符串信息
 * Notes       :
 *
 ********************************************************************************/
uint8 *_get_page_title(uint16 top, uint8 index)
{
    uint8 num_count;
    uint8 file_str[12];
    libc_memset(file_str, 0x0, 12);
    if ((page_multiple_sel > 1) && ((top + index) != 0))
    {
        num_count = libc_itoa(top + index, &file_str[2], 1);
        if ((2 + num_count) <= 11)
        {
            libc_strncpy(&file_str[2 + num_count], page_multiply, 1);
            libc_itoa(page_multiple_sel, &file_str[2 + num_count + 1], 0);
        }
    }
    else
    {
        libc_itoa(top + index + 1, &file_str[2], 1);
    }
    libc_strncpy(&file_str[0], page_str, 2);
    libc_memcpy(&file_list[index], file_str, 12);
    return (uint8*) &file_list[index];
}

