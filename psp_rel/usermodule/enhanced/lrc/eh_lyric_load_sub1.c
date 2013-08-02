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
 * \file     eh_lrc_load_sub1.c
 * \brief    解析歌词
 * \author   liminxian
 * \par      GENERAL DESCRIPTION:
 *               这里对文件进行描述
 * \par      EXTERNALIZED FUNCTIONS:
 *               FS
 * \version 1.0
 * \date  2011/9/15
 *******************************************************************************/

#include "eh_lrc.h"

#define LRC_EXT_BITMAP EXT_NAME_LRC

static const char offset_keyword[] =
{ 8, '[', 'O', 'F', 'F', 'S', 'E', 'T', ':' };//8是buffer的长度

static const uint8 lrc_asc[] = "LRC";
static pfile_offset_t temp_pfile_offset _BANK_DATA_ATTR_;
static pdir_layer_t temp_pdir_layer _BANK_DATA_ATTR_;

//static const uint16 lrc_unicode[4]={'L','R','C','\0'};

extern uint32 lrc_pos_buffer;
extern uint32 lrc_pos_file;
extern uint32 lrc_file_len;

extern uint8 positiveflag; //TRUE--'+',FALSE---'-'
extern uint8 offset_sec; // 1 s
extern uint8 offset_ms; //10ms


extern lrc_lable_t *lrc_lab_p; // 存时间标签指针
extern uint8 *buf_point; //lrc的buf解析指针
extern uint8 *save_lrc_buf_p; //存歌词内容指针
extern uint8 *lrc_vram_p;

extern uint8 get_time_buf[8]; //获取时间标签临时缓存
extern uint8 *get_time_p; //缓存单个时间标签指针


extern uint16 same_str_count; //同歌词的时间标签总数,1-只有1个标签，2-有2个标签
extern uint16 lrc_str_len; //每条歌词的长度
extern uint16 lab_lrc_offset; //每条歌词在内容中的偏移位置


extern uint8 lrc_temp_buf[SECTOR_SIZE]; //读lrc缓存
extern uint8 lrc_save_buf[SECTOR_SIZE]; //存歌词内容缓存


//void deal_time_lab(void);
//bool add_time_offset(void);

//extern void write_lrc_len(void);
extern bool read_page_data(void);
extern char_type_e check_char_type(char data);

bool get_offset_data(void);
bool mem_search_key(uint8 *sBuff, const char *key_buf);
uint16 len_sting(uint8 *str, uint8 str_type);

/******************************************************************************/
/*
 * \par  Description: 从指定的buffer查找指定的关键key
 *
 * \param[in]    sBuff，指定buffer
 key_buf,关键key

 * \param[out]

 * \return        TRUE OR FALSE

 * \note
 *******************************************************************************/
bool mem_search_key(uint8 *sBuff, const char *key_buf)
{

    uint8 k;
    uint16 j;
    bool result;
    j = 0;

    while (lrc_pos_file <= lrc_file_len)
    {
        result = read_page_data();//如数据不够就读一个PAGE数据
        if (!result)
        {
            return result;//读失败就退出
        }

        k = sBuff[lrc_pos_buffer];

        if (check_char_type((char) k) == CHAR_TYPE_DNER)
        {
            k = k - 0x20;//变为大写来处理
        }

        if (k != key_buf[j + 1])//因为第一个BUFF是数组的长度
        {
            j = 0;
        }
        else
        {
            j++;
        }
        lrc_pos_buffer++;

        if (j >= key_buf[0])
        {
            //是否全部查找完
            return TRUE;
        }
    }
    return 0;//cancel the warmning

}

/******************************************************************************/
/*
 * \par  Description: 查歌词时间差异项
 *
 * \param[in]

 * \param[out]

 * \return        TRUE OR FALSE

 * \note
 *******************************************************************************/
bool read_offset(void)
{
    bool result;

    offset_sec = 0;// 1 s
    offset_ms = 0; //10ms

    lrc_pos_buffer = SECTOR_SIZE;//要求读数据(因大等于512就要读数据)
    lrc_pos_file = 0;

    while (lrc_pos_file <= lrc_file_len)
    {
        //全部是一个字符一个字符地检测，所以要常读
        result = read_page_data();//读一个PAGE数据
        if (!result)
        {
            //            vfs_file_seek(vfs_mount,0,SEEK_SET,lrc_handle);//回到文件的开始位置
            return FALSE;
        }

        //找关键字 [OFFSET:
        result = mem_search_key(lrc_temp_buf, offset_keyword);//找字串
        if (!result)
        {
            //            vfs_file_seek(vfs_mount,0,SEEK_SET,lrc_handle);//回到文件的开始位置
            return FALSE;
        }

        //找到关键字，读取偏移时间
        result = get_offset_data();
        if (FALSE != result)
        {
            //成功找到Offset的值，退出
            //            vfs_file_seek(vfs_mount,0,SEEK_SET,lrc_handle);//回到文件的开始位置
            return TRUE;
        }
        //循环的意义是，读偏移时间是若发现[OFFEST:XXX]这种情况会出错退出
        //而这个时候文件还未结束，可以继续搜索
    }
    return 0;//cancel the warmning
}

/******************************************************************************/
/*
 * \par  Description: 读歌词时间差异值
 *
 * \param[in]

 * \param[out]

 * \return        TRUE OR FALSE

 * \note
 *******************************************************************************/
