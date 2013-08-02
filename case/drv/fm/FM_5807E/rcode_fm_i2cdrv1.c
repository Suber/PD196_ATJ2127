/*
 ******************************************************************************
 *                               US212A
 *                            Module: Radio
 *                 Copyright(c) 2002-2010 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * Descripton: main module of radio
 * Module:  RDA5807E
 * History:
 *      <author>    <time>           <version >             <desc>
 *       Mikeyang    2010-11-25 14:35     1.0             build this file
 ******************************************************************************
 */
#include "fm_drv.h"

//初始化值 for 5807hp
const uint8 InitBuffer_HP[110] =
{ 0xc4, 0x51, //02h:         bit10 ???? ????1.8v samsung flash
        //0xc0, 0x01,	  //
        0x00, 0x00, 0x04, 0x00, 0x86, 0xad, //05h://86
        0x40, 0x00, 0x56, 0xc6, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //0x0ah
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, //0x10h
        0x00, 0x19,// //0x00,0x09,//0830
        0x2a, 0x11, 0x00, 0x2e, 0x2a, 0x30, 0xb8, 0x3c, //0x15h
        0x90, 0x00, 0x2a, 0x91, 0x84, 0x12, 0x00, 0xa8, 0xc4, 0x00, //0x1ah
        0xe0, 0x00, 0x30, 0x1d,//0x24,0x9d,1ch
        0x81, 0x6a, 0x46, 0x08, 0x00, 0x86, //0x1fh
        0x06, 0x61,// 0x16,0x61, 20h
        0x00, 0x00, 0x10, 0x9e, 0x20, 0x40,//   0x24,0x47,//0830//23h
        0x04, 0x08,//0830
        0x06, 0x08, //0x06,0x08,//0830  //0x25h
        0xe1, 0x05, 0x3b, 0x6c, 0x2b, 0xec, 0x09, 0x0f, 0x34, 0x14, //0x2ah
        0x14, 0x50, 0x09, 0x6d, 0x2d, 0x96, 0x01, 0xda, 0x2a, 0x7b, 0x08, 0x21, //0x30h
        0x13, 0xd5, 0x48, 0x91, 0x00, 0xbc, 0x08, 0x96,//0830  0x34h
        0x15, 0x3c, 0x0b, 0x80, 0x25, 0xc7, 0x00, 0x00,
};

//初始化值for 5807P  32k
const uint8 InitBuffer_NP[64] =
{ 0xC0, 0x01, //0x51, //02H:
        0x00, 0x00, 0x04, 0x00, 0x8b, 0xAF, //05H:
        0x00, 0x00, 0x46, 0xC6, 0x50, 0x96, 0x00, 0x00, 0x40, 0x00, //0AH:
        0x00, 0x8F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF0, 0x05, //10H:
        0x90, 0x00, 0xF4, 0x84, 0x70, 0x01, 0x40, 0xF0, 0x21, 0x80, //15H:
        0x7A, 0xD0, 0x3E, 0x40, 0x55, 0xA9, 0xE8, 0x48, 0x50, 0x80, //1AH:
        0x00, 0x00, 0x37, 0xB6, 0x40, 0x0C, 0x07, 0x9B, 0x4C, 0x1D, 0x81, 0x11, //20H:
        0x45, 0x80,
};

