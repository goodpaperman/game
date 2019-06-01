// tankDlg.cpp : implementation file
//

#include "stdafx.h"
#include "tank.h"
#include "tankDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

BOOL CTankDlg::bShowDead = FALSE ; 
BOOL CTankDlg::bShowRoutine = FALSE ; 
BOOL CTankDlg::bAutoAttack = FALSE ; 
BOOL CTankDlg::bMoveAttack = TRUE ; 
BOOL CTankDlg::bShowFireRange = FALSE ; 
BOOL CTankDlg::bShowID = FALSE ; 
BOOL CTankDlg::bShowAttackDes = FALSE ; 
RANK CTankDlg::tankRank = slow ; 
UINT CTankDlg::tankFireRange = 0 ; 

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
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTankDlg dialog

CTankDlg::CTankDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTankDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTankDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	bPause = FALSE ; 
}

void CTankDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTankDlg)
	DDX_Control(pDX, IDC_STATIC1, m_background);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTankDlg, CDialog)
	//{{AFX_MSG_MAP(CTankDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_COMMAND(ID_SHOW_DEAD, OnShowDead)
	ON_COMMAND(ID_SHOW_ROUTINE, OnShowRoutine)
	ON_COMMAND(ID_PAUSE, OnPause)
	ON_COMMAND(ID_RESTART, OnRestart)
	ON_COMMAND(ID_ENEMY_AUTO, OnEnemyAuto)
	ON_COMMAND(ID_MOVE_ATTACK, OnMoveAttack)
	ON_COMMAND(ID_SHOW_FIRERANGE, OnShowFirerange)
	ON_COMMAND(ID_SHOW_ID, OnShowId)
	ON_COMMAND(ID_SHOW_ATTACK_DES, OnShowAttackDes)
	ON_COMMAND(ID_ADD_TANK, OnAddTank)
	ON_COMMAND(ID_TANK_RANK_SLOW, OnTankRankSlow)
	ON_COMMAND(ID_TANK_RANK_QUICK, OnTankRankQuick)
	ON_COMMAND(ID_TANK_RANK_NORMAL, OnTankRankNormal)
	ON_COMMAND(ID_TANK_RANK_FAST, OnTankRankFast)
	ON_COMMAND(ID_TANK_FIRERANGE_10, OnTankFirerange10)
	ON_COMMAND(ID_TANK_FIRERANGE_20, OnTankFirerange20)
	ON_COMMAND(ID_TANK_FIRERANGE_30, OnTankFirerange30)
	ON_COMMAND(ID_TANK_FIRERANGE_40, OnTankFirerange40)
	ON_COMMAND(ID_TANK_FIRERANGE_00, OnTankFirerange00)
	ON_UPDATE_COMMAND_UI(ID_ENEMY_AUTO, OnUpdateEnemyAuto)
	ON_UPDATE_COMMAND_UI(ID_MOVE_ATTACK, OnUpdateMoveAttack)
	ON_UPDATE_COMMAND_UI(ID_PAUSE, OnUpdatePause)
	ON_UPDATE_COMMAND_UI(ID_SHOW_ATTACK_DES, OnUpdateShowAttackDes)
	ON_UPDATE_COMMAND_UI(ID_SHOW_DEAD, OnUpdateShowDead)
	ON_UPDATE_COMMAND_UI(ID_SHOW_FIRERANGE, OnUpdateShowFirerange)
	ON_UPDATE_COMMAND_UI(ID_SHOW_ID, OnUpdateShowId)
	ON_UPDATE_COMMAND_UI(ID_SHOW_ROUTINE, OnUpdateShowRoutine)
	ON_UPDATE_COMMAND_UI(ID_TANK_FIRERANGE_00, OnUpdateTankFirerange00)
	ON_UPDATE_COMMAND_UI(ID_TANK_FIRERANGE_10, OnUpdateTankFirerange10)
	ON_UPDATE_COMMAND_UI(ID_TANK_FIRERANGE_20, OnUpdateTankFirerange20)
	ON_UPDATE_COMMAND_UI(ID_TANK_FIRERANGE_30, OnUpdateTankFirerange30)
	ON_UPDATE_COMMAND_UI(ID_TANK_FIRERANGE_40, OnUpdateTankFirerange40)
	ON_UPDATE_COMMAND_UI(ID_TANK_RANK_FAST, OnUpdateTankRankFast)
	ON_UPDATE_COMMAND_UI(ID_TANK_RANK_NORMAL, OnUpdateTankRankNormal)
	ON_UPDATE_COMMAND_UI(ID_TANK_RANK_QUICK, OnUpdateTankRankQuick)
	ON_UPDATE_COMMAND_UI(ID_TANK_RANK_SLOW, OnUpdateTankRankSlow)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTankDlg message handlers

