#if !defined(AFX_ENUMSESSION_H__5BE28FA1_6DD2_11D2_AA8A_444553540000__INCLUDED_)
#define AFX_ENUMSESSION_H__5BE28FA1_6DD2_11D2_AA8A_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// EnumSession.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CEnumSession dialog

class CEnumSession : public CDialog
{
// Construction
public:
    CEnumSession(LPDIRECTPLAY2 pIDP, GUID *pSessionGuid /* OUT */);

// Dialog Data
    //{{AFX_DATA(CEnumSession)
    enum { IDD = IDD_ENUMSESSIONS_DIALOG };
    CStatic m_Static;
    CListBox    m_ListBox;
    //}}AFX_DATA


// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CEnumSession)
    protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(CEnumSession)
    afx_msg void OnRefresh();
    virtual void OnCancel();
    virtual void OnOK();
    virtual BOOL OnInitDialog();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
private:
    // Methods.
    void EnumSession();
    void EmptyListBox();
    // Members.
    LPDIRECTPLAY2   m_pIDP;
    GUID   *m_pSessionGuid;
    DWORD   m_dwTimeOut;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ENUMSESSION_H__5BE28FA1_6DD2_11D2_AA8A_444553540000__INCLUDED_)
