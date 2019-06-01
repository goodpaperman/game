/*****************************************************************************
*                                                                             
*   CommSettingDlg.cpp
*                                                                             
*   Electrical Engineering Faculty - Software Lab                             
*   Spring semester 1998                                                      
*                                                                             
*   Tanks game                                                                
*                                                                             
*   Module description: Implements the communiction settings dialog enables 
*                       the user specify a nickname, select between hosting or
*                       connecting to a host and selecting the connection 
*                       provider.
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
#include "CommSettingDlg.h"
#include "CommManager.h"
#include "dplay.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCommSettingDlg dialog


CCommSettingDlg::CCommSettingDlg()
    : CDialog(CCommSettingDlg::IDD, NULL)
{
    //{{AFX_DATA_INIT(CCommSettingDlg)
    m_strPlayerName = _T("");
    //}}AFX_DATA_INIT
}


void CCommSettingDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CCommSettingDlg)
    DDX_Control(pDX, IDOK, m_ctrOK);
    DDX_Control(pDX, IDCANCEL, m_ctrCancel);
    DDX_Control(pDX, IDC_SPLISTBOX, m_ConnectionsList);
    DDX_Text(pDX, IDC_PLAYERNAME_EDIT, m_strPlayerName);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCommSettingDlg, CDialog)
    //{{AFX_MSG_MAP(CCommSettingDlg)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCommSettingDlg message handlers


void CCommSettingDlg::OnOK() 
{
    // Get data from text box:
    UpdateData(TRUE);
    // Return data to CommManager:
    TANKS_APP->SetStoredPlayerName(m_strPlayerName);
    TANKS_APP->SetStoredGUID (
        *(LPGUID)(m_ConnectionsList.GetItemData(m_ConnectionsList.GetCurSel())) );
    BOOL bIsHost = (1 == ((CButton*)GetDlgItem(IDC_HOST_RADIO))->GetCheck());
    TANKS_APP->SetStoredIsHostFlag (bIsHost);
    AfxGetMainWnd()->GetMenu()->EnableMenuItem 
        (ID_SERVER_MGMT, bIsHost ? (MF_BYCOMMAND | MF_ENABLED) : (MF_BYCOMMAND | MF_GRAYED));
    CDialog::OnOK();
}

BOOL CCommSettingDlg::OnInitDialog() 
{
    CDialog::OnInitDialog();

    // Limit EditBox text to max player name:
    ((CEdit*)GetDlgItem(IDC_PLAYERNAME_EDIT))->LimitText(MAX_PLAYER_NAME);

    // Init player name:
    m_strPlayerName = TANKS_APP->GetStoredPlayerName();

    if (TANKS_APP->GetStoredIsHostFlag())
        // Init radio button on Host:
        ((CButton*)GetDlgItem(IDC_HOST_RADIO))->SetCheck(TRUE);
    else
        ((CButton*)GetDlgItem(IDC_PLAYER_RADIO))->SetCheck(TRUE);

    // Init the connection list box:
    DirectPlayEnumerate (EnumConnections, m_hWnd);
    
    // Init animated buttons:
    m_ctrCancel.LoadAVI(IDR_AVI_CANCEL);
    m_ctrOK.LoadAVI(IDR_AVI_OK);

    UpdateData(FALSE);
    
    // Select stored GUID, if exists:
    SelectStoredGUID();

    GetDlgItem(IDOK)->SetFocus();

    return TRUE;  // return TRUE unless you set the focus to a control
}

BOOL FAR PASCAL EnumConnections(
    LPGUID lpSPGuid, 
    LPTSTR lpszSPName, 
    DWORD /*dwMajorVersion*/,
    DWORD /*dwMinorVersion*/, 
    LPVOID lpContext)
{
    HWND    hWnd = (HWND)lpContext;
    LRESULT iIndex;

    // Store the service provider name:
    iIndex = SendDlgItemMessage(hWnd, IDC_SPLISTBOX, LB_ADDSTRING,
        0, (LPARAM) lpszSPName);

    // Store the service provider guid:
    if (iIndex != LB_ERR)
        SendDlgItemMessage(hWnd, IDC_SPLISTBOX, LB_SETITEMDATA, 
            (WPARAM) iIndex, (LPARAM) lpSPGuid);

    return(TRUE);
}

void
CCommSettingDlg::SelectStoredGUID()
{
    GUID guidStored;
    int ind = 0;

    // Check if there is a stored GUID:
    if (TANKS_APP->GetStoredGUID(&guidStored)) 
    {
        // Find index of entry with the same GUID as in registry:
        for (; ind < m_ConnectionsList.GetCount(); ind++)
            if (*(LPGUID)(m_ConnectionsList.GetItemData(ind)) == guidStored)
                break;
        // If the stored GUID doesn't match any of the enumerated GUID's
        // select the 1st:
        if (ind == m_ConnectionsList.GetCount())
            ind = 0;
    }
    // Select stored GUID:
    m_ConnectionsList.SetCurSel(ind);
}