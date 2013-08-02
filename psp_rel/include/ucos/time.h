/********************************************************************************
 *                               USDK(1100)
 *                            Module: TIME
 *                 Copyright(c) 2001-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *        wuyueqian     2011-9-10 9:42     1.0             build this file
 ********************************************************************************/
/*!
 * \file     time.h
 * \brief    时间头文件
 * \author   wuyueqian
 * \version 1.0
 * \date   2011-9-10
 *******************************************************************************/
#ifndef __OS_TIME_H__
#define __OS_TIME_H__

#define HZ 100

#ifndef _ASSEMBLER_

#include <ucos/types.h>
#include <ucos/init.h>

/*! time, second */
typedef long _time_t;

/*! time, micro second */
typedef long _suseconds_t;

/*!
 * \brief
 *      timeval, 系统时间，包括m和us
 */
struct timeval {
	/*! seconds */
	_time_t tv_sec;
	/*! microseconds */
	_suseconds_t tv_usec;
};

/* Parameters used to convert the timespec values */
#define MSEC_PER_SEC (1000L)
#define USEC_PER_SEC (1000000L)
//#define NSEC_PER_SEC (1000000000L)
//#define NSEC_PER_USEC (1000L)

extern void hw_timer_setup();

typedef struct {
	uint16 year;
	uint8 month;
	uint8 day;
} date_t;

typedef struct {
	uint8 hour;
	uint8 minute;
	uint8 second;
} time_t;

extern void set_time(time_t *time, void *null1, void *null2);
extern void get_time(time_t *time, void *null1, void *null2);
extern int set_date(date_t *date, void *null1, void *null2);
extern void get_date(date_t *date, void *null1, void *null2);
extern void set_alarm_time(time_t *time, void *null1, void *null2);
extern void get_alarm_time(time_t *time, void *null1, void *null2);

extern void set_gwtime(time_t *time, void *null1, void *null2);
extern void get_gwtime(time_t *time, void *null1, void *null2);
extern int set_gwdate(date_t *date, void *null1, void *null2);
extern void get_gwdate(date_t *date, void *null1, void *null2);



#endif /*_ASSEMBLER_*/
#endif /*__OS_TIME_H__*/
