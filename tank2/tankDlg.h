// tankDlg.h : header file
//

#if !defined(AFX_TANKDLG_H__49CDA421_4BE5_4D2E_B3E9_BBDB0ABCB240__INCLUDED_)
#define AFX_TANKDLG_H__49CDA421_4BE5_4D2E_B3E9_BBDB0ABCB240__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CTankDlg dialog

#include "Background.h"

typedef enum {slow = 0, normal, quick, fast}RANK ; 
class CTankDlg : public CDialog
{
// Construction
public:
	CTankDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CTankDlg)
	enum { IDD = IDD_TANK_DIALOG };
	CBackground	m_background;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTankDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

public :		
	//control
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

// Implementation
protected:
	HICON m_hIcon;
	CToolTipCtrl tip ; 

	// Generated message map functions
	//{{AFX_MSG(CTankDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
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

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TANKDLG_H__49CDA421_4BE5_4D2E_B3E9_BBDB0ABCB240__INCLUDED_)
