/*******************************************************************************
 *                              US212A
 *                            Module: PLAYLSIT
 *                 Copyright(c) 2003-2011 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       liminxian  2011-9-15 15:37    1.0             build this file
 *******************************************************************************/
/*!
 * \file     *.c
 * \brief    这里填写文件的概述
 * \author   liminxian
 * \par      GENERAL DESCRIPTION:
 *               这里对文件进行描述
 * \par      EXTERNALIZED FUNCTIONS:
 *               这里描述调用到外面的模块
 * \version 1.0
 * \date  2011/9/15
 *******************************************************************************/

#include "plist.h"
#include "rdata_ex.h"

//#define DIRLAYER_MAX 8

static const uint8 rec_dir_name[] = "RECORD  ";

static uint8 plist_dir_layer _BANK_DATA_ATTR_;
static uint16 plist_file_no _BANK_DATA_ATTR_;
static uint32 plist_cur_ext_name _BANK_DATA_ATTR_;
static uint32 plist_sel_bitmap _BANK_DATA_ATTR_;

static bool filtrate_dir(void);
//static bool dir_next_layer(uint8 *strfile);
static bool dir_next(uint8* strfile);
bool plist_fsel_init(uint32 file_type_bit);

//#define PLIST_DEBUG_FSEL

#ifdef PLIST_DEBUG_FSEL
static uint8 dir_name_buffer[64];
static uint8 file_name_buffer[64];
#endif
/******************************************************************************/
/*
 * \par  Description:初始化文件选择器
 *
 * \param[in]    file_type_bit--指定文件的扩展名bitmap

 * \param[out]

 * \return       TRUE OR FALSE

 * \note
 *******************************************************************************/

bool plist_fsel_init(uint32 file_type_bit)
{
    plist_dir_layer = 0;
    plist_file_no = 0;
    plist_cur_ext_name = 0;
    plist_sel_bitmap = file_type_bit;
    //默认进入根目录
    vfs_cd(ap_vfs_mount, CD_ROOT, 0);
    return TRUE;

}

/******************************************************************************/
/*
 * \par  Description:重设选择的文件类型
 *
 * \param[in]    file_type_bit--文件扩展名bitmap
 reset_dir_flag 重设目录标,若ture重新初始化到根目录

 * \param[out]   none

 * \return       TRUE or FALSE

 * \note
 *******************************************************************************/

bool plist_fsel_set_typebit(uint32 file_type_bit, bool reset_dir_flag)
{
    plist_sel_bitmap = file_type_bit;
    if (reset_dir_flag == TRUE)
    {
        plist_fsel_init(file_type_bit);
    }
    return TRUE;
}

/******************************************************************************/
/*
 * \par  Description: 获取磁盘的下一个文件
 *
 * \param[in]

 * \param[out]    strfile--存放文件的扩展名

 * \return        TRUE OR FALSE

 * \note
 *******************************************************************************/
//bool  plist_fsel_get_nextfile (char *strfile )
//{
//    return dir_next_layer(strfile);
//}

/******************************************************************************/
/*
 * \par  Description: 获取当前目录下的下个文件
 *
 * \param[in]

 * \param[out]      strfile--存放文件的扩展名

 * \return          TRUE OR FALSE

 * \note
 *******************************************************************************/
static bool dir_next(uint8* strfile)
{
    uint8 dir_type;

    if (plist_file_no == 0)
    {
        dir_type = DIR_HEAD;
    }
    else
    {
        dir_type = DIR_NEXT;
    }

    plist_cur_ext_name = vfs_dir(ap_vfs_mount, dir_type, NULL, plist_sel_bitmap);
    if (0 != plist_cur_ext_name)
    {
        plist_file_no++;
        *(uint32*) strfile = plist_cur_ext_name;

#ifdef PLIST_DEBUG_FSEL
        vfs_get_name(ap_vfs_mount, file_name_buffer,30);
#endif

        return TRUE;
    }
    return FALSE;
}

/******************************************************************************/
/*
 * \par  Description: 获取磁盘的下一个文件
 *
 * \param[in]

 * \param[out]        strfile--存放文件的扩展名

 * \return            TRUE OR FALSE

 * \note
 *******************************************************************************/
