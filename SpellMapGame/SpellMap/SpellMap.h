// SpellMap.h : main header file for the SPELLMAP application
//

#if !defined(AFX_SPELLMAP_H__A0CB2096_4630_499E_A407_2D348C4FC0E8__INCLUDED_)
#define AFX_SPELLMAP_H__A0CB2096_4630_499E_A407_2D348C4FC0E8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CSpellMapApp:
// See SpellMap.cpp for the implementation of this class
//

class CSpellMapApp : public CWinApp
{
public:
	CSpellMapApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSpellMapApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CSpellMapApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SPELLMAP_H__A0CB2096_4630_499E_A407_2D348C4FC0E8__INCLUDED_)
