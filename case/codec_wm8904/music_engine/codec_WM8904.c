/*
 *******************************************************************************
 *                              us212a
 *                             Module: record
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : codec_WM8904.c
 * History:
 *      <author>    <time>           <version >             <desc>
 *
 *******************************************************************************
 */
/* include file of this application, 本应用的头文件 */
#include "app_mengine.h"

//#define  GPIO_SCL_BIT1    (0x00000001<<17)
//#define  GPIO_SDA_BIT1    (0x00000001<<17)
/* I2C 所用模拟GPIO 信息结构*/
const gpio_init_cfg_t   gpio_i2c[2] =
{
     //SCL==GPIO_A15
    { GPIO_AINEN, GPIO_AOUTEN, GPIO_ADAT, GPIO_SCL_BIT },
    //SDA==GPIO_A16
    { GPIO_AINEN, GPIO_AOUTEN, GPIO_ADAT, GPIO_SDA_BIT} 
};

/*
static const int rec_sample_rate[11] =
{ 8000, 8000, 16000, 32000, 32000, 16000, 16000, 24000, 32000, 48000, -1 };
*/
static const uint16 codec_rate_set[7] = 
{0xc00,0xc01,0xc02,0xc03,0xc04,0xc05,0x405};

static const uint16 sample_rate_val[7] = 
{8,12,16,24,32,48,96};


UINT32 mfp_ctlx_bak;
//gpio_init_cfg_t *I2C_gpio;
UINT8  codec_i2s_sn = 0xff;
UINT8  cur_sample_set;


struct reg_default
{
	uint8 reg;
	uint16 rdata;
};


