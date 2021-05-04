# Microsoft Developer Studio Project File - Name="SeaBattle" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=SeaBattle - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "SeaBattle.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "SeaBattle.mak" CFG="SeaBattle - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "SeaBattle - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "SeaBattle - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "SeaBattle - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x419 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x419 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "SeaBattle - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x419 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x419 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "SeaBattle - Win32 Release"
# Name "SeaBattle - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\ButWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\Field.cpp
# End Source File
# Begin Source File

SOURCE=.\MovesBox.cpp
# End Source File
# Begin Source File

SOURCE=.\MyButton.cpp
# End Source File
# Begin Source File

SOURCE=.\NetworkDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Picture.cpp
# End Source File
# Begin Source File

SOURCE=.\SaveDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\SeaBattle.cpp
# End Source File
# Begin Source File

SOURCE=.\SeaBattle.rc
# End Source File
# Begin Source File

SOURCE=.\SeaBattleDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\ButWnd.h
# End Source File
# Begin Source File

SOURCE=.\ClientServer.h
# End Source File
# Begin Source File

SOURCE=.\Field.h
# End Source File
# Begin Source File

SOURCE=.\MovesBox.h
# End Source File
# Begin Source File

SOURCE=.\MyButton.h
# End Source File
# Begin Source File

SOURCE=.\NetworkDlg.h
# End Source File
# Begin Source File

SOURCE=.\Picture.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\SaveDialog.h
# End Source File
# Begin Source File

SOURCE=.\SeaBattle.h
# End Source File
# Begin Source File

SOURCE=.\SeaBattleDlg.h
# End Source File
# Begin Source File

SOURCE=.\Ship.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\BackGround.jpg
# End Source File
# Begin Source File

SOURCE=.\res\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00001.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00002.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00003.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00004.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00005.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00006.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00007.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00008.bmp
# End Source File
# Begin Source File

SOURCE=.\res\BT_ACT_Help.jpg
# End Source File
# Begin Source File

SOURCE=.\res\BT_ACT_Network.bmp
# End Source File
# Begin Source File

SOURCE=.\res\BT_ACT_Network.jpg
# End Source File
# Begin Source File

SOURCE=.\res\BT_ACT_NEW_GAME.jpg
# End Source File
# Begin Source File

SOURCE=.\res\BT_ACT_Ships.bmp
# End Source File
# Begin Source File

SOURCE=.\res\BT_ACT_Ships.jpg
# End Source File
# Begin Source File

SOURCE=.\res\BT_ACT_SINGLE.jpg
# End Source File
# Begin Source File

SOURCE=.\res\BT_Help.jpg
# End Source File
# Begin Source File

SOURCE=.\res\BT_Network.bmp
# End Source File
# Begin Source File

SOURCE=.\res\BT_Network.jpg
# End Source File
# Begin Source File

SOURCE=.\res\BT_NEW_GAME.JPG
# End Source File
# Begin Source File

SOURCE=.\res\BT_Ships.bmp
# End Source File
# Begin Source File

SOURCE=.\res\BT_Ships.jpg
# End Source File
# Begin Source File

SOURCE=.\res\BT_SINGLE.jpg
# End Source File
# Begin Source File

SOURCE=.\res\CLOSE.bmp
# End Source File
# Begin Source File

SOURCE=.\res\CLOSE.jpg
# End Source File
# Begin Source File

SOURCE=.\res\cur00001.cur
# End Source File
# Begin Source File

SOURCE=.\res\cur00002.cur
# End Source File
# Begin Source File

SOURCE=.\res\cur00003.cur
# End Source File
# Begin Source File

SOURCE=.\res\cursor1.cur
# End Source File
# Begin Source File

SOURCE=.\res\Fire.bmp
# End Source File
# Begin Source File

SOURCE=.\res\H_move.cur
# End Source File
# Begin Source File

SOURCE=.\res\H_nw.cur
# End Source File
# Begin Source File

SOURCE=.\res\H_point.cur
# End Source File
# Begin Source File

SOURCE=.\res\HIDE.bmp
# End Source File
# Begin Source File

SOURCE=.\res\HIDE.jpg
# End Source File
# Begin Source File

SOURCE=.\res\icon1.ico
# End Source File
# Begin Source File

SOURCE=.\res\Killed.jpg
# End Source File
# Begin Source File

SOURCE=.\res\Logo.bmp
# End Source File
# Begin Source File

SOURCE=.\res\MonochromeFire.bmp
# End Source File
# Begin Source File

SOURCE=.\res\one_deck.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ont_deck.bmp
# End Source File
# Begin Source File

SOURCE=.\res\OPEN.jpg
# End Source File
# Begin Source File

SOURCE=.\res\SeaBattle.ico
# End Source File
# Begin Source File

SOURCE=.\res\SeaBattle.rc2
# End Source File
# Begin Source File

SOURCE=.\res\SHIP_1_DECK.bmp
# End Source File
# Begin Source File

SOURCE=.\res\SHIP_2_DECKS.bmp
# End Source File
# Begin Source File

SOURCE=.\res\SHIP_2_DECKS_R.bmp
# End Source File
# Begin Source File

SOURCE=.\res\SHIP_3_DECKS.bmp
# End Source File
# Begin Source File

SOURCE=.\res\SHIP_3_DECKS_R.bmp
# End Source File
# Begin Source File

SOURCE=.\res\SHIP_4_DECKS.bmp
# End Source File
# Begin Source File

SOURCE=.\res\SHIP_4_DECKS_R.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Title.jpg
# End Source File
# Begin Source File

SOURCE=.\res\TURNOFF.jpg
# End Source File
# Begin Source File

SOURCE=.\res\TURNON.bmp
# End Source File
# Begin Source File

SOURCE=.\res\TURNOUT.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
