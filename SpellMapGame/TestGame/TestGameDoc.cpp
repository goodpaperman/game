// TestGameDoc.cpp : implementation of the CTestGameDoc class
//

#include "stdafx.h"
#include "TestGame.h"

#include "TestGameDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTestGameDoc

IMPLEMENT_DYNCREATE(CTestGameDoc, CDocument)

BEGIN_MESSAGE_MAP(CTestGameDoc, CDocument)
	//{{AFX_MSG_MAP(CTestGameDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTestGameDoc construction/destruction

CTestGameDoc::CTestGameDoc()
{
	// TODO: add one-time construction code here

}

CTestGameDoc::~CTestGameDoc()
{
}

BOOL CTestGameDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CTestGameDoc serialization

void CTestGameDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CTestGameDoc diagnostics

#ifdef _DEBUG
void CTestGameDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CTestGameDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTestGameDoc commands
