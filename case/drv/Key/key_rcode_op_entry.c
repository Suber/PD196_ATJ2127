/*******************************************************************************
 *                              US212A
 *                            Module: Key driver
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       reagan     2011-9-2 14:45    1.0             build this file
 *******************************************************************************/
/*!
 * \file     key_op_entry.c
 * \brief    key api entrance
 * \author   reagan
 * \version 1.0
 * \date  2011-9-2
 *******************************************************************************/

#include "key_inner.h"

typedef void* (*key_op_i)(void*, void*, void*);
/*1.驱动内部实现的具体接口函数声明*/
struct block_key_operations
{
    key_op_i key_chargeget;
    key_op_i key_chargeset;
    key_op_i key_keytabaddr;
    key_op_i key_getholdstate;
    key_op_i key_speakcheck;
};

/*2.定义接口操作集结构体，并进行初始化*/
#ifndef PC
struct block_key_operations blk_op =
{   .key_chargeget = (key_op_i)key_inner_chargeget,
    .key_chargeset = (key_op_i)key_inner_chargeset,
    .key_keytabaddr = (key_op_i)key_inner_getkeytabaddress,
    .key_getholdstate = (key_op_i)key_inner_holdcheck,
    .key_speakcheck=(key_op_i)key_inner_speakcheck,
};
#else
struct block_key_operations blk_op =
{
    (key_op_i)key_inner_chargeget,
    (key_op_i)key_inner_chargeset,
    (key_op_i)key_inner_getkeytabaddress,
    (key_op_i)key_inner_holdcheck,
    (key_op_i)key_inner_speakcheck,
};
#endif

