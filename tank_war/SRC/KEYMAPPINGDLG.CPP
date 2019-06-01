/*****************************************************************************
*                                                                             
*   KeyMappingDlg.cpp
*                                                                             
*   Electrical Engineering Faculty - Software Lab                             
*   Spring semester 1998                                                      
*                                                                             
*   Tanks game                                                                
*                                                                             
*   Module description: Implements the key setting dialog that enables the user
*                       to specify the tanks control keys.
*                       
*                                                                             
*   Authors: Eran Yariv - 28484475                                           
*            Moshe Zur  - 24070856                                           
*                                                                            
*                                                                            
*   Date: 23/09/98                                                           
*                                                                            
******************************************************************************/
#include "stdafx.h"
#include "tanks.h"
#include "KeyMappingDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CKeyMappingDlg dialog

// Default Ctor
CKeyMappingDlg::CKeyMappingDlg(CKeysTable* pKeys, CWnd* pParent)
	: CDialog(CKeyMappingDlg::IDD, pParent), m_pKeys(pKeys),
    m_Right(pKeys, CManouverSet::TURN_RIGHT), 
    m_Left(pKeys, CManouverSet::TURN_LEFT), 
    m_Shell(pKeys, CManouverSet::FIRE_SHELL), 
    m_Bullet(pKeys, CManouverSet::FIRE_BULLET),
    m_Mine(pKeys, CManouverSet::DROP_MINE), 
    m_Backward(pKeys, CManouverSet::BACKWARD), 
    m_Aerial(pKeys, CManouverSet::AERIAL_SUPPORT), 
    m_Forward(pKeys, CManouverSet::FORWARD)
{
	//{{AFX_DATA_INIT(CKeyMappingDlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = TANKS_APP->LoadIcon(IDR_MAINFRAME);
}

void CKeyMappingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CKeyMappingDlg)
	DDX_Control(pDX, IDC_EDIT_TURN_RIGHT, m_Right);
	DDX_Control(pDX, IDC_EDIT_TURN_LEFT, m_Left);
	DDX_Control(pDX, IDC_EDIT_FIRE_SHELL, m_Shell);
	DDX_Control(pDX, IDC_EDIT_FIRE_BULLET, m_Bullet);
	DDX_Control(pDX, IDC_EDIT_DROP_MINE, m_Mine);
	DDX_Control(pDX, IDC_EDIT_BACKWARD, m_Backward);
	DDX_Control(pDX, IDC_EDIT_AERIAL_SUPPORT, m_Aerial);
	DDX_Control(pDX, IDC_EDIT_FORWARD, m_Forward);
	DDX_Control(pDX, IDOK, m_ctrOK);
	DDX_Control(pDX, IDCANCEL, m_ctrCancel);
	DDX_Control(pDX, IDC_BUTTON_DEFAULT_SETTINGS, m_ctrDefault);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CKeyMappingDlg, CDialog)
	//{{AFX_MSG_MAP(CKeyMappingDlg)
	ON_BN_CLICKED(IDC_BUTTON_DEFAULT_SETTINGS, OnButtonDefaultSettings)
	ON_EN_SETFOCUS(IDC_EDIT_AERIAL_SUPPORT, OnSetfocusEditAerialSupport)
	ON_EN_SETFOCUS(IDC_EDIT_BACKWARD, OnSetfocusEditBackward)
	ON_EN_SETFOCUS(IDC_EDIT_DROP_MINE, OnSetfocusEditDropMine)
	ON_EN_SETFOCUS(IDC_EDIT_FIRE_BULLET, OnSetfocusEditFireBullet)
	ON_EN_SETFOCUS(IDC_EDIT_FIRE_SHELL, OnSetfocusEditFireShell)
	ON_EN_SETFOCUS(IDC_EDIT_FORWARD, OnSetfocusEditForward)
	ON_EN_SETFOCUS(IDC_EDIT_TURN_LEFT, OnSetfocusEditTurnLeft)
	ON_EN_SETFOCUS(IDC_EDIT_TURN_RIGHT, OnSetfocusEditTurnRight)
	ON_WM_LBUTTONDOWN()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKeyMappingDlg message handlers

BOOL CKeyMappingDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
/*
	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
*/	
	// Init edit boxes with current keys setting:
    UpdateAllCEdit	();
	m_ctrCancel.LoadAVI(IDR_AVI_CANCEL);
	m_ctrOK.LoadAVI(IDR_AVI_OK);
	m_ctrDefault.LoadAVI(IDR_AVI_DEFAULT);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CKeyMappingDlg::OnOK() 
{
	// UpdateData(TRUE);
    CDialog::OnOK();
}

void CKeyMappingDlg::UpdateAllCEdit()
{
    m_Right.UpdateText();
    m_Left.UpdateText();
    m_Shell.UpdateText();
    m_Bullet.UpdateText();
    m_Mine.UpdateText();
    m_Backward.UpdateText();
    m_Aerial.UpdateText();
    m_Forward.UpdateText();
    /*
    for (int i = 0; i < CManouverSet::MAX_MANOUVER_BIT; i++)
    {
        CString cstr;
            // Get key's caption:
        BOOL KeyIsValid = KeyToString(m_pKeys->GetKey(i), cstr);
        ASSERT (KeyIsValid);
            // Set edit control:
        GetDlgItem(IDC_EDIT_TURN_RIGHT + i) -> SetWindowText(cstr);
    }
    */
}

void CKeyMappingDlg::OnButtonDefaultSettings() 
{
	// Reset all controls to default
	m_pKeys->RestoreDefault();
    UpdateAllCEdit();
}

void CKeyMappingDlg::OnSetfocusEditAerialSupport() 
{
    m_Aerial.PostMessage(WM_LBUTTONDBLCLK,0,0);
}


void CKeyMappingDlg::OnSetfocusEditBackward() 
{
    m_Backward.PostMessage(WM_LBUTTONDBLCLK,0,0);
}

void CKeyMappingDlg::OnSetfocusEditDropMine() 
{
    m_Mine.PostMessage(WM_LBUTTONDBLCLK,0,0);
}

void CKeyMappingDlg::OnSetfocusEditFireBullet() 
{
    m_Bullet.PostMessage(WM_LBUTTONDBLCLK,0,0);
}

void CKeyMappingDlg::OnSetfocusEditFireShell() 
{
    m_Shell.PostMessage(WM_LBUTTONDBLCLK,0,0);
}

void CKeyMappingDlg::OnSetfocusEditForward() 
{
    m_Forward.PostMessage(WM_LBUTTONDBLCLK,0,0);
}

void CKeyMappingDlg::OnSetfocusEditTurnLeft() 
{
    m_Left.PostMessage(WM_LBUTTONDBLCLK,0,0);
}

void CKeyMappingDlg::OnSetfocusEditTurnRight() 
{
    m_Right.PostMessage(WM_LBUTTONDBLCLK,0,0);
}
