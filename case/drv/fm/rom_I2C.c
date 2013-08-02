/*******************************************************************************
 *                              US212A
 *                            Module: IIC driver
 *                 Copyright(c) 2003-2011 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>        <time>           <version >            <desc>
 *     mikeyang     2011-11-03 15:00     1.0             build this file
 *******************************************************************************/
#include "I2C.h"

void IIC_Init(gpio_init_cfg_t* gpio, uint8* delay);
void IIC_Exit(gpio_init_cfg_t* gpio);
void IIC_Delay(uint8* delay);
void IIC_SDA_high_hold(gpio_init_cfg_t* gpio, uint8* delay);
void IIC_SDA_low_hold(gpio_init_cfg_t* gpio, uint8* delay);
void IIC_SCL_high_hold(gpio_init_cfg_t* gpio, uint8* delay);
void IIC_SCL_low_hold(gpio_init_cfg_t* gpio, uint8* delay);

void IIC_SDA_Output_En(gpio_init_cfg_t* gpio);
void IIC_SDA_Output_Dis(gpio_init_cfg_t* gpio);
void IIC_SDA_Input_En(gpio_init_cfg_t* gpio);
void IIC_SDA_Input_Dis(gpio_init_cfg_t* gpio);
void IIC_SCL_Output_En(gpio_init_cfg_t* gpio);
void IIC_SCL_Output_Dis(gpio_init_cfg_t* gpio);
void IIC_SCL_Input_En(gpio_init_cfg_t* gpio);
void IIC_SCL_Input_Dis(gpio_init_cfg_t* gpio);
uint8 IIC_Data_Input(gpio_init_cfg_t* gpio);

void IIC_Start(gpio_init_cfg_t* gpio, uint8* delay);
void IIC_Stop(gpio_init_cfg_t* gpio, uint8* delay);

uint8 I2C_Trans_Bytes(uint8 *buf, uint8 length, gpio_init_cfg_t* gpio, uint8* delay);
uint8 I2C_Recev_Bytes(uint8 *buf, uint8 address, uint8 length, gpio_init_cfg_t* gpio, uint8* delay);
void IIC_WriteByte(uint8 dat, gpio_init_cfg_t* gpio, uint8* delay);
uint8 IIC_ReadByte(gpio_init_cfg_t* gpio, uint8* delay);
void IIC_SendAck(uint8 ack, gpio_init_cfg_t* gpio, uint8* delay);
uint8 IIC_GetAck(gpio_init_cfg_t* gpio, uint8* delay);

/*
 *******************************************************************************
 *             void IIC_Init(gpio_init_cfg_t* gpio, uint8* delay)
 *
 * Description : I2C初始化
 *
 * Arguments   : 模拟I2C  操作所用的IO ,  操作延时

 * Returns     : NULL

 * Notes       :
 *
 *******************************************************************************
 */
void IIC_Init(gpio_init_cfg_t* gpio, uint8* delay)
{
    IIC_SDA_high_hold(gpio, delay);
    IIC_SCL_high_hold(gpio, delay);
    IIC_SDA_Output_En(gpio);
    IIC_SCL_Output_En(gpio);
    IIC_SCL_Input_En(gpio);
    IIC_SDA_Input_En(gpio);

}

/*
 *******************************************************************************
 *             void IIC_Exit(gpio_init_cfg_t* gpio)
 *
 * Description : I2C 退出
 *
 * Arguments   : 模拟I2C  操作所用的IO 

 * Returns     : NULL

 * Notes       :
 *
 *******************************************************************************
 */
void IIC_Exit(gpio_init_cfg_t* gpio)
{
    IIC_SDA_Output_Dis(gpio);
    IIC_SDA_Input_Dis(gpio);
    IIC_SCL_Output_Dis(gpio);
    IIC_SCL_Input_Dis(gpio);
}

/************************************************************************
 * Description:  模拟IIC  总线IIC 操作延时
 * Input:   操作延时
 * Output: none
 * Note:
 *************************************************************************
 **/
