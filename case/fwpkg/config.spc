tab=app;
DISP_EN=app config;
DISP_CH=应用配置;

tab=setting;
DISP_EN=app setting;
DISP_CH=系统配置;

tab=usb;
DISP_EN=usb property;
DISP_CH=usb属性;

tab=music;
DISP_EN=music property;
DISP_CH=音乐属性;

tab=photo;
DISP_EN=photo property;
DISP_CH=图片属性;

tab=radio;
DISP_EN=radio property;
DISP_CH=收音机属性;

tab=record;
DISP_EN=record property;
DISP_CH=录音属性;

key=CFG_SOFT_WATCHDOG_ENABLE;
type=DATA;
operate=SINGLE;
len=512;
RANGE=0,1;
tabname=app;
DISP_EN=soft watchdog enable:0-disable,1-enable;
DISP_CH=软看门狗开关:0-关闭,1-开启

key=MAINMENU_AP_ID_ARRAY;
type=ARRAY;
operate=EDIT;
len=;
RANGE=;
tabname=app;
DISP_EN=desktop ap array:0-music,1-audible,2-video,3-photo,4-text,5-browser,6-voice,7-record,8-radio,9-tools,10-setting,11,12,13reserve
DISP_CH=主界面ap顺序:0-音乐,1-audible,2-视频,3-图片,4-电子书,5-文件夹浏览,6-语音,7-录音,8-收音机,9-工具,10-设置,11,12,13预留

key=CFG_KEY_ARRAY;
type=ARRAY;
operate=EDIT;
len=;
RANGE=;
tabname=app;
DISP_EN=key array:0-NULL,1,2,8-reserve,3-mode,4-prev,5-play,6-next,7-V-,9-v+,10-vol
DISP_CH=按键定义:0-NULL,1,2,8-reserve,3-mode,4-prev,5-play,6-next,7-V-,9-v+,10-vol

key=SETTING_AP_ID_LIGHTNESS;
type=DATA;
operate=SINGLE;
len=;
RANGE=1-11;
tabname=setting;
DISP_EN=backlight;
DISP_CH=背光等级;

key=SETTING_AP_ID_SCREEN_SAVER_MODE;
type=DATA;
operate=SINGLE;
len=;
RANGE=0-4;
tabname=setting;
DISP_EN=screen save mode:0-none，1-digital clock，2-albumart，3-screen off，4-demo mode
DISP_CH=屏幕保护模式:0-无，1-数字时钟，2-相册图画，3-关闭屏幕，4-演示模式

key=SETTING_AP_ID_LIGHT_TIMER;
type=DATA;
operate=SINGLE;
len=;
RANGE=0-60(20);
tabname=setting;
DISP_EN=backlight time:1 means 0.5s
DISP_CH=背光时间:1表示0.5秒

key=SETTING_AP_ID_SCREEN_SAVER_TIMER;
type=DATA;
operate=SINGLE;
len=;
RANGE=0-180(30);
tabname=setting;
DISP_EN=screen save time:1 means 0.5s
DISP_CH=屏幕保护时间:1表示0.5秒

key=SETTING_AP_ID_SLEEP_TIMER;
type=DATA;
operate=SINGLE;
len=;
RANGE=0-99;
tabname=setting;
DISP_EN=sleep time:1 means 1min
DISP_CH=睡眠时间:1表示1分钟

key=SETTING_AP_ID_TIME_FORMAT;
type=DATA;
operate=SINGLE;
len=;
RANGE=0,1;
tabname=setting;
DISP_EN=time format:0-12 hours, 1-24hours
DISP_CH=时间格式:0表示12小时制，1表示24小时制

key=SETTING_AP_ID_DATE_FORMAT;
type=DATA;
operate=SINGLE;
len=;
RANGE=0,1,2;
tabname=setting;
DISP_EN=date format:0：DD_MM_YYYY 1：MM_DD_YYYY 2：YYYY_MM_DD
DISP_CH=日期格式:0：DD_MM_YYYY 1：MM_DD_YYYY 2：YYYY_MM_DD

