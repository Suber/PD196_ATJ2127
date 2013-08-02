# Microsoft Developer Studio Project File - Name="comlib" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=comlib - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "comlib.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "comlib.mak" CFG="comlib - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "comlib - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "comlib - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "comlib"
# PROP Scc_LocalPath "..\..\.."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "comlib - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "comlib - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /Zp1 /MDd /W3 /Gm /GX /ZI /Od /I "..\..\..\psp_rel\include\ucos" /I "..\..\..\psp_rel\include" /I "..\..\..\case\inc" /I "..\..\..\..\usdk212a\psp_rel\usermodule\enhanced\common" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "PC" /FR /FD /GZ /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\..\bin\Debug\comlib.lib"

!ENDIF 

# Begin Target

# Name "comlib - Win32 Release"
# Name "comlib - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "applib"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\case\ap\common\applib\app_manager_bank.c
# End Source File
# Begin Source File

SOURCE=..\..\..\case\ap\common\applib\app_timer_bank.c
# End Source File
# Begin Source File

SOURCE=..\..\..\case\ap\common\applib\app_timer_rcode.c
# End Source File
# Begin Source File

SOURCE=..\..\..\case\ap\common\applib\message_bank_broadcast.c
# End Source File
# Begin Source File

SOURCE=..\..\..\case\ap\common\applib\message_bank_init.c
# End Source File
# Begin Source File

SOURCE=..\..\..\case\ap\common\applib\message_bank_send.c
# End Source File
# Begin Source File

SOURCE=..\..\..\case\ap\common\applib\message_rcode.c
# End Source File
# Begin Source File

SOURCE=..\..\..\case\ap\common\applib\message_rcode_for_engine.c
# End Source File
# End Group
# Begin Group "common_ui"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\case\ap\common\common_ui\ui_animation.c
# End Source File
# Begin Source File

SOURCE=..\..\..\case\ap\common\common_ui\ui_delete.c
# End Source File
# Begin Source File

SOURCE=..\..\..\case\ap\common\common_ui\ui_delete_sub.c
# End Source File
# Begin Source File

SOURCE=..\..\..\case\ap\common\common_ui\ui_dialog.c
# End Source File
# Begin Source File

SOURCE=..\..\..\case\ap\common\common_ui\ui_directory.c
# End Source File
# Begin Source File

SOURCE=..\..\..\case\ap\common\common_ui\ui_directory2.c
# End Source File
# Begin Source File

SOURCE=..\..\..\case\ap\common\common_ui\ui_directory_sub.c
# End Source File
# Begin Source File

SOURCE=..\..\..\case\ap\common\common_ui\ui_directory_sub2.c
# End Source File
# Begin Source File

SOURCE=..\..\..\case\ap\common\common_ui\ui_headbar.c
# End Source File
# Begin Source File

SOURCE=..\..\..\case\ap\common\common_ui\O2\ui_headbar_handle.c
# End Source File
# Begin Source File

SOURCE=..\..\..\case\ap\common\common_ui\ui_keylock.c
# End Source File
# Begin Source File

SOURCE=..\..\..\case\ap\common\common_ui\ui_menulist.c
# End Source File
# Begin Source File

SOURCE=..\..\..\case\ap\common\common_ui\ui_menulist2.c
# End Source File
# Begin Source File

SOURCE=..\..\..\case\ap\common\common_ui\ui_menulist_simple.c
# End Source File
# Begin Source File

SOURCE=..\..\..\case\ap\common\common_ui\ui_menulist_sub.c
# End Source File
# Begin Source File

SOURCE=..\..\..\case\ap\common\common_ui\ui_menulist_sub2.c
# End Source File
# Begin Source File

SOURCE=..\..\..\case\ap\common\common_ui\ui_parameter.c
# End Source File
# Begin Source File

SOURCE=..\..\..\case\ap\common\common_ui\ui_screensave.c
# End Source File
# Begin Source File

SOURCE=..\..\..\case\ap\common\common_ui\ui_screensave_sub.c
# End Source File
# Begin Source File

SOURCE=..\..\..\case\ap\common\common_ui\ui_shutoff.c
# End Source File
# Begin Source File

