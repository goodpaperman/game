/*****************************************************************************
*                                                                             
*   KeyMappingDlg.h
*                                                                             
*   Electrical Engineering Faculty - Software Lab                             
*   Spring semester 1998                                                      
*                                                                             
*   Tanks game                                                                
*                                                                             
*   Module description: Interface of the Key Mapping dialog.
*                       
*                                                                             
*   Authors: Eran Yariv - 28484475                                           
*            Moshe Zur  - 24070856                                           
*                                                                            
*                                                                            
*   Date: 23/09/98                                                           
*                                                                            
******************************************************************************/
#if !defined(AFX_KEYMAPPINGDLG_H__77E8DF4F_E5D5_11D1_AA8A_444553540000__INCLUDED_)
#define AFX_KEYMAPPINGDLG_H__77E8DF4F_E5D5_11D1_AA8A_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// KeyMappingDlg.h : header file
//
#include "MyEdit.h"     // Defines CMyEdit
#include "KeysTable.h"
#include "resource.h"
#include <AniButton.h>

/////////////////////////////////////////////////////////////////////////////
// CKeyMappingDlg dialog

class CKeyMappingDlg : public CDialog
{
// Construction
public:
	CKeyMappingDlg(CKeysTable* pKeys, CWnd* pParent = NULL); // Constructor

//    BOOL SetCEditString(int);   // Return true if focus is on an Edit control

// Dialog Data
	//{{AFX_DATA(CKeyMappingDlg)
	enum { IDD = IDD_KEYMAPPING_DIALOG };
	CAniButton	m_ctrOK;
    CAniButton  m_ctrCancel;
    CAniButton  m_ctrDefault;
	CMyEdit	m_Right;
	CMyEdit	m_Left;
	CMyEdit	m_Shell;
	CMyEdit	m_Bullet;
	CMyEdit	m_Mine;
	CMyEdit	m_Backward;
	CMyEdit	m_Aerial;
	CMyEdit	m_Forward;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CKeyMappingDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;
    CKeysTable* m_pKeys;  // Pointer to a copy of keys settings

//    BOOL KeyToString (int iKey, CString& cstr);
    void UpdateAllCEdit();
	// Generated message map functions
	//{{AFX_MSG(CKeyMappingDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnButtonDefaultSettings();
	afx_msg void OnSetfocusEditAerialSupport();
	afx_msg void OnSetfocusEditBackward();
	afx_msg void OnSetfocusEditDropMine();
	afx_msg void OnSetfocusEditFireBullet();
	afx_msg void OnSetfocusEditFireShell();
	afx_msg void OnSetfocusEditForward();
	afx_msg void OnSetfocusEditTurnLeft();
	afx_msg void OnSetfocusEditTurnRight();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_KEYMAPPINGDLG_H__77E8DF4F_E5D5_11D1_AA8A_444553540000__INCLUDED_)
