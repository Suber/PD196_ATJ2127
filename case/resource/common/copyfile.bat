@echo off
set mydir=%cd%
xcopy  %mydir%\UIFile\*.sty  %mydir%\..\..\fwpkg\sty\ /d /s /y
xcopy  %mydir%\UIFile\*.h  %mydir%\..\..\inc\ /d /s /y
pause
