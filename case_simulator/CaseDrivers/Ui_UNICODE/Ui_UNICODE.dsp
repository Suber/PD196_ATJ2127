# Microsoft Developer Studio Project File - Name="Ui_UNICODE" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=Ui_UNICODE - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Ui_UNICODE.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Ui_UNICODE.mak" CFG="Ui_UNICODE - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Ui_UNICODE - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Ui_UNICODE - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Ui_UNICODE - Win32 Release"

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
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "UI_UNICODE_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "UI_UNICODE_EXPORTS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386

!ELSEIF  "$(CFG)" == "Ui_UNICODE - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "UI_UNICODE_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /Zp1 /MDd /W3 /Gm /GX /ZI /I "..\..\..\psp_rel\include\ucos" /I "..\..\..\psp_rel\include" /I "..\..\..\case\inc" /I "..\..\..\case\drv\ui\ui_drv_rom" /I "..\..\..\case\drv\ui" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "UI_UNICODE_EXPORTS" /D non_banked= /D banked= /D "__IAR_SYSTEMS_ICC" /D "PC" /FR /FD /GZ ..\..\..\usdk212a\psp_rel\include\ucos,..\..\..\usdk212a\psp_rel\include,..\..\..\usdk212a\case\inc /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 ..\..\bin\debug\simbase.lib ..\..\bin\debug\simkernel.lib /nologo /dll /debug /machine:I386 /def:".\Ui_UNICODE.def" /out:"..\..\bin\debug\drv_ui.drv" /implib:"../../bin/debug/ui_ucode.lib" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "Ui_UNICODE - Win32 Release"
# Name "Ui_UNICODE - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\Sim_api.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\Ui_UNICODE.def

!IF  "$(CFG)" == "Ui_UNICODE - Win32 Release"

!ELSEIF  "$(CFG)" == "Ui_UNICODE - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# End Group
# Begin Group "Sdrv_code"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\case\drv\ui\bank_a_fix_ui_string.c
# End Source File
# Begin Source File

SOURCE=..\..\..\case\drv\ui\bank_a_fix_ui_string_2.c
# End Source File
# Begin Source File

SOURCE=..\..\..\case\drv\ui\bank_a_ui_draw_3Drect.c
# End Source File
# Begin Source File

SOURCE=..\..\..\case\drv\ui\bank_a_ui_draw_rect_frame.c
# End Source File
# Begin Source File

SOURCE=..\..\..\case\drv\ui\bank_a_ui_fill_rect_dump.c
# End Source File
# Begin Source File

SOURCE=..\..\..\case\drv\ui\bank_a_ui_get_listbox_attrb.c
# End Source File
# Begin Source File

SOURCE=..\..\..\case\drv\ui\bank_a_ui_get_picbox_attrb.c
# End Source File
# Begin Source File

SOURCE=..\..\..\case\drv\ui\bank_a_ui_get_textbox_attrb.c
# End Source File
# Begin Source File

SOURCE=..\..\..\case\drv\ui\bank_a_ui_itoa.c
# End Source File
# Begin Source File

SOURCE=..\..\..\case\drv\ui\bank_a_ui_load_attribute.c
# End Source File
# Begin Source File

SOURCE=..\..\..\case\drv\ui\bank_a_ui_putstring_sub_2.c
# End Source File
# Begin Source File

SOURCE=..\..\..\case\drv\ui\bank_a_ui_res_open.c
# End Source File
# Begin Source File

SOURCE=..\..\..\case\drv\ui\bank_a_ui_show_listbox.c
# End Source File
# Begin Source File

SOURCE=..\..\..\case\drv\ui\bank_a_ui_show_listbox_2.c
# End Source File
# Begin Source File

SOURCE=..\..\..\case\drv\ui\bank_a_ui_show_numbox_1.c
# End Source File
# Begin Source File

SOURCE=..\..\..\case\drv\ui\bank_a_ui_show_parambox.c
# End Source File
# Begin Source File

SOURCE=..\..\..\case\drv\ui\bank_a_ui_show_parambox_2.c
# End Source File
# Begin Source File

SOURCE=..\..\..\case\drv\ui\bank_a_ui_show_sliderbar.c
# End Source File
# Begin Source File

SOURCE=..\..\..\case\drv\ui\bank_a_ui_show_sliderbar_3.c
# End Source File
# Begin Source File

SOURCE=..\..\..\case\drv\ui\bank_a_ui_show_timebox_3.c
# End Source File
# Begin Source File

