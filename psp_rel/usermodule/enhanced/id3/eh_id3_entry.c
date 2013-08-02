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
 * \file     eh_id3_entry.c
 * \brief    这里填写文件的概述
 * \author   liminxian
 * \par      GENERAL DESCRIPTION:
 *               这里对文件进行描述
 * \par      EXTERNALIZED FUNCTIONS:
 *               这里描述调用到外面的模块
 * \version 1.0
 * \date  2011/9/15
 *******************************************************************************/

#include "eh_id3.h"

typedef bool (*id3_func_f)(void);

static const id3_func_f get_id3_func[ID3_TYPE_END] =
{ get_id3_aa, get_id3_aac, get_id3_aax, get_id3_ape, get_id3_flac, get_id3_mp3, get_id3_ogg, get_id3_wma };

id3_save_t id3_save_buf _BANK_DATA_ATTR_;

uint8 id3_temp_buf[SECTOR_SIZE] _BANK_DATA_ATTR_;
uint8 key_buf[KEY_BUF_LEN] _BANK_DATA_ATTR_;
uint8 check_count _BANK_DATA_ATTR_;
uint8 check_flag[8] _BANK_DATA_ATTR_; //查找ID3要素标记澹:0--为需要查找,1--为不需要查找
uint16 id3_pos_buffer _BANK_DATA_ATTR_; //当前TempBuf未处理字符的索引
uint32 id3_pos_file _BANK_DATA_ATTR_; //当前文件的精确指针
id3_save_t *id3_save_p _BANK_DATA_ATTR_; //FrameID存储位置
id3_info_t *id3_info_p _BANK_DATA_ATTR_; //ap层提供的存储结构


extern bool mp3_v2_parse(void);
extern bool mp3_v1_parse(void);

extern void check_id3_buffer(id3_info_t *id3_info,id3_type_e file_type) __FAR__;

/******************************************************************************/
/*
 * \par  Description: 获取指定文件的id3信息
 *
 * \param[in][out]    id3_info,存放输入参数和输出id3信息
 * \param[in]       filename,文件名,为NULL即已定位当前目录项
 * \param[in]       file_type,id3类型
 * \param[out]      none
 * \return          TRUE OR FALSE

 * \note
 *******************************************************************************/
bool get_id3_info(id3_info_t *id3_info, const char *filename, id3_type_e file_type)
{
    bool ret_val;
    uint8 i;
    uint8 *pt;
    uint32 *str_buf;
    //    id3_func_f id3_func_p;


    if (id3_info == NULL)
    {
        return FALSE;
    }

    id3_info_p = id3_info;
    id3_save_p = &id3_save_buf;

    libc_memset(check_flag, 0, sizeof(check_flag));
    pt = (uint8*) id3_info_p + 24;
    str_buf = (uint32*) id3_info_p;
    check_count = 0;

    for (i = 0; i < 6; i++)
    {
        if (*pt > 0)
        {
            check_flag[i] = 1; //置获取标志
            check_count++;
            *(uint16*) (*str_buf) = 0x00; //预填入结束符号
        }
        pt++;
        str_buf++;
    }

    id3_info_p->track_num = 0xffff;

    if (id3_info->apic_flag == 1)
    {
        check_flag[6] = 1;
        check_count++;
    }

    if (id3_info->ldb_flag == 1)
    {
        check_flag[7] = 1;
        check_count++;
    }

    id3_info->apic_type = 0;

    //不支持的文件格式，也要默认填入ID3信息
    if (file_type < ID3_TYPE_END)
    {
        id3_handle = vfs_file_open(vfs_mount, filename, R_NORMAL_SEEK);

        if (0 == id3_handle)
        {
            return FALSE;
        }

        ret_val = get_id3_func[file_type - ID3_TYPE_AA]();

        vfs_file_close(vfs_mount, id3_handle);

    }

    //切bank1
    check_id3_buffer(id3_info, file_type);

    return TRUE;

}

/******************************************************************************/
/*
 * \par  Description: 解析文件前，初始化动作
 *
 * \param[in]

 * \param[out]

 * \return

 * \note
 *******************************************************************************/

bool get_init(void)
{
    id3_pos_buffer = 0;
    id3_pos_file = 0;

    vfs_file_read(vfs_mount, id3_temp_buf, SECTOR_SIZE, id3_handle); //读一PAGE数据
    return TRUE;
}

