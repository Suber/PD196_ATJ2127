/*
 ******************************************************************************
 *                               US212A
 *                            Module: Radio
 *                 Copyright(c) 2002-2010 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * Descripton: main module of radio
 * Module:  AR1019/AR1010
 * History:
 *      <author>    <time>           <version >             <desc>
 *       mikeyang    2012-1-25  14:35     1.0             build this file
 ******************************************************************************
 */
#include "fm_drv.h"

/* 实际传输所用buffer */
uint8 TransBuffer[38];
/* 写buffer */
uint8 WriteBuffer[36];
/* 读buffer */
uint8 ReadBuffer[2];

//CLK 24M
/***********************************************************
 *Description: R0~R17
 * using external reference clock
 * 75-us de-emphasis,  disable smute & hmute
 * freq 87.5MHz,
 * seek up,  seek disable, space 100k, US/Europe band, seekth 16
 * volume control in R3&R14,
 * CLK  24MHz
 * seek_wrap:  no wrap
 ************************************************************/
const uint8 InitBuffer[36] =
{ 0xFF, 0x7B, 0x5B, 0x11, 0xD4, 0xB9, 0xA0, 0x10, 0x07, 0x80, 0x28, 0xAB, 0x64, 0x00, 0x1f, 0x87, 0x71, 0x41, 0x00,
        0x80, 0x81, 0xc6, 0x4F, 0x55, 0x97, 0x0c, 0xb8, 0x45, 0xfc, 0x2d, 0x80, 0x97, 0x04, 0xE1, 0xDF, 0x6A
};

//CLK 32k	,disable seek wrap
/***********************************************************
 *Description: R0~R17
 * using external reference clock
 * 75-us de-emphasis,  disable smute & hmute
 * freq 87.5MHz,
 * seek up,  seek disable, space 100k, US/Europe band, seekth 16
 * volume control in R3&R14,
 * CLK  32.768KHz
 * seek_wrap:  no wrap
 ************************************************************/
/*const uint8 InitBuffer[36]={0xFF,0x7B,0x5B,0x11,0xD4,0xB9,0xA0,0x10,0x07,0x80,0x28,0xAB,0x64,0x00,0x1e,0xe7,0x71,0x41,
 0x00,0x7d,0x82,0xc6,0x4F,0x55,0x97,0x0c,0xb8,0x45,0xfc,0x2d,0x80,0x97,0x04,0xA1,0xDF,0x6A};*/

/* CHAN<9:0>*/
uint16 Freq_RF;
/* 电台信号强度*/
uint16 signal_stg;
/* 当前波段信息*/
uint8 Set_Band;
/* 是否采用50K 步长*/
uint8 Space50k;
/* 电台RSSI */
uint8 RSSI2;
/* 电台RSSI */
uint8 RSSI1;
/* 驱动层:   电台立体声信息  0~~立体声   1~~单声道*/
uint8 ST_info;
/* 硬件seek 状态标识*/
uint8 hardseek_flag;

uint8 WriteOneFrame(uint8 Addr, uint8 Count);
uint8 ReadOneFrame(uint8 Addr);
uint8 WaitSTC(void);
uint8 CheckStation(void);
uint8 FM_SetFrequency(uint16 Freq);
uint8 WriteRegister(uint8 Addr, uint8 Count);
uint8 ReadRegister(uint8 RegAddr);
uint8 TuneControl(uint8 mode);

extern uint16 FreqToChan(uint16 freq, uint8 space)
__FAR__;
extern uint16 ChanToFreq(uint16 chan, uint8 space)
__FAR__;

