; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CTankApp
LastTemplate=CStatic
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "tank.h"

ClassCount=4
Class1=CTankApp
Class2=CTankDlg
Class3=CAboutDlg

ResourceCount=4
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Class4=CBackground
Resource3=IDD_TANK_DIALOG
Resource4=IDR_TANK

[CLS:CTankApp]
Type=0
HeaderFile=tank.h
ImplementationFile=tank.cpp
Filter=N
BaseClass=CWinApp
VirtualFilter=AC
LastObject=CTankApp

[CLS:CTankDlg]
Type=0
HeaderFile=tankDlg.h
ImplementationFile=tankDlg.cpp
Filter=D
LastObject=CTankDlg
BaseClass=CDialog
VirtualFilter=dWC

[CLS:CAboutDlg]
Type=0
HeaderFile=tankDlg.h
ImplementationFile=tankDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_TANK_DIALOG]
Type=1
Class=CTankDlg
ControlCount=1
Control1=IDC_STATIC1,static,1350696960

[CLS:CBackground]
Type=0
HeaderFile=Background.h
ImplementationFile=Background.cpp
BaseClass=CStatic
Filter=W
LastObject=CBackground
VirtualFilter=WC

[MNU:IDR_TANK]
Type=1
Class=CBackground
Command1=ID_SHOW_FIRERANGE
Command2=ID_SHOW_ATTACK_DES
Command3=ID_SHOW_ROUTINE
Command4=ID_SHOW_DEAD
Command5=ID_SHOW_ID
Command6=ID_PAUSE
Command7=ID_RESTART
Command8=ID_ADD_TANK
Command9=ID_ENEMY_AUTO
Command10=ID_MOVE_ATTACK
Command11=ID_TANK_RANK_SLOW
Command12=ID_TANK_RANK_NORMAL
Command13=ID_TANK_RANK_QUICK
Command14=ID_TANK_RANK_FAST
Command15=ID_TANK_FIRERANGE_00
Command16=ID_TANK_FIRERANGE_10
Command17=ID_TANK_FIRERANGE_20
Command18=ID_TANK_FIRERANGE_30
Command19=ID_TANK_FIRERANGE_40
CommandCount=19

