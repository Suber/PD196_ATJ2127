/*******************************************************************************
 *                              us212A
 *                            Module: Common
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 *
 * History:         
 *      <author>    <time>           <version >             <desc>
 *       lzcai     2011-10-14 16:58     1.0             build this file 
*******************************************************************************/
/*!
 * \file     _common_msgmap_data.c
 * \brief    按键消息映射表定义集合。
 * \author   lzcai
 * \par      GENERAL DESCRIPTION:
 *               包含所有 common 消息循环按键映射表定义。
 * \par      EXTERNALIZED FUNCTIONS:
 *               
 * \version  1.0
 * \date     2012-2-16
*******************************************************************************/

#include "common_func.h"

/*! \cond COMMON_API */

/*!
 * \brief
 *  delete_key_map_list：删除文件控件按键映射表
 */
const key_map_t delete_key_map_list[] = 
{
    /*! 短按 VOL 键，终止删除后续文件 */
    {{KEY_VOL, 0, KEY_TYPE_SHORT_UP}, EVENT_DELETE_STOP},
    
    /*! 不响应以下快捷键 */
    {{KEY_LOCKHOLD, 0, KEY_TYPE_ALL}, MSG_NULL},
    {{KEY_VOL, 0, KEY_TYPE_LONG}, MSG_NULL},
    
    /*! 结束标志 */
    {{KEY_NULL, 0, KEY_TYPE_NULL}, MSG_NULL},
};

/*!
 * \brief
 *  dialog_key_map_list：对话框控件按键映射表
 */
const key_map_t dialog_key_map_list[] = 
{
    /*! PREV 按键转换为 PREV_BUTTON 事件 */
    {{KEY_PREV, 0, KEY_TYPE_DOWN | KEY_TYPE_LONG | KEY_TYPE_HOLD}, EVENT_DIALOG_PREV_BUTTON},
    /*! NEXT 按键转换为 NEXT_BUTTON事件 */
    {{KEY_NEXT, 0, KEY_TYPE_DOWN | KEY_TYPE_LONG | KEY_TYPE_HOLD}, EVENT_DIALOG_NEXT_BUTTON},
    /*! 短按KEY_PLAY 按键转换为 CONFIRM事件 */
    {{KEY_PLAY, 0, KEY_TYPE_SHORT_UP}, EVENT_DIALOG_CONFIRM},
    /*! 短按KEY_VOL 按键转换为 CANCEL事件 */
    {{KEY_VOL, 0, KEY_TYPE_SHORT_UP}, EVENT_DIALOG_CANCEL},
    /*! 短按KEY_MODE 按键转换为 CANCEL事件 */
    {{KEY_MODE, 0, KEY_TYPE_SHORT_UP}, EVENT_DIALOG_CANCEL},
    
    /*! 结束标志 */
    {{KEY_NULL, 0, KEY_TYPE_NULL}, MSG_NULL},
};

/*!
 * \brief
 *  directory_key_map_list：文件浏览器按键映射表
 */
const key_map_t directory_key_map_list[] = 
{
    /*! PREV 按键转换为 SELECT_PREV 事件 */
    {{KEY_PREV, 0, KEY_TYPE_DOWN | KEY_TYPE_LONG | KEY_TYPE_HOLD}, EVENT_DIRECTORY_SELECT_PREV},
    /*! PREV 按键 抬起 转换为 SELECT_STOP 事件 */
    {{KEY_PREV, 0, KEY_TYPE_SHORT_UP}, EVENT_DIRECTORY_SELECT_STOP},
    /*! NEXT 按键转换为 SELECT_NEXT 事件 */
    {{KEY_NEXT, 0, KEY_TYPE_DOWN | KEY_TYPE_LONG | KEY_TYPE_HOLD}, EVENT_DIRECTORY_SELECT_NEXT},
    /*! NEXT 按键 抬起 转换为 SELECT_STOP 事件 */
    {{KEY_NEXT, 0, KEY_TYPE_SHORT_UP}, EVENT_DIRECTORY_SELECT_STOP},
    /*! 短按KEY_PLAY 按键转换为 ENTER_SON 事件 */
    {{KEY_PLAY, 0, KEY_TYPE_SHORT_UP}, EVENT_DIRECTORY_ENTER_SON},
    /*! 短按KEY_VOL 按键转换为 BACK_PARENT 事件 */
    {{KEY_VOL, 0, KEY_TYPE_SHORT_UP}, EVENT_DIRECTORY_BACK_PARENT},
    /*! 短按KEY_MENU 按键转换为 ENTER_OPTION 事件 */
    {{KEY_MODE, 0, KEY_TYPE_SHORT_UP}, EVENT_DIRECTORY_ENTER_OPTION},
    
    /*! 结束标志 */
    {{KEY_NULL, 0, KEY_TYPE_NULL}, MSG_NULL},
};

