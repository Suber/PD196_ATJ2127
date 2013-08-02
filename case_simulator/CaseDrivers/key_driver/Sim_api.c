//#include "key.h"

typedef int EntryFunc;

extern struct block_key_operations blk_op;
extern void key_drv_init(void);

_declspec(dllexport) EntryFunc DriverOpEntry(void)
{
	return (EntryFunc)&blk_op;
}

_declspec(dllexport) EntryFunc DriverInit(void)
{
	return (EntryFunc)key_drv_init;
}