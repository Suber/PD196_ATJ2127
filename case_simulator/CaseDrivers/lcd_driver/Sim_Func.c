//#include "Simulator.h"

/***************************************
该文件主要用于模拟部分常用的显示驱动接口
****************************************/
//#include "drv_s6b33b0a.h"
#include "display.h"

//extern DLL_IMP char gbLCMBuff[];
// extern DLL_IMP char DataTransTemp[];
//extern DLL_IMP char RefreshFlag;
//extern DLL_IMP void RefreshDisplayPC(void);
extern DLL_IMP uint32 display_buf_addr;//采屏仅作为中转站 最终用gbLCMBuff显示，黑白屏则直接用DisplayBuffer显示
//uint32 display_buf_addr = 0x3000;
extern DLL_IMP  unsigned char gbframe_buffer_addr_pc[];
extern DLL_IMP int updatescreen_flag;
//extern DLL_IMP int  gbDisplayWidth;
//extern DLL_IMP int  gbDisplayHight;
int  gbDisplayWidth = DISPLAY_LENGTH;
int  gbDisplayHight = DISPLAY_HEIGHT;

region_t region;
uint8 trans_mode;
uint8 pic_or_char = 2;	// 2 - pic   0 - char

uint16 x_counter, y_counter;
//uint8 DMA3_RCNT; 
//uint8 DMA3_CCNTH; 
//uint8 DMA3_CCNTL; 
uint32 pixel_cnt; 
//uint8 DMA3_CCNTH; 
//uint8 DMA3_CCNTL; 
typedef void*(*lcd_op_func)(void*, void*, void*);


typedef struct{
lcd_op_func   set_window;
lcd_op_func   set_contrast;
lcd_op_func   set_draw_mode;
lcd_op_func   standby_screen;
lcd_op_func   backlight_on_off;
lcd_op_func   update_screen;
lcd_op_func   buff_data_trans;
lcd_op_func   get_buff_data;
lcd_op_func   invert_buff_data_trans;
lcd_op_func   controller_init;
lcd_op_func   dma_set_counter;
lcd_op_func   dma_start_trans;
lcd_op_func   dma_set_src_addr;
lcd_op_func   mcu_set_JRAM_clk;
lcd_op_func   restore_JRAM_clk;
}lcd_driver_operations;





lcd_driver_operations lcd_driver_op={
(lcd_op_func)set_window,
(lcd_op_func)set_contrast,
(lcd_op_func)set_draw_mode,
(lcd_op_func)standby_screen,
(lcd_op_func)backlight_on_off,
(lcd_op_func)update_screen,
(lcd_op_func)buff_data_trans,
(lcd_op_func)get_buff_data,
(lcd_op_func)invert_buff_data_trans,
(lcd_op_func)controller_init,
(lcd_op_func)dma_set_counter,
(lcd_op_func)dma_start_trans,
(lcd_op_func)dma_set_src_addr,
(lcd_op_func)mcu_set_JRAM_clk,
(lcd_op_func)restore_JRAM_clk,
};



void lcd_drv_init(void)
{
    return;
}


WORD ConvertColor(WORD color)
{
	return ((color&0x00ff)<<8) + ((color&0xff00)>>8);
}

//将SoureBufPos字库上下颠倒 cnt: 字符列数  SoureBufPos：点阵数组
void ReplacePoint(int cnt,char *SoureBufPos)
{
	int i;
	BYTE btemp;

	if(cnt == 8)
	{
		for(i=0; i<16/2; i++)
		{
			btemp = SoureBufPos[i];
			SoureBufPos[i] = SoureBufPos[15-i];
			SoureBufPos[15-i] = btemp;
		}
	}
	else if(cnt == 16)
	{
		for(i=0; i<16/2; i++)
		{
			//左半
			btemp = SoureBufPos[2*i];
			SoureBufPos[2*i] = SoureBufPos[30-2*i];
			SoureBufPos[30-2*i] = btemp;
			//右半
			btemp = SoureBufPos[2*i+1];
			SoureBufPos[2*i+1] = SoureBufPos[32-(2*i+1)];
			SoureBufPos[32-(2*i+1)] = btemp;
		}
	}
}

void set_window(region_t *rgn,void *null2,void *null3)
{
	region = *rgn;	
	x_counter = 0;
	y_counter = 0;
}

void set_contrast(uint8 contrast_value,void *null2,void *null3)
{
	
}



//用图方向模式定义
// #define  DrawModeH_def         0  //横向优先
// #define  DrawModeV_def         1  //纵向优先
// #define  DrawModeHPic_def    2  //看图横向模式
// #define  DrawModeVPic_def    3  //看图纵向模式
// #define  DrawModeAvi_def     4  //AVI模式
// #define  DrawModeJpg_def    5  
// #define  DrawModeBMP_def   6
void set_draw_mode(uint8 mode,void *null2,void *null3)
{
	// 设置图片或者字符串模式
	pic_or_char = mode;
			
}