//初始化值for 5807sp  24M --- 0xC4, 0x51;     32k --- 0xC0, 0x01
const uint8 InitBuffer_SP[104] =
{ //0xC4, 0x51, //02H:
    0xC0, 0x01, //02H:
            0x1b, 0x90, 0x04, 0x00, 0x86, 0xad, //05H://86
            0x80, 0x00, 0x5F, 0x1A, //07H  change the seek time;
            0x5e, 0xc6, 0x00, 0x00, 0x40, 0x6e, //0AH:
            0x2d, 0x80, 0x58, 0x03, 0x58, 0x04, 0x58, 0x04, 0x58, 0x04, 0x00, 0x47, //10H:
            0x90, 0x00, 0xF5, 0x87, 0x7F, 0x0B, //13H: reset ADC's parameters
            0x04, 0xF1, 0x5E, 0xc0, //15H: 0x42, 0xc0
            0x41, 0xe0, 0x50, 0x6f, 0x55, 0x92, 0x00, 0x7d, 0x10, 0xC0, // 1AH
            0x07, 0x80, 0x41, 0x1d, // 1CH,
            0x40, 0x06, 0x1f, 0x9B, 0x4c, 0x2b, // 1FH.
            0x81, 0x10, //20H:
            0x45, 0xa0, // 21H
            0x19, 0x3F, //22H: change AFC threshold
            0xaf, 0x40, 0x06, 0x81, 0x1b, 0x2a, //25H
            0x0D, 0x04, //26H, shutdown Rssi_autodown function
            0x80, 0x9F, //0x80, 0x2F,
            0x17, 0x8A, 0xD3, 0x49, 0x11, 0x42, 0xA0, 0xC4, // 2BH
            0x3E, 0xBB, 0x00, 0x00, 0x58, 0x04, 0x58, 0x04, // 2FH
            0x58, 0x04, 0x00, 0x74, 0x3D, 0x00, 0x03, 0x0C, 0x2F, 0x68, 0x38, 0x77, //35H
};

uint16 Freq_RF; //Freq infor read from reg, 0ah bit[9:0],freq = FREQ_RF*channel sapcing + 87M(76M)
uint16 signal_stg; //驱动层，电台信号强度信息
uint8 Set_Band; //Band,03 h bit[3:2](00 = US,01 = Japan)
uint8 ST_info; //驱动层:   电台立体声信息  0~~立体声   1~~单声道
uint8 hardseek_flag; //是否处于硬件seek 过程中的标志，0:  处于硬件seek 过程中   1:  不处于硬件seek 过程中
uint8 WriteNum; //不同模组初始化时需写入字节数
uint8 RDA_model; //模组型号:    1  RDA 5807SP      2   RDA 5807HP    3  RDA 5807P

uint8 ReadBuffer[10]; //读buffer
uint8 WriteBuffer[111]; //写buffer

int FM_SetFrequency(uint16 Freq);
uint8 CheckStation(void); //检查电台有效性
uint8 WaitFlag(uint8 mode); //等待指定标识
uint8 ReadRegister(uint8 count); //读寄存器
int WriteRegister(uint8 count);//写寄存器

void DisAssemble_WriteBuffer(void);
uint16 FreqToChan(uint16 freq, uint8 mode);
uint16 ChanToFreq(uint16 Chan, uint8 mode);

/*
 ********************************************************************************
 * int sFM_SetFreq(uint16 freq, void* null2, void* null3)
 *
 * Description : 设置频点,进行tune操作
 *
 * Arguments : 需设置的频点值( 实际频率khz 为单位的低2 字节)
 *
 * Returns :   成功:  返回1； 失败:  返回0
 *
 * Notes :
 *
 ********************************************************************************
 */
int sFM_SetFreq(uint16 freq, void* null2, void* null3)
{
    int result;
    uint16 TempFreq;

    TempFreq = FreqToChan(freq, Set_Band); //将电台频率转换成寄存器需写入的值
    result = FM_SetFrequency(TempFreq);
    return result;
}

/*
 ********************************************************************************
 * int sFM_GetStatus(void * pstruct_buf, uint8 mode, void* null3)
 *
 * Description : 获取当前电台的相关信息，包括当前电台频率
 *                   信号强度值，立体声状态，当前波段
 *
 * Arguments : pstruct_buf   保存电台信息的结构体指针
 *                   mode==0,  正常播放( 非seek 过程中)  取信息
 *                   mode==1,  在硬件seek 过程中取信息
 * Returns : //是否读取状态成功,如果读取成功,则返回值为1
 * 否则,返回0
 * Notes :
 *
 ********************************************************************************
 */