key=SETTING_AP_ID_AUTORUN_SET;
type=DATA;
operate=SINGLE;
len=;
RANGE=0,1;
tabname=setting;
DISP_EN=auto run:0-disable,1-enable
DISP_CH=自动运行选项:0-关闭,1-开启

key=SETTING_AP_ID_SUPPORT_CARD;
type=DATA;
operate=SINGLE;
len=;
RANGE=0,1;
tabname=setting;
DISP_EN=support card:0-not support, 1-support
DISP_CH=支持卡选项:0-不支持,1-支持

key=SETTING_AP_ID_MUSIC_REPEAT_MODE;
type=DATA;
operate=SINGLE;
len=;
RANGE=0-3;
tabname=setting;
DISP_EN=music repeat mode:0-disbale,1-repeate one,2-repeate all,3-view playing
DISP_CH=循环方式:0-关闭,1-单曲循环,2-全部循环,3-浏览播放

key=SETTING_AP_ID_MUSIC_SHUFFLE;
type=DATA;
operate=SINGLE;
len=;
RANGE=0,16;
tabname=setting;
DISP_EN=shuffle:0-disable,16-enable
DISP_CH=随机播放:0-关闭,16-开启

key=SETTING_AP_ID_EQ_SETTING;
type=DATA;
operate=SINGLE;
len=;
RANGE=0,1,2,3,4,5,6,7;
tabname=setting;
DISP_EN=eq:0-disable 1-rock 2-funk 3-hip hop 4-jazz 5-classical 6-techno 7-custom;
DISP_CH=eq:0-关闭 1-遥滚 2-放克 3-希普霍普 4-爵士 5-古典 6-电子乐 7-自定义

key=SETTING_AP_ID_VOLUME_LIMIT;
type=DATA;
operate=SINGLE;
len=;
RANGE=0-31;
tabname=setting;
DISP_EN=volume limited;
DISP_CH=音量限制;

key=SETTING_AP_ID_VOLUME_CURRENT;
type=DATA;
operate=SINGLE;
len=;
RANGE=0-31;
tabname=setting;
DISP_EN=default volume;
DISP_CH=默认音量;

key=SETTING_AP_ID_SLIDE_TIME;
type=DATA;
operate=SINGLE;
len=;
RANGE=2-30;
tabname=setting;
DISP_EN=slide time;
DISP_CH=每张幻灯片播放时间;

key=SETTING_AP_ID_SLIDE_REPEAT_MODE;
type=DATA;
operate=SINGLE;
len=;
RANGE=0,2;
tabname=setting;
DISP_EN=slide mode:0-disbale, 2-enable
DISP_CH=幻灯重复开关:0-关闭,2-开启

key=SETTING_AP_ID_SLIDE_SHUFFLE;
type=DATA;
operate=SINGLE;
len=;
RANGE=0,1;
tabname=setting;
DISP_EN=slide suffle enable:0-disbale, 1-enable
DISP_CH=幻灯随机播放开关:0-关闭,1-开启


key=SETTING_AP_ID_POWEROFF_TIMER;
type=DATA;
operate=SINGLE;
len=;
RANGE=0-240;
tabname=setting;
DISP_EN=power save:1 means 0.5s
DISP_CH=省电关机时间:1表示0.5秒

key=SETTING_AP_ID_PLAYING_TIMER;
type=DATA;
operate=SINGLE;
len=;
RANGE=0-90;
tabname=setting;
DISP_EN=back to nowplay time:1 means 0.5s
DISP_CH=返回正在播放的时间:1表示0.5秒

key=PHOTO_AP_ID_THUMBNAIL;
type=DATA;
operate=SINGLE;
len=;
RANGE=0,1;
tabname=photo;
DISP_EN=support:0-not support, 1-support
DISP_CH=缩略图模块是否支持:0-否，1-是

