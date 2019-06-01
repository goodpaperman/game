/*****************************************************************************
*                                                                             
*   ServerMgmtDlg.cpp                                                            
*                                                                             
*   Electrical Engineering Faculty - Software Lab                             
*   Spring semester 1998                                                      
*                                                                             
*   Tanks game                                                                
*                                                                             
*   Module description: Implements the server management dialog.
*                       Displays the host game instance with details of 
*                       players connected to session, and enables the host to
*                       disconnect the players.
*                       
*                                                                             
*   Authors: Eran Yariv - 28484475                                           
*            Moshe Zur  - 24070856                                           
*                                                                            
*                                                                            
*   Date: 23/09/98                                                           
*                                                                            
******************************************************************************/
// servermgmtdlg.cpp : implementation file
//

#include "stdafx.h"
#include <Tanks.h>
#include <ServerMgmtDlg.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define MAKE_PLAYER_DATA(id,judge)          (DWORD((id) + ((judge) << 31)))
#define GET_PLAYER_ID(data)                 ((data) & (0x7FFFFFFF))
#define IS_JUDGE(data)                      (BOOL((data) >> 31))

/////////////////////////////////////////////////////////////////////////////
// CServerMgmtDlg dialog


CServerMgmtDlg::CServerMgmtDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CServerMgmtDlg::IDD, pParent),
      m_gCommManager (TANKS_APP->m_gCommManager)
{
	//{{AFX_DATA_INIT(CServerMgmtDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CServerMgmtDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CServerMgmtDlg)
	DDX_Control(pDX, IDC_PLAYERS_LIST, m_PlayersList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CServerMgmtDlg, CDialog)
	//{{AFX_MSG_MAP(CServerMgmtDlg)
	ON_BN_CLICKED(IDC_DICONNECT, OnDisconnectPlayer)
	ON_WM_CLOSE()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CServerMgmtDlg * CServerMgmtDlg::m_pDlg = NULL;

/////////////////////////////////////////////////////////////////////////////
// CServerMgmtDlg message handlers

#define TIMER_ID            55
#define REFRESH_RATE      1000


void CServerMgmtDlg::OnClose() 
{
    KillTimer (TIMER_ID);
	CDialog::OnClose();
}

void 
CServerMgmtDlg::PostNcDestroy( )
{
    Close ();
}

void 
CServerMgmtDlg::OnCancel ()
{
    DestroyWindow ();
}


int CServerMgmtDlg::GetCurSelection()
{
    return m_PlayersList.GetNextItem (-1, LVNI_SELECTED);
}

BOOL CServerMgmtDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

    m_ImgList.Create (16,16,FALSE,0,100);
    m_PlayersList.SetImageList (&m_ImgList, LVSIL_SMALL);
    HINSTANCE hinst = AfxGetInstanceHandle();
    for (int i=0; i<MAX_TANKS; i++)
    {
        HICON hIcon= LoadIcon (hinst, MAKEINTRESOURCE(IDI_TANK1 + i));
        m_ImgIndex[i] = m_ImgList.Add (hIcon);
    }
    m_PlayersList.InsertColumn (0,"Player name", LVCFMT_LEFT, 140);
    m_PlayersList.InsertColumn (1,"Duration", LVCFMT_LEFT, 55);
    m_PlayersList.InsertColumn (2,"Round trip time", LVCFMT_LEFT, 90);
    m_PlayersList.SetBkColor (RGB(0,0,0));
    m_PlayersList.SetTextColor (RGB(255,255,255));
    m_PlayersList.SetTextBkColor (RGB(0,0,0));
    m_PlayersList.SendMessage (LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LVS_EX_FULLROWSELECT);
	
	SetTimer  (TIMER_ID, REFRESH_RATE, NULL);

    RefreshList (); // Do 1st refresh
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CServerMgmtDlg::OnTimer(UINT nIDEvent) 
{
    RefreshList ();
	CDialog::OnTimer(nIDEvent);
}


void 
CServerMgmtDlg::Open (CWnd *pParent)
{   // This function is static !!!!
    if (NULL != m_pDlg)
        return; // Already open
    m_pDlg = new CServerMgmtDlg (pParent);
    ASSERT (m_pDlg);
    m_pDlg->Create (IDD_SERVER_MGMT, pParent);
    m_pDlg->ShowWindow (SW_SHOW);
    m_pDlg->SetFocus ();
}

void
CServerMgmtDlg::Close ()
{   // This function is static !!!!
    if (NULL == m_pDlg)
        return; // Already closed
    delete m_pDlg;	// Tricky - delete only after call to PostNcDestroy !!!
    m_pDlg = NULL;
}

void 
CServerMgmtDlg::UpdateOrAddPlayersData (PBOOL pbPlayers)
{
    CString cstrDuration,
            cstrLatency,
            cstrPlayerName;
    int index;
    for (int i=0; i<MAX_TANKS; i++)
    {
        DWORD   dwDuration;
        DWORD   dwLatency;
        BOOL    bIsJudge;
        
        if (m_gCommManager.GetPlayerInfo (i,
                                          bIsJudge,
                                          cstrPlayerName,
                                          dwDuration,   // In millisecs
                                          dwLatency))
        { // Player i exists
            pbPlayers[i] = TRUE;
            dwDuration /= 1000; // Convert to secs
            cstrDuration.Format ("%02d:%02d", dwDuration / 60, dwDuration % 60);
            cstrLatency.Format ("%4d millisecs", dwLatency);
            index = FindPlayerIndex (i);    // Find player's i index in the list
            DWORD dwItemData = MAKE_PLAYER_DATA(i, bIsJudge);
            if (-1 == index)
            {   // Player is new to the list
                index = m_PlayersList.InsertItem (LVIF_TEXT | LVIF_IMAGE, 
                                                  i, 
                                                  cstrPlayerName,
                                                  0,
                                                  0,
                                                  m_ImgIndex[i],
                                                  dwItemData);
                m_PlayersList.SetItemData (index, dwItemData);
            }
            m_PlayersList.SetItem (index, 
                                   1, 
                                   LVIF_TEXT, 
                                   cstrDuration,
                                   0,
                                   0,
                                   0,
                                   0);
            m_PlayersList.SetItem (index, 
                                   2, 
                                   LVIF_TEXT, 
                                   cstrLatency,
                                   0,
                                   0,
                                   0,
                                   0);
        }
    }
}


void 
CServerMgmtDlg::RemoveNonExistantPlayers (PBOOL pbPlayers)
{
    int index = -1;
    BOOL bStopScan = FALSE;
    while (!bStopScan)
    {
        index = m_PlayersList.GetNextItem(index, 0);
        if (-1 != index)
        {
            DWORD dwPlayerData = m_PlayersList.GetItemData(index);
            DWORD dwPlayerID = GET_PLAYER_ID(dwPlayerData);
            ASSERT (dwPlayerID < MAX_TANKS);
            if (!pbPlayers[dwPlayerID])
            {   // Found non-existing player in the list ,remove it
                m_PlayersList.DeleteItem (index);
                index = -1; // Start scanning from list head
            }
        }
        else
        {   // End of list
            bStopScan = TRUE;
        }
    }
}

void 
CServerMgmtDlg::RefreshList()
{
    m_PlayersList.SetRedraw (FALSE);
    BOOL abPlayerExists[MAX_TANKS];
    memset (abPlayerExists, 0, sizeof (abPlayerExists));
    UpdateOrAddPlayersData (abPlayerExists);
    RemoveNonExistantPlayers (abPlayerExists);
    m_PlayersList.SetRedraw (TRUE);
}

void CServerMgmtDlg::OnDisconnectPlayer() 
{
    int i = GetCurSelection ();
    if (-1 == i)
    {
        AfxMessageBox (IDS_MUST_SELECT_PLAYER_TO_REMOVE, MB_OK | MB_ICONINFORMATION);
        return; // No item selected
    }
    DWORD dwPlayerData = m_PlayersList.GetItemData (i);
    BOOL bIsJudge = IS_JUDGE (dwPlayerData);
    if (bIsJudge)
    {   // User tried to kill the judge - Never allow this
        AfxMessageBox (IDS_CANT_KILL_JUDGE, MB_OK | MB_ICONINFORMATION);
        return; // No item selected
    }
    DWORD dwPlayerID = GET_PLAYER_ID (dwPlayerData);
    ASSERT (dwPlayerID < MAX_TANKS);
    // Now, ask the host to kill the tank (dwPlayerID)
	if (!m_gCommManager.KillPlayer (dwPlayerID))
    {   // Can't kill - either already dead or bad ID
        AfxMessageBox (IDS_CANT_KILL_PLAYER, MB_OK | MB_ICONINFORMATION);
        return; // No item selected
    }
}

int CServerMgmtDlg::FindPlayerIndex (UINT uPlayerID)
{
    for (int index=-1; ;)
    {
        index = m_PlayersList.GetNextItem(index, 0);
        if (-1 == index)
            // end of list - return not found 
            return -1;

        DWORD dwPlayerData = m_PlayersList.GetItemData(index);
        DWORD dwPlayerID = GET_PLAYER_ID(dwPlayerData);
        if (uPlayerID == dwPlayerID)
            return index;
    }
}
            
