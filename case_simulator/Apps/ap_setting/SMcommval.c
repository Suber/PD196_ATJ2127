#include "psp_includes.h"

typedef int (*MainFunc)(int);
//int main(int);
int ap_start(void *arg);

extern int8 main(void* data);
/*main中要填充这两个值，有系统人员分配*/
OS_STK ap_stack;
OS_STK *ptos=&ap_stack;

extern INT8U prio;

_declspec(dllexport) MainFunc GetMainFun(void)
{
	return ap_start;
}

void *process_start(void *app_param)
{
    int8 ret;
    ret = (int8)main((int)app_param, 0);
    libc_exit((int32)ret);
    return 0;
}

/*
该函数由exece函数调用，exece是进程管理器调用。
不支持construct 属性，即main前不会调用AP中的函数
所以不需要libcrt库
arg由exece传下来*/
int ap_start(void *arg)
{
   pthread_param_t pthread_param;
   int8 process_struct_index = libc_get_process_struct();

   if (process_struct_index == -1)
   {
   	/*process too many*/
	return -1;
   }

   pthread_param.start_rtn = process_start;
   pthread_param.arg = arg;
   pthread_param.ptos = ptos;

   sys_os_sched_lock();

   if(libc_pthread_create(&pthread_param, prio, process_struct_index) < 0)
   {
      libc_free_process_struct(process_struct_index);
      sys_os_sched_unlock();
   	  return -1;
   }

    sys_os_sched_unlock();
    return 0;
}
//#pragma comment(lib,"..\\..\\bin\\debug\\lib\\comlib")

