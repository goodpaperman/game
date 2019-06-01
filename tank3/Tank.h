// Tank.h : main header file for the TANK application
//

#if !defined(AFX_TANK_H__B2E96A5F_E7EF_479A_B09D_729B2F3FA2DD__INCLUDED_)
#define AFX_TANK_H__B2E96A5F_E7EF_479A_B09D_729B2F3FA2DD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CTankApp:
// See Tank.cpp for the implementation of this class
//

class CTankApp : public CWinApp
{
public:
	CTankApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTankApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CTankApp)
	afx_msg void OnAppAbout();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TANK_H__B2E96A5F_E7EF_479A_B09D_729B2F3FA2DD__INCLUDED_)
