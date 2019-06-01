// TankView.h : interface of the CTankView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_TANKVIEW_H__FCA08D23_2699_4A34_822A_C14492753A3B__INCLUDED_)
#define AFX_TANKVIEW_H__FCA08D23_2699_4A34_822A_C14492753A3B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Background.h"
#include "TankDoc.h"

typedef enum {slow = 0, normal, quick, fast}RANK ; 
class CTankView : public CView
{
protected: // create from serialization only
	CTankView();
	DECLARE_DYNCREATE(CTankView)

// Attributes
public:
	CTankDoc* GetDocument();
	CBackground	m_background;

	static BOOL bShowDead ; 
	static BOOL bShowRoutine ; 
	static BOOL bAutoAttack ; 
	static BOOL bMoveAttack ;
	static BOOL bShowFireRange ; 
	static BOOL bShowID ; 
	static BOOL bShowAttackDes ; 
	static RANK tankRank ; 
	static UINT tankFireRange ; 
	BOOL bPause ; 


// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTankView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void OnInitialUpdate(); // called first time after construct
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTankView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CTankView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct); 
	//afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    //afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    //afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    //afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnShowDead();
	afx_msg void OnShowRoutine();
	afx_msg void OnPause();
	afx_msg void OnRestart();
	afx_msg void OnEnemyAuto();
	afx_msg void OnMoveAttack();
	afx_msg void OnShowFirerange();
	afx_msg void OnShowId();
	afx_msg void OnShowAttackDes();
	afx_msg void OnAddTank();
	afx_msg void OnTankRankSlow();
	afx_msg void OnTankRankQuick();
	afx_msg void OnTankRankNormal();
	afx_msg void OnTankRankFast();
	afx_msg void OnTankFirerange10();
	afx_msg void OnTankFirerange20();
	afx_msg void OnTankFirerange30();
	afx_msg void OnTankFirerange40();
	afx_msg void OnTankFirerange00();
	afx_msg void OnUpdateEnemyAuto(CCmdUI* pCmdUI);
	afx_msg void OnUpdateMoveAttack(CCmdUI* pCmdUI);
	afx_msg void OnUpdatePause(CCmdUI* pCmdUI);
	afx_msg void OnUpdateRestart(CCmdUI* pCmdUI);
	afx_msg void OnUpdateShowAttackDes(CCmdUI* pCmdUI);
	afx_msg void OnUpdateShowDead(CCmdUI* pCmdUI);
	afx_msg void OnUpdateShowFirerange(CCmdUI* pCmdUI);
	afx_msg void OnUpdateShowId(CCmdUI* pCmdUI);
	afx_msg void OnUpdateShowRoutine(CCmdUI* pCmdUI);
	afx_msg void OnUpdateTankFirerange00(CCmdUI* pCmdUI);
	afx_msg void OnUpdateTankFirerange10(CCmdUI* pCmdUI);
	afx_msg void OnUpdateTankFirerange20(CCmdUI* pCmdUI);
	afx_msg void OnUpdateTankFirerange30(CCmdUI* pCmdUI);
	afx_msg void OnUpdateTankFirerange40(CCmdUI* pCmdUI);
	afx_msg void OnUpdateTankRankFast(CCmdUI* pCmdUI);
	afx_msg void OnUpdateTankRankNormal(CCmdUI* pCmdUI);
	afx_msg void OnUpdateTankRankQuick(CCmdUI* pCmdUI);
	afx_msg void OnUpdateTankRankSlow(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in TankView.cpp
inline CTankDoc* CTankView::GetDocument()
   { return (CTankDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TANKVIEW_H__FCA08D23_2699_4A34_822A_C14492753A3B__INCLUDED_)
