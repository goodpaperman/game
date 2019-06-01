/*****************************************************************************
*                                                                             
*   ChatDlg.cpp                                                            
*                                                                             
*   Electrical Engineering Faculty - Software Lab                             
*   Spring semester 1998                                                      
*                                                                             
*   Tanks game                                                                
*                                                                             
*   Module description: The Chat Dialog implementation.
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
#include "ChatDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChatDlg dialog

CChatDlg * CChatDlg::m_pDlg = NULL;


CChatDlg::CChatDlg(CWnd* pParent /*=NULL*/)
    : CDialog(CChatDlg::IDD, pParent),
      m_gCommManager (TANKS_APP->m_gCommManager)
{
    //{{AFX_DATA_INIT(CChatDlg)
    //}}AFX_DATA_INIT
}


void CChatDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CChatDlg)
    DDX_Control(pDX, IDC_CHAT_LIST, m_List);
    DDX_Control(pDX, IDC_CHAT_EDIT, m_Edit);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CChatDlg, CDialog)
    //{{AFX_MSG_MAP(CChatDlg)
    ON_WM_CLOSE()
    ON_WM_CTLCOLOR()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChatDlg message handlers

BOOL CChatDlg::OnInitDialog() 
{
    CDialog::OnInitDialog();
    
    // Set edit as default control:
    m_Edit.LimitText(MAX_CHAT_MSG_LEN);
    m_Edit.SetFocus();

    return FALSE;  // return TRUE unless you set the focus to a control
                  // EXCEPTION: OCX Property Pages should return FALSE
}

void
CChatDlg::Close()
{
    if (m_pDlg)
        m_pDlg->DestroyWindow();
}

void 
CChatDlg::PostNcDestroy( )
{
    Clean ();
}

/*------------------------------------------------------------------------------

  Function: Open

  Purpose:  This static function creates a new Chat dialog. It allows single
            dialog Chat to be open at a time.

  Input:    pParent: Pointer to the main game dialog.

  Output:   None.

------------------------------------------------------------------------------*/
void 
CChatDlg::Open (CWnd *pParent)
{   // This function is static !!!!
    if (NULL != m_pDlg)
        return; // Already open
    m_pDlg = new CChatDlg (pParent);
    ASSERT (m_pDlg);
    m_pDlg->Create (IDD_CHAT_DIALOG, pParent);
    m_pDlg->ShowWindow (SW_SHOW);
    m_pDlg->SetFocus ();
}

/*------------------------------------------------------------------------------

  Function: Clean

  Purpose:  Called just before exiting the dialog, to delete the object.

  Input:    None.

  Output:   None.

------------------------------------------------------------------------------*/
void
CChatDlg::Clean ()
{   // This function is static !!!!
    if (NULL == m_pDlg)
        return; // Already closed
    delete m_pDlg;  // Tricky - delete only after call to PostNcDestroy !!!
    m_pDlg = NULL;
}

void 
CChatDlg::OnCancel ()
{
    DestroyWindow ();
}


void 
CChatDlg::OnClose() 
{
    CDialog::OnClose(); 
}

/*------------------------------------------------------------------------------

  Function: OnOK

  Purpose:  Send Chat message when user press the Enter key.

  Input:    None.

  Output:   None.

  Remarks:   

------------------------------------------------------------------------------*/
void
CChatDlg::OnOK()
{
    CString cstrMsg;
    m_Edit.GetWindowText(cstrMsg);
    if (cstrMsg.IsEmpty())
        return;
    if (cstrMsg.GetLength() > MAX_CHAT_MSG_LEN)
        cstrMsg.SetAt(MAX_CHAT_MSG_LEN, '\0');
    m_gCommManager.NotifyChatMsg (cstrMsg);
    // Clear edit box:
    m_Edit.SetSel (0, -1);  // All
    m_Edit.Clear();
    return;
}

/*------------------------------------------------------------------------------

  Function: AddMessage

  Purpose:  The function is called on the arrival of a Chat message over the net.
            The Chat message contains the string and information used to get the
            sending player's name and tank's color.

  Input:    idFrom: DirectPlay ID of the sending player.
            dwTankID: ID of tank in game manager.
            szMsg: String containing the message.

  Output:   None.

  Remarks:  When the dialog is opened, we have 2 instances of CChatDlg:
            The one in the main dialog which we use for the open and close calls,
            and the one we point to after the call to open.
            Since we are interesting in the 2nd object when a new message should
            be displayed, we need to look at the this pointer to determine which
            instance of ChatDlg is currently invoked.

------------------------------------------------------------------------------*/
void 
CChatDlg::AddMessage(DPID idFrom, DWORD dwTankID, LPCSTR szMsg)
{
    if (! m_pDlg)   // User didn't open the dialog
        return;
    if (this != (CChatDlg*)m_pDlg)
    {   // Marshal the call to the dlg object:
        m_pDlg->AddMessage(idFrom, dwTankID, szMsg);
        return;
    }
    m_List.SetRedraw (FALSE);
    // Format String
    CString str;
    CString cstrName;
    m_gCommManager.GetPlayerName(idFrom, cstrName);
    str.Format ("%s: %s", cstrName, szMsg);
    // Try to add string to list box:
    int ind = m_List.AddString (str);
    if (LB_ERRSPACE == ind)
    {   // Delete the oldest string in the list box
        m_List.DeleteString (0);
        ind = m_List.AddString (str);
    }
    if (LB_ERR == ind)     // Unknown error
    {
        m_List.SetRedraw (TRUE);
        return;
    }
    m_List.SetItemData (ind, dwTankID);  // attach tank ID to line
    m_List.SetTopIndex (ind);
    m_List.SetRedraw (TRUE);
}

/*
HBRUSH CChatDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
    HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);


    if (CTLCOLOR_LISTBOX == nCtlColor &&
        m_List.GetCount())
    {
        COLORREF crColor;
        switch (m_List.GetItemData(m_List.GetCount()-1))
        {
        case 0: crColor = RGB(128,  64,   8); break;    // Brown
        case 1: crColor = RGB(  0,   0, 255); break;    // Blue
        case 2: crColor = RGB(255,   0,   0); break;    // Red
        case 3: crColor = RGB(180, 180,  20); break;    // Yellow
        default:
            ASSERT(FALSE);
        }
        pDC->SetTextColor (crColor);
    }

    // TODO: Return a different brush if the default is not desired
    return hbr;
}
*/