void standby_screen(bool standby_flag,void *null2,void *null3)
{
	
}

void backlight_on_off(bool on_off,void *null2,void *null3)
{
	
}

void update_screen(region_t *rgn,void*null2,void *null3)
{

}

//外部数据总线传输，高字节在前
void buff_data_trans(uint8 *buff,uint16 pix_cnt,void *null3)
{
	uint16 i;	
	updatescreen_flag = 1;
	
	if(pic_or_char == DRAW_MODE_H_PIC_DEF ) //从左到右，从下到上
	{
	    for(i=0; i<pix_cnt; i++)
	    {        	                   	
	        if(x_counter >= region.width)
			{
                x_counter = 0;
	            y_counter++;
	        }
	        gbframe_buffer_addr_pc[(region.y + region.height - 1 - y_counter)*gbDisplayWidth*2 + region.x*2 + x_counter*2] = buff[i * 2];
	        gbframe_buffer_addr_pc[(region.y + region.height - 1 - y_counter)*gbDisplayWidth*2 + region.x*2 + x_counter*2 + 1] = buff[i * 2 + 1];
	        x_counter++;
	    }
	}	
	else if(pic_or_char == DRAW_MODE_H_DEF) //从左到右，从上到下
	{
	    for(i=0; i<pix_cnt; i++)
	    {    
	    	if(x_counter >= region.width)
			{
                x_counter = 0;
	            y_counter++;
	        }    	                   	       
	        gbframe_buffer_addr_pc[(region.y + y_counter)*gbDisplayWidth*2 + region.x*2 + x_counter*2] = buff[i * 2];
	        gbframe_buffer_addr_pc[(region.y + y_counter)*gbDisplayWidth*2 + region.x*2 + x_counter*2 + 1] = buff[i * 2 + 1];
	        x_counter++;
	    }
	}	
	else if(pic_or_char == DRAW_MODE_V_DEF)  //从下到上，从左到右
	{
	    for(i=0; i<pix_cnt*2; i+=2)
	    {    
	    	if(y_counter >= region.width)
			{
                y_counter = 0;
	            x_counter++;
	        }    	                   	       
			gbframe_buffer_addr_pc[(region.y + region.width - 1 - y_counter)*gbDisplayWidth*2 + region.x*2 + x_counter*2] = buff[i];
			gbframe_buffer_addr_pc[(region.y + region.width - 1 - y_counter)*gbDisplayWidth*2 + region.x*2 + x_counter*2 + 1] = buff[i+1];
			y_counter++;
	    }
	}	
	else if(pic_or_char == DRAW_MODE_V_PIC_DEF ) //从左到右，从下到上
	{
	    for(i=0; i<pix_cnt; i++)
	    {        	                   	
	        if(x_counter >= region.width)
			{
                x_counter = 0;
	            y_counter++;
	        }
	        gbframe_buffer_addr_pc[(region.y + region.height - 1 - y_counter)*gbDisplayWidth*2 + region.x*2 + x_counter*2] = buff[i * 2];
	        gbframe_buffer_addr_pc[(region.y + region.height - 1 - y_counter)*gbDisplayWidth*2 + region.x*2 + x_counter*2 + 1] = buff[i * 2 + 1];
	        x_counter++;
	    }
	}
	else if(pic_or_char == DRAW_MODE_H_SCROLL_DEF)//从上到下，从左到右
	{
		for(i=0; i<pix_cnt; i++)
		{
			if(y_counter >= region.height)
			{
				y_counter = 0;
				x_counter++;
			}
			gbframe_buffer_addr_pc[(region.y + y_counter)*gbDisplayWidth*2 + region.x*2 + x_counter*2] = buff[i * 2];
	        gbframe_buffer_addr_pc[(region.y + y_counter)*gbDisplayWidth*2 + region.x*2 + x_counter*2 + 1] = buff[i * 2 + 1];
	        y_counter++;
		}
	}

	//RefreshFlag = 1;
}

