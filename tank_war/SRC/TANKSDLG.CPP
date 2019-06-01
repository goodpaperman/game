/*****************************************************************************
*                                                                             
*   TanksDlg.cpp
*                                                                             
*   Electrical Engineering Faculty - Software Lab                             
*   Spring semester 1998                                                      
*                                                                             
*   Tanks game                                                                
*                                                                             
*   Module description: Implements the main game dialog.
*                       
*                                                                             
*   Authors: Eran Yariv - 28484475                                           
*            Moshe Zur  - 24070856                                           
*                                                                            
*                                                                            
*   Date: 23/09/98                                                           
*                                                                            
******************************************************************************/
#include <time.h>

#include "stdafx.h"
#include "Tanks.h"
#include "TanksDlg.h"
#include <GameSetup.h>
#include <GameBoard.h>
#include <TankObj.h>
#include <GameOver.h>
#include <Message.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
    CAboutDlg();

// Dialog Data
    //{{AFX_DATA(CAboutDlg)
    enum { IDD = IDD_ABOUTBOX };
    //}}AFX_DATA

    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CAboutDlg)
    protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:
    //{{AFX_MSG(CAboutDlg)
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
    //{{AFX_DATA_INIT(CAboutDlg)
    //}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CAboutDlg)
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
    //{{AFX_MSG_MAP(CAboutDlg)
    ON_WM_LBUTTONDOWN()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTanksDlg dialog

CTanksDlg::CTanksDlg(CWnd* pParent /*=NULL*/)
    : CDialog(CTanksDlg::IDD, pParent),
      m_GameState (GAME_OFF),
      m_KbdManager(TANKS_APP->m_gKbdManager),
      m_GameManager(TANKS_APP->m_gGameManager)
{
    //{{AFX_DATA_INIT(CTanksDlg)
        // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
    // Note that LoadIcon does not require a subsequent DestroyIcon in Win32
    m_hIcon = TANKS_APP->LoadIcon(IDR_MAINFRAME);
}

CTanksDlg::~CTanksDlg ()
{
    TANKS_APP->m_gDrawDIB.Close ();
}

void CTanksDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CTanksDlg)
    DDX_Control(pDX, IDC_AMMO_STATUS, m_AmmoStatus);
    DDX_Control(pDX, IDC_BONUS_STATUS, m_BonusStatus);
    DDX_Control(pDX, IDC_SHIELD_STATUS, m_ShieldStatus);
    DDX_Control(pDX, IDC_MAP, m_Map);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTanksDlg, CDialog)
    //{{AFX_MSG_MAP(CTanksDlg)
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_COMMAND(ID_EXIT, OnExit)
    ON_COMMAND(ID_HELP_ABOUT, OnHelpAbout)
    ON_COMMAND(ID_KBD_MAPPING, OnKbdMapping)
    ON_COMMAND(ID_NEW_GAME, OnNewGame)
    ON_COMMAND(ID_SET_COMM, OnSetComm)
    ON_COMMAND(ID_SET_GAME, OnSetGame)
    ON_COMMAND(ID_STOP_GAME, OnStopGame)
	ON_COMMAND(ID_SOUND_MUTE, OnSoundMute)
    ON_WM_DRAWITEM()
    ON_WM_CLOSE()
    ON_WM_QUERYNEWPALETTE()
    ON_WM_PALETTECHANGED()
    ON_COMMAND(ID_SERVER_MGMT, OnServerMgmt)
    ON_WM_ERASEBKGND()
	ON_COMMAND(ID_PALYERS_GUIDE, OnPlayersGuide)
	ON_COMMAND(ID_CHAT_OPEN, OnChatOpen)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTanksDlg message handlers

