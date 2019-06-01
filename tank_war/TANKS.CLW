; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CTanksApp
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "tanks.h"
LastPage=0

ClassCount=10
Class1=CCommSettingDlg
Class2=CConnectDlg
Class3=CGameSetup
Class4=CAniButton
Class5=CChatDlg
Class6=CServerMgmtDlg
Class7=CKeyMappingDlg
Class8=CTanksApp
Class9=CAboutDlg
Class10=CTanksDlg

ResourceCount=21
Resource1=IDD_TANKS_DIALOG (English (U.S.))
Resource2=IDD_GAME_SETUP (English (U.S.))
Resource3=IDD_COMM_SETTING (English (U.S.))
Resource4=IDD_SERVER_MGMT (English (U.S.))
Resource5=IDD_KEYMAPPING_DIALOG (English (U.S.))
Resource6=IDD_ABOUTBOX (English (U.S.))
Resource7=IDR_MAIN_DLG_ACCELERATOR (English (U.S.))
Resource8=IDR_MAIN_MENU (English (U.S.))
Resource9=IDD_CHAT_DIALOG
Resource10=IDD_CHAT_DIALOG (Unknown language: 0xD, 0x1)
Resource11=IDD_KEYMAPPING_DIALOG
Resource12=IDD_ABOUTBOX
Resource13=IDD_TANKS_DIALOG
Resource14=IDD_GAME_SETUP
Resource15=IDD_COMM_SETTING
Resource16=IDD_SERVER_MGMT
Resource17=IDD_CHAT_DIALOG (Unknown language: 0xD
Resource18=IDR_MAIN_DLG_ACCELERATOR
Resource19=IDD_CHAT_DIALOG (Unknown language: 0xD, 0x1)
Resource20=IDD_CHAT_DIALOG (Unknown language: 0xD, 0x1)
Resource21=IDR_MAIN_MENU

[CLS:CCommSettingDlg]
Type=0
BaseClass=CDialog
HeaderFile=CommSettingDlg.h
ImplementationFile=CommSettingDlg.cpp

[CLS:CConnectDlg]
Type=0
BaseClass=CDialog
HeaderFile=Connect.h
ImplementationFile=Connect.cpp

[CLS:CGameSetup]
Type=0
BaseClass=CDialog
HeaderFile=GameSetup.h
ImplementationFile=GameSetup.cpp

[CLS:CAniButton]
Type=0
BaseClass=CButton
HeaderFile=inc\AniButton.h
ImplementationFile=src\AniButton.cpp

[CLS:CChatDlg]
Type=0
BaseClass=CDialog
HeaderFile=inc\ChatDlg.h
ImplementationFile=src\ChatDlg.cpp
LastObject=CChatDlg
Filter=D
VirtualFilter=dWC

[CLS:CServerMgmtDlg]
Type=0
BaseClass=CDialog
HeaderFile=inc\ServerMgmtDlg.h
ImplementationFile=src\ServerMgmtDlg.cpp
LastObject=CServerMgmtDlg

[CLS:CKeyMappingDlg]
Type=0
BaseClass=CDialog
HeaderFile=KeyMappingDlg.h
ImplementationFile=KeyMappingDlg.cpp

[CLS:CTanksApp]
Type=0
BaseClass=CWinApp
HeaderFile=Tanks.h
ImplementationFile=Tanks.cpp
Filter=N
VirtualFilter=AC
LastObject=ID_SOUND_MUTE

[CLS:CAboutDlg]
Type=0
BaseClass=CDialog
HeaderFile=TanksDlg.cpp
ImplementationFile=TanksDlg.cpp

[CLS:CTanksDlg]
Type=0
BaseClass=CDialog
HeaderFile=TanksDlg.h
ImplementationFile=TanksDlg.cpp
LastObject=CTanksDlg

[DLG:IDD_COMM_SETTING]
Type=1
Class=CCommSettingDlg
ControlCount=10
Control1=IDOK,button,1342242827
Control2=IDCANCEL,button,1342242827
Control3=IDC_PLAYERNAME_EDIT,edit,1350631552
Control4=IDC_HOST_RADIO,button,1342177289
Control5=IDC_PLAYER_RADIO,button,1342177289
Control6=IDC_SPLISTBOX,listbox,1352728835
Control7=IDC_STATIC,button,1342177287
Control8=IDC_STATIC,static,1342308352
Control9=IDC_STATIC,button,1342177287
Control10=IDC_STATIC,static,1342308352

[DLG:IDD_CONNECTION]
Type=1
Class=CConnectDlg

[DLG:IDD_GAME_SETUP]
Type=1
Class=CGameSetup
ControlCount=10
Control1=IDC_ANIMATE_TANK4,SysAnimate32,1350565889
Control2=IDC_ANIMATE_TANK3,SysAnimate32,1350565889
Control3=IDC_ANIMATE_TANK2,SysAnimate32,1350565889
Control4=IDC_ANIMATE_TANK1,SysAnimate32,1350565889
Control5=IDOK,button,1342242827
Control6=IDCANCEL,button,1342242827
Control7=IDC_STATIC1,button,1342177287
Control8=IDC_COMPLEXITY,msctls_trackbar32,1342242817
Control9=IDC_STATIC3,static,1342308364
Control10=IDC_STATIC2,static,1342308352

[DLG:IDD_CHAT_DIALOG]
Type=1
Class=CChatDlg
ControlCount=4
Control1=IDC_CHAT_EDIT,edit,1350631552
Control2=IDCANCEL,button,1342242816
Control3=IDC_CHAT_LIST,listbox,1352728835
Control4=IDC_STATIC,static,1342308352

[DLG:IDD_SERVER_MGMT]
Type=1
Class=CServerMgmtDlg
ControlCount=3
Control1=IDCANCEL,button,1342242816
Control2=IDC_DICONNECT,button,1342242816
Control3=IDC_PLAYERS_LIST,SysListView32,1350631709

[DLG:IDD_KEYMAPPING_DIALOG]
Type=1
Class=CKeyMappingDlg
ControlCount=21
Control1=IDOK,button,1342242827
Control2=IDCANCEL,button,1342242827
Control3=IDC_BUTTON_DEFAULT_SETTINGS,button,1342242827
Control4=IDC_EDIT_FORWARD,edit,1350633600
Control5=IDC_EDIT_TURN_RIGHT,edit,1350633600
Control6=IDC_EDIT_BACKWARD,edit,1350633600
Control7=IDC_EDIT_TURN_LEFT,edit,1350633600
Control8=IDC_EDIT_FIRE_SHELL,edit,1350633600
Control9=IDC_EDIT_FIRE_BULLET,edit,1350633600
Control10=IDC_EDIT_DROP_MINE,edit,1350633600
Control11=IDC_EDIT_AERIAL_SUPPORT,edit,1350633600
Control12=IDC_STATIC,static,1342308865
Control13=IDC_STATIC,button,1342178055
Control14=IDC_STATIC,static,1342308865
Control15=IDC_STATIC,static,1342308865
Control16=IDC_STATIC,static,1342308865
Control17=IDC_STATIC,static,1342308865
Control18=IDC_STATIC,static,1342308865
Control19=IDC_STATIC,static,1342308865
Control20=IDC_STATIC,static,1342308865
Control21=IDC_STATIC,button,1342178055

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_TANKS_DIALOG]
Type=1
Class=CTanksDlg
ControlCount=4
Control1=IDC_MAP,button,1342181131
Control2=IDC_SHIELD_STATUS,button,1073774603
Control3=IDC_AMMO_STATUS,button,1073774603
Control4=IDC_BONUS_STATUS,button,1073774603

