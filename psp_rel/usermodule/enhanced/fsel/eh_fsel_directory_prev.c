/*******************************************************************************
 *                              US212A
 *                            Module: ENHANCED
 *                 Copyright(c) 2003-2011 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       liminxian  2011-9-15 15:37    1.0             build this file
 *******************************************************************************/
/*!
 * \file     *.c
 * \brief    磁盘播放定位
 * \author   liminxian
 * \par      GENERAL DESCRIPTION:
 *               这里对文件进行描述
 * \par      EXTERNALIZED FUNCTIONS:
 *               FS
 * \version 1.0
 * \date  2011/9/15
 *******************************************************************************/

#include "eh_fsel.h"

static bool dir_prev(uint8* strfile);
static bool dir_prev_layer(uint8 *strfile);
static bool sel_last_dir(void);

extern uint16 total_file_cur(uint8* dir_info, uint8 check_flag);
extern bool dir_get_byno(uint8 *strfile, uint16 num);

/******************************************************************************/
/*
 * \par  Description:
 *
 * \param[in]

 * \param[out]

 * \return

 * \note
 *******************************************************************************/
bool dir_get_prev(uint8 *strfile)
{
    //    char temp[12];
    bool ret_val;
    //    uint16 temp_num;

    switch (fsel_init_val.fsel_mode)
    {
        case FSEL_MODE_LOOPONE:
        {
            if (0 == file_total)
            {
                ret_val = FALSE;
            }
            else if (0 == file_no)
            {
                ret_val = dir_get_byno(strfile, 1);
            }
            else
            {
                if (strfile != NULL)
                {
                    *(uint32*) strfile = cur_ext_name;
                }
                ret_val = TRUE;
            }
            break;
        }

        case FSEL_MODE_DIRNOR:
        case FSEL_MODE_INTRO:
        {
            ret_val = dir_prev(strfile);
            break;
        }

        case FSEL_MODE_NORMAL:
        {
            if ((0 == file_no) && (0 == dir_layer))
            {
                init_fsel_sysdata();
                while (FALSE != sel_last_dir())
                {
                    vfs_cd(vfs_mount, CD_SUB, NULL);
                    dir_layer++;
                    file_total = 0;
                    file_no = 0;
                }
            }
            ret_val = dir_prev_layer(strfile);
            break;
        }

        case FSEL_MODE_LOOPALL:
        {
            if ((0 == file_no) && (0 == dir_layer))
            {
                goto prev_init_get;
            }

            if (FALSE != dir_prev_layer(strfile))
            {
                ret_val = TRUE;
            }
            else //已是最前的一个，切换到磁盘目录最后一个文件
            {
                prev_init_get: 
                init_fsel_sysdata();
                while (FALSE != sel_last_dir())
                {
                    vfs_cd(vfs_mount, CD_SUB, NULL);
                    dir_layer++;
                    file_total = 0;
                    file_no = 0;
                }
                ret_val = dir_prev_layer(strfile);
            }
            break;
        }

        case FSEL_MODE_LOOPONEDIR:
        {
            if (FALSE != dir_prev(strfile))
            {
                ret_val = TRUE;
            }
            else
            {
                file_no = 0;
                if (TRUE == shuffle_sw)
                {
                    init_shuffle(file_total);
                }
                ret_val = dir_prev(strfile);
            }
            break;
        }

        default:
        {
            ret_val = FALSE;
            break;
        }
    }

    return ret_val;
}

/******************************************************************************/
/*
 * \par  Description:
 *
 * \param[in]

 * \param[out]

 * \return

 * \note
 *******************************************************************************/

static bool dir_prev(uint8* strfile)
{
    uint8 dir_type = DIR_PREV;
    uint16 temp_num;

    if (file_total == 0)
    {
        return FALSE;
    }

    if (TRUE == shuffle_sw)
    {
        temp_num = shuffle_prev_file();
        if (temp_num == 0)
        {
            return FALSE;
        }
        return dir_get_byno(strfile, temp_num);
    }

    if (file_no == 1)//表示前一个已经目录的第一个
    {
        fsel_error = FSEL_ERR_OUTOF;
        return FALSE;
    }

    if (file_no == 0)//表示刚进入该目录
    {
        file_no = file_total;
        //定位到最后一个目录项

        dir_type = DIR_TAIL;

        //        if (file_total == 0)
        //        {
        //            fsel_error = FSEL_ERR_EMPTY;
        //            return FALSE;
        //        }
        //        else
        //        {
        //            dir_type = DIR_TAIL;
        //        }

    }
    else
    {
        file_no--;
    }
    cur_ext_name = vfs_dir(vfs_mount, dir_type, NULL, fsel_init_val.file_type_bit);
    if (cur_ext_name != 0)
    {
        if (strfile != NULL)
        {
            *(uint32*) strfile = cur_ext_name;
        }
        return TRUE;
    }

    fsel_error = FSEL_ERR_FAIL;
    return FALSE;
}

/******************************************************************************/
/*
 * \par  Description:
 *
 * \param[in]

 * \param[out]

 * \return

 * \note
 *******************************************************************************/
static bool dir_prev_layer(uint8 *strfile)
{

    if (file_total == 0)
    {
        file_total = total_file_cur(NULL, 0);
    }

    if (FALSE == dir_prev(strfile))
    {
        //检查父目录
        ParentDiragainNext:

        if (0 == dir_layer)
        {
            return FALSE;
        }

        //        if (0 != vfs_dir(vfs_mount,DIR_PARE_PRVE,strfile,EH_GET_EXT_DIR))
        //        {
        if (FALSE != vfs_cd(vfs_mount, CD_BACK, NULL))
        {
            dir_layer--;
            if (0 != vfs_dir(vfs_mount, DIR_PREV, NULL, EH_GET_EXT_DIR))
            {
                vfs_cd(vfs_mount, CD_SUB, NULL);
                dir_layer++;
                while (FALSE != sel_last_dir())
                {
                    vfs_cd(vfs_mount, CD_SUB, NULL);
                    dir_layer++;
                    file_total = 0;
                    file_no = 0;
                }

                file_total = total_file_cur(NULL, 0);
                if (file_total != 0)
                {
                    file_no = 0;
                    return dir_prev(strfile);
                }
                else
                {
                    goto ParentDiragainNext;
                }
            }
            else
            {
                //当前目录下已找完文件夹，开始找文件
                file_total = total_file_cur(NULL, 0);
                if (file_total != 0)
                {
                    file_no = 0;
                    return dir_prev(strfile);
                }
                goto ParentDiragainNext;
            }
        }
        else
        {
            //出错了
            init_fsel_sysdata();
            fsel_error = FSEL_ERR_OUTOF;
            return FALSE;
        }
    }

    return TRUE;
}

/******************************************************************************/
/*
 * \par  Description:定位到当前目录的最后一个文件夹目录项位置
 *
 * \param[in]

 * \param[out]

 * \return

 * \note
 *******************************************************************************/
static bool sel_last_dir(void)
{

    if (dir_layer >= (DIRLAYER_MAX - 1))//
    {
        return FALSE;
    }

    if (0 == vfs_dir(vfs_mount, DIR_TAIL, NULL, EH_GET_EXT_DIR)) // if no dir ,return error
    {
        return FALSE;
    }

    return TRUE;
}

