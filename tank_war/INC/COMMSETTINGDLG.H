/*****************************************************************************
*                                                                             
*   CommSettingDlg.h
*                                                                             
*   Electrical Engineering Faculty - Software Lab                             
*   Spring semester 1998                                                      
*                                                                             
*   Tanks game                                                                
*                                                                             
*   Module description: Interface of the Communication Setting dialog.
*                       
*                                                                             
*   Authors: Eran Yariv - 28484475                                           
*            Moshe Zur  - 24070856                                           
*                                                                            
*                                                                            
*   Date: 23/09/98                                                           
*                                                                            
******************************************************************************/
#if !defined(AFX_COMMSETTINGDLG_H__6756D661_0D98_11D2_91DB_444553540000__INCLUDED_)
#define AFX_COMMSETTINGDLG_H__6756D661_0D98_11D2_91DB_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include <AniButton.h>

// CommSettingDlg.h : header file
//

//
//  Callback function for DPlay to display connection available:
//
BOOL FAR PASCAL EnumConnections(
    LPGUID lpSPGuid, LPTSTR lpszSPName, DWORD dwMajorVersion,
    DWORD dwMinorVersion, LPVOID lpContext);

/////////////////////////////////////////////////////////////////////////////
// CCommSettingDlg dialog

class CCommSettingDlg : public CDialog
{
// Construction
public:
    CCommSettingDlg();

// Dialog Data
    //{{AFX_DATA(CCommSettingDlg)
    enum { IDD = IDD_COMM_SETTING };
    CAniButton  m_ctrOK;
    CAniButton  m_ctrCancel;
    CListBox    m_ConnectionsList;
    CString     m_strPlayerName;
    //}}AFX_DATA


// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CCommSettingDlg)
    protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:
    // Generated message map functions
    //{{AFX_MSG(CCommSettingDlg)
    virtual void OnOK();
    virtual BOOL OnInitDialog();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
private:
    void SelectStoredGUID();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COMMSETTINGDLG_H__6756D661_0D98_11D2_91DB_444553540000__INCLUDED_)
