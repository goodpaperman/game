// TankView.cpp : implementation of the CTankView class
//

#include "stdafx.h"
#include "Tank.h"

#include "TankDoc.h"
#include "TankView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTankView

BOOL CTankView::bShowDead = FALSE ; 
BOOL CTankView::bShowRoutine = FALSE ; 
BOOL CTankView::bAutoAttack = FALSE ; 
BOOL CTankView::bMoveAttack = TRUE ; 
BOOL CTankView::bShowFireRange = FALSE ; 
BOOL CTankView::bShowID = FALSE ; 
BOOL CTankView::bShowAttackDes = FALSE ; 
RANK CTankView::tankRank = fast ; 
UINT CTankView::tankFireRange = 40 ; 

IMPLEMENT_DYNCREATE(CTankView, CView)

BEGIN_MESSAGE_MAP(CTankView, CView)
	//{{AFX_MSG_MAP(CTankView)
	ON_WM_CREATE()
    //ON_WM_LBUTTONDOWN()
    //ON_WM_MOUSEMOVE()
    //ON_WM_LBUTTONUP()
    //ON_WM_RBUTTONDOWN()
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
// CTankView construction/destruction

CTankView::CTankView()
{
	bPause = FALSE ; 
}

CTankView::~CTankView()
{
}

BOOL CTankView::PreCreateWindow(CREATESTRUCT& cs)
{
	return CView::PreCreateWindow(cs);
}

int CTankView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if(CView::OnCreate(lpCreateStruct) == -1)
		return -1; 

	CRect rect; 
	rect.SetRectEmpty(); 
    if(!m_background.Create("", WS_BORDER | WS_VISIBLE | SS_NOTIFY, rect, this, IDC_BKGND_STATIC))
		TRACE0("Create Background failed."); 
	
	return 0; 
}

/////////////////////////////////////////////////////////////////////////////
// CTankView drawing

void CTankView::OnDraw(CDC* pDC)
{
	CTankDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
}

void CTankView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

    //CSize sizeTotal;
    //sizeTotal.cx = sizeTotal.cy = 1024;
    //SetScrollSizes(MM_TEXT, sizeTotal);
	
	CRect rect; 
	GetClientRect(&rect); 
	m_background.SetWindowPos(NULL, 0, 0, rect.Width(), rect.Height(), SWP_NOZORDER | SWP_NOACTIVATE); 
	m_background.InitBackground(); 
}

/////////////////////////////////////////////////////////////////////////////
// CTankView diagnostics

#ifdef _DEBUG
void CTankView::AssertValid() const
{
	CView::AssertValid();
}

