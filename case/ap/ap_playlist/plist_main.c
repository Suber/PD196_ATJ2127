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

#define MENU_REC_MAX  5

extern uint8 m3u_ext_buf[4];

static fsel_param_t fsel_param _BANK_DATA_ATTR_; //bank data

static void init_sys_data(void);
static void init_plist_data(void);
static bool music_build_plist(void);
static bool audible_build_plist(void);
static bool others_build_plist(void);
typedef bool (*object_build_f)(void);

static const object_build_f object_build_plist[PL_LIB_MAX] =
{ music_build_plist, audible_build_plist, others_build_plist, others_build_plist, others_build_plist,
        others_build_plist 
};

static const uint8 stg_drv_type[] =
{ DISK_C, DISK_H, DISK_U };

static const char plist_ui_sty[] = "playlist.sty";

#ifdef PC

//simulator 环境下请把M3U后缀改成LRC，调试M3U列表创建流程
static const uint32 file_type_bitmap[PL_LIB_MAX] =
{ MUSIC_BITMAP, AUDIBLE_BITMAP, VIDEO_BITMAP, PICTURE_BITMAP, TEXT_BITMAP,EXT_NAME_LRC};//

#else

static const uint32 file_type_bitmap[PL_LIB_MAX] =
{ MUSIC_BITMAP, AUDIBLE_BITMAP, VIDEO_BITMAP, PICTURE_BITMAP, TEXT_BITMAP,(uint32)m3u_ext_buf };//

#endif

static const uint8 stg_creat_fail[] =
{ S_PLIST_C_CREAT_FAIL, S_PLIST_H_CREAT_FAIL, S_PLIST_U_CREAT_FAIL };

extern bool creat_plist_lib(uint8 type) __FAR__;
extern bool close_plist_lib(uint8 type) __FAR__;

extern uint16 scan_music_file(void) __FAR__;
extern uint16 scan_audible_file(void) __FAR__;
extern uint16 scan_others_info(void) __FAR__;

extern void sort_music_list(void) __FAR__;
extern void sort_audible_list(void) __FAR__;
extern void sort_others_list(void) __FAR__;

extern bool plist_fsel_set_typebit(uint32 file_type_bit, bool reset_dir_flag) __FAR__;

extern void plist_timer_proc(void) __FAR__;
extern void plist_reflash_progress(uint8 prog_value) __FAR__;
extern void _app_display_init(void) __FAR__;

extern bool check_bookmark_exist(void) __FAR__;
extern void creat_ebook_bmk(void) __FAR__;
extern bool creat_userpl_table(void) __FAR__;

extern int8 delete_lib_file(uint8 lib_bitmap,uint8 tmp_drv_sn) __FAR__;

extern void plist_sys_init(void) __FAR__;
extern void plist_sys_exit(app_param_e enter_mode) __FAR__;

extern bool check_m3u_exist(void) __FAR__;
extern void plist_reflash_string(uint8 str_id) __FAR__;

/***************************/
void _read_var(void)
{
    setting_comval_t *g_setting_comval_p;
    /*读取common VM公共变量信息*/
    g_setting_comval_p = (setting_comval_t*) temp_plist_buf;
    sys_vm_read(g_setting_comval_p, VM_AP_SETTING, sizeof(setting_comval_t));
    libc_memcpy(&g_comval, &g_setting_comval_p->g_comval, sizeof(comval_t));
    com_set_sys_comval(&g_comval);
    language_id = g_setting_comval_p->g_comval.language_id;

}

/******************************************************************************/
/*
 * \par  Description: AP入口
 *
 * \param[in]    argc--输入参数个数
 argv--参数类型指针
 * \param[out]

 * \return       >0 成功，<=0 失败

 * \note
 *******************************************************************************/

