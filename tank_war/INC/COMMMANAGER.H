/*****************************************************************************
*                                                                             
*   CommManager.h
*                                                                             
*   Electrical Engineering Faculty - Software Lab                             
*   Spring semester 1998                                                      
*                                                                             
*   Tanks game                                                                
*                                                                             
*   Module description: Interface for the communication manager class.
*                       
*                                                                             
*   Authors: Eran Yariv - 28484475                                           
*            Moshe Zur  - 24070856                                           
*                                                                            
*                                                                            
*   Date: 23/09/98                                                           
*                                                                            
******************************************************************************/
#ifndef _COMM_MANAGER_H_
#define _COMM_MANAGER_H_

#include ".\\dplay.h"
#include "CommSettingDlg.h"
#include "WorkerThread.h"

struct CCommMessage
{
    // Consts:
    enum { MAX_BUFFER_LENGTH = 2048 };
    BYTE            m_aBuffer[MAX_BUFFER_LENGTH];
    DWORD           m_dwLength;
};

// Forward decl.
class CClient;
class CHost;

class CCommManager : public CWorkerThread
{
public:
    
    CCommManager();             // Alloc and initialize DPlay
    ~CCommManager ();           // Free DPlay

    void SetCommParams();       // Present the Comm. pref. dialog

    BOOL OnNewGame();           // Player : connects to server - presents a progress dlg
                                // Host   : start the server - game starts w/o delay

    BOOL OnStopGame();          // Player : disconnect from server
                                // Host   : end session

    BOOL IsConnected();         // Player : participate in a session
                                // Host   : session started

    BOOL IsHost();

    UINT ThreadEntry (LPVOID pParam = 0);

    void NotifyBonusEaten (UINT uTankID);               // Tell all the players a tank ate the current bonus
    void NotifyNewBonus (BonusType, DWORD, CPoint &);   // Tell all the game managers a new bonus is born
    void NotifyExplodingTank(UINT uTankID);             // Tell all the players a tank is exploding
    void NotifyCheckSum(CMessage::MessageData &);       // Tell the host about a new checksum
    void NotifyChatMsg(LPCSTR szMsg);                   // Tell the host to dispatch the chat msg
                                                        // Called by the ChatDlg on new msg.

    CCommMessage& ExposeMessage();
    void SetHostID();
    void SendAsHost(DPID idTo);   

    // Consts
    static const BYTE ACK;
    static const DPID INVALID_PLAYER_ID;

    DPID GetHostID ()           { return m_idHost;   }

    BOOL GetPlayerInfo (UINT ind, BOOL &, CString &, DWORD &, DWORD &);
    void GetPlayerName (DPID id, CString &);    // Get the player name for tank (i)
    BOOL KillPlayer (UINT uPlayerID);


private:
    // methods:
    void CreateDPInterface ();
    void OpenSession();
    void CloseSession();
    void CreatePlayer();
    void DestroyPlayer();
    void EndThread(BOOL bWait);
    void DisplayMessageAndEndGame (UINT u=0);

    void HandleClientMessage();
    void HandleHostMessage();

    // members:
    LPDIRECTPLAY2   m_pIDP;
    DPID            m_idPlayer;
    DPID            m_idHost;
    DPCAPS          m_SessionCaps;

    CClient*        m_pClient;
    CHost*          m_pHost;
    BOOL            m_fIsConnected;             // Session is on
    BOOL            m_fIsHost;                  // Session was opened by us
    BOOL            m_fIsPlaying;               // Player was created

    CMsgQueue&      m_IncomingMsgQ;
    CMsgQueue&      m_OutgoingMsgQ;
    HANDLE          m_hRecvEvent;
    HANDLE          m_hQuitEvent;

    CCommMessage    m_Message;
    DPID            m_idFrom;
    DPID            m_idTo;

    CCriticalSection  m_CSHost;
    BOOL            m_bLostServerConnection;    // Has client lost connection to host?


#ifdef GATHER_NETWORK_STATS
    // Times and totals:
    DWORD           m_dwSessionStartTime;                   // Store DPlay session start time
    DWORD           m_dwTotalBytesSent;                     // Keep total bytes sent
    DWORD           m_dwTotalBytesReceived;                 // Keep total bytes received
    DWORD           m_dwTotalClientMsgsSent;                // Total number of client messages sent
    DWORD           m_dwTotalClientMsgsReceived;            // Total number of client messages received
    DWORD           m_dwTotalHostMsgsSent;                  // Total number of host messages sent
    DWORD           m_dwTotalHostMsgsReceived;              // Total number of host messages received

    // Packet size statistics:
    DWORD           m_dwBiggestSentPacketSize;              // Keep biggest sent packet size
    DWORD           m_dwSmallestSentPacketSize;             // Keep smallest sent packet size
    DWORD           m_dwBiggestReceivedPacketSize;          // Keep biggest received packet size
    DWORD           m_dwSmallestReceivedPacketSize;         // Keep smallest received packet size

    // Time window statistics:
    DWORD           m_dwLastSecTimeRecv;                    // Used to trace one second of time (approx.)
    DWORD           m_dwLastSecTimeSend;                    // Used to trace one second of time (approx.)
    DWORD           m_dwLastRecvTotal;
    DWORD           m_dwLastSentTotal;
    DWORD           m_dwMaxBytesSentInLastSecond;           // Max sent packet size in last second
    DWORD           m_dwMaxBytesReceivedInLastSecond;       // Max received packet size in last second

    // Errors:
    DWORD           m_dwClientSendErrors;                   // Total number of client message sending errors
    DWORD           m_dwHostSendErrors;                     // Total number of host message sending errors
    DWORD           m_dwReceiveErrors;                      // Total number of message receive errors

    void UpdateReceive ();
    void UpdateSend ();
    void TraceNetworkStats();

#endif // GATHER_NETWORK_STATS
};

// {E8660CE0-0DA6-11d2-91DB-444553540000}
DEFINE_GUID(TANKS_GUID, 
    0xe8660ce0, 0xda6, 0x11d2, 0x91, 0xdb, 0x44, 0x45, 0x53, 0x54, 0x0, 0x0);

#include "CommManager.inl"

#endif