static const struct reg_default wm8904_reg_defaults[] = {
	{ 4,   0x0019 },     /* R4   - Bias Control 0 */
	{ 5,   0x0043 },     /* R5   - VMID Control 0 */
	{ 6,   0x007f },     /* R6   - Mic Bias Control 0 */
	{ 7,   0x0007 },     /* R7   - Mic Bias Control 1 */
	{ 8,   0x0001 },     /* R8   - Analogue DAC 0 */
	{ 9,   0x9696 },     /* R9   - mic Filter Control */
	{ 10,  0x0001 },     /* R10  - Analogue ADC 0 */
	{ 12,  0x0003 },     /* R12  - Power Management 0 */
	{ 14,  0x0003 },     /* R14  - Power Management 2 */
	{ 15,  0x0000 },     /* R15  - Power Management 3 */
	{ 18,  0x000C },     /* R18  - Power Management 6 */
	{ 20,  0x845E },     /* R20  - Clock Rates 0 */
	{ 21,  0x0C05 },     /* R21  - Clock Rates 1 */
	{ 22,  0x0006 },     /* R22  - Clock Rates 2 */
	{ 24,  0x0050 },     /* R24  - Audio Interface 0 */
	{ 25,  0x010e },     /* R25  - Audio Interface 1 */ /*固定32bit*/
	{ 26,  0x00E4 },     /* R26  - Audio Interface 2 */
	{ 27,  0x0040 },     /* R27  - Audio Interface 3 */
	{ 30,  0x00C0 },     /* R30  - DAC Digital Volume Left */
	{ 31,  0x00C0 },     /* R31  - DAC Digital Volume Right */
	{ 32,  0x0000 },     /* R32  - DAC Digital 0 */
	{ 33,  0x0000 },     /* R33  - DAC Digital 1 */
	{ 36,  0x00C0 },     /* R36  - ADC Digital Volume Left */
	{ 37,  0x00C0 },     /* R37  - ADC Digital Volume Right */
	{ 38,  0x0010 },     /* R38  - ADC Digital 0 */
	{ 39,  0x0000 },     /* R39  - Digital Microphone 0 */
	{ 40,  0x01AF },     /* R40  - DRC 0 */
	{ 41,  0x3248 },     /* R41  - DRC 1 */
	{ 42,  0x0000 },     /* R42  - DRC 2 */
	{ 43,  0x0000 },     /* R43  - DRC 3 */
	{ 44,  0x001f },     /* R44  - Analogue Left Input 0 */
	{ 45,  0x001f },     /* R45  - Analogue Right Input 0 */
	{ 46,  0x0046 },     /* R46  - Analogue Left Input 1 */
	{ 47,  0x0046 },     /* R47  - Analogue Right Input 1 */
	{ 57,  0x002D },     /* R57  - Analogue OUT1 Left */
	{ 58,  0x002D },     /* R58  - Analogue OUT1 Right */
	{ 59,  0x0039 },     /* R59  - Analogue OUT2 Left */
	{ 60,  0x0039 },     /* R60  - Analogue OUT2 Right */
	{ 61,  0x0000 },     /* R61  - Analogue OUT12 ZC */
	{ 67,  0x0003 },     /* R67  - DC Servo 0 */
	{ 69,  0xAAAA },     /* R69  - DC Servo 2 */
	{ 71,  0xAAAA },     /* R71  - DC Servo 4 */
	{ 72,  0xAAAA },     /* R72  - DC Servo 5 */
	{ 90,  0x00ff },     /* R90  - Analogue HP 0 */
	{ 94,  0x0000 },     /* R94  - Analogue Lineout 0 */
	{ 98,  0x0001 },     /* R98  - Charge Pump 0 */
	{ 104, 0x0005 },     /* R104 - Class W 0 */
	{ 108, 0x0000 },     /* R108 - Write Sequencer 0 */
	{ 109, 0x0000 },     /* R109 - Write Sequencer 1 */
	{ 110, 0x0000 },     /* R110 - Write Sequencer 2 */
	{ 111, 0x0000 },     /* R111 - Write Sequencer 3 */
	{ 112, 0x0000 },     /* R112 - Write Sequencer 4 */
	{ 116, 0x0000 },     /* R116 - FLL Control 1 */
	{ 117, 0x0007 },     /* R117 - FLL Control 2 */
	{ 118, 0x0000 },     /* R118 - FLL Control 3 */
	{ 119, 0x2EE0 },     /* R119 - FLL Control 4 */
	{ 120, 0x0004 },     /* R120 - FLL Control 5 */
	{ 121, 0x0014 },     /* R121 - GPIO Control 1 */
	{ 122, 0x0010 },     /* R122 - GPIO Control 2 */
	{ 123, 0x0010 },     /* R123 - GPIO Control 3 */
	{ 124, 0x0000 },     /* R124 - GPIO Control 4 */
	{ 126, 0x0000 },     /* R126 - Digital Pulls */
	{ 128, 0xFFFF },     /* R128 - Interrupt Status Mask */
	{ 129, 0x0000 },     /* R129 - Interrupt Polarity */
	{ 130, 0x0000 },     /* R130 - Interrupt Debounce */
	{ 134, 0x0000 },     /* R134 - EQ1 */
	{ 135, 0x000C },     /* R135 - EQ2 */
	{ 136, 0x000C },     /* R136 - EQ3 */
	{ 137, 0x000C },     /* R137 - EQ4 */
	{ 138, 0x000C },     /* R138 - EQ5 */
	{ 139, 0x000C },     /* R139 - EQ6 */
	{ 140, 0x0FCA },     /* R140 - EQ7 */
	{ 141, 0x0400 },     /* R141 - EQ8 */
	{ 142, 0x00D8 },     /* R142 - EQ9 */
	{ 143, 0x1EB5 },     /* R143 - EQ10 */
	{ 144, 0xF145 },     /* R144 - EQ11 */
	{ 145, 0x0B75 },     /* R145 - EQ12 */
	{ 146, 0x01C5 },     /* R146 - EQ13 */
	{ 147, 0x1C58 },     /* R147 - EQ14 */
	{ 148, 0xF373 },     /* R148 - EQ15 */
	{ 149, 0x0A54 },     /* R149 - EQ16 */
	{ 150, 0x0558 },     /* R150 - EQ17 */
	{ 151, 0x168E },     /* R151 - EQ18 */
	{ 152, 0xF829 },     /* R152 - EQ19 */
	{ 153, 0x07AD },     /* R153 - EQ20 */
	{ 154, 0x1103 },     /* R154 - EQ21 */
	{ 155, 0x0564 },     /* R155 - EQ22 */
	{ 156, 0x0559 },     /* R156 - EQ23 */
	{ 157, 0x4000 },     /* R157 - EQ24 */
	{ 161, 0x0000 },     /* R161 - Control Interface Test 1 */
	{ 204, 0x0000 },     /* R204 - Analogue Output Bias 0 */
	{ 247, 0x0000 },     /* R247 - FLL NCO Test 0 */
	{ 248, 0x0019 },     /* R248 - FLL NCO Test 1 */
	
};

