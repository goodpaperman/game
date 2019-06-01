/*****************************************************************************
*                                                                             
*   Tanks.h
*                                                                             
*   Electrical Engineering Faculty - Software Lab                             
*   Spring semester 1998                                                      
*                                                                             
*   Tanks game                                                                
*                                                                             
*   Module description: Interface of the main application class.
*                       
*                                                                             
*   Authors: Eran Yariv - 28484475                                           
*            Moshe Zur  - 24070856                                           
*                                                                            
*                                                                            
*   Date: 23/09/98                                                           
*                                                                            
******************************************************************************/
#if !defined(AFX_TANKS_H__22F0A6A5_E0C9_11D1_9738_E570871C4325__INCLUDED_)
#define AFX_TANKS_H__22F0A6A5_E0C9_11D1_9738_E570871C4325__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
#include <ImageManager.h>
#include <ManouverSet.h>
#include <KbdManager.h>
#include <GameManager.h>
#include <Timer.h>
#include <MsgQueue.h>
#include <CommManager.h>
#include <SoundManager.h>

// Registry sections
#define     GAME_SETUP_SECTION      "GameSetup"
#define     KEYBOARD_SECTION        "Keyboard mapping"
#define     COMM_SECTION            "Communication setting"

// Registry entries
#define     TANK_TYPE_ENTRY         "TankType"
#define     MAP_DIFF_ENTRY          "MapComplexity"
#define     RIGHT_KEY_ENTRY         "Key-Right"
#define     LEFT_KEY_ENTRY          "Key-Left"
#define     FORWARD_KEY_ENTRY       "Key-Forward"
#define     BACKWARD_KEY_ENTRY      "Key-Backward"
#define     SHELL_KEY_ENTRY         "Key-Shell"    
#define     BULLET_KEY_ENTRY        "Key-Bullet"
#define     MINE_KEY_ENTRY          "Key-Mine"
#define     AERIAL_KEY_ENTRY        "Key-Aerial"
#define     PLAYER_NAME_ENTRY       "Player name"
#define     IS_HOST_ENTRY           "Is Host"
#define     CONNECTION_GUID_ENTRY   "Connection GUID"
#define     MAX_RTT_ENTRY           "Max RTT (ms)"
#define     MUTE_MODE_ENTRY         "Game Sound"


/////////////////////////////////////////////////////////////////////////////
// CTanksApp:
// See Tanks.cpp for the implementation of this class
//

class CTanksApp : public CWinApp
{
public:
	CTanksApp();
    ~CTanksApp();

    BOOL StartGameThreads ();
    void EndGameThreads ();

    // Persistence :

    UINT GetStoredPreferedTankID ()
        { return GetProfileInt (GAME_SETUP_SECTION, TANK_TYPE_ENTRY, 0); }
    UINT GetStoredMapComplexity ()
        { return GetProfileInt (GAME_SETUP_SECTION, MAP_DIFF_ENTRY, 10); }

    UINT GetStoredRightKey (UINT uDefault)
        { return GetProfileInt (KEYBOARD_SECTION, RIGHT_KEY_ENTRY, uDefault); }
    BOOL SetStoredRightKey (UINT uKey)
        { return WriteProfileInt (KEYBOARD_SECTION, RIGHT_KEY_ENTRY, uKey); }

    UINT GetStoredLeftKey (UINT uDefault)
        { return GetProfileInt (KEYBOARD_SECTION, LEFT_KEY_ENTRY, uDefault); }
    BOOL SetStoredLeftKey (UINT uKey)
        { return WriteProfileInt (KEYBOARD_SECTION, LEFT_KEY_ENTRY, uKey); }

    UINT GetStoredForwardKey (UINT uDefault)
        { return GetProfileInt (KEYBOARD_SECTION, FORWARD_KEY_ENTRY, uDefault); }
    BOOL SetStoredForwardKey (UINT uKey)
        { return WriteProfileInt (KEYBOARD_SECTION, FORWARD_KEY_ENTRY, uKey); }

    UINT GetStoredBackwardKey (UINT uDefault)
        { return GetProfileInt (KEYBOARD_SECTION, BACKWARD_KEY_ENTRY, uDefault); }
    BOOL SetStoredBackwardKey (UINT uKey)
        { return WriteProfileInt (KEYBOARD_SECTION, BACKWARD_KEY_ENTRY, uKey); }