//static bool dir_next_layer(uint8 *strfile)
bool plist_fsel_get_nextfile(uint8 *strfile)
{
    //在当前目录向下找文件

    if (FALSE == dir_next(strfile))
    {
        CurrentDiragain:

        if (plist_dir_layer >= (DIRLAYER_MAX - 1))//如果当前是8级目录就不找子文件夹
        {
            goto ParentDiragainNext;
        }

        //寻找该目录的第一个子文件夹
        if ((0 != vfs_dir(ap_vfs_mount, DIR_HEAD, NULL, EXT_NAME_ALL_DIR)) && (FALSE == filtrate_dir()))
        {

#ifdef PLIST_DEBUG_FSEL
            vfs_get_name(ap_vfs_mount, dir_name_buffer,30);
#endif

            if (FALSE != vfs_cd(ap_vfs_mount, CD_SUB, NULL))
            {
                //进入子文件夹，获得该子文件夹的文件总数
                plist_dir_layer++;
                plist_file_no = 0;
                // 检查当前目录是否有合适的文件
                if (FALSE != dir_next(strfile))
                {
                    return TRUE;
                }
                else //检查子目录
                {
                    //if(plist_dir_layer>DIRLAYER_MAX-1)
                    //    goto ParentDiragainNext;
                    //else
                    goto CurrentDiragain;
                }
            }
            else //出错了
            {
                return FALSE;
            }
        }
        else //如果没有子文件夹，则向上跳一层，寻找当前目录的下一个目录（平级寻找）
        {
            ParentDiragainNext:

            if (0 == plist_dir_layer)
            {
                return FALSE;
            }

            if (FALSE != vfs_cd(ap_vfs_mount, CD_BACK, NULL))
            {
                plist_dir_layer--;
                if ((0 != vfs_dir(ap_vfs_mount, DIR_NEXT, NULL, EXT_NAME_ALL_DIR)) && (FALSE == filtrate_dir()))
                {
#ifdef PLIST_DEBUG_FSEL
                    vfs_get_name(ap_vfs_mount, dir_name_buffer,30);
#endif

                    vfs_cd(ap_vfs_mount, CD_SUB, NULL);
                    plist_dir_layer++;
                    plist_file_no = 0;

                    if (FALSE != dir_next(strfile))
                    {
                        return TRUE;
                    }
                    else
                    {
                        // 检查子目录.
                        //if(plist_dir_layer>=DIRLAYER_MAX)
                        //    goto ParentDiragainNext;
                        //else
                        goto CurrentDiragain;
                    }
                }
                else
                {
                    goto ParentDiragainNext;
                }
            }

            else
            { //异常出错
                return FALSE;
            }
        }
    }

    return TRUE;
}

/******************************************************************************/
/*
 * \par  Description:过滤根目录下的RECORD目录
 *
 * \param[in]

 * \param[out]

 * \return       TRUE OR FALSE

 * \note
 *******************************************************************************/
static bool filtrate_dir(void)
{
    uint8 dir_name_buffer[32];
    uint16 name_len;
    uint8 add_len, i, j;
    uint8 *buf;

    if ((plist_dir_layer != 0) || ((fsel_scan_flag & SCAN_FILTRATE_BIT) != 0))
    {
        return FALSE;
    }

    name_len = vfs_get_name(ap_vfs_mount, dir_name_buffer, 16);

    if ((name_len == 11) && (dir_name_buffer[0] != 0xff))
    {
        //8+3
        buf = dir_name_buffer;
        name_len = 8;
        add_len = 1;
    }
    else
    {
        if ((name_len == 8) && (dir_name_buffer[0] == 0xff))
        {
            //unciode
            buf = dir_name_buffer + 2;
            name_len = 6;
            add_len = 2;
        }
        else
        {
            return FALSE;
        }
    }

    i = 0;
    for (j = 0; j < name_len; j++)
    {
        if (rec_dir_name[j] != buf[i])
        {
            return FALSE;
        }
        i = i + add_len;
    }

    fsel_scan_flag = fsel_scan_flag | SCAN_FILTRATE_BIT;
    
    //过滤成功，找下一个文件夹返回
    if(0!=vfs_dir(ap_vfs_mount, DIR_NEXT, NULL, EXT_NAME_ALL_DIR))
    {
    //找到，跑找到文件夹流程	    
        return FALSE;
    }
    else
    {
    //未找到，跑递归流程	    
        return TRUE;
    }

}

