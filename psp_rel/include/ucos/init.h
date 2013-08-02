/********************************************************************************
 *                              USDK(1100)
 *                            Module: INIT
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *      wuyueqian     2011-9-10 15:00     1.0             build this file
 ********************************************************************************/
/*!
 * \file     init.h
 * \brief    module_init/_exit∫Í∂®“Â
 * \author   wuyueqian
 * \version 1.0
 * \date  2011-9-10
 *******************************************************************************/

#ifndef __INIT_H__
#define __INIT_H__
#include <attr.h>

#ifndef PC
#define __init	__section__ (".init")
#define __exit	__section__ (".fini")
#else
#define __init
#define __exit
#endif

typedef void (*execption_print_i)(void *param);

typedef int (*initcall_t)(void*, void*, void*); /* module init function type. */
typedef int (*exitcall_t)(void*, void*, void*); /* module exit function type. */

/* Each module must use one module_init(), or one no_module_init */
#define module_init(initfn)					\
	static inline initcall_t __inittest(void)		\
	{ return initfn; }					\
	int init_module(void) __attribute__((alias(#initfn)));

/* This is only required if you want to be unloadable. */
#define module_exit(exitfn)					\
	static inline exitcall_t __exittest(void)		\
	{ return exitfn; }					\
	void cleanup_module(void) __attribute__((alias(#exitfn)));

#endif  /* __INIT_H__ */
