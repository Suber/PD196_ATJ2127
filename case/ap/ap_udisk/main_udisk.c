/*******************************************************************************
 *                              US212A
 *                            Module: udiskap
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       yliang     2011-11-07      1.0             build this file
 *******************************************************************************/
/*!
 * \file     main_udisk.c
 * \brief    main for ap_udisk
 * \author   yliang
 * \version  1.0
 * \date  2011/10/07
 *******************************************************************************/
#include  "ap_udisk.h"

res_size_t res_connect_size[10]; //资源信息缓存
uint32 update_flag = 0;     //bit0为1表示Flash盘被写过，bit16为1表示卡盘被写过
uint8 returnkey_press = 0;  //收到返回按键标志
uint8 iscardout = 0;    //卡拔出标志
uint8 usbstatus;        //USB当前工作状态
uint8 usbstatus_bak;    //暂存USB工作状态，用于区分前后状态是否改变
charge_state_e charge_state;     //当前充电状态，从充电使能状态取得
charge_state_e charge_state_bak; //用于暂存充电状态，用于区分前后状态是否改变
//定时器id
int8 timer_usb_connect_id;  //USB忙图标显示定时器ID
int8 timer_usb_battery_id;  //电池充电显示定时器ID
uint8 time_count;   //时间计算器
/*main中要填充这两个值，有系统人员分配*/
OS_STK *ptos = (OS_STK *) AP_UDISK_STK_POS;
INT8U prio = AP_UDISK_PRIO;
/*!
 * \brief
 *  udisk_key_map_list：udisk应用按键映射表格
 */
const key_map_t udisk_key_map_list[] =
{
    {{ KEY_MODE, 0, KEY_TYPE_SHORT_UP }, EVENT_UDISK_EXIT_LAST },
    /*{{ KEY_PLAY, 0, KEY_TYPE_SHORT_UP }, EVENT_UDISK_EXIT_MTP },*/
    {{ KEY_NULL, 0, KEY_TYPE_NULL }, MSG_NULL }, 
};
//const uint16 style_usb_point[5] =
//{ UDISK_POINT_5, UDISK_POINT_4, UDISK_POINT_3, UDISK_POINT_2, UDISK_POINT_1 };
/*全局变量定义*/
//应用自己需要的软定时器个数
#define APP_TIMER_COUNT     2
/*!
 *  \brief
 *  app_timer_vector：应用软定时器数组指针，指向应用空间的软定时器数组
 */
app_timer_t usb_app_timer_vector[COMMON_TIMER_COUNT + APP_TIMER_COUNT];
void msgmanager(void);
app_result_e usb_main_control(int argc);

/******************************************************************************/
/*!
 * \par  Description:
 *    message mananger
 * \param[in]
 * \param[out]   none
 * \retval       none
 * \ingroup      main_udisk.c
 * \note
 *******************************************************************************/
void msgmanager(void)
{
    bool msg_result;
    input_gui_msg_t udisk_gui_msg;
    msg_apps_type_e gui_event;
    private_msg_t udisk_app_msg;

    if (returnkey_press == 1) //已收到return键,不再收消息,以防非空闲状态下跑该函数把KeyUp收走
    {
        return;
    }
    /*处理gui消息*/
    msg_result = get_gui_msg(&udisk_gui_msg);
    if (msg_result == TRUE)
    {
        if (com_key_mapping(&udisk_gui_msg, &gui_event, udisk_key_map_list) == TRUE)
        {
            if(gui_event == EVENT_UDISK_EXIT_LAST)
            {
                if (usbstatus == 0)
                {
                    returnkey_press = 0x01;
                    return;
                }
            }
            #if 0
            else if(gui_event == EVENT_UDISK_EXIT_MTP)
            {
                if (usbstatus == 0)
                {
                    playkey_press = 0x01;
                    return;
                }
            }
            #endif
            else if((gui_event == EVENT_ENTER_KEYLOCK) || (gui_event == EVENT_ENTER_KEYLOCK_UN))
            {
                //直接把有效执行代码复制过来，以免发生bank切换
                if(get_keylock_state() == TRUE)
                {
                    change_keylock_state(FALSE);
                }
                else
                {
                    change_keylock_state(TRUE);
                }
            }
            else
            {
            }
        }
    }
    msg_result = get_app_msg(&udisk_app_msg);
    if (msg_result == TRUE)
    {
        if(udisk_app_msg.msg.type == MSG_SD_OUT)
        {
            iscardout = 0x01;
            CardDetectState = -1;
        }
        else if(udisk_app_msg.msg.type == MSG_SD_IN)   
        {
            CardDetectState = 0;
        }
        else if(udisk_app_msg.msg.type == MSG_EARPHONE_IN)//耳机（天线）插入
        {
            change_antenna_state(TRUE);
        }
        else if(udisk_app_msg.msg.type == MSG_EARPHONE_OUT)//耳机（天线）拔出
        {
            change_antenna_state(FALSE);
        }
        else
        {
        }
    }
}

