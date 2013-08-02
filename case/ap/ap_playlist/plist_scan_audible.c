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

//audible playlist
static const char no_author[] = "Unknown Author";

static bool get_audible_info(uint32 ext_name);
static void deal_audible_id3info(void);
static void store_audible_info(uint16 file_num);

extern id3_type_e check_id3_type(uint32 ext_name) __FAR__;
extern bool unicode_to_char(char *str,uint16 len,unsigned char encode) __FAR__;
extern void copy_deal_string(char *dest,char * source,uint8 land_id,uint8 size) __FAR__;
extern bool plist_fsel_get_nextfile (char *strfile) __FAR__;
/******************************************************************************/
/*
 * \par  Description: 获取文件的信息(ID3,磁盘信息)
 *
 * \param[in]    ext_name--扩展名

 * \param[out]

 * \return       TRUE OR FALSE

 * \note
 *******************************************************************************/
static bool get_audible_info(uint32 ext_name)
{
    id3_type_e id3_type;
    id3_type = check_id3_type(ext_name);

    if (id3_type < ID3_TYPE_END)
    {
        if (FALSE != get_id3_info(&id3_file_info, NULL, id3_type))
        {
            audible_f_p->id3_tag_flag = 1;

            if ((id3_file_info.track_num != 0) && (id3_file_info.track_num != UNICODE_FLAG))
            {
                audible_f_p->track_num = id3_file_info.track_num;
            }

        }
        else
        {
            audible_f_p->id3_tag_flag = 0;
        }

    }
    else
    {
        audible_f_p->id3_tag_flag = 0;
    }

    *(uint32*) (audible_f_p->filename) = ext_name;
    vfs_file_dir_offset(ap_vfs_mount, &audible_f_p->dir_layer_info, &audible_f_p->cluster_no, 0);

    deal_audible_id3info();

    audible_f_p->flag = 1;

    return TRUE;
}

//过滤空格,转换为 xx.yyy格式
//返回字符长度，不包含结束符
static void deal_if_83name(uint8 *name_buf)
{

#if 0  //已在id3获取时填入title，此次不需要处理
    uint8 i,j;

    if((name_buf[0]==0xff) && (name_buf[1]==0xfe))
    {
        return;
    }

    //扩展名往后移1个字节
    for(i=11;i>8;i--)
    {
        name_buf[i]=name_buf[i-1];
    }

    //过滤文件名空格
    for(i=7;i>0;i--)
    {
        if(name_buf[i]!=0x20)
        {
            break;
        }
    }

    //加点
    i++;
    name_buf[i]=0x2e; //加"."

    //过滤扩展名空格
    for(j=9;j<12;j++)
    {
        if(name_buf[j]!=0x20)
        {
            i++;
            name_buf[i]= name_buf[j];
        }
    }

    if(name_buf[i]!=0x2e)
    {
        i++;
    }

    name_buf[i]=0x00;

#endif

    return;
}

/******************************************************************************/
/*
 * \par  Description: 对获取到的ID3信息进行处理
 *
 * \param[in]

 * \param[out]

 * \return

 * \note
 *******************************************************************************/
static void deal_audible_id3info(void)
{

    //此处判断ID3信息是否为空可以去掉，因为在获取id3是已自动填入谢谢
    if ((audible_f_p->title_string == 0) || (audible_f_p->id3_tag_flag == 0))
    {
        vfs_get_name(ap_vfs_mount, audible_f_p->title_string, (ID3_ITEM_SIZE * 2) / 2);
        deal_if_83name((uint8*) &audible_f_p->title_string);
    }

    if ((audible_f_p->artist_string == 0) || (audible_f_p->id3_tag_flag == 0))
    {
        libc_memcpy(audible_f_p->artist_string, no_author, sizeof(no_author));
    }

    if (audible_f_p->id3_tag_flag == 0)
    {
        audible_f_p->track_num = 0;
    }
    else
    {

        if (audible_f_p->track_num == UNICODE_FLAG)
        {
            unicode_to_char(audible_f_p->track_string, 8, language_id);
            audible_f_p->track_num = 0;
        }

        if ((audible_f_p->track_num == 0) && (audible_f_p->track_string[0] != 0))
        {
            uint8 i;
            uint8 j = 1;
            for (i = 0; i < 4; i++)
            {
                if ((audible_f_p->track_string[i] == 0) || (audible_f_p->track_string[i] == '/'))
                {
                    break;
                }
                if ((audible_f_p->track_string[i] < 0x30) || (audible_f_p->track_string[i] > 0x39))
                {
                    break;
                }
                else
                {
                    audible_f_p->track_string[i] -= 0x30;
                }
            }
            while (i > 0)
            {
                i--;
                audible_f_p->track_num += audible_f_p->track_string[i] * j;
                j *= 10;

            }
        }

        if (audible_f_p->track_num == 0)
        {
            audible_f_p->track_num = TRACK_NULL;
        }

    }

}

