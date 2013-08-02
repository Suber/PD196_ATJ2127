/*******************************************************************************
 *                              us212A
 *                            Module: Common
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved. 
 *
 * History:         
 *      <author>    <time>           <version >             <desc>
 *       lzcai     2011-9-23 14:46     1.0             build this file 
*******************************************************************************/
/*!
 * \file     _common_history.c
 * \brief    路径记忆功能接口实现。
 * \author   lzcai
 * \par      GENERAL DESCRIPTION:
 *               实现了一套路径记忆功能接口，包括读取记忆，保存记忆，清除记忆等。
 * \par      EXTERNALIZED FUNCTIONS:
 *               kernel vram_rw，psp，等等。
 * \version  1.0
 * \date     2011-9-23
*******************************************************************************/

#include "common_func.h"

#define HISTORY_LAYER_ONE   (4)     //每级路径记忆 4 字节
#define DIR_HISTORY_ONE     (4 * 16)//支持 8 级子目录，需要 9 级路径记忆，为了方便 512 对齐设置为 16 级
#define DIR_PATH_ID_MAX     (4)     //应用最多支持 4 个文件浏览路径记忆
#define MENU_HISTORY_ONE    (4 * 8) //支持 8 级路径记忆
#define MENU_PATH_ID_MAX    (8)     //应用最多支持 8 个菜单列表路径记忆
#define DIR_PATH_SEC_MAX    (32 * DIR_PATH_ID_MAX * DIR_HISTORY_ONE / 512)
#define MENU_PATH_SEC_MAX   (32 * MENU_PATH_ID_MAX * MENU_HISTORY_ONE / 512)
static uint8 get_dir_path_id(uint8 disk);

/*! \cond COMMON_API */

//文件浏览器路径记忆：
//在VRAM中开设 32 * DIR_PATH_ID_MAX * DIR_HISTORY_ONE 结构体矩阵，按 app_id 和 path_id 组成矩阵索引号访问。
//菜单列表路径记忆：
//在VRAM中开设 32 * MENU_PATH_ID_MAX * MENU_HISTORY_ONE 结构体矩阵，按 app_id 和 path_id 组成矩阵索引号访问。

//写路径记录整合临时缓冲区
uint8 history_buf[512] _BANK_DATA_ATTR_;

#ifdef PC
uint8 tmp_dir_history_buf[DIR_HISTORY_ONE];
uint8 tmp_menu_history_buf[MENU_HISTORY_ONE];
#endif

/******************************************************************************/
/*!                    
 * \par  Description:
 *    获取路径记忆项。
 * \param[in]    index 路径记忆索引，详细见history_index_t定义
 * \param[out]   history 指向路径记忆项，用于返回路径记忆项
 * \return       bool
 * \retval           TRUE 获取路径记忆项成功。
 * \retval           FALSE 获取路径记忆项失败。
 * \ingroup      common_func
 * \note 
*******************************************************************************/
bool com_get_history_item(history_index_t *index, history_item_t *history)
{
    uint32 base_addr;
    uint32 path_size;
    uint8 *history_addr;
    
    //计算路径记忆结构体地址
    if(index->type == PATH_HISTORY_MENU)
    {
        //菜单列表
        path_size = MENU_HISTORY_ONE;
        base_addr = VM_MENU_HISTORY + index->app_id * (path_size*MENU_PATH_ID_MAX) + index->path_id*path_size;
        
        if(menu_history_valid == FALSE)
        {
            menu_history_addr = NULL;
            #ifndef PC
            sys_malloc(&menu_history_addr, path_size);
            #else
            menu_history_addr = tmp_menu_history_buf;
            #endif
            if(menu_history_addr == NULL)
            {
                return FALSE;
            }
            
            //读回整个路径记忆项
            if(sys_vm_read(menu_history_addr, base_addr, path_size) == -1)
            {
                return FALSE;
            }
            menu_history_valid = TRUE;
        }
        history_addr = menu_history_addr;
    }
    else
    {
        //文件浏览器
        path_size = DIR_HISTORY_ONE;
        base_addr = VM_DIR_HISTORY + index->app_id * (path_size*DIR_PATH_ID_MAX);
        base_addr += get_dir_path_id(index->path_id)*path_size;
        
        if(dir_history_valid == FALSE)
        {
            dir_history_addr = NULL;
            #ifndef PC
            sys_malloc(&dir_history_addr, path_size);
            #else
            dir_history_addr = tmp_dir_history_buf;
            #endif
            if(dir_history_addr == NULL)
            {
                return FALSE;
            }
            
            //读回整个路径记忆项
            if(sys_vm_read(dir_history_addr, base_addr, path_size) == -1)
            {
                return FALSE;
            }
            dir_history_valid = TRUE;
        }
        history_addr = dir_history_addr;
    }
    
    //拷贝当前层路径记忆
    libc_memcpy((uint8 *)history, history_addr + index->layer * HISTORY_LAYER_ONE, HISTORY_LAYER_ONE);
    
    return TRUE;
}

