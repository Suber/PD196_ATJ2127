/*******************************************************************************
 *                              US212A
 *                            Module: radio ui
 *                 Copyright(c) 2003-2012 Actions Semiconductor
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *     mikeyang   2011-09-23          1.0              create this file
 *******************************************************************************/

#include "app_radio.h"
#include "app_radio_menu_config.h"

extern bool parse_userlist_uni(char* station_name, userlist_parse_e mode, uint8 num);
extern bool parse_userlist_mul(char* station_name, userlist_parse_e mode, uint8 num);

//用户电台列表文件名( 只在flash  或卡盘的固定位置查找)
extern const char userlist[12];

/* 存储介质驱动名*/
static const char card_driver_name[] = "card.drv";
static const char flash_driver_name[] = "nand.drv";

/* 文件读取临时buffer */
uint8 tmp_buf[SECTOR_SIZE] _BANK_DATA_ATTR_;

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e radioUI_scene_optionlist(radio_opt_subscene_e mode)
 * \进入option  子菜单界面
 * \param[in]    mode   子菜单入口模式
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \note  包括播放场景的option  和预设列表的option
 */
/*******************************************************************************/
app_result_e radioUI_scene_optionlist(radio_opt_subscene_e mode)
{
    app_result_e result = 0;
    /*初始化菜单控件*/
    menu_com_data_t op_menu;

    op_menu.app_id = APP_ID_RADIO;
    op_menu.browse_mode = 0;
    op_menu.menu_func = NULL;

    /*定义不同模式下的入口菜单*/
    if (mode == SUBS_OPT_STATION)
    {
        op_menu.menu_entry = OPTION_MENU_STATION;
        op_menu.path_id = 1;
    }
    else
    {
        op_menu.menu_entry = OPTION_PLAYWIN;
        op_menu.path_id = 2;
    }

    /* 控件显示菜单*/
    result = gui_menulist(MENULIST_OPTION, &op_menu);

    //按return 键返回
    if (result == RESULT_REDRAW)
    {
        //回到预设列表
        if (mode == SUBS_OPT_STATION)
        {
            result = RESULT_RADIO_STATION_LIST;
            g_radio_listtype = STATIONLIST_SAVE;
            /* 正常浏览进入*/
            g_stalist_entry = ENTER_NORMAL;
        }
        //回到电台播放界面
        else
        {
            //从播放的option 菜单回播放界面, 不重设频率
            need_set_freq = FALSE;
            result = RESULT_RADIO_START_PLAY;
        }
    }
    return result;
}

//以下是用户电台所用部分代码

/******************************************************************************/
/*
 * \par  Description: mbyte to wchar
 *
 * \param[in]    dest，src

 * \param[out]

 * \return  UNICODE 个数，不包含标识符和结束符

 * \note    ASC转成UNICODE，自动加入标识符和结束符
 *******************************************************************************/
static uint16 mbyte_to_wchar(uint8 *dest, uint8 *src)
{
    uint16 num = 0;

#ifdef USE_83NAME_CREAT

    while(*src!=0x00)
    {
        *dest=*src;
        dest++;
        src++;
        num++;
    }

    dest=0x00;

#else

    *dest = 0xff;
    *(dest + 1) = 0xfe;
    dest += 2;

    while (*src != 0x00)
    {
        *dest = *src;
        *(dest + 1) = 0x00;
        dest += 2;
        src++;
        num++;
    }

    *dest = 0x00;
    *(dest + 1) = 0x00;

#endif

    return num;

}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool check_UserStation_path(void)
 * \在指定目录查找用户电台列表文件
 * \param[in]    void  para1
 * \param[out]   none
 * \return       int the result
 * \retval           1 sucess
 * \retval           0 failed
 * \note
 */