/******************************************************************************/
/*
 * \par  Description: 存储信息到RAM或VM区
 *
 * \param[in]    file_num--当前已存放的文件信息总数

 * \param[out]

 * \return

 * \note        先判断buffer是否满了，再写入
 *******************************************************************************/
static void store_audible_info(uint16 file_num)
{

    if (file_num != 0)
    {
        if (file_num == PLIST_SAVE_MAX)
        {
            write_to_vram(title_vram_pt, (uint8*) TITLE_DATA_ADDR, TITLE_BUF_SIZE);
            write_to_vram(artist_vram_pt, (uint8*) ARTIST_DATA_ADDR, ARTIST_BUF_SIZE);
            //        write_to_vram(track_vram_pt,GENRE_DATA_ADDR,SECTOR_SIZE*4);

            title_vram_pt = title_vram_pt + TITLE_BUF_SIZE;
            artist_vram_pt = artist_vram_pt + ARTIST_BUF_SIZE;
            //        track_vram_pt=track_vram_pt+SECTOR_SIZE*4;

            title_data_pt = (uint8*) TITLE_DATA_ADDR;
            artist_data_pt = (uint8*) ARTIST_DATA_ADDR;
            //        track_data_pt=TRACK_DATA_ADDR;
        }
        else
        {
            title_data_pt += TITLE_CMP_LENGTH;
            artist_data_pt += ARTIST_CMP_LENGTH;
            //        track_data_pt+=1;
        }

        if ((file_num % (FILE_INFO_BUF_SIZE / PLIST_FILE_SIZE)) == 0)
        {
            save_to_file((uint8*) FILE_INFO_ADDR, FILE_INFO_BUF_SIZE);
            fileinfo_data_pt = (uint8*) FILE_INFO_ADDR;
        }
        else
        {
            fileinfo_data_pt += PLIST_FILE_SIZE;
        }
    }

    audible_f_p->prev_offset = file_num - 1;
    audible_f_p->next_offset = file_num + 1;

    copy_deal_string(title_data_pt, audible_f_p->title_string, language_id, CMPSTR_MAX_LEN1);
    copy_deal_string(artist_data_pt, audible_f_p->artist_string, language_id, CMPSTR_MAX_LEN2);
    libc_memcpy(fileinfo_data_pt, audible_f_p, sizeof(plist_f_info_t));

}

/******************************************************************************/
/*
 * \par  Description: 扫描文件信息
 *
 * \param[in]

 * \param[out]

 * \return        文件的总数

 * \note
 *******************************************************************************/
uint16 scan_audible_file(void)
{
    //    uint8 *path_vm_pt,*path_data_pt;
    uint32 temp;
    uint32 ext_name;

    //    utf8_flag=1;

    audible_f_p = (void*) file_buffer;

    id3_file_info.tit2_buffer = audible_f_p->title_string;
    id3_file_info.tit2_length = ID3_ITEM_SIZE * 2;
    id3_file_info.tpe1_buffer = audible_f_p->artist_string;
    id3_file_info.tpe1_length = ID3_ITEM_SIZE;
    id3_file_info.talb_buffer = 0;
    id3_file_info.talb_length = 0;
    id3_file_info.genre_buffer = 0;
    id3_file_info.genre_length = 0;

    id3_file_info.track_buffer = audible_f_p->track_string;
    id3_file_info.track_length = 8;
    id3_file_info.drm_length = 0;
    id3_file_info.drm_buffer = 0;

#ifdef ENHANCED_FIND_MODE
    while(FALSE!=fsel_get_nextfile((char*)&ext_name))
#else
    while (FALSE != plist_fsel_get_nextfile((char*) &ext_name))
#endif

    {
        if (0 == (ap_plist_file_total & 0x07))
        {
            plist_get_app_msg();
        }
        libc_memset(audible_f_p, 0x00, sizeof(plist_f_info_t));
        get_audible_info(ext_name);
        store_audible_info(ap_plist_file_total);
        ap_plist_file_total++;
        if (ap_plist_file_total >= PLIST_AUDIBLE_MAX)
        {
            break;
        }
    }

    if (ap_plist_file_total > PLIST_SAVE_MAX)
    {
        temp = (ap_plist_file_total % PLIST_SAVE_MAX) * TITLE_CMP_LENGTH;
        write_to_vram(title_vram_pt, (uint8*) TITLE_DATA_ADDR, temp);
        temp = (ap_plist_file_total % PLIST_SAVE_MAX) * ARTIST_CMP_LENGTH;
        write_to_vram(artist_vram_pt, (uint8*) ARTIST_DATA_ADDR, temp);
    }

    if (ap_plist_file_total > 0)
    {
        temp = ap_plist_file_total % (FILE_INFO_BUF_SIZE / PLIST_FILE_SIZE);
        if (temp == 0)
        {
            //刚好写满
            temp = FILE_INFO_BUF_SIZE / PLIST_FILE_SIZE;
        }
        save_to_file((uint8*) FILE_INFO_ADDR, temp * PLIST_FILE_SIZE);
    }

    plist_get_app_msg();
    return ap_plist_file_total;

}

