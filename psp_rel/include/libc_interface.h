/********************************************************************************
 *                              USDK(GL5110)
 *                            Module: KERNEL
 *                 Copyright(c) 2003-2011 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>      <time>           <version >             <desc>
 *      wuyueqian     2011-9-10 09:00     1.0             build this file
 ********************************************************************************/
/*!
 * \file     libc_interface.h
 * \brief    libc总接口
 * \author   wuyueqian
 * \version  1.0
 * \date  2011/9/10
 *******************************************************************************/

#ifndef __LIBC_DEFINE_H__
#define __LIBC_DEFINE_H__

#include <attr.h>
#include <typeext.h>

typedef enum
{
	MEMCPY = 0,
	MEMSET,
	MEMCMP,
	STRLEN,
	STRNCAT,
	STRNCMP,
	STRNCPY,
	STRLENUNI,
	STRNCPYUNI,
	STRNCATUNI,
	ITOA,
	PRINT,

	PTHREAD_EXIT,
	PTHREAD_CREATE,
	EXIT,
	WAITPID,
	GET_PROCESS_STRUCT,
	FREE_PROCESS_STRUCT,
	SEM_INIT,
	SEM_DESTROY,
	SEM_WAIT,
	SEM_TRYWAIT,
	SEM_POST,
	PTHREAD_MUTEX_INIT,
	PTHREAD_MUTEX_DESTROY,
	PTHREAD_MUTEX_LOCK,
	PTHREAD_MUTEX_UNLOCK,
	PTHREAD_MUTEX_TRYLOCK,
	PTHREAD_COND_INIT,
	PTHREAD_COND_DESTROY,
	PTHREAD_COND_WAIT,
	PTHREAD_COND_SIGNAL,
	
	C_RESERVE_FUN1,
	C_RESERVE_FUN2,

} libc_cmd_e;

/*
 KERNEL向上层提供统一的接口，即只提供一个API，并以ID的方式区分
 */
extern void* libc_op_entry(void *param1, void *param2, void *param3, libc_cmd_e cmd_type) __FAR__;

typedef void* (*libc_op_i)(void*, void*, void*);

struct libc_operations {
	libc_op_i memcpy;
	libc_op_i memset;
	libc_op_i memcmp;
	libc_op_i strlen;
	libc_op_i strncat;
	libc_op_i strncmp;
	libc_op_i strncpy;
	libc_op_i strlenuni;
	libc_op_i strncpyuni;
	libc_op_i strncatuni;
	libc_op_i itoa;
	libc_op_i print;

	libc_op_i pthread_exit;
	libc_op_i pthread_create;
	libc_op_i exit;
	libc_op_i waitpid;
	libc_op_i get_process_struct;
	libc_op_i free_process_struct;
	libc_op_i sem_init;
	libc_op_i sem_destroy;
	libc_op_i sem_wait;
	libc_op_i sem_trywait;
	libc_op_i sem_post;
	libc_op_i pthread_mutex_init;
	libc_op_i pthread_mutex_destroy;
	libc_op_i pthread_mutex_lock;
	libc_op_i pthread_mutex_unlock;
	libc_op_i pthread_mutex_trylock;
	libc_op_i pthread_cond_init;
	libc_op_i pthread_cond_destroy;
	libc_op_i pthread_cond_wait;
	libc_op_i pthread_cond_signal;
	
	libc_op_i c_reserve1;
	libc_op_i c_reserve2;

};

#define libc_memcpy(a,b,c)          libc_op_entry((void*)(a), (void*)(b), (void*)(c), MEMCPY)
#define libc_memset(a,b,c)          libc_op_entry((void*)(a), (void*)(b), (void*)(c), MEMSET)
#define libc_memcmp(a,b,c)          (int)libc_op_entry((void*)(a), (void*)(b), (void*)(c), MEMCMP)
#define libc_strlen(a)              (unsigned int)libc_op_entry((void*)(a), (void*)0, (void*)0, STRLEN)
#define libc_strncat(a,b,c)         (char *)libc_op_entry((void*)(a), (void*)(b), (void*)(c), STRNCAT)