SOURCE=..\..\..\case\ap\common\common_ui\ui_textread.c
# End Source File
# Begin Source File

SOURCE=..\..\..\case\ap\common\common_ui\ui_textread_dec.c
# End Source File
# Begin Source File

SOURCE=..\..\..\case\ap\common\common_ui\ui_textread_sub.c
# End Source File
# Begin Source File

SOURCE=..\..\..\case\ap\common\common_ui\ui_usbconnect.c
# End Source File
# Begin Source File

SOURCE=..\..\..\case\ap\common\common_ui\ui_volumebar.c
# End Source File
# Begin Source File

SOURCE=..\..\..\case\ap\common\common_ui\ui_volumebar_sub.c
# End Source File
# End Group
# Begin Group "common_func"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\case\ap\common\common_func\common_config.c
# End Source File
# Begin Source File

SOURCE=..\..\..\case\ap\common\common_func\common_confmenu.c
# End Source File
# Begin Source File

SOURCE=..\..\..\case\ap\common\common_func\common_history.c
# End Source File
# Begin Source File

SOURCE=..\..\..\case\ap\common\common_func\common_key.c
# End Source File
# Begin Source File

SOURCE=..\..\..\case\ap\common\common_func\common_msgmap.c
# End Source File
# Begin Source File

SOURCE=..\..\..\case\ap\common\common_func\common_msgmap_data.c
# End Source File
# Begin Source File

SOURCE=..\..\..\case\ap\common\common_func\common_sound.c
# End Source File
# Begin Source File

SOURCE=..\..\..\case\ap\common\common_func\common_string.c
# End Source File
# Begin Source File

SOURCE=..\..\..\case\ap\common\common_func\common_string2.c
# End Source File
# End Group
# Begin Group "data"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\case\ap\common\data\applib_app_data.c
# End Source File
# Begin Source File

SOURCE=..\..\..\case\ap\common\data\applib_globe_data.c
# End Source File
# Begin Source File

SOURCE=..\..\..\case\ap\common\data\common_func_data.c
# End Source File
# Begin Source File

SOURCE=..\..\..\case\ap\common\data\common_ui_data.c
# End Source File
# End Group
# Begin Group "enhanced"

# PROP Default_Filter ""
# Begin Group "bs"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\psp_rel\usermodule\enhanced\bs\eh_bs_directory.c
# End Source File
# Begin Source File

SOURCE=..\..\..\psp_rel\usermodule\enhanced\bs\eh_bs_directory_func.c
# End Source File
# Begin Source File

SOURCE=..\..\..\psp_rel\usermodule\enhanced\bs\compile_o2\eh_bs_entry.c
# End Source File
# Begin Source File

SOURCE=..\..\..\psp_rel\usermodule\enhanced\bs\eh_bs_favorite.c
# End Source File
# Begin Source File

SOURCE=..\..\..\psp_rel\usermodule\enhanced\bs\eh_bs_favorite_func.c
# End Source File
# Begin Source File

SOURCE=..\..\..\psp_rel\usermodule\enhanced\bs\compile_o2\eh_bs_function.c
# End Source File
# Begin Source File

SOURCE=..\..\..\psp_rel\usermodule\enhanced\bs\eh_bs_function_sub.c
# End Source File
# Begin Source File

SOURCE=..\..\..\psp_rel\usermodule\enhanced\bs\eh_bs_other_deal.c
# End Source File
# Begin Source File

SOURCE=..\..\..\psp_rel\usermodule\enhanced\bs\eh_bs_playlist2.c
# End Source File
# Begin Source File

SOURCE=..\..\..\psp_rel\usermodule\enhanced\bs\eh_bs_playlist_del.c
# End Source File
# Begin Source File

SOURCE=..\..\..\psp_rel\usermodule\enhanced\bs\eh_bs_rdata.c
# End Source File
# End Group
# Begin Group "common"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\psp_rel\usermodule\enhanced\common\eh_module.c
# End Source File
# Begin Source File

SOURCE=..\..\..\psp_rel\usermodule\enhanced\common\eh_module_sub.c
# End Source File
# Begin Source File

