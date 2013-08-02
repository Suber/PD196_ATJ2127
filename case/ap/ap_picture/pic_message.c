/*******************************************************************************
 *                              US212A
 *                            Module: Picture
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       zhangxs     2011-09-05 10:00     1.0             build this file
 *******************************************************************************/
/*!
 * \file     pic_message.c
 * \brief    Picture 的消息处理函数模块
 * \author   zhangxs
 * \version  1.0
 * \date  2011/9/15
 *******************************************************************************/
#include  "picture.h"


/******************************************************************************/
/*!
 * \par  Description:
 *	  Picture 检测解码是否结束
 * \param[in]    none
 * \param[out]   none
 * \return       bool 解码完成和解码错误都会返回TRUE表明线程解码结束，否则返回FALSE
 * \ingroup      pic_comfunc
 * \note
 *******************************************************************************/
bool pic_check_decode(void)
{
    bool ret_val;
    mmm_id_status_flag_t status;

    status = g_picture_userinfor.dec_status;

    if (status == MMM_ID_FREE)
    {
        ret_val = TRUE;
    }
    else if (status == MMM_ID_ERROR)
    {
        g_picture_change_file |= up_picture_changefile;
        ret_val = TRUE;
    }
    else
    {
        ret_val = FALSE;
    }
    return ret_val;
}
/******************************************************************************/
/*!
 * \par  Description:
 *	  Picture 释放解码线程
 * \param[in]    none
 * \param[out]   none
 * \return       app_result_e
 * \ingroup      pic_comfunc
 * \note
 * \li   关闭解码线程并卸载解码中间件 
 *******************************************************************************/
void pic_decoder_free(void)
{
    //如果当前线程未释放
    if (g_mmm_id_free == FALSE)
    {
        mmm_id_cmd(picture_handle, MMM_ID_CLOSE, 0);
        sys_free_mmm(FALSE);
        g_mmm_id_free = TRUE;
    }
}

