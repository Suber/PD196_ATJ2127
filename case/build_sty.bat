@echo off

set CASEPATH=%~dp0

%CASEPATH%\tools\UI-EDITOR\resBuildApp -plugindir  "%CASEPATH%\tools\UI-EDITOR"  -res -sty -styhead -reshead -lanHead "%CASEPATH%\resource\common\UIFile\common.com_ui"
xcopy  %CASEPATH%\resource\common\UIFile\*.sty  %CASEPATH%\fwpkg\sty\ /d /s /y
xcopy  %CASEPATH%\resource\common\UIFile\*.h  %CASEPATH%\inc\ /d /s /y

%CASEPATH%\tools\UI-EDITOR\resBuildApp -plugindir  "%CASEPATH%\tools\UI-EDITOR"  -res -sty -styhead -reshead "%CASEPATH%\resource\music\music.ui"
xcopy  %CASEPATH%\resource\music\*.sty  %CASEPATH%\fwpkg\sty\ /d /s /y
xcopy  %CASEPATH%\resource\music\*.h  %CASEPATH%\ap\ap_music\ /d /s /y

%CASEPATH%\tools\UI-EDITOR\resBuildApp -plugindir  "%CASEPATH%\tools\UI-EDITOR"  -res -sty -styhead -reshead "%CASEPATH%\resource\alarm\alarm.ui"
xcopy  %CASEPATH%\resource\alarm\*.sty  %CASEPATH%\fwpkg\sty\ /d /s /y
xcopy  %CASEPATH%\resource\alarm\*.h  %CASEPATH%\ap\ap_alarm\ /d /s /y

%CASEPATH%\tools\UI-EDITOR\resBuildApp -plugindir  "%CASEPATH%\tools\UI-EDITOR"  -res -sty -styhead -reshead "%CASEPATH%\resource\browser\browser.ui"
xcopy  %CASEPATH%\resource\browser\*.sty  %CASEPATH%\fwpkg\sty\ /d /s /y
xcopy  %CASEPATH%\resource\browser\*.h  %CASEPATH%\ap\ap_browser\ /d /s /y

%CASEPATH%\tools\UI-EDITOR\resBuildApp -plugindir  "%CASEPATH%\tools\UI-EDITOR"  -res -sty -styhead -reshead "%CASEPATH%\resource\config\config.ui"
xcopy  %CASEPATH%\resource\config\*.sty  %CASEPATH%\fwpkg\sty\ /d /s /y
xcopy  %CASEPATH%\resource\config\*.h  %CASEPATH%\ap\ap_config\ /d /s /y

%CASEPATH%\tools\UI-EDITOR\resBuildApp -plugindir  "%CASEPATH%\tools\UI-EDITOR"  -res -sty -styhead -reshead "%CASEPATH%\resource\ebook\ebook.ui"
xcopy  %CASEPATH%\resource\ebook\*.sty  %CASEPATH%\fwpkg\sty\ /d /s /y
xcopy  %CASEPATH%\resource\ebook\*.h  %CASEPATH%\ap\ap_ebook\ /d /s /y

%CASEPATH%\tools\UI-EDITOR\resBuildApp -plugindir  "%CASEPATH%\tools\UI-EDITOR"  -res -sty -styhead -reshead "%CASEPATH%\resource\MainMenu\MainMenu.ui"
xcopy  %CASEPATH%\resource\MainMenu\*.sty  %CASEPATH%\fwpkg\sty\ /d /s /y
xcopy  %CASEPATH%\resource\MainMenu\*.h  %CASEPATH%\ap\ap_MainMenu\ /d /s /y

%CASEPATH%\tools\UI-EDITOR\resBuildApp -plugindir  "%CASEPATH%\tools\UI-EDITOR"  -res -sty -styhead -reshead "%CASEPATH%\resource\picture\picture.ui"
xcopy  %CASEPATH%\resource\picture\*.sty  %CASEPATH%\fwpkg\sty\ /d /s /y
xcopy  %CASEPATH%\resource\picture\*.h  %CASEPATH%\ap\ap_picture\ /d /s /y