BOOL CTanksDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

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
    SetIcon(m_hIcon, TRUE);         // Set big icon
    SetIcon(m_hIcon, FALSE);        // Set small icon


        // Allign the main dialog, the map and the status bars
    AlignControls();    
        // Start with the "stop game" menu option disabled
    GetMenu()->EnableMenuItem(ID_STOP_GAME, MF_BYCOMMAND | MF_GRAYED);
    GetMenu()->EnableMenuItem 
        (ID_SERVER_MGMT, TANKS_APP->GetStoredIsHostFlag() ? 
            (MF_BYCOMMAND | MF_ENABLED) : (MF_BYCOMMAND | MF_GRAYED));
        // Start with "chat open" option disabled
    GetMenu()->EnableMenuItem(ID_CHAT_OPEN, MF_BYCOMMAND | MF_GRAYED);

        // Start the image manager
    TANKS_APP->m_gImageManager.LoadImages ();
        // Start the sound manager
    if (! TANKS_APP->m_gSoundManager.Init(m_hWnd))
    {   // Failed to create DirectSound, disable sound menu item:
        GetMenu()->EnableMenuItem(ID_SOUND_MUTE, MF_BYCOMMAND | MF_GRAYED);
            // To be on the safe side :
        TANKS_APP->m_gSoundManager.SetMuteMode(TRUE);   
    } else
    {
        BOOL bMuteMode = TANKS_APP->GetStoredMuteMode();

        GetMenu()->EnableMenuItem(ID_SOUND_MUTE, MF_BYCOMMAND | MF_ENABLED);
        GetMenu()->CheckMenuItem(ID_SOUND_MUTE, 
             bMuteMode ? MF_CHECKED : MF_UNCHECKED);
            // Init the mute mode
        TANKS_APP->m_gSoundManager.SetMuteMode(bMuteMode);   
    }
        // Create status bars
    CreateStatusPanes ();
    StartDIBDrawingEngine ();
    CheckDPlayVersion();
    return TRUE;  // return TRUE  unless you set the focus to a control
}

void CTanksDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
    if ((nID & 0xFFF0) == IDM_ABOUTBOX)
    {
        CAboutDlg dlgAbout;
        dlgAbout.DoModal();
    }
    else
    {
        CDialog::OnSysCommand(nID, lParam);
    }
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CTanksDlg::OnPaint() 
{
    if (IsIconic())
    {
        CPaintDC dc(this); // device context for painting

        SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

        // Center icon in client rectangle
        int cxIcon = GetSystemMetrics(SM_CXICON);
        int cyIcon = GetSystemMetrics(SM_CYICON);
        CRect rect;
        GetClientRect(&rect);
        int x = (rect.Width() - cxIcon + 1) / 2;
        int y = (rect.Height() - cyIcon + 1) / 2;

        // Draw the icon
        dc.DrawIcon(x, y, m_hIcon);
    }
    else
    {
        CDialog::OnPaint();
    }
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CTanksDlg::OnQueryDragIcon()
{
    return (HCURSOR) m_hIcon;
}


void CTanksDlg::OnExit() 
{
    if (IsGameOn()) {
        UINT uStrInd = TANKS_APP->m_gCommManager.IsHost() 
            ? IDS_HOST_ON_EXIT 
            : IDS_PLAYER_ON_EXIT;
        if (IDNO == AfxMessageBox(uStrInd, MB_YESNO | MB_ICONQUESTION))
            return;
    }
    CDialog::OnCancel();
}

void CTanksDlg::OnHelpAbout() 
{
    CAboutDlg dlgAbout;
    dlgAbout.DoModal();
}

void CTanksDlg::OnKbdMapping() 
{
    TANKS_APP->m_gKbdManager.SetKbdMapping();

}

void CTanksDlg::OnSetComm() 
{
    TANKS_APP->m_gCommManager.SetCommParams();  
}

void CTanksDlg::OnSetGame() 
{
    CGameSetup GameSetupDlg;
    GameSetupDlg.DoModal(); 
}

void CTanksDlg::OnNewGame() 
{
        // Can only be called from GAME_OFF state
    ASSERT (GAME_OFF == m_GameState);
        // Disable "new game" menu item
    GetMenu()->EnableMenuItem(ID_NEW_GAME, MF_BYCOMMAND | MF_GRAYED);
        // Enable "stop game" menu item
    GetMenu()->EnableMenuItem(ID_STOP_GAME, MF_BYCOMMAND | MF_ENABLED);
        // Enable "chat open" menu item
    GetMenu()->EnableMenuItem(ID_CHAT_OPEN, MF_BYCOMMAND | MF_ENABLED);

    TANKS_APP->SetMapHWND( m_Map.m_hWnd );

    if (! TANKS_APP->StartGameThreads ()) {
            // Enable "new game" menu item
        GetMenu()->EnableMenuItem(ID_NEW_GAME, MF_BYCOMMAND | MF_ENABLED);
            // Disable "stop game" menu item
        GetMenu()->EnableMenuItem(ID_STOP_GAME, MF_BYCOMMAND | MF_GRAYED);
        return;
    }
        // Show game controls
    HideGameControl (m_ShieldStatus, FALSE);
    HideGameControl (m_AmmoStatus, FALSE);
    HideGameControl (m_BonusStatus, FALSE);

    m_GameState = GAME_ON;
}

void CTanksDlg::GameOver()
{
        // Can only be called from GAME_ON state
    ASSERT (GAME_ON == m_GameState);
        // Disable "stop game" menu item
    GetMenu()->EnableMenuItem(ID_STOP_GAME, MF_BYCOMMAND | MF_GRAYED);
    m_GameState = GAME_OVER;
        // Disable chat dialog. We can leave it open though, because our tank ID
        // is still occupied:
    GetMenu()->EnableMenuItem(ID_CHAT_OPEN, MF_BYCOMMAND | MF_GRAYED);

        // Add the GameOver animation object to the game:
    CMessage::MessageData DummyParams;
    TANKS_APP->m_gIncomingMsgQueue.Enqueue (CMessage::ADD_GAME_OVER, DummyParams);
}

void CTanksDlg::OnStopGame() 
// Called after the user pressed a key in the "Game-Over" animation sequence
{
        // Can only be called from GAME_OVER or GAME_ONstate
    ASSERT (GAME_OFF != m_GameState);
        // Warn host before we continue:
    if (TANKS_APP->m_gCommManager.IsHost()) {
        if (IDNO == AfxMessageBox(IDS_HOST_ON_STOP, MB_YESNO | MB_ICONQUESTION))
            return;
    }

        // Hide the controls
    HideGameControl (m_ShieldStatus, TRUE);
    HideGameControl (m_AmmoStatus, TRUE);
    HideGameControl (m_BonusStatus, TRUE);
    
        // Enable "new game" menu item
    GetMenu()->EnableMenuItem(ID_NEW_GAME, MF_BYCOMMAND | MF_ENABLED);
        // Disable "stop game" menu item
    GetMenu()->EnableMenuItem(ID_STOP_GAME, MF_BYCOMMAND | MF_GRAYED);
        // Disable "chat open" menu item
    m_ChatDlg.Close();
    GetMenu()->EnableMenuItem(ID_CHAT_OPEN, MF_BYCOMMAND | MF_GRAYED);

    m_GameState = GAME_OFF;

        // Stop player, and close session:
    TANKS_APP->m_gCommManager.OnStopGame();

    TANKS_APP->EndGameThreads();
        // Cause the logo screen to redraw
    m_Map.Invalidate (TRUE);
}



void CTanksDlg::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
    CDC *pdc = CDC::FromHandle (lpDrawItemStruct->hDC);

    switch (nIDCtl) {
        case IDC_MAP:   // Draw map region 
            { 
                if (m_GameState != GAME_OFF)
                {   // Let the renderer do its job
                    m_GameManager.RefreshDisplay ();
                }
                else 
                {
                    // Now paste the splash screen to the DC please........

                    CDIB dib;
                    if (dib.ReadFromResource (IDB_SPLASH))
                        dib.Paint (pdc);
                }
            }
            break;

        case IDC_SHIELD_STATUS:
            m_ShieldStatus.GetImage () -> Paint (pdc);
            break;

        case IDC_AMMO_STATUS:
            m_AmmoStatus.GetImage () -> Paint (pdc);
            break;

        case IDC_BONUS_STATUS:
            m_BonusStatus.GetImage () -> Paint (pdc);
            break;

        default:
            CDialog::OnDrawItem(nIDCtl, lpDrawItemStruct);
            break;
    }
}

