#ifndef _APP_MUSIC_BOOKMARK_H
#define _APP_MUSIC_BOOKMARK_H

#include "psp_includes.h"
#include "case_include.h"
#include "mmm_mp.h"

#define LIST_NUM_ONE_PAGE 6

#define BM_BUFF_SIZE  512

#define BM_SORT_INDEX_SIZE   12       //每个索引项12字节
#define MAX_BOOKMARKS        10      // 每个文件可记录的书签数
#define BM_ITEM_LEN 12

#define BM_INDEX_PER_SECTOR  40     //每扇区40个索引项

#define BM_INVALIDE_INDEX    0xffff
#define BM_NO_FREE_INDEX     0xfffe

//#define BM_ITEM_SIZE         sizeof(mfile_bm_items_t)

#define BM_SECTOR_PER_SORT_INDEX (512 / BM_SORT_INDEX_SIZE)    //一个扇区记录64个索引项
//#define BM_SECTOR_PER_ITEMS      (512 / BM_ITEM_SIZE)          //一个扇区记录1个数据项


#define MODE_ADD_BOOKMARK  0
#define MODE_SELECT_BOOKMARK 1
#define MODE_DEL_BOOKMARK  2
#define MODE_TEST_BOOKMARK 3

#define SECTOR_ATTR_CLEAN 0
#define SECTOR_ATTR_DIRTY 1

#define BMK_DATA_SIZE 256
#define SEC_SIZE_USE  512

#define BMK_ITEM_DATA_LEN 224

//#define TEST_BM                    //是否测试bookmark

typedef struct
{
    mmm_mp_bp_info_t break_param; //12bytes
    time_t break_time;//3bytes
    uint8 reserved;
} music_bm_item; // 16byte

typedef struct
{
    music_bm_item mfile_bmk_items[MAX_BOOKMARKS];//160bytes
    uint8 mfile_name[64];//64bytes
    uint8 reserved[32];
} mfile_bm_items_t; //224byte


typedef struct
{
    uint32 bmk_clus_no; //4bytes
    uint32 bmk_dir_eny; //4bytes
    uint16 prev;        //2bytes
    uint16 next;        //2bytes
} mbmk_index_t;//索引项数据结构


typedef struct
{
    uint16 sector;
    uint16 attr;
} bmk_cache_t;

//全局变量
extern uint16 bmk_cache_sector;
extern music_bm_head bm_head;

//在.xdata段，该文件.xdata段置于bank段
extern uint8 bm_buf[BM_BUFF_SIZE] _BANK_DATA_ATTR_;

//子功能模块
extern void file_read(void *buffer, uint32 len, uint32 fp);
extern void write_sector(uint32 sector_num, uint32 fp);
extern void read_sector(uint32 sector_num, uint32 fp);
extern mbmk_index_t *read_bmk_index(uint16 data_index, uint32 bm_fp);
extern void write_bmk_index(uint16 data_index, uint32 bm_fp);
extern mfile_bm_items_t *read_bmk_item(uint16 index, uint32 bm_fp);

//外部声明函数
//extern uint32 open_bookmark(void);
extern uint16 ui_get_bookmark_index(void);
extern uint8 handle_bookmark(uint8 active, uint8 mode, uint16 cur_index);
extern uint16 get_bookmark_index(uint32 cluster_no, uint32 dir_entry, uint8* name_str, uint32 bm_fp);
extern uint16 ui_show_bookmark(uint8 mode, msg_cb_func msg_call_back);
extern uint8 get_bookmark_time(uint16 index, time_t *bm_item_buf, uint32 bm_fp);
extern void del_bookmark_file(file_location_t *plocation);
extern uint16 find_index(uint32 cluster_no, uint32 dir_entry, uint32 bm_fp, uint8 mode);
extern uint32 open_bookmark(void);
extern void close_bookmark(uint32 bm_fp);
extern void music_del_bookmark(file_location_t *plocation, uint16 cur_index, uint32 bm_fp);
#ifdef TEST_BM
extern uint8 test_bm(void);
#endif

#endif

