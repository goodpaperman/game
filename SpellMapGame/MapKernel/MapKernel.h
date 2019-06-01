// MapKernel.h : main header file for the MAPKERNEL DLL
//

#if !defined(AFX_MAPKERNEL_H__A29BBD3B_9077_4ED0_9406_8923E1444900__INCLUDED_)
#define AFX_MAPKERNEL_H__A29BBD3B_9077_4ED0_9406_8923E1444900__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CMapKernelApp
// See MapKernel.cpp for the implementation of this class
//

class CMapKernelApp : public CWinApp
{
public:
	CMapKernelApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMapKernelApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CMapKernelApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAPKERNEL_H__A29BBD3B_9077_4ED0_9406_8923E1444900__INCLUDED_)
