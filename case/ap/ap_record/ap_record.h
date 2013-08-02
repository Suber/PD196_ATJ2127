/********************************************************************************
 *                            Module: music_ui
 *                 Copyright(c) 2003-2011 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *      leiming    2011-10-14          1.0             build this file
 ********************************************************************************/

#ifndef _AP_RECORD_H_
#define _AP_RECORD_H_

#include "psp_includes.h"
#include "case_include.h"       //display.h包含在common_ui.h中
#include "enhanced.h"
#include "Mmm_mr.h"

#include "setting_common.h"
#include "record_res.h"
#include "record_sty.h"

//record debug macro

/*!定义视频应用的任务栈和主线程优先级*/
#define     AP_RECORD_STK_POS     AP_FRONT_LOW_STK_POS
#define     AP_RECORD_PRIO        AP_FRONT_LOW_PRIO

//#define     FILE_NUM_MAX          999       //录音文件最大个数
#define     FILE_NUM_MAX          200       //录音文件最大个数
#define     REC_SPACE_LOW         200        //磁盘空间满最小扇区
//#define     SETDISKOKFLAG_DEF     0x01

#define     REC_SCENE_TIMER_INTERVAL  125   //录音timer定时时长，ms
#define     WAV_WRITE_BACK_TIMES      300   //录音掉电重写的wav定时写vram，定时器计数值
#define     WAV_FILE_SIZE_MAX       (0x400000-1)	//wav文件size限制到2G，sectors
#define     MP3_FILE_SIZE_MAX	    (0x400000-1)	//mp3文件size限制到2G，sectors
#define     MAGIC_RECORD            0x00   //录音全局变量魔术数

#define     VM_AP_RECORD_WAV	    VM_AP_RECORD+0x200  //断电恢复的数据记录地址

#define     NR_LEVEL_SHIFT 0            //降噪级别为1-6，越低越厉害，推荐值3
#define     RESERVE_LEVEL_SHIFT 4       //保留参数
#define     AGC_LEVEL_SHIFT 8           //AGC级别为1-16，单位1000，越大声音越响，推荐值8，
#define     STABLE_PERIOD_SHIFT 16      //1-255, 越大越适合稳定噪声环境，反之越小越适合变化大的环境，推荐值100
#define     DENOISELEVEL(a,b,c,d)	\
            (((a) << NR_LEVEL_SHIFT) | ((b) << RESERVE_LEVEL_SHIFT) \
            | ((c) << AGC_LEVEL_SHIFT) | ((d) << STABLE_PERIOD_SHIFT))

/**********record界面显示元素标识****************/
#define up_recordPauseIcon      0x00000400    //清除录音pause icon
#define up_recordProgress       0x00000200    //录音进度条显示
#define up_recordTime           0x00000020    //录音now time
#define up_recordFMFreq         0x00000010    //FM频点显示txt
#define up_recordFileTxt        0x00000008    //文件名显示txt
#define up_recordPlayIcon       0x00000004    //录音状态显示图标
#define up_recordHeadBar        0x00000002    //显示录音标题栏
#define up_recordBackGDPic      0x00000001    //背景
//-----------------------------清除刷新标志
#define clr_recordPauseIcon     0xfffffbff    //清除录音pause icon
#define clr_recordProgress      0xfffffdff    //清除录音进度条显示
#define clr_recordTime          0xffffffdf    //清除录音now time
#define clr_recordFMFreq        0xffffffef    //清除FM频点显示txt
#define clr_recordFileTxt       0xfffffff7    //清除文件名显示txt
#define clr_recordPlayIcon      0xfffffffb    //清除录音状态显示图标
#define clr_recordHeadBar       0xfffffffd    //清除录音标题栏
#define clr_recordBackGDPic     0xfffffffe    //清除背景
//********************************************************************************

#define SetFullInterface()             \
{                                      \
    g_scene_param.display_bitmap=g_scene_param.display_bitmap|   \
                    up_recordBackGDPic |   \
                    up_recordHeadBar |   \
                    up_recordPlayIcon |   \
                    up_recordFileTxt |     \
                    up_recordFMFreq |   \
                    up_recordTime| \
                    up_recordProgress; \
}

/*录音场景信息*/
typedef enum
{
    /*录音场景 : 录音界面*/
    SCENE_UI_REC,
    /*录音场景 : 录音菜单*/
    SCENE_MENU_REC,
    /*录音场景 : 退出场景*/
    SCENE_EXIT,
    /*录音场景 : 保留*/
    SCENE_MAX
} record_scene_e;

/*录音状态枚举*/
typedef enum
{
    /*录音状态 : 正在录音状态*/
    REC_PLAY_STATUS,
    /*录音状态 : 暂停状态*/
    REC_PAUSE_STATUS,
    /*录音状态 : 停止状态*/
    REC_STOP_STATUS,
/*录音状态 : 错误状态*/
//REC_ERROR
} record_status_e;

