/*******************************************************************************
 *                              US212A
 *                            Module: radio_ui
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>         <time>             <version>             <desc>
 *      mikeyang    2011-9-23 11:11:27           1.0              build this file
 *******************************************************************************/

#include "app_radio.h"

uint16 Read_Data_Uni(uint32 offset);

/* 文件读取buffer */
uint8 radio_buf_uni[SECTOR_SIZE] _BANK_DATA_ATTR_;

/* 记录当前读取位置，扇区号*/
uint16 cursec_num_uni _BANK_DATA_ATTR_;
/* 记录当前读取位置，字节偏移*/
uint32 cur_offset_uni _BANK_DATA_ATTR_;
/* 频率字串数据*/
uint8 freq_string_uni[FREQ_LENTH*2] _BANK_DATA_ATTR_;

/******************************************************************************/
/*!
 * \par  Description:
 * \static uint32 atoi_uni(const char *src, int i)
 * \字符串转换为整型
 * \param[in]    src   字串指针
 * \                 i       字节数
 * \param[out]
 * \return    转换后的值
 * \retval
 * \retval
 * \note
 */
/*******************************************************************************/
static uint32 atoi_uni(const char *src, int i)
{
    int total = 0;

    while (i != 0)
    {
        total = total * 10 + (int) (*src - '0');
        src++;
        i--;
    }

    return total;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \void deal_station_num_uni(uint8 *ptr_num, uint8 index)
 * \处理用户电台列表的电台号
 * \param[in]    ptr_num   用户电台号字串指针
 * \                 index       当前处理电台号在用户电台列表文件中索引
 * \param[out]   none
 * \return
 * \retval
 * \retval
 * \note
 */
/*******************************************************************************/
void deal_station_num_uni(uint8 *ptr_num, uint8 index)
{
    uint8 station_num;
    station_num = (uint8) atoi_uni(ptr_num, 2);
    if (index < MAX_STATION_COUNT)
    {
        //更新用户电台列表的索引表
        g_userlist_table[index] = station_num;
    }
    return;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \void deal_user_freq_uni(char* ptr_freq, uint8 index)
 * \ 处理用户电台列表频点，保存到合适的位置
 * \param[in]    ptr_freq  频率字串指针
 * \                 index  当前处理频点在用户电台列表文件中索引
 * \param[out]   none
 * \return
 * \retval
 * \retval
 * \note
 */
/*******************************************************************************/
void deal_user_freq_uni(char* ptr_freq, uint8 index)
{
    uint32 freq;
    uint16 save_freq;
    uint8 station_num;

    freq = atoi_uni(ptr_freq, FREQ_LENTH);
    //只保存频点值的低2 字节，最高bit 恒为1
    save_freq = (uint16)(freq & 0x0ffff);

    station_num = g_userlist_table[index]; //电台号
    if (station_num >= 1)
    {
        //正常情况下，电台号都符合条件CH01~CH30
        g_user_station.fmstation_user[station_num - 1] = save_freq; //保存用户电台频点值
    }
    return;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \void  seek_to_pos_uni(uint8 mode)
 * \读指针定位到需要操作的位置
 * \param[in]    mode==0,  定位到下一个频率值开始处
 * \                 mode==1, 定位到下一个名称开始处
 * \param[out]   none
 * \return
 * \retval
 * \retval
 * \note
 */
/*******************************************************************************/
void seek_to_pos_uni(uint8 mode)
{
    uint32 byte_start, byte_cnt;
    uint32 temp;
    byte_start = cur_offset_uni;
    temp = byte_start;

    /* 排除最后[END] 共10 字节*/
    for (byte_cnt = byte_start; byte_cnt < (file_total_byte - 10); byte_cnt = byte_cnt + 2)
    {
        if (mode == 0)
        {
            //根据'=' 定位频率字串
            if (Read_Data_Uni(byte_cnt) == 0x003D)
            {
                //跳过'=' ，定位在频率位置
                cur_offset_uni += 2;
                break;
            }
        }
        else
        {
            //根据',' 定位电台名称字串
            if (Read_Data_Uni(byte_cnt) == 0x002C)
            {
                //跳过','，定位在频率位置
                cur_offset_uni += 2;
                break;
            }
        }
    }
    return;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \uint16 Read_Data_Uni(uint32 offset)
 * \在列表文件中读取指定偏移位置处的半字(  unicode 编码)
 * \param[in]    offset  读取位置~  字节偏移
 * \param[out]   none
 * \return       val_ret
 * \retval
 * \retval
 * \note  此函数会修改读写位置cur_offset
 */
/*******************************************************************************/
uint16 Read_Data_Uni(uint32 offset)
{
    uint16 val_ret;

    /*读取的数据不在当前的buff*/
    if ((uint16)(offset / 512) != cursec_num_uni)
    {
        cursec_num_uni = (uint16)(offset / 512);
        vfs_file_seek(vfs_mount_radio, cursec_num_uni * 512, SEEK_SET, fp_radio);
        /* 读取整扇区数据到buf */
        vfs_file_read(vfs_mount_radio, radio_buf_uni, SECTOR_SIZE, fp_radio);
    }
    cur_offset_uni = offset;

    val_ret = *(uint16*) &radio_buf_uni[offset % 512];
    return val_ret;
}

/*********************************************************************************************/
/*!
 * \par  Description:
 * \uint8 get_need_filenum_uni(userlist_parse_e mode, uint8 num)
 * \param[in]    mode
 * \ mode = ALL_FREQ, 从头开始查找有效电台总数
 * \ mode = INDEX_FROM_START,从头开始查找到索引num对应的位置(num from 0 to total-1)
 * \ mode = NEXT_FROM_CUR,从当前位置查找下一个有效的位置
 * \param[out]   none
 * \return
 * \retval
 * \retval
 * \note
 */
/**********************************************************************************************/
uint8 get_need_filenum_uni(userlist_parse_e mode, uint8 num)
{
    uint8 file_total = 0;
    uint32 byte_cnt, byte_start;

    if (mode == NEXT_FROM_CUR)
    {
        byte_start = cur_offset_uni;
    }
    else
    {
        /* 跳过文件头的unicode 标识*/
        byte_start = 2;
    }
    for (byte_cnt = byte_start; byte_cnt < (file_total_byte - 2); byte_cnt = byte_cnt + 2)
    {
        //回车换行后紧接着电台信息
        if ((Read_Data_Uni(byte_cnt) == 0x000D) && (Read_Data_Uni(byte_cnt + 2) == 0x000A))
        {
            /* '['  END 起始，找到最后了*/
            if (Read_Data_Uni(byte_cnt + 4) == 0x005B)
            {
                if (mode == ALL_FREQ)
                {
                    /* 已经找到文件尾，获得有效电台个数*/
                    return file_total;
                }
                else
                {
                    /* 已到文件尾，准备回第一个*/
                    return 0xff;
                }
            }

            /*找到下一个文件*/
            if (mode == NEXT_FROM_CUR)
            {
                break;
            }
            else if (mode == INDEX_FROM_START)
            {
                if (file_total == num)
                {
                    break;
                }
            }
            else
            {
            }
            file_total++;
        }
    }
    return file_total;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \void get_cur_data_uni(uint8 byte_cnt)
 * \从当前offset 开始，获取指定长度的内容
 * \param[in]    byte_cnt   需读取的字节长度
 * \param[out]   none
 * \return       none
 * \retval
 * \retval
 * \note
 */
/*******************************************************************************/
void get_cur_data_uni(uint8 byte_cnt)
{
    uint16 value;
    uint8 i;
    uint8 temp;
    uint32 byte_start;
    byte_start = cur_offset_uni;

    libc_memset(&freq_string_uni[0], 0, FREQ_LENTH * 2);
    for (i = 0; i < byte_cnt; i = i + 2)
    {
        if (i >= (FREQ_LENTH * 2))
        {
            /* 超出buffer 范围*/
            break;
        }
        value = Read_Data_Uni(byte_start + i);
        *(uint16*) (&freq_string_uni[i]) = value;
    }

    //将获取到的unicode 字符舍弃高字节，获得ASCILL
    //因为此函数只供获取电台号和频率所用
    for (i = 0; i < (byte_cnt / 2); i++)
    {
        temp = freq_string_uni[i * 2];
        freq_string_uni[i] = temp;
    }
    for (; i < byte_cnt; i++)
    {
        freq_string_uni[i] = 0;
    }
    return;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \void get_all_freqinfo_uni(void)
 * \获取用户电台列表中所有的频点信息
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \retval
 * \retval
 * \note
 */
/*******************************************************************************/
void get_all_freqinfo_uni(void)
{
    uint8 cnt;

    //用户电台列表频率信息清空
    libc_memset(g_user_station.fmstation_user, 0, MAX_STATION_COUNT * 2);

    /* 用户电台列表中电台总数*/
    g_userlist_total = get_need_filenum_uni(ALL_FREQ, 0);
    if (g_userlist_total > MAX_STATION_COUNT)
    {
        //支持最多添加30 个用户电台
        g_userlist_total = MAX_STATION_COUNT;
    }

    /*定位到第一个电台信息开始位置，index 0*/
    get_need_filenum_uni(INDEX_FROM_START, 0);

    //循环获取各个用户电台的频率信息，到g_user_station.fmstation_user[]
    //没有的电台，频点值填充为0
    for (cnt = 0; cnt < g_userlist_total; cnt++)
    {
        /*获取8  字节电台号*/
        get_cur_data_uni(STANUM_LENTH * 2);
        deal_station_num_uni(&freq_string_uni[2], cnt);
        /* 定位到频率值开始处*/
        seek_to_pos_uni(0);
        get_cur_data_uni(FREQ_LENTH * 2);
        /*保存用户电台频点*/
        deal_user_freq_uni(&freq_string_uni[0], cnt);
        get_need_filenum_uni(NEXT_FROM_CUR, 0); /*定位到下一个电台开始位置*/
    }
}

/******************************************************************************/
/*!
 * \par  Description:
 * void deal_station_info_uni(void)
 * \ 获取用户电台的电台信息，用于播放界面显示
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \retval
 * \retval
 * \note  获取当前用户电台名称到g_user_station.station_name[40]
 */
/*******************************************************************************/
void deal_station_info_uni(void)
{
    uint16 value;
    uint8 i;
    uint32 byte_start;
    byte_start = cur_offset_uni;

    /* 清空用户电台名称buffer */
    libc_memset(g_user_station.station_name, 0, 40);
    for (i = 0; i < 38; i = i + 2)
    {
        value = Read_Data_Uni(byte_start + i);
        if (value != 0x000D)
        {
            *(uint16*) (&g_user_station.station_name[i]) = value;
        }
        else
        {
            break;
        }
    }
    *(uint16*) (&g_user_station.station_name[i]) = 0; //结束符
    return;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool parse_userlist_uni(char* station_name, userlist_parse_e mode, uint8 num)
 * \对unicode 编码的用户电台列表进行解析
 * \param[in]    char* station_name，mode，num
 * \param[out]   none
 * \return       int the result
 * \retval           1 sucess
 * \retval           0 failed
 * \note
 */
/*******************************************************************************/
bool parse_userlist_uni(char* station_name, userlist_parse_e mode, uint8 num)
{
    bool ret = TRUE;

    //初始化变量为-1	，buffer 数据不能直接使用
    cursec_num_uni = 0xffff;
    cur_offset_uni = 0;

    //所有频点频率值解析，用于用户电台列表显示
    if (mode == ALL_FREQ)
    {
        get_all_freqinfo_uni();
    }
    //根据索引号，获取电台名称显示
    //此时，索引号和电台号的映射表已经建立，可直接获得电台号
    //根据电台号，可获得频率值
    //所以，用户电台列表上下切换时，可先得到频点开始播放
    //再获取名称显示
    else if (mode == INDEX_FROM_START)
    {
        /* 定位到该项索引开始位置CH*/
        get_need_filenum_uni(INDEX_FROM_START, num);
        /* 向后找到','，定位到名称开始位置*/
        seek_to_pos_uni(1);
        deal_station_info_uni();
    }
    //NEXT_FROM_CUR 模式供内部使用
    else
    {
        ret = FALSE;
    }
    return ret;
}

