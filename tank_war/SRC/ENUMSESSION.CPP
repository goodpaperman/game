// EnumSession.cpp : implementation file
//

#include "stdafx.h"
#include "tanks.h"
#include "dplay.h"
#include "EnumSession.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BOOL g_bEnumAll;    // Global flag used in enum session call back.
/////////////////////////////////////////////////////////////////////////////
// CEnumSession dialog


CEnumSession::CEnumSession(LPDIRECTPLAY2 pIDP, GUID *pSessionGuid)
    : CDialog(CEnumSession::IDD, NULL),
      m_pIDP(pIDP),
      m_pSessionGuid(pSessionGuid),
      m_dwTimeOut(0)
{
    //{{AFX_DATA_INIT(CEnumSession)
        // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
    g_bEnumAll = FALSE;     // By default don't list all sessions.
}


void CEnumSession::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CEnumSession)
    DDX_Control(pDX, IDC_ENUMSESSION_TEXT, m_Static);
    DDX_Control(pDX, IDC_ENUMSESSION_LIST, m_ListBox);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEnumSession, CDialog)
    //{{AFX_MSG_MAP(CEnumSession)
    ON_BN_CLICKED(IDC_REFRESH, OnRefresh)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEnumSession message handlers

void CEnumSession::OnRefresh() 
{
    EmptyListBox();
    g_bEnumAll = (1 == ((CButton*)GetDlgItem(IDC_RADIO_ENUMALL))->GetCheck());
    EnumSession();
}

void CEnumSession::OnCancel() 
{
    EmptyListBox();
    CDialog::OnCancel();
}

void CEnumSession::OnOK() 
{
    int ind = m_ListBox.GetCurSel();
    if (CB_ERR == ind)
    {
        ASSERT(m_ListBox.GetCount());   // We shouldn't be here if list is empty
        AfxMessageBox("No session was selected.\nSelect one or press Cancel");
        return;
    }
    *m_pSessionGuid = *(LPGUID)m_ListBox.GetItemData(ind);
    // Now we can empty the list box.
    EmptyListBox();
    CDialog::OnOK();
}

