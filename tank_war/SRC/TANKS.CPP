/*****************************************************************************
*                                                                             
*   Tanks.cpp
*                                                                             
*   Electrical Engineering Faculty - Software Lab                             
*   Spring semester 1998                                                      
*                                                                             
*   Tanks game                                                                
*                                                                             
*   Module description: Implements the main application object. Holds the 
*                       singleton objects that are shared by the apps threads.
*                       
*                                                                             
*   Authors: Eran Yariv - 28484475                                           
*            Moshe Zur  - 24070856                                           
*                                                                            
*                                                                            
*   Date: 23/09/98                                                           
*                                                                            
******************************************************************************/
#include "stdafx.h"
#include "Tanks.h"
#include "TanksDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTanksApp

BEGIN_MESSAGE_MAP(CTanksApp, CWinApp)
	//{{AFX_MSG_MAP(CTanksApp)
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTanksApp construction

CTanksApp::CTanksApp() :
    m_gIncomingMsgQueue(FALSE), // Incoming queue is non blocking
    m_hSingleInstanceSemaphore (0),
    m_uMaxRTT(MAX_VALID_RTT)
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
    m_pMainWnd = NULL;
    for (int i = 0; i < MAX_TANKS; i++)
        m_guRemoteTanksDirs[i] = INVALID_DIRECTION;
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CTanksApp object

CTanksApp theApp;
/////////////////////////////////////////////////////////////////////////////
// CTanksApp initialization

BOOL CTanksApp::InitInstance()
{
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.
    if (!IsOnlyInstance())
        return FALSE;   // Cannot run more than one instance on same machine 

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

    SetRegistryKey(IDS_REGISTRY_KEY);

    m_gKbdManager.InitKeysTable();  // Set keys bindings before dialog is displayed

    CTanksDlg dlg;
	m_pMainWnd = &dlg;
    SetDialogBkColor (RGB(0,0,0), RGB(192, 192, 192));
	dlg.DoModal();
    if (dlg.IsGameOn())
        // Game dialog was closed abruptly
        EndGameThreads ();
	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

BOOL CTanksApp::StartGameThreads ()
{
    CWaitCursor cws;

    BOOL bCommManagerRes =
        TANKS_APP->m_gCommManager.OnNewGame();

    if (FALSE == bCommManagerRes)
    {   // Comm. manager failed to start:
        TANKS_APP->m_gCommManager.OnStopGame();
        return FALSE;
    }
        // Start the game rendering engine:
    m_gGameManager.BeginGame ();
    return TRUE;
}

void CTanksApp::EndGameThreads ()
{
    CWaitCursor cws;
    m_gGameManager.EndGame ();
    m_gCommManager.OnStopGame();
}

BOOL
CTanksApp::GetStoredGUID (GUID *pGUID)
{
    wchar_t wszGUID[39];

    // Get stored IID as string:
    CString strGUID = GetProfileString (COMM_SECTION, CONNECTION_GUID_ENTRY, "");
    
    if (strGUID.IsEmpty())
        return FALSE;
    // Convert string to wide chars:
    int nRes = mbstowcs (wszGUID, strGUID.GetBuffer(39), 39);
    if (-1 == nRes)
        return FALSE;

    return (S_OK == IIDFromString (wszGUID, pGUID));
}

BOOL 
CTanksApp::SetStoredGUID (GUID& GUID)
{
    wchar_t *wszGUID = NULL;
    LPSTR szGUID = NULL;
    BOOL bRes = FALSE;

    if (S_OK != StringFromIID (GUID, &wszGUID))
        goto CLEAN_UP;

    szGUID = new char[wcslen(wszGUID) + 1];

    if (-1 == wcstombs (szGUID, wszGUID, wcslen (wszGUID) + 1))
        goto CLEAN_UP;

    bRes = WriteProfileString(COMM_SECTION, CONNECTION_GUID_ENTRY, CString (szGUID));

CLEAN_UP:
    if (wszGUID)
        ::CoTaskMemFree(wszGUID);
    if (szGUID)
        delete [] szGUID;

    return bRes;
}


BOOL
CTanksApp::IsOnlyInstance ()
{

#define SINGLE_INSTANCE_SEM_NAME        "TanksAppSingleInstanceSemaphore"

    m_hSingleInstanceSemaphore = OpenSemaphore (SEMAPHORE_ALL_ACCESS, TRUE, SINGLE_INSTANCE_SEM_NAME);
    if (NULL != m_hSingleInstanceSemaphore)
    {   // Other instance exists
        AfxMessageBox (IDS_SINGLE_INSTANCE, MB_OK | MB_ICONEXCLAMATION);
        return FALSE;
    }

    m_hSingleInstanceSemaphore = CreateSemaphore (NULL, 0, 1, SINGLE_INSTANCE_SEM_NAME);
    if (NULL != m_hSingleInstanceSemaphore)
        return TRUE;

    DWORD dwErr = GetLastError ();
    CString cstr;
    cstr.Format ("Cannot create semaphore : %d", dwErr);
    AfxMessageBox (cstr,MB_OK | MB_ICONHAND);
    return FALSE;
}

CTanksApp::~CTanksApp ()
{
    if (m_hSingleInstanceSemaphore)
        CloseHandle (m_hSingleInstanceSemaphore);
}