BOOL CTankDlg::OnInitDialog()
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
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	m_background.InitBackground() ; 
	SetFocus() ; 
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CTankDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CTankDlg::OnPaint() 
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
HCURSOR CTankDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CTankDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default 

	SetCapture() ; 
//	TRACE("\n***%d, %d", point.x, point.y) ; 
	CPoint local = point ; 
	ClientToScreen(&local) ; 
	m_background.ScreenToClient(&local) ; 
//	TRACE("\n***%d, %d", local.x, local.y) ; 

	CSize size = m_background.GetSize() ; 
	if( local.x>=0 && local.x<=size.cx   
		&& local.y>=0 && local.y<=size.cy )
	{
		m_background.OnLButtonDown(nFlags, local) ; 
	}
	CDialog::OnLButtonDown(nFlags, point);
}

void CTankDlg::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
//	TRACE("\n***%d, %d", point.x, point.y) ; 
	CPoint local = point ; 
	ClientToScreen(&local) ; 
	m_background.ScreenToClient(&local) ; 
//	TRACE("\n***%d, %d", local.x, local.y) ; 

	CSize size = m_background.GetSize() ; 
	if( local.x>=0 && local.x<=size.cx   
		&& local.y>=0 && local.y<=size.cy )
	{
		m_background.OnMouseMove(nFlags, local) ; 
	}
	
	CDialog::OnMouseMove(nFlags, point);
}

void CTankDlg::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
//	TRACE("\n***%d, %d", point.x, point.y) ; 
	CPoint local = point ; 
	ClientToScreen(&local) ; 
	m_background.ScreenToClient(&local) ; 
//	TRACE("\n***%d, %d", local.x, local.y) ; 

	CSize size = m_background.GetSize() ; 
	if( local.x>=0 && local.x<=size.cx   
		&& local.y>=0 && local.y<=size.cy )
	{
		m_background.OnLButtonUp(nFlags, local) ; 
	}
	ReleaseCapture() ; 
	
	CDialog::OnLButtonUp(nFlags, point);
}

void CTankDlg::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default	
//	TRACE("\n***%d, %d", point.x, point.y) ; 
	CPoint local = point ; 
	ClientToScreen(&local) ; 
	m_background.ScreenToClient(&local) ; 
//	TRACE("\n***%d, %d", local.x, local.y) ; 

	CSize size = m_background.GetSize() ; 
	if( local.x>=0 && local.x<=size.cx   
		&& local.y>=0 && local.y<=size.cy )
	{
		m_background.OnRButtonDown(nFlags, local) ; 
	}
	CDialog::OnRButtonDown(nFlags, point);
}

void CTankDlg::OnShowDead() 
{
	// TODO: Add your command handler code here
	bShowDead = !bShowDead ; 
}

void CTankDlg::OnUpdateShowDead(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(bShowDead) ; 
}

void CTankDlg::OnShowRoutine() 
{
	// TODO: Add your command handler code here
	bShowRoutine = !bShowRoutine ; 
}

