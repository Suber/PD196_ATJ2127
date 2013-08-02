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
 * \brief    播放列表播放
 * \author   liminxian
 * \par      GENERAL DESCRIPTION:
 *               这里对文件进行描述
 * \par      EXTERNALIZED FUNCTIONS:
 *               FS
 * \version 1.0
 * \date  2011/9/15
 *******************************************************************************/

#include "eh_fsel.h"

extern uint8 fsel_temp_buf[SECTOR_SIZE];
extern pdir_layer_t pdir_layer_buffer;
extern pfile_offset_t pfile_offset_buffer;

static bool plist_next(uint8 *strfile);
static bool plist_prev(uint8 *strfile);
static uint16 plist_get_file_index_byno(uint16 index_start_offset, uint16 start_index, uint16 t_file_no);
static plist_f_info_t * plist_get_file_info(uint16 file_offset);
static bool plist_get_file_byno(uint8 *strfile, uint16 t_file_no);

extern bool fsel_read_sector_data(uint16 sector_offset) __FAR__;

/******************************************************************************/
/*
 * \par  Description:PLAYLIST获取下一个播放文件入口
 *
 * \param[in]

 * \param[out]   strfile--存放ext的buffer指针

 * \return      TRUE OR FALSE

 * \note
 *******************************************************************************/
bool plist_get_next(uint8 *strfile)
{
    //    uint16 temp;
    bool ret_val;

    if (0 == fsel_open_object(R_NORMAL_SEEK))
    {
        return FALSE;
    }

    if (0 == file_total)
    {
        fsel_close_object();
        return FALSE;
    }

    switch (fsel_init_val.fsel_mode)
    {
        //当前一个文件
        case FSEL_MODE_LOOPONE:
        {
            if (0 == file_no)
            {
                file_no = 1;
            }
            ret_val = plist_get_file_byno(strfile, file_no);

            break;
        }

        case FSEL_MODE_DIRNOR:
        case FSEL_MODE_INTRO:
        case FSEL_MODE_NORMAL:
        {
            ret_val = plist_next(strfile);
            break;
        }

        case FSEL_MODE_LOOPALL:
        case FSEL_MODE_LOOPONEDIR:
        {
            if (FALSE != plist_next(strfile))
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
                ret_val = plist_next(strfile);
            }
            break;
        }
        default:
        {
            ret_val = FALSE;
            break;
        }
    }

    fsel_close_object();
    return ret_val;
}

/******************************************************************************/
/*
 * \par  Description:PLAYLIST获取上一个播放文件入口
 *
 * \param[in]

 * \param[out]      strfile--存放ext的buffer指针

 * \return        TRUE OR FALSE

 * \note
 *******************************************************************************/
bool plist_get_prev(uint8 *strfile)
{
    //    uint16 temp;
    bool ret_val;

    if (0 == fsel_open_object(R_NORMAL_SEEK))
    {
        return FALSE;
    }

    if (0 == file_total)
    {
        fsel_close_object();
        return FALSE;
    }

    switch (fsel_init_val.fsel_mode)
    {
        //当前一个文件
        case FSEL_MODE_LOOPONE:
        {
            if (0 == file_no)
            {
                file_no = 1;
            }
            ret_val = plist_get_file_byno(strfile, file_no);

            break;
        }

        case FSEL_MODE_DIRNOR:
        case FSEL_MODE_INTRO:
        case FSEL_MODE_NORMAL:
        {
            ret_val = plist_prev(strfile);
            break;
        }

        case FSEL_MODE_LOOPALL:
        case FSEL_MODE_LOOPONEDIR:
        {
            if (FALSE != plist_prev(strfile))
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
                ret_val = plist_prev(strfile);
            }
            break;
        }

        default:
        {
            ret_val = FALSE;
            break;
        }
    }

    fsel_close_object();
    return ret_val;

}

/******************************************************************************/
/*
 * \par  Description:通过播放序号来选择文件播放
 *
 * \param[in]    t_file_no--播放序号

 * \param[out]   strfile--存放ext的buffer指针

 * \return        TRUE OR FALSE

 * \note 会更新file_no
 *******************************************************************************/
bool plist_get_byno(uint8 *strfile, uint16 t_file_no)
{
    bool ret_val;

    if (0 == fsel_open_object(R_NORMAL_SEEK))
    {
        return FALSE;
    }

    ret_val = plist_get_file_byno(strfile, t_file_no);

    fsel_close_object();

    return TRUE;

}

/******************************************************************************/
/*
 * \par  Description:PLAYLIST顺序获取下一个播放文件
 *
 * \param[in]

 * \param[out]      strfile--存放ext的buffer指针

 * \return        TRUE OR FALSE

 * \note
 *******************************************************************************/
static bool plist_next(uint8 *strfile)
{
    uint16 temp_num;
    bool ret_val;

    if (shuffle_sw == TRUE)
    {
        temp_num = shuffle_next_file();
        if (0 != temp_num)
        {
            ret_val = plist_get_file_byno(strfile, temp_num);
        }
        else
        {
            ret_val = FALSE;
        }
    }
    else
    {
        if (file_no < file_total)
        {
            file_no++;
            ret_val = plist_get_file_byno(strfile, file_no);
        }
        else
        {
            ret_val = FALSE;
        }
    }

    return ret_val;

}

/******************************************************************************/
/*
 * \par  Description: PLAYLIST顺序获取上一个播放文件
 *
 * \param[in]

 * \param[out]      strfile--存放ext的buffer指针

 * \return          TRUE OR FALSE

 * \note
 *******************************************************************************/