BOOL CEnumSession::OnInitDialog() 
{
    CDialog::OnInitDialog();
    
    m_Static.SetWindowText("Press Refresh to start looking for sessions");
    GetDlgItem(IDC_REFRESH)->SetFocus();
    GetDlgItem(IDOK)->EnableWindow(FALSE);  // We can't press OK w/o a session
    SetDefID(IDC_REFRESH);
    if (g_bEnumAll)
    {
        ((CButton*)GetDlgItem(IDC_RADIO_ENUMALL))->SetCheck(TRUE);
    } else
    {
        ((CButton*)GetDlgItem(IDC_RADIO_JOINTOFIRST))->SetCheck(TRUE);
    }
    
    return TRUE;  // return TRUE unless you set the focus to a control
                  // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL FAR PASCAL 
EnumSessionsCallback(
    LPCDPSESSIONDESC2 lpSessionDesc, LPDWORD lpdwTimeOut,
    DWORD dwFlags, LPVOID lpContext)
{
    HWND hWnd = (HWND)lpContext;
    HGLOBAL hGuid;
    LPGUID lpGuid;
    LONG iIndex;

    // Determine if the enumeration has timed out.
    if (dwFlags & DPESC_TIMEDOUT)
        return FALSE;               // Do not try again

    // Store the session name in the list
    iIndex = SendDlgItemMessage(hWnd, IDC_ENUMSESSION_LIST, LB_ADDSTRING,
        (WPARAM)0, (LPARAM)lpSessionDesc->lpszSessionNameA);
    if (CB_ERR == iIndex)
        goto FAILURE;

    // Make space for the session instance GUID.
    hGuid = GlobalAlloc(GHND, sizeof(GUID));
    if (! hGuid)
        goto FAILURE;

    lpGuid = (LPGUID)GlobalLock(hGuid);
    if (NULL == lpGuid)
    {
        GlobalUnlock(hGuid);
        goto FAILURE;
    }

    // Store the pointer to the GUID in the list.
    *lpGuid = lpSessionDesc->guidInstance;
    SendDlgItemMessage(hWnd, IDC_ENUMSESSION_LIST, LB_SETITEMDATA,
        (WPARAM) iIndex, (LPARAM) lpGuid);

    // We can decrease the timeout here, after the user is displayed with
    // a session
    *lpdwTimeOut = 0;

    // Check if we can leave after the first session was found.
    if (! g_bEnumAll)
        return FALSE;

FAILURE:
    return TRUE;                   // Try again
}

void
CEnumSession::EnumSession()
{
    // Put an hourglass cursor
    CWaitCursor wc;

    // Set static caption
    m_Static.SetWindowText ("Looking for sessions. This may take a minute.");

    // Disable all buttons
    GetDlgItem(IDCANCEL)->EnableWindow(FALSE);
    GetDlgItem(IDOK)->EnableWindow(FALSE);
    GetDlgItem(IDC_REFRESH)->EnableWindow(FALSE);

    // Enumerate sessions
    DPSESSIONDESC2 SessionDesc;

    ZeroMemory(&SessionDesc, sizeof(DPSESSIONDESC2));
    SessionDesc.dwSize = sizeof(DPSESSIONDESC2);
    SessionDesc.guidApplication = TANKS_GUID;

    // Set the timeout value.
    // We start with 0 (means DPlay put its own value). If we are
    // called again then we restart with 20sec. and after that we mulitply
    // by 2 until we reach 80sec.
    if (m_dwTimeOut < 60000)
    {   
        if (! m_dwTimeOut)         // We don't want to multiply zero..
            m_dwTimeOut = 20000;   // 20 sec.
        else
            m_dwTimeOut *= 2;
    }

    HRESULT hr = m_pIDP->EnumSessions(
        &SessionDesc, 
        m_dwTimeOut, 
        EnumSessionsCallback,
        (LPVOID)m_hWnd, 
        DPENUMSESSIONS_AVAILABLE
        );
    if FAILED(hr)
    {
        TRACE ("EnumSessions failed with error code %ul\n", hr);
        m_Static.SetWindowText("Can not enumerate sessions.");
        // Make the Cancel button the only option.
        GetDlgItem(IDCANCEL)->EnableWindow(TRUE);
        GetDlgItem(IDCANCEL)->SetFocus();
        SetDefID(IDCANCEL);
        return;
    }

    // Enable Cancel and Refresh buttons.
    GetDlgItem(IDCANCEL)->EnableWindow(TRUE);
    GetDlgItem(IDC_REFRESH)->EnableWindow(TRUE);

    // Set static caption
    if (! m_ListBox.GetCount())
    {   // No session were found.
        m_Static.SetWindowText ("No sessions found. "
            "To try again press the refresh button.");
    } else 
    {   // We found a session.
        // Set the text.
        m_Static.SetWindowText ("Select a session and press the OK button.");
        // Enable the OK button.
        GetDlgItem(IDOK)->EnableWindow(TRUE);
        GetDlgItem(IDOK)->SetFocus();
        SetDefID(IDOK);
        // Set the selection on the first session.
        m_ListBox.SetCurSel(0);
        // If we can join the first found - exit dialog.
        if (!g_bEnumAll)
        {
            OnOK();
            return;
        }
    }
}

void
CEnumSession::EmptyListBox()
{
    for (int i = 0; i < m_ListBox.GetCount(); i++)
    {
        HGLOBAL hg = GlobalHandle((LPVOID)m_ListBox.GetItemData(0));
        if (hg)
        {
            GlobalUnlock(hg);
            GlobalFree(hg);
        }
        m_ListBox.DeleteString(0);
    }
}
