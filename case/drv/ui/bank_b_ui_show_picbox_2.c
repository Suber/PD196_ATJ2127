/*******************************************************************************
 *                              US212A
 *                            Module: ui driver
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>           <time>          <version >             <desc>
 *       liutingting     2011-09-05 10:00     1.0             build this file
 *******************************************************************************/
/*!
 * \file    ui_show_picbox.c
 * \brief   显示picbox的模块
 * \author   liutingting
 * \par      GENERAL DESCRIPTION:
 *               实现picbox控件数据的解析并显示picbox
 * \par      EXTERNALIZED FUNCTIONS:
 *              直接调用系统SD文件系统的接口函数解析并显示picbox
 * \version 1.0
 * \date  2011-9-05
 *******************************************************************************/
#include "display.h"
#include "ui_driver.h"
//void buff_data_trans(uint8 *buff, uint16 pix_cnt, void *null3);

uint8 *u_data_buf=NULL;

#define PICBOX_SHOW_EN                  (0x0001 << 0)
#define PICBOX_FRAME_OPTION_SHIFT       (1)
#define PICBOX_FRAME_OPTION             (0x0001 << PICBOX_FRAME_OPTION_SHIFT)
#define PICBOX_FRAME_OPTION_ID          (0x0000 << PICBOX_FRAME_OPTION_SHIFT)
#define PICBOX_FRAME_OPTION_ADDR        (0x0001 << PICBOX_FRAME_OPTION_SHIFT)
#define PICBOX_FRAME_COUNT_SHIFT        (8)
#define PICBOX_FRAME_COUNT(attrib)      (((attrib) >> PICBOX_FRAME_COUNT_SHIFT) & 0xff)

#define ONE_PAGE_READ_NUM			(2)


bool u_put_sd_image(sd_handle *res_fp, uint32 img_addr, region_t *img_region)
{
#ifndef PC
    uint8 *display_buffer = (uint8 *) display_buf_addr;
#else
    uint8 *display_buffer = GET_REAL_ADDR(display_buf_addr);
#endif
    uint32 cnt;
    uint32 start_addr,next_start_addr;
    uint16 count;
    bool result = TRUE;
    //uint16 i,j,m,k=0;
    region_t tmp_region;
	uint16 read_loop_cnt;
	bool Segmentation_Read_Flag;//是否分段读标准。
	uint8 Segmentation_Num;
	uint8 page_num;//图片占Page数
    uint8 page_num_cnt;
	uint8 addr_num;
	uint16  page_data_len;
	
    //检测显示区域是否超出
    if (((img_region->x + img_region->width) > (region_win.x + region_win.width))
     || ((img_region->y + img_region->height) > (region_win.y + region_win.height)))
    {
        return FALSE;//X轴或者Y轴越界
    }
    
    //初始化数据大小及下次读取地址，以及缓冲区地址
    cnt = ((uint32) img_region->width) * ((uint32) img_region->height) * 2;
    start_addr = img_addr;
    libc_memcpy(&tmp_region, img_region, sizeof(region_t));
   if(lcd_buffer_type == LCD_RAM_INDEX)
   {

   tmp_region.height = 8;
   page_data_len = ((uint32) img_region->width) * 8 * 2;
  	page_num  = (uint8)img_region->height/8;
	if (img_region->height%8 !=0)
	{
		page_num++;
	}
	Segmentation_Read_Flag = FALSE;
	page_num_cnt = 0;
		
    while (page_num_cnt < page_num)
    {
        //读取数据
	
		if (page_data_len <= (uint16) LCD_BUF_LEN)
        {
            count = (uint16) page_data_len;
			Segmentation_Read_Flag = FALSE;
			read_loop_cnt = 1;
        }
        else
        {
			read_loop_cnt = 0;
			tmp_region.width = 64;
			tmp_region.x = img_region->x;
			Segmentation_Read_Flag  = TRUE;
        }        

		while(read_loop_cnt < ONE_PAGE_READ_NUM)
		{
			if(Segmentation_Read_Flag  == TRUE)
			{				
				Segmentation_Num = 8;
				
				if(read_loop_cnt == 0) //page分段的第一部分
				{
					tmp_region.width = 64;
					count = (uint16)tmp_region.width * 2;
					tmp_region.x = img_region->x;
					start_addr = img_addr + page_num_cnt * img_region->width * 8 * 2; 
				}
				else //page分段的第二部分
				{
					tmp_region.width = img_region->width - 64;
					count = (uint16)tmp_region.width * 2;

					tmp_region.x = img_region->x + 64;
					start_addr = img_addr + page_num_cnt * img_region->width * 8 * 2 + 64 * 2; 
					
				}
			}
			else
			{
				Segmentation_Num = 1;
				
				start_addr = img_addr + page_num_cnt * img_region->width * 8 * 2;
			}
			
#ifndef PC
		    display_buffer = display_buf_addr;
#else
		    display_buffer = GET_REAL_ADDR(display_buf_addr);
#endif

			while(Segmentation_Num)
			{
		        sys_sd_fseek(res_fp, SEEK_SET, start_addr);
		        sys_sd_fread(res_fp, display_buffer, (uint32) count);

				display_buffer = display_buffer + count;
				
				Segmentation_Num--;
				start_addr = start_addr + img_region->width * 2;
			}
			
			//设置图片显示模式及窗口
	      	set_lcd_param_hook(&tmp_region, LCD_PARAM_IMAGE);
	        dma_buffer_data_hook(0, 0);				

	        //更新剩余数据大小及下次读取地址
			
			read_loop_cnt++;
		}
			
		page_num_cnt++;
		tmp_region.y += 8;
    }
   }
   else
   {

		tmp_region.height = 16;

		while (cnt > 0)
		{
		    //读取数据
		    if (cnt >= (uint32) display_buf_len)
		    {
		        //count = display_buf_len;
				count = ((uint32) img_region->width) * 16 * 2;
		    }
		    else
		    {
		        count = (uint16) cnt;
		    }
		    sys_sd_fseek(res_fp, SEEK_SET, start_addr);
		    sys_sd_fread(res_fp, display_buffer, (uint32) count);
			
			//设置图片显示模式及窗口
		  	set_lcd_param_hook(&tmp_region, LCD_PARAM_IMAGE);
		    dma_buffer_data_hook(0, 0);
		   
		    //更新剩余数据大小及下次读取地址
		    tmp_region.y += 16;
		    cnt = cnt - count;
		    start_addr += count;
		}
			
   }
   
    result = TRUE;
 
    return result;
}