    UINT GetStoredShellKey (UINT uDefault)
        { return GetProfileInt (KEYBOARD_SECTION, SHELL_KEY_ENTRY, uDefault); }
    BOOL SetStoredShellKey (UINT uKey)
        { return WriteProfileInt (KEYBOARD_SECTION, SHELL_KEY_ENTRY, uKey); }

    UINT GetStoredBulletKey (UINT uDefault)
        { return GetProfileInt (KEYBOARD_SECTION, BULLET_KEY_ENTRY, uDefault); }
    BOOL SetStoredBulletKey (UINT uKey)
        { return WriteProfileInt (KEYBOARD_SECTION, BULLET_KEY_ENTRY, uKey); }

    UINT GetStoredMineKey (UINT uDefault)
        { return GetProfileInt (KEYBOARD_SECTION, MINE_KEY_ENTRY, uDefault); }
    BOOL SetStoredMineKey (UINT uKey)
        { return WriteProfileInt (KEYBOARD_SECTION, MINE_KEY_ENTRY, uKey); }

    UINT GetStoredAerialKey (UINT uDefault)
        { return GetProfileInt (KEYBOARD_SECTION, AERIAL_KEY_ENTRY, uDefault); }
    BOOL SetStoredAerialKey (UINT uKey)
        { return WriteProfileInt (KEYBOARD_SECTION, AERIAL_KEY_ENTRY, uKey); }

    CString GetStoredPlayerName ()
        { return GetProfileString (COMM_SECTION, PLAYER_NAME_ENTRY, "Anonymous"); }
    BOOL SetStoredPlayerName (CString strName)
        { return WriteProfileString (COMM_SECTION, PLAYER_NAME_ENTRY, strName); }

    BOOL GetStoredIsHostFlag ()
        { return (0 != GetProfileInt (COMM_SECTION, IS_HOST_ENTRY, 1)); }
    BOOL SetStoredIsHostFlag (UINT uKey)
        { return WriteProfileInt (COMM_SECTION, IS_HOST_ENTRY, uKey); }

    BOOL GetStoredMuteMode ()
        { return (0 != GetProfileInt (GAME_SETUP_SECTION, MUTE_MODE_ENTRY, 1)); }
    BOOL SetStoredMuteMode (BOOL bMuteMode)
        { return WriteProfileInt (GAME_SETUP_SECTION, MUTE_MODE_ENTRY, bMuteMode); }

    void SetMaxRTT (UINT uDefault)
    { 
        m_uMaxRTT = GetProfileInt (COMM_SECTION, MAX_RTT_ENTRY, uDefault); 
        WriteProfileInt (COMM_SECTION, MAX_RTT_ENTRY, m_uMaxRTT);
    }
    UINT GetMaxRTT ()
        { return m_uMaxRTT; }

    BOOL GetStoredGUID (GUID *pGUID);
    BOOL SetStoredGUID (GUID& GUID);

	void SetMapHWND (HWND h)		{m_gHwndMap = h;}

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTanksApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CTanksApp)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
    BOOL                IsOnlyInstance ();
    HANDLE              m_hSingleInstanceSemaphore;
    UINT                m_uMaxRTT;       // Init from reg.when starting new game

public:
    CImageManager       m_gImageManager;
    CManouverSet        m_gManouverSets[MAX_TANKS];
    UINT                m_guRemoteTanksDirs[MAX_TANKS];
    CKbdManager         m_gKbdManager;
    CGameManager        m_gGameManager;
    TIMER_CLASS         m_gTimer;
    CMsgQueue           m_gIncomingMsgQueue;
    CMsgQueue           m_gOutgoingMsgQueue;
    CCommManager        m_gCommManager;
    HWND                m_gHwndMap;
    CDrawDib            m_gDrawDIB;      // DIB drawing mechanism
    CSoundManager       m_gSoundManager;
};

#define TANKS_APP   ((CTanksApp*)AfxGetApp())
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TANKS_H__22F0A6A5_E0C9_11D1_9738_E570871C4325__INCLUDED_)