[DLG:IDD_KEYMAPPING_DIALOG (English (U.S.))]
Type=1
Class=?
ControlCount=21
Control1=IDOK,button,1342242827
Control2=IDCANCEL,button,1342242827
Control3=IDC_BUTTON_DEFAULT_SETTINGS,button,1342242827
Control4=IDC_EDIT_FORWARD,edit,1350633600
Control5=IDC_EDIT_TURN_RIGHT,edit,1350633600
Control6=IDC_EDIT_BACKWARD,edit,1350633600
Control7=IDC_EDIT_TURN_LEFT,edit,1350633600
Control8=IDC_EDIT_FIRE_SHELL,edit,1350633600
Control9=IDC_EDIT_FIRE_BULLET,edit,1350633600
Control10=IDC_EDIT_DROP_MINE,edit,1350633600
Control11=IDC_EDIT_AERIAL_SUPPORT,edit,1350633600
Control12=IDC_STATIC,static,1342308865
Control13=IDC_STATIC,button,1342178055
Control14=IDC_STATIC,static,1342308865
Control15=IDC_STATIC,static,1342308865
Control16=IDC_STATIC,static,1342308865
Control17=IDC_STATIC,static,1342308865
Control18=IDC_STATIC,static,1342308865
Control19=IDC_STATIC,static,1342308865
Control20=IDC_STATIC,static,1342308865
Control21=IDC_STATIC,button,1342178055