void CTanksDlg::AlignControls()
{

    CRect ctlRect, dlgRect;

    GetWindowRect (dlgRect);
    GetClientRect (ctlRect);

    int iExtraRoom = dlgRect.Height() - ctlRect.Height() + 10;

    MoveWindow (dlgRect.left, dlgRect.top, 
                MAP_WIDTH + 20 + SHIELD_STATUS_WIDTH + AMMO_STATUS_WIDTH,
                MAP_HEIGHT + iExtraRoom);

    m_ShieldStatus.GetClientRect (ctlRect);
    m_ShieldStatus.MoveWindow ( ctlRect.left + 5,
                                ctlRect.top + 5, 
                                SHIELD_STATUS_WIDTH, 
                                MAP_HEIGHT,
                                FALSE);

    m_Map.GetClientRect (ctlRect);
    int iMapX = ctlRect.left + 10 + SHIELD_STATUS_WIDTH,
        iMapY = ctlRect.top + 5;

    m_Map.MoveWindow (iMapX, iMapY, MAP_WIDTH, MAP_HEIGHT, FALSE);

    m_AmmoStatus.GetClientRect (ctlRect);
    m_AmmoStatus.MoveWindow (   ctlRect.left + MAP_WIDTH + SHIELD_STATUS_WIDTH + 15, 
                                ctlRect.top + 5, 
                                AMMO_STATUS_WIDTH, 
                                AMMO_STATUS_HEIGHT,
                                FALSE);

    m_BonusStatus.GetClientRect (ctlRect);
    m_BonusStatus.MoveWindow (  ctlRect.left + MAP_WIDTH + SHIELD_STATUS_WIDTH + 15, 
                                ctlRect.top + AMMO_STATUS_HEIGHT + 5, 
                                AMMO_STATUS_WIDTH, 
                                MAP_HEIGHT - AMMO_STATUS_HEIGHT,
                                FALSE);
}

BOOL CTanksDlg::OnEraseBkgnd(CDC* pDC) 
{
    // Keep Windows from erasing the background of our map
    if (pDC == m_Map.GetDC())
        return TRUE;
    return CDialog::OnEraseBkgnd (pDC);
}

BOOL 
CTanksDlg::PreTranslateMessage(MSG* pMsg)
{
    if (GAME_OFF != m_GameState)
    {
        // Catch key strokes only if a game is active
        switch (pMsg->message) {
            case WM_KEYDOWN:
            case WM_SYSKEYDOWN:
                if (GAME_OVER == m_GameState)
                {
                    // Waiting for any key from the user, and one just arrived.
                    OnStopGame();
                    return TRUE;    // Don't let the dialog have this key
                }
                else 
                {
                    if (m_KbdManager.SetKey(pMsg->wParam) ||
                        (VK_ESCAPE == pMsg->wParam))
                            // Key was successfully mapped to game 
                            // or it was Esc
                        return TRUE;
                    else
                            // We don't know this key => dispatch it to the dialog
                        return FALSE;
                }
                break;
            case WM_KEYUP:
            case WM_SYSKEYUP:
                if (m_KbdManager.UnsetKey(pMsg->wParam) ||
                    (VK_ESCAPE == pMsg->wParam))
                        // Key was successfully mapped to game 
                        // or it was Esc
                    return TRUE;
                else
                        // We don't know this key => dispatch it to the dialog
                    return FALSE;
                break;
        }
    }
    return CDialog::PreTranslateMessage(pMsg);
}


