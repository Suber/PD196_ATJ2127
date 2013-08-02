#ifndef _AP_UPGRADE_H
#define _AP_UPGRADE_H

#include  "psp_includes.h"
#include  "case_include.h"
#include  "upgrade_res.h"
#include  "upgrade_sty.h"
#include "Sys_info.h"

#define    AP_PLAYLIST_STK_POS  AP_FRONT_LOW_STK_POS
#define    AP_PLAYLIST_PRIO     AP_FRONT_LOW_PRIO

#ifdef  PC

extern uint8 sys_vram_space[0x20000];//256k
extern uint8 sys_sdram_space[0x20000];//128k

#define SYS_VRAM_ADDRESS   VM_PLAYLIST_INFO//sys_vram_space
#define SYS_SRAM_ADDRESS   sys_sdram_space

#else //#ifdef PLIST_SYS_DEF_SPACE
#define SYS_VRAM_ADDRESS   VM_PLAYLIST_INFO
#define SYS_SRAM_ADDRESS   0x9fc28000

#endif

#define SRAM_BASE_ADDR          0x9fc2e000
#define NAND_CACHE_ADDR         0x9fc3bc00 //解密过程中nand cache地址
#define NAND_DEFAULT_ADDR       0x9fc32000 // 默认nand cache地址

#define DECRYPT_INIT_BUF_SIZE 0x5000 //初始化解密buffer 20k
#define DECRYPT_USE_BUF_SIZE 0x4800 //解密用buffer 18k
#define DECRYPT_GL_BUF_SIZE  300
#define DECRYPT_WRITE_BUF_SIZE 0x4000 //写nandflash buffer 16k

#define DECRYPT_INIT_BUF_ADDR (SRAM_BASE_ADDR + 0x0000) //20k
#define DECRYPT_RW_BUF_ADDR  (SRAM_BASE_ADDR + 0x0000) //16k
#define DECRYPT_USE_BUF_ADDR (DECRYPT_INIT_BUF_ADDR + DECRYPT_INIT_BUF_SIZE) //18k
#define DECRYPT_GL_BUF_ADDR  (DECRYPT_USE_BUF_ADDR + DECRYPT_USE_BUF_SIZE) //300

#define DECRYPT_INIT_BUF_LEN  19888
#define DECRYPT_GL_BUF_LEN  300
#define DECRYPT_USR_BUF_LEN 18432


#define FILE_READ_ADDR    (SYS_SRAM_ADDRESS)              //16K
#define FILE_INFO_ADDR    (SYS_SRAM_ADDRESS + 16 * 1024)  //16K

//#define UPGRADE_DEBUG

typedef struct
{
    uint32 file_offset;
    uint32 sec_num;
    uint32 sram_addr;    
} ud_sector_param_t;

typedef enum
{
    FW_UPGRADE_NO_ERR,
    FW_UPGRADE_FILE_ERR,
    FW_UPGRADE_WRITE_ERR,
    FW_UPGRADE_CHECK_ERR
} upgrade_status_e;

//目录项结构
typedef struct
{
    char filename[11];
    char reserve1[5];
    unsigned long offset; 
    unsigned long length;
    char reserve2[4];
    unsigned long checksum; 
}__attribute__ ((packed)) DIR_t; //32字节

typedef struct
{
    LFIHead_t lfi_head;
    DIR_t diritem[240];
}__attribute__ ((packed)) LFIHead_dir_t; //8k bytes



typedef struct
{
    uint8 mode;
    uint8 *buffer;
    uint32 len;
    uint32 fp;
} decrypt_rw_t;

//全局变量

//系统变量
extern setting_comval_t g_setting_comval;

//文件系统具备
extern int32 g_file_sys_id;

//读写page缓冲区
extern uint8 *g_data_buffer;

//FW偏移
extern uint32 g_fw_offset;

//FW长度
extern uint32 g_fw_length;

//FLASH_ID偏移
extern uint32 g_flashid_offset;

//FLASH_ID长度
extern uint32 g_flashid_length;

//BREC长度
extern uint32 g_brec_length;

//前台频率备份
extern uint32 g_save_freq;

//flash id
extern uint8 g_flash_id[64];

//解密数据偏移
extern uint32 g_firmware_offset;

//百分比计数
extern uint8 g_precent_count;

//刷新定时器ID
extern int8 g_flush_timer_id;

//cpu释放计时器
extern uint8 cpu_release_count; 

//写入扇区计数
extern uint32 g_write_sector;

//解密数据结构体
extern PCFWDecrypt_struct g_decrypt;

//函数声明

extern upgrade_status_e _upgrade_firmware(void);

extern upgrade_status_e _firmware_brec_write(uint32 nandid_offset, uint32 nandid_size, uint32 file_handle);

extern AFI_DIR_t *check_key_offset_exist(char *buffer, uint32 buffer_len, const uint8 *key_word, 
    uint8 key_len, uint8 offset) __FAR__;

extern uint32 _read_file_translate(uint8 mode, uint32 fp, PCFWDecrypt_struct *decrypt_rw, uint32 read_len) __FAR__;

extern bool _read_file(uint32 read_len, uint32 fp) __FAR__;

extern bool ud_sector_write(void *pbuf, uint32 addr, uint32 len) __FAR__;

extern bool ud_sector_read(void *pbuf, uint32 addr, uint32 len) __FAR__;

extern bool brec_sector_write(void *pbuf, uint32 addr, uint32 len) __FAR__;

extern void _upgrade_proc_timer(void) __FAR__;

extern void _upgrade_refreash_progress(uint8 value) __FAR__;

extern app_result_e upgrade_get_app_msg(void) __FAR__;

extern upgrade_status_e _firmware_brec_write(uint32 nandid_offset, uint32 nandid_size, uint32 file_handle) __FAR__;

extern upgrade_status_e _upgrade_firmware(void) __FAR__;

extern uint32 _firmware_load_bank(void) __FAR__;

#endif