%CASEPATH%\tools\UI-EDITOR\resBuildApp -plugindir  "%CASEPATH%\tools\UI-EDITOR"  -res -sty -styhead -reshead "%CASEPATH%\resource\playlist\playlist.ui"
xcopy  %CASEPATH%\resource\playlist\*.sty  %CASEPATH%\fwpkg\sty\ /d /s /y
xcopy  %CASEPATH%\resource\playlist\*.h  %CASEPATH%\ap\ap_playlist\ /d /s /y

%CASEPATH%\tools\UI-EDITOR\resBuildApp -plugindir  "%CASEPATH%\tools\UI-EDITOR"  -res -sty -styhead -reshead "%CASEPATH%\resource\radio\radio.ui"
xcopy  %CASEPATH%\resource\radio\*.sty  %CASEPATH%\fwpkg\sty\ /d /s /y
xcopy  %CASEPATH%\resource\radio\*.h  %CASEPATH%\ap\ap_radio\ /d /s /y

%CASEPATH%\tools\UI-EDITOR\resBuildApp -plugindir  "%CASEPATH%\tools\UI-EDITOR"  -res -sty -styhead -reshead "%CASEPATH%\resource\record\record.ui"
xcopy  %CASEPATH%\resource\record\*.sty  %CASEPATH%\fwpkg\sty\ /d /s /y
xcopy  %CASEPATH%\resource\record\*.h  %CASEPATH%\ap\ap_record\ /d /s /y

%CASEPATH%\tools\UI-EDITOR\resBuildApp -plugindir  "%CASEPATH%\tools\UI-EDITOR"  -res -sty -styhead -reshead "%CASEPATH%\resource\setting\setting.ui"
xcopy  %CASEPATH%\resource\setting\*.sty  %CASEPATH%\fwpkg\sty\ /d /s /y
xcopy  %CASEPATH%\resource\setting\*.h  %CASEPATH%\ap\ap_setting\ /d /s /y

%CASEPATH%\tools\UI-EDITOR\resBuildApp -plugindir  "%CASEPATH%\tools\UI-EDITOR"  -res -sty -styhead -reshead "%CASEPATH%\resource\tools\tools.ui"
xcopy  %CASEPATH%\resource\tools\*.sty  %CASEPATH%\fwpkg\sty\ /d /s /y
xcopy  %CASEPATH%\resource\tools\*.h  %CASEPATH%\ap\ap_tools\ /d /s /y

%CASEPATH%\tools\UI-EDITOR\resBuildApp -plugindir  "%CASEPATH%\tools\UI-EDITOR"  -res -sty -styhead -reshead "%CASEPATH%\resource\udisk\udisk.ui"
xcopy  %CASEPATH%\resource\udisk\*.sty  %CASEPATH%\fwpkg\sty\ /d /s /y
xcopy  %CASEPATH%\resource\udisk\*.h  %CASEPATH%\ap\ap_udisk\ /d /s /y

%CASEPATH%\tools\UI-EDITOR\resBuildApp -plugindir  "%CASEPATH%\tools\UI-EDITOR"  -res -sty -styhead -reshead "%CASEPATH%\resource\upgrade\upgrade.ui"
xcopy  %CASEPATH%\resource\upgrade\*.sty  %CASEPATH%\fwpkg\sty\ /d /s /y
xcopy  %CASEPATH%\resource\upgrade\*.h  %CASEPATH%\ap\ap_upgrade\ /d /s /y

%CASEPATH%\tools\UI-EDITOR\resBuildApp -plugindir  "%CASEPATH%\tools\UI-EDITOR"  -res -sty -styhead -reshead "%CASEPATH%\resource\video\video.ui"
xcopy  %CASEPATH%\resource\video\*.sty  %CASEPATH%\fwpkg\sty\ /d /s /y
xcopy  %CASEPATH%\resource\video\*.h  %CASEPATH%\ap\ap_video\ /d /s /y


pause