void CTankDlg::OnUpdateShowRoutine(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(bShowRoutine) ; 
}

void CTankDlg::OnPause() 
{
	// TODO: Add your command handler code here
	bPause = !bPause ; 
	TRACE("pause : %d \n", bPause) ; 
	m_background.Pause(bPause) ; 
}

void CTankDlg::OnUpdatePause(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(bPause); 
}

void CTankDlg::OnRestart() 
{
	// TODO: Add your command handler code here
	m_background.Restart() ; 
}

void CTankDlg::OnAddTank() 
{
	// TODO: Add your command handler code here
	m_background.AddTank() ; 
}

void CTankDlg::OnEnemyAuto() 
{
	// TODO: Add your command handler code here
	bAutoAttack = !bAutoAttack ; 
}

void CTankDlg::OnUpdateEnemyAuto(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(bAutoAttack); 
}

void CTankDlg::OnMoveAttack() 
{
	// TODO: Add your command handler code here
	bMoveAttack = !bMoveAttack ; 
}

void CTankDlg::OnUpdateMoveAttack(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(bMoveAttack); 
}

void CTankDlg::OnShowFirerange() 
{
	// TODO: Add your command handler code here
	bShowFireRange = !bShowFireRange ; 
}

void CTankDlg::OnUpdateShowFirerange(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(bShowFireRange); 
}

void CTankDlg::OnShowId() 
{
	// TODO: Add your command handler code here
	bShowID = !bShowID ; 
}

void CTankDlg::OnUpdateShowId(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(bShowID); 
}


void CTankDlg::OnShowAttackDes() 
{
	// TODO: Add your command handler code here
	bShowAttackDes = !bShowAttackDes ; 
}

void CTankDlg::OnUpdateShowAttackDes(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(bShowAttackDes); 
}

void CTankDlg::OnTankRankSlow() 
{
	// TODO: Add your command handler code here
	tankRank = slow ; 
}

void CTankDlg::OnUpdateTankRankSlow(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetRadio(tankRank == slow); 
}

void CTankDlg::OnTankRankNormal() 
{
	// TODO: Add your command handler code here
	tankRank = normal ; 	
}

void CTankDlg::OnUpdateTankRankNormal(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetRadio(tankRank == normal); 
}

void CTankDlg::OnTankRankQuick() 
{
	// TODO: Add your command handler code here
	tankRank = quick ; 
}

void CTankDlg::OnUpdateTankRankQuick(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetRadio(tankRank == quick); 
}


void CTankDlg::OnTankRankFast() 
{
	// TODO: Add your command handler code here
	tankRank = fast ; 
}

void CTankDlg::OnUpdateTankRankFast(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetRadio(tankRank == fast); 
}

void CTankDlg::OnTankFirerange00() 
{
	// TODO: Add your command handler code here
	tankFireRange = 0 ; 
}

void CTankDlg::OnUpdateTankFirerange00(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetRadio(tankFireRange == 0); 
}

void CTankDlg::OnTankFirerange10() 
{
	// TODO: Add your command handler code here
	tankFireRange = 10 ; 
}

void CTankDlg::OnUpdateTankFirerange10(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetRadio(tankFireRange == 10); 
}

void CTankDlg::OnTankFirerange20() 
{
	// TODO: Add your command handler code here
	tankFireRange = 20 ; 
}

void CTankDlg::OnUpdateTankFirerange20(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetRadio(tankFireRange == 20); 
}

void CTankDlg::OnTankFirerange30() 
{
	// TODO: Add your command handler code here
	tankFireRange = 30 ; 
}

void CTankDlg::OnUpdateTankFirerange30(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetRadio(tankFireRange == 30); 
}

void CTankDlg::OnTankFirerange40() 
{
	// TODO: Add your command handler code here
	tankFireRange = 40 ; 
}

void CTankDlg::OnUpdateTankFirerange40(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetRadio(tankFireRange == 40); 
}