int main(int argc, const char *argv[])
{
    uint8 disk_param;
    uint8 build_param;
    int8 app_timer_id; //软定时器id
    uint16 plist_param;
    uint16 lib_file_total[MENU_REC_MAX];

#ifndef PC
    uint32 save_freq_level;//, prev_nandpllctl;
#endif

    bool creat_file_flag;

    app_param_e enter_mode = (app_param_e) (argc & 0x1f);

#ifndef PC
    save_freq_level = sys_adjust_clk(FREQ_108M, 0);/*第一次调频*/
    sys_mdelay(1);
    //    prev_nandpllctl = act_readl(NANDCLKCTL);
    //    act_writel(prev_nandpllctl | 0x01, NANDCLKCTL);
    //    sys_mdelay(1);
#endif

    plist_sys_init();

    _read_var();
    //读出MAIN_MENU的VM区
    sys_vm_read(lib_file_total, VM_FILE_NUM, sizeof(lib_file_total));

    com_clear_all_history(PATH_HISTORY_DIR);

    //    plist_param=(PLIST_DISK_C+PLIST_DISK_H)<<8;//NAND+CARD

    //argc 的bit7~bit5为创建播放列表的介质
    plist_param = (uint16) ((uint8) argc & PLIST_DISK_ALL) << 8;

#ifdef PC
    //simulator 只支持card 和 nand
    plist_param = plist_param&0xC000;
#endif

    //默认创建所用的表项
    plist_param = plist_param + (PL_BUILD_ALL & 0x1f);// don't creat m3u
    libc_memset(lib_file_total, 0x0000, sizeof(lib_file_total));

    /*
     //确定调用者AP
     switch (enter_mode)
     {
     // 从 config ap 进入
     case PARAM_FROM_CONFIG:
     case PARAM_FROM_UDISK:
     plist_param += PL_BUILD_ALL;
     libc_memset(lib_file_total, 0x0000, sizeof(lib_file_total));
     creat_file_flag = TRUE;
     break;

     // 从 music ap 进入
     case PARAM_FROM_MUSIC:
     plist_param += (PL_BUILD_MUSIC | PL_BUILD_AUDIBLE);
     lib_file_total[0] = 0x0000;
     creat_file_flag = TRUE;
     break;

     // 从 audible ap 进入
     case PARAM_FROM_AUDIBLE:
     plist_param += PL_BUILD_AUDIBLE;
     lib_file_total[1] = 0x0000;
     break;

     // 从 video ap 进入
     case PARAM_FROM_VIDEO:
     plist_param += PL_BUILD_VIDEO;
     lib_file_total[2] = 0x0000;
     break;

     // 从 picture ap 进入
     case PARAM_FROM_PICTURE:
     plist_param += (PL_BUILD_PICTURE | PL_BUILD_PIC2);
     lib_file_total[3] = 0x0000;
     break;

     // 从 ebook ap 进入
     case PARAM_FROM_EBOOK:
     plist_param += PL_BUILD_EBOOK;
     lib_file_total[4] = 0x0000;
     break;

     default:
     plist_param = 0;
     break;
     }
     */

    disk_param = (uint8) ((plist_param >> 8) & PLIST_DISK_ALL);
    build_param = (uint8) (plist_param & PL_BUILD_ALL);

    if((disk_param & PLIST_DISK_H) != 0)
    {
        if (sys_detect_disk(DRV_GROUP_STG_CARD) == -1)
        {
            disk_param = disk_param & (~PLIST_DISK_H);
        }
    }

/*    
    暂不支持Uhost
    if((disk_param & PLIST_DISK_U) != 0)
    {
        if (sys_detect_disk(DRV_GROUP_STG_UHOST) == -1)
        {
            disk_param = disk_param & (~PLIST_DISK_U);
        }
    }
*/    
    if ((disk_param == 0) || (build_param == 0))
    {
        //参数无效
        goto ap_exit;
    }

    if ((enter_mode == PARAM_FROM_CONFIG) || (enter_mode == PARAM_FROM_UDISK))
    {
        creat_file_flag = TRUE;        
    }
    else if (0 != (disk_param & (PLIST_DISK_H | PLIST_DISK_U)))
    {
        creat_file_flag = TRUE;
    }
    else
    {
        creat_file_flag = FALSE;
    }
    
    if(creat_file_flag == TRUE)
    {
        plist_param = plist_param | PL_BUILD_M3U;//creat m3u
        build_param = build_param | PL_BUILD_M3U;//creat m3u
    }

    //设置UI不使用picture buffer
    ui_set_display_buf(LCD_RAM_INDEX);
    //初始化applib库，前台AP
    applib_init(APP_ID_PLAYLIST, APP_TYPE_GUI);

    //初始化 applib 消息模块
    applib_message_init(NULL);

    //初始化软定时器
    init_app_timers(playlist_app_timer_vector, APP_TIMER_TOTAL);
    ui_res_open(plist_ui_sty, UI_AP);

    //定时器350ms
    app_timer_id = set_app_timer(APP_TIMER_ATTRB_UI, 350, plist_timer_proc);

    _app_display_init();

    cpu_release_count = 0;
    drv_sn = 0;

    while (disk_param != 0)
    {
        if ((disk_param & 0x80) == 0)
        {
            goto next_disk;
        }

        //BANK DATA,必须每次都要初始化
        fsel_param.brow_type = 0;
        fsel_param.fsel_mode = FSEL_MODE_NORMAL;
        fsel_param.fsel_type = FSEL_TYPE_COMMONDIR;
        fsel_param.file_type_bit = file_type_bitmap[drv_sn];
        fsel_param.disk = stg_drv_type[drv_sn];

        ap_vfs_mount = fsel_init(&fsel_param, MODE_NORMAL);
        lib_num = 0;

        if (ap_vfs_mount == -1)
        {
            goto next_disk;
        }

        //读VRAM中的LIB的OFFSET
        //        sys_vm_read(plist_lib_offset, VM_LIB_PL_OFFSET +
        //(drv_sn * sizeof(plist_lib_offset)), sizeof(plist_lib_offset));

        build_param = (uint8) (plist_param & PL_BUILD_ALL);

        //            //先一次性删除所有即将要创建的lib文件，优化文件系统的速度
        //            if (0 != delete_lib_file(build_param, drv_sn))
        //            {
        //                //空间不够，不建表
        //                goto stg_exit;
        //            }


        while (build_param != 0)
        {
            if ((build_param & 0x01) != 0)
            {
                plist_get_app_msg();

#ifdef ENHANCED_FIND_MODE
                fsel_set_typebit(file_type_bitmap[lib_num],TRUE);//默认进入root
#else
                plist_fsel_set_typebit(file_type_bitmap[lib_num], TRUE);
#endif
                //fsel_param.file_type_bit=file_type_bitmap[lib_num];
                init_sys_data();

                plist_reflash_progress(-1);

                if (FALSE != creat_plist_lib(lib_num))
                {
                    init_plist_data();
                    ap_plist_file_offset = ap_pos_file_offset;
                    plist_get_app_msg();
                    object_build_plist[lib_num]();
                    //                        plist_reflash_progress(-1);
                    if (FALSE == close_plist_lib(lib_num))
                    {
                        goto build_err;
                    }

                    //nand建表记录总个数
                    if ((drv_sn == 0) && (lib_num < MENU_REC_MAX))
                    {
                        lib_file_total[lib_num] = ap_plist_file_total;
                    }

                    plist_reflash_progress(-1);

                }
                else
                {
                    build_err: plist_reflash_string(stg_creat_fail[drv_sn & 0x03]);
                    sys_os_time_dly(35);//显示界面使用
                    goto stg_exit;
                }

            }
            lib_num++;
            build_param = build_param >> 1;
        }

        //创建收藏夹文件
        if (creat_file_flag == TRUE)
        {
            //创建电子书
            creat_ebook_bmk();
            creat_userpl_table();
            plist_get_app_msg();
            check_bookmark_exist();

            //解析3uu.lib，生成dat;
            check_m3u_exist();
        }

        if (drv_sn == 1)
        {
            //card build succeed,set flag
            change_card_plist(TRUE);
        }

        stg_exit:

        //回写LIB的OFFSET
        //        sys_vm_read(temp_plist_buf, VM_LIB_PL_OFFSET, SECTOR_SIZE);
        //        libc_memcpy(temp_plist_buf + (drv_sn * sizeof(plist_lib_offset)),
        //        plist_lib_offset, sizeof(plist_lib_offset));
        //        sys_vm_write(temp_plist_buf, VM_LIB_PL_OFFSET);
        fsel_exit();

        next_disk: drv_sn++;
        disk_param = disk_param << 1;
    }

    plist_reflash_progress(10);

    kill_app_timer(app_timer_id);
    //关闭ui资源文件
    ui_res_close(UI_AP);

    ui_set_display_buf(JPEG_RAM_INDEX);

    //执行applib库的注销操作
    applib_quit();

    ap_exit:

    //更新总个数表,回写到MAIN_MENU的VM区
    if (((uint8) argc & PLIST_DISK_C) != 0)
    {
        //有nand创建播放列表才回写总数
        sys_vm_read(temp_plist_buf, VM_FILE_NUM, SECTOR_SIZE);
        libc_memcpy(temp_plist_buf, lib_file_total, sizeof(lib_file_total));
        sys_vm_write(temp_plist_buf, VM_FILE_NUM);
    }

    plist_sys_exit(enter_mode);

#ifndef PC
    //    act_writel(prev_nandpllctl, NANDCLKCTL);
    //    sys_mdelay(1);
    sys_adjust_clk(save_freq_level, 0);
#endif

    return 1;
}

