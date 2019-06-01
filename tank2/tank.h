// tank.h : main header file for the TANK application
//

#if !defined(AFX_TANK_H__3DA02C44_1039_4578_A33A_EDC88AAABA10__INCLUDED_)
#define AFX_TANK_H__3DA02C44_1039_4578_A33A_EDC88AAABA10__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CTankApp:
// See tank.cpp for the implementation of this class
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
	virtual BOOL OnIdle(LONG lCount);
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CTankApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TANK_H__3DA02C44_1039_4578_A33A_EDC88AAABA10__INCLUDED_)