void IIC_Delay(uint8* delay)
{
    uint8 j, value;

    value = *delay;
    for (j = 0; j < value; j++)
    {
        //延时函数
        //;
    }
}

/************************************************************************
 * Description:  模拟IIC  总线数据线拉高
 * Input:   模拟I2C  操作所用的IO ,  操作延时
 * Output: none
 * Note:
 *************************************************************************
 **/
void IIC_SDA_high_hold(gpio_init_cfg_t* gpio, uint8* delay)
{
    act_writel((act_readl((gpio + 1)->reg_data) | ((gpio + 1)->reg_bit)), (gpio + 1)->reg_data);
    IIC_Delay(delay);
}

/************************************************************************
 * Description:  模拟IIC  总线数据线拉低
 * Input:   模拟I2C  操作所用的IO ,  操作延时
 * Output: none
 * Note:
 *************************************************************************
 **/
void IIC_SDA_low_hold(gpio_init_cfg_t* gpio, uint8* delay)
{
    act_writel((act_readl((gpio + 1)->reg_data) & (~((gpio + 1)->reg_bit))), (gpio + 1)->reg_data);
    IIC_Delay(delay);
}

/************************************************************************
 * Description:  模拟IIC  总线时钟线拉高
 * Input:   模拟I2C  操作所用的IO ,  操作延时
 * Output: none
 * Note:
 *************************************************************************
 **/
void IIC_SCL_high_hold(gpio_init_cfg_t* gpio, uint8* delay)
{
    act_writel((act_readl(gpio->reg_data) | (gpio->reg_bit)), gpio->reg_data);
    IIC_Delay(delay);
}

/************************************************************************
 * Description:  模拟IIC  总线时钟线拉低
 * Input:   模拟I2C  操作所用的IO ,  操作延时
 * Output: none
 * Note:
 *************************************************************************
 **/
void IIC_SCL_low_hold(gpio_init_cfg_t* gpio, uint8* delay)
{
    act_writel((act_readl(gpio->reg_data) & (~(gpio->reg_bit))), gpio->reg_data);
    IIC_Delay(delay);
}

/************************************************************************
 * Description:  模拟IIC  总线数据线输出使能
 * Input:   模拟I2C  操作所用的IO 
 * Output: none
 * Note:
 *************************************************************************
 **/
void IIC_SDA_Output_En(gpio_init_cfg_t* gpio)
{
    act_writel((act_readl((gpio + 1)->reg_out) | ((gpio + 1)->reg_bit)), (gpio + 1)->reg_out);
}

/************************************************************************
 * Description:  模拟IIC  总线数据线输出disable
 * Input:   模拟I2C  操作所用的IO 
 * Output: none
 * Note:
 *************************************************************************
 **/
void IIC_SDA_Output_Dis(gpio_init_cfg_t* gpio)
{
    act_writel((act_readl((gpio + 1)->reg_out) & (~((gpio + 1)->reg_bit))), (gpio + 1)->reg_out);
}

/************************************************************************
 * Description:  模拟IIC  总线数据线输入使能
 * Input:  模拟I2C  操作所用的IO 
 * Output: none
 * Note:
 *************************************************************************
 **/
void IIC_SDA_Input_En(gpio_init_cfg_t* gpio)
{
    act_writel((act_readl((gpio + 1)->reg_in) | ((gpio + 1)->reg_bit)), (gpio + 1)->reg_in);
}

/************************************************************************
 * Description:  模拟IIC  总线数据线输入disable
 * Input:   模拟I2C  操作所用的IO 
 * Output: none
 * Note:
 *************************************************************************
 **/
void IIC_SDA_Input_Dis(gpio_init_cfg_t* gpio)
{
    act_writel((act_readl((gpio + 1)->reg_in) & (~((gpio + 1)->reg_bit))), (gpio + 1)->reg_in);
}