key=PHOTO_AP_ID_REVOLVE;
type=DATA;
operate=SINGLE;
len=;
RANGE=0,1;
tabname=photo;
DISP_EN=photo auto rotate:0-no, 1-yes;
DISP_CH=图片是否自动旋转:0-否，1-是

key=MUSIC_LRC_SUPPORT;
type=DATA;
operate=SINGLE;
len=;
RANGE=0,1;
tabname=music;
DISP_EN=music lrc support:0-no, 1-yes;
DISP_CH=音乐是否支持歌词显示:0-否，1-是


key=MUSIC_ALBUM_ART_SUPPORT;
type=DATA;
operate=SINGLE;
len=;
RANGE=0,1;
tabname=music;
DISP_EN=music album art support:0-no, 1-yes;
DISP_CH=音乐是否支持专辑图片显示:0-否，1-是

key=RADIO_AP_ID_BAND;
type=DATA;
operate=SINGLE;
len=;
RANGE=0,1,2;
tabname=radio;
DISP_EN=default band:0-normal，1-japan，2-europe
DISP_CH=电台默认波段:0-普通波段，1-日本波段，2-欧洲波段

key=RADIO_AP_ID_SEEKMODE;
type=DATA;
operate=SINGLE;
len=;
RANGE=0,1;
tabname=radio;
DISP_EN=seek mode:0-hardware mode, 1-software mode
DISP_CH=默认搜台模式:0-硬件seek模式，1-软件seek模式

key=FMENGINE_AP_ID_SEEKTH;
type=DATA;
operate=SINGLE;
len=;
RANGE=0-127;
tabname=radio;
DISP_EN=threshold for auto search
DISP_CH=搜台门限

key=RECORD_GAIN_INPUT_LINEIN;
type=DATA;
operate=SINGLE;
len=;
RANGE=0-7;
tabname=record;
DISP_EN=line-n gain:0:-12db,1:-3db,2:0db,3:1.5db,4:3db,5:4.5db,6:6db,7:7.5db
DISP_CH=录音line-in增益:0:-12db,1:-3db,2:0db,3:1.5db,4:3db,5:4.5db,6:6db,7:7.5db

key=RECORD_GAIN_ADC_LINEIN;
type=DATA;
operate=SINGLE;
len=;
RANGE=0-63;
tabname=record;
DISP_EN=line-in gain:0:-70.5db, step:1.5db
DISP_CH=ADC line-in增益:0:-70.5db, 每级递增1.5db

key=RECORD_GAIN_INPUT_MIC;
type=DATA;
operate=SINGLE;
len=;
RANGE=0-7;
tabname=record;
DISP_EN=mic-in gain:0:26db,1:30db,2:31.5db,3:33db,4:34.5db,5:36db,6:37.5db,7:39db
DISP_CH=录音mic-in增益:0:26db,1:30db,2:31.5db,3:33db,4:34.5db,5:36db,6:37.5db,7:39db

key=RECORD_GAIN_ADC_MIC;
type=DATA;
operate=SINGLE;
len=;
RANGE=0-63;
tabname=record;
DISP_EN=ADC mic-in gain:0:-70.5db, step:1.5db
DISP_CH=ADC mic-in增益:0:-70.5db, 每级递增1.5db

key=RECORD_TRACK_LEVEL_BREAKSONG;
type=DATA;
operate=SINGLE;
len=;
RANGE=0-128;
tabname=record;
DISP_EN=threshold for auto breaksong:0-avarage noise energy, none zero -0.5db/step
DISP_CH=自动分曲电压门槛值:0表示采用初始录音的噪声平均值，非零则-0.5dB一级

key=RECORD_TRACK_LEVEL_AVR;
type=DATA;
operate=SINGLE;
len=;
RANGE=0-128;
tabname=record;
DISP_EN=threshold for avr:0-avarage noise energy, none zero -0.5db/step
DISP_CH=avr电压门槛值:0表示采用初始录音的噪声平均值，非零则-0.5dB一级

