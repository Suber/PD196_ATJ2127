/*******************************************************************************
 *                              US212A
 *                            Module: music ui
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *     fiona.yang   2011-09-23          1.0              create this file
 *******************************************************************************/

#include "app_music.h"

#include "app_music_bookmark.h"

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e set_scene_bookmark_add_sure(void)
 * \添加书签
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \ingroup      music_setmenu_bookmark.c
 * \note
 */
/*******************************************************************************/
app_result_e set_scene_bookmark_add_sure(void *param)
{
    return ui_show_bookmark(MODE_ADD_BOOKMARK, music_message_deal);
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e set_scene_bookmark_sel_sure(void)
 * \选择书签
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \ingroup      music_setmenu_bookmark.c
 * \note
 */
/*******************************************************************************/
app_result_e set_scene_bookmark_sel_sure(void *param)
{
    return ui_show_bookmark(MODE_SELECT_BOOKMARK, music_message_deal);
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e set_scene_bookmark_del_sure(void)
 * \删除书签
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \ingroup      music_setmenu_bookmark.c
 * \note
 */
/*******************************************************************************/
app_result_e set_scene_bookmark_del_sure(void *param)
{
    return ui_show_bookmark(MODE_DEL_BOOKMARK, music_message_deal);
}
