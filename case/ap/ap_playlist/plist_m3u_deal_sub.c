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
 * \file     plist_m3u_deal.c
 * \brief    解析m3u格式的文件
 * \author   liminxian
 * \par      GENERAL DESCRIPTION:
 *           解析m3u文件，展开为lib文件的格式；enhanced统一列表方式读取
 * \par      EXTERNALIZED FUNCTIONS:
 *           FS
 * \version 1.0
 * \date  2011/9/15
 *******************************************************************************/
#include "plist.h"
#include "rdata_ex.h"

extern void parse_ext_name(uint8 *buf_ext) __FAR__;
extern uint8 parse_obj_name(uint8* buf_name ,uint8 name_len,uint8 dir_flag) __FAR__;
extern uint32 check_ext_bitmap(uint32 tmp_ext_name) __FAR__;
extern bool compare_name(uint8 *name_obj_buf, uint16 buf_cnt,uint8 name_type) __FAR__;
extern uint8 read_m3u_data(uint32 offset) __FAR__;
extern uint8 set_m3ufile_path(void) __FAR__;

/******************************************************************************/
/*
 * \par  Description: 判断文件是否已经写入dat表中
 *
 * \param[in]

 * \param[out]  file_index

 * \return        TRUE OR FALSE

 * \note         如果已写入，在文件信息中填入映射index
 *******************************************************************************/
uint16 check_filepos_exist(uint8 *dir_info, uint8 cur_m3u_index)
{
    uint8 audible_flag = 0;
    uint16 temp_save, m, flag, t_file_index;
    file_pos_t *t_m3u_pos_pt;
    m3ulist_f_info_t *t_m3u_info_pt;

    if (ap_plist_file_total != 0)
    {
        t_m3u_pos_pt = (file_pos_t*) ALBUM_DATA_ADDR;

        for (t_file_index = 0; t_file_index < ap_plist_file_total; t_file_index++)
        {
            if (0 == libc_memcmp(dir_info, t_m3u_pos_pt, sizeof(file_pos_t)))
            {
                break;
            }

            t_m3u_pos_pt++;
        }
    }
    else
    {
        t_file_index = 0xffff;
    }

    if (t_file_index >= ap_plist_file_total)
    {
        return 0xffff;
    }
    else
    {
        flag = 0;
        temp_save = 0;
        m = ap_plist_file_total % (FILE_INFO_BUF_SIZE / PLIST_FILE_SIZE);
        if (t_file_index < (ap_plist_file_total - m))
        {
            //info is in m3u.dat
            flag = 1;
            temp_save = ap_pos_file_offset;
            vfs_file_seek(ap_vfs_mount, M3U_DAT_FILEOFFSET + (t_file_index >> 1) * SECTOR_SIZE, 
            SEEK_SET, ap_obj_handle);

            vfs_file_read(ap_vfs_mount, temp_plist_buf, SECTOR_SIZE, ap_obj_handle);

            if ((t_file_index & 0x01) != 0)
            {
                t_m3u_info_pt = (m3ulist_f_info_t *) (temp_plist_buf + M3U_DAT_FILESIZE);
            }
            else
            {
                t_m3u_info_pt = (m3ulist_f_info_t *) temp_plist_buf;
            }

        }
        else
        {
            //info is in ram
            if (m == ap_plist_file_total)
            {
                //未写入过文件,all file in ram
                m = t_file_index;
            }
            else
            {
                m = ap_plist_file_total - t_file_index - 1;
            }
            t_m3u_info_pt = (m3ulist_f_info_t *) (FILE_INFO_ADDR + m * M3U_DAT_FILESIZE);
        }

        audible_flag = t_m3u_info_pt->m3u_index[0] & 0x80;

        for (m = 1; m < M3U_LIST_MAX; m++)//第0个肯定存在
        {
            if (t_m3u_info_pt->m3u_index[m] == 0xff)
            {
                t_m3u_info_pt->m3u_index[m] = cur_m3u_index | audible_flag; //置映射audible类型标记
                break;
            }
        }

        if (flag == 1)
        {
            //info is in m3u.dat
            vfs_file_seek(ap_vfs_mount, 0 - SECTOR_SIZE, SEEK_CUR, ap_obj_handle);
            vfs_file_write(ap_vfs_mount, temp_plist_buf, SECTOR_SIZE, ap_obj_handle);
            vfs_file_seek(ap_vfs_mount, temp_save * SECTOR_SIZE, SEEK_SET, ap_obj_handle);
            ap_pos_file_offset = temp_save;
        }

    }

    return t_file_index;

}

/******************************************************************************/
/*
 * \par  Description: 扫描m3u列表中的文件
 *
 * \param[in]

 * \param[out]

 * \return           LOW WORD--文件总数,HIGH WORD --  audible 总数

 * \note
 *******************************************************************************/
