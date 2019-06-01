/*****************************************************************************
*                                                                             
*   ChatDlg.h
*                                                                             
*   Electrical Engineering Faculty - Software Lab                             
*   Spring semester 1998                                                      
*                                                                             
*   Tanks game                                                                
*                                                                             
*   Module description: Interface of the chat dialog.
*                       
*                                                                             
*   Authors: Eran Yariv - 28484475                                           
*            Moshe Zur  - 24070856                                           
*                                                                            
*                                                                            
*   Date: 23/09/98                                                           
*                                                                            
******************************************************************************/
#if !defined(AFX_CHATDLG_H__A61D98E1_4FB8_11D2_AE86_444553540000__INCLUDED_)
#define AFX_CHATDLG_H__A61D98E1_4FB8_11D2_AE86_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// ChatDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CChatDlg dialog

class CChatDlg : public CDialog
{
// Construction
public:
    CChatDlg(CWnd* pParent = NULL);   // standard constructor

    static void Open (CWnd *pParent);
    static void Close();
    void OnCancel ();
    void AddMessage(DPID idFrom, DWORD dwTankID, LPCSTR szMsg);
    void PostNcDestroy( );

// Dialog Data
    //{{AFX_DATA(CChatDlg)
    enum { IDD = IDD_CHAT_DIALOG };
    CListBox    m_List;
    CEdit   m_Edit;
    //}}AFX_DATA


// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CChatDlg)
    protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(CChatDlg)
    virtual BOOL OnInitDialog();
    afx_msg void OnClose();
    //afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
private:
    // methods:
    static void Clean ();
    void OnOK();
    // members:
    CCommManager   &m_gCommManager;
    static CChatDlg *m_pDlg;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHATDLG_H__A61D98E1_4FB8_11D2_AE86_444553540000__INCLUDED_)
