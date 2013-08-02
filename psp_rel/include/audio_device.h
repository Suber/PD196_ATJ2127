#ifndef __AUDIO_DEVICE_DEFINE_H__
#define __AUDIO_DEVICE_DEFINE_H__

#include <attr.h>

/*ANALOG INPUT TYPE*/
#define MIC_INPUT 0
#define FM_INPUT 1
#define AIN2_INPUT 2
#define AIN_RESERVE 3
#define LINE_INPUT AIN2_INPUT
//SPECIAL CASE
#define MICIN_KARA 4

/*DAC INPUT TYPE*/
#define AHB_INPUT 0
#define ADC_INPUT 1
#define AUD_INPUT 2
#define IIS_INPUT 3
#define SDMA_INPUT 4
#define DAC_INPUT_RESERVE1 5
#define DAC_INPUT_RESERVE2 6
#define DAC_INPUT_RESERVE3 7

/*CAR OP CHANNEL*/
#define FL_OUT 0
#define FR_OUT 1
#define RL_OUT 2
#define RR_OUT 3

typedef void* (*audio_device_op_i)(void*, void*, void*);

typedef enum {
	ENABLE_PA = 0,
	DISABLE_PA,
	ENABLE_PA_DDV,
	DISABLE_PA_DDV,
	SET_PA_VOLUME,
	GET_PA_VOLUME,
	ENABLE_DAC,
	DISABLE_DAC,
	SET_DAC_RATE,
	GET_DAC_RATE,
	ENABLE_AIN,
	DISABLE_AIN,
	SET_AIN_GAIN,
	SET_ADC_GAIN,
	ENABLE_ADC,
	DISABLE_ADC,
	SET_KEY_TONE_VOL,
	SET_KEY_TONE_FREQ,
	SET_KEY_TONE_PERIOD,	
	START_KEY_TONE,
	SET_CAR_OP,
	SET_CAR_GAIN,			
} audio_device_cmd_e;

struct audio_device_operations {
	audio_device_op_i enable_pa;
	audio_device_op_i disable_pa;
	audio_device_op_i enable_pa_ddv;
	audio_device_op_i disable_pa_ddv;
	audio_device_op_i set_pa_volume;
	audio_device_op_i get_pa_volume;
	audio_device_op_i enable_dac;
	audio_device_op_i disable_dac;
	audio_device_op_i set_dac_rate;
	audio_device_op_i get_dac_rate;
	audio_device_op_i enable_ain;
	audio_device_op_i disable_ain;
	audio_device_op_i set_ain_gain;
	audio_device_op_i set_adc_gain;
	audio_device_op_i enable_adc;
	audio_device_op_i disable_adc;
	audio_device_op_i set_key_tone_vol;
	audio_device_op_i set_key_tone_freq;
	audio_device_op_i set_key_tone_period;
	audio_device_op_i start_key_tone;
	audio_device_op_i set_car_op;
	audio_device_op_i set_car_gain;					
};

/*
 audio device 向上层提供统一的接口，即只提供一个API，并以ID的方式区分
 */
extern void* audio_device_op_entry(void *param1, void *param2, void *param3, audio_device_cmd_e cmd_type) __FAR__;

#define enable_pa(a,b) audio_device_op_entry((void*)(a), (void*)(b), (void*)(0), ENABLE_PA)
#define disable_pa() audio_device_op_entry((void*)(0), (void*)(0), (void*)(0), DISABLE_PA)
#define enable_pa_ddv(a,b) audio_device_op_entry((void*)(a), (void*)(b), (void*)(0), ENABLE_PA_DDV)
#define disable_pa_ddv() audio_device_op_entry((void*)(0), (void*)(0), (void*)(0), DISABLE_PA_DDV)
#define set_pa_volume(a) audio_device_op_entry((void*)(a), (void*)(0), (void*)(0), SET_PA_VOLUME)
#define get_pa_volume() audio_device_op_entry((void*)(0), (void*)(0), (void*)(0), GET_PA_VOLUME)
#define enable_dac(a,b) audio_device_op_entry((void*)(a), (void*)(b), (void*)(0), ENABLE_DAC)
#define disable_dac() audio_device_op_entry((void*)(0), (void*)(0), (void*)(0), DISABLE_DAC)
#define set_dac_rate(a) audio_device_op_entry((void*)(a), (void*)(0), (void*)(0), SET_DAC_RATE)
#define get_dac_rate() audio_device_op_entry((void*)(0), (void*)(0), (void*)(0), GET_DAC_RATE)
#define enable_ain(a,b) audio_device_op_entry((void*)(a), (void*)(b), (void*)(0), ENABLE_AIN)
#define disable_ain(a) audio_device_op_entry((void*)(a), (void*)(0), (void*)(0), DISABLE_AIN)
#define set_ain_gain(a,b) audio_device_op_entry((void*)(a), (void*)(b), (void*)(0), SET_AIN_GAIN)
#define set_adc_gain(a) audio_device_op_entry((void*)(a), (void*)(0), (void*)(0), SET_ADC_GAIN)
#define enable_adc(a,b) audio_device_op_entry((void*)(a), (void*)(b), (void*)(0), ENABLE_ADC)
#define disable_adc() audio_device_op_entry((void*)(0), (void*)(0), (void*)(0), DISABLE_ADC)
#define set_key_tone_vol(a) audio_device_op_entry((void*)(a), (void*)(0), (void*)(0), SET_KEY_TONE_VOL)
#define set_key_tone_freq(a) audio_device_op_entry((void*)(a), (void*)(0), (void*)(0), SET_KEY_TONE_FREQ)
#define set_key_tone_period(a) audio_device_op_entry((void*)(a), (void*)(0), (void*)(0), SET_KEY_TONE_PERIOD)
#define start_key_tone(a) audio_device_op_entry((void*)(a), (void*)(0), (void*)(0), START_KEY_TONE)
#define set_car_op(a) audio_device_op_entry((void*)(a), (void*)(0), (void*)(0), SET_KEY_TONE_PERIOD)
#define set_car_gain(a,b) audio_device_op_entry((void*)(a), (void*)(b), (void*)(0), START_KEY_TONE)


/* hereinafter, for fw-decrypt. xiaomaky.  2012-11-12 16:48.    */
typedef struct
{
   unsigned char *pInOutBuffer;
   long InOutLen;
   long FileLength;
   unsigned char *pGLBuffer;
   unsigned char *initusebuffer;
   long initusebufferlen;
} PCFWDecrypt_struct;

extern int PCFWDecryptInit(PCFWDecrypt_struct *PCFWDecrypt_s) __FAR__;
extern void PCFWDecrypt(unsigned char *pInOutBuffer, long BufferLength, unsigned char *pGLBuffer) __FAR__;

struct fw_decrypt_operations
{
    audio_device_op_i fw_decrypt_init;
    audio_device_op_i fw_decrypt_run;
};

#define fw_decrypt_init(a) audio_device_op_entry((void*)(a), (void*)(0), (void*)(0), ENABLE_PA)
#define fw_decrypt_run(a,b,c) audio_device_op_entry((void*)(a), (void*)(b), (void*)(c), DISABLE_PA)

#endif