/*录音采样率*/
typedef enum
{
    /*录音采样率：8KHZ*/
    RATE_8KHZ = 0,
    /*录音采样率：16KHZ*/
    RATE_16KHZ = 1,
    /*录音采样率：24KHZ*/
    RATE_24KHZ = 2,
    /*录音采样率：32KHZ*/
    RATE_32KHZ = 3,
/*录音采样率：无效值*/
//RATE_MAX = 4,
} record_sample_rate_e;

/*录音比特率*/
typedef enum
{
    /*录音比特率：8KBPS*/
    BIT_8KBPS = 0,
    /*录音比特率：32KBPS*/
    BIT_32KBPS = 1,
    /*录音比特率：64KBPS*/
    BIT_64KBPS = 2,
    /*录音比特率：128KBPS*/
    BIT_128KBPS = 3,
    /*录音比特率：192KBPS*/
    BIT_192KBPS = 4,
    /*录音比特率：256KBPS*/
    BIT_256KBPS = 5,
    /*录音比特率：256KBPS*/
    BIT_512KBPS = 6,
    /*录音比特率：64KBPS*/
    BIT_768KBPS = 7,
    /*录音比特率：128KBPS*/
    BIT_1024KBPS = 8,
    /*录音比特率：192KBPS*/
    BIT_1536KBPS = 9,
/*录音比特率：无效值*/
//BIT_MAX = 5,
} record_bitrate_e;

/*录音文件格式*/
typedef enum
{
    /*录音格式 ：wav录音*/
    REC_FORMAT_WAV = 0,
    /*录音格式 ：mp3录音*/
    REC_FORMAT_MP3 = 1,
    /*录音格式 ：ACT录音*/
    REC_FORMAT_ACT = 2,
/*录音格式 ：无效录音格式*/
//REC_FORMAT_INVALID = 2,
} record_file_type_e;

/*录音通道*/
typedef enum
{
    /*录音通道 ：单通道*/
    SINGLE_CH = 1,
    /*录音通道 ：双通道*/
    DUAL_CH = 2,
} record_channelnum_e;

/*分曲开关*/
typedef enum
{
    /*普通模式*/
    TRACK_OFF = 0,
    /*自动分曲*/
    TRACK_ON = 1,
} record_track_mode_e;

/*avr开关*/
typedef enum
{
    /*普通模式*/
    AVR_OFF = 0,
    /*自动分曲*/
    AVR_ON = 1,
} record_avr_mode_e;

/*录音codec参数集合*/
typedef struct
{
    /*recorder codec handle*/
    void* mr_handle;
    /*录音audio参数*/
    mmm_mr_audio_param_t mr_audio_param;
    /*录音声音设置参数*/
    mmm_mr_ain_setting_t mr_ain_setting;
    /*录音实时状态参数*/
    mmm_mr_status_t mr_status;
} record_mr_param_t;

/*录音全局变量*/
typedef struct
{
    /*魔方数：判断vm数据是否有效*/
    uint16 maigc;
    /*录音采样率*/
    record_sample_rate_e sample_rate;
    /*录音比特率 ：0/1/2/3/4/5 对应 32/64/128/192/256bps.(通过BitRate_Tab[]转换成module层的接口参数)*/
    record_bitrate_e bitrate;
    record_bitrate_e mp3_bitrate;
    record_bitrate_e wav_bitrate;
    record_bitrate_e act_bitrate;
    /*录音文件格式 : WAV/MP3*/
    record_file_type_e file_type;
    /*录音input增益*/
    uint8 gain_input_linein;
    /*录音adc增益*/
    uint8 gain_adc_linein;
    /*录音input增益*/
    uint8 gain_input_mic;
    /*录音adc增益*/
    uint8 gain_adc_mic;
    /*录音通道*/
    record_channelnum_e channel_num;
    /*录音分曲模式：0,自动分曲；1,关闭分曲*/
    record_track_mode_e track_mode;
    record_avr_mode_e avr_mode;
    /*录音avr门限*/
    uint8 track_level_avr;
    /*录音avr自动分曲门限*/
    uint8 track_level_breaksong;
    /*录音avr延时时间,0~0xff*/
    uint8 track_delay;
    /*录音降噪级别*/ 
    uint32 noise_cancel_level;
    /*当前设置的录音源:0-MIC;1-FM;2-LINEIN;3-NULL*/
    mmm_ai_type_t ai_source;
    /*录音编码格式:0-PCM;1-ADPCM;2-MP3*/
    mmm_mr_encode_mode_t encode_type;
    /*录音文件location*/
    file_path_info_t path;
    /*目前没有使用*/
    uint16 file_num;
    /*一般录音(MIC/LINE-IN)文件编号*/
    uint16 rec_num;
    /*FM录音文件编号*/
    uint16 fmrec_num;
    /*卡上一般录音(MIC/LINE-IN)文件编号*/
    uint16 rec_num_card;
    /*卡上FM录音文件编号*/
    uint16 fmrec_num_card;
} record_vars_t;

