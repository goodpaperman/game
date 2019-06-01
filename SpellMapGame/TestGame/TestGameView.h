// TestGameView.h : interface of the CTestGameView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_TESTGAMEVIEW_H__D2A2308F_FDCA_4136_9C5E_DA5A37004F57__INCLUDED_)
#define AFX_TESTGAMEVIEW_H__D2A2308F_FDCA_4136_9C5E_DA5A37004F57__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CTestGameView : public CView
{
protected: // create from serialization only
	CTestGameView();
	DECLARE_DYNCREATE(CTestGameView)

// Attributes
public:
	CTestGameDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTestGameView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTestGameView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CTestGameView)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnEditUndo();
	afx_msg void OnUpdateEditUndo(CCmdUI* pCmdUI);
	afx_msg void OnEditRedo();
	afx_msg void OnUpdateEditRedo(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in TestGameView.cpp
inline CTestGameDoc* CTestGameView::GetDocument()
   { return (CTestGameDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TESTGAMEVIEW_H__D2A2308F_FDCA_4136_9C5E_DA5A37004F57__INCLUDED_)
