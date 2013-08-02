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

static bool dir_next(uint8* strfile);
static bool dir_next_layer(uint8 *strfile);
uint16 total_file_cur(uint8* dir_info, uint8 check_flag);

extern pdir_layer_t pdir_layer_buffer;
extern pfile_offset_t pfile_offset_buffer;
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
bool dir_get_next(uint8 *strfile)
{
    bool ret_val;
    //    uint16 temp_num;

    switch (fsel_init_val.fsel_mode)
    {
        //重复当前一个文件
        case FSEL_MODE_LOOPONE:
        {
            if (0 == file_total)
            {
                return FALSE;
            }
            else if (0 == file_no)
            {
                //默认取第一首
                ret_val = dir_get_byno(strfile, 1);
            }
            else
            {//之前已取,直接返回ext
                if (strfile != NULL)
                {
                    *(uint32*) strfile = cur_ext_name;
                }
                ret_val = TRUE;
            }
            break;
        }

        //当前目录下的下一首，到目录尾就结束
        case FSEL_MODE_DIRNOR:
        case FSEL_MODE_INTRO:
        {
            ret_val = dir_next(strfile);
            break;
        }

        //所有文件顺序读取，不重复
        case FSEL_MODE_NORMAL:
        {
            ret_val = dir_next_layer(strfile);
            break;
        }

        //所有文件顺序读取，重复
        case FSEL_MODE_LOOPALL:
        {
            if (FALSE != dir_next_layer(strfile))
            {
                ret_val = TRUE;
            }
            else
            {
                init_fsel_sysdata();
                ret_val = dir_next_layer(strfile);
            }
            break;
        }
        //当前目录重复播放
        case FSEL_MODE_LOOPONEDIR:
        {
            if (FALSE != dir_next(strfile))
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

                ret_val = dir_next(strfile);
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
static bool dir_next(uint8* strfile)
{
    uint8 dir_type;
    uint16 temp_num;

    if (file_total == 0)
    {
        return FALSE;
    }
    if (TRUE == shuffle_sw)
    {
        temp_num = shuffle_next_file();
        if (temp_num == 0)
        {
            fsel_error = FSEL_ERR_OUTOF;
            return FALSE;
        }
        return dir_get_byno(strfile, temp_num);
    }

    if (file_no == file_total)//表示当前已是最后一个，结束
    {
        fsel_error = FSEL_ERR_OUTOF;
        return FALSE;
    }

    if (file_no == 0)
    {
        dir_type = DIR_HEAD;
    }
    else
    {
        dir_type = DIR_NEXT;
    }

    cur_ext_name = vfs_dir(vfs_mount, dir_type, NULL, fsel_init_val.file_type_bit);
    if (0 != cur_ext_name)
    {
        file_no++;
        if (strfile != NULL)
        {
            *(uint32*) strfile = cur_ext_name;
        }
        return TRUE;
    }
    else
    {
        fsel_error = FSEL_ERR_FAIL;
    }
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
static bool dir_next_layer(uint8 *strfile)
{
    //在当前目录向下找文件
    //    uint16 temp_num;

    if (0 == file_total)
    {
        file_total = total_file_cur(NULL, 0);
        file_no = 0;
    }

    if (FALSE == dir_next(strfile))
    {
        CurrentDiragain:

        if (dir_layer >= (DIRLAYER_MAX - 1))
        {
            goto ParentDiragainNext;
        }

        //寻找该目录的第一个子文件夹
        if (0 != vfs_dir(vfs_mount, DIR_HEAD, NULL, EH_GET_EXT_DIR))
        {
            if (FALSE != vfs_cd(vfs_mount, CD_SUB, NULL))
            {
                //进入子文件夹，获得该子文件夹的文件总数
                file_total = total_file_cur(NULL, 0);
                dir_layer++;
                file_no = 0;
                // 检查当前目录是否有合适的文件
                if ((file_total > 0) && (FALSE != dir_next(strfile)))
                {
                    fsel_error = 0;
                    return TRUE;
                }
                else //检查子目录
                {
                    //if(dir_layer>DIRLAYER_MAX-1)
                    //    goto ParentDiragainNext;
                    //else
                    goto CurrentDiragain;
                }
            }
            else //出错了
            {
                init_fsel_sysdata();
                fsel_error = FSEL_ERR_OUTOF;
                return FALSE;
            }
        }
        else //如果没有子文件夹，则向上跳一层，寻找当前目录的下一个目录（平级寻找）
        {
            ParentDiragainNext:

            if (0 == dir_layer)
            {
                init_fsel_sysdata();
                fsel_error = FSEL_ERR_OUTOF;
                return FALSE;
            }

            if (FALSE != vfs_cd(vfs_mount, CD_BACK, NULL))
            {
                dir_layer--;
                if (0 != vfs_dir(vfs_mount, DIR_NEXT, NULL, EH_GET_EXT_DIR))
                {
                    vfs_cd(vfs_mount, CD_SUB, NULL);
                    dir_layer++;

                    file_total = total_file_cur(NULL, 0);
                    file_no = 0;

                    if ((file_total > 0) && (FALSE != dir_next(strfile)))
                    {
                        fsel_error = 0;
                        return TRUE;
                    }
                    else
                    {
                        // 检查子目录.
                        //if(dir_layer>=DIRLAYER_MAX)
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
                init_fsel_sysdata();
                fsel_error = FSEL_ERR_EXCEPTION;
                return FALSE;
            }
        }
    }

    return TRUE;
}

/******************************************************************************/
/*
 * \par  Description:统计当前目录下文件的总数
 *
 * \param[in]

 * \param[out]

 * \return

 * \note
 *******************************************************************************/
uint16 total_file_cur(uint8* dir_info, uint8 check_flag)
{
    uint8 temp_offset_buf[8];
    uint8 dir_type = DIR_HEAD;
    uint16 total = 0;

    //save current path
    vfs_file_dir_offset(vfs_mount, &pdir_layer_buffer, &pfile_offset_buffer, 0);

    do
    {
        if (0 != vfs_dir(vfs_mount, dir_type, NULL, fsel_init_val.file_type_bit))
        {
            total++;
            if (check_flag == 1)
            {
                //比较8字节offset
                vfs_file_dir_offset(vfs_mount, NULL, &temp_offset_buf, 0);
                if (libc_memcmp(dir_info, &temp_offset_buf, 8) == 0)
                {
                    file_no = total;
                    check_flag = 0;
                }
            }
        }
        else
        {
            break;
        }
        dir_type = DIR_NEXT;
    } while (1);

    //back up path
    vfs_file_dir_offset(vfs_mount, &pdir_layer_buffer, &pfile_offset_buffer, 1);

    if (check_flag == 1)
    {
        file_no = 0;
    }

    if ((TRUE == shuffle_sw) && (total > 0))
    {
        init_shuffle(total);
    }

    return total;
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
static uint16 total_file_all(uint8* dir_info, uint8 check_flag)
{
    uint8 temp_offset_buf[8];
    uint16 total;
    uint32 ext_name;

    //save current path
    vfs_file_dir_offset(vfs_mount, &pdir_layer_buffer, &pfile_offset_buffer, 0);

    init_fsel_sysdata();
    file_total = 0;
    total = 0;
    do
    {
        if (FALSE != dir_next_layer((uint8*) &ext_name))
        {
            total++;
            if (check_flag == 1)
            {
                //比较8字节offset
                vfs_file_dir_offset(vfs_mount, NULL, &temp_offset_buf, 0);
                if (libc_memcmp(dir_info, &temp_offset_buf, 8) == 0)
                {
                    file_no = total;
                    check_flag = 0;
                }
            }
        }
        else
        {
            break;
        }
    } while (1);

    //back up path
    vfs_file_dir_offset(vfs_mount, &pdir_layer_buffer, &pfile_offset_buffer, 1);

    if (check_flag == 1)
    {
        file_no = 0;
    }

    if (TRUE == shuffle_sw)
    {
        init_shuffle(file_total);
    }

    return total;

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
bool dir_get_location(file_location_t *location, fsel_type_e type)
{

    type = type;
    //libc_memcpy(&location->filename,cur_ext_name,4);
    *(uint32*) &location->filename = cur_ext_name;

    if (FALSE == vfs_file_dir_offset(vfs_mount, &location->dir_layer_info, &location->cluster_no, 0))
    {
        return FALSE;
    }

    location->disk = fsel_init_val.disk;
    location->dir_layer = dir_layer;
    location->file_total = file_total;
    location->file_num = file_no;

    return TRUE;
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
bool dir_set_location(file_location_t *location, fsel_type_e type)
{
    bool ret_val;

    type = type;

    if ((location == NULL) || (*(uint32*) &location->filename == 0))
    {
        file_total = total_file_cur(NULL, 0);
        file_no = 0;
        return FALSE;
    }

    ret_val = vfs_file_dir_offset(vfs_mount, &location->dir_layer_info, &location->cluster_no, 1);
    if (FALSE != ret_val)
    {
        vfs_get_name(vfs_mount, &cur_ext_name, 0);
        if (*(uint32*) &location->filename != cur_ext_name)
        {
            ret_val = FALSE;
        }
        else
        {
            //file_no 在total_file_cur里赋值
            file_total = total_file_cur((uint8*) &location->cluster_no, 1);
        }

    }

    if ((FALSE == ret_val) || (file_no == 0))
    {
        //目录空或目录信息有误，回到根目录
        init_fsel_sysdata();
        file_total = total_file_cur(NULL, 0);
        return FALSE;
    }
    dir_layer = location->dir_layer;

    //赋值更新cur_ext_name
    libc_memcpy(&cur_ext_name, location->filename, 4);

    //启动断点播放，重新初始化随机表
    if (TRUE == shuffle_sw)
    {
        init_shuffle(file_total);
        filtrate_shuffle_no = file_no;
    }

    return TRUE;
}