SOURCE=..\..\..\psp_rel\usermodule\enhanced\common\eh_rdata.c
# End Source File
# End Group
# Begin Group "fsel"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\psp_rel\usermodule\enhanced\fsel\eh_fsel_directory_func.c
# End Source File
# Begin Source File

SOURCE=..\..\..\psp_rel\usermodule\enhanced\fsel\eh_fsel_directory_next.c
# End Source File
# Begin Source File

SOURCE=..\..\..\psp_rel\usermodule\enhanced\fsel\eh_fsel_directory_prev.c
# End Source File
# Begin Source File

SOURCE=..\..\..\psp_rel\usermodule\enhanced\fsel\compile_o2\eh_fsel_entry.c
# End Source File
# Begin Source File

SOURCE=..\..\..\psp_rel\usermodule\enhanced\fsel\eh_fsel_favorite_func.c
# End Source File
# Begin Source File

SOURCE=..\..\..\psp_rel\usermodule\enhanced\fsel\eh_fsel_favorite_sel.c
# End Source File
# Begin Source File

SOURCE=..\..\..\psp_rel\usermodule\enhanced\fsel\eh_fsel_function.c
# End Source File
# Begin Source File

SOURCE=..\..\..\psp_rel\usermodule\enhanced\fsel\eh_fsel_function_sub.c
# End Source File
# Begin Source File

SOURCE=..\..\..\psp_rel\usermodule\enhanced\fsel\eh_fsel_playlist_del.c
# End Source File
# Begin Source File

SOURCE=..\..\..\psp_rel\usermodule\enhanced\fsel\eh_fsel_playlist_func.c
# End Source File
# Begin Source File

SOURCE=..\..\..\psp_rel\usermodule\enhanced\fsel\eh_fsel_playlist_sel.c
# End Source File
# Begin Source File

SOURCE=..\..\..\psp_rel\usermodule\enhanced\fsel\eh_fsel_rdata.c
# End Source File
# Begin Source File

SOURCE=..\..\..\psp_rel\usermodule\enhanced\fsel\eh_fsel_shuffle.c
# End Source File
# Begin Source File

SOURCE=..\..\..\psp_rel\usermodule\enhanced\fsel\eh_fsel_shuffle2.c
# End Source File
# End Group
# Begin Group "id3"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\psp_rel\usermodule\enhanced\id3\eh_id3_aa.c
# End Source File
# Begin Source File

SOURCE=..\..\..\psp_rel\usermodule\enhanced\id3\eh_id3_aac.c
# End Source File
# Begin Source File

SOURCE=..\..\..\psp_rel\usermodule\enhanced\id3\eh_id3_aax.c
# End Source File
# Begin Source File

SOURCE=..\..\..\psp_rel\usermodule\enhanced\id3\eh_id3_ape.c
# End Source File
# Begin Source File

SOURCE=..\..\..\psp_rel\usermodule\enhanced\id3\eh_id3_entry.c
# End Source File
# Begin Source File

SOURCE=..\..\..\psp_rel\usermodule\enhanced\id3\eh_id3_flac.c
# End Source File
# Begin Source File

SOURCE=..\..\..\psp_rel\usermodule\enhanced\id3\eh_id3_function.c
# End Source File
# Begin Source File

SOURCE=..\..\..\psp_rel\usermodule\enhanced\id3\eh_id3_function_sub.c
# End Source File
# Begin Source File

SOURCE=..\..\..\psp_rel\usermodule\enhanced\id3\eh_id3_mp3_v1.c
# End Source File
# Begin Source File

SOURCE=..\..\..\psp_rel\usermodule\enhanced\id3\eh_id3_mp3_v2.c
# End Source File
# Begin Source File

SOURCE=..\..\..\psp_rel\usermodule\enhanced\id3\eh_id3_ogg.c
# End Source File
# Begin Source File

SOURCE=..\..\..\psp_rel\usermodule\enhanced\id3\eh_id3_rdata.c
# End Source File
# Begin Source File

SOURCE=..\..\..\psp_rel\usermodule\enhanced\id3\eh_id3_utf8_deal.c
# End Source File
# Begin Source File

