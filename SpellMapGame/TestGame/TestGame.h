// TestGame.h : main header file for the TESTGAME application
//

#if !defined(AFX_TESTGAME_H__928F6F95_5DF8_40A8_934F_A1F3350C77A5__INCLUDED_)
#define AFX_TESTGAME_H__928F6F95_5DF8_40A8_934F_A1F3350C77A5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols
#include "..\include\Game.h"

/////////////////////////////////////////////////////////////////////////////
// CTestGameApp:
// See TestGame.cpp for the implementation of this class
//

#define		GRID_SIZE	50

class CTestGameApp : public CWinApp
{
public:
	CTestGameApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTestGameApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

	CGame m_game ; 

// Implementation
	//{{AFX_MSG(CTestGameApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TESTGAME_H__928F6F95_5DF8_40A8_934F_A1F3350C77A5__INCLUDED_)