int sFM_GetStatus(void * pstruct_buf, uint8 mode, void* null3)
{
    int result;
    Engine_Status_t * ptr_buf = (Engine_Status_t *) pstruct_buf;

    //硬件seek 过程中，直接取寄存器信息
    if (mode == 1)
    {
        result = ReadRegister(4);
    }
    //非硬件seek 过程中，需等待STC
    else
    {
        result = (int) WaitFlag(0);
    }
    if (result == 1)
    {
        DisAssemble_WriteBuffer();
        ptr_buf->FM_CurrentFreq = ChanToFreq(Freq_RF, Set_Band);
        ptr_buf->FM_SignalStg = signal_stg;
        if (Set_Band == 0x04)
        {
            ptr_buf->FM_CurBand = Band_Japan;
        }
        else if (Set_Band == 0x02)
        {
            ptr_buf->FM_CurBand = Band_Europe;
        }
        else
        {
            ptr_buf->FM_CurBand = Band_China_US;
        }
        ptr_buf->FM_Stereo_Status = (FM_Audio_e) ST_info;

        if (mode != 1)
        {
            //非硬件seek 过程中
            hardseek_flag = 1;
        }

        if (hardseek_flag == 0)
        {
            ptr_buf->STC_flag = HARDSEEK_DOING;
        }
        else
        {
            ptr_buf->STC_flag = HARDSEEK_COMPLETE;
        }
        result = 1;
    }
    else
    {
        result = 0;
    }
    return result;
}

/*
 ********************************************************************************
 *            int  sFM_Search(uint16 freq,  uint8 direct, void* null3)
 *
 * Description : FM  软件搜台，tune 单频点，判断是否有效电台
 *
 * Arguments :  软件搜台方式,需设置的频率值
 *
 * Returns : 电台为真台,则返回1;否则,返回0
 *
 * Notes : 此模组在tune 或seek 操作完成后，才能读取到更新的
 *            频率值，所以在硬件seek 过程中不能实时显示频率
 *            建议做软件seek
 ********************************************************************************
 */
int sFM_Search(uint16 freq, uint8 direct, void* null3)
{
    int result;

    direct = direct;
    result = sFM_SetFreq(freq, (void*) 0, (void*) 0);
    if (result != 0)
    {
        if (CheckStation() == 1) // check station
        {
            //当前频点为有效电台
            result = 1;
        }
        else
        {
            //当前为无效电台
            result = 0;
        }
    }
    else
    {
        result = 0;
    }
    return result;
}

/*
 ********************************************************************************
 *            int sFM_HardSeek(uint16 freq, uint8 direct, void* null3)
 *
 * Description : 启动FM 硬件搜台过程
 *
 * Arguments :
 param1: 硬件搜台方式，搜台起始频点
 param2: 搜索方向
 * Returns :  成功启动硬件搜台，返回1；否则返回0
 *
 * Notes :  搜台方向说明
 * bit0   ~~~  向上或向下搜台。0:   UP;    1: DOWN
 * bit1   ~~~  是否边界回绕。     0:   回绕;    1:  不回绕
 ********************************************************************************
 */
int sFM_HardSeek(uint16 freq, uint8 direct, void* null3)
{
    int result;
    uint8 temp;

    //默认硬件seek  尚未启动
    hardseek_flag = 1;

    //tune 到起始频点
    result = sFM_SetFreq(freq, (void*) 0, (void*) 0);
    if (result != 0)
    {
        temp = direct & 0x01;
        //seek down
        if (temp != 0)
        {
            WriteBuffer[1] &= (uint8) 0xfd;
        }
        //seek up
        else
        {
            WriteBuffer[1] |= 0x02;
        }

        temp = direct & 0x02;
        //不回绕
        if (temp != 0)
        {
            WriteBuffer[2] |= (uint8) 0x80;
        }
        //回绕
        else
        {
            WriteBuffer[2] &= 0x7f;
        }
        WriteBuffer[2] |= 0x01;
        WriteBuffer[1] |= 0x01; //start seek
        result = WriteRegister(2); //写相关寄存器
        if (result != 0)
        {
            //硬件seek 已启动
            hardseek_flag = 0;
        }
    }

    return result;
}