/************************************************************************
 * Description:  模拟IIC  总线时钟线输出使能
 * Input:   模拟I2C  操作所用的IO 
 * Output: none
 * Note:
 *************************************************************************
 **/
void IIC_SCL_Output_En(gpio_init_cfg_t* gpio)
{
    act_writel((act_readl(gpio->reg_out) | (gpio->reg_bit)), gpio->reg_out);
}

/************************************************************************
 * Description:  模拟IIC  总线时钟线输出disable
 * Input:   模拟I2C  操作所用的IO 
 * Output: none
 * Note:
 *************************************************************************
 **/
void IIC_SCL_Output_Dis(gpio_init_cfg_t* gpio)
{
    act_writel((act_readl(gpio->reg_out) & (~(gpio->reg_bit))), gpio->reg_out);
}

/************************************************************************
 * Description:  模拟IIC  总线时钟线输入使能
 * Input:   模拟I2C  操作所用的IO 
 * Output: none
 * Note:
 *************************************************************************
 **/
void IIC_SCL_Input_En(gpio_init_cfg_t* gpio)
{
    act_writel((act_readl(gpio->reg_in) | (gpio->reg_bit)), gpio->reg_in);
}

/************************************************************************
 * Description:  模拟IIC  总线时钟线输入disable
 * Input:   模拟I2C  操作所用的IO 
 * Output: none
 * Note:
 *************************************************************************
 **/
void IIC_SCL_Input_Dis(gpio_init_cfg_t* gpio)
{
    act_writel((act_readl(gpio->reg_in) & (~(gpio->reg_bit))), gpio->reg_in);
}

/************************************************************************
 * Description:  输入的32 bit 数中哪一个bit 为1
 * Input:   uint32
 * Output: 所找到的为1 的bit 位
 * Note:
 *************************************************************************
 **/
uint8 IIC_Data_Bit(uint32 value)
{
    uint8 loop;
    uint32 temp;
    for (loop = 0; loop < 32; loop++)
    {
        temp = (value >> loop) & 0x00000001;
        if (temp != 0)
        {
            break;
        }
    }
    return loop;
}

/************************************************************************
 * Description:  模拟IIC  总线采样数据线上数据
 * Input:   模拟I2C  操作所用的IO 
 * Output: 在数据线上采样到的bit 值，ret  bit0
 * Note:
 *************************************************************************
 **/
uint8 IIC_Data_Input(gpio_init_cfg_t* gpio)
{
    uint32 value;
    uint8 ret;
    value = act_readl((gpio + 1)->reg_data) & ((gpio + 1)->reg_bit);
    ret = IIC_Data_Bit((gpio + 1)->reg_bit);
    value = (value >> ret) & 0x00000001;
    ret = (uint8) value;
    return ret;
}

/*
 *******************************************************************************
 *             IIC_Start
 *
 * Description :  模拟IIC   Start  流程
 *
 * Arguments   : 模拟I2C  操作所用的IO ,  操作延时
 *
 * Returns     : NULL
 *
 * Notes       :
 *
 *******************************************************************************
 */
/*
 ____
 SDA:    |_______

 ______
 SCL:      |_____
 */

void IIC_Start(gpio_init_cfg_t* gpio, uint8* delay) /*IIC start command*/
{
    IIC_SDA_high_hold(gpio, delay); /*发送起始条件的数据信号*/
    IIC_SCL_high_hold(gpio, delay); /*起始条件建立时间大于4.7us,延时*/
    IIC_SDA_low_hold(gpio, delay); /*发送起始信号*//* 起始条件锁定时间大于4μs*/
    IIC_SCL_low_hold(gpio, delay); /*钳住IIC总线，SCL为低电平，准备发送或接收数据 */
}

/*
 *******************************************************************************
 *             IIC_Stop
 *
 * Description :  模拟IIC  Stop  流程
 *
 * Arguments   : 模拟I2C  操作所用的IO ,  操作延时
 *
 * Returns     : NULL
 *
 * Notes       :
 *
 *******************************************************************************
 */