static bool plist_prev(uint8 *strfile)
{
    uint16 temp_num;
    bool ret_val;

    if (shuffle_sw == TRUE)
    {
        temp_num = shuffle_prev_file();
        if (0 != temp_num)
        {
            ret_val = plist_get_file_byno(strfile, temp_num);
        }
        else
        {
            ret_val = FALSE;
        }
    }
    else
    {
        if (file_no > 1)
        {
            ret_val = plist_get_file_byno(strfile, file_no - 1);
        }
        else
        {
            if (file_no == 0)
            {
                ret_val = plist_get_file_byno(strfile, file_total);
            }
            else
            {
                ret_val = FALSE;
            }
        }
    }

    return ret_val;
}

/******************************************************************************/
/*
 * \par  Description:通过播放序号来选择文件播放
 *
 * \param[in]    t_file_no--播放序号

 * \param[out]   strfile--存放ext的buffer指针

 * \return        TRUE OR FALSE

 * \note 会更新file_no
 *******************************************************************************/
static bool plist_get_file_byno(uint8 *strfile, uint16 t_file_no)
{
    bool ret_val;
    plist_f_info_t *plist_f_info;

    plist_cur_file_index = plist_get_file_index_byno(plist_item_info.file_index_offset, plist_start_sort_index,
            t_file_no);

    plist_f_info = plist_get_file_info(plist_cur_file_index);

#ifdef _VC_DEBUG
    printf("\n%s \n",plist_f_info->title_string);
#endif

    ret_val = vfs_file_dir_offset(vfs_mount, &plist_f_info->dir_layer_info, &plist_f_info->cluster_no, 1);

    libc_memcpy(&cur_ext_name, plist_f_info->filename, 4);

    if (strfile != NULL)
    {
        libc_memcpy(strfile, plist_f_info->filename, 4);
    }
    file_no = t_file_no;

    return ret_val;

}

/******************************************************************************/
/*
 * \par  Description:根据播放序号找出对应的文件序号
 *
 * \param[in]    index_start_offset--播放file_index存放的位置偏移（扇区）
 start_index --file_index的偏移
 t_file_no--播放序号

 * \param[out]

 * \return        文件序号file_index(0~N)

 * \note
 *******************************************************************************/
//get file_index  by file_no
static uint16 plist_get_file_index_byno(uint16 index_start_offset, uint16 start_index, uint16 t_file_no)
{
    uint16 sector_offset;
    uint16 buf_offset;
    uint16 t_type;
    uint16 temp16;
    uint16 *buf_pt;

    temp16 = 0xffff;

    sector_offset = index_start_offset + start_index / (SECTOR_SIZE / 2);
    buf_offset = (start_index * 2) % SECTOR_SIZE;

    buf_pt = (uint16*) (fsel_temp_buf + buf_offset);
    //    vfs_file_seek(vfs_mount,sector_offset*SECTOR_SIZE,SEEK_SET,obj_handle);
    //    vfs_file_read(vfs_mount,fsel_temp_buf,SECTOR_SIZE, obj_handle);
    fsel_read_sector_data(sector_offset);

    t_type = 0;
    if (m3u_curlist_index != 0xffff)
    {
        if ((plist_layer_offset & 0x80) != 0)
        {
            t_type = 2; //AUDIBLE file
        }
        else
        {
            t_type = 1;// MUSIC file
        }
    }

    while (t_file_no > 0)
    {
        t_file_no--;
        while (1)
        {
            if (buf_pt >= (uint16 *) (fsel_temp_buf + SECTOR_SIZE))
            {
                sector_offset++;
                buf_pt = (uint16*) fsel_temp_buf;
                fsel_read_sector_data(sector_offset);
            }

            if (*buf_pt != PLIST_INVALID_INDEX)
            {
                temp16 = *buf_pt;
                if (t_type == 0)
                {
                    break;
                }
                else if ((t_type == 1) && ((temp16 & 0x8000) == 0))
                {
                    break;
                }
                else if ((t_type == 2) && ((temp16 & 0x8000) != 0))
                {
                    break;
                }
                else
                {
                    ;
                }
            }
            buf_pt++;

        }
        buf_pt++;
    }

    return (temp16 & 0x1fff);//*(buf_pt - 1)&0x1fff;
}

/******************************************************************************/
/*
 * \par  Description:根据file_index获取对应文件的plist_f_info信息。
 *
 * \param[in]    file_offset--文件序号(0~N)

 * \param[out]

 * \return       文件信息存放buffer指针

 * \note
 *******************************************************************************/
static plist_f_info_t * plist_get_file_info(uint16 file_offset)
{
    uint16 buf_offset;
    uint16 sector_offset;

    sector_offset = plist_info_offset + file_offset / (SECTOR_SIZE / PLIST_FILE_SIZE);
    buf_offset = (file_offset * PLIST_FILE_SIZE) % SECTOR_SIZE;

    fsel_read_sector_data(sector_offset);

    return (plist_f_info_t*) (fsel_temp_buf + buf_offset);
}

/******************************************************************************/
/*
 * \par  Description:根据 file_index 获取对应文件的播放序号file_no信息。
 *
 * \param[in]    file_index--文件序号(0~N)

 * \param[out]

 * \return       file_no

 * \note
 *******************************************************************************/
//get file_no  by file_index
uint16 plist_get_fileNo_byIndex(uint16 index_start_offset, uint16 start_index, uint16 t_file_index)
{
    uint16 i;

    for (i = 1; i < file_total + 1; i++)
    {
        if (t_file_index == plist_get_file_index_byno(index_start_offset, start_index, i))
        {
            return i;
        }
    }

    return 0;

}