/******************************************************************************/
/*
 * \par  Description: 读文件数据到指定buffer
 *
 * \param[in]    Tbuff，数据buffer
 size，读取的数据长度（单位byte）
 * \param[out]

 * \return       TRUE OR FALSE

 * \note
 *******************************************************************************/

bool read_buf_data(uint8 *Tbuff, uint32 size)
{

    uint32 avail, remain;
    uint32 copy_size;
    uint32 next_pos;

    next_pos = id3_pos_buffer + size;

    copy_size = size;
    if (copy_size > KEY_BUF_LEN)
    {
        copy_size = KEY_BUF_LEN;
    }

    if (next_pos < SECTOR_SIZE) // if end of data is within buffer
    {
        libc_memcpy(Tbuff, &id3_temp_buf[id3_pos_buffer], copy_size);// copy data to vector
    }
    else
    {
        avail = SECTOR_SIZE - id3_pos_buffer; //how much data is in current buffer
        if (avail > copy_size)
        {
            libc_memcpy(Tbuff, &id3_temp_buf[id3_pos_buffer], copy_size); //copy to vector
            vfs_file_read(vfs_mount, id3_temp_buf, SECTOR_SIZE, id3_handle); // read next sector
        }
        else
        {
            libc_memcpy(Tbuff, &id3_temp_buf[id3_pos_buffer], avail); //copy to vector
            vfs_file_read(vfs_mount, id3_temp_buf, SECTOR_SIZE, id3_handle); // read next sector
            remain = copy_size - avail; // how much data must still be copied
            libc_memcpy(&Tbuff[avail], id3_temp_buf, remain); // copy remain data
        }
    }

    id3_pos_file = id3_pos_file + size; //update address in file
    id3_pos_buffer = (uint16) (id3_pos_file & (SECTOR_SIZE - 1)); //update position in buffer

    return TRUE;

}

/******************************************************************************/
/*
 * \par  Description: 调整读文件指针
 *
 * \param[in]    size--调整长度，byte
 last_pos_file--文件的最新位置；当不为0时，须重新定位文件的位置
 * \param[out]

 * \return       TRUE OR FALSE

 * \note
 *******************************************************************************/
char reset_file_pos(uint32 size, uint32 last_pos_file)
{
    uint32 next_pos;
    uint32 sector;

    if (last_pos_file != 0)
    {
        id3_pos_file = last_pos_file;
    }

    id3_pos_file = id3_pos_file + size; // update address in file
    next_pos = id3_pos_buffer + size;

    if ((last_pos_file == 0) && (next_pos < SECTOR_SIZE)) //if next position is within buffer
    {
        id3_pos_buffer = (uint16) next_pos;
    }
    else
    {
        sector = id3_pos_file / SECTOR_SIZE;
        vfs_file_seek(vfs_mount, sector * SECTOR_SIZE, SEEK_SET, id3_handle);
        vfs_file_read(vfs_mount, id3_temp_buf, SECTOR_SIZE, id3_handle);
        id3_pos_buffer = (uint16) (id3_pos_file & (SECTOR_SIZE - 1));
    }

    return TRUE;

}

/******************************************************************************/
/*
 * \par  Description:把指定字符串转换成大写
 *
 * \param[in]    str--字符串

 * \param[out]   end_char--字符串结束符

 * \return       返回字符串的长度

 * \note
 *******************************************************************************/
uint8 str_to_capital(char *str, char end_char)
{
    uint32 i = 0;
    while ((str[i] != end_char) && (str[i] != '\0'))
    {
        if ((str[i] >= 'a') && (str[i] <= 'z'))
        {
            str[i] = str[i] - 'a' + 'A';
        }
        i++;

        if (i == KEY_BUF_LEN)
        {
            return 0;
        }

    }

    return i;
}

/******************************************************************************/
/*
 * \par  Description: 解析mp3类型文件
 *
 * \param[in]

 * \param[out]

 * \return        TRUE OR FALSE

 * \note
 *******************************************************************************/
bool get_id3_mp3(void)
{
    if (FALSE == mp3_v2_parse())
    {
        return mp3_v1_parse();
    }
    return TRUE;
}