/*!
 * \brief
 *  keylock_list：按键锁UI按键映射表
 */
const key_map_t keylock_list[] = 
{
    /*! 解锁过程中把锁键拨到常关状态，终止解锁，显示上锁动画 */
    {{KEY_LOCK, 0, KEY_TYPE_DOWN}, EVENT_ENTER_KEYLOCK},
    /*! 上锁过程中把锁键拨到常开状态，终止上锁，显示解锁动画 */
    {{KEY_UNLOCK, 0, KEY_TYPE_DOWN}, EVENT_ENTER_KEYLOCK},
    
    /*! lock 状态下不处理音量快捷键 */
    {{KEY_VOL, 0, KEY_TYPE_LONG | KEY_TYPE_HOLD}, MSG_NULL},
    
    /*! 结束标志 */
    {{KEY_NULL, 0, KEY_TYPE_NULL}, MSG_NULL},
};

/*!
 * \brief
 *  menulist_key_map_list：菜单列表按键映射表
 */
const key_map_t menulist_key_map_list[] = 
{
    /*! PREV 按键转换为 SELECT_PREV 事件 */
    {{KEY_PREV, 0, KEY_TYPE_DOWN | KEY_TYPE_LONG | KEY_TYPE_HOLD}, EVENT_MENULIST_SELECT_PREV},
    /*! NEXT 按键转换为 SELECT_NEXT 事件 */
    {{KEY_NEXT, 0, KEY_TYPE_DOWN | KEY_TYPE_LONG | KEY_TYPE_HOLD}, EVENT_MENULIST_SELECT_NEXT},
    /*! 短按KEY_PLAY 按键转换为 ENTER_SON 事件 */
    {{KEY_PLAY, 0, KEY_TYPE_SHORT_UP}, EVENT_MENULIST_ENTER_SON},
    /*! 短按KEY_VOL 按键转换为 BACK_PARENT 事件 */
    {{KEY_VOL, 0, KEY_TYPE_SHORT_UP}, EVENT_MENULIST_BACK_PARENT},
    /*! 短按KEY_MENU 按键转换为 ENTER_OPTION 事件 */
    {{KEY_MODE, 0, KEY_TYPE_SHORT_UP}, EVENT_MENULIST_ENTER_OPTION},
    
    /*! 结束标志 */
    {{KEY_NULL, 0, KEY_TYPE_NULL}, MSG_NULL},
};

/*!
 * \brief
 *  parameter_key_map_list：参数设置框按键映射表
 */
const key_map_t parameter_key_map_list[] = 
{
    /*! MODE 按键转换为 SELECT_NEXT 事件 */
    {{KEY_MODE, 0, KEY_TYPE_SHORT_UP}, EVENT_PARAMETER_SELECT_NEXT},
    /*! NEXT 按键转换为 VALUE_INC 事件 */
    {{KEY_NEXT, 0, KEY_TYPE_DOWN | KEY_TYPE_LONG | KEY_TYPE_HOLD}, EVENT_PARAMETER_VALUE_INC},
    /*! PREV 按键转换为 VALUE_DEC 事件 */
    {{KEY_PREV, 0, KEY_TYPE_DOWN | KEY_TYPE_LONG | KEY_TYPE_HOLD}, EVENT_PARAMETER_VALUE_DEC},
    /*! 短按KEY_PLAY 按键转换为 CONFIRM 事件 */
    {{KEY_PLAY, 0, KEY_TYPE_SHORT_UP}, EVENT_PARAMETER_CONFIRM},
    /*! 短按KEY_VOL 按键转换为 CANCEL 事件 */
    {{KEY_VOL, 0, KEY_TYPE_SHORT_UP}, EVENT_PARAMETER_CANCEL},
    
    /*! 结束标志 */
    {{KEY_NULL, 0, KEY_TYPE_NULL}, MSG_NULL},
};

