/*******************************************************************************
 *                              US212A
 *                            Module: ui driver
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>           <time>          <version >             <desc>
 *       liutingting     2011-09-05 10:00     1.0             build this file
 *******************************************************************************/
/*!
 * \file     ui_res_open.c
 * \brief    资源文件的操作模块
 * \author   liutingting
 * \par      GENERAL DESCRIPTION:
 *               实现资源文件的打开、关闭及数据查找等功能
 * \par      EXTERNALIZED FUNCTIONS:
 *              直接调用系统SD文件系统的接口函数操作资源文件。
 * \version 1.0
 * \date  2011-9-05
 *******************************************************************************/
#include "display.h"
#include "ui_driver.h"

void get_styctrl_offset(uint16 style_num, uint32 style_addr, res_open_infor_t *style_ptr);
/*! \cond UI_DRIVER_API */
/******************************************************************************/
/*!
 * \par  Description:
 *    打开资源文件
 * \param[in]    filename：资源文件的名字
 * \param[in]    type：资源类型，AP或COMMON资源
 * \param[out]   none
 * \return       bool类型的参数，打开资源文件是否成功
 * \retval       TRUE 打开资源文件成功。
 * \retval       FALSE 打开资源文件失败。
 * \ingroup      res_manager
 * \par          exmaple code
 * \code   
    例子1：打开 common.sty，然后就可以使用 common 工程控件及图片或字符串资源
    ui_res_open("common.sty", UI_COM);
    
    例子2：打开 music.sty，然后就可以使用 music 工程控件及图片或字符串资源
    ui_res_open("music.sty", UI_AP);
 * \endcode
 * \note
 *******************************************************************************/
bool res_open(char *filename, res_type_e type, void *null3)
{
    res_head_t res_head;
    res_open_infor_t *res_ptr;

    /*根据打开资源文件的类型，将指针指向不同的全局变量*/
    if (type == UI_AP)
    {
        res_ptr = &g_ap_res;
    }
    else
    {
        res_ptr = &g_com_res;
    }

    res_ptr->res_fp = sys_sd_fopen(filename);
    if (res_ptr->res_fp == NULL)
    {
        return FALSE;
    }
    sys_sd_fread(res_ptr->res_fp, &res_head, sizeof(res_head_t));
    /*判断文件格式是否合法*/
    if ((res_head.magic[0] != 'R') || (res_head.magic[1] != 'U') 
     || (res_head.magic[2] != '3') || (res_head.magic[3] != 0x19))
    {
        sys_sd_fclose(res_ptr->res_fp);
        return FALSE;
    }
    /*资源文件为ap的类型*/
    sys_sd_fseek(res_ptr->res_fp, SEEK_SET, RESHEADITEM);
    sys_sd_fread(res_ptr->res_fp, &res_ptr->pic_entry_offset, sizeof(res_ptr->pic_entry_offset));

    /*读取多国语言索引的entry地址*/
    sys_sd_fseek(res_ptr->res_fp, SEEK_SET, RESHEADITEM * 2);
    sys_sd_fread(res_ptr->res_fp, &res_ptr->string_entry_offset, sizeof(res_ptr->string_entry_offset));

    //读取当前语言类型的索引entry地址
    sys_sd_fseek(res_ptr->res_fp, SEEK_SET, res_ptr->string_entry_offset + (uint32) language_id * RESHEADITEM);
    sys_sd_fread(res_ptr->res_fp, &res_ptr->string_entry_offset, sizeof(res_ptr->string_entry_offset));
    //读取style文件的索引偏移地址
    get_styctrl_offset(0, res_head.style_addr, res_ptr);
    return TRUE;
}

/******************************************************************************/
/*!
 * \par  Description:
 *    关闭资源文件
 * \param[in]   type：资源类型，AP或COMMON资源
 * \param[out]  none
 * \return      none
 * \ingroup     res_manager
 * \par         exmaple code
 * \code   
    例子1：关闭打开了的 common.sty 
    ui_res_close(UI_COM);
    
    例子2：关闭打开了的 ap 工程，比如 music.sty 
    ui_res_close(UI_AP);
 * \endcode
 * \note
 *******************************************************************************/
void res_close(res_type_e type, void *null2, void *null3)
{
    if (type == UI_AP)
    {
        sys_sd_fclose(g_ap_res.res_fp);
    }
    else
    {
        sys_sd_fclose(g_com_res.res_fp);
    }
}
/*! \endcond */

/*! \cond */
/******************************************************************************/
/*!
 * \par  Description:
 *    获取picbox和textbox两个控件在文件中的起始地址
 * \param[in]    style_num：当前style号
 * \param[in]    style_ptr：当前资源信息结构体指针
 * \param[out]  none
 * \return      none
 * \note
 *******************************************************************************/
void get_styctrl_offset(uint16 style_num, uint32 style_addr, res_open_infor_t *style_ptr)
{
    uint32 style_count;
    //获取style的总数
    sys_sd_fseek(style_ptr->res_fp, SEEK_SET, style_addr + 4);
    sys_sd_fread(style_ptr->res_fp, &style_count, sizeof(uint32));
    if (style_num < style_count)
    {
        //获取当前风格的起始地址
        sys_sd_fseek(style_ptr->res_fp, SEEK_SET, (uint32) (style_addr + 8 + style_num * sizeof(style_t)));
        sys_sd_fread(style_ptr->res_fp, &style_ptr->cur_style_begin_offset, sizeof(uint32));
    }
    //获取当前风格picbox的起始地址
    sys_sd_fseek(style_ptr->res_fp, SEEK_SET, (uint32) (style_ptr->cur_style_begin_offset + PICBOX * 8));
    sys_sd_fread(style_ptr->res_fp, &style_ptr->style_picbox_offset, sizeof(uint32));
}
/*! \endcond */