void CTanksDlg::HideGameControl(CWnd &ctrl, BOOL bHide)
{
    ctrl.EnableWindow (bHide ? FALSE : TRUE);
    ctrl.ShowWindow (bHide ? SW_HIDE : SW_SHOW);
}

void CTanksDlg::OnClose() 
{
    // Catch application before exiting:
    if (IsGameOn()) {
        UINT uStrInd = TANKS_APP->m_gCommManager.IsHost() 
            ? IDS_HOST_ON_EXIT 
            : IDS_PLAYER_ON_EXIT;
        if (IDNO == AfxMessageBox(uStrInd, MB_YESNO | MB_ICONQUESTION))
            return;
    }   
    CDialog::OnClose();
}

void CTanksDlg::CreateStatusPanes()
{
    m_AmmoStatus.Create ();
    m_BonusStatus.Create ();

    m_ShieldStatus.SetLevel (0);
    m_AmmoStatus.SetShellsCount (0);
    m_AmmoStatus.SetBulletsCount (0);
    m_AmmoStatus.SetMinesCount (0);
    m_BonusStatus.SetAerialSupport (FALSE);
    m_BonusStatus.SetFastFireRate (FALSE);
}

void CTanksDlg::StartDIBDrawingEngine()
{
    TANKS_APP->m_gDrawDIB.Open ();
}

BOOL CTanksDlg::OnQueryNewPalette() 
{
    return TRUE;
}


void CTanksDlg::OnPaletteChanged(CWnd* pFocusWnd) 
{
    if (pFocusWnd != this)
        OnQueryNewPalette ();
}



void CTanksDlg::CheckDPlayVersion()
{
/*  In this function we should check that we can create a DPlay2 interface.
    If not - we should display a detailed dialog to the user, telling him
    that he cannot connect or host a session and point the user to the URL
    where he can download the latest SP (for NT) or DirectX runtime.
*/
}

void CTanksDlg::OnOK()
{
    // This function is here to override the default CDialog::OnOK which
    // closes the dialog window  - we don't want that to happen
}

void CTanksDlg::OnServerMgmt() 
{
    if (!(TANKS_APP->GetStoredIsHostFlag()))
        return; // Player is not hosting a game
    m_SrvMgmtDlg.Open (this);
}

void
CTanksDlg::OnChatOpen()
{
    if (m_GameState != GAME_ON)   // We have to have a local tank ID to send chat msg:
        return;
    m_ChatDlg.Open(this);
}

void CTanksDlg::OnPlayersGuide() 
{
    CString cstrHTML = TANKS_APP->m_pszHelpFilePath;
    int i=cstrHTML.ReverseFind ('\\');
    cstrHTML = cstrHTML.Left (i);
    cstrHTML += "\\HTML\\players.htm";
    
    HINSTANCE hRes = ShellExecute ( m_hWnd, 
                                    "open", 
                                    cstrHTML,
                                    NULL,
                                    NULL,
                                    SW_MAXIMIZE);
    if (DWORD(hRes) <= 32)
        AfxMessageBox (IDS_CANT_SHOW_IE, MB_OK);
}            

void 
CTanksDlg::OnSoundMute() 
{
    UINT uFlag = GetMenu()->GetMenuState(ID_SOUND_MUTE, MF_BYCOMMAND);
    if (MF_CHECKED == uFlag)
    {   // The user wants to uncheck (return the game sound):
        GetMenu()->CheckMenuItem(ID_SOUND_MUTE, MF_UNCHECKED);
        TANKS_APP->m_gSoundManager.SetMuteMode(FALSE);
        TANKS_APP->SetStoredMuteMode(FALSE);
    } else if (MF_UNCHECKED == uFlag)
    {   // The user wants to check (mute the game sound):
        GetMenu()->CheckMenuItem(ID_SOUND_MUTE, MF_CHECKED);
        TANKS_APP->m_gSoundManager.SetMuteMode(TRUE);
        TANKS_APP->SetStoredMuteMode(TRUE);
    }
}
