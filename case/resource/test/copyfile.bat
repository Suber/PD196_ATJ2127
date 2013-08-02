@echo off
set mydir=%cd%
xcopy  %mydir%\*.sty  %mydir%\..\..\fwpkg\sty\ /d /s /y
xcopy  %mydir%\*.sty  %mydir%\..\..\fwpkg\ap\ /d /s /y
xcopy  %mydir%\*.h  %mydir%\..\..\ap\ap_test\ /d /s /y
pause
