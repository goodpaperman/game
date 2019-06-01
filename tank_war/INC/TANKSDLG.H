/*****************************************************************************
*                                                                             
*   TanksDlg.h
*                                                                             
*   Electrical Engineering Faculty - Software Lab                             
*   Spring semester 1998                                                      
*                                                                             
*   Tanks game                                                                
*                                                                             
*   Module description: Interface of the main tanks dialog class.
*                       
*                                                                             
*   Authors: Eran Yariv - 28484475                                           
*            Moshe Zur  - 24070856                                           
*                                                                            
*                                                                            
*   Date: 23/09/98                                                           
*                                                                            
******************************************************************************/
#if !defined(AFX_TANKSDLG_H__22F0A6A7_E0C9_11D1_9738_E570871C4325__INCLUDED_)
#define AFX_TANKSDLG_H__22F0A6A7_E0C9_11D1_9738_E570871C4325__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "resource.h"
#include "ShieldStatus.h"
#include "AmmoStatus.h"
#include "BonusStatus.h"
#include "GameConsts.h"
#include "DrawDib.h"
#include "KbdManager.h"
#include "GameManager.h"
#include <servermgmtdlg.h>
#include "chatdlg.h"


class CTanksApp;    // Pre-declaration

/////////////////////////////////////////////////////////////////////////////
// CTanksDlg dialog

typedef enum {
    GAME_OFF,   // Game is not active, all threads are dead
    GAME_ON,    // Game is active, threads are alive, local tank is alive
    GAME_OVER   // Game is active, threads are alive, local tank is dead
} GameStateType;
                
/* The following table describes how the game changes from state to state:

    This table might change in the future as comm. with the server is introduced.

   +---------------+-----------------+-------------------+------------------+
   | From state    | to GAME_OFF     | to GAME_ON        | to GAME_OVER     |
   +===============+=================+===================+==================+
   | GAME_OFF      | (already there) | User starts a new | (no direct path) |
   |               |                 | game              |                  |
   +---------------+-----------------+-------------------+------------------+
   | GAME_ON       | (no direct path)| (already there)   | Local tank dies  |
   |               |                 |                   |                  |
   +---------------+-----------------+-------------------+------------------+
   | GAME_OVER     | User presses any| (no direct path)  | (already there)  |
   |               | key             |                   |                  |
   +---------------+-----------------+-------------------+------------------+
*/

class CTanksDlg : public CDialog
{
// Construction
public:
	CTanksDlg(CWnd* pParent = NULL);	// standard constructor
    virtual ~CTanksDlg ();

    // Local tank report functions:
    void SetShieldLevel (UINT);
    void SetShellsCount (UINT);
    void SetBulletsCount (UINT);
    void SetMinesCount (UINT);
    void SetAerialSupport (BOOL);
    void SetFastFireRate (BOOL);

    BOOL OnEraseBkgnd(CDC* pDC);

    BOOL IsGameOn ()
    {   return (GAME_OFF != m_GameState); }

    BOOL PreTranslateMessage(MSG* pMsg);    // respond for user input

    void MarshalChatMsg(DPID dwDPlayID, DWORD dwTankID, LPCSTR szMsg);

// Dialog Data
	//{{AFX_DATA(CTanksDlg)
	enum { IDD = IDD_TANKS_DIALOG };
	CAmmoStatus     m_AmmoStatus;
	CBonusStatus    m_BonusStatus;
	CShieldStatus  	m_ShieldStatus;
	CButton         m_Map;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTanksDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CTanksDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnExit();
	afx_msg void OnHelpAbout();
	afx_msg void OnKbdMapping();
	afx_msg void OnNewGame();
	afx_msg void OnSetComm();
	afx_msg void OnSetGame();
	afx_msg void OnStopGame();
	afx_msg void OnSoundMute();
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg void OnClose();
	afx_msg BOOL OnQueryNewPalette();
	afx_msg void OnPaletteChanged(CWnd* pFocusWnd);
	afx_msg void OnServerMgmt();
	afx_msg void OnPlayersGuide();
    afx_msg void OnChatOpen();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	void GameOver (void);   // Called by a dying tank object

private:
	void CheckDPlayVersion ();
	void StartDIBDrawingEngine ();
	void CreateStatusPanes ();
	void HideGameControl (CWnd &ctrl, BOOL bHide);
	void AlignControls ();
    void OnOK ();

    GameStateType    m_GameState;       // Current game state
    CKbdManager     &m_KbdManager;      // Alias to kbd manager
    CGameManager    &m_GameManager;     // Alias to the game manager

    CServerMgmtDlg   m_SrvMgmtDlg;
    CChatDlg         m_ChatDlg;
};

void CALLBACK EXPORT BonusGenerator(
       HWND hWnd,      // handle of CWnd that called SetTimer
       UINT nMsg,      // WM_TIMER
       UINT nIDEvent,  // timer identification
       DWORD dwTime    // system time
    );


#endif // !defined(AFX_TANKSDLG_H__22F0A6A7_E0C9_11D1_9738_E570871C4325__INCLUDED_)

inline void CTanksDlg::SetShieldLevel (UINT uLevel)
{
    m_ShieldStatus.SetLevel (uLevel);
}
        
inline void CTanksDlg::SetShellsCount (UINT uCount)
{
    m_AmmoStatus.SetShellsCount (uCount);
}

inline void CTanksDlg::SetBulletsCount (UINT uCount)
{
    m_AmmoStatus.SetBulletsCount (uCount);
}

inline void CTanksDlg::SetMinesCount (UINT uCount)
{
    m_AmmoStatus.SetMinesCount (uCount);
}

inline void CTanksDlg::SetAerialSupport (BOOL bOn)
{
    m_BonusStatus.SetAerialSupport (bOn);
}

inline void CTanksDlg::SetFastFireRate (BOOL bOn)
{
    m_BonusStatus.SetFastFireRate (bOn);
}

inline void 
CTanksDlg::MarshalChatMsg(DPID dwDPlayID, DWORD dwTankID, LPCSTR szMsg)
{
    m_ChatDlg.AddMessage (dwDPlayID, dwTankID, szMsg);
}