void Enable_i2s(uint8 sn)
{
    uint32 tmp32;
    
    if(sn == 0)
    {         
        mfp_ctlx_bak = act_readl(MFP_CTL2);
        act_writel((mfp_ctlx_bak & 0xfff8dfff), MFP_CTL2);  //I2S GPIOB 7 6 5 0
    }
    else
    {
        mfp_ctlx_bak = act_readl(MFP_CTL3);
        tmp32 = mfp_ctlx_bak & 0xffffe000;      //I2S GPIOB 15 14 13 12
        act_writel((tmp32|0x1b35), MFP_CTL3); 
    } 
}


void Disable_i2s(uint8 sn)
{        
    if(sn == 0)
    {         
        act_writel(mfp_ctlx_bak,MFP_CTL2);
    }
    else
    {
        act_writel(mfp_ctlx_bak,MFP_CTL3);
    }        
}

/*
function：初始化wm8904和I2S配置
input：   i2s_sn -- i2s GPIO组选择，0 或 1
ouput:    0 or -1
*/
int Codec_init(uint8 i2s_sn)
{
    uint8 IIC_data[6];
    uint16 i;
    uint32 flags;
    uint8 delay_i2c = 20;
    
    if(i2s_sn == codec_i2s_sn)
    {
        ;
    }
    else if(codec_i2s_sn != 0xff)
    {
        Disable_i2s(codec_i2s_sn);
    }
    else
    {
        Enable_i2s(i2s_sn);  
    }
        
    codec_i2s_sn = i2s_sn;
    cur_sample_set = 0xff;
    
	/* I2C 配置指针*/
    gpio_init_cfg_t *I2C_gpio = (gpio_init_cfg_t *) gpio_i2c;
    //关中断
    flags = sys_local_irq_save();    
    
    IIC_Init(I2C_gpio,&delay_i2c);

    IIC_data[0] = 0x34;
    IIC_data[1] = 0;
    IIC_data[2] = 0;
    IIC_data[3] = 0;
    I2C_Trans_Bytes(IIC_data, 4, I2C_gpio, &delay_i2c);

    IIC_data[0] = 0; //reg
    I2C_Recev_Bytes(IIC_data,0x34,2, I2C_gpio, &delay_i2c);
  
	IIC_data[0] = 0x34;
	
	for(i=0;i<93;i++)//
	{
		IIC_data[1] = wm8904_reg_defaults[i].reg;
		IIC_data[2] = (wm8904_reg_defaults[i].rdata>>8)&0xff;
		IIC_data[3] = wm8904_reg_defaults[i].rdata&0xff;
		I2C_Trans_Bytes(IIC_data, 4, I2C_gpio, &delay_i2c);
	}
	
    IIC_Exit(I2C_gpio);
            
	sys_local_irq_restore(flags);
    
	return 0;
}