SOURCE=..\..\..\psp_rel\usermodule\enhanced\id3\eh_id3_wma.c
# End Source File
# End Group
# Begin Group "lrc"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\psp_rel\usermodule\enhanced\lrc\eh_lrc_get_rdata.c
# End Source File
# Begin Source File

SOURCE=..\..\..\psp_rel\usermodule\enhanced\lrc\eh_lrc_load_rdata.c
# End Source File
# Begin Source File

SOURCE=..\..\..\psp_rel\usermodule\enhanced\lrc\eh_lyric_get.c
# End Source File
# Begin Source File

SOURCE=..\..\..\psp_rel\usermodule\enhanced\lrc\eh_lyric_get_sub.c
# End Source File
# Begin Source File

SOURCE=..\..\..\psp_rel\usermodule\enhanced\lrc\eh_lyric_load.c
# End Source File
# Begin Source File

SOURCE=..\..\..\psp_rel\usermodule\enhanced\lrc\eh_lyric_load_sub1.c
# End Source File
# Begin Source File

SOURCE=..\..\..\psp_rel\usermodule\enhanced\lrc\eh_lyric_load_sub2.c
# End Source File
# Begin Source File

SOURCE=..\..\..\psp_rel\usermodule\enhanced\lrc\eh_lyric_load_sub3.c
# End Source File
# End Group
# Begin Group "func"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\psp_rel\usermodule\enhanced\func\eh_change_location.c
# End Source File
# Begin Source File

SOURCE=..\..\..\psp_rel\usermodule\enhanced\func\eh_change_location_sub.c
# End Source File
# Begin Source File

SOURCE=..\..\..\psp_rel\usermodule\enhanced\func\eh_make_listlayer.c
# End Source File
# Begin Source File

SOURCE=..\..\..\psp_rel\usermodule\enhanced\func\eh_make_listlayer_sub.c
# End Source File
# End Group
# Begin Group "fav"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\psp_rel\usermodule\enhanced\fav\eh_change_fav_location.c
# End Source File
# Begin Source File

SOURCE=..\..\..\psp_rel\usermodule\enhanced\fav\eh_fav_operation.c
# End Source File
# Begin Source File

SOURCE=..\..\..\psp_rel\usermodule\enhanced\fav\eh_fav_operation_sub.c
# End Source File
# Begin Source File

SOURCE=..\..\..\psp_rel\usermodule\enhanced\fav\eh_fav_operation_sub2.c
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\..\psp_rel\usermodule\enhanced\bs\compile_o2\eh_bs_playlist.c
# End Source File
# Begin Source File

SOURCE=..\..\..\psp_rel\usermodule\enhanced\bs\compile_o2\eh_bs_playlist_func.c
# End Source File
# End Group
# Begin Group "common_misc"

# PROP Default_Filter "*.c;"
# Begin Source File

SOURCE=..\..\..\case\ap\common\common_misc\alarm_msg_dispatch.c
# End Source File
# Begin Source File

SOURCE=..\..\..\case\ap\common\common_misc\alarm_msg_dispatch2.c
# End Source File
# Begin Source File

SOURCE=..\..\..\case\ap\common\common_misc\antenna_detect.c
# End Source File
# Begin Source File

SOURCE=..\..\..\case\ap\common\common_misc\app_sleep.c
# End Source File
# Begin Source File

SOURCE=..\..\..\case\ap\common\common_misc\common_msg.c
# End Source File
# Begin Source File

SOURCE=..\..\..\case\ap\common\common_misc\message_box.c
# End Source File
# Begin Source File

SOURCE=..\..\..\case\ap\common\common_misc\miscellaneous.c
# End Source File
# Begin Source File

SOURCE=..\..\..\case\ap\common\common_misc\setting_comval.c
# End Source File
# Begin Source File

SOURCE=..\..\..\case\ap\common\common_misc\O2\sys_counter.c
# End Source File
# Begin Source File

SOURCE=..\..\..\case\ap\common\common_misc\sys_timer.c
# End Source File
# End Group
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# End Group
# End Target
# End Project