#define libc_strncmp(a,b,c)         (int)libc_op_entry((void*)(a), (void*)(b), (void*)(c), STRNCMP)
#define libc_strncpy(a,b,c)         (char *)libc_op_entry((void*)(a), (void*)(b), (void*)(c), STRNCPY)
#define libc_strlenuni(a)           libc_op_entry((void*)(a), (void*)0, (void*)0, STRLENUNI)
#define libc_strncpyuni(a,b,c)      (int)libc_op_entry((void*)(a), (void*)(b), (void*)(c), STRNCPYUNI)
#define libc_strncatuni(a,b,c)		(int)libc_op_entry((void*)(a), (void*)(b), (void*)(c), STRNCATUNI)
#define libc_itoa(a,b,c)            (uint8)(uint32)libc_op_entry((void*)(uint32)(a), (void*)(b), (void*)(uint32)(c), ITOA)
#define libc_print(a,b,c)           libc_op_entry((void*)(a), (void*)(b), (void*)(uint32)(c), PRINT)

#define libc_pthread_exit()         libc_op_entry((void*)0, (void*)0, (void*)0, PTHREAD_EXIT)
#define libc_pthread_create(a,b,c)  \
                            (int)libc_op_entry((void*)(a), (void*)(uint32)(b), (void*)(uint32)(c), PTHREAD_CREATE)

#define libc_exit(a)                libc_op_entry((void*)(int32)(a), (void*)0, (void*)0, EXIT)
#define libc_waitpid(a,b)           (int32)libc_op_entry((void*)(a), (void*)(b), (void*)0, WAITPID)
#define libc_get_process_struct()   (int8)(int32)libc_op_entry((void*)0, (void*)0, (void*)0, GET_PROCESS_STRUCT)
#define libc_free_process_struct(a) libc_op_entry((void*)(int32)(a), (void*)0, (void*)0, FREE_PROCESS_STRUCT)
#define libc_sem_init(a,b)          (int)libc_op_entry((void*)(a), (void*)(b), (void*)0, SEM_INIT)

#define libc_sem_destroy(a)         (int)libc_op_entry((void*)(a), (void*)0, (void*)0, SEM_DESTROY)
#define libc_sem_wait(a,b)          (int)libc_op_entry((void*)(a), (void*)(uint32)(b), (void*)0, SEM_WAIT)
#define libc_sem_trywait(a)         (int)libc_op_entry((void*)(a), (void*)0, (void*)0, SEM_TRYWAIT)
#define libc_sem_post(a)            (int)libc_op_entry((void*)(a), (void*)0, (void*)0, SEM_POST)

#define libc_pthread_mutex_init(a)      (int)libc_op_entry((void*)(a), (void*)0, (void*)0, PTHREAD_MUTEX_INIT)
#define libc_pthread_mutex_destroy(a)   (int)libc_op_entry((void*)(a), (void*)0, (void*)0, PTHREAD_MUTEX_DESTROY)
#define libc_pthread_mutex_lock(a)      (int)libc_op_entry((void*)(a), (void*)0, (void*)0, PTHREAD_MUTEX_LOCK)
#define libc_pthread_mutex_unlock(a)    (int)libc_op_entry((void*)(a), (void*)0, (void*)0, PTHREAD_MUTEX_UNLOCK)
#define libc_pthread_mutex_trylock(a)   (int)libc_op_entry((void*)(a), (void*)0, (void*)0, PTHREAD_MUTEX_TRYLOCK)
#define libc_pthread_cond_init(a,b)       (int)libc_op_entry((void*)(a), (void*)(b), (void*)0, PTHREAD_COND_INIT)
#define libc_pthread_cond_destroy(a)    (int)libc_op_entry((void*)(a), (void*)0, (void*)0, PTHREAD_COND_DESTROY)
#define libc_pthread_cond_wait(a,b,c)   (int)libc_op_entry((void*)(a), (void*)(b), (void*)(uint32)(c), PTHREAD_COND_WAIT)
#define libc_pthread_cond_signal(a)     (int)libc_op_entry((void*)(a), (void*)0, (void*)0, PTHREAD_COND_SIGNAL)

#endif

