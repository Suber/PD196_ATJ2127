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
 * \file     eh_bs_directory.c
 * \brief    磁盘文件浏览
 * \author   liminxian
 * \par      GENERAL DESCRIPTION:
 *               磁盘文件浏览接口处理
 * \par      EXTERNALIZED FUNCTIONS:
 *               文件系统模块
 * \version 1.0
 * \date  2011/9/15
 *******************************************************************************/

#include "eh_bs.h"

extern uint8 bs_temp_buf[SECTOR_SIZE];
extern bs_dir_location_t g_bs_dir_loc;

static bool bs_dir_get_total(void);

extern bool bs_dir_function(uint8 dir_type, uint32 object_type);
extern bool bs_dir_set_dirpos(uint32 object_type, uint16 dir_num);
extern void deal_if_83name(uint8 *name_buf);

static const char root_dir_str[] =
{ "ROOT" };

/******************************************************************************/
/*
 * \par  Description: 扫描当前目录下的子目录和文件的总数
 *
 * \param[in]

 * \param[out]

 * \return        TRUE OR FALSE

 * \note
 *******************************************************************************/
//扫描当前目录下的子目录和文件的总数
static bool bs_dir_get_total(void)
{
    uint8 t_dir_type;

    bs_dir_total = 0;
    bs_file_total = 0;

    vfs_file_dir_offset(vfs_mount, bs_temp_buf, bs_temp_buf + 128, 0);

    if ((fsel_init_val.brow_type == FSEL_BROW_ALL) || (fsel_init_val.brow_type == FSEL_BROW_DIR))
    {
        t_dir_type = DIR_HEAD;
        while (FALSE != bs_dir_function(t_dir_type, EH_GET_EXT_DIR))
        {
            bs_dir_total++;
            t_dir_type = DIR_NEXT;
        }
    }

    if ((fsel_init_val.brow_type == FSEL_BROW_ALL) || (fsel_init_val.brow_type == FSEL_BROW_FILE))
    {
        t_dir_type = DIR_HEAD;
        while (FALSE != bs_dir_function(t_dir_type, fsel_init_val.file_type_bit))
        {
            bs_file_total++;
            t_dir_type = DIR_NEXT;
        }
    }

    vfs_file_dir_offset(vfs_mount, bs_temp_buf, bs_temp_buf + 128, 1);

    return TRUE;

}

/******************************************************************************/
/*
 * \par  Description: 进入目录操作
 *
 * \param[in]    type--操作类型
 list_no--列表序号（1~N）
 * \param[out]   dir_brow--目录信息

 * \return       TRUE OR FALSE

 * \note
 *******************************************************************************/
bool bs_dir_enter(brow_dir_e type, uint16 list_no, dir_brow_t *dir_brow)
{
    //    uint8 t_dir_flag = 0; //0-file,1-dir

    switch (type)
    {
        case CUR_DIR:

        bs_dir_get_total();
        //        t_dir_flag = 1;
        break;

        case SON_DIR:

        if ((list_no == 0) || (list_no > bs_dir_total))
        {
            //                bs_dir_set_dirpos(fsel_init_val.file_type_bit,list_no-bs_dir_total);
            return FALSE;
        }
        else
        {
            if (FALSE != bs_dir_set_dirpos(EH_GET_EXT_DIR, list_no))
            {
                if (FALSE == vfs_cd(vfs_mount, CD_SUB, NULL))
                {
                    return FALSE;
                }
                bs_dir_get_total();
                //                t_dir_flag = 1;
                dir_layer++;
            }
            else
            {
                return FALSE;
            }

        }

        break;

        case PARENT_DIR:

        if (FALSE == vfs_cd(vfs_mount, CD_UP, NULL))
        {
            //当前是ROOT
            return FALSE;
        }
        bs_dir_get_total();
        //        t_dir_flag = 1;
        dir_layer--;
        break;

        case ROOT_DIR:
        if (FALSE == vfs_cd(vfs_mount, CD_ROOT, NULL))
        {
            //当前是ROOT
            return FALSE;
        }
        bs_dir_get_total();
        //        t_dir_flag = 1;
        dir_layer = 0;
        break;

        default:
        return FALSE;
        break;

    }

    if (dir_brow != NULL)
    {
        if (FALSE == vfs_cd(vfs_mount, CD_BACK, NULL))
        {
            if (dir_brow->name_len != 0)
            {
                libc_memcpy(dir_brow->name_buf, root_dir_str, sizeof(root_dir_str));
            }
        }
        else
        {
            if (dir_brow->name_len != 0)
            {
                vfs_get_name(vfs_mount, dir_brow->name_buf, dir_brow->name_len / 2);
                if (dir_brow->name_buf[0] != 0xff)
                {
                    //短命转换
                    deal_if_83name(dir_brow->name_buf);
                }
            }
            vfs_cd(vfs_mount, CD_SUB, NULL);

        }

        dir_brow->dir_total = bs_dir_total;
        dir_brow->file_total = bs_file_total;
        dir_brow->layer_no = dir_layer;

    }

    bs_select_listno = 0;

    return TRUE;

}

