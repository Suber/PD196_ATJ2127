#include "display.h"

typedef int EntryFunc;

extern struct ui_driver_operations ui_driver_op;

extern bool ui_init(void);

_declspec(dllexport) EntryFunc DriverOpEntry(void)
{
	return (EntryFunc)&ui_driver_op;
}

_declspec(dllexport) EntryFunc DriverInit(void)
{
	return (EntryFunc)ui_init;
}