/*
 ********************************************************************************
 *             uint8 CheckStation(void)
 *
 * Description : 真台判断
 *
 * Arguments :
 *
 * Returns : 若搜到的电台为真台,返回1;否则,返回0
 *
 * Notes :
 *
 ********************************************************************************
 */
uint8 CheckStation(void)
{
    uint8 result;
    uint8 temp;
    //check fm ready flag
    result = WaitFlag(1);
    if (result != 0)
    {
        //软件搜台方式，真台判断处理，根据FM_TRUE 判断是否真台
        temp = (uint8)(ReadBuffer[2] & 0x01);
        if (temp == 0x01)
        {
            DisAssemble_WriteBuffer(); //读取电台频率

            //普通波段，96M 为假台
            if ((Set_Band == 0) && (Freq_RF >= 89) && (Freq_RF <= 91))
            {
                return 0;
            }
            //欧洲波段，96M 为假台
            else if ((Set_Band == 2) && (Freq_RF >= 178) && (Freq_RF <= 182))
            {
                return 0;
            }
            else
            {
                return 1;
            }
        }
    }
    return 0;
}

/*
 ********************************************************************************
 *             int FM_SetFrequency(uint16 Freq)
 *
 * Description : 频点设置
 *
 * Arguments : 需设置的频率值(  实际写入 FM  IC  寄存器)
 *
 * Returns : 若频点设置成功，返回1;否则,返回0
 *
 * Notes :
 *
 ********************************************************************************
 */
int FM_SetFrequency(uint16 Freq)
{
    int result = 0;
    WriteBuffer[1] &= (uint8) 0xfe; //禁止SEEK ,注意硬件SEEK方式,在设置频点\静音 时,必须disable seek!!!!
    WriteBuffer[2] |= (0x01 << 0); //Enable FM
    WriteBuffer[3] = (uint8)(Freq >> 8); //设置要写入的频率
    WriteBuffer[4] &= 0x3f;
    WriteBuffer[4] |= (uint8)(Freq % 256);
    WriteBuffer[4] |= (0x01 << 4); //enable tune
    result = WriteRegister(4); //设置频点,写相关寄存器
    if (result != 0)
    {
        //设置频率成功，等待tune 完成
        result = (int) WaitFlag(0);
    }
    return result;
}

/*
 ********************************************************************************
 *           void SwitchGPIO(void)
 *
 * Description : 将GPIO disable
 *
 * Arguments :  NULL
 *
 * Returns :  NULL
 *
 * Notes : 使用完I2C 模拟IO 口后，disable  其输入输出，
 *            不再用作GPIO , 避免影响其他硬件模块
 *
 ********************************************************************************
 */
void SwitchGPIO(void)
{
    IIC_SDA_Output_Dis(I2C_gpio);
    IIC_SDA_Input_Dis(I2C_gpio);
    IIC_SCL_Output_Dis(I2C_gpio);
    IIC_SCL_Input_Dis(I2C_gpio);
}

/*
 ********************************************************************************
 *           uint8 ReadRegister(uint8 count)
 *
 * Description : 读FM 模组寄存器
 *
 * Arguments :  需读取的字节数
 *
 * Returns :  成功:  返回1；失败:   返回0
 *
 * Notes :
 *
 ********************************************************************************
 */
uint8 ReadRegister(uint8 count)
{
    uint8 result;
    uint8 i;
    uint8 ret;
    uint32 flags;
    //关中断
    flags = sys_local_irq_save();

    for (i = 0; i < 3; i++)
    {
        result = I2C_Recev_Bytes(ReadBuffer, 0x21, count, I2C_gpio, &delay_i2c);
        if (result != 0)
        {
            ret = 1;
            break;
        }
    }
    if (i == 3)
    {
        ret = 0;
    }
    SwitchGPIO();
    //开中断
    sys_local_irq_restore(flags);
    return ret;
}

