# BasicWinAPI
This is my basic winAPI and compile by minGW on command line

:: MinGW ::

How to compile. You need install MinGW : http://www.mingw.org/
VDO how to install : https://www.youtube.com/watch?v=xSOji8LMcMk

:: Compile Source ::

Sample 0 
g++ 0_helloWin32.cpp -Wl,-subsystem,windows -o bin/run0

Sample 1 
g++ 1_createWindow.cpp -Wl,-subsystem,windows -o bin/run1

Sample 2 
g++ 2_handingMessage.cpp -Wl,-subsystem,windows -o bin/run2

Sample 3 
windres rc/rc1.rc obj/rc1.o
g++ -c 3_MenuAndIcon.cpp -o obj/3_MenuAndIcon.o
g++ obj/3_MenuAndIcon.o obj/rc1.o -Wl,-subsystem,windows -o bin/run3

Sample 4 
g++ 4_MenuAndIcon2.cpp -Wl,-subsystem,windows -o bin/run4

Sample 5 
windres rc/rc2.rc obj/rc2.o
g++ -c 5_Dialogs1.cpp -o obj/5_Dialogs1.o
g++ obj/5_Dialogs1.o obj/rc2.o -Wl,-subsystem,windows -o bin/run5

Sample 6 
windres rc/rc3.rc obj/rc3.o
g++ -c 6_Dialogs2.cpp -o obj/6_Dialogs2.o
g++ obj/6_Dialogs2.o obj/rc3.o -Wl,-subsystem,windows -o bin/run6

Sample 7 
windres rc/rc4.rc obj/rc4.o
g++ -c 7_Dialogs3.cpp -o obj/7_Dialogs3.o
g++ -static -mwindows obj/7_Dialogs3.o obj/rc4.o -Wl,-subsystem,windows -o bin/run7

Sample 8 
windres rc/rc5.rc obj/rc5.o
g++ -c 8_StandardControl.cpp -o obj/8_StandardControl.o
g++ obj/8_StandardControl.o obj/rc5.o -Wl,-subsystem,windows -o bin/run8

Sample 9 
g++ -static -mwindows 9_CreateControlsAtRuntime.cpp -Wl,-subsystem,windows -o bin/run9

Sample 10 
windres rc/rc6.rc obj/rc6.o
g++ -c 10_UsingFileAndCommonDialogs.cpp -o obj/10_UsingFileAndCommonDialogs.o
g++ -static -mwindows obj/10_UsingFileAndCommonDialogs.o obj/rc6.o -Wl,-subsystem,windows -o bin/run10

Sample 11 
windres rc/rc6.rc obj/rc6.o
g++ -c 11_ToolAndStatusBars.cpp -o obj/11_ToolAndStatusBars.o
g++ -static -mwindows obj/11_ToolAndStatusBars.o obj/rc6.o -Wl,-subsystem,windows -o bin/run11

Sample 12 
windres rc/rc7.rc obj/rc7.o
g++ -c 12_MultipleDocumentInterface.cpp -o obj/12_MultipleDocumentInterface.o
g++ -static -mwindows obj/12_MultipleDocumentInterface.o obj/rc7.o -Wl,-subsystem,windows -o bin/run12

Sample 13 
windres rc/rc8.rc obj/rc8.o
g++ -c 13_BitmapDeviceContextAndBitBlt.cpp -o obj/13_BitmapDeviceContextAndBitBlt.o
g++ -static -mwindows obj/13_BitmapDeviceContextAndBitBlt.o obj/rc8.o -Wl,-subsystem,windows -o bin/run13

Sample 14 
windres rc/rc8.rc obj/rc8.o
g++ -c 14_TransparentBitmaps.cpp -o obj/14_TransparentBitmaps.o
g++ -static -mwindows obj/14_TransparentBitmaps.o obj/rc8.o -Wl,-subsystem,windows -o bin/run14

Sample 15 
windres rc/rc8.rc obj/rc8.o
g++ -c 15_TimersAndAnimation.cpp -o obj/15_TimersAndAnimation.o
g++ -static -mwindows obj/15_TimersAndAnimation.o obj/rc8.o -Wl,-subsystem,windows -o bin/run15

Sample 16 
windres rc/rc9.rc obj/rc9.o
g++ -c 16_TextandFonts.cpp -o obj/16_TextandFonts.o
g++ -static -mwindows obj/16_TextandFonts.o obj/rc9.o -Wl,-subsystem,windows -o bin/run16

Thank you
