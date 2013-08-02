使用外挂codec wm8904，已调试通过 录音放音功能，并可支持采样率96K


/******************************************************************/

一、ap_record，录音修改

1、提供接口

/*
function：初始化wm8904和I2S配置
input：   i2s_sn -- i2s GPIO组选择，0 或 1
ouput:    0 or -1
*/
int Codec_init(uint8 i2s_sn)


/*
function：根据录音比特率配置WM8904采样率
input：   bitrate_type -- 0~10,参照record_bitrate_e定义
ouput:    0 or -1
*/
int Codec_set_brat(uint8 bitrate_type)


/*
function：配置WM8904的adc增益
input：   gain_type -- 0~15,增益范围
ouput:    0 or -1
*/
int Codec_set_adcgain(uint8 gain_type)//0~15


/*
function：退出wm8904和I2S配置
ouput:    0 or -1
*/
int Codec_exit(void)

2、与中间件相关
1)调用enable_adc必须配置参数1为I2S类型
2)g_record_vars.ai_source  的类型必须为 AI_SOURCE_I2S;用作传参给中间件使用


/******************************************************************/
二、music_engine，放音修改
1、提供接口

/*
function：初始化wm8904和I2S配置
input：   i2s_sn -- i2s GPIO组选择，0 或 1
ouput:    0 or -1
*/
int Codec_init(uint8 i2s_sn)


/*
function：配置WM8904的I2S采样率
input：   sample_rate -- {8,12,16,24,32,48,96};KHZ
ouput:    0 or -1
*/
int Codec_set_sample(uint16 sample_rate)

/*
function：配置WM8904的dac增益
input：   gain_type -- 0~15,增益范围
ouput:    0 or -1
*/
int Codec_set_dacgain(uint8 gain_type)//0~15

/*
function：退出wm8904和I2S配置
ouput:    0 or -1
*/
int Codec_exit(void)

2、与中间件相关
1)调用enable_dac必须配置参数1为I2S类型




李敏贤
2013-05-15