/*
function：配置WM8904的I2S采样率
input：   sample_rate -- {8,12,16,24,32,48,96};KHZ
ouput:    0 or -1
*/
int Codec_set_sample(uint16 sample_rate)//单位KHZ
{
    uint8 IIC_data[6];
    uint16 tmp16;
    uint32 flags;
    uint8 sample_rate_type = 0;
    uint8 delay_i2c = 20;
    
    /* I2C 配置指针*/
    gpio_init_cfg_t *I2C_gpio = (gpio_init_cfg_t *) gpio_i2c;
    
    if(codec_i2s_sn == 0xff)
    {
        return -1;    
    }
    
    if(sample_rate == 0)
    {
        return -1;    
    }
        
    for(tmp16 = 1; tmp16 < 7; tmp16++)
    {
        if (sample_rate >= sample_rate_val[tmp16])
        {
            sample_rate_type++;
        }
        else
        {
            break;    
        }
    }    
        
    if(sample_rate_type > 6)
    {
        sample_rate_type = 6;    
    }
        
    if(cur_sample_set != sample_rate_type)    
    {
        cur_sample_set = sample_rate_type;
    }    
    else
    {
        return 0;    
    }
                    
    //关中断
    flags = sys_local_irq_save();
    
    IIC_Init(I2C_gpio,&delay_i2c);

    if(sample_rate_type == 6)
    {
        //96K采样
        tmp16 = 0x845F;
    }
    else
    {
        tmp16 = 0x845E;
    }

    IIC_data[0] = 0x34;
    IIC_data[1] = 0x14;
    IIC_data[2] = (uint8)((tmp16>>8)&0xff);
    IIC_data[3] = (uint8)(tmp16&0xff);
    I2C_Trans_Bytes(IIC_data, 4, I2C_gpio, &delay_i2c);

    tmp16 = codec_rate_set[sample_rate_type];       
    
    IIC_data[0] = 0x34;
    IIC_data[1] = 0x15;
    IIC_data[2] = (uint8)((tmp16>>8)&0xff);
    IIC_data[3] = (uint8)(tmp16&0xff);
    I2C_Trans_Bytes(IIC_data, 4, I2C_gpio, &delay_i2c);
    
    IIC_Exit(I2C_gpio);
       
    sys_local_irq_restore(flags);
    
//    Codec_set_dacgain(2);//test code
    
    return 0;
}


/*
function：配置WM8904的DAC增益
input：   gain_type -- 0~15,增益范围
ouput:    0 or -1
*/
int Codec_set_dacgain(uint8 gain_type)//0~15
{
    uint8 IIC_data[6];
    uint16 tmp16;
    uint32 flags;
    uint8 delay_i2c = 20;
    
    /* I2C 配置指针*/
    gpio_init_cfg_t *I2C_gpio = (gpio_init_cfg_t *) gpio_i2c;
    
    if(codec_i2s_sn == 0xff)
    {
        return -1;    
    }
           
    //关中断
    flags = sys_local_irq_save();
    
    IIC_Init(I2C_gpio,&delay_i2c);
    
    if(gain_type > 15)
    {
        gain_type = 15;
    }
    
    tmp16 = 0x01c0 + (gain_type<<1);//步进为0.75 db
    
    IIC_data[0] = 0x34;
    IIC_data[1] = 0x1E;
    IIC_data[2] = (uint8)((tmp16>>8)&0xff);
    IIC_data[3] = (uint8)(tmp16&0xff);
    I2C_Trans_Bytes(IIC_data, 4, I2C_gpio, &delay_i2c);
    
    IIC_data[0] = 0x34;
    IIC_data[1] = 0x1F;
    IIC_data[2] = (uint8)((tmp16>>8)&0xff);
    IIC_data[3] = (uint8)(tmp16&0xff);
    I2C_Trans_Bytes(IIC_data, 4, I2C_gpio, &delay_i2c);
    
    IIC_Exit(I2C_gpio);    
    
    sys_local_irq_restore(flags);
    
    return 0;
}


/*
function：退出wm8904和I2S配置
ouput:    0 or -1
*/
int Codec_exit(void)
{
    uint8 IIC_data[6];
    uint32 flags;
    uint8 delay_i2c = 20;    
    /* I2C 配置指针*/
    gpio_init_cfg_t *I2C_gpio = (gpio_init_cfg_t *) gpio_i2c;
    
    if(codec_i2s_sn == 0xff)
    {
        return -1;    
    }
            
    //关中断
    flags = sys_local_irq_save(); 

    IIC_Init(I2C_gpio,&delay_i2c);

    IIC_data[0] = 0x34;
    IIC_data[1] = 0;
    IIC_data[2] = 0;
    IIC_data[3] = 0;
    I2C_Trans_Bytes(IIC_data, 4, I2C_gpio, &delay_i2c);
        
    IIC_Exit(I2C_gpio);    
        
    
    Disable_i2s(codec_i2s_sn);
    codec_i2s_sn = 0xFF;
    
    sys_local_irq_restore(flags);
    return 0;
}



