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
* \file     kernel_interface.h
* \brief    内核接口列表
* \author   wuyueqian
* \version  1.0
* \date  2011/9/10
*******************************************************************************/

#ifndef __KERNEL_DEFINE_H__
#define __KERNEL_DEFINE_H__
#include <attr.h>
#include <typeext.h>

typedef enum
{
EXECE_AP = 0,
LOAD_CODEC,
LOAD_MMM,
FREE_AP,
FREE_CODEC,
FREE_MMM,
BANK_FLUSH,
DETECT_DISK,
DRV_INSTALL,
DRV_UNINSTALL,
GET_DRV_INSTALL_INFO,
REQUEST_DMA_CHAN,
FREE_DMA_CHAN,

SD_FOPEN,
SD_FCLOSE,
SD_FSEEK,
SD_FTELL,
SD_FREAD,

VM_READ,
VM_WRITE,

REQUEST_IRQ,
FREE_IRQ,

MALLOC,
FREE,

MQ_SEND,
MQ_RECEIVE,
MQ_FLUSH,

GET_FW_INFO,
GET_AB_TIMER,

MDELAY,
UDELAY,
SLEEP,
USLEEP,

SET_IRQ_TIMER1,
DEL_IRQ_TIMER1,


SET_TIME,
GET_TIME,
SET_DATE,
GET_DATE,
SET_ALARM_TIME,
GET_ALARM_TIME,

MOUNT_FS,
UNMOUNT_FS,
GET_FAT_TYPE_AFTER_MOUNT,
FORMAT_DISK,

LOCAL_IRQ_SAVE,
LOCAL_IRQ_RESTORE,

OS_TASK_RESUME,
OS_TASK_SUSPEND,
OS_TIME_DLY,
OS_TIME_DLY_RESUME,
OS_SCHED_LOCK,
OS_SCHED_UNLOCK,

RANDOM,
SET_GWTIME,
GET_GWTIME,
SET_GWDATE,
GET_GWDATE,

SET_VIDEO_APP,
GET_SYS_INFO,
REQUEST_CLKADJUST,
FREE_CLKADJUST,
ADJUST_CLK,
VM_WRITE_MULTI,
RESERVE_7,
RESERVE_8,
RESERVE_9,
    
}kernel_cmd_e;

/*
KERNEL向上层提供统一的接口，即只提供一个API，并以ID的方式区分
*/
extern void* sys_op_entry(void *param1, void *param2, void *param3, kernel_cmd_e cmd_type) __FAR__;

typedef void* (*sys_op_i)(void*, void*, void*);

struct sys_operations {
sys_op_i exece_ap;
sys_op_i load_codec;
sys_op_i load_mmm;
sys_op_i free_ap;
sys_op_i free_codec;
sys_op_i free_mmm;
sys_op_i bank_flush;
sys_op_i detect_disk;
sys_op_i drv_install;
sys_op_i drv_uninstall;
sys_op_i get_drv_install_info;
sys_op_i request_dma_chan;
sys_op_i free_dma_chan;

sys_op_i sd_fopen;
sys_op_i sd_fclose;
sys_op_i sd_fseek;
sys_op_i sd_ftell;
sys_op_i sd_fread;

sys_op_i vm_read;
sys_op_i vm_write;

sys_op_i request_irq;
sys_op_i free_irq;

sys_op_i malloc;
sys_op_i free;

sys_op_i mq_send;
sys_op_i mq_receive;
sys_op_i mq_flush;

sys_op_i get_fw_info;
sys_op_i get_ab_timer;

sys_op_i mdelay;
sys_op_i udelay;
sys_op_i sleep;
sys_op_i usleep;

sys_op_i set_irq_timer1;
sys_op_i del_irq_timer1;


sys_op_i set_time;
sys_op_i get_time;
sys_op_i set_date;
sys_op_i get_date;
sys_op_i set_alarm_time;
sys_op_i get_alarm_time;

sys_op_i mount_fs;
sys_op_i unmount_fs;
sys_op_i get_fat_type_after_mount;
sys_op_i format_disk;

sys_op_i local_irq_save;
sys_op_i local_irq_restore;
sys_op_i os_task_resume;
sys_op_i os_task_suspend;
sys_op_i os_time_dly;
sys_op_i os_time_dly_resume;
sys_op_i os_sched_lock;
sys_op_i os_sched_unlock;

sys_op_i random;
sys_op_i set_gwtime;
sys_op_i get_gwtime;
sys_op_i set_gwdate;
sys_op_i get_gwdate;

sys_op_i set_video_app;
sys_op_i get_sys_info;
sys_op_i request_clkadjust;
sys_op_i free_clkadjust;
sys_op_i adjust_clk;
sys_op_i vm_write_multi;
sys_op_i reserve_7;
sys_op_i reserve_8;
sys_op_i reserve_9;

};



