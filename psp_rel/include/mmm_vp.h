/*! \cond MMM_VP_API*/
/********************************************************************************
 *                            GL5110
 *                            Module: videoplay.c
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>         <time>           <version >             <desc>
 *   yonglu,bruceding    2011-8-25            1.0             build this file
 ********************************************************************************/
/*!
 * \file     mmm_vp.h
 * \brief    定义音乐播放与应用层的接口及参数
 * \author   yonglu,bruceding
 * \version 1.0
 * \date  2011-8-25
 *******************************************************************************/
/*! \endcond*/
#ifndef __MMM_VP_H__
#define __MMM_VP_H__

#ifdef __cplusplus
extern "C"
{
#endif

/*!
 * \brief
 *      加载系统头文件
 */
#ifdef WIN32
/* VC */
#define __FAR__
#else
/* MIPS */
#include <psp_includes.h>
#endif

/*! \cond MMM_VP_API*/
/*!
 * \brief
 *      提供给应用层的公共播放命令
 */
typedef enum {
	/*! 打开视频中间件 */
	MMM_VP_OPEN = 0,
	/*! 关闭视频中间件 */
	MMM_VP_CLOSE,
	/*! 设置播放的文件名 */
	MMM_VP_SET_FILE,
	/*! 获取多媒体文件的信息 */
	MMM_VP_GET_MEDIA_INFO,
	/*! 显示进度条 */
	MMM_VP_PROGRESSBAR,
	/*! 显示音量条 */
	MMM_VP_VOLUMEBAR,
	/*! 取消进度条 */
	MMM_VP_NOPROGRESS,
	/*! 取消音量条*/
	MMM_VP_NOVOLUME,
	/*! 播放 */
	MMM_VP_PLAY,
	/*! 停止 */
	MMM_VP_STOP,
	/*! 暂停 */
	MMM_VP_PAUSE,
	/*! 快进 */
	MMM_VP_FAST_FORWARD,
	/*! 快退 */
	MMM_VP_FAST_BACKWARD,
	/*! 获取错误信息 */
	MMM_VP_GET_ERRNO,
} mmm_vp_cmd_t;

/*!
 *  \brief
 *      cmd 返回值
 */

typedef enum {
	/*! 无错 */
	MMM_CMD_OK,
	/*! 到文件尾 */
	MMM_CMD_FILEEND,
	/*! 到文件头 */
	MMM_CMD_FILEHEAD,
	/*! 解码出错 */
	MMM_CMD_ERR,
} mmm_vp_cmd_return_t;

/*!
 *  \brief
 *      视频中间件错误号
 */

typedef enum {
	/*! 无错 */
	MMM_VP_NO_ERR,
	/*! 打开文件失败 */
	MMM_VP_ERR_OPEN_FILE,
	/*! 文件格式不支持 */
	MMM_VP_ERR_FILE_NOT_SUPPORT,
	/*! 解码出错 */
	MMM_VP_ERR_DECODER,

} mmm_vp_errno_t;

/*!
 *  \brief
 *  枚举视频类型
 */
typedef enum {
	VIDEO_AMV = 0, VIDEO_AVI
} video_type_t;

/*!
 *  \brief
 *     视频文件的信息
 */
typedef struct {
	/*! 视频类型 */
	video_type_t type;
	/*! 文件的总时间（ms计） */
	unsigned int total_time;
	/*! 视频文件的帧率（fps） */
	unsigned int frame_rate;
	/*! 视频的宽（pixels） */
	unsigned int width;
	/*! 视频的高（pixels） */
	unsigned int height;
	/*! 文件长度 */
	unsigned int file_len;
} mmm_vp_file_info_t;

/*!
 *  \brief
 *     显示区域
 */
typedef struct {
	/*! x坐标 */
	unsigned short x;
	/*! y坐标 */
	unsigned short y;
	/*! 宽 */
	unsigned short w;
	/*! 高 */
	unsigned short h;
} wregion_t;

/*!
 *  \brief
 *     屏以及控制信息
 */
typedef struct {

	/*! 淡出 */
	unsigned int fadeout;
	/*! 进度条 第17bit 0表示覆盖型，1表示缩小型；第16bit 表示位置，0在下，1在上；低16bits表示高度；*/
	unsigned int progressbar;
	/*! 音量条 第17bit 0表示覆盖型，1表示缩小型；第16bit 表示位置，0在下，1在上；低16bits表示高度；*/
	unsigned int volumebar;
	/*! 软音量 */
	unsigned int softvolume;
	/*! 保留 */
	unsigned int reserved;
} ctlinfor_t;

/*!
 *  \brief
 *     视频断点的信息
 */
typedef struct {
	/*! 在文件中的位置 */
	unsigned int bpbyte; // breakpointer  flash counter
	/*! 帧数 */
	unsigned int framecnt;
	/*! 时间（单位秒） */
	unsigned int time;
} videobpinfor_t;//

/*!
 *  \brief
 *     应用层的控制信息
 */
typedef struct {
    /*! 驱动 */
    unsigned int mountindex;	
	/*! 文件名 */
	unsigned char *filename;
	/*! 控制信息 */
	ctlinfor_t *plcminfor;
	/*! 视频断点的信息 */
	videobpinfor_t *pbreakinfor;
	/*! 满屏*/
	wregion_t *pregion1;
	/*! 进度条*/
	wregion_t *pregion2;
	/*有进度条或音量条的显示区域*/
	wregion_t *pregion3;

} mmm_vp_userparam_t;

/*!
 *  \brief
 *     接口函数声明
 *     接口函数定义在kernel_api.S，加载驱动以后使用，驱动API号在api.h，后续会统一整理
 */
extern int mmm_vp_cmd(void *vp_handle, unsigned int cmd, unsigned int param)__FAR__;

#ifdef __cplusplus
}
#endif

#endif

/*! \endcond*/