/******************************************************************************/
/*!                    
 * \par  Description:
 *    设置路径记忆项。
 * \param[in]    index 路径记忆索引，详细见history_index_t定义
 * \param[in]    history 指向路径记忆项
 * \param[out]   none
 * \return       bool
 * \retval           TRUE 设置路径记忆项成功。
 * \retval           FALSE 设置路径记忆项失败。
 * \ingroup      common_func
 * \note 
*******************************************************************************/
bool com_set_history_item(history_index_t *index, history_item_t *history)
{
    uint32 offset;
    uint32 path_size;
    uint8 *history_addr;
    
    //计算路径记忆结构体地址
    if(index->type == PATH_HISTORY_MENU)
    {
        //菜单列表
        path_size = MENU_HISTORY_ONE;
        history_addr = menu_history_addr;
    }
    else
    {
        //文件浏览器
        path_size = DIR_HISTORY_ONE;
        history_addr = dir_history_addr;
    }
    
    offset = index->layer * HISTORY_LAYER_ONE;
    
    //比较当前要设置的记录项是否发生改变（只比较list_no），如果改变，则把往下层次清为 0xffff
    if( (*((uint16 *)(history_addr + offset) + 1)) != (*((uint16 *)history + 1)) )
    {
        libc_memset(history_addr + offset, 0xff, path_size - offset);
    }
    
    //设置路径记忆项
    libc_memcpy(history_addr + offset, (uint8 *)history, HISTORY_LAYER_ONE);
    
    return TRUE;
}

/******************************************************************************/
/*!                    
 * \par  Description:
 *    设置路径记忆项。
 * \param[in]    index 路径记忆索引，详细见history_index_t定义
 * \param[in]    history 指向路径记忆项
 * \param[out]   none
 * \return       bool
 * \retval           TRUE 设置路径记忆项成功。
 * \retval           FALSE 设置路径记忆项失败。
 * \ingroup      common_func
 * \note 
*******************************************************************************/
bool com_update_path_history(history_index_t *index)
{
    uint32 base_addr;
    uint32 sec_addr;
    uint32 offset;
    uint32 path_size;
    uint8 *history_addr;
    
    //计算路径记忆结构体地址
    if(index->type == PATH_HISTORY_MENU)
    {
        path_size = MENU_HISTORY_ONE;
        //如果没有读回有效path记忆，不需要写也不能写，直接返回即可
        if(menu_history_valid == FALSE)
        {
            return TRUE;
        }
        //菜单列表
        base_addr = VM_MENU_HISTORY + index->app_id * (path_size*MENU_PATH_ID_MAX) + index->path_id*path_size;
        
        history_addr = menu_history_addr;
    }
    else
    {
        path_size = DIR_HISTORY_ONE;
        //如果没有读回有效path记忆，不需要写也不能写，直接返回即可
        if(dir_history_valid == FALSE)
        {
            return TRUE;
        }
        //文件浏览器
        base_addr = VM_DIR_HISTORY + index->app_id * (path_size * DIR_PATH_ID_MAX);
        base_addr += get_dir_path_id(index->path_id) * path_size;
        
        history_addr = dir_history_addr;
    }
    
    //读出要更新路径记忆所在的扇区
    sec_addr = base_addr;
    offset = sec_addr & 0x1ff;
    sec_addr = sec_addr - offset;
    if(sys_vm_read(history_buf, sec_addr, 512) == -1)
    {
        return FALSE;
    }
    
    //更新路径记忆
    libc_memcpy(history_buf + offset, history_addr, path_size);
    if(sys_vm_write(history_buf, sec_addr) == -1)
    {
        return FALSE;
    }
    
    //销毁动态内存空间，并清除标志
    if(index->type == PATH_HISTORY_MENU)
    {
        #ifndef PC
        sys_free(&menu_history_addr);
        #endif
        menu_history_valid = FALSE;
    }
    else
    {
        #ifndef PC
        sys_free(&dir_history_addr);
        #endif
        dir_history_valid = FALSE;
    }
    
    return TRUE;
}