#define sys_exece_ap(a,b,c)         (int)sys_op_entry((void*)(a), (void*)(uint32)(b), (void*)(c), EXECE_AP) 
#define sys_load_codec(a,b)         (int)sys_op_entry((void*)(a), (void*)(uint32)(b), (void*)0, LOAD_CODEC)
#define sys_load_mmm(a,b)           (int)sys_op_entry((void*)(a), (void*)(uint32)(b), (void*)0, LOAD_MMM)
#define sys_free_ap(a)              sys_op_entry((void*)(uint32)(a), (void*)0, (void*)0, FREE_AP)
#define sys_free_codec(a)           sys_op_entry((void*)(uint32)(a), (void*)0, (void*)0, FREE_CODEC)
#define sys_free_mmm(a)             sys_op_entry((void*)(uint32)(a), (void*)0, (void*)0, FREE_MMM)
#define sys_bank_flush(a)             (int)sys_op_entry((void*)(uint32)(a), (void*)0, (void*)0, BANK_FLUSH)

#define sys_detect_disk(a)          (int)sys_op_entry((void*)(uint32)(a), (void*)0, (void*)0, DETECT_DISK)
#define sys_drv_install(a,b,c)        (int)sys_op_entry((void*)(uint32)(a), (void*)(uint32)(b), (void*)(c), DRV_INSTALL)
#define sys_drv_uninstall(a)        (int)sys_op_entry((void*)(uint32)(a), (void*)0, (void*)0, DRV_UNINSTALL)
#define sys_get_drv_install_info(a) (int)sys_op_entry((void*)(uint32)(a), (void*)0, (void*)0, GET_DRV_INSTALL_INFO)

#define sys_request_dma_chan(a)     (int)sys_op_entry((void*)(a), (void*)0, (void*)0, REQUEST_DMA_CHAN) 
#define sys_free_dma_chan(a)        (int)sys_op_entry((void*)(a), (void*)0, (void*)0, FREE_DMA_CHAN)

#define sys_sd_fopen(a)             (sd_file_t *)sys_op_entry((void*)(a), (void*)0, (void*)0, SD_FOPEN)
#define sys_sd_fclose(a)            (int)sys_op_entry((void*)(a), (void*)0, (void*)0, SD_FCLOSE)
#define sys_sd_fseek(a,b,c)         (int)sys_op_entry((void*)(a), (void*)(uint32)(b), (void*)(c), SD_FSEEK)

#define sys_sd_ftell(a)             (uint32)sys_op_entry((void*)(a), (void*)0, (void*)0, SD_FTELL) 
#define sys_sd_fread(a,b,c)         (int32)sys_op_entry((void*)(a), (void*)(b), (void*)(c), SD_FREAD)
#define sys_vm_read(a,b,c)          (int)sys_op_entry((void*)(a), (void*)(b), (void*)(c), VM_READ)
#define sys_vm_write(a,b)           (int)sys_op_entry((void*)(a), (void*)(b), (void*)0, VM_WRITE)
#define sys_vm_write_multi(a,b,c)           (int)sys_op_entry((void*)(a), (void*)(b), (void*)(c), VM_WRITE_MULTI)

#define sys_request_irq(a,b)        (int)sys_op_entry((void*)(a), (void*)(b), (void*)0, REQUEST_IRQ)
#define sys_free_irq(a)             sys_op_entry((void*)(a), (void*)0, (void*)0, FREE_IRQ) 
#define sys_malloc(a,b)             (int)sys_op_entry((void*)(a), (void*)(b), (void*)0, MALLOC)
#define sys_free(a)                 (int)sys_op_entry((void*)(a), (void*)0, (void*)0, FREE)
#define sys_mq_send(a,b)            (int)sys_op_entry((void*)(uint32)(a), (void*)(b), (void*)0, MQ_SEND)

