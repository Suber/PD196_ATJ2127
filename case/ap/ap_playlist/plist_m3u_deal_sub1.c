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

extern uint32 parse_m3u_file(uint8 cur_m3u_index) __FAR__;

/******************************************************************************/
/*
 * \par  Description: 创建m3u的lib文件
 *
 * \param[in]

 * \param[out]

 * \return        TRUE OR FALSE

 * \note
 *******************************************************************************/
void wirte_m3u_dat(uint16 t_list_total, uint16 t_list_fileoffset)
{
    uint16 i;
    uint32 temp32;
    m3udat_head_t* t_head_pt;
    uint8 *t_read_buf;

    ap_pos_file_offset = 0;
    ap_plist_file_total = 0;
    count_buf_a = (uint16*) (TITLE_DATA_ADDR + SECTOR_SIZE * 0); //1k
    filepos_p = (file_pos_t*) ALBUM_DATA_ADDR;//32k
    t_read_buf = (uint8*) (TITLE_DATA_ADDR + SECTOR_SIZE * 7); //0.5k

    m3u_dat_file_pt = (m3ulist_f_info_t *) FILE_INFO_ADDR;
    libc_memset(FILE_INFO_ADDR, 0x00, FILE_INFO_BUF_SIZE);
    ap_plist_file_offset = (uint16) (M3U_DAT_FILEOFFSET / SECTOR_SIZE);

    for (i = 0; i < t_list_total; i++)
    {
        if ((i & 0x01) != 0)
        {
            other_f_p = (plist_f_info_t *) (t_read_buf + PLIST_FILE_SIZE);
        }
        else
        {
            //读取lib 获取M3U list文件位置
            vfs_file_seek(ap_vfs_mount, (t_list_fileoffset + (i >> 1)) * SECTOR_SIZE, SEEK_SET, ap_obj_handle);
            vfs_file_read(ap_vfs_mount, t_read_buf, SECTOR_SIZE, ap_obj_handle);
            other_f_p = (plist_f_info_t *) t_read_buf;
        }

        if (0 != vfs_file_dir_offset(ap_vfs_mount, &other_f_p->dir_layer_info, &other_f_p->cluster_no, 1))
        {
            //OPEN *.M3U 文件
            m3u_handle = vfs_file_open(ap_vfs_mount, NULL, R_NORMAL_SEEK);
            if (0 != m3u_handle)
            {
                m3u_curpos_offset = 0x0000;
                m3u_cursec_num = 0xffff;
                count_buf_a[i] = 0;

                m3u_get_name_buf = (uint8*) (TITLE_DATA_ADDR + SECTOR_SIZE * 2); //0.5k
                m3u_obj_name_buf = (uint8*) (TITLE_DATA_ADDR + SECTOR_SIZE * 3); //0.5k
                //                m3u_obj_name_buf_uni = (uint8*) (TITLE_DATA_ADDR + SECTOR_SIZE * 4);   //0.5k,已用
                m3u_ext_name = (uint8*) (TITLE_DATA_ADDR + SECTOR_SIZE * 5); //0.5k
                //              read_m3u_buffer = (uint8*) (TITLE_DATA_ADDR + SECTOR_SIZE * 6);          //0.5k 已用
                //t_read_buf = (uint8*) (TITLE_DATA_ADDR + SECTOR_SIZE * 7);//0.5k //已使用
                file_index_buf = (uint16*) (TITLE_DATA_ADDR + SECTOR_SIZE * 8); //8K
                libc_memset(file_index_buf, 0x00, SECTOR_SIZE);//clear,for seek debug
                // 定位到DAT 填入文件信息位置
                ap_pos_file_offset = ap_plist_file_offset;//修改savefile 接口中ap_pos_file_offset值
                vfs_file_seek(ap_vfs_mount, ap_plist_file_offset * SECTOR_SIZE, SEEK_SET, ap_obj_handle);
                //只写入文件信息
                temp32 = parse_m3u_file((uint8) i);
                count_buf_a[i] = (uint16) (temp32 & 0xffff);
                count_buf_a[i + M3U_LIST_MAX] = (uint16) (temp32 >> 16);

                vfs_file_close(ap_vfs_mount, m3u_handle);
                ap_plist_file_offset = ap_pos_file_offset;

                //save file_index_buf
                if (count_buf_a[i] != 0)
                {
                    vfs_file_seek(ap_vfs_mount, M3U_DAT_INDEXOFFSET + i * M3U_DAT_INDEXSIZE, SEEK_SET, ap_obj_handle);
                    save_to_file((void*) file_index_buf, count_buf_a[i] * 2);
                }

            }
        }

    }

    //判断回写文件信息区域数据
    i = (uint16) ap_plist_file_total % (FILE_INFO_BUF_SIZE / M3U_DAT_FILESIZE);
    if (i != 0)
    {
        vfs_file_seek(ap_vfs_mount, ap_plist_file_offset * SECTOR_SIZE, SEEK_SET, ap_obj_handle);
        save_to_file((uint8 *) FILE_INFO_ADDR, i * M3U_DAT_FILESIZE);
    }

    if (0 != ap_plist_file_total)
    {
        vfs_file_seek(ap_vfs_mount, M3U_DAT_POS_INDEXOFFSET, SEEK_SET, ap_obj_handle);
        save_to_file((uint8 *) ALBUM_DATA_ADDR, 8 * ap_plist_file_total); //save pos index
    }

    //写DAT 文件的头信息
    vfs_file_seek(ap_vfs_mount, 0, SEEK_SET, ap_obj_handle);
    vfs_file_read(ap_vfs_mount, temp_plist_buf, SECTOR_SIZE, ap_obj_handle);
    t_head_pt = (m3udat_head_t*) (temp_plist_buf + M3U_DAT_HEADOFFSET);
    libc_memcpy(t_head_pt->list_file_num, count_buf_a, t_list_total * 2);//
    libc_memcpy(t_head_pt->audible_file_num, &count_buf_a[M3U_LIST_MAX], t_list_total * 2);//
    t_head_pt->dat_file_total = ap_plist_file_total;

    vfs_file_seek(ap_vfs_mount, 0, SEEK_SET, ap_obj_handle);
    save_to_file(temp_plist_buf, SECTOR_SIZE);
}