/*
//** Description:  把LCM上从当前位置开始的 datacnt 个象素的数据读出到指定的Buff中。
//**
//**  input
//**    char  *buff ：输出缓冲区的首址；(de)
//**    int datacnt ：要输出的像素个数；(bc)
*/
void get_buff_data(uint8 *buff,uint16 pix_cnt,void *null3)
{
	uint16 i;

	updatescreen_flag = 1;
	
	if(pic_or_char == DRAW_MODE_H_PIC_DEF ) //从左到右，从下到上
	{
	    for(i=0; i<pix_cnt; i++)
	    {        	                   	
	        if(x_counter >= region.width)
			{
                x_counter = 0;
	            y_counter++;
	        }
	        buff[i * 2] = gbframe_buffer_addr_pc[(region.y + region.height - 1 - y_counter)*gbDisplayWidth*2 + region.x*2 + x_counter*2];
	        buff[i * 2 + 1] = gbframe_buffer_addr_pc[(region.y + region.height - 1 - y_counter)*gbDisplayWidth*2 + region.x*2 + x_counter*2 + 1];
	        x_counter++;
	    }
	}	
	else if(pic_or_char == DRAW_MODE_H_DEF) //从左到右，从上到下
	{
	    for(i=0; i<pix_cnt; i++)
	    {    
	    	if(x_counter >= region.width)
			{
                x_counter = 0;
	            y_counter++;
	        }    	                   	       
	        buff[i * 2] = gbframe_buffer_addr_pc[(region.y + y_counter)*gbDisplayWidth*2 + region.x*2 + x_counter*2];
	        buff[i * 2 + 1] = gbframe_buffer_addr_pc[(region.y + y_counter)*gbDisplayWidth*2 + region.x*2 + x_counter*2 + 1];
	        x_counter++;
	    }
	}	
	else if(pic_or_char == DRAW_MODE_V_DEF)  //从下到上，从左到右
	{
	    for(i=0; i<pix_cnt*2; i+=2)
	    {    
	    	if(y_counter >= region.width)
			{
                y_counter = 0;
	            x_counter++;
	        }    	                   	       
			buff[i] = gbframe_buffer_addr_pc[(region.y + region.width - 1 - y_counter)*gbDisplayWidth*2 + region.x*2 + x_counter*2];
			buff[i+1] = gbframe_buffer_addr_pc[(region.y + region.width - 1 - y_counter)*gbDisplayWidth*2 + region.x*2 + x_counter*2 + 1];
			y_counter++;
	    }
	}	
	if(pic_or_char == DRAW_MODE_V_PIC_DEF ) //从左到右，从下到上
	{
	    for(i=0; i<pix_cnt; i++)
	    {        	                   	
	        if(x_counter >= region.width)
			{
                x_counter = 0;
	            y_counter++;
	        }
	        buff[i * 2] = gbframe_buffer_addr_pc[(region.y + region.height - 1 - y_counter)*gbDisplayWidth*2 + region.x*2 + x_counter*2];
	        buff[i * 2 + 1] = gbframe_buffer_addr_pc[(region.y + region.height - 1 - y_counter)*gbDisplayWidth*2 + region.x*2 + x_counter*2 + 1];
	        x_counter++;
	    }
	}	

	//RefreshFlag = 1;
}

void invert_buff_data_trans(uint8 *buff,uint16 pix_cnt,void *null3)
{
}

void controller_init(uint8 lcd_mode,void *null2,void *null3)
{
    trans_mode = lcd_mode;
}

void dma_set_counter(uint32 pix_cnt,uint16 data_width,void *null3)
{
   // DMA3_RCNT = row; //CNT常设为0
  //  DMA3_CCNTH = (uint8) (col >> 8); //set DMA3 Column high counter
   // DMA3_CCNTL = (uint8) col; //set DMA3 Column low counter
	pixel_cnt = pix_cnt;
}

bool dma_start_trans(uint8 dma_ram, void *null2,void *null3)
{
    
	uint8 *DisplayBuffer;
	      
    // 将解码数据搬到显示屏上
    //DisplayBuffer = GET_REAL_ADDR(display_buf_addr);
    //buff_data_trans(DisplayBuffer, pixel_cnt,0);
    if (trans_mode == RGB_MODE)
    {
        if (dma_ram == LCD_RAM_INDEX)
        {
            dma_set_src_addr(LCD_BUF_ADDR, 0, 0);
            DisplayBuffer = GET_REAL_ADDR(display_buf_addr);
        }
        else if (dma_ram == JPEG_RAM_INDEX)
        {
            dma_set_src_addr(JPEG_BUF_ADDR, 0, 0);
            DisplayBuffer = GET_REAL_ADDR(display_buf_addr);
        }
        else
        {
            DisplayBuffer = display_buf_addr;
        }
    }
    buff_data_trans(DisplayBuffer, pixel_cnt,0);

    return TRUE;
}

void dma_set_src_addr(uint32 addr0,uint32 addr1,uint32 addr2)
{
	if (trans_mode == RGB_MODE)
	{
        //DisplayBuffer = GET_REAL_ADDR(addr0);
        display_buf_addr = addr0;
    }
}

void mcu_set_JRAM_clk(void *null1, void *null2,void *null3)
{

}

void restore_JRAM_clk(void *null1, void *null2,void *null3)
{

}
