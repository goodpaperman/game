// TestGameDoc.h : interface of the CTestGameDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_TESTGAMEDOC_H__8428B63B_62AC_4273_A99F_12757E3958BE__INCLUDED_)
#define AFX_TESTGAMEDOC_H__8428B63B_62AC_4273_A99F_12757E3958BE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CTestGameDoc : public CDocument
{
protected: // create from serialization only
	CTestGameDoc();
	DECLARE_DYNCREATE(CTestGameDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTestGameDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTestGameDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CTestGameDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TESTGAMEDOC_H__8428B63B_62AC_4273_A99F_12757E3958BE__INCLUDED_)