/******************************************************************************/
/*
 * \par  Description:初始化公共参数
 *
 * \param[in]    none

 * \param[out]   none

 * \return       none

 * \note
 *******************************************************************************/
void bs_dir_initdata(void)
{
    dir_layer = 0;
    root_dir = 0;

    save_read_offset = 0xffff; //
    write_data_flag = FALSE; //

    //默认进入根目录
    vfs_cd(vfs_mount, CD_ROOT, 0);
}

/******************************************************************************/
/*
 * \par  Description: 设置浏览位置信息
 *
 * \param[in]      location--浏览文件location
 type--no used
 * \param[out]

 * \return         TRUE OR FALSE

 * \note
 *******************************************************************************/
bool bs_dir_set_location(file_location_t *location, fsel_type_e type)
{
    bool ret_val = TRUE;

    type = type;

    bs_select_listno = 0;

    if ((location == NULL) || (*(uint32*) &location->filename == 0))
    {
        bs_dir_get_total();
        return FALSE;
    }

    if (0 == libc_memcmp(location->filename, "ROOT", 4))
    {
        //回到根目录
        bs_dir_initdata();
        bs_dir_get_total();
        return TRUE;
    }
    else
    {
        ret_val = vfs_file_dir_offset(vfs_mount, &location->dir_layer_info, &location->cluster_no, 1);
    }

    if (FALSE != ret_val)
    {

        vfs_get_name(vfs_mount, &cur_ext_name, 0);
        if (*(uint32*) &location->filename != cur_ext_name)
        {
            if ((cur_ext_name != 0x202020) || (*(uint32*) &location->filename != EH_DIR_FLAG))
            {
                ret_val = FALSE;
            }
        }
        else
        {
            bs_dir_get_total();
        }
        /*
         if(location->file_total!=bs_file_total ||location->file_num>bs_file_total)
         {
         ret_val=FALSE;
         }
         */
    }

    if (FALSE == ret_val)
    {
        //回到根目录
        bs_dir_initdata();
        bs_dir_get_total();
        return FALSE;
    }

    dir_layer = location->dir_layer;

    if (location->file_num <= bs_file_total)
    {
        bs_select_listno = location->file_num;
    }

    //赋值更新cur_ext_name
    libc_memcpy(&cur_ext_name, location->filename, 4);

    return TRUE;
}

/******************************************************************************/
/*
 * \par  Description: 获取浏览位置信息
 *
 * \param[in]         location--位置信息
 type--no used
 * \param[out]

 * \return            TRUE OR FALSE

 * \note
 *******************************************************************************/
bool bs_dir_get_location(file_location_t *location, fsel_type_e type)
{
    uint8 t_file_num = 0;
    type = type;

    if (bs_select_listno == 0)
    {
        if (dir_layer != 0)
        {
            //非根目录，退出
            return FALSE;
        }
    }

    else if (bs_select_listno > bs_dir_total)
    {
        //选择的是文件
        if (FALSE == bs_dir_set_dirpos(fsel_init_val.file_type_bit, bs_select_listno - bs_dir_total))
        {
            return FALSE;
        }
        t_file_num = (uint8)(bs_select_listno - bs_dir_total);
    }
    else
    {

        if (FALSE == bs_dir_set_dirpos(EH_GET_EXT_DIR, bs_select_listno))
        {
            return FALSE;
        }
        //返回文件夹的位置,作目录路径保存使用
        t_file_num = bs_select_listno;

        //选择的是文件夹,取文件夹下的第一个文件
        /*
         vfs_cd(vfs_mount, CD_SUB, NULL);

         if (FALSE == bs_dir_set_dirpos(fsel_init_val.file_type_bit, 1))
         {
         //退回上级
         vfs_cd(vfs_mount, CD_BACK, NULL);
         fsel_error = FSEL_ERR_EMPTY;
         return FALSE;
         }
         else
         {
         dir_layer++;
         t_file_num = 1;
         }
         */

    }

    if (bs_select_listno != 0)
    {
        libc_memcpy(location->filename, &cur_ext_name, 4);
    }
    else
    {
        libc_memcpy(location->filename, "ROOT", 4);
    }

    if (FALSE == vfs_file_dir_offset(vfs_mount, &location->dir_layer_info, &location->cluster_no, 0))
    {
        fsel_error = FSEL_ERR_FS;
        return FALSE;
    }

    location->disk = fsel_init_val.disk;
    location->dir_layer = dir_layer;
    location->file_total = bs_file_total;//
    location->file_num = t_file_num;

    return TRUE;
}