key=RECORD_TRACK_DELAY;
type=DATA;
operate=SINGLE;
len=;
RANGE=1-10;
tabname=record;
DISP_EN=avr delay time:uint: s
DISP_CH=avr延时时间:单位：s

key=RECORD_INPUT_SOURCE;
type=DATA;
operate=SINGLE;
len=;
RANGE=0-2;
tabname=record;
DISP_EN=input source:0:mic-in;1:fm-in;2:line-in
DISP_CH=录音来源:0:mic-in;1:fm-in;2:line-in

key=RECORD_FORMAT_TYPE;
type=DATA;
operate=SINGLE;
len=;
RANGE=0,1;
tabname=record;
DISP_EN=record format:0-wav; 1-mp3
DISP_CH=录音格式:0-wav; 1-mp3

key=RECORD_WAV_BITRATE;
type=DATA;
operate=SINGLE;
len=;
RANGE=5-8;
tabname=record;
DISP_EN=wav bit rate:4:256kbps; 5:512kbps; 6:768kbps; 7:1024kbps; 8:1536kbps
DISP_CH=wav比特率:4:256kbps; 5:512kbps; 6:768kbps; 7:1024kbps; 8:1536kbps

key=RECORD_MP3_BITRATE;
type=DATA;
operate=SINGLE;
len=;
RANGE=0-3;
tabname=record;
DISP_EN=mp3 bit rate:0:32kbps; 1:64kbps; 2:128kbps; 3:192kbps;
DISP_CH=mp3比特率:0:32kbps; 1:64kbps; 2:128kbps; 3:192kbps;

key=RECORD_TRACK_MODE;
type=DATA;
operate=SINGLE;
len=;
RANGE=0,1;
tabname=record;
DISP_EN=auto breaksong:0-disable; 1-enable
DISP_CH=自动分曲模式:0-关闭;1-开启

key=RECORD_AVR_MODE;
type=DATA;
operate=SINGLE;
len=;
RANGE=0,1;
tabname=record;
DISP_EN=avr mode:0-disable; 1-enable
DISP_CH=avr模式:0-关闭;1-开启

key=USB_VID;
type=STR;
operate=EDIT;
len=6;
RANGE=;
tabname=usb;
DISP_EN=USB VID
DISP_CH=USB VID

key=USB_PID;
type=STR;
operate=EDIT;
len=6;
RANGE=;
tabname=usb;
DISP_EN=USB PID
DISP_CH=USB PID

key=INF_USB_VENDOR;
type=STR;
operate=EDIT;
len=8;
RANGE=;
tabname=usb;
DISP_EN=usb vendor
DISP_CH=usb 设备厂商

key=INF_USB_PRODUCTID;
type=STR;
operate=EDIT;
len=16;
RANGE=;
tabname=usb;
DISP_EN=usb product id
DISP_CH=usb 设备

key=INF_DEVICE_PRODUCER;
type=STR;
operate=EDIT;
len=32;
RANGE=;
tabname=usb;
DISP_EN=usb producer
DISP_CH=usb 制造商

key=INF_INQUIRY_INFORMATION_FOB;
type=STR;
operate=EDIT;
len=58;
RANGE=;
tabname=usb;
DISP_EN=infomation of Udisk
DISP_CH=Udisk 信息

key=INF_INQUIRY_INFORMATION_CDROM;
type=STR;
operate=EDIT;
len=58;
RANGE=;
tabname=usb;
DISP_EN=infomation of CDROM disk
DISP_CH=usb CDROM 盘信息

key=INF_CARD_DISPLAY;
type=STR;
operate=EDIT;
len=5;
RANGE=;
tabname=usb;
DISP_EN=display card enable
DISP_CH=是否显示卡盘

key=INF_UDISK_LABEL;
type=STR;
operate=EDIT;
len=11;
RANGE=;
tabname=usb;
DISP_EN=show udisk label
DISP_CH=磁盘卷标名
