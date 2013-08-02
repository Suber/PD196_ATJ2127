/********************************************************************************
 *                              USDK
 *                             Module: ebook
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : ebook_fileoperate.c
 * History:
 *      <author>    <time>           <version >             <desc>
 *      stevenluo       2011-11-11         v1.0              create this file
 ********************************************************************************/

#include  "ebook.h"


//检测是否为不带BOM的utf-8

bool Is_utf8_nobom(uint8 * str,uint16 len)
{
//        编码规则，字母x表示可用编码的位。
//        'Unicode符号范围       | UTF-8编码方式
//        '(十六进制)            | （二进制）
//        '----------------------+---------------------------------------
//        '0000 0000-0000 007F   | 0xxxxxxx
//        '0000 0080-0000 07FF   | 110xxxxx 10xxxxxx
//        '0000 0800-0000 FFFF   | 1110xxxx 10xxxxxx 10xxxxxx
//        '0001 0000-0010 FFFF   | 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
//        '==============================================================
//        
//        单个字情况下 UNICODE 与 UTF-8 有重叠的部分:
//        第一个字节          第二个字节     
//        0xCX-0xDX           0x8X-0xBX     
//        例如:'联'、'通'等
//        所以需要多次判断，最终才能准确识别

#define CHECH_UTF8_TIME     5
    uint8           maxcheck_utf8_time;
    uint16          index;
    uint8           offset;
    uint8           check_utf8_time=0;
    
    if( (CHECH_UTF8_TIME * 4) > len )
    {
        maxcheck_utf8_time = 1;                 //太少了，则只校验1次
    }
    else
    {
        maxcheck_utf8_time=CHECH_UTF8_TIME;
        len    -=  4;
    }
    
    for(index=0,offset=0;index< len ;index+=(offset+1),offset=0)
    {
        //识别到超过一定次数，就返回
        if(check_utf8_time >= maxcheck_utf8_time )
        {
            return  TRUE; 
        }
      
        //0xxxxxxx 开头是不做处理,ANSI与UTF-8都是一致的
        if ((str[index   ] & 0x80 ) == 0x00) 
        {
            continue;
        }
        
        //10xxxxxx 开头是不可能出现的
        if ((str[index   ] & 0xC0 ) == 0x80) 
        {
            return  FALSE; 
        }
        
        //110xxxxx 10xxxxxx   
        if ((str[index   ] & 0xE0 ) == 0xC0) 
        {
            //可能出现部分中文误识别
            if  (   ((str[index+1 ] & 0xC0 ) == 0x80) )
            {
                check_utf8_time++; 
                offset =1;
                continue;
            }
            return  FALSE; 
        }
        
        //1110xxxx 10xxxxxx 10xxxxxx
        if ((str[index   ] & 0xF0 ) == 0xE0) 
        {
            if  (   ((str[index+1 ] & 0xC0 ) == 0x80) &&
                    ((str[index+2 ] & 0xC0 ) == 0x80)  )
            {
                check_utf8_time++; 
                offset = 2;
                continue;
            }
            return  FALSE; 
        }

        //11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
        if ((str[index   ] & 0xF8 ) == 0xF0) 
        {
            if (    ((str[index+1 ] & 0xC0 ) == 0x80) &&
                    ((str[index+2 ] & 0xC0 ) == 0x80) &&
                    ((str[index+3 ] & 0xC0 ) == 0x80)  )
            {
                offset =3;
                check_utf8_time++;
                continue;
            }
            return  FALSE; 
        }

        //111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
        if ((str[index   ] & 0xFC ) == 0xF8) 
        {
            if (    ((str[index+1 ] & 0xC0 ) == 0x80) &&
                    ((str[index+2 ] & 0xC0 ) == 0x80) &&
                    ((str[index+3 ] & 0xC0 ) == 0x80) &&
                    ((str[index+4 ] & 0xC0 ) == 0x80)  )
            {
                offset =4;
                check_utf8_time++;
                continue;
            }
            return  FALSE; 
        }
        
        //1111110x 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
        if ((str[index   ] & 0xFE ) == 0xFC) 
        {
            if (    ((str[index+1 ] & 0xC0 ) == 0x80) &&
                    ((str[index+2 ] & 0xC0 ) == 0x80) &&
                    ((str[index+3 ] & 0xC0 ) == 0x80) &&
                    ((str[index+4 ] & 0xC0 ) == 0x80) &&
                    ((str[index+5 ] & 0xC0 ) == 0x80)  )
            {
                offset =5;
                check_utf8_time++;
                continue;
            }
            return  FALSE; 
        }
        
        //11111110 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
        if (str[index   ]  == 0xFE) 
        {
            if (    ((str[index+1 ] & 0xC0 ) == 0x80) &&
                    ((str[index+2 ] & 0xC0 ) == 0x80) &&
                    ((str[index+3 ] & 0xC0 ) == 0x80) &&
                    ((str[index+4 ] & 0xC0 ) == 0x80) &&
                    ((str[index+5 ] & 0xC0 ) == 0x80) &&
                    ((str[index+6 ] & 0xC0 ) == 0x80)  )
            {
                offset =6;
                check_utf8_time++;
                continue;
            }
            return  FALSE; 
        }
        
        //11111111 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx   
        if (str[index   ]  == 0xFF) 
        {
            if (    ((str[index+1 ] & 0xC0 ) == 0x80) &&
                    ((str[index+2 ] & 0xC0 ) == 0x80) &&
                    ((str[index+3 ] & 0xC0 ) == 0x80) &&
                    ((str[index+4 ] & 0xC0 ) == 0x80) &&
                    ((str[index+5 ] & 0xC0 ) == 0x80) &&
                    ((str[index+6 ] & 0xC0 ) == 0x80) &&
                    ((str[index+7 ] & 0xC0 ) == 0x80)  )
            {
                offset =7;
                check_utf8_time++;
                continue;
            }
            return  FALSE; 
        }
        
        return  FALSE;               //不是utf-8 开头的，则肯定不是utf-8，直接结束
        
    }
    return  FALSE;                  //循环里都没符合条件，则认为不是utf-8
}




















