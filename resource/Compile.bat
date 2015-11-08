@ECHO OFF

MKDIR obj
MKDIR bin
CD bin
MKDIR resource
CD ..
XCOPY "resource\icon*.ico" "bin\resource" /Y /D

CLS

ECHO :::: Compile WinAPI Source by Samsung BagIdea ::::

ECHO.
ECHO Compile[g++] 0_helloWin32.cpp to run0.exe
g++ 0_helloWin32.cpp -Wl,-subsystem,windows -o bin/run0

ECHO.
ECHO Compile[g++] 1_createWindow.cpp to run1.exe
g++ 1_createWindow.cpp -Wl,-subsystem,windows -o bin/run1

ECHO.
ECHO Compile[g++] 2_handingMessage.cpp to run2.exe
g++ 2_handingMessage.cpp -Wl,-subsystem,windows -o bin/run2

ECHO.
ECHO Compile[windres] rc1.rc to rc1.o
windres rc/rc1.rc obj/rc1.o
ECHO Compile[g++] 3_MenuAndIcon.cpp to 3_MenuAndIcon.o
g++ -c 3_MenuAndIcon.cpp -o obj/3_MenuAndIcon.o
ECHO Compile[g++] 3_MenuAndIcon.o and rc1.o to run3.exe
g++ obj/3_MenuAndIcon.o obj/rc1.o -Wl,-subsystem,windows -o bin/run3

ECHO.
ECHO Compile[g++] 4_MenuAndIcon2.cpp to run4.exe
g++ 4_MenuAndIcon2.cpp -Wl,-subsystem,windows -o bin/run4

ECHO.
ECHO Compile[windres] rc2.rc to rc2.o
windres rc/rc2.rc obj/rc2.o
ECHO Compile[g++] 5_Dialogs1.cpp to 5_Dialogs1.o
g++ -c 5_Dialogs1.cpp -o obj/5_Dialogs1.o
ECHO Compile[g++] 5_Dialogs1.o and rc2.o to run5.exe
g++ obj/5_Dialogs1.o obj/rc2.o -Wl,-subsystem,windows -o bin/run5

ECHO.
ECHO Compile[windres] rc3.rc to rc3.o
windres rc/rc3.rc obj/rc3.o
ECHO Compile[g++] 6_Dialogs2.cpp to 6_Dialogs2.o
g++ -c 6_Dialogs2.cpp -o obj/6_Dialogs2.o
ECHO Compile[g++] 6_Dialogs2.o and rc3.o to run6.exe
g++ obj/6_Dialogs2.o obj/rc3.o -Wl,-subsystem,windows -o bin/run6

ECHO.
ECHO Compile[windres] rc4.rc to rc4.o
windres rc/rc4.rc obj/rc4.o
ECHO Compile[g++] 7_Dialogs3.cpp to 7_Dialogs3.o
g++ -c 7_Dialogs3.cpp -o obj/7_Dialogs3.o
ECHO Compile[g++ -static -mwindows] 7_Dialogs3.o and rc4.o to run7.exe
g++ -static -mwindows obj/7_Dialogs3.o obj/rc4.o -Wl,-subsystem,windows -o bin/run7

ECHO.
ECHO Compile[windres] rc5.rc to rc5.o
windres rc/rc5.rc obj/rc5.o
ECHO Compile[g++] 8_StandardControl.cpp to 8_StandardControl.o
g++ -c 8_StandardControl.cpp -o obj/8_StandardControl.o
ECHO Compile[g++] 8_StandardControl.o and rc5.o to run8.exe
g++ obj/8_StandardControl.o obj/rc5.o -Wl,-subsystem,windows -o bin/run8

ECHO.
ECHO Compile[g++ -static -mwindows] 9_CreateControlsAtRuntime.cpp to run9.exe
g++ -static -mwindows 9_CreateControlsAtRuntime.cpp -Wl,-subsystem,windows -o bin/run9

