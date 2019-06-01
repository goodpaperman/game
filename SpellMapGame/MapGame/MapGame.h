// MapGame.h : main header file for the MAPGAME DLL
//

#if !defined(AFX_MAPGAME_H__59F03716_810F_4FB2_BA86_176BBFFEC865__INCLUDED_)
#define AFX_MAPGAME_H__59F03716_810F_4FB2_BA86_176BBFFEC865__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CMapGameApp
// See MapGame.cpp for the implementation of this class
//

class CMapGameApp : public CWinApp
{
public:
	CMapGameApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMapGameApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CMapGameApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAPGAME_H__59F03716_810F_4FB2_BA86_176BBFFEC865__INCLUDED_)
