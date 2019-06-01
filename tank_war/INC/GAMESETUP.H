/*****************************************************************************
*                                                                             
*   GameSetup.h
*                                                                             
*   Electrical Engineering Faculty - Software Lab                             
*   Spring semester 1998                                                      
*                                                                             
*   Tanks game                                                                
*                                                                             
*   Module description: Interface of the GameSetup dialog.
*                       
*                                                                             
*   Authors: Eran Yariv - 28484475                                           
*            Moshe Zur  - 24070856                                           
*                                                                            
*                                                                            
*   Date: 23/09/98                                                           
*                                                                            
******************************************************************************/
#if !defined(AFX_GAMESETUP_H__3161B0C4_E818_11D1_9738_B48DE1E5CB1C__INCLUDED_)
#define AFX_GAMESETUP_H__3161B0C4_E818_11D1_9738_B48DE1E5CB1C__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// GameSetup.h : header file
//

#include "AnimateTank.h"
#include <GameConsts.h>
#include <AniButton.h>

/////////////////////////////////////////////////////////////////////////////
// CGameSetup dialog

class CGameSetup : public CDialog
{
// Construction
public:
	CGameSetup(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CGameSetup)
	enum { IDD = IDD_GAME_SETUP };
	CSliderCtrl	m_Complexity;
	CAniButton	m_ctrOK;
    CAniButton  m_ctrCancel;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGameSetup)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CGameSetup)
	virtual BOOL OnInitDialog();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void LoadSettings (void);
    UINT            m_uSelectedTankID;
	CAnimateTank    m_AnimTanks[MAX_TANKS];
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GAMESETUP_H__3161B0C4_E818_11D1_9738_B48DE1E5CB1C__INCLUDED_)
