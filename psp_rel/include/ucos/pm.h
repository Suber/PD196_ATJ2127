/********************************************************************************
 *                              USDK 1100
 *                            Module: PM
 *                 Copyright(c) 2001-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       wuyueqian     2011-9-10 9:42     1.0             build this file
 ********************************************************************************/
/*!
 * \file     pm.h
 * \brief    功耗管理宏定义和函数接口声明
 * \author   wuyueqian
 * \version 1.0
 * \date   2011-9-10
 *******************************************************************************/
#ifndef __PM_H__
#define __PM_H__

typedef struct{
	uint8 mucpll;	
	uint8 vdd;
	uint8 vcc;
	/*只对CPU分频进行设置，AHB总线分频固定为1/2*/
	uint8 cpuclk_ctl;
	/*! 频点cp0_count溢出时间*/
	uint32 overflow_value_us;
}frequency_param_t;


typedef struct
{
    uint8 freq;
    uint8 cycle;
    uint8 divisor;
}spi_clk_param_t;


#define K_VCC_2_7               0
#define K_VCC_2_8               1
#define K_VCC_2_9               2
#define K_VCC_3_0               3
#define K_VCC_3_1               4
#define K_VCC_3_2               5
#define K_VCC_3_3               6
#define K_VCC_3_4               7

#define K_VDD_0_8               0       // 0.80V
#define K_VDD_0_85              1       // 0.85V
#define K_VDD_0_9               2       // 0.9V
#define K_VDD_0_95              3       // 0.95V
#define K_VDD_1_0               4       // 1.0V
#define K_VDD_1_05              5       // 1.05V
#define K_VDD_1_1               6       // 1.1V
#define K_VDD_1_15              7       // 1.15V
#define K_VDD_1_2               8       // 1.2V
#define K_VDD_1_25              9       // 1.25V
#define K_VDD_1_3               10      // 1.3V
#define K_VDD_1_35              11      // 1.35V
#define K_VDD_1_4               12      // 1.4V
#define K_VDD_1_45              13      // 1.45V
#define K_VDD_1_5               15      // 1.5V


typedef enum{
	FREQ_0M = 0,
	FREQ_3M,
	FREQ_6M,
	FREQ_12M,
	FREQ_24M,
	FREQ_30M,
	FREQ_36M,
	FREQ_42M,
	FREQ_48M,
	FREQ_54M,
	FREQ_60M,
	FREQ_72M,
	FREQ_90M,
	FREQ_96M,
	FREQ_108M,
	FREQ_120M,
	FREQ_132M,
	FREQ_144M,
	FREQ_156M,
	FREQ_168M,
	FREQ_180M,
	FREQ_186M,							
}freq_level_e;

typedef struct {
	/*!调频回调函数指针 */
	void (*handler)(const frequency_param_t *curclk);
} clk_cb_action_t;




extern unsigned int local_irq_save(void *null0, void* null1, void* null2) __FAR__;
extern void local_irq_restore(unsigned int flags, void* null1, void* null2) __FAR__;
extern int request_clkadjust(void(*handler)(const frequency_param_t *curclk), void* null2, void *null3) __FAR__;
extern int free_clkadjust(unsigned int index, void* null2, void *null3) __FAR__;
extern int adjust_clk(uint8 freq_level, bool is_back, void* null3) __FAR__;
extern void adjust_nand_clk(const frequency_param_t *freq_param) __FAR__;
extern void adjust_nor_clk(const frequency_param_t *freq_param) __FAR__;
extern void adjust_base_device_clk(const frequency_param_t *freq_param) __FAR__;





#endif /*__PM_H__*/
