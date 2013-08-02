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

//M3U.LIB
static const uint8 m3u_libname[] =
{ 
    0xff, 0xfe, 0x4D, 0x00, 0x33, 0x00, 0x55, 0x00, 0x2E, 0x00, 0x4C, 0x00, 0x49, 0x00, 0x42, 0x00, 0x00, 0x00 
};

extern bool char_to_unicode(uint8 *dest, uint8 *src, uint16 len) __FAR__;
extern void wirte_m3u_dat(uint16 t_list_total, uint16 t_list_fileoffset) __FAR__;

/******************************************************************************
 m3u.dat
 |---------------------------|
 |       head (1K)           |
 |---------------------------|
 |       file_index(8K)      |
 |---------------------------|
 |       ....(M3U_LIST_MAX)  |
 |---------------------------|
 |     file_index(8K),       |
 |---------------------------|
 |     file_ino  (1M)        |
 |---------------------------|
 |       ....(4000 file)     |
 |---------------------------|
 |       file_pos_index (8k) |
 |---------------------------|
 ******************************************************************************/
/////
/***/

//对应BROM固化表，不能修改，NOTE!!!!!!
static const char ext_vec_rom[32][4] =
{ "***", "MP3", "WMA", "WAV", "AAC", "FLAC", "APE", "AVI", "TXT", "JPG", "JPEG", "BMP", "GIF", "LRC", "ASF", "OGG",
        "WMV", "AMV", "AAX", "AA ", "MP1", "MP2", "M4A", "** ", "** ", "** ", "** ", "** ", "** ", "**", "*f", "*d" 
};

//************************************
uint32 check_ext_bitmap(uint32 tmp_ext_name)
{
    uint8 i;
    uint32 bit_map = 0x80000000;

    for (i = 0; i < 32; i++)
    {
        if (0 == libc_memcmp(ext_vec_rom[i], &tmp_ext_name, 4))
        {
            break;
        }
        else
        {
            bit_map = bit_map >> 1;
        }
    }

    if (i == 32)
    {
        return 0;
    }

    return bit_map;

}

/******************************************************************************/
/*
 * \par  Description: 检查m3u文件是否存在
 *
 * \param[in]

 * \param[out]

 * \return        TRUE OR FALSE

 * \note
 *******************************************************************************/
bool check_m3u_exist(void)
{
    bool retval = FALSE;
    uint16 m3u_list_total;
    plist_head_t *head_p;

    //定位到根目录
    vfs_cd(ap_vfs_mount, CD_ROOT, 0);
    ap_obj_handle = vfs_file_open(ap_vfs_mount, m3u_libname, R_NORMAL_SEEK);

    if (0 != ap_obj_handle)
    {
        vfs_file_read(ap_vfs_mount, temp_plist_buf, SECTOR_SIZE, ap_obj_handle);
        if ((temp_plist_buf[510] == 0x55) && (temp_plist_buf[511] == 0xAA))
        {
            head_p = (plist_head_t*) temp_plist_buf;
            m3u_list_total = head_p->file_total;

            if (m3u_list_total != 0)
            {
                wirte_m3u_dat(m3u_list_total, head_p->file_info_offset);
                retval = TRUE;
            }
        }
    }

    if (0 != ap_obj_handle)
    {
        vfs_file_close(ap_vfs_mount, ap_obj_handle);
    }

    return retval;
}

/******************************************************************************/
/*
 * \par  Description: 整理扩展buffer
 *
 * \param[in]    buf_ext，缓存地址

 * \param[out]

 * \return        TRUE OR FALSE

 * \note
 *******************************************************************************/
void parse_ext_name(uint8 *buf_ext)
{
    uint8 *target_buf;
    uint8 val;

    target_buf = (uint8 *) buf_ext;

    //处理 .YY ,.YYY和.YYYY情况
    if (*buf_ext == 0x2e)
    {
        buf_ext++;
    }
    else if (*(buf_ext + 1) == 0x2e)
    {
        buf_ext = buf_ext + 2;
        *(buf_ext + 2) = 0x20;//两位后缀名,加空格
    }
    else
    {
        ;//QAC
    }

    while (*buf_ext != 0)
    {
        val = (uint8) ((*buf_ext) & 0xff);
        buf_ext++;
        if ((val >= 'a') && (val <= 'z'))
        {
            val = val - 0x20;
        }
        *target_buf = val;
        target_buf++;
    }

    *target_buf = 0x00;

    return;

}

/******************************************************************************/
/*
 * \par  Description: 整理name buff
 *
 * \param[in]     buf_name,缓存地址
 name_len
 dir_flag

 * \param[out]

 * \return        目标文件名 类型，0--短名，1--长名

 * \note          会生产对应的长名
 *******************************************************************************/