/******************************************************************************/
/*!
 * \par  Description:
 *    浏览文件夹，以循环播放模式浏览
 * \param[in]    dir_browser：当前文件夹结构体指针，如果返回文件夹，会作为输出参数
 * \param[in]    type：文件夹遍历类型，
 * \param[out]   path_info：当前浏览到的目录节点
 * \return       browser_result_e
 * \retval            见 browser_result_e 定义
 * \par          exmaple code
 * \code
 *               libc_memcpy(&browser_dir_browser, &g_dir_browser, sizeof(dir_brow_t));
 *               browser_result = common_browserdir (path_info, &browser_dir_browser, BROWSERDIR_TYPE_DELETE);//删除文件遍历
 *               if(browser_result == BROWSER_RESULT_FILE)//返回文件
 *               {}
 *               else//返回文件夹
 *               {
 *                   if(browser_result == BROWSER_RESULT_SELF)//文件夹自身
 *                   {}//结束浏览
 *               }
 * \endcode
 * \note         1）已经完成文件系统初始化。
 *               2）已经获得文件夹标题 g_dir_browser。
 *               3）搜索顺序为：先搜索文件夹下的文件，再搜索子文件夹，最后返回自身。
 *               4）在浏览过程中，即使删除了文件，也不要更新结构体，可以继续往下找。
 *               5）搜索文件中，是否可以把非匹配文件也统计起来，这样可以判断文件夹下是否为空。
 *******************************************************************************/
scanfile_result_e fsel_browser_scanfile(file_path_info_t *path_info, dir_brow_t *dir_browser, scanfile_type_e type)
{
    //进入文件浏览循环
    uint16 browser_list_no;

    if ((NULL == path_info) || (NULL == dir_browser))
    {
        //参数有误
        return SCANFILE_RESULT_ERR;
    }

    while (1)
    {
        //1.先搜索文件夹下所有文件
        if ((g_bs_dir_loc.bs_dir_no[g_bs_dir_loc.bs_layer] == 0)//正在浏览文件列表
                && (dir_browser->file_total > 0))//尚未到最后一个文件
        {
            if (type == SCANFILE_TYPE_DELETE)
            {
                //删除文件模式下永远选第一个
                g_bs_dir_loc.list_no = 1;
            }
            fsel_browser_select(dir_browser->dir_total + g_bs_dir_loc.list_no);//LIST_UPDATE_NEXT
            fsel_browser_get_location(&(path_info->file_path), path_info->file_source);
            g_bs_dir_loc.list_no++;
            dir_browser->file_total--;
            return SCANFILE_RESULT_FILE;
        }
        //2.再进入子文件夹进行搜索
        else if ((dir_browser->dir_total > 0)//有子文件夹
                && (dir_browser->dir_total > g_bs_dir_loc.bs_dir_no[g_bs_dir_loc.bs_layer])//尚未到最后一个子文件夹
                && (g_bs_dir_loc.bs_layer < (DIRLAYER_MAX - 1)))//控制深度不超过8层子目录
        {

            if (type == SCANFILE_TYPE_DELETE)
            {
                browser_list_no = 1;
            }
            else
            {
                g_bs_dir_loc.bs_dir_no[g_bs_dir_loc.bs_layer]++;//进入下一个子文件夹
                browser_list_no = g_bs_dir_loc.bs_dir_no[g_bs_dir_loc.bs_layer];
            }
            fsel_browser_enter_dir(SON_DIR, browser_list_no, dir_browser);
            g_bs_dir_loc.bs_layer = dir_browser->layer_no;
            g_bs_dir_loc.list_no = dir_browser->dir_total + 1;
            return SCANFILE_RESULT_SONDIR;
        }
        //3.到达最底层后返回父目录，接着搜索父目录的右兄弟目录
        else
        {
            fsel_browser_enter_dir(PARENT_DIR, 0, dir_browser);
            g_bs_dir_loc.bs_layer = dir_browser->layer_no;
            if (g_bs_dir_loc.bs_layer < g_bs_dir_loc.root_layer)//回到所浏览子文件夹的父目录
            {
                return SCANFILE_RESULT_SELF;
            }
            else
            {
                return SCANFILE_RESULT_PARENTDIR;
            }
        }
    }
}

/******************************************************************************/
/*!
 * \par  Description:
 *    浏览文件夹初始化，即初始化bs 浏览路径管理结构体
 * \param[in]    dir_browser：指向某个子文件夹（layer_no >= 1）
 * \param[out]   none
 * \return       none
 * \note
 *******************************************************************************/
bool fsel_browser_scanfile_init(dir_brow_t *dir_browser)
{
    if (NULL == dir_browser)
    {
        return FALSE;
    }

    libc_memset(g_bs_dir_loc.bs_dir_no, 0, DIRLAYER_MAX);
    g_bs_dir_loc.bs_layer = dir_browser->layer_no;
    g_bs_dir_loc.root_layer = dir_browser->layer_no;
    g_bs_dir_loc.list_no = dir_browser->dir_total + 1;
    return TRUE;
}