/*
 ********************************************************************************
 *           uint8 WaitFlag(uint8 mode)
 *
 * Description : 等待标志
 *
 * Arguments : mode:     0---STC Flag;    1---FM Ready Flag
 *
 * Returns :   成功等到指定标识,  返回1；否则返回0
 *
 * Notes :
 *
 ********************************************************************************
 */
uint8 WaitFlag(uint8 mode)
{
    uint8 result;
    uint8 temp = 0;
    uint8 count = 0;
    sys_mdelay(10); //延时10ms，等待FM Tune或SEEK完成
    do
    {
        result = ReadRegister(10);
        if (result != 0)
        {
            if (mode == 0)
            {
                temp = (uint8)(ReadBuffer[0] | 0xbf); //check stc flag
            }
            else
            {
                temp = (uint8)(ReadBuffer[3] | 0x7f); //check fm ready flag, used for soft seek
            }
            if (temp == 0xff)
            {
                return 1; //如果等到标志，则退出
            }
        }
        count++;
        sys_mdelay(10);
    } while (/*(temp != 0xff) && (count < 255)*/count < 255);
    return 0;
}

/*
 ********************************************************************************
 *           int WriteRegister(uint8 count)
 *
 * Description : 写FM 模组寄存器
 *
 * Arguments :   count:写寄存器的个数
 *
 * Returns :  成功:  返回1；失败:  返回0
 *
 * Notes :
 *
 ********************************************************************************
 */
int WriteRegister(uint8 count)
{
    uint8 i;
    uint8 result;
    uint32 flags;
    int ret;

    //关中断
    flags = sys_local_irq_save();

    //一次操作不成功，可以操作第二次
    for (i = 0; i < 3; i++)
    {
        /*要写入的字节数为写入寄存器的字节数加上I2C器件地址*/
        result = I2C_Trans_Bytes(WriteBuffer, count + 1, I2C_gpio, &delay_i2c);
        if (result != 0)
        {
            ret = 1;
            break;
        }
    }

    if (i == 3)
    {
        ret = 0;
    }
    SwitchGPIO();
    //开中断
    sys_local_irq_restore(flags);
    return ret;
}

/*
 ********************************************************************************
 *           void DisAssemble_WriteBuffer(void)
 *
 * Description : 将ReadBuffer  的内容转换为具体的信息
 *
 * Arguments :  none
 *
 * Returns :   none
 *
 * Notes :
 *
 ********************************************************************************
 */
void DisAssemble_WriteBuffer(void)
{
    //读取搜索到的电台频点
    Freq_RF = (uint16)(ReadBuffer[0] & 0x03);
    Freq_RF <<= 8;
    Freq_RF += ReadBuffer[1];

    //信号强度RSSI
    signal_stg = (uint16)((ReadBuffer[2] & 0xfe) >> 1);

    //立体声信息
    ST_info = (ReadBuffer[0] & 0x04) >> 2;
    ST_info = 1 - ST_info; //驱动IC  和上层定义相反

    //read STC，硬件seek 过程是否结束
    hardseek_flag = (ReadBuffer[0] & 0x40) >> 6;
    return;
}

/*
 ****************************************************************************************
 *           uint16 FreqToChan(uint16 freq, uint8 mode)
 *
 * Description : 将频点转换成写寄存器的值
 *
 * Arguments :  freq:  需进行转换的频率( 实际频率khz 为单位的低2 字节)
 *			   mode：频段
 * Returns :	   转换出来的要写入寄存器的电台值
 *
 * Notes :  各个频率值( 以1khz 为单位)  的最高bit  恒为1
 *
 *****************************************************************************************
 */