/*******************************************************************************/
bool check_UserStation_path(void)
{
    char* drv_name;
    bool result = TRUE;
    uint8 disk;

    //如果没有安装flash 驱动，判断是否有卡驱动
    if ((sys_get_drv_install_info(DRV_GROUP_STG_BASE) & 0xf) == 0)
    {
        drv_type = DRV_GROUP_STG_CARD;
        if ((sys_get_drv_install_info(drv_type) & 0xf) == 0)
        {
            //如果flash 和卡驱动都未安装，默认取flash 上的文件解析
            drv_type = DRV_GROUP_STG_BASE;
        }
    }
    else
    {
        //如果有安装nand 驱动，比如在播放nand 上的歌曲，进来浏览用户电台
        drv_type = DRV_GROUP_STG_BASE;
    }

    if (drv_type == DRV_GROUP_STG_BASE)
    {
        drv_name = (char *) flash_driver_name;
        disk = DISK_C;
    }
    else
    {
        drv_name = (char *) card_driver_name;
        disk = DISK_H;
    }

    //安装存储驱动
    if (sys_drv_install(drv_type, 0, drv_name) != 0)
    {
        return FALSE;
    }

    //挂载FS
    vfs_mount_radio = (uint32) sys_mount_fs(drv_type, disk, 0);
    //if(0>vfs_mount_radio ||2<vfs_mount_radio)
    if (vfs_mount_radio == -1)
    {
        sys_drv_uninstall(drv_type);
        return FALSE;
    }

    //默认进入根目录，查找解析文件
    vfs_cd(vfs_mount_radio, CD_ROOT, 0);
    mbyte_to_wchar(tmp_buf, (uint8 *) userlist);
    //fp_radio = vfs_file_open(vfs_mount_radio, tmp_buf, FS_OPEN_NORMAL);
    fp_radio = vfs_file_open(vfs_mount_radio, tmp_buf, R_NORMAL_SEEK);

    //打开文件成功
    if (0 != fp_radio)
    {
        vfs_file_read(vfs_mount_radio, tmp_buf, SECTOR_SIZE, fp_radio);
        /* 确定用户电台列表文件编码方式*/
        if ((tmp_buf[0] == 0xff) && (tmp_buf[1] == 0xfe))
        {
            encode_mode = ENCODE_UNICODE;
        }
        else
        {
            encode_mode = ENCODE_MUL;
        }

        //初始化变量为-1	，buffer 数据不能直接使用
        //cursec_num = 0xffff;
        //cur_offset=0;

        //获取文件长度
        result = vfs_file_get_size(vfs_mount_radio, &file_total_byte, fp_radio, 0);
        //获取长度失败，关闭文件，卸载驱动
        if (!result)
        {
            vfs_file_close(vfs_mount_radio, fp_radio);

            if (vfs_mount_radio != -1)
            {
                //卸载文件系统驱动
                sys_unmount_fs(vfs_mount_radio);
                vfs_mount_radio = -1;
            }

            //卸载存储驱动，卸载和装载必须匹配
            sys_drv_uninstall(drv_type);
        }
        return result;
    }

    //打开文件失败，卸载驱动
    if (vfs_mount_radio != -1)
    {
        //卸载文件系统驱动
        sys_unmount_fs(vfs_mount_radio);
        vfs_mount_radio = -1;
    }

    //卸载存储驱动，卸载和装载必须匹配
    sys_drv_uninstall(drv_type);
    return FALSE;
}

/**********************************************************************
 * Description: 用于对用户自定义电台列表文件进行解析
 * Input: mode-- 分析模式
 *          num -- 需解析的电台索引号
 * Output:  station_name --- 用于保存所获取的电台名称
 * Return:  bool   成功/ 失败
 * Note:   用户电台列表当前暂支持内码编码和unicode 编码
 ***********************************************************************
 **/
bool radio_parse_userlist(char* station_name, userlist_parse_e mode, uint8 num)
{
    bool ret;
    ret = check_UserStation_path();

    if (!ret)
    {
        return FALSE;
    }

    if (encode_mode == ENCODE_UNICODE)
    {
        ret = parse_userlist_uni(station_name, mode, num);
    }
    else
    {
        ret = parse_userlist_mul(station_name, mode, num);
    }

    //电台解析完成，关闭文件
    vfs_file_close(vfs_mount_radio, fp_radio);

    if (vfs_mount_radio != -1)
    {
        //卸载文件系统驱动
        sys_unmount_fs(vfs_mount_radio);
        vfs_mount_radio = -1;
    }

    //用户电台解析完成，卸载存储驱动，卸载和装载必须匹配
    sys_drv_uninstall(drv_type);
    return ret;
}

