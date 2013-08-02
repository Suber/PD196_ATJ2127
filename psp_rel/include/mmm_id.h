/*! \cond MMM_ID_API*/
/********************************************************************************
 *                              5110
 *                            Module: image_decode
 *                 Copyright(c) 2003-2011 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>       <time>           <version >             <desc>
 * yonglu,bruceding    2011-08-27           1.0             build this file
 ********************************************************************************/
/*!
 * \file     mmm_id.h
 * \brief    定义图片播放中间提供的接口及参数
 * \author   bruceding
 * \version  1.0
 * \date  2011/09/19
 *******************************************************************************/
/*! \endcond*/
#ifndef __MMM_ID_H__
#define __MMM_ID_H__

#ifdef __cplusplus
extern "C" {
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

/*! \cond MMM_ID_API*/
/*!
 *  \brief
 *     提供给应用层的图片解码命令
 */
typedef enum {
	/*! 打开图片中间件 */
	MMM_ID_OPEN = 0,
	/*! 关闭图片中间件 */
	MMM_ID_CLOSE,
	/*! 设置专集图片信息*/
	MMM_ID_SET_ALBUM_INFO,
	/*! 设置播放的文件名 */
	MMM_ID_SET_FILE,
	/*! 获取多媒体文件的信息 */
	MMM_ID_GET_MEDIA_INFO,
	/*! 获取解码信息*/
	MMM_ID_GET_DECODER_STATUS,
	/*! 图片缩略图解码*/
	MMM_ID_BROWSE,
	/*! 图片解码*/
	MMM_ID_DECODE,
	/*! 取消图片解码*/
	MMM_ID_ABORT_DECODE,
} mmm_id_cmd_t;

/*!
 *  \brief
 *      图片引擎状态
 */
typedef enum {
	/*! 出错状态 */
	MMM_ID_ENGINE_ERROR = -1,
	/*! 正常状态 */
	MMM_ID_ENGINE_NORMAL
} mmm_id_engine_t;

/*!
 *  \brief
 *     中间件图片解码状态
 */
typedef enum {
	/*! 解码状态*/
   MMM_ID_BUSY = 10,
	/*! 处于空闲状态*/
	MMM_ID_FREE,
	/*! 处于错误状态*/
	MMM_ID_ERROR
} mmm_id_status_flag_t;

/*!
 * \brief
 *      图片解码情况
 */
typedef struct {
	/*! 解码状态*/
	mmm_id_status_flag_t status;
	/*! 错误号*/
	unsigned int err_no;
	/*! 文件是否达到尾部的标志*/
	unsigned int end_of_file;
} mmm_id_status_t;

/*!
 * \brief
 *      图片其他信息
 */
typedef struct {
	/*! 图片拍摄日期*/
	char datetime[20];
} mmm_id_exif_t;

/*!
 *  \brief
 *  枚举图片类型
 */
typedef enum {
	/*! JPEG 图片*/
	IMAGE_JPG = 0,
	/*! BMP 图片*/
	IMAGE_BMP,
	/*! GIF 图片*/
	IMAGE_GIF,
	/*! 不支持图片*/
	UNKOWN,
} image_type_t;

/*!
 * \brief
 *      图片信息
 */
typedef struct {
	/*! 图片格式*/
	image_type_t file_format;
	/*! 图片宽度*/
	unsigned int width;
	/*! 图片高度*/
	unsigned int height;
	/*! 图片其他信息*/
	mmm_id_exif_t exif;
} mmm_id_file_info_t;

/*!
 * \brief
 *      输入信息结构，
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
 * \brief
 *      输入信息结构，
 */
typedef struct {
	/*! 驱动 */
	unsigned int mountindex;
	/*! 文件名 */
	unsigned char *filename;
	/* 图片在文件中的偏移量 */
	unsigned int data_offset;
	/* 屏幕上的 绘图区*/
	wregion_t *pregion1;
	/* 实际上的 绘图区 */
	wregion_t *pregion2;
	/*! 图片格式*/
	image_type_t file_format;
	/* 播放的标志位 bit0 auto rotation (0 no,1 yes) ，bit1： 0，app为正常解码输出，1，app为缩略图输出*/
	unsigned int playflag;
	/* 返回的标志位  0bit winh2v   (0 no,1 yes)*/
	unsigned int returnflag;
	/* 解码器状态位*/
	mmm_id_status_flag_t dec_status;	      
} ColorLcmInfor;

/*!
 *  \brief
 *     接口函数声明
 *     接口函数定义在kernel_api.S，加载驱动以后使用，驱动API号在api.h，后续会统一整理
 */
extern int mmm_id_cmd(void *id_handle,mmm_id_cmd_t cmd,unsigned int param)__FAR__;

/*! \endcond */
#ifdef __cplusplus
}
#endif

#endif // __MMM_ID_H_
