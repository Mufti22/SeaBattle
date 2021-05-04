; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CSaveDialog
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "SeaBattle.h"

ClassCount=8
Class1=CSeaBattleApp
Class2=CSeaBattleDlg

ResourceCount=5
Resource1=IDR_MAINFRAME
Class3=CField
Resource2=IDD_SEABATTLE_DIALOG
Resource3=IDD_HELP
Resource4=IDD_NETWORK
Class4=CNetworkDlg
Class5=CButWnd
Class6=CMovesBox
Class7=CMyButton
Class8=CSaveDialog
Resource5=IDD_SAVE_RESULT

[CLS:CSeaBattleApp]
Type=0
HeaderFile=SeaBattle.h
ImplementationFile=SeaBattle.cpp
Filter=N
LastObject=CSeaBattleApp

[CLS:CSeaBattleDlg]
Type=0
HeaderFile=SeaBattleDlg.h
ImplementationFile=SeaBattleDlg.cpp
Filter=D
LastObject=CSeaBattleDlg
BaseClass=CDialog
VirtualFilter=dWC



[DLG:IDD_SEABATTLE_DIALOG]
Type=1
Class=CSeaBattleDlg
ControlCount=0

[CLS:CField]
Type=0
HeaderFile=Field.h
ImplementationFile=Field.cpp
BaseClass=CStatic
Filter=W
LastObject=CField
VirtualFilter=WC

[DLG:IDD_HELP]
Type=1
Class=?
ControlCount=3
Control1=IDOK,button,1342275584
Control2=IDC_STATIC,static,1342312448
Control3=IDC_STATIC,static,1342177294

[DLG:IDD_NETWORK]
Type=1
Class=CNetworkDlg
ControlCount=10
Control1=IDOK,button,1342275585
Control2=IDC_LIST1,listbox,1352728835
Control3=ID_CREATE_SERV,button,1342275585
Control4=ID_CONNECT,button,1342275585
Control5=ID_BY_HAND,button,1342275585
Control6=ID_REFRESH,button,1342275585
Control7=IDC_LIST_OF_SERVERS,static,1342308353
Control8=IDC_IPADDRESS3,SysIPAddress32,1342242816
Control9=IDC_OK_CON,button,1342242816
Control10=IDC_CANSEL,button,1342242816

[CLS:CNetworkDlg]
Type=0
HeaderFile=NetworkDlg.h
ImplementationFile=NetworkDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=IDC_CANSEL
VirtualFilter=dWC

[CLS:CButWnd]
Type=0
HeaderFile=ButWnd.h
ImplementationFile=ButWnd.cpp
BaseClass=CWnd
Filter=W
VirtualFilter=WC
LastObject=CButWnd

[CLS:CMovesBox]
Type=0
HeaderFile=MovesBox.h
ImplementationFile=MovesBox.cpp
BaseClass=CListCtrl
Filter=W
LastObject=CMovesBox
VirtualFilter=bWC

[CLS:CMyButton]
Type=0
HeaderFile=MyButton.h
ImplementationFile=MyButton.cpp
BaseClass=CButton
Filter=W
VirtualFilter=BWC
LastObject=CMyButton

[DLG:IDD_SAVE_RESULT]
Type=1
Class=CSaveDialog
ControlCount=3
Control1=IDOK,button,1342242817
Control2=IDC_STATIC,static,1342308353
Control3=IDC_EDIT1,edit,1350631552

[CLS:CSaveDialog]
Type=0
HeaderFile=SaveDialog.h
ImplementationFile=SaveDialog.cpp
BaseClass=CDialog
Filter=D
LastObject=IDOK
VirtualFilter=dWC