/*
 ********************************************************************************
 *  int sFM_SetFreq(uint16 freq, void* null2, void* null3)
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

    TempFreq = FreqToChan(freq, Space50k); //将电台频率转换成寄存器需写入的值
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

    Engine_Status_t * ptr_buf = (Engine_Status_t *)pstruct_buf;
    uint8 tmp;

    //硬件seek 过程中，直接取寄存器信息
    if(mode == 1)
    {
        //read status reg
        result = ReadRegister(0x13);
    }
    //非硬件seek 过程中，需等待STC

    else
    {
        result = WaitSTC();
    }

    if(result == 1)
    {
        Freq_RF = (uint16)ReadBuffer[0] * 4;
        tmp = (uint8)((ReadBuffer[1] & 0x80)>>6);
        tmp +=(ReadBuffer[1] & 0x01);
        //CHAN<9:0>
        Freq_RF += tmp;
        ptr_buf->FM_CurrentFreq = ChanToFreq(Freq_RF, Space50k);

        tmp = ReadBuffer[1] & 0x08;
        if(tmp==0)
        {
            //mono
            ST_info = 1;
        }
        else
        {
            //stereo
            ST_info = 0;
        }
        ptr_buf->FM_Stereo_Status = (FM_Audio_e)ST_info;

        //STC Flag
        hardseek_flag = (ReadBuffer[1] & 0x20)>>5;
        if(mode!=1)
        {
            hardseek_flag = 1;
        }
        if(hardseek_flag==0)
        {
            ptr_buf->STC_flag = HARDSEEK_DOING;
        }
        else
        {
            ptr_buf->STC_flag = HARDSEEK_COMPLETE;
        }

        if(Set_Band == 0x10)
        {
            ptr_buf->FM_CurBand = Band_Japan;
        }
        else
        {
            if(Space50k == 0x01)
            {
                ptr_buf->FM_CurBand = Band_Europe;
            }
            else
            {
                ptr_buf->FM_CurBand = Band_China_US;
            }
        }

        result = ReadRegister(0x12);
        signal_stg = (uint16)((ReadBuffer[0] & 0xfe)>>1);
        ptr_buf->FM_SignalStg = signal_stg;
        result =1;
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
 * Arguments :  软件搜台方式,需设置的频率值及搜索方向
 *
 * Returns : 电台为真台,则返回1;否则,返回0
 *
 * Notes :
 *
 ********************************************************************************
 */