/******************************************************************************/
/*!
 * \par  Description:
 *    MainMenu 应用的入口函数和场景管理器模块
 * \param[in]    0, 无播放模式，1，正在播放模式，2，上一次播放模式
 * \param[out]   none
 * \return       the result
 * \retval       0 sucess
 * \retval       1 failed
 * \ingroup      module name
 * \par          exmaple code
 * \code
 *               list example code in here
 * \endcode
 * \note
 *******************************************************************************/
int main(int argc, const char *argv[])
{
    app_result_e result;

    //初始化applib库，前台AP
    applib_init(APP_ID_UDISK, APP_TYPE_GUI);

    //初始化软定时器
    init_app_timers(usb_app_timer_vector, COMMON_TIMER_COUNT + APP_TIMER_COUNT);

    //初始化 applib 消息模块
    applib_message_init(NULL);

    /*读取ui资源文件*/
    ui_res_open("udisk.sty", UI_AP);
    ui_res_open("common.sty", UI_COM);

    CardDetectState = 0;

    key_chargeset(CHARGE_START, ChargeCurrent250mA, BATFULL_LEVEL1);
    charge_state = (charge_state_e) (uint32) key_chargeget(0);

    result = usb_main_control(argc);
    ui_set_display_buf(JPEG_RAM_INDEX);

    /*关闭ui资源文件*/
    ui_res_close(UI_AP);
    ui_res_close(UI_COM);
    //执行applib库的注销操作
    applib_quit();
    return result;
}

/******************************************************************************/
/*!
 * \par  Description:
 *    
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      mainmenu
 * \note
 *******************************************************************************/
static void get_usb_info(void)
{
    uint8 *info_p;
    char card_info[6] = {0};
    info_p = get_usb_data_pointer(0);
    if (NULL != info_p)
    {
        com_get_config_struct(USB_VID,info_p,6);
    }

    info_p = get_usb_data_pointer(1);
    if (NULL != info_p)
    {
        com_get_config_struct(USB_PID,info_p,6);
    }

    info_p = get_usb_data_pointer(2);
    if (NULL != info_p)
    {
        com_get_config_struct(INF_USB_VENDOR,info_p,8);
    }

    info_p = get_usb_data_pointer(3);
    if (NULL != info_p)
    {
        com_get_config_struct(INF_USB_PRODUCTID,info_p,16);
    }
    info_p = get_usb_data_pointer(4);
    if (NULL != info_p)
    {
        com_get_config_struct(INF_INQUIRY_INFORMATION_FOB,info_p,52); //CMD_Inquiry_information_FOB
    }
    info_p = get_usb_data_pointer(5);
    if (NULL != info_p)
    {
        com_get_config_struct(INF_INQUIRY_INFORMATION_CDROM,info_p,52); //CMD_Inquiry_information_CDROM
    }
    com_get_config_struct(INF_CARD_DISPLAY,(uint8 *)&card_info,3); //是否支持卡
    if ((('Y' == card_info[0]) || ('y' == card_info[0])) &&  
        (('E' == card_info[1]) || ('e' == card_info[1])))    
    {
        ShowMultiDiskFlag = 1;
    }
} 
/******************************************************************************/
/*!
 * \par  Description:
 *    MainMenu 读取应用VM全局变量,初始化全局变量
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      mainmenu
 * \note
 *******************************************************************************/
app_result_e usb_main_control(int argc)
{
    uint8 count_num;
    timer_usb_connect_id = -1;
    timer_usb_battery_id = -1;
    time_count = 0;
    usbstatus_bak = (uint8) - 1;
    charge_state_bak = CHARGE_NONE;
    get_usb_info();
    //调用usb初始化函数
    udisk_init_all();
    for (count_num = 0; count_num < 0x7f; count_num++)
    {
        ; //delay
    }
    //udisk ap主函数体
    while (1)
    {
        msgmanager(); //消息解释
        if (iscardout == 1)
        {
            udiskpullcardout();
            iscardout = 0;
        }
        udiskhandle(); //调用状态机执行命令
        usbstatus = udiskgetstatus();
        //0:空闲  10h:上传  20h:下传  30h:弹出要求退出  40h:拔线要求退出
        //60h:分区处理状态  70h:插入到USB充电器  80h:进入Suspend状态  90h:退出Suspend状态
        if ((usbstatus == 0x30) || (usbstatus == 0x40) || (usbstatus == 0x70) || (usbstatus == 0x51))
        {
            break;
        }
        display();
        /*if ((((returnkey_press == 1) || (playkey_press == 1)) && (usbstatus == 0)) || (usbstatus == 0x90))
         {
         break;
         }*/
        if (((returnkey_press == 1) && (usbstatus == 0)) || (usbstatus == 0x90))
        {
            break;
        }
        else
        {
            returnkey_press = 0;
            //playkey_press = 0;
        }
        //挂起10ms，多任务调度
        //sys_os_time_dly(2);
    }
    if (timer_usb_connect_id != -1)
    {
        kill_app_timer(timer_usb_connect_id);
    }
    if (timer_usb_battery_id != -1)
    {
        kill_app_timer(timer_usb_battery_id);
    }
    return udisk_exit_all(argc);
}

