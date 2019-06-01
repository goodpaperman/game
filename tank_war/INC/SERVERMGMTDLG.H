/*****************************************************************************
*                                                                             
*   ServerMgmtDlg.h
*                                                                             
*   Electrical Engineering Faculty - Software Lab                             
*   Spring semester 1998                                                      
*                                                                             
*   Tanks game                                                                
*                                                                             
*   Module description: Implements the server management dialog.
*                       
*                                                                             
*   Authors: Eran Yariv - 28484475                                           
*            Moshe Zur  - 24070856                                           
*                                                                            
*                                                                            
*   Date: 23/09/98                                                           
*                                                                            
******************************************************************************/
#if !defined(AFX_SERVERMGMTDLG_H__0F2E7C42_49A2_11D2_8865_B232F056F5C3__INCLUDED_)
#define AFX_SERVERMGMTDLG_H__0F2E7C42_49A2_11D2_8865_B232F056F5C3__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// servermgmtdlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CServerMgmtDlg dialog

class CServerMgmtDlg : public CDialog
{
// Construction
public:
	CServerMgmtDlg(CWnd* pParent = NULL);   // standard constructor
    ~CServerMgmtDlg () {}

    static void Open (CWnd *pParent);

    void PostNcDestroy( );
    void OnCancel ();

// Dialog Data
	//{{AFX_DATA(CServerMgmtDlg)
	enum { IDD = IDD_SERVER_MGMT };
	CListCtrl	m_PlayersList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CServerMgmtDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CServerMgmtDlg)
	afx_msg void OnDisconnectPlayer();
	afx_msg void OnClose();
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	int         GetCurSelection ();
	void        RefreshList ();
    static void Close ();
    int         FindPlayerIndex (UINT);
    void        RemoveNonExistantPlayers (PBOOL pbPlayers);
    void        UpdateOrAddPlayersData (PBOOL pbPlayers);

    CImageList      m_ImgList;
    CCommManager   &m_gCommManager;

    int             m_ImgIndex[MAX_TANKS];
    static CServerMgmtDlg *m_pDlg;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SERVERMGMTDLG_H__0F2E7C42_49A2_11D2_8865_B232F056F5C3__INCLUDED_)
