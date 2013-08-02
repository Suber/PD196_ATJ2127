/*******************************************************************************
 *                              US212A
 *                            Module: music ui
 *                 Copyright(c) 2003-2012 Actions Semiconductor
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *     fiona.yang   2011-09-27          1.0              create this file
 *******************************************************************************/

#include "app_music.h"

#include "app_music_favorite.h"


/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e set_scene_addfavor_sure(void *param)
 * \添加到收藏夹的确认函数
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \ingroup      music_setmenu_favordeal.c
 * \note
 */
/*******************************************************************************/
app_result_e set_scene_addfavor_sure(void *param)
{
    //返回参数
    menu_title_action_t* paramptr = (menu_title_action_t*) param;
    
    paramptr->style_id = MENULIST_TITLE;

    paramptr->title_id = S_ADD_TO_WHICH;
    //进入下一级菜单
    return RESULT_MENU_SON;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e set_scene_addfavor1_sure(void *param)
 * \添加到收藏夹1的确认函数
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \ingroup      music_setmenu_favordeal.c
 * \note
 */
/*******************************************************************************/
app_result_e set_scene_addfavor1_sure(void *param)
{

    return add_favorite_setmenu(FSEL_TYPE_LISTFAVOR1);
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e set_scene_addfavor2_sure(void *param)
 * \添加到收藏夹2的确认函数
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \ingroup      music_setmenu_favordeal.c
 * \note
 */
/*******************************************************************************/
app_result_e set_scene_addfavor2_sure(void *param)
{

    return add_favorite_setmenu(FSEL_TYPE_LISTFAVOR2);
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e set_scene_addfavor3_sure(void *param)
 * \添加到收藏夹3的确认函数
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \ingroup      music_setmenu_favordeal.c
 * \note
 */
/*******************************************************************************/
app_result_e set_scene_addfavor3_sure(void *param)
{

    return add_favorite_setmenu(FSEL_TYPE_LISTFAVOR3);
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e set_scene_delfavor1_sure(void *param)
 * \从收藏夹1删除的确认函数
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \ingroup      music_setmenu_favordeal.c
 * \note
 */
/*******************************************************************************/
app_result_e set_scene_delfavor1_sure(void *param)
{

    return del_favorite_setmenu(FSEL_TYPE_LISTFAVOR1);
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e set_scene_delfavor2_sure(void *param)
 * \从收藏夹2删除的确认函数
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \ingroup      music_setmenu_favordeal.c
 * \note
 */
/*******************************************************************************/
app_result_e set_scene_delfavor2_sure(void *param)
{

    return del_favorite_setmenu(FSEL_TYPE_LISTFAVOR2);
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e set_scene_delfavor3_sure(void *param)
 * \从收藏夹3删除的确认函数
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \ingroup      music_setmenu_favordeal.c
 * \note
 */
/*******************************************************************************/
app_result_e set_scene_delfavor3_sure(void *param)
{
    return del_favorite_setmenu(FSEL_TYPE_LISTFAVOR3);
}