/*!
 * \brief
 *  shutoff_key_map_list：关机确定UI按键映射表
 */
const key_map_t shutoff_key_map_list[] = 
{
    /*! 只处理 PLAY 按键抬起时终止 */
    {{KEY_PLAY, 0, KEY_TYPE_SHORT_UP}, EVENT_SHUTOFF_QUIT},
    
    /*! 结束标志 */
    {{KEY_NULL, 0, KEY_TYPE_NULL}, MSG_NULL},
};

/*!
 * \brief
 *  textread_key_map_list：文本信息查看器按键映射表
 */
const key_map_t textread_key_map_list[] = 
{
    /*! PREV 按键转换为 PREV_PAGE 事件 */
    {{KEY_PREV, 0, KEY_TYPE_DOWN | KEY_TYPE_LONG | KEY_TYPE_HOLD}, EVENT_TEXTREAD_PREV_PAGE},
    /*! NEXT 按键转换为 NEXT_PAGE 事件 */
    {{KEY_NEXT, 0, KEY_TYPE_DOWN | KEY_TYPE_LONG | KEY_TYPE_HOLD}, EVENT_TEXTREAD_NEXT_PAGE},
    /*! 短按KEY_VOL 按键转换为 CANCEL 事件 */
    {{KEY_VOL, 0, KEY_TYPE_SHORT_UP}, EVENT_TEXTREAD_CANCEL},
    /*! 短按KEY_PLAY 按键转换为 CANCEL 事件 */
    {{KEY_PLAY, 0, KEY_TYPE_SHORT_UP}, EVENT_TEXTREAD_CANCEL},
    
    /*! 结束标志 */
    {{KEY_NULL, 0, KEY_TYPE_NULL}, MSG_NULL},
};

/*!
 * \brief
 *  usbconnect_key_map_list：设备连接对话框按键映射表
 */
const key_map_t usbconnect_key_map_list[] = 
{
    /*! PREV 按键转换为 PREV_BUTTON 事件 */
    {{KEY_PREV, 0, KEY_TYPE_DOWN | KEY_TYPE_LONG | KEY_TYPE_HOLD}, EVENT_DIALOG_PREV_BUTTON},
    /*! NEXT 按键转换为 NEXT_BUTTON 事件 */
    {{KEY_NEXT, 0, KEY_TYPE_DOWN | KEY_TYPE_LONG | KEY_TYPE_HOLD}, EVENT_DIALOG_NEXT_BUTTON},
    /*! 短按KEY_PLAY 按键转换为 DIALOG_CONFIRM 事件 */
    {{KEY_PLAY, 0, KEY_TYPE_SHORT_UP}, EVENT_DIALOG_CONFIRM},
    /*! 短按KEY_VOL 按键转换为 DIALOG_CANCEL 事件 */
    {{KEY_VOL, 0, KEY_TYPE_SHORT_UP}, EVENT_DIALOG_CANCEL},
    /*! 短按KEY_MODE 按键转换为 DIALOG_CANCEL 事件 */
    {{KEY_MODE, 0, KEY_TYPE_SHORT_UP}, EVENT_DIALOG_CANCEL},
    
    /*! 不响应以下快捷键 */
    {{KEY_PLAY, 0, KEY_TYPE_LONG}, MSG_NULL},
    {{KEY_LOCKHOLD, 0, KEY_TYPE_ALL}, MSG_NULL},
    {{KEY_VOL, 0, KEY_TYPE_LONG | KEY_TYPE_HOLD}, MSG_NULL},
    
    /*! 结束标志 */
    {{KEY_NULL, 0, KEY_TYPE_NULL}, MSG_NULL},
};

/*!
 * \brief
 *  volume_key_map_list：音量条按键映射表
 */
