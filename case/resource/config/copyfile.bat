@echo off
set mydir=%cd%
xcopy  %mydir%\*.sty  %mydir%\..\..\fwpkg\sty\ /d /s /y
xcopy  %mydir%\*.h  %mydir%\..\..\ap\ap_config\ /d /s /y
pause