/******************************************************************************/
/*
 * \par  Description: 初始化系统内部数据
 *
 * \param[in]    NULL

 * \param[out]   NULL

 * \return       NULL

 * \note
 *******************************************************************************/
static void init_sys_data(void)
{

    title_vram_pt = (uint8*) TITLE_VRAM_ADDR;
    album_vram_pt = (uint8*) ALBUM_VRAM_ADDR;
    artist_vram_pt = (uint8*) ARTIST_VRAM_ADDR;
    genre_vram_pt = (uint8*) GENRE_VRAM_ADDR;
    //    audible_vram_pt=(uint8*)AUDIBLE_VRAM_ADDR;
    track_vram_pt = (uint8*) TRACK_VRAM_ADDR;

    title_data_pt = (uint8*) TITLE_DATA_ADDR;
    album_data_pt = (uint8*) ALBUM_DATA_ADDR;
    artist_data_pt = (uint8*) ARTIST_DATA_ADDR;
    genre_data_pt = (uint8*) GENRE_DATA_ADDR;
    //    audible_data_pt=(uint8*)AUDIBLE_DATA_ADDR;
    fileinfo_data_pt = (uint8*) FILE_INFO_ADDR;
    track_data_pt = (uint8*) TRACK_DATA_ADDR;

    tidy_buf_a = (uint16*) TIDY_BUF_A_ADDR;
    tidy_buf_b = (uint16*) TIDY_BUF_B_ADDR;

    file_index_buf = (uint16*) FILE_INDEX_BUF_ADDR;
    sort_index_buf = (uint16*) SORT_INDEX_BUF_ADDR;

    //    index_parent_buf = (uint16*) INDEX_PARENT_BUF_ADDR;
    //    index_parent_buf = (uint16*) COUNT_BUF_A_ADDR; //

    count_buf_a = (uint16*) COUNT_BUF_A_ADDR;
    count_buf_b = (uint16*) COUNT_BUF_B_ADDR;
    count_buf_c = (uint16*) COUNT_BUF_C_ADDR;
    return;

}
/******************************************************************************/
/*
 * \par  Description: 初始化创建每个列表使用到的数据
 *
 * \param[in]    NULL

 * \param[out]   NULL

 * \return       NULL

 * \note
 *******************************************************************************/
