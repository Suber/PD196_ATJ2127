#ifndef __ANALOG_DEFINE_H__
#define __ANALOG_DEFINE_H__

#include <ucos/init.h>
#include <attr.h>
#include <typeext.h>

#include <asm-mips/mach-gl5110/GL5110_Design_Specification.h>
typedef volatile unsigned int*      REG32;

extern int32 audio_device_enable_pa(uint32 volume_level, uint32 pa_swing, void* null3) __FAR__;
extern int32 audio_device_disable_pa(void* null1, void* null2, void* null3) __FAR__;
extern int32 audio_device_enable_pa_ddv(uint32 volume_level, uint32 pa_swing,void* null3) __FAR__;
extern int32 audio_device_disable_pa_ddv(void* null1, void* null2, void* null3) __FAR__;
extern int32 audio_device_set_pa_volume(uint32 volume_level, void* null2,void* null3) __FAR__;
extern int32 audio_device_get_pa_volume(void* null1, void* null2, void* null3) __FAR__;
extern int32 audio_device_enable_dac(uint32 src_type, uint32 dac_rate, void* null3) __FAR__;
extern int32 audio_device_disable_dac(void* null1, void* null2, void* null3) __FAR__;
extern int32 audio_device_set_dac_rate(uint32 dac_rate, void* null2, void* null3) __FAR__;
extern int32 audio_device_get_dac_rate(void* null1, void* null2, void* null3) __FAR__;
extern int32 audio_device_enable_ain(uint32 ain_type, uint32 ain_gain, void* null3) __FAR__;
extern int32 audio_device_disable_ain(uint32 ain_type, void* null2, void* null3) __FAR__;
extern int32 audio_device_set_ain_gain(uint32 ain_type, uint32 ain_gain, void* null3) __FAR__;
extern int32 audio_device_set_adc_gain(uint32 adc_gain, void* null2, void* null3) __FAR__;
extern int32 audio_device_enable_adc(uint32 src_type, uint32 adc_rate, void* null3) __FAR__;
extern int32 audio_device_disable_adc(void* null1, void* null2, void* null3) __FAR__;
extern int32 audio_device_set_key_tone_vol(uint32 volume, void* null2, void* null3) __FAR__;
extern int32 audio_device_set_key_tone_freq(uint32 frequency, void* null2, void* null3) __FAR__;
extern int32 audio_device_set_key_tone_period(uint32 period, void* null2, void* null3) __FAR__;
extern int32 audio_device_start_key_tone(void* null1, void* null2, void* null3) __FAR__;
extern int32 audio_device_set_car_op(uint32 para, void* null2, void* null3) __FAR__;
extern int32 audio_device_set_car_gain(uint32 channel, uint32 gain, void* null3) __FAR__;
extern int32 audio_device_set_adc_rate(uint32 adc_rate) __FAR__;
#endif
