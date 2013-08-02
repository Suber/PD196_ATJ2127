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

//static  uint16 total_dir_cur(void);

extern pdir_layer_t pdir_layer_buffer;
extern pfile_offset_t pfile_offset_buffer;

extern uint16 total_file_cur(uint8* dir_info, uint8 check_flag);
/******************************************************************************/
/*
 * \par  Description:
 *
 * \param[in]

 * \param[out]

 * \return

 * \note
 *******************************************************************************/
bool dir_enter_dir(uint8 mode, char *dir_name)
{
    bool ret_dir;

    if (fsel_module_type == FSEL_MODULE_DIR)
    {
        ret_dir = vfs_cd(vfs_mount, mode, dir_name);
        if (FALSE != ret_dir)
        {
            if ((mode == CD_BACK) || (mode == CD_UP))
            {
                dir_layer--;
            }
            else if (mode == CD_ROOT)
            {
                dir_layer = 0;
            }
            else
            {
                dir_layer++;
            }

            file_total = total_file_cur(NULL, 0);
            file_no = 0;
            if ((file_total > 0) && (TRUE == shuffle_sw))
            {
                init_shuffle(file_total);
            }
            return TRUE;
        }
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
/*
 static uint16 total_dir_cur(void)
 {
 uint16 total = 0;
 uint8 dir_type = DIR_HEAD;

 //save current path
 vfs_file_dir_offset(vfs_mount, &pdir_layer_buffer, &pfile_offset_buffer,0);

 do
 {
 if(0!= vfs_dir(vfs_mount,dir_type,NULL,EH_GET_EXT_DIR))
 {
 total++;
 }
 else
 {
 break;
 }
 dir_type = DIR_NEXT;
 } while (1);

 //back up path
 vfs_file_dir_offset(vfs_mount, &pdir_layer_buffer, &pfile_offset_buffer,1);

 return total;

 }

 */
/******************************************************************************/
/*
 * \par  Description:
 *
 * \param[in]

 * \param[out]

 * \return

 * \note
 *******************************************************************************/
bool dir_del_one(uint16 del_num)
{
    bool ret_val = FALSE;
    //    uint16 t_sort_index;
    //    uint16 t_index_parent;

    ret_val = vfs_file_dir_remove(vfs_mount, NULL, FS_FILE);

    // 选择文件已删除，初始化参数
    file_no = 0;
    file_total = 0;

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
bool dir_next_dir(uint8 *strdir)
{
    //    uint8 subDirNum;
    uint8 dir_type = DIR_NEXT;

    if (dir_no == 0)
    {
        dir_type = DIR_HEAD;
    }

    //若要切换目录，请在接口前准备好
    cur_ext_name = vfs_dir(vfs_mount, dir_type, NULL, EH_GET_EXT_DIR);
    if (cur_ext_name == 0)
    {
        return FALSE;
    }

    dir_no++;
    if (strdir != NULL)
    {
        *(uint32*) strdir = cur_ext_name;
    }

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
bool dir_prev_dir(uint8 *strdir)
{
    uint8 dir_type = DIR_PREV;

    if (dir_no == 0)
    {
        dir_type = DIR_TAIL;
    }

    cur_ext_name = vfs_dir(vfs_mount, dir_type, NULL, EH_GET_EXT_DIR);

    if (cur_ext_name == 0)
    {
        return FALSE;
    }

    dir_no++;

    if (strdir != NULL)
    {
        *(uint32*) strdir = cur_ext_name;
    }

    return TRUE;
}

/******************************************************************************/
/*
 * \par  Description:
 *
 * \param[in]

 * \param[out]

 * \return

 * \note  会改变file_no的值;取文件时调用前不能改变前一首的file_no
          因为该接口要根据file_no走不同的流程
 *******************************************************************************/
bool dir_get_byno(uint8 *strfile, uint16 num)
{
    uint8 dir_type;
    uint16 dir_num;

    if ((num > file_total) || (num == 0))
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
    if (file_no < num)/*大于当前文件号，向后Dir*/
    {
        dir_num = num - file_no;
        file_no = 0;
        while (file_no < dir_num)
        {
            cur_ext_name = vfs_dir(vfs_mount, dir_type, NULL, fsel_init_val.file_type_bit);
            if (0 != cur_ext_name)
            {
                file_no++;
            }
            else
            {
                return FALSE;
            }
            dir_type = DIR_NEXT;
        }
    }
    else if (file_no > num) /*小于当前文件号，向前Dir*/
    {
        dir_type = DIR_PREV;
        dir_num = file_no - num;
        file_no = 0;
        while (file_no < dir_num)
        {
            cur_ext_name = vfs_dir(vfs_mount, dir_type, NULL, fsel_init_val.file_type_bit);
            if (0 != cur_ext_name)
            {
                file_no++;
            }
            else
            {
                return FALSE;
            }
        }
    }

    else /*和当前的文件号相等*/
    {
        if (file_no == 1) /*第一个文件*/
        {
            dir_type = DIR_HEAD;
            file_no = 0;
            cur_ext_name = vfs_dir(vfs_mount, dir_type, NULL, fsel_init_val.file_type_bit);
            if (0 == cur_ext_name)
            {
                return FALSE;
            }
        }
        //        else
        //        {
        //
        //        }
    }

    file_no = num;
    if (strfile != NULL)
    {
        *(uint32*) strfile = cur_ext_name;
    }

    return TRUE;
}