/******************************************************************************/
/*!                    
 * \par  Description:
 *    将所有路径记忆VRAM 数据区清为无效（0xff）。
 * \param[in]    type 路径类型， PATH_HISTORY_MENU 表示菜单， PATH_HISTORY_DIR 表示文件浏览
 * \param[out]   none
 * \return       bool
 * \retval           TRUE 清空路径记忆成功。
 * \retval           FALSE 清空路径记忆成功。
 * \ingroup      common_func
 * \par          exmaple code
 * \code
    例子1：清除所有应用菜单列表路径记录，在系统启动时调用
    com_clear_all_history(PATH_HISTORY_MENU);
    
    例子2：清除所有应用文件浏览路径记录，在创建播放列表时调用
    com_clear_all_history(PATH_HISTORY_DIR);
 * \endcode
 * \note 
*******************************************************************************/
bool com_clear_all_history(uint8 type)
{
    uint32 sec_addr;
    uint16 sec_count, i;
    
    if(type == PATH_HISTORY_MENU)
    {
        //菜单列表
        sec_addr = VM_MENU_HISTORY;
        sec_count = MENU_PATH_SEC_MAX;
    }
    else
    {
        //文件浏览器
        sec_addr = VM_DIR_HISTORY;
        sec_count = DIR_PATH_SEC_MAX;
    }
    
    libc_memset(history_buf, 0xff, 512);
    for(i = 0; i < sec_count; i++)
    {
        if(sys_vm_write(history_buf, sec_addr) == -1)
        {
            return FALSE;
        }
        sec_addr += 512;
    }
    
    return TRUE;
}

/******************************************************************************/
/*!                    
 * \par  Description:
 *    将指定AP路径记忆VRAM 数据区清为无效（0xff）。
 * \param[in]    type 路径类型，PATH_HISTORY_MENU表示菜单，PATH_HISTORY_DIR表示文件浏览
 * \param[in]    app_id 指定要清除路径记录AP
 * \param[in]    disk 指定要清除的盘符，可为 DISK_C，或 DISK_H，或 DISK_U
 * \param[out]   none
 * \return       bool
 * \retval           TRUE 清空路径记忆成功。
 * \retval           FALSE 清空路径记忆成功。
 * \ingroup      common_func
 * \par          exmaple code
 * \code
    例子1：清除某应用文件浏览路径记录，比如文件选择器出现错误时调用
    com_clear_app_history(PATH_HISTORY_DIR, APP_ID_PICTURE, DISK_C);
 * \endcode
 * \note 
 * \li  该接口暂时只能清除指定AP文件浏览器路径记忆，不支持菜单。
*******************************************************************************/
bool com_clear_app_history(uint8 type, uint8 app_id, uint8 disk)
{
    uint32 base_addr;
    uint32 sec_addr;
    uint32 offset;
    uint32 path_size;
    
    //只对文件浏览器有效
    if(type != PATH_HISTORY_DIR)
    {
        return FALSE;
    }
    
    //读出要清除配置项所在的扇区
    path_size = DIR_HISTORY_ONE;
    base_addr = VM_DIR_HISTORY + app_id * (path_size * DIR_PATH_ID_MAX) + get_dir_path_id(disk) * path_size;
    offset = base_addr & 0x1ff;
    sec_addr = base_addr - offset;
    if(sys_vm_read(history_buf, sec_addr, 512) == -1)
    {
        return FALSE;
    }
    
    //清除路径记忆
    libc_memset(history_buf + offset, 0xff, path_size);
    //设置路径记忆项
    if(sys_vm_write(history_buf, sec_addr) == -1)
    {
        return FALSE;
    }
    
    return TRUE;
}

/*! \endcond */

/*! \cond COMMON_API */

///获取文件浏览器路径记录 path_id，DISK_C为0，DISK_H为1，DISK_U为2
static uint8 get_dir_path_id(uint8 disk)
{
    if(disk == DISK_C)
    {
        return 0;
    }
    else if(disk == DISK_H)
    {
        return 1;
    }
    else//disk == DISK_U
    {
        return 2;
    }
}

/*! \endcond */
