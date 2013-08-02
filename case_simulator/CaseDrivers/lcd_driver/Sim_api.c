#include "display.h"

typedef int EntryFunc;

extern struct lcd_driver_operations lcd_driver_op;
extern void lcd_drv_init(void);

_declspec(dllexport) EntryFunc DriverOpEntry(void)
{
	return (EntryFunc)&lcd_driver_op;
}

_declspec(dllexport) EntryFunc DriverInit(void)
{
	return (EntryFunc)lcd_drv_init;
}