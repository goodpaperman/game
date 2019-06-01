// SpellMapShell.h : header file
//

#if !defined(AFX_SPELLMAPSHELL_H__9C2F0948_8164_48B2_B854_F8FE234D8E05__INCLUDED_)
#define AFX_SPELLMAPSHELL_H__9C2F0948_8164_48B2_B854_F8FE234D8E05__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CSpellMapShell dialog


class CSpellMapShell : public CDialog
{
// Construction
public:
	CSpellMapShell(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CSpellMapShell)
	enum { IDD = IDD_SPELLMAP_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSpellMapShell)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CSpellMapShell)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SPELLMAPSHELL_H__9C2F0948_8164_48B2_B854_F8FE234D8E05__INCLUDED_)
