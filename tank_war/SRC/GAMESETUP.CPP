/*****************************************************************************
*                                                                             
*   GameSetup.cpp
*                                                                             
*   Electrical Engineering Faculty - Software Lab                             
*   Spring semester 1998                                                      
*                                                                             
*   Tanks game                                                                
*                                                                             
*   Module description: Implements the game setup dialog that enables the user
*                       to specify the tank's color and game board complexity.
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
#include "Tanks.h"
#include "TanksDlg.h"
#include "GameSetup.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGameSetup dialog


CGameSetup::CGameSetup(CWnd* pParent /*=NULL*/)
	: CDialog(CGameSetup::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGameSetup)
	//}}AFX_DATA_INIT
}


void CGameSetup::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGameSetup)
	DDX_Control(pDX, IDC_COMPLEXITY, m_Complexity);
	DDX_Control(pDX, IDOK, m_ctrOK);
	DDX_Control(pDX, IDCANCEL, m_ctrCancel);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGameSetup, CDialog)
	//{{AFX_MSG_MAP(CGameSetup)
	ON_WM_LBUTTONDOWN()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGameSetup message handlers

BOOL CGameSetup::OnInitDialog() 
{
	CDialog::OnInitDialog();

    RECT MyRect;
    GetWindowRect (&MyRect);
    SetWindowPos (&wndTop, MyRect.left, MyRect.top,
                  MAX_TANKS * (10+TANK_ANIM_WIDTH) + 14, MyRect.bottom - MyRect.top, 0);
    for (int i = 0; i < MAX_TANKS; i++) 
        m_AnimTanks[i].Init (i+1, this);

    m_Complexity.SetRange (1,20, TRUE);
	m_ctrCancel.LoadAVI(IDR_AVI_CANCEL);
	m_ctrOK.LoadAVI(IDR_AVI_OK);
    LoadSettings();
    m_AnimTanks[m_uSelectedTankID].Select ();   // Start with selected tank

    CTanksDlg *pTanksDlg = (CTanksDlg *)GetParent();
    if (!(TANKS_APP->GetStoredIsHostFlag()) ||  // Machine configured to play as a client or
        pTanksDlg->IsGameOn())                  // Game is in session
    {
        // Don't allow the user to change map complexity level:
        m_Complexity.EnableWindow (FALSE);
        GetDlgItem (IDC_STATIC1)->EnableWindow (FALSE); // Static frame
        GetDlgItem (IDC_STATIC2)->EnableWindow (FALSE); // "Complex" static text
        GetDlgItem (IDC_STATIC3)->EnableWindow (FALSE); // "Simple" static text
    }
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}



void CGameSetup::OnLButtonDown(UINT nFlags, CPoint point) 
{
    CWnd *pSon = ChildWindowFromPoint (point);
    for (UINT i=0; i<MAX_TANKS; i++)
        if (pSon == GetDlgItem (IDC_ANIMATE_TANK1+i))
        {
            if (i == m_uSelectedTankID)
                break;  // Already selected

                // Stop previously selected tank:
            m_AnimTanks[m_uSelectedTankID].Unselect ();
                // Select this one:
            ((CAnimateTank*)pSon)->Select();
            m_uSelectedTankID = i;
            break;
        }
	CDialog::OnLButtonDown(nFlags, point);
}

void CGameSetup::OnOK() 
{
    if (!UpdateData())
        // Error in data
        return;

    BOOL bRes =
        TANKS_APP->WriteProfileInt (GAME_SETUP_SECTION, TANK_TYPE_ENTRY, m_uSelectedTankID);
    if (bRes)
        bRes = TANKS_APP->WriteProfileInt (GAME_SETUP_SECTION, MAP_DIFF_ENTRY, m_Complexity.GetPos());
    if (!bRes)
    {
        AfxMessageBox (IDS_CANT_SAVE_GAME_SETUP);
        return;
    }
    CDialog::OnOK();
}

void CGameSetup::LoadSettings()
{
    m_uSelectedTankID = 
        TANKS_APP->GetStoredPreferedTankID();
    m_Complexity.SetPos (TANKS_APP->GetStoredMapComplexity());
    UpdateData (FALSE);
}