const key_map_t volume_key_map_list[] = 
{
    /*! NEXT 按键转换为 VOLUME_INC 事件 */
    {{KEY_NEXT, 0, KEY_TYPE_DOWN | KEY_TYPE_LONG | KEY_TYPE_HOLD}, EVENT_VOLUME_INC},
    /*! PREV 按键转换为 VOLUME_DEC 事件 */
    {{KEY_PREV, 0, KEY_TYPE_DOWN | KEY_TYPE_LONG | KEY_TYPE_HOLD}, EVENT_VOLUME_DEC},
    /*! 短按KEY_PLAY 按键转换为 VOLUME_CANCEL 事件，音量调节是即时调节的，无需返回CONFRIM */
    {{KEY_PLAY, 0, KEY_TYPE_SHORT_UP}, EVENT_VOLUME_CANCEL},
    /*! 短按KEY_VOL 按键转换为 VOLUME_CANCEL 事件，音量调节是即时调节的，无需返回CONFRIM */
    {{KEY_VOL, 0, KEY_TYPE_SHORT_UP}, EVENT_VOLUME_CANCEL},
    /*! 短按KEY_MODE 按键转换为 VOLUME_CANCEL 事件，音量调节是即时调节的，无需返回CONFRIM */
    {{KEY_MODE, 0, KEY_TYPE_SHORT_UP}, EVENT_VOLUME_CANCEL},
    
    /*! 过滤音量调节快捷键 */
    {{KEY_VOL, 0, KEY_TYPE_LONG | KEY_TYPE_HOLD}, MSG_NULL},
    
    /*! 结束标志 */
    {{KEY_NULL, 0, KEY_TYPE_NULL}, MSG_NULL},
};

/*!
 * \brief
 *  limit_key_map_list：音量限制设置按键映射表
 */
const key_map_t limit_key_map_list[] = 
{
    /*! NEXT 按键转换为 VOLUME_INC 事件 */
    {{KEY_NEXT, 0, KEY_TYPE_DOWN | KEY_TYPE_LONG | KEY_TYPE_HOLD}, EVENT_VOLUME_INC},
    /*! PREV 按键转换为 VOLUME_DEC 事件 */
    {{KEY_PREV, 0, KEY_TYPE_DOWN | KEY_TYPE_LONG | KEY_TYPE_HOLD}, EVENT_VOLUME_DEC},
    /*! 短按KEY_PLAY 按键转换为 VOLUME_CONFIRM事件 */
    {{KEY_PLAY, 0, KEY_TYPE_SHORT_UP}, EVENT_VOLUME_CONFIRM},
    /*! 短按KEY_VOL 按键转换为 VOLUME_CANCEL事件 */
    {{KEY_VOL, 0, KEY_TYPE_SHORT_UP}, EVENT_VOLUME_CANCEL},
    /*! 短按KEY_MODE 按键转换为 VOLUME_CANCEL事件 */
    {{KEY_MODE, 0, KEY_TYPE_SHORT_UP}, EVENT_VOLUME_CANCEL},
    
    /*! 过滤音量调节快捷键 */
    {{KEY_VOL, 0, KEY_TYPE_LONG | KEY_TYPE_HOLD}, MSG_NULL},
    
    /*! 结束标志 */
    {{KEY_NULL, 0, KEY_TYPE_NULL}, MSG_NULL},
};

/*!
 * \brief
 *  key_shortcut_list：快捷键映射表
 */
const key_map_t key_shortcut_list[] = 
{
    /*! 长按 PLAY 按键关机 */
    {{KEY_PLAY, 0, KEY_TYPE_LONG}, EVENT_ENTER_SHUTOFF_ASK},
    /*! 锁键拨到常关状态，键盘上锁 */
    {{KEY_LOCK, 0, KEY_TYPE_DOWN}, EVENT_ENTER_KEYLOCK},
    /*! 锁键拨到常开状态，键盘解锁 */
    {{KEY_UNLOCK, 0, KEY_TYPE_DOWN}, EVENT_ENTER_KEYLOCK_UN},
    /*! 虚拟按键，当键盘锁住时按下任何其他按键，会转换为 KEY_LOCKHOLD，显示键盘锁住 */
    {{KEY_LOCKHOLD, 0, KEY_TYPE_ALL}, EVENT_ENTER_KEYLOCK_HOLD},
    /*! 长按 VOL 键弹出音量调节框 */
    {{KEY_VOL, 0, KEY_TYPE_LONG | KEY_TYPE_HOLD}, EVENT_ENTER_VOLUMEBAR},
    
    /*! 结束标志 */
    {{KEY_NULL, 0, KEY_TYPE_NULL}, MSG_NULL},
};

/*! \endcond */