int sFM_Search(uint16 freq, uint8 direct, void* null3)
{
    int result;
    uint16 TempFreq;
    uint16 freq_tmp;
    uint8 chan_bit0;
    uint8 ret;

    //切换到single_seek 模式
    WriteBuffer[28] |= 0x02;
    ret=WriteRegister(0x0E,0x02);

    sFM_Mute(SetMUTE, 0, 0);

    if(direct == 0)
    {
        //seek down
        if(Space50k == 0x01)
        {
            freq_tmp = freq +50;
        }
        else
        {
            freq_tmp = freq +100;
        }
    }
    else
    {
        //seek up
        if(Space50k == 0x01)
        {
            freq_tmp = freq -50;
        }
        else
        {
            freq_tmp = freq -100;
        }
    }
    TempFreq = FreqToChan(freq_tmp, Space50k); //将电台频率转换成寄存器需写入的值
    chan_bit0 = (uint8)(TempFreq%2);
    freq_tmp = (TempFreq>>1); // CHAN<9:1>

    WriteBuffer[4] &= (uint8)0xf4; //disable tune
    WriteBuffer[4] |= (chan_bit0<<3);
    WriteBuffer[4] |= (uint8)(freq_tmp>>8);
    WriteBuffer[5] = (uint8)(freq_tmp%256); //update chan
    ret=WriteRegister(0x02,0x02);

    WriteBuffer[6] &= (uint8)~(0x01<<6); //disable seek
    ret=WriteRegister(0x03,0x02);

    WriteBuffer[6] &= 0x7f; //设置SEEK direct
    WriteBuffer[6] |= (uint8)(direct<<7); //设置SEEK direct
    WriteBuffer[6] |= (uint8)(0x01<<6); //enable SEEK
    ret=WriteRegister(0x03,0x02);

    //已经启动single_seek
    if (ret != 0)
    {
        //软件搜台，判断所设置频点是否有效电台
        if (CheckStation() == 1) // check station

        {
            //0x17700~~96MHz, 过滤96MHz 频点
            if( freq !=0x7700)
            {
                result = 1;
            }
            else
            {
                result = 0;
            }
        }
        else
        {
            result = 0;
        }

    }
    else
    {
        result = 0;
    }

    //切换回正常模式
    WriteBuffer[28] &= (uint8)0xfd;
    ret=WriteRegister(0x0E,0x02);
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
    uint16 TempFreq;
    uint8 chan_bit0;
    uint16 freq_tmp;

    //默认硬件seek  尚未启动
    hardseek_flag = 1;

    //tune 到起始频点
    TempFreq = FreqToChan(freq, Space50k); //将电台频率转换成寄存器需写入的值
    chan_bit0 = (uint8)(TempFreq % 2);
    freq_tmp = (TempFreq >> (uint8) 1); // CHAN<9:1>

    WriteBuffer[4] &= (uint8) 0xf4; //disable tune
    WriteBuffer[4] |= (chan_bit0 << 3);
    WriteBuffer[4] |= (uint8)(freq_tmp >> 8);
    WriteBuffer[5] = (uint8)(freq_tmp % 256); //update chan
    result = WriteRegister(0x02, 0x02);

    WriteBuffer[6] &= (uint8) ~(0x01 << 6); //disable seek
    result = WriteRegister(0x03, 0x02);

    if (result != 0)
    {
        temp = direct & 0x01;
        //seek down
        if (temp != 0)
        {
            //R3--bit15
            WriteBuffer[6] &= (uint8) 0x7f;
        }
        //seek up
        else
        {
            WriteBuffer[6] |= (uint8) 0x80;
        }

        temp = direct & 0x02;
        //不回绕
        if (temp != 0)
        {
            //R10--bit3
            WriteBuffer[21] &= (uint8) 0xf7;
        }
        //回绕
        else
        {
            WriteBuffer[21] |= (uint8) 0x08;
        }
        WriteBuffer[1] |= 0x01;
        WriteBuffer[6] |= 0x40; //start seek
        result = WriteRegister(0x01, 20); //R1~R10
        if (result != 0)
        {
            //硬件seek 已启动，未完成
            hardseek_flag = 0;
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
 *             uint8 CheckStation(void)
 *
 * Description : 真台判断
 *
 * Arguments   :
 *
 * Returns     :  若搜到的电台为真台,返回1;否则,返回0
 *
 * Notes       :
 *
 ********************************************************************************
 */
uint8 CheckStation(void)
{
    uint8 result;
    uint8 temp;
    uint8 tmp;

    result = WaitSTC(); //wait STC flag
    if (result != 0)
    {
        temp = ReadBuffer[1] & 0x10; // 判断的是SF  这一位
        if (temp == 0)
        {
            Freq_RF = (uint16) ReadBuffer[0] * 4;
            tmp = (ReadBuffer[1] & (uint8) 0x80) >> 6;
            tmp += (ReadBuffer[1] & 0x01);
            //CHAN<9:0>
            Freq_RF += tmp;
            return 1;
        }
    }
    return 0;
}

/*
 ********************************************************************************
 *             uint8 FM_SetFrequency(uint16 Freq)
 *
 * Description : 频点设置
 *
 * Arguments   :  需设置的频率值 CHAN <9:0>
 *
 * Returns     :   若频点设置成功，返回1;否则,返回0
 *
 * Notes       :
 *
 ********************************************************************************
 */
uint8 FM_SetFrequency(uint16 Freq)
{
    uint8 result;
    uint8 chan_bit0;
    uint16 freq_tmp;

    chan_bit0 = (uint8)(Freq % 2);
    freq_tmp = (Freq >> 1); // CHAN<9:1>
    WriteBuffer[4] &= (uint8) 0xfd; //clear tune
    WriteBuffer[6] &= (uint8) ~(0x01 << 6); //clear SEEK
    result = WriteRegister(0x02, 0x04);

    WriteBuffer[22] &= 0x7f; //Read Low-side LO Injection
    WriteBuffer[23] &= (uint8) 0xfa;
    result = WriteRegister(0x0b, 0x02);

    //set band/space/chan,enable tune
    WriteBuffer[4] &= (uint8) 0xf6;
    WriteBuffer[4] |= (chan_bit0 << 3);
    WriteBuffer[4] |= (uint8)(freq_tmp >> 8);
    WriteBuffer[5] = (uint8)(freq_tmp % 256);
    result = TuneControl(1); //start tune

    result = WaitSTC(); //wait STC flag
    result = ReadRegister(0x12); //get RSSI (RSSI1)
    RSSI1 = ReadBuffer[0];

    //clear tune
    result = TuneControl(0);

    WriteBuffer[22] |= (uint8) 0x80; //Read High-side LO Injection
    WriteBuffer[23] |= 0x05;
    result = WriteRegister(0x0b, 0x02);

    //enable tune
    result = TuneControl(1);

    result = WaitSTC();
    result = ReadRegister(0x12); //get RSSI (RSSI2)
    RSSI2 = ReadBuffer[0];

    //clear tune
    result = TuneControl(0);

    if (RSSI1 >= RSSI2) //Set D15,  Clear D0/D2
    {
        WriteBuffer[22] |= (uint8) 0x80;
        WriteBuffer[23] &= (uint8) 0xfa;
    }
    else //Clear D15, Set D0/D2
    {
        WriteBuffer[22] &= 0x7f;
        WriteBuffer[23] |= 0x05;
    }
    result = WriteRegister(0x0b, 0x02);

    //enable tune
    result = TuneControl(1);
    if (result != 0)
    {
        result = WaitSTC();
    }

    return result;
}

/*
 ********************************************************************************
 *             uint8 WaitSTC(void)
 *
 * Description : wait STC flag,tune or seek complete
 *
 * Arguments   :
 *
 * Returns     :  若STC标志置1，则返回1，否则，返回0
 *
 * Notes       :
 *
 ********************************************************************************
 */
uint8 WaitSTC(void)
{
    uint8 result;
    uint8 temp;
    uint8 count = 0;
    sys_mdelay(50); //延时50ms，等待TUNE OR SEEK 结束
    do
    {
        CLEAR_WATCHDOG();
        result = ReadRegister(0x13); // read status reg
        temp = ReadBuffer[1] | (uint8) 0xdf; //STC
        if (temp == 0xff)
        {
            return 1; //STC =1
        }
        count++;
        sys_mdelay(20);
    } while (/*(temp != (uint8)0xff) && (count < 255)*/count < 255);
    return 0;
}

/*
 ********************************************************************************
 *           uint8 WriteOneFrame(uint8 Addr,uint8 Count)
 *
 * Description : 从指定地址的寄存器开始，写指定长度字节的
 *                    数据到寄存器
 *
 * Arguments :   Addr:  指定地址的寄存器   R0(Addr=0), R1(Addr=1)...
 *                     Count:  写指定长度
 *
 * Returns :  成功:  返回1；失败:  返回0
 *
 * Notes :
 *
 ********************************************************************************
 */
uint8 WriteOneFrame(uint8 Addr, uint8 Count)
{
    uint8 i, result;

    if ((Count > 36) || ((Count + 2* Addr ) > 36))
    {
        result = 0;
        return result;
    }
    //先发设备地址，再发从哪个寄存器地址开始写
    TransBuffer[0] = 0x20;
    TransBuffer[1] = Addr;
    //后面就持续写入需写的寄存器内容
    for (i = 0; i < Count; i++)
    {
        //从register table 看，一个Addr  为两个字节
        TransBuffer[2 + i] = WriteBuffer[i + 2* Addr ];
    }
    result = I2C_Trans_Bytes(TransBuffer, Count + 2, I2C_gpio, &delay_i2c);
    return result;

}

/*
 ********************************************************************************
 *           uint8 ReadOneFrame(uint8 RegAddr)
 *
 * Description : 读指定地址的寄存器内容
 *
 * Arguments :   RegAddr:  指定地址的寄存器   R0(Addr=0), R1(Addr=1)...
 *
 *
 * Returns :  成功:  返回1；失败:  返回0
 *
 * Notes :
 *
 ********************************************************************************
 */
uint8 ReadOneFrame(uint8 RegAddr)
{
    uint8 result = 0;

    IIC_Init(I2C_gpio, &delay_i2c);

    IIC_Start(I2C_gpio, &delay_i2c);
    //写设备地址
    IIC_WriteByte(0x20, I2C_gpio, &delay_i2c);
    if (IIC_GetAck(I2C_gpio, &delay_i2c) != 0)
    {
        //需读取的寄存器地址
        IIC_WriteByte(RegAddr, I2C_gpio, &delay_i2c);
        if (IIC_GetAck(I2C_gpio, &delay_i2c) != 0)
        {
            IIC_Start(I2C_gpio, &delay_i2c);
            //设备地址~  读
            IIC_WriteByte(0x21, I2C_gpio, &delay_i2c);
            if (IIC_GetAck(I2C_gpio, &delay_i2c) != 0)
            {
                ReadBuffer[0] = IIC_ReadByte(I2C_gpio, &delay_i2c);
                IIC_SendAck(0, I2C_gpio, &delay_i2c);
                ReadBuffer[1] = IIC_ReadByte(I2C_gpio, &delay_i2c);
                IIC_SendAck(1, I2C_gpio, &delay_i2c);
                result = 1;
            }
        }
    }
    IIC_Stop(I2C_gpio, &delay_i2c);

    return result;

}

/*
 ********************************************************************************
 *           void SwitchGPIO(void)
 *
 * Description : 将GPIO disable
 *
 * Arguments : NULL
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
 *           uint8 WriteRegister(uint8 Addr,uint8 Count)
 *
 * Description : 写FM 模组寄存器
 *
 * Arguments :   Addr:  寄存器地址 count:写寄存器的个数
 *
 * Returns :  成功:  返回1；失败:  返回0
 *
 * Notes :
 *
 ********************************************************************************
 */
uint8 WriteRegister(uint8 Addr, uint8 Count)
{
    uint8 i;
    uint8 result;
    uint32 flags;
    uint8 ret;

    //关中断
    flags = sys_local_irq_save();
    //成功返回，不成功可试3   次
    for (i = 0; i < 3; i++)
    {
        result = WriteOneFrame(Addr, Count);
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
 *           uint8 ReadRegister(uint8 RegAddr)
 *
 * Description : 读FM 模组的一个寄存器
 *
 * Arguments :   RegAddr:  寄存器地址
 *
 * Returns :  成功:  返回1；失败:  返回0
 *
 * Notes :
 *
 ********************************************************************************
 */
uint8 ReadRegister(uint8 RegAddr)
{
    uint8 i;
    uint8 result;
    uint8 ret;
    uint32 flags;
    //关中断
    flags = sys_local_irq_save();
    //成功返回，不成功可试3   次
    for (i = 0; i < 3; i++)
    {
        result = ReadOneFrame(RegAddr);
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
 *           uint8 TuneControl(uint8 mode)
 *
 * Description : tune 控制
 *
 * Arguments :   mode   0: clear tune;   1: start tune
 *
 * Returns :  成功:  返回1；失败:  返回0
 *
 * Notes :
 *
 ********************************************************************************
 */
uint8 TuneControl(uint8 mode)
{
    if (mode == 0)
    {
        WriteBuffer[4] &= ~(0x01 << 1); //clear tune
    }
    else
    {
        WriteBuffer[4] |= (0x01 << 1); //start tune
    }
    return WriteRegister(0x02, 0x02);

}

/*
 ********************************************************************************
 *             int  sFM_GetHardSeekflag(void* flag, void* null2, void* null3)
 *
 * Description : 获取硬件seek 过程是否结束标志
 *
 * Arguments : 保存结束标志的指针，1  硬件seek 结束
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
    result = ReadRegister(0x13);
    if (result == 1)
    {
        //STC Flag
        hardseek_flag = (ReadBuffer[1] & 0x20) >> 5;
        //seek 过程结束
        if (hardseek_flag == 0x01)
        {
            *stc_flag = 0x01;
            if ((ReadBuffer[1] & 0x10) == 0) //SF = 0
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
 * Arguments :
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

    result = ReadRegister(0x13);
    if (result == 1)
    {
        //STC Flag
        hardseek_flag = (ReadBuffer[1] & 0x20) >> 5;
        //仍处于硬件seek 过程中
        if (hardseek_flag == 0)
        {
            WriteBuffer[6] &= (uint8) 0xbf; //stop seek
            result = WriteRegister(0x03, 0x02); //写相关寄存器
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

