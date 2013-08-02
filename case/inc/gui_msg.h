/*******************************************************************************
 *                              us211A
 *                            Module: Applib
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved. 
 *
 * History:         
 *      <author>    <time>           <version >             <desc>
 *       lzcai     2011-11-27 22:03     1.0             build this file 
*******************************************************************************/
/*!
 * \file     gui_msg.h
 * \brief    gui 消息处理模块相关数据结构，宏定义等
 * \author   lzcai
 * \version  1.0
 * \date     2011-11-27
*******************************************************************************/

#ifndef _gui_msg_H_
#define _gui_msg_H_

#include "psp_includes.h"

/*!
 * \brief
 *  input_msg_type_e 输入型消息类型，也叫 gui 消息类型
 */
typedef enum
{
    /*! 普通按键 */
    INPUT_MSG_KEY       = 0x00,
    /*! 触摸屏 */
    INPUT_MSG_TOUCH     = 0x01,
    /*! g-sensor */
    INPUT_MSG_GSENSOR   = 0x02,
} input_msg_type_e;

/*!
 * \brief
 *  key_value_e 逻辑按键枚举类型
 */
typedef enum
{
    /*! 空按键 */
    KEY_NULL        = 0x00,
    /*! 播放/暂停按键 */
    KEY_PLAY        = 0x05,
    /*! 上一项（左）按键 */
    KEY_PREV        = 0x04,
    /*! 下一项（右）按键 */
    KEY_NEXT        = 0x06,
    /*! 音量调节快捷按键 */
    KEY_VOL         = 0x0a,
    /*! 音量加快捷按键 */
    KEY_VADD        = 0x09,
    /*! 音量减快捷按键 */
    KEY_VSUB        = 0x07,
    /*! 菜单/选项按键 */
    KEY_MODE        = 0x03,
    /*! 锁键关 */
    KEY_LOCK        = 0x0c,
    /*! 锁键开，只在拨动开关需要 */
    KEY_UNLOCK      = 0x0d,
    /*! 虚拟锁保持按键，按键锁住状态下，按键普通按键后，转换为该按键 */
    KEY_LOCKHOLD    = 0x7f,
    
    KEY_RETURN      = 0x80,
    KEY_POWER       = 0x81,
    KEY_UP          = 0x82,
    KEY_DOWN        = 0x83,
} key_value_e;

/*!
 * \brief
 *  key_type_e 按键类型
 */
typedef enum
{
    /*! 没有按键*/
    KEY_TYPE_NULL       = 0x0000,
    /*! 按键按下*/
    KEY_TYPE_DOWN       = 0x2000,
    /*! 按键长按*/
    KEY_TYPE_LONG       = 0x1000,
    /*! 按键保持*/
    KEY_TYPE_HOLD       = 0x0800,
    /*! 按键短按弹起*/
    KEY_TYPE_SHORT_UP   = 0x0400,
    /*! 按键长按弹起*/
    KEY_TYPE_LONG_UP    = 0x0200,
    /*! 按键保持弹起*/
    KEY_TYPE_HOLD_UP    = 0x0100,
    /*! 任意按键类型 */
    KEY_TYPE_ALL        = 0x3f00,
} key_type_e;


/*!
 * \brief  
 *  key_event_t 按键事件结构体
 */
typedef struct
{
    /*! 按键（逻辑）值*/
    key_value_e val;
    uint8 reserve;
    /*! 按键类型*/
    key_type_e type;
} key_event_t;

/*!
 * \brief  
 *  tp_type_e 触摸类型
 */
typedef enum
{
    /*! 触摸按下*/
    TP_TYPE_DOWN        = 0x0,
    /*! 触摸弹起*/
    TP_TYPE_UP          = 0x1,
    /*! 触摸移动*/
    TP_TYPE_MOVE        = 0x2,
} tp_type_e;

/*!
 * \brief  
 *  touch_event_t 触摸事件结构体，压缩为24bit
 */
typedef struct
{
    /*! 触摸点X坐标，10bit取值范围：0~1023*/
    uint16 x;
    /*! 触摸点Y坐标，10bit取值范围：0~1023*/
    uint16 y;
    /*! 触摸类型，4bit有效 */
    tp_type_e type;
} touch_event_t;

/*!
 * \brief  
 *  input_gui_msg_t 输入型消息，又称 gui 消息结构体
 */
typedef struct
{
    /*! 消息类型*/
    input_msg_type_e type;
    /*! 消息数据*/
    union
    {
        /*! 按键事件*/
        key_event_t kmsg;
        /*! 触摸事件*/
        touch_event_t tmsg;
    }data;
} input_gui_msg_t;

#endif
