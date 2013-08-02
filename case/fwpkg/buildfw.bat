@echo off
echo ***************for EVB*********************************
..\..\psp_rel\tools\Maker.exe -c .\fwimage.cfg -o .\US212A_EVB.fw
echo.
echo.

::    for demo
echo ***************for DEMO********************************
mkdir .\drv\bak\
copy .\drv\*.* .\drv\bak >nul
copy .\drv\demo_apple\*.* .\drv >nul
..\..\psp_rel\tools\Maker.exe -c .\fwimage.cfg -o .\US212A_DEMO.fw
copy .\drv\bak\*.* .\drv >nul
rmdir /s/q .\drv\bak
echo.
echo.



::    for auto-upgrade
::    echo ***************auto update*****************************
::    ..\..\psp_rel\tools\Maker.exe -c .\fwimage.cfg -upg .\UPGRADE.hex
echo.
echo.

pause



 