ECHO.
ECHO Compile[windres] rc6.rc to rc6.o
windres rc/rc6.rc obj/rc6.o
ECHO Compile[g++] 10_UsingFileAndCommonDialogs.cpp to 10_UsingFileAndCommonDialogs.o
g++ -c 10_UsingFileAndCommonDialogs.cpp -o obj/10_UsingFileAndCommonDialogs.o
ECHO Compile[g++ -static -mwindows] 10_UsingFileAndCommonDialogs.o and rc6.o to run10.exe
g++ -static -mwindows obj/10_UsingFileAndCommonDialogs.o obj/rc6.o -Wl,-subsystem,windows -o bin/run10

ECHO.
ECHO Compile[windres] rc6.rc to rc6.o
windres rc/rc6.rc obj/rc6.o
ECHO Compile[g++] 11_ToolAndStatusBars.cpp to 11_ToolAndStatusBars.o
g++ -c 11_ToolAndStatusBars.cpp -o obj/11_ToolAndStatusBars.o
ECHO Compile[g++ -static -mwindows] 11_ToolAndStatusBars.o and rc6.o to run11.exe
g++ -static -mwindows obj/11_ToolAndStatusBars.o obj/rc6.o -Wl,-subsystem,windows -o bin/run11

ECHO.
ECHO Compile[windres] rc7.rc to rc7.o
windres rc/rc7.rc obj/rc7.o
ECHO Compile[g++] 12_MultipleDocumentInterface.cpp to 12_MultipleDocumentInterface.o
g++ -c 12_MultipleDocumentInterface.cpp -o obj/12_MultipleDocumentInterface.o
ECHO Compile[g++ -static -mwindows] 12_MultipleDocumentInterface.o and rc7.o to run12.exe
g++ -static -mwindows obj/12_MultipleDocumentInterface.o obj/rc7.o -Wl,-subsystem,windows -o bin/run12

ECHO.
ECHO Compile[windres] rc8.rc to rc8.o
windres rc/rc8.rc obj/rc8.o
ECHO Compile[g++] 13_BitmapDeviceContextAndBitBlt.cpp to 13_BitmapDeviceContextAndBitBlt.o
g++ -c 13_BitmapDeviceContextAndBitBlt.cpp -o obj/13_BitmapDeviceContextAndBitBlt.o
ECHO Compile[g++ -static -mwindows] 13_BitmapDeviceContextAndBitBlt.o and rc8.o to run13.exe
g++ -static -mwindows obj/13_BitmapDeviceContextAndBitBlt.o obj/rc8.o -Wl,-subsystem,windows -o bin/run13

ECHO.
ECHO Compile[windres] rc8.rc to rc8.o
windres rc/rc8.rc obj/rc8.o
ECHO Compile[g++] 14_TransparentBitmaps.cpp to 14_TransparentBitmaps.o
g++ -c 14_TransparentBitmaps.cpp -o obj/14_TransparentBitmaps.o
ECHO Compile[g++ -static -mwindows] 14_TransparentBitmaps.o and rc8.o to run14.exe
g++ -static -mwindows obj/14_TransparentBitmaps.o obj/rc8.o -Wl,-subsystem,windows -o bin/run14

ECHO.
ECHO Compile[windres] rc8.rc to rc8.o
windres rc/rc8.rc obj/rc8.o
ECHO Compile[g++] 15_TimersAndAnimation.cpp to 15_TimersAndAnimation.o
g++ -c 15_TimersAndAnimation.cpp -o obj/15_TimersAndAnimation.o
ECHO Compile[g++ -static -mwindows] 15_TimersAndAnimationo and rc8.o to run15.exe
g++ -static -mwindows obj/15_TimersAndAnimation.o obj/rc8.o -Wl,-subsystem,windows -o bin/run15

ECHO.
ECHO Compile[windres] rc9.rc to rc9.o
windres rc/rc9.rc obj/rc9.o
ECHO Compile[g++] 16_TextandFonts.cpp to 16_TextandFonts.o
g++ -c 16_TextandFonts.cpp -o obj/16_TextandFonts.o
ECHO Compile[g++ -static -mwindows] 15_TimersAndAnimationo.o and rc9.o to run16.exe
g++ -static -mwindows obj/16_TextandFonts.o obj/rc9.o -Wl,-subsystem,windows -o bin/run16

ECHO.
SET /P end=::::::::::: End Program Compile Source :::::::::::