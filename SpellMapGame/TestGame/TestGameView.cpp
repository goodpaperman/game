// TestGameView.cpp : implementation of the CTestGameView class
//

#include "stdafx.h"
#include "TestGame.h"

#include "TestGameDoc.h"
#include "TestGameView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTestGameView

IMPLEMENT_DYNCREATE(CTestGameView, CView)

BEGIN_MESSAGE_MAP(CTestGameView, CView)
	//{{AFX_MSG_MAP(CTestGameView)
	ON_WM_LBUTTONDOWN()
	ON_WM_KEYDOWN()
	ON_COMMAND(ID_EDIT_UNDO, OnEditUndo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO, OnUpdateEditUndo)
	ON_COMMAND(ID_EDIT_REDO, OnEditRedo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_REDO, OnUpdateEditRedo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTestGameView construction/destruction

CTestGameView::CTestGameView()
{
	// TODO: add construction code here

}

CTestGameView::~CTestGameView()
{
}

BOOL CTestGameView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CTestGameView drawing

void CTestGameView::OnDraw(CDC* pDC)
{
	CTestGameDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here

	CGame *pGame = &(((CTestGameApp *)AfxGetApp())->m_game) ;
	int row = pGame->GetRow() ;
	int col = pGame->GetCol() ;
	pGame->PrintMap() ;

	for(int i=0; i<row; i++)
	{
		for(int j=0; j<col; j++)
		{
			RECT rect = pGame->GetGrid(i, j) ;
			pDC->Rectangle(&rect) ;
			int value = pGame->GetGridValue(i, j) ;
			CString str ; 
			if(value == row * col -1 )
				str = "" ;
			else 
				str.Format("%d", value) ;
			pDC->DrawText(str, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE) ;
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// CTestGameView diagnostics

#ifdef _DEBUG
void CTestGameView::AssertValid() const
{
	CView::AssertValid();
}

void CTestGameView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CTestGameDoc* CTestGameView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTestGameDoc)));
	return (CTestGameDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTestGameView message handlers

void CTestGameView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	POINT pt ; 
	pt.x = point.x ;
	pt.y = point.y ;
	TRACE("Click at: (%d, %d)\n", pt.x, pt.y) ;
	CGame* pGame = &(((CTestGameApp *)AfxGetApp())->m_game) ;
	pGame->MoveTo(pt) ;
	//pGame->PrintMap() ;

	CView::OnLButtonDown(nFlags, point);
}

void CTestGameView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	CGame* pGame = &(((CTestGameApp *)AfxGetApp())->m_game) ;
	Direction direct = down ;
	BOOL bOther = FALSE ;
	switch(nChar)
	{
	case 37:
		TRACE("Move to left\n") ;
		direct = left ;
		break ;
	case 38:
		TRACE("Move to up\n") ;
		direct = up ;
		break ;
	case 39:
		TRACE("Move to right\n") ;
		direct = right ;
		break ;
	case 40:
		TRACE("Move to down\n") ;
		direct = down ;
		break ;
	default:
		bOther = TRUE ;
		break ;
	}

	if(!bOther)
	{
		pGame->MoveTo(direct) ;
		//pGame->PrintMap() ;
	}
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CTestGameView::OnEditUndo() 
{
	// TODO: Add your command handler code here
	CGame* pGame = &(((CTestGameApp *)AfxGetApp())->m_game) ;
	pGame->Undo() ;	
}

void CTestGameView::OnUpdateEditUndo(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CGame* pGame = &(((CTestGameApp *)AfxGetApp())->m_game) ;
	pCmdUI->Enable(pGame->CanUndo()); 
}

void CTestGameView::OnEditRedo() 
{
	// TODO: Add your command handler code here
	CGame* pGame = &(((CTestGameApp *)AfxGetApp())->m_game) ;
	pGame->Redo() ;
}

void CTestGameView::OnUpdateEditRedo(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CGame* pGame = &(((CTestGameApp *)AfxGetApp())->m_game) ;
	pCmdUI->Enable(pGame->CanRedo()); 
	
}