SOURCE=..\..\..\case\drv\ui\bank_a_ui_thai_point.c
# End Source File
# Begin Source File

SOURCE=..\..\..\case\drv\ui\bank_b_ui_arabic_ligature.c
# End Source File
# Begin Source File

SOURCE=..\..\..\case\drv\ui\bank_b_ui_arabic_nsm.c
# End Source File
# Begin Source File

SOURCE=..\..\..\case\drv\ui\bank_b_ui_copy_arabic.c
# End Source File
# Begin Source File

SOURCE=..\..\..\case\drv\ui\bank_b_ui_get_text_line.c
# End Source File
# Begin Source File

SOURCE=..\..\..\case\drv\ui\bank_b_ui_putstring.c
# End Source File
# Begin Source File

SOURCE=..\..\..\case\drv\ui\bank_b_ui_scroll_string.c
# End Source File
# Begin Source File

SOURCE=..\..\..\case\drv\ui\bank_b_ui_scroll_string_ext.c
# End Source File
# Begin Source File

SOURCE=..\..\..\case\drv\ui\bank_b_ui_show_dialogbox.c
# End Source File
# Begin Source File

SOURCE=..\..\..\case\drv\ui\bank_b_ui_show_listbox_1.c
# End Source File
# Begin Source File

SOURCE=..\..\..\case\drv\ui\bank_b_ui_show_numbox.c
# End Source File
# Begin Source File

SOURCE=..\..\..\case\drv\ui\bank_b_ui_show_parambox_1.c
# End Source File
# Begin Source File

SOURCE=..\..\..\case\drv\ui\bank_b_ui_show_picbox.c
# End Source File
# Begin Source File

SOURCE=..\..\..\case\drv\ui\bank_b_ui_show_progressbar.c
# End Source File
# Begin Source File

SOURCE=..\..\..\case\drv\ui\bank_b_ui_show_sliderbar_1.c
# End Source File
# Begin Source File

SOURCE=..\..\..\case\drv\ui\bank_b_ui_show_sliderbar_2.c
# End Source File
# Begin Source File

SOURCE=..\..\..\case\drv\ui\bank_b_ui_show_textbox.c
# End Source File
# Begin Source File

SOURCE=..\..\..\case\drv\ui\bank_b_ui_show_timebox.c
# End Source File
# Begin Source File

SOURCE=..\..\..\case\drv\ui\bank_b_ui_show_timebox_1.c
# End Source File
# Begin Source File

SOURCE=..\..\..\case\drv\ui\bank_b_ui_show_timebox_2.c
# End Source File
# Begin Source File

SOURCE=..\..\..\case\drv\ui\bank_b_ui_string_sdimage.c
# End Source File
# Begin Source File

SOURCE=..\..\..\case\drv\ui\bank_c_misc_functions.c
# End Source File
# Begin Source File

SOURCE=..\..\..\case\drv\ui\bank_c_ui_char_to_unicode.c
# End Source File
# Begin Source File

SOURCE=..\..\..\case\drv\ui\bank_c_ui_ellipsis_to_longstr.c
# End Source File
# Begin Source File

SOURCE=..\..\..\case\drv\ui\bank_c_ui_init.c
# End Source File
# Begin Source File

SOURCE=..\..\..\case\drv\ui\bank_c_ui_putstring_sub.c
# End Source File
# Begin Source File

SOURCE=..\..\..\case\drv\ui\bank_c_ui_set_language.c
# End Source File
# Begin Source File

SOURCE=..\..\..\case\drv\ui\bank_c_ui_show_transparent.c
# End Source File
# Begin Source File

SOURCE=..\..\..\case\drv\ui\bank_c_ui_unicode_to_char.c
# End Source File
# Begin Source File

SOURCE=..\..\..\case\drv\ui\bank_c_ui_utf8_to_unicode.c
# End Source File
# Begin Source File

SOURCE=..\..\..\case\drv\ui\rcode_ui_functions.c
# End Source File
# Begin Source File

SOURCE=..\..\..\case\drv\ui\rcode_ui_op_entry.c
# End Source File
# End Group
# Begin Group "Sdrv_rom"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\case\drv\ui\ui_drv_rom\rom_data_and_common.c
# End Source File
# Begin Source File

SOURCE=..\..\..\case\drv\ui\ui_drv_rom\rom_ui_image.c
# End Source File
# Begin Source File

SOURCE=..\..\..\case\drv\ui\ui_drv_rom\rom_ui_string.c
# End Source File
# End Group
# End Target
# End Project