void CTankView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CTankDoc* CTankView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTankDoc)));
	return (CTankDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTankView message handlers


//void CTankView::OnLButtonDown(UINT nFlags, CPoint point) 
//{
//    // TODO: Add your message handler code here and/or call default 

//    SetCapture() ; 
////    TRACE("\n***%d, %d", point.x, point.y) ; 
//    CPoint local = point ; 
//    ClientToScreen(&local) ; 
//    m_background.ScreenToClient(&local) ; 
////    TRACE("\n***%d, %d", local.x, local.y) ; 

//    CSize size = m_background.GetSize() ; 
//    if( local.x>=0 && local.x<=size.cx   
//        && local.y>=0 && local.y<=size.cy )
//    {
//        m_background.OnLButtonDown(nFlags, local) ; 
//    }
//    CView::OnLButtonDown(nFlags, point);
//}

//void CTankView::OnMouseMove(UINT nFlags, CPoint point) 
//{
//    // TODO: Add your message handler code here and/or call default
//    if(GetCapture() == this)
//    {
//        //TRACE("\n***%d, %d", point.x, point.y) ; 
//        CPoint local = point ; 
//        ClientToScreen(&local) ; 
//        m_background.ScreenToClient(&local) ; 
//        //TRACE("\n***%d, %d", local.x, local.y) ; 
    
//        CSize size = m_background.GetSize() ; 
//        if( local.x>=0 && local.x<=size.cx   
//            && local.y>=0 && local.y<=size.cy )
//        {
//            m_background.OnMouseMove(nFlags, local) ; 
//        }
//    }
//    CView::OnMouseMove(nFlags, point);
//}

//void CTankView::OnLButtonUp(UINT nFlags, CPoint point) 
//{
//    // TODO: Add your message handler code here and/or call default
////    TRACE("\n***%d, %d", point.x, point.y) ; 
//    CPoint local = point ; 
//    ClientToScreen(&local) ; 
//    m_background.ScreenToClient(&local) ; 
////    TRACE("\n***%d, %d", local.x, local.y) ; 

//    CSize size = m_background.GetSize() ; 
//    if( local.x>=0 && local.x<=size.cx   
//        && local.y>=0 && local.y<=size.cy )
//    {
//        m_background.OnLButtonUp(nFlags, local) ; 
//    }
//    ReleaseCapture() ; 
    
//    CView::OnLButtonUp(nFlags, point);
//}

//void CTankView::OnRButtonDown(UINT nFlags, CPoint point) 
//{
//    // TODO: Add your message handler code here and/or call default    
////    TRACE("\n***%d, %d", point.x, point.y) ; 
//    CPoint local = point ; 
//    ClientToScreen(&local) ; 
//    m_background.ScreenToClient(&local) ; 
////    TRACE("\n***%d, %d", local.x, local.y) ; 

//    CSize size = m_background.GetSize() ; 
//    if( local.x>=0 && local.x<=size.cx   
//        && local.y>=0 && local.y<=size.cy )
//    {
//        m_background.OnRButtonDown(nFlags, local) ; 
//    }
//    CView::OnRButtonDown(nFlags, point);
//}

void CTankView::OnShowDead() 
{
	// TODO: Add your command handler code here
	bShowDead = !bShowDead ; 
}

void CTankView::OnUpdateShowDead(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(bShowDead) ; 
}

void CTankView::OnShowRoutine() 
{
	// TODO: Add your command handler code here
	bShowRoutine = !bShowRoutine ; 
}

void CTankView::OnUpdateShowRoutine(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(bShowRoutine) ; 
}

void CTankView::OnPause() 
{
	// TODO: Add your command handler code here
	bPause = !bPause ; 
	TRACE("pause : %d \n", bPause) ; 
	m_background.Pause(bPause) ; 
}

void CTankView::OnUpdatePause(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(bPause); 
}

void CTankView::OnRestart() 
{
	// TODO: Add your command handler code here
	m_background.Restart() ; 
}

void CTankView::OnAddTank() 
{
	// TODO: Add your command handler code here
	m_background.AddTank() ; 
}

void CTankView::OnEnemyAuto() 
{
	// TODO: Add your command handler code here
	bAutoAttack = !bAutoAttack ; 
}

void CTankView::OnUpdateEnemyAuto(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(bAutoAttack); 
}

void CTankView::OnMoveAttack() 
{
	// TODO: Add your command handler code here
	bMoveAttack = !bMoveAttack ; 
}

void CTankView::OnUpdateMoveAttack(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(bMoveAttack); 
}

void CTankView::OnShowFirerange() 
{
	// TODO: Add your command handler code here
	bShowFireRange = !bShowFireRange ; 
}

void CTankView::OnUpdateShowFirerange(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(bShowFireRange); 
}

void CTankView::OnShowId() 
{
	// TODO: Add your command handler code here
	bShowID = !bShowID ; 
}

void CTankView::OnUpdateShowId(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(bShowID); 
}


void CTankView::OnShowAttackDes() 
{
	// TODO: Add your command handler code here
	bShowAttackDes = !bShowAttackDes ; 
}

void CTankView::OnUpdateShowAttackDes(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(bShowAttackDes); 
}

void CTankView::OnTankRankSlow() 
{
	// TODO: Add your command handler code here
	tankRank = slow ; 
}

void CTankView::OnUpdateTankRankSlow(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetRadio(tankRank == slow); 
}

void CTankView::OnTankRankNormal() 
{
	// TODO: Add your command handler code here
	tankRank = normal ; 	
}

void CTankView::OnUpdateTankRankNormal(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetRadio(tankRank == normal); 
}

void CTankView::OnTankRankQuick() 
{
	// TODO: Add your command handler code here
	tankRank = quick ; 
}

void CTankView::OnUpdateTankRankQuick(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetRadio(tankRank == quick); 
}


void CTankView::OnTankRankFast() 
{
	// TODO: Add your command handler code here
	tankRank = fast ; 
}

void CTankView::OnUpdateTankRankFast(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetRadio(tankRank == fast); 
}

void CTankView::OnTankFirerange00() 
{
	// TODO: Add your command handler code here
	tankFireRange = 0 ; 
}

void CTankView::OnUpdateTankFirerange00(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetRadio(tankFireRange == 0); 
}

void CTankView::OnTankFirerange10() 
{
	// TODO: Add your command handler code here
	tankFireRange = 10 ; 
}

void CTankView::OnUpdateTankFirerange10(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetRadio(tankFireRange == 10); 
}

void CTankView::OnTankFirerange20() 
{
	// TODO: Add your command handler code here
	tankFireRange = 20 ; 
}

void CTankView::OnUpdateTankFirerange20(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetRadio(tankFireRange == 20); 
}

void CTankView::OnTankFirerange30() 
{
	// TODO: Add your command handler code here
	tankFireRange = 30 ; 
}

void CTankView::OnUpdateTankFirerange30(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetRadio(tankFireRange == 30); 
}

void CTankView::OnTankFirerange40() 
{
	// TODO: Add your command handler code here
	tankFireRange = 40 ; 
}

void CTankView::OnUpdateTankFirerange40(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetRadio(tankFireRange == 40); 
}