/*
 _______
 SDA: _______|
 ________
 SCL: ______|
 */

void IIC_Stop(gpio_init_cfg_t* gpio, uint8* delay) /*IIC stop command*/
{
    IIC_SDA_low_hold(gpio, delay); /*发送结束条件的数据信号*/
    IIC_SCL_high_hold(gpio, delay); /*结束条件建立时间大于4μs*/
    IIC_SDA_high_hold(gpio, delay); /*发送IIC总线结束信号*/
}

//=============================================================================================
/*
 ******************************************************************************
 *             uint8 I2C_Trans_Bytes(uint8 *buf, uint8 length)
 *
 * Description : 将存放到*buf地址里长度为length的数据发送出去
 *
 * Arguments   : unsigned char *buf: 数据存放地址
 unsigned char length: 数据长度, 以byte为单位
 *
 * Returns     : FALSE: 发送数据接收到nack
 TRUE: 发送数据接收到ack
 *
 * Notes       : 由主控向I2C 设备发送数据，接收来自设备的响应
 *                  驱动内部函数，不向外提供接口
 ********************************************************************************
 */
uint8 I2C_Trans_Bytes(uint8 *buf, uint8 length, gpio_init_cfg_t* gpio, uint8* delay)
{
    uint8 ret;

    IIC_Init(gpio, delay);

    IIC_Start(gpio, delay);
    //发送数据
    do
    {
        IIC_WriteByte(*buf, gpio, delay);
        buf++;
        if (!IIC_GetAck(gpio, delay))
        {
            break;
        }
        --length;
    } while (length != 0);

    if (length == 0)
    {
        ret = 1;
    }
    else
    {
        ret = 0;
    }
    IIC_Stop(gpio, delay);
    return ret;
}

//=============================================================================================
/*
 ******************************************************************************
 *             uint8 I2C_Recev_Bytes(uint8 *buf, uint8 address, uint8 length)
 *
 * Description : 读取长度length
 *
 * Arguments   : unsigned char *buf: 数据存放地址
 unsigned char address: slave地址
 *               unsigned char length: 数据长度,以byte为单位
 *
 * Returns     : FALSE: slave不响应
 TRUE: 收到数据
 *
 * Notes       :  驱动内部接口，不向外提供
 *
 ********************************************************************************
 */
uint8 I2C_Recev_Bytes(uint8 *buf, uint8 address, uint8 length, gpio_init_cfg_t* gpio, uint8* delay)
{
    uint8 ret = 0;
    uint8 *p;

    p = buf;

    IIC_Init(gpio, delay);

    IIC_Start(gpio, delay);

    /*发送slave 地址*/
    IIC_WriteByte(address, gpio, delay);
    if (!IIC_GetAck(gpio, delay))
    {
        ret = 0;
        goto recev_ret;
    }

    while (length != 0)
    {
        if (length == 1) //the last byte, sent nack
        {
            *p = IIC_ReadByte(gpio, delay);
            IIC_SendAck(1, gpio, delay);
        }
        else
        {
            *p = IIC_ReadByte(gpio, delay);
            IIC_SendAck(0, gpio, delay);
        }
        p++;
        length--;
    }
    ret = 1;
    recev_ret: IIC_Stop(gpio, delay);
    return ret;
}

/*
 *******************************************************************************
 *             void IIC_WriteByte(uint8 dat)
 *
 * Description : 写1个字节的数据到slave
 *
 * Arguments   : 1个字节数据

 *
 * Returns     : 1 ack; 0 nack
 *
 * Notes       :
 *
 *******************************************************************************
 */