bool get_offset_data(void)
{
    uint8 result;
    uint8 i = 0;
    uint8 tmpbuff[5] =
    { 0, 0, 0, 0, 0 };

    positiveflag = TRUE;//默认为正号

    if (lrc_temp_buf[lrc_pos_buffer] == '-')
    {
        positiveflag = FALSE;
        lrc_pos_buffer++;
        result = read_page_data();
        if (!result)
        {
            return FALSE;
        }
    }
    else if (lrc_temp_buf[lrc_pos_buffer] == '+')
    {
        lrc_pos_buffer++;
        result = read_page_data();
        if (!result)
        {
            return FALSE;
        }
    }
    else
    {
    }

    while (lrc_pos_file <= lrc_file_len)
    {
        if (lrc_temp_buf[lrc_pos_buffer] == ']')
        {
            //已读到数据，作转换，并返回真 毫秒是单位
            //例如[OFFEST:234],偏移234毫秒，读为[2][3][4][][] 则应移成[0][0][2][3][4]
            //数字在LRC文件里面也是ASCII字符
            for (; i < 5; i++)
            {
                //右移一位,左边补0
                tmpbuff[4] = tmpbuff[3];
                tmpbuff[3] = tmpbuff[2];
                tmpbuff[2] = tmpbuff[1];
                tmpbuff[1] = tmpbuff[0];
                tmpbuff[0] = 0;
            }

            offset_sec = tmpbuff[1] + (uint8) (tmpbuff[0] * 10);//求出秒数
            offset_ms = tmpbuff[3] + tmpbuff[2] * 10; //求取10ms级
            return TRUE;
        }

        //防止  [OFFEST:2XXX]这种情况
        if (check_char_type((char) lrc_temp_buf[lrc_pos_buffer]) != CHAR_TYPE_NUM)
        {
            //碰到不是数字的就出
            return FALSE;
        }

        if (i < 5)
        {
            //BUFF 只有五个byte,超过了就不再补，这里可能会造成误差
            tmpbuff[i] = lrc_temp_buf[lrc_pos_buffer] - '0';
            i++;
        }

        lrc_pos_buffer++;
        result = read_page_data();
        if (!result)
        {
            return FALSE; //
        }
    }
    return 0;//cancel the warmning
}

/******************************************************************************/
/*
 * \par  Description: 解析歌词初始参数
 *
 * \param[in]

 * \param[out]

 * \return

 * \note
 *******************************************************************************/
void explain_lrc_init(void)
{
    vfs_file_seek(vfs_mount, 0, SEEK_SET, lrc_handle);
    lrc_pos_buffer = 0xffff;
    lrc_pos_file = 0;
    read_page_data();

    lrc_lab_p = (lrc_lable_t *) LRC_LABLE_ADDR;
    buf_point = lrc_temp_buf;
    get_time_p = get_time_buf;
    save_lrc_buf_p = lrc_save_buf;

    time_lab_count = 0;
    same_str_count = 0;
    lrc_str_len = 0;
    lab_lrc_offset = 0;
    lrc_vram_p = (uint8 *) LRC_CONTENT_VM;

    return;
}

/******************************************************************************/
/*
 * \par  Description: 计算字符串的长度
 *
 * \param[in]    str，字符串内容
 str_type,字符的类型，1-unicode，0-内码

 * \param[out]

 * \return       字符的长度

 * \note
 *******************************************************************************/
uint16 len_sting(uint8 *str, uint8 str_type)
{
    uint16 len = 0;

    if (str_type == 1)
    {
        while (*(uint16*) str != 0x00)
        {
            len++;
            str = str + 2;
        }
    }
    else
    {
        while (*str != 0x00)
        {
            len++;
            str++;
        }
    }

    return len;

}

/******************************************************************************/
/*
 * \par  Description:打开歌词文件
 *
 * \param[in]    lrc_name，歌词文件名

 * \param[out]

 * \return      TRUE OR FALSE

 * \note
 *******************************************************************************/
bool open_lrc_file(uint8 *lrc_name)
{
    //    uint8 *lrc_name;
    uint8 i;
    uint16 temp;

    if (lrc_name == NULL)
    {
        return FALSE;
    }

    if (*(uint16*) lrc_name == 0xfeff)
    {//unicode
        i = 1;
    }
    else
    {
        i = 0;
    }

    temp = len_sting(lrc_name, i);
    if ((0 == temp) || ((SECTOR_SIZE / 2) < temp))
    {
        return FALSE;
    }

    //save current path
    vfs_file_dir_offset(vfs_mount, &temp_pdir_layer, &temp_pfile_offset, 0);

    if (i > 0)
    {
        //unicode
        *(uint16*) (lrc_name + (temp - 3) * 2) = 0x0000;//把MUSIC的后缀名去掉
        if (0 != vfs_dir(vfs_mount, DIR_HEAD, lrc_name, LRC_EXT_BITMAP))
        {
            lrc_handle = vfs_file_open(vfs_mount, NULL, R_NORMAL_SEEK);
        }
        else
        {
            lrc_handle = 0;
        }
    }
    else
    {
        temp = temp - 3;
        libc_memcpy(lrc_temp_buf, lrc_name, (uint32) temp);
        libc_memcpy(lrc_temp_buf + temp, lrc_asc, 4);
        lrc_handle = vfs_file_open(vfs_mount, lrc_temp_buf, R_NORMAL_SEEK);
    }

    //back up path
    vfs_file_dir_offset(vfs_mount, &temp_pdir_layer, &temp_pfile_offset, 1);

    if (lrc_handle == 0)
    {
        return FALSE;
    }

    lrc_file_len = 0;
    vfs_file_get_size(vfs_mount, &lrc_file_len, lrc_handle, 0);

    if (lrc_file_len < 16)
    {
        vfs_file_close(vfs_mount, lrc_handle);
        lrc_handle = 0;
        return FALSE;
    }

    return TRUE;
}