uint16 FreqToChan(uint16 freq, uint8 mode)
{
    uint16 Chan, tmp_freq;
    uint32 temp;

    /* 先将输入转换为以50khz 为单位的频率*/
    temp = (uint32) freq + 0x10000; //实际频率值，以khz 为单位
    tmp_freq = (uint16)(temp / 50);

    if (mode == 0) //中国/ 美国
    {
        Chan = ((tmp_freq / 2 - 870) << 6) + 0x0010;
    }
    else if (mode == 1) //日本
    {
        Chan = ((tmp_freq / 2 - 760) << 6) + 0x0010;
    }
    else //欧洲
    {
        Chan = ((tmp_freq - 870* 2 ) << 6) + 0x0010; 
    }
    return Chan;
}

/*
 ********************************************************************************
 *           uint16 ChanToFreq(uint16 chan, uint8 mode)
 *
 * Description : 将读出的寄存器频率值转换成上层需要的频率
 *
 * Arguments :  chan:读出的频率值
 *			   mode：频段
 *
 * Returns :	   转换出来的实际频点值(以1KHz 为单位的低2   字节)
 *
 * Notes :
 *
 ********************************************************************************
 */
uint16 ChanToFreq(uint16 chan, uint8 mode)
{
    uint16 Freq;
    uint32 temp;

    //先转换为以50khz 为单位频点
    if (mode == 0)
    {
        Freq = (chan + 870) * 2;
    }
    else if (mode == 1)
    {
        Freq = (chan + 760) * 2;
    }
    else
    {
        Freq = chan + 870* 2 ;
    }

    //转换为以1khz  为单位
    temp = Freq*50;
    Freq = (uint16)(temp & 0x0ffff);
    return Freq;
}

/*
 ********************************************************************************
 *             int  sFM_GetHardSeekflag(void* flag, void* null2, void* null3)
 *
 * Description : 获取硬件seek 过程是否结束标志
 *
 * Arguments : 保存结束标志的指针
 *
 * Returns : 若获取成功，返回1;否则,返回0
 *
 * Notes :   flag :  bit0  此轮硬件seek 是否结束    1:  已结束   0:  未结束
 *                       bit1  此轮硬件seek 找到的是否有效台   1:  有效台
 ********************************************************************************
 */
int sFM_GetHardSeekflag(void* flag, void* null2, void* null3)
{
    int result;
    uint8* stc_flag;

    stc_flag = (uint8*) flag;
    result = ReadRegister(4);
    if (result == 1)
    {
        DisAssemble_WriteBuffer();
        //seek 过程结束
        if (hardseek_flag == 0x01)
        {
            *stc_flag = 0x01;
            if ((ReadBuffer[0] & 0x20) == 0) //SF = 0
            {
                //seek 到有效台
                *stc_flag |= (0x01 << 1);
            }
        }
        //seek 过程未结束
        else
        {
            *stc_flag = 0;
        }
        result = 1;
    }
    else
    {
        *stc_flag = 0;
        result = 0;
    }
    return result;

}

/*
 ********************************************************************************
 *             int  sFM_BreakSeek(void* null1, void* null2, void* null3);
 *
 * Description : 硬件seek 过程中，手动停止seek 过程
 *
 * Arguments :  NULL
 *
 * Returns : 成功停止，返回1;否则,返回0
 *
 * Notes :
 *
 ********************************************************************************
 */
int sFM_BreakSeek(void* null1, void* null2, void* null3)
{
    int result;

    result = ReadRegister(4);
    if (result == 1)
    {
        DisAssemble_WriteBuffer();
        //仍处于硬件seek 过程中
        if (hardseek_flag == 0)
        {
            WriteBuffer[1] &= (uint8) 0xfe; //stop seek
            result = WriteRegister(2); //写相关寄存器
            if (result != 0)
            {
                hardseek_flag = 1;
                result = 1;
            }
            else
            {
                result = 0;
            }
        }
        //已经不处于硬件seek 过程，直接返回停止成功
        else
        {
            result = 1;
        }
    }
    else
    {
        result = 0;
    }
    return result;
}