[DLG:IDD_ABOUTBOX (English (U.S.))]
Type=1
Class=?
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_TANKS_DIALOG (English (U.S.))]
Type=1
Class=?
ControlCount=4
Control1=IDC_MAP,button,1342181131
Control2=IDC_SHIELD_STATUS,button,1073774603
Control3=IDC_AMMO_STATUS,button,1073774603
Control4=IDC_BONUS_STATUS,button,1073774603

[DLG:IDD_GAME_SETUP (English (U.S.))]
Type=1
Class=?
ControlCount=10
Control1=IDC_ANIMATE_TANK4,SysAnimate32,1350565889
Control2=IDC_ANIMATE_TANK3,SysAnimate32,1350565889
Control3=IDC_ANIMATE_TANK2,SysAnimate32,1350565889
Control4=IDC_ANIMATE_TANK1,SysAnimate32,1350565889
Control5=IDOK,button,1342242827
Control6=IDCANCEL,button,1342242827
Control7=IDC_STATIC1,button,1342177287
Control8=IDC_COMPLEXITY,msctls_trackbar32,1342242817
Control9=IDC_STATIC3,static,1342308364
Control10=IDC_STATIC2,static,1342308352

[DLG:IDD_COMM_SETTING (English (U.S.))]
Type=1
Class=?
ControlCount=10
Control1=IDOK,button,1342242827
Control2=IDCANCEL,button,1342242827
Control3=IDC_PLAYERNAME_EDIT,edit,1350631552
Control4=IDC_HOST_RADIO,button,1342177289
Control5=IDC_PLAYER_RADIO,button,1342177289
Control6=IDC_SPLISTBOX,listbox,1352728835
Control7=IDC_STATIC,button,1342177287
Control8=IDC_STATIC,static,1342308352
Control9=IDC_STATIC,button,1342177287
Control10=IDC_STATIC,static,1342308352

[DLG:IDD_SERVER_MGMT (English (U.S.))]
Type=1
Class=?
ControlCount=3
Control1=IDCANCEL,button,1342242816
Control2=IDC_DICONNECT,button,1342242816
Control3=IDC_PLAYERS_LIST,SysListView32,1350631709

[MNU:IDR_MAIN_MENU (English (U.S.))]
Type=1
Class=?
Command1=ID_EXIT
Command2=ID_NEW_GAME
Command3=ID_STOP_GAME
Command4=ID_KBD_MAPPING
Command5=ID_SET_GAME
Command6=ID_SET_COMM
Command7=ID_SERVER_MGMT
Command8=ID_LOG_GAME
Command9=ID_PLAYBACK
Command10=ID_CHAT_OPEN
Command11=ID_HELP_ABOUT
Command12=ID_PALYERS_GUIDE
CommandCount=12

[ACL:IDR_MAIN_DLG_ACCELERATOR (English (U.S.))]
Type=1
Class=?
Command1=ID_KBD_MAPPING
Command2=IDD_GAME_SETUP
Command3=ID_SET_COMM
Command4=ID_NEW_GAME
Command5=ID_LOG_GAME
Command6=ID_PLAYBACK
Command7=ID_EXIT
CommandCount=7

[DLG:IDD_CHAT_DIALOG (Unknown language: 0xD, 0x1)]
Type=1
ControlCount=4
Control1=IDC_CHAT_EDIT,edit,1350631552
Control2=IDCANCEL,button,1342242816
Control3=IDC_CHAT_LIST,listbox,1352728833
Control4=IDC_STATIC,static,1342308352

[MNU:IDR_MAIN_MENU]
Type=1
Command1=ID_EXIT
Command2=ID_NEW_GAME
Command3=ID_STOP_GAME
Command4=ID_KBD_MAPPING
Command5=ID_SET_GAME
Command6=ID_SET_COMM
Command7=ID_SERVER_MGMT
Command8=ID_SOUND_MUTE
Command9=ID_CHAT_OPEN
Command10=ID_HELP_ABOUT
Command11=ID_PALYERS_GUIDE
CommandCount=11

[ACL:IDR_MAIN_DLG_ACCELERATOR]
Type=1
Command1=ID_KBD_MAPPING
Command2=IDD_GAME_SETUP
Command3=ID_SET_COMM
Command4=ID_NEW_GAME
Command5=ID_LOG_GAME
Command6=ID_PLAYBACK
Command7=ID_EXIT
CommandCount=7

