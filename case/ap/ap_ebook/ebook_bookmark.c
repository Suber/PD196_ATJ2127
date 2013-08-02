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
/* include file of this application, 本应用的头文件 */
#include "ebook.h"

//倍数据转换表
const uint16 multiple_num[8] =
{ 1, 1, 5, 10, 50, 100, 500, 1000 };

/****************************************************************************/
/*                                  代码段                                                     */
/****************************************************************************/

/********************************************************************************
 * Description :获取书签总数
 *
 * Arguments  :
 *                  inlet_type:菜单列表的入口类型(参照ebook.h中的ebookmark_mode_e)
 *                  (inlet_type)0-书签选择菜单，1-书签删除菜单,2-页数选择菜单，
 * Returns     :
 *                  如果inlet_type=PAGE_SEL,返回总页数，如果inlet_type=PAGE_SEL,返回书签总数
 * Notes       :
 *
 ********************************************************************************/
uint16 _get_bookmark_nums(inlet_type_e inlet_type)
{
    uint8 count_num = 0;
    if (inlet_type == PAGE_SEL)     //如果是页码选择
    {
        //由于超过10M 的文本文件，总的页数都超过65535页，所以显示
        //时只能以倍数的方式来显示和选择
        count_num = _count_num_bytes(page_totalnum);
        if (count_num < 9)
        {
            page_multiple_sel = multiple_num[count_num - 1];
        }
        if (page_multiple_sel > 1)
        {
            return (uint16) ((page_totalnum / page_multiple_sel) + 1);
        }
        else
        {
            return (uint16) page_totalnum;
        }
    }
    else
    {
        //如果bookmark_total为0xff，说明没有计算过书签总数，则从书签文件中
        //读取书签信息，然后计算书签总数
        //否则的话直接返回书签总数就行了。
        if (bookmark_total == 0xFF)
        {
            _seek_and_read(0, 0x100, (uint8*) bkmarks);
        }
        else
        {
            return bookmark_total;
        }
        bookmark_total = 0;
        //计算书签总数，如果pagenum为0xFFFFFFFF，说明后面的数据都不是书签信息
        //在PC里，如果pagenum为0，说明后面的数据都不是书签信息
        while (1)
        {
            if (    (bookmark_total >= MAX_BKMARK_NUM) 
                ||  (bkmarks[bookmark_total].pagenum == V_U32_INVALID)
                ||  (bkmarks[bookmark_total].title[0] == 0)
                )
            {
                break;
            }
            bookmark_total++;
        }
        return bookmark_total;
    }
}

/********************************************************************************
 * Description :删除书签
 *
 * Arguments  :
 *              id:当前选择的书签索引号
 * Returns     :
 *                  0:书签删除完毕.1:书签总数不为0
 * Notes       :
 *            根据id号从bkmarks书签列表中删除选中的书签内容，然后再把后面书签相应往前移
 ********************************************************************************/
void _del_bookmark(uint16 id)
{
    uint16 i = id;
    //当前id号以后的书签往前移，然后将移位后的最后一个填充为0xFFFFFFFF
    for (; i < bookmark_total; i++)
    {
        if (i < 15)
        {
            bkmarks[i].pagenum = bkmarks[i + 1].pagenum;
            libc_memcpy(bkmarks[i].title, bkmarks[i + 1].title, (MAX_TITLE_LEN));
        }
    }
    bookmark_total--;
    bkmarks[bookmark_total].pagenum = V_U32_INVALID;
    bmk_modify_flag = TRUE;
}

/********************************************************************************
 * Description :添加加书签
 *
 * Arguments  :
 *              无
 * Returns     :
 *                  0:添加失败。1:添加成功
 * Notes       :
 *
 ********************************************************************************/
bool _add_bookmark(void)
{
    int i, j;
    //如果书签总数已经超过最大的限制数，则提示是否删除第一个文件，
    //替换当前的书签，也就是所有书签往前移一位。
    if (bookmark_total >= MAX_BKMARK_NUM)
    {
        return 0;
    }
    //如果书签总数为0的话则直接将书签存放进去。
    if (bookmark_total == 0)
    {
        i = 0;
    }
    else
    {
        for (i = (bookmark_total - 1); i >= 0; i--)
        {
            //如果书签中已经存在该页数，则直接返回
            if (curpagenum == bkmarks[i].pagenum)
            {
                return 1;
            }
            //如果当前页数大于获取到的页数，则说明需要在i到(i+1)这个位置插入页数
            else if (curpagenum > bkmarks[i].pagenum)
            {
                break;
            }
            //如果当前页数小于获取到的页数，则继续往上查找
            else
            {

            }
        }
        i++;
        /* 在链表中间插入 页数*/
        /* [1],[2],...[*],[*+1],[*+2],.... */
        /*             |    |              */
        /*           prev  cur             */
        /*            [*],[×],[*+1]        */
        /*                 |               */
        /*              pre,cur          */
        for (j = bookmark_total; j > i; j--)
        {
            bkmarks[(uint8) j].pagenum = bkmarks[j - 1].pagenum;
            libc_memcpy(bkmarks[j].title, bkmarks[j - 1].title, (MAX_TITLE_LEN));
            libc_memset(&bkmarks[j].title[MAX_TITLE_LEN - 2], 0x0, 2);
        }
    }
    bkmarks[(uint8) i].pagenum = curpagenum;
    libc_memcpy(bkmarks[i].title, BOOKMARK_filename, (MAX_TITLE_LEN));
    libc_memset(&bkmarks[i].title[MAX_TITLE_LEN - 2], 0x0, 2);
    bookmark_total++;
    bmk_modify_flag = TRUE;
    //   _seek_and_write(0, bookmark_total*NODELEN,bkmarks);
    return 1;
}
/********************************************************************************
 * Description :获取当前书签标题
 *
 * Arguments  :
 *              index:书签列表的索引号
 * Returns     :
 *                  标题字符串指针
 * Notes       :
 *
 ********************************************************************************/

uint8 *_get_bmk_title(uint8 index)
{
    return bkmarks[index].title;
}
/********************************************************************************
 * Description :获取当前书签所处的页数
 *
 * Arguments  :
 *              index:书签列表的索引号
 * Returns     :
 *                  页数
 * Notes       :
 *
 ********************************************************************************/
uint32 _get_bmk_page(uint16 index)
{
    return bkmarks[index].pagenum;
}
/********************************************************************************
 * Description :计算当前数字的位数
 *
 * Arguments  :
 *              num:需要计算的数字
 * Returns     :
 *                    计算出来的位数
 * Notes       :
 *
 ********************************************************************************/
uint8 _count_num_bytes(uint32 num)
{
    uint8 count = 0;
    while (1)
    {
        count++;
        num /= 10;
        if (num == 0)
        {
            break;
        }
    }
    return count;
}