uint8 parse_obj_name(uint8* buf_name, uint8 name_len, uint8 dir_flag)
{
    uint8 i, j;
    uint8 flag = 0;
    uint8 *target_buf = (uint8*) (TITLE_DATA_ADDR + SECTOR_SIZE * 4); //0.5k
    uint16 *unicode_pt;

    unicode_pt = (uint16*) (target_buf + 2);
    char_to_unicode((uint8*) unicode_pt, buf_name, name_len);

    if (dir_flag == 1)
    {
        j = 9;
    }
    else
    {
        j = 12;
    }

    i = 0;
    while (*unicode_pt != 0x0000)
    {
        i++;
        unicode_pt++;
    }

    *target_buf = i;
    target_buf = target_buf + 256;

    if (name_len < j)
    {
        //文件可能是短名
        for (i = 0; i < name_len; i++)
        {
            if (buf_name[i] > 0x80)
            {
                flag = 1;
                break;
            }
            else if ((buf_name[i] >= 'a') && (buf_name[i] <= 'z'))
            {
                buf_name[i] = buf_name[i] - 0x20;//转成大写
            }
            else
            {
                ;//QAC
            }
        }
    }
    else
    {
        //文件是长名
        flag = 1;
    }

    if (flag == 0)
    {
        libc_memset(target_buf, 0x20, 11);
        //转成 8+3格式
        if (dir_flag == 0)
        {
            for (i = 0; i < name_len; i++)
            {
                if (buf_name[i] != 0x2e)
                {
                    target_buf[i] = buf_name[i];
                }
                else
                {
                    break;
                }
            }

            j = 8;
            for (i = i + 1; i < name_len; i++)
            {
                target_buf[j] = buf_name[i];
                j++;
            }
        }
        else
        {
            libc_memcpy(target_buf, buf_name, (uint32) name_len);
        }
        libc_memcpy(buf_name, target_buf, 11);
    }
    return flag;
}

/******************************************************************************/
/*
 * \par  Description: 取出当前位置的文件，比较文件名
 *
 * \param[in]
 name_obj_buf
 buf_cnt--字符个数
 name_type    0--短名，1--长名

 * \param[out]

 * \return        TRUE OR FALSE

 * \note         不能改变 name_obj_buf 的内容
 *******************************************************************************/
bool compare_name(uint8 *name_obj_buf, uint16 buf_cnt, uint8 name_type)
{
    uint8 uni_cnt;
    uint16 name_len;
    uint8 *unicode_name_buf;

    unicode_name_buf = (uint8*) (TITLE_DATA_ADDR + SECTOR_SIZE * 4);
    uni_cnt = *unicode_name_buf;
    unicode_name_buf = unicode_name_buf + 2;

    name_len = vfs_get_name(ap_vfs_mount, m3u_get_name_buf, 32);//64 BYTES
    if (*(uint16*) m3u_get_name_buf == UNICODE_FLAG)
    {
        //unicode
        if (uni_cnt > (name_len - 2))//不比较头和结束符
        {
            uni_cnt = (uint8) name_len - 2;
        }

        m3u_get_name_buf = m3u_get_name_buf + 2;//去掉头信息 0xfeff

        if (0 == libc_memcmp(m3u_get_name_buf, unicode_name_buf, uni_cnt * 2))
        {
            return TRUE;
        }
    }
    else if (name_type == 0)
    {
        //短名固定比较8字节长度
        if (0 == libc_memcmp(m3u_get_name_buf, name_obj_buf, 8))
        {
            return TRUE;
        }
    }
    else
    {
        ;//QAC
    }

    return FALSE;
}

/******************************************************************************/
/*
 * \par  Description: 检查m3u文件是否存在
 *
 * \param[in]

 * \param[out]

 * \return        key value

 * \note
 *******************************************************************************/
/*读取m3u文件offset对应的字节位置信息*/
uint8 read_m3u_data(uint32 offset)
{
    uint8 *t_data_buf = (uint8*) (TITLE_DATA_ADDR + SECTOR_SIZE * 6); //0.5k
    /*读取的数据不在当前的buff*/
    if ((offset / SECTOR_SIZE) != m3u_cursec_num)
    {
        m3u_cursec_num = (uint16) (offset / SECTOR_SIZE);
        vfs_file_seek(ap_vfs_mount, m3u_cursec_num * SECTOR_SIZE, SEEK_SET, m3u_handle);
        vfs_file_read(ap_vfs_mount, t_data_buf, SECTOR_SIZE, m3u_handle);
    }
    m3u_curpos_offset = (uint16) offset;
    return t_data_buf[offset % SECTOR_SIZE];
}

/******************************************************************************/
/*
 * \par  Description: 配置到M3U文件的路径
 *
 * \param[in]

 * \param[out]

 * \return        dir_layer;

 * \note
 *******************************************************************************/

uint8 set_m3ufile_path(void)
{
    vfs_file_dir_offset(ap_vfs_mount, &other_f_p->dir_layer_info, &other_f_p->cluster_no, 1);
    return other_f_p->dir_layer_info.buffer[MAX_DIR_LAYER * 8];
}