uint32 parse_m3u_file(uint8 cur_m3u_index)
{
    uint8 t_dir_layer;
    uint8 dir_type;
    uint8 restart_find;
    uint8 value;
    uint8 name_type;
    uint16 name_cnt;//字符 的个数
    uint16 scanfile_cnt;
    uint16 t_file_index;
    uint16 t_file_total, t_audible_total;
    uint32 byte_cnt;
    uint32 m3u_total_byte;
    uint32 tmp_bitmap;

    restart_find = FALSE;
    t_dir_layer = 0xff;
    name_cnt = 0;
    t_file_total = 0;
    t_audible_total = 0;
    scanfile_cnt = 0;

    vfs_file_get_size(ap_vfs_mount, &m3u_total_byte, m3u_handle, 0);

    for (byte_cnt = 0; byte_cnt < m3u_total_byte; byte_cnt++)
    {
        value = read_m3u_data(byte_cnt);

        if (TRUE == restart_find)
        {
            //当前信息有错或解析完，查找下一条信息
            if (0x0a == value)//换行符
            {
                t_dir_layer = 0xff;
                name_cnt = 0;
                restart_find = FALSE;
            }
            continue;//for(;;;)
        }

        if (value == 0x23)//'#'
        {
            //注释信息，不处理，跳过
            restart_find = TRUE;
            continue;//for(;;;)
        }
        else if (value == 0x5c)//'\'
        {
            //文件夹操作,找到第一个斜杆
            if (t_dir_layer == (DIRLAYER_MAX - 1))
            {
                //目录深度已超，不处理
                restart_find = TRUE;
                continue;//for(;;;)
            }

            if ((name_cnt == 2) && (m3u_obj_name_buf[0] == 0x2e) && (m3u_obj_name_buf[1] == 0x2e))
            {
                // ..\相对路径处理
                if (t_dir_layer == 0xff)//到根目录
                {
                    t_dir_layer = set_m3ufile_path();
                }

                if (t_dir_layer > 0)
                {
                    if (FALSE != vfs_cd(ap_vfs_mount, CD_BACK, NULL))
                    {
                        name_cnt = 0;
                        t_dir_layer--;
                        continue;//for(;;;)
                    }
                }
                restart_find = TRUE;
                continue;//for(;;;)
            }

            if (t_dir_layer == 0xff)//到根目录
            {
                //盘符标志，cd到根目录
                vfs_cd(ap_vfs_mount, CD_ROOT, 0);
                if ((name_cnt == 0) || (m3u_obj_name_buf[name_cnt - 1] == ':'))
                {
                    //支持a:\xxx\1.mp3, 和\xxx\1.mp3格式
                    name_cnt = 0;
                    t_dir_layer = 0;
                }
                else
                {
                    //支持xxx\1.mp3格式
                    goto match_find_dir;
                }

                //NOTE:xxx文件夹必须在根目录
            }
            else
            {
                match_find_dir:
                //查找磁盘上相同名字的目录
                dir_type = DIR_HEAD;
                m3u_obj_name_buf[name_cnt] = 0x00;
                name_type = parse_obj_name(m3u_obj_name_buf, (uint8) name_cnt, 1);

                while (1)
                {
                    if (0 != vfs_dir(ap_vfs_mount, dir_type, NULL, EXT_NAME_ALL_DIR))
                    {
                        if (TRUE == compare_name(m3u_obj_name_buf, name_cnt, name_type))
                        {
                            if (t_dir_layer == 0xff)
                            {
                                t_dir_layer = 0;//goto match_find_dir;
                            }

                            vfs_cd(ap_vfs_mount, CD_SUB, NULL);
                            t_dir_layer++;
                            name_cnt = 0;
                            break;//while(1)
                        }
                        dir_type = DIR_NEXT;
                    }
                    else
                    {
                        //相对路径尝试,M3U文件路径,再dir
                        if (t_dir_layer == 0xff)
                        {
                            //goto match_find_dir;
                            t_dir_layer = set_m3ufile_path();
                            dir_type = DIR_HEAD;
                        }
                        else
                        {
                            restart_find = TRUE;
                            break;//while(1)
                        }

                        //                        restart_find = TRUE;
                        //                        break;//while(1)
                    }
                }
            }
        }
        else if (value == 0x0d)//回车
        {
            scanfile_cnt++;

            if (0 == (scanfile_cnt & 0x0f))
            {
                //定时刷新图标
                plist_get_app_msg();
            }

            //已到文件级
            if (name_cnt > 3)//. + 两位扩展名
            {
                if (t_dir_layer == 0xff)
                {
                    //可能没有斜杠和文件夹，直接文件
                    vfs_cd(ap_vfs_mount, CD_ROOT, 0);
                }

                m3u_obj_name_buf[name_cnt] = 0x00;//添加结束符
                //读取ext
                m3u_ext_name[0] = read_m3u_data(byte_cnt - 4);
                m3u_ext_name[1] = read_m3u_data(byte_cnt - 3);
                m3u_ext_name[2] = read_m3u_data(byte_cnt - 2);
                m3u_ext_name[3] = read_m3u_data(byte_cnt - 1);
                m3u_ext_name[4] = 0x00;
                m3u_ext_name[5] = 0x00;
                //恢复read指针
                read_m3u_data(byte_cnt);
            }
            else
            {
                //文件名长度无效
                restart_find = TRUE;
                continue;
            }
            //处理，获取后缀名
            parse_ext_name(m3u_ext_name);
            tmp_bitmap = check_ext_bitmap(*(uint32*) m3u_ext_name);

            if ((tmp_bitmap & (MUSIC_BITMAP | AUDIBLE_BITMAP)) == 0)
            {
                //除MUSIC 和 audible格式外，其他格式不支持
                restart_find = TRUE;
                continue;
            }

            //先存文件名，因为parse_obj_name可能会改变m3u_obj_name_buf的内容
            libc_memcpy(m3u_dat_file_pt->title_string, m3u_obj_name_buf, name_cnt * 2);

            //处理文件名，转成8+3 和 UNICODE
            name_type = parse_obj_name(m3u_obj_name_buf, (uint8) name_cnt, 0);

            dir_type = DIR_HEAD;
            while (1)
            {
                if (0 != vfs_dir(ap_vfs_mount, dir_type, NULL, tmp_bitmap))//MDY FOR DEBUG
                {
                    if (TRUE == compare_name(m3u_obj_name_buf, 11, name_type))//减一是为了去点
                    {
                        //已确定文件
                        if ((ID3_ITEM_SIZE - 1) < name_cnt)
                        {
                            name_cnt = ID3_ITEM_SIZE - 1;
                        }
                        vfs_file_dir_offset(ap_vfs_mount, &m3u_dat_file_pt->dir_layer_info,
                                &m3u_dat_file_pt->cluster_no, 0);
                        t_file_index = check_filepos_exist((uint8*) &m3u_dat_file_pt->cluster_no, cur_m3u_index);
                        if (t_file_index == 0xffff)
                        {
                            // file is not exist,add new info

                            if (ap_plist_file_total >= M3U_DAT_FILE_MAX)
                            {
                                // file num is max , lose this file
                                restart_find = TRUE;
                                break;
                            }

                            libc_memcpy(filepos_p, &m3u_dat_file_pt->cluster_no, sizeof(file_pos_t));
                            filepos_p++;

                            libc_memcpy(m3u_dat_file_pt->filename, m3u_ext_name, 4);
                            libc_memset(m3u_dat_file_pt->m3u_index, 0xff, M3U_LIST_MAX);

                            if ((tmp_bitmap & AUDIBLE_BITMAP) != 0)
                            {
                                //audible 文件bit7 置1
                                m3u_dat_file_pt->m3u_index[0] = cur_m3u_index | 0x80;
                            }
                            else
                            {
                                m3u_dat_file_pt->m3u_index[0] = cur_m3u_index;
                            }

                            m3u_dat_file_pt->title_string[name_cnt * 2] = 0x00;
                            m3u_dat_file_pt->title_string[name_cnt * 2 + 1] = 0x00;
                            m3u_dat_file_pt->track_num = 0xffff;
                            m3u_dat_file_pt->prev_offset = ap_plist_file_total - 1;
                            m3u_dat_file_pt->next_offset = ap_plist_file_total + 1;
                            m3u_dat_file_pt->flag = 1;
                            m3u_dat_file_pt->id3_tag_flag = 1;
                            t_file_index = ap_plist_file_total;
                            ap_plist_file_total++;

                            if ((ap_plist_file_total % (FILE_INFO_BUF_SIZE / PLIST_FILE_SIZE)) == 0)
                            {
                                save_to_file((uint8 *) FILE_INFO_ADDR, FILE_INFO_BUF_SIZE);
                                m3u_dat_file_pt = (m3ulist_f_info_t*) (FILE_INFO_ADDR);
                                libc_memset(FILE_INFO_ADDR, 0x00, FILE_INFO_BUF_SIZE);
                            }
                            else
                            {
                                m3u_dat_file_pt = (m3ulist_f_info_t*) ((uint8*) m3u_dat_file_pt + M3U_DAT_FILESIZE);
                            }

                        }

                        if ((tmp_bitmap & AUDIBLE_BITMAP) != 0)
                        {
                            t_audible_total++;
                            //AUDIBLE 文件file_index 的bit15 置1
                            t_file_index = t_file_index | 0x8000;
                        }

                        file_index_buf[t_file_total] = t_file_index;
                        t_file_total++;

                        restart_find = TRUE;
                        break;//while(1)
                    }
                    dir_type = DIR_NEXT;
                }
                else
                {
                    restart_find = TRUE;
                    break;//while(1)
                }
            }

        }
        else
        {
            //文件名保存到缓冲区
            if (name_cnt < 63)
            {
                m3u_obj_name_buf[name_cnt] = value;
                name_cnt++;
            }
            else
            {
                //buf已满
                name_cnt = 63;
            }
        }
    }

    //    name_cnt = ap_plist_file_total % (FILE_INFO_BUF_SIZE / M3U_FILE_SIZE);
    //    if (name_cnt != 0)
    //    {
    //        save_to_file((uint8 *) FILE_INFO_ADDR, name_cnt * M3U_FILE_SIZE);
    //    }

    return ((uint32) t_audible_total << 16) | t_file_total;

}