static void init_plist_data(void)
{
    libc_memset(ap_file_index_offset, 0x00, sizeof(ap_file_index_offset));
    libc_memset(ap_sort_index_offset, 0x00, sizeof(ap_sort_index_offset));
    libc_memset(ap_index_parent_offset, 0x00, sizeof(ap_index_parent_offset));
    libc_memset(ap_son_tree_num, 0x00, sizeof(ap_son_tree_num));
    libc_memset(ap_son_tree_offset, 0x00, sizeof(ap_son_tree_offset));

    //    music_num=0;
    //    audible_num=0;
    //    music_info_offset=0;
    //    audible_info_offset=0;

    ap_plist_file_total = 0;

    fsel_scan_flag = 0;

    //    utf8_flag=0;

    return;
}
/******************************************************************************/
/*
 * \par  Description: 创建MUSIC播放列表
 *
 * \param[in]

 * \param[out]

 * \return        TRUE OR FALSE

 * \note
 *******************************************************************************/
static bool music_build_plist(void)
{
    if (scan_music_file() > 0)
    {
        sort_music_list();
    }

    return TRUE;
}
/******************************************************************************/
/*
 * \par  Description: 创建AUDIBLE播放列表
 *
 * \param[in]

 * \param[out]

 * \return        TRUE OR FALSE

 * \note
 *******************************************************************************/
static bool audible_build_plist(void)
{
    if (scan_audible_file() > 0)
    {
        sort_audible_list();
    }

    return TRUE;
}
/******************************************************************************/
/*
 * \par  Description: 创建 视频 or 图片 or 电子的播放列表
 *
 * \param[in]

 * \param[out]

 * \return        TRUE OR FALSE

 * \note
 *******************************************************************************/
//video,picture,ebook
static bool others_build_plist(void)
{
    if (scan_others_info() > 0)
    {
        sort_others_list();
    }

    return TRUE;
}

