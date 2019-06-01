; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CSpellMapShell
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "SpellMap.h"

ClassCount=3
Class1=CSpellMapApp
Class2=CSpellMapShell
Class3=CAboutDlg

ResourceCount=3
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Resource3=IDD_SPELLMAP_DIALOG

[CLS:CSpellMapApp]
Type=0
HeaderFile=SpellMap.h
ImplementationFile=SpellMap.cpp
Filter=N

[CLS:CSpellMapShell]
Type=0
HeaderFile=SpellMapShell.h
ImplementationFile=SpellMapShell.cpp
Filter=D
LastObject=CSpellMapShell
BaseClass=CDialog
VirtualFilter=dWC

[CLS:CAboutDlg]
Type=0
HeaderFile=SpellMapShell.h
ImplementationFile=SpellMapShell.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_SPELLMAP_DIALOG]
Type=1
Class=CSpellMapShell
ControlCount=2
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816