void IIC_WriteByte(uint8 dat, gpio_init_cfg_t* gpio, uint8* delay)
{
    uint8 i;
    for (i = 0; i < 8; i++)
    {
        if (((dat << i) & 0x80) != 0) //对于一个字节，从高bit 到低bit 依次发送
        {
            IIC_SDA_high_hold(gpio, delay);
        }
        else
        {
            IIC_SDA_low_hold(gpio, delay);
        }
        IIC_SCL_high_hold(gpio, delay);
        IIC_SCL_low_hold(gpio, delay);
    }
}

/*
 *******************************************************************************
 *             uint8 IIC_ReadByte(void)
 *
 * Description : 读取1个字节的数据,并回ack或者nack给slave
 *
 * Arguments   : unsigned char ack: 1 ack; 0 nack

 *
 * Returns     : 读到的数据
 *
 * Notes       :
 *
 *******************************************************************************
 */
uint8 IIC_ReadByte(gpio_init_cfg_t* gpio, uint8* delay)
{
    uint8 t = 8, dat = 0;
    uint8 IIC_DATA;

    //IIC_SDA_high_hold(gpio, delay);

    //IIC_Delay(delay);
    IIC_SDA_Output_Dis(gpio);
    //IIC_SDA_Input_En(gpio);
    IIC_Delay(delay);

    do
    {
        IIC_SCL_high_hold(gpio, delay);

        dat <<= 1;
        IIC_DATA = IIC_Data_Input(gpio);
        if (IIC_DATA != 0)
        {
            dat |= 0x01;
        } //从I2C 数据线上采样一个字节的数据，是从高bit 到低bit 采样

        IIC_SCL_low_hold(gpio, delay);
        --t;
    } while (t != 0);

    //IIC_Delay(delay);
    //IIC_SDA_Input_Dis(gpio);
    IIC_SDA_Output_En(gpio);
    IIC_Delay(delay);
    return dat;
}

/*
 *******************************************************************************
 *             void IIC_SendAck(uint8 ack)
 *
 * Description : 发送ack还是nack
 *
 * Arguments   : unsigned char ack: 1 noack; 0 ack

 *
 * Returns     :
 *
 * Notes       :
 *
 *******************************************************************************
 */
void IIC_SendAck(uint8 ack, gpio_init_cfg_t* gpio, uint8* delay)
{
    if (ack != 0)
    {
        IIC_SDA_high_hold(gpio, delay); //nack
    }
    else
    {
        IIC_SDA_low_hold(gpio, delay); //ack
    }
    IIC_SCL_high_hold(gpio, delay);
    IIC_SCL_low_hold(gpio, delay);
}
/*
 *******************************************************************************
 *             uint8 IIC_GetAck(void)
 *
 * Description : Return Ack From the slave device, one is Ack, but zero NoAck.
 *
 * Arguments   : unsigned char ack: 1 noack; 0 ack

 *
 * Returns     :
 *
 * Notes       :   获取从设备端发送过来的响应信号Ack  or  NAck
 *
 *******************************************************************************
 */
uint8 IIC_GetAck(gpio_init_cfg_t* gpio, uint8* delay)
{
    uint8 ack;
    uint8 IIC_DATA;

    // IIC_SDA_high_hold(gpio, delay);


    //IIC_Delay(delay);
    IIC_SDA_Output_Dis(gpio);
    //IIC_SDA_Input_En(gpio);
    IIC_Delay(delay);
    IIC_SCL_high_hold(gpio, delay);
    IIC_DATA = IIC_Data_Input(gpio); //只采样一个bit
    //IIC_SDA_Input_Dis(gpio);
    if (IIC_DATA != 0)
    {
        ack = 0; //nack
        //IIC_SDA_high_hold(gpio, delay);
    }
    else
    {
        ack = 1; //ack
        //IIC_SDA_low_hold(gpio, delay);
    }
    IIC_SDA_Output_En(gpio);
    IIC_Delay(delay);
    IIC_SCL_low_hold(gpio, delay);
    IIC_SDA_low_hold(gpio, delay);
    //IIC_SDA_Input_Dis(gpio);

    return ack;
}

