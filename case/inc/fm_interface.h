#ifndef _FM_INTERFACE_H         
#define _FM_INTERFACE_H

#include "psp_includes.h"

typedef struct
{
    uint16 freq; //当前电台的频率。单位为1k (最高位恒为1)，取最低2 字节
    uint8 station; //当前电台对应的电台表项号，from 1 to station_count，当前电台未保存时为0
    uint8 station_count; //电台表中的保存电台数。
}fm_play_status_t;


/* FM 驱动命令枚举类型*/
typedef enum
{
    /*FM模组初始化*/
    FM_INIT = 0,
    /*FM模组进入standby*/
    FM_STANDBY,
    /*设置频点开始播放*/
    FM_SETFREQ,         
    /*获取当前状态信息*/
    FM_GETSTATUS,
    /*静音与解除静音*/
    FM_MUTE_VOL,
    /*开启软件搜台过程*/
    FM_SEARCH,
    /*开启硬件搜台过程*/
    FM_HARDSEEK,
    /* 设置电台频段*/
    FM_SETBAND,
    /* 设置搜台门限*/
    FM_SETTHROD,
    /* 手动退出硬件seek 过程*/
    FM_BREAKSEEK,
    /* 获取硬件seek 是否完成标识*/
    FM_GET_FLAG,
    /* 获取当前频段信息*/
    FM_GET_BAND,
    /* 获取当前频点信息*/
    FM_GET_FREQ,
    /* 获取当前电台信号强度*/
    FM_GET_INTENTY,
    /* 获取天线状态*/
    FM_GET_ANTEN,
    /* 获取电台立体声状态*/
    FM_GET_STEREO,
} fm_cmd_e;

/* FM 驱动向上层提供的统一入口定义*/
typedef void* (*fm_op_func)(void*, void*, void*);

/* 驱动内部实现的具体接口函数声明*/
typedef struct
{
    fm_op_func  Init;
    fm_op_func  Standby;
    fm_op_func  SetFreq;
    fm_op_func  GetStatus;
    fm_op_func  Mute;
    fm_op_func  Search;
    fm_op_func  HardSeek;
    fm_op_func  SetBand;    
    fm_op_func  SetThrod;   
    fm_op_func  SeekBreak;
    fm_op_func  GetSeekOver;
    fm_op_func  GetBand;
    fm_op_func  GetFreq;
    fm_op_func  GetIntsity;
    fm_op_func  GetAntenna;
    fm_op_func  GetStereo;  
} fm_driver_operations;

extern void* fm_op_entry(void *param1, void *param2, void *param3, fm_cmd_e cmd)__FAR__;

#define fm_open(a,b,c)            (int)fm_op_entry((void*)(uint32)(a), (void*)(uint32)(b), (void*)(uint32)(c), FM_INIT)
#define fm_close()              (int)fm_op_entry((void*)(0), (void*)(0), (void*)(0), FM_STANDBY)
#define fm_set_freq(a)          (int)fm_op_entry((void*)(uint32)(a), (void*)(0), (void*)(0), FM_SETFREQ)
#define fm_get_status(a,b)      (int)fm_op_entry((void*)(a), (void*)(uint32)(b), (void*)(0), FM_GETSTATUS)
#define fm_mute(a)              (int)fm_op_entry((void*)(uint32)(a), (void*)(0), (void*)(0), FM_MUTE_VOL)
#define fm_soft_seek(a,b)         (int)fm_op_entry((void*)(uint32)(a),  (void*)(uint32)(b), (void*)(0), FM_SEARCH)
#define fm_hard_seek(a,b)       (int)fm_op_entry((void*)(uint32)(a), (void*)(uint32)(b), (void*)(0), FM_HARDSEEK)
#define fm_set_band(a)          (int)fm_op_entry((void*)(uint32)(a), (void*)(0), (void*)(0), FM_SETBAND)
#define fm_set_throd(a)         (int)fm_op_entry((void*)(uint32)(a), (void*)(0), (void*)(0), FM_SETTHROD)
#define fm_set_breakseek()      (int)fm_op_entry((void*)(0), (void*)(0), (void*)(0), FM_BREAKSEEK)
#define fm_get_seekflag(a)      (int)fm_op_entry((void*)(a), (void*)(0), (void*)(0), FM_GET_FLAG)
#define fm_get_band(a)          (int)fm_op_entry((void*)(a), (void*)(0), (void*)(0), FM_GET_BAND)
#define fm_get_freq(a)          (int)fm_op_entry((void*)(a), (void*)(0), (void*)(0), FM_GET_FREQ)
#define fm_get_intensity(a)     (int)fm_op_entry((void*)(a), (void*)(0), (void*)(0), FM_GET_INTENTY)
#define fm_get_antena(a)        (int)fm_op_entry((void*)(a), (void*)(0), (void*)(0), FM_GET_ANTEN)
#define fm_get_stereo(a)        (int)fm_op_entry((void*)(a), (void*)(0), (void*)(0), FM_GET_STEREO)



#endif