/*录音文件相关参数集合*/
typedef struct
{
    /*录音文件mount id*/
    int32 vfs_handle;
    /*剩余磁盘空间,扇区数*/
    uint32 free_space;
    /*fhandle文件句柄*/
    handle fhandle;
    /*录音文件短名，8+3格式*/
    //uint8 rec_filename[12];
    /*录音长文件名*/
    uint8 rec_filename[28];
    /*显示录音文件名，带点去空格格式*/
    uint8 show_filename[13];
    /*录音文件总数*/
    int8 file_num;
    /*flash格式化标识*/
    uint8 disk_flag;
} record_fs_param_t;

/*录音场景参数集合*/
typedef struct
{
    /*录音的场景信息*/
    record_scene_e record_scene;
    /*录音的状态信息*/
    record_status_e record_status;
    /*FM频点*/
    uint8 fm_freq[8];
    /*显示刷新标志*/
    bool paint_flag;
    /*剩余录音的时间*/
    uint32 free_time;
    /*显示刷新bitmap*/
    uint32 display_bitmap;
    /*定时器句柄*/
    int8 timer_id;
    /*用于中断处理函数传出，退出recording场景*/
    app_result_e record_result;
} record_scene_param_t;

/*wav录音掉电回写文件头结构体*/
typedef struct
{
    /*长名*/
    uint8 rec_filename[28];
    /*文件偏移信息*/
    pfile_offset_t ptr_file_offset;
    /*已录文件大小*/
    uint32 file_bytes;
    /*录音比特率*/
    uint32 bitrate;
    /*目录信息,73byte,需要做对齐*/
    pdir_layer_t ptr_layer;
    /*盘符信息*/
    uint8 disk_no;
    /*录音格式，0-wav, 1-mp3*/
    uint8 rec_format;
    /*需要重写标志*/
    bool rewrite_need;
} record_writehead_t;

extern uint32 rec_free_space; //单位，sectors
extern uint32 rec_free_time;
extern uint16 rewrite_times; //掉电重写的定时计数，计满清0

/***********全局变量extern*****************/
/*系统公共变量*/
extern comval_t g_comval;

/*record的全局变量*/
extern record_vars_t g_record_vars;

/*ap 入口模式*/
extern app_param_e g_entry_mode;

/*record codec params*/
extern record_mr_param_t g_mr_param;

/*录音场景参数集合*/
extern record_scene_param_t g_scene_param;

/*录音文件相关参数集合*/
extern record_fs_param_t g_fs_param;

/*wav录音掉电回写文件头结构体*/
extern record_writehead_t g_writehead;

extern bool need_fsel_init;
//add by liuwzh for alarm record
extern uint8 g_record_from_alarm;
extern uint32 alarm_rec_time;
extern uint16 fmrec_frequency; //FM频段
extern uint8 fmrec_volume; //FM音量
extern bool g_record_timeout;
extern uint8 vram_wr_cnt;
//用于录音文件序号的计数
extern uint16 g_rec_num;
//用于fm录音文件序号的计数
extern uint16 g_fmrec_num;
//第一次进录音ADC开启标志
extern uint8 g_adc_flag;
//是否第一次进入录音应用
extern uint8 g_first_enter_flag;
//record文件夹文件总数
extern uint16 g_file_total;
//定时FM定时器
extern int8 g_alarm_timer;


/*录音场景管理*/
extern app_result_e record_scene_recording(void); //record场景处理函数

/*录音菜单场景管理*/
extern app_result_e record_scene_menu(void);

/*record场景的UI显示函数*/
extern void _record_scene_paint(uint32* pdisplay_bitmap);

/*装载磁盘驱动与文件系统*/
extern bool _record_fs_init(uint8 diskno);

/*录音之前的目录检验，磁盘空间检验，录音文件生成*/
extern bool _record_file_setup(void);

/*wav文件录音完毕后回写文件头*/
extern bool _record_write_back_filehead(void);

/*开始录音*/
extern app_result_e _recording_start(void) __FAR__;

/*暂停录音*/
extern app_result_e _recording_pause(void) __FAR__;

/*恢复录音*/
extern app_result_e _recording_resume(void) __FAR__;

/*停止录音*/
extern app_result_e _recording_stop(void) __FAR__;

/*关闭录音*/
extern app_result_e _recording_close(void) __FAR__;

/*录音消息处理*/
extern app_result_e record_message_deal(private_msg_t *msg) __FAR__;
extern app_result_e _scene_record_get_msg(void) __FAR__;
extern void _record_show_time_progress(uint32 *pdisplay_bitmap, uint8 para, uint8 type);

/*录音场景初始化和退出处理*/
extern bool _scene_recording_init(void);
extern bool _scene_recording_exit(void);

extern void _radio_get_freq(void);
extern bool radio_modual_init(void);
extern bool radio_modual_reinstall(void);



/*FM相关处理*/
extern bool radio_alarm_init(void);
extern bool radio_close_engine(void);
#endif