#define sys_mq_receive(a,b)         (int)sys_op_entry((void*)(uint32)(a), (void*)(b), (void*)0, MQ_RECEIVE) 
#define sys_mq_flush(a)             (int)sys_op_entry((void*)(uint32)(a), (void*)0, (void*)0, MQ_FLUSH)
#define sys_get_fw_info(a,b,c)      sys_op_entry((void*)(a), (void*)(uint32)(b), (void*)(uint32)(c), GET_FW_INFO)
#define sys_get_ab_timer()          (unsigned int)sys_op_entry((void*)0, (void*)0, (void*)0, GET_AB_TIMER)
#define sys_mdelay(a)               sys_op_entry((void*)(a), (void*)(0), (void*)(0), MDELAY)
#define sys_udelay(a)               sys_op_entry((void*)(a), (void*)0, (void*)(0), UDELAY) 
#define sys_sleep(a)                sys_op_entry((void*)(a), (void*)(0), (void*)0, SLEEP)
#define sys_usleep(a)               sys_op_entry((void*)(a), (void*)(0), (void*)(0), USLEEP)
#define sys_set_irq_timer1(a,b)     (int)sys_op_entry((void*)(a), (void*)(uint32)(b), (void*)0, SET_IRQ_TIMER1)
#define sys_del_irq_timer1(a)       (int)sys_op_entry((void*)(a), (void*)(0), (void*)(0), DEL_IRQ_TIMER1)

#define sys_set_time(a)             sys_op_entry((void*)(a), (void*)(0), (void*)(0), SET_TIME)
#define sys_get_time(a)             sys_op_entry((void*)(a), (void*)(0), (void*)(0), GET_TIME)
#define sys_set_date(a)             (int)sys_op_entry((void*)(a), (void*)(0), (void*)(0), SET_DATE)
#define sys_get_date(a)             sys_op_entry((void*)(a), (void*)(0), (void*)(0), GET_DATE)
#define sys_set_alarm_time(a)       sys_op_entry((void*)(a), (void*)(0), (void*)(0), SET_ALARM_TIME)
#define sys_get_alarm_time(a)       sys_op_entry((void*)(a), (void*)(0), (void*)(0), GET_ALARM_TIME)

#define sys_mount_fs(a,b,c)           (int)sys_op_entry((void*)(uint32)(a), (void*)(uint32)(b), (void*)(c), MOUNT_FS)
#define sys_unmount_fs(a)           (int)sys_op_entry((void*)(uint32)(a), (void*)0, (void*)0, UNMOUNT_FS)
#define sys_get_fat_type_after_mount(a) \
                        (uint8)(uint32)sys_op_entry((void*)(uint32)(a), (void*)0, (void*)0, GET_FAT_TYPE_AFTER_MOUNT)
#define sys_format_disk(a,b,c)      \
                        (int)sys_op_entry((void*)(uint32)(a), (void*)(uint32)(b), (void*)(uint32)(c), FORMAT_DISK)
#define sys_local_irq_save()        (unsigned int)sys_op_entry((void*)0, (void*)0, (void*)0, LOCAL_IRQ_SAVE)
#define sys_local_irq_restore(a)    sys_op_entry((void*)(a), (void*)0, (void*)0, LOCAL_IRQ_RESTORE)
#define sys_os_task_resume(a)       \
                        (unsigned char)(uint32)sys_op_entry((void*)(uint32)(a), (void*)0, (void*)0, OS_TASK_RESUME)
#define sys_os_task_suspend(a)      \
                        (unsigned char)(uint32)sys_op_entry((void*)(uint32)(a), (void*)0, (void*)0, OS_TASK_SUSPEND)
#define sys_os_time_dly(a)          sys_op_entry((void*)(uint32)(a), (void*)0, (void*)0, OS_TIME_DLY)
#define sys_os_time_dly_resume(a)   \
                        (uint8)(uint32)sys_op_entry((void*)(uint32)(a), (void*)0, (void*)0, OS_TIME_DLY_RESUME)
#define sys_os_sched_lock()         sys_op_entry((void*)0, (void*)0, (void*)0, OS_SCHED_LOCK)
#define sys_os_sched_unlock()       sys_op_entry((void*)0, (void*)0, (void*)0, OS_SCHED_UNLOCK)

#define sys_random()      (uint32)sys_op_entry((void*)0, (void*)0, (void*)0, RANDOM)

#define sys_set_video_app(a)      (uint32)sys_op_entry((void*)(uint32)a, (void*)0, (void*)0, SET_VIDEO_APP)
#define sys_get_sys_info(a)	     (uint32)sys_op_entry((void*)(uint32)a, (void*)0, (void*)0, GET_SYS_INFO)			

#define sys_request_clkadjust(a)	(uint32)sys_op_entry((void*)(uint32)(a), (void*)0, (void*)0, REQUEST_CLKADJUST)			
#define sys_free_clkadjust(a)	    (uint32)sys_op_entry((void*)(uint32)(a), (void*)0, (void*)0, FREE_CLKADJUST)			
#define sys_adjust_clk(a, b)	    (uint32)sys_op_entry((void*)(uint32)(a), (void*)(uint32)(b), (void*)0, ADJUST_CLK)


#endif




