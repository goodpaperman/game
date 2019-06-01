/*****************************************************************************
*                                                                             
*   CommManager.cpp                                                            
*                                                                             
*   Electrical Engineering Faculty - Software Lab                             
*   Spring semester 1998                                                      
*                                                                             
*   Tanks game                                                                
*                                                                             
*   Module description: The Communication Manager wraps the DPlay API, and 
*                       manages the communication setting, initializing,
*                       termination, and messages traffic to and from the app.
*                                                                             
*   Authors: Eran Yariv - 28484475                                           
*            Moshe Zur  - 24070856                                           
*                                                                            
*                                                                            
*   Date: 23/09/98                                                           
*                                                                            
******************************************************************************/
#include "stdafx.h"
#include <objbase.h>    // we need this or else initguid.h won't let us compile
#include <initguid.h>   // including the header makes DEFINE_GUID define the GUID.
#include "Tanks.h"
#include "Client.h"
#include "Host.h"
#include "EnumSession.h"

#ifdef GATHER_NETWORK_STATS

    #define SET_SESSION_START_TIME(t)           {m_dwSessionStartTime=(t);      \
                                                 m_dwLastSecTimeRecv = m_dwLastSecTimeSend = m_dwSessionStartTime;}

    #define INC_TOTAL_BYTES_SENT(cb)            m_dwTotalBytesSent+=(cb);
    #define INC_TOTAL_BYTES_RECEIVED(cb)        m_dwTotalBytesReceived+=(cb);
    #define INC_TOTAL_CLIENT_MESSAGES_SENT      m_dwTotalClientMsgsSent++;
    #define INC_TOTAL_CLIENT_MESSAGES_RECEIVED  m_dwTotalClientMsgsReceived++;
    #define INC_TOTAL_HOST_MESSAGES_SENT        m_dwTotalHostMsgsSent++;
    #define INC_TOTAL_HOST_MESSAGES_RECEIVED    m_dwTotalHostMsgsReceived++;
    
    #define UPDATE_BIGGEST_PACKET_SENT(cb)      m_dwBiggestSentPacketSize = max (m_dwBiggestSentPacketSize, (cb));
    #define UPDATE_SMALLEST_PACKET_SENT(cb)     m_dwSmallestSentPacketSize = min (m_dwSmallestSentPacketSize, (cb));
    #define UPDATE_BIGGEST_PACKET_RECEIVED(cb)  m_dwBiggestReceivedPacketSize = max (m_dwBiggestReceivedPacketSize, (cb));
    #define UPDATE_SMALLEST_PACKET_RECEIVED(cb) m_dwSmallestReceivedPacketSize = min (m_dwSmallestReceivedPacketSize,(cb));

    #define INC_CLIENT_SEND_ERRORS              m_dwClientSendErrors++;
    #define INC_HOST_SEND_ERRORS                m_dwHostSendErrors++;
    #define INC_RECEIVE_ERRORS                  m_dwReceiveErrors++;

    #define UPDATE_RECEIVE                      UpdateReceive();
    #define UPDATE_SEND                         UpdateSend();
    #define TRACE_NETWORK_STATS                 TraceNetworkStats();


#else       // !defined GATHER_NETWORK_STATS

    #define SET_SESSION_START_TIME(t)           
    #define INC_TOTAL_BYTES_SENT(cb)            
    #define INC_TOTAL_BYTES_RECEIVED(cb)        
    #define INC_TOTAL_CLIENT_MESSAGES_SENT      
    #define INC_TOTAL_CLIENT_MESSAGES_RECEIVED  
    #define INC_TOTAL_HOST_MESSAGES_SENT        
    #define INC_TOTAL_HOST_MESSAGES_RECEIVED    
    
    #define UPDATE_BIGGEST_PACKET_SENT(cb)      
    #define UPDATE_SMALLEST_PACKET_SENT(cb)     
    #define UPDATE_BIGGEST_PACKET_RECEIVED(cb)  
    #define UPDATE_SMALLEST_PACKET_RECEIVED(cb) 
    #define INC_CLIENT_SEND_ERRORS              
    #define INC_HOST_SEND_ERRORS                
    #define INC_RECEIVE_ERRORS                  
    #define UPDATE_RECEIVE                      
    #define UPDATE_SEND
    #define TRACE_NETWORK_STATS

#endif // defined GATHER_NETWORK_STATS



char TANKS_SESSION_NAME[] = "Tanks Game";
const BYTE CCommManager::ACK = 0xFF;
const DPID CCommManager::INVALID_PLAYER_ID = MAX_DWORD;

CCommManager::CCommManager() :
    m_pIDP(NULL),
    m_fIsConnected(FALSE),
    m_fIsPlaying(FALSE),
    m_fIsHost(TRUE),
    m_IncomingMsgQ(TANKS_APP->m_gIncomingMsgQueue),
    m_OutgoingMsgQ(TANKS_APP->m_gOutgoingMsgQueue),
    m_idHost(INVALID_PLAYER_ID),
    m_idPlayer(INVALID_PLAYER_ID),
    m_pHost(NULL)

#ifdef GATHER_NETWORK_STATS
    ,
    m_dwTotalBytesSent               (0),
    m_dwTotalBytesReceived           (0),
    m_dwBiggestSentPacketSize        (0),
    m_dwLastRecvTotal                (0),
    m_dwLastSentTotal                (0),
    m_dwSmallestSentPacketSize       (MAX_DWORD),
    m_dwBiggestReceivedPacketSize    (0),
    m_dwSmallestReceivedPacketSize   (MAX_DWORD),
    m_dwMaxBytesSentInLastSecond     (0),
    m_dwMaxBytesReceivedInLastSecond (0),
    m_dwTotalClientMsgsSent          (0),
    m_dwTotalClientMsgsReceived      (0),
    m_dwClientSendErrors             (0),   
    m_dwTotalHostMsgsSent            (0),
    m_dwTotalHostMsgsReceived        (0),
    m_dwHostSendErrors               (0),
    m_dwReceiveErrors                (0)
#endif //   GATHER_NETWORK_STATS
{
    m_hRecvEvent = CreateEvent (NULL, FALSE, FALSE, NULL);
    m_hQuitEvent = CreateEvent (NULL, FALSE, FALSE, NULL);
    // Create client
    m_pClient = new CClient(*this);
}

CCommManager::~CCommManager ()
{
    if (m_pIDP)
        m_pIDP->Release();

    if (m_pClient)
        delete m_pClient;

    if (m_pHost)
        delete m_pHost;

    CloseHandle (m_hRecvEvent);
    CloseHandle (m_hQuitEvent);
}


void
CCommManager::SetCommParams()
{
    CCommSettingDlg dlg;

    dlg.DoModal();

    return;
}

/*------------------------------------------------------------------------------

  Function: OnNewGame

  Purpose:  Initailizes the DPlay object, and starts the connection protocol w/
            the server. In case this is a server machine, it also starts the
            host object.

  Input:    None.

  Output:   Return TRUE if succeeded to connect to a game session.

------------------------------------------------------------------------------*/
BOOL 
CCommManager::OnNewGame()
{
    try {
        // Create DirectPlay2 interface
        CreateDPInterface();
        // Start session
        OpenSession();
        // Create player
        CreatePlayer();
    } catch (CString &strErr) {
        AfxMessageBox(strErr, MB_OK | MB_ICONEXCLAMATION);
        return FALSE;
    }
    //
    //  At last - all is well, start the message pumping:
    //
        // Empty both queues:
    TANKS_APP->m_gOutgoingMsgQueue.Empty();
    TANKS_APP->m_gOutgoingMsgQueue.Empty();

    m_bLostServerConnection = FALSE;
    TANKS_APP->SetMaxRTT(MAX_VALID_RTT);  // Init the global max RTT param from the reg.
    StartWorkerThread();

    if (! m_pClient->WaitForGameProperties()) {
        AfxMessageBox("Server timed out");
        return FALSE;
    }
    return TRUE;
}

BOOL 
CCommManager::OnStopGame()
{
    if (m_fIsPlaying) {
        // Stop the working thread:
        EndThread(TRUE);
        // Destroy the player object:
        DestroyPlayer();
        // Stop the session:
        CloseSession();
    }
    return TRUE;
}

void
CCommManager::CreateDPInterface ()
{
    LPDIRECTPLAY   lpDirectPlay1 = NULL;
    HRESULT        hr;

    //
    //  Get the GUID of the connection provider.
    //
    GUID guidCurr;
    if (! TANKS_APP->GetStoredGUID(&guidCurr)) 
    {   // Its the first run on this machine and no connection was selected.
        throw CString ("No connection was selected.\n"
            "Use Setting->Communication prefernces menu\n"
            "to select a connection and try again.");
        return;
    }

    //
    //  Release previous interface.
    //
    if (m_pIDP) 
    {    
        CloseSession();     // Just to make sure
        m_pIDP->Release();  // Release the old interface
        m_pIDP = NULL;      // Prepare for a failure
    }

    //
    //  Get a new interface.
    //

    // Retrieve a DirectPlay 1.0 interface.
    hr = DirectPlayCreate(&guidCurr, &lpDirectPlay1, NULL);
    if FAILED(hr) {
        TRACE("DirectPlayCreate failed HR=%ul\n", hr);
        throw CString("Communication failure.\n(DirectPlayCreate failed)");
        return;
    }
    
    // Query for an ANSI DirectPlay2 interface.
    hr = lpDirectPlay1->QueryInterface(IID_IDirectPlay2A, (LPVOID *) &m_pIDP);

    // Release DirectPlay 1.0 interface
    if (lpDirectPlay1)
        lpDirectPlay1->Release();

    if FAILED(hr) {
        TRACE("QueryInterface for IID_IDirectPlay2A failed HR=%ul\n", hr);
        throw CString("Communication failure.\n"
            "(QueryInterface failed)");
        return;
    }
}

void
CCommManager::OpenSession()
{
    HRESULT hr;
    DPSESSIONDESC2 SessionDesc;

    // Close any exiting session.
    if (m_fIsConnected)
        CloseSession();

    ZeroMemory(&SessionDesc, sizeof(DPSESSIONDESC2));
    SessionDesc.dwSize = sizeof(DPSESSIONDESC2);
    SessionDesc.guidApplication = TANKS_GUID;
    
    // Get user choice from registry:
    m_fIsHost = TANKS_APP->GetStoredIsHostFlag();
    //
    //  Open session:
    //
    if (m_fIsHost) 
    {    // Create a new session
        CString strName = TANKS_APP->GetStoredPlayerName();

        SessionDesc.dwFlags =   DPSESSION_KEEPALIVE         | 
                                DPSESSION_NODATAMESSAGES;
        // We add one for our Host that occupies an additional player.
        SessionDesc.dwMaxPlayers = MAX_TANKS + 1;   
        SessionDesc.lpszSessionNameA = const_cast<LPSTR>((LPCSTR)strName);
        hr = m_pIDP->Open(&SessionDesc, DPOPEN_CREATE);
        if FAILED(hr) {
            throw CString("Cannot create new session");
            return;
        }
        // Create the host object:
        m_CSHost.Lock();
        m_pHost = new CHost(*this);
        if (! m_pHost) 
        {
            m_CSHost.Unlock();
            throw CString("Cannot create new session");
            return;
        }
        m_CSHost.Unlock();
    } 
    else 
    {
        //
        // Let user select a session.
        //
        DPSESSIONDESC2 dpDesc;

        ZeroMemory(&dpDesc, sizeof(DPSESSIONDESC2));
        dpDesc.dwSize = sizeof(DPSESSIONDESC2);
        dpDesc.guidApplication = TANKS_GUID;

        // Bring the select session dialog.
        CEnumSession dlg(m_pIDP, &dpDesc.guidInstance);
        if (IDOK != dlg.DoModal())
        {   // User aborted or EnumSession failed.
            throw CString("No session selected");
            return;
        }
        // Join the session:
        hr = m_pIDP->Open(&dpDesc, DPOPEN_JOIN);
        if FAILED(hr) {
            throw CString("Cannot join session");
            return;
        }
    }

/*  The remarked code was used to check if its a TCP/IP connection.
    // Check the session is supporting guarentied delivery:
    ZeroMemory(&m_SessionCaps, sizeof(DPCAPS));
    m_SessionCaps.dwSize = sizeof(DPCAPS);
    hr = m_pIDP->GetCaps(&m_SessionCaps, DPGETCAPS_GUARANTEED);

    if (FAILED(hr) || 
        ! (m_SessionCaps.dwFlags & DPCAPS_GUARANTEEDSUPPORTED)) {
        // We don't support the connection chosen by user:
        m_pIDP->Close();
        throw CString("The connection selected isn't supported.\n"
            "Select another connection\nand try again");
        return;
    }
*/
    m_fIsConnected = TRUE;
    SET_SESSION_START_TIME (GetTickCount());
}

void
CCommManager::CloseSession()
{
    HRESULT hr;

    if (m_fIsConnected) {

        m_fIsConnected = FALSE;

        if (m_fIsHost) {
            m_pIDP->DestroyPlayer(m_idHost);
            ASSERT (m_pHost);
            m_CSHost.Lock();
            delete m_pHost;
            m_pHost = NULL;
            m_CSHost.Unlock();
        }
        m_idHost = INVALID_PLAYER_ID;

        hr = m_pIDP->Close();

        if FAILED(hr)
        {
            TRACE("DirectPlay Close() session failed HR=%ul\n", hr);
            throw CString("Communication failure.\nFailed to close session");
            return;
        }

        TRACE_NETWORK_STATS;
    }
}

void
CCommManager::CreatePlayer()
{
    HRESULT hr;
    DPNAME dpName;

    ASSERT (! m_fIsPlaying);    // We shouldn't be here if we are playing!!

    // Check the RecvEvent handle:
    if (! m_hRecvEvent) {
        throw CString("Cannot create player:\nReceive event is null");
        return;
    }

    // Create host's player object:
    if (m_fIsHost) {
        ZeroMemory (&dpName, sizeof(DPNAME));
        dpName.dwSize = sizeof(DPNAME);
        dpName.lpszShortNameA = "Host";
    
        hr = m_pIDP->CreatePlayer(&m_idHost, &dpName, m_hRecvEvent, NULL, 0, 0);

        if FAILED(hr) {
            TRACE ("CreatePlayer failed HR=%ul\n", hr);
            throw CString("Communication failure.\nCannot create host player");
            return;
        }
    }

    // Get user name:
    CString strPlayerName = TANKS_APP->GetStoredPlayerName();

    ZeroMemory (&dpName, sizeof(DPNAME));
    dpName.dwSize = sizeof(DPNAME);
    dpName.lpszLongNameA = 
        dpName.lpszShortNameA = const_cast<char*>((LPCSTR)strPlayerName);
    // Try to create player object:    
    hr = m_pIDP->CreatePlayer(&m_idPlayer, &dpName, m_hRecvEvent, NULL, 0, 0);

    if FAILED(hr) {
            TRACE ("CreatePlayer failed HR=%ul\n", hr);
            throw CString("Communication failure.\n"
                "Cannot create client player");
        return;
    }

    m_pIDP->SetPlayerData (
        m_idPlayer, 
        dpName.lpszLongNameA, 
        strPlayerName.GetLength() + 1, 
        DPSET_REMOTE | DPSET_GUARANTEED);

    m_fIsPlaying = TRUE;
}

void
CCommManager::DestroyPlayer()
{
    if (m_fIsPlaying) {
        m_fIsPlaying = FALSE;

        m_pIDP->DestroyPlayer(m_idPlayer);

        m_idPlayer = INVALID_PLAYER_ID;
    }
}

/*------------------------------------------------------------------------------

  Function: ThreadEntry

  Purpose:  Main message loop. Thread is waiting for signals from outgoing and
            incoming queues and when signaled delivers messages to their destination.

  Input:    None.

  Output:   None.

  Remarks:  Incoming messages are dispatched to host or client objects to be
            processed.
            All outgoing messages are sent to the host machine.

------------------------------------------------------------------------------*/
UINT 
CCommManager::ThreadEntry (LPVOID /*pParam*/)
{
#define WAIT_OBJECT_1   (WAIT_OBJECT_0 + 1)
#define WAIT_OBJECT_2   (WAIT_OBJECT_1 + 1)

    HRESULT hr;
    HANDLE  aHandles[3];

    // Check the QuitEvent handle:
    if (! m_hQuitEvent) {
        ASSERT(m_hQuitEvent);
        return 1;
    }

    // Attempt to create the enqueue event:
    HANDLE hEnqueueEvent = CreateEvent (NULL, FALSE, FALSE, NULL);
    if (! hEnqueueEvent) {
        ASSERT(hEnqueueEvent);
        return 1;
    }
    
    // Ask the incoming message queue to set the enqueue event on every enqueue.
    m_OutgoingMsgQ.SetNotificationEvent (hEnqueueEvent);

    aHandles[0] = m_hRecvEvent;
    aHandles[1] = hEnqueueEvent;
    aHandles[2] = m_hQuitEvent;

    SetPriority (COMM_MANAGER_THREAD_PRIORITY);

    while (! m_bEndThread) {
        DWORD nResult = WaitForMultipleObjects(3, aHandles, FALSE, INFINITE);

        switch (nResult) {
        case WAIT_OBJECT_0: // new message arrived (from the network):
            {
                DWORD dwMsgCount = 0;
                DWORD dwHostMsgCount = 0;
                // Get messages for both players:
                hr = m_pIDP->GetMessageCount(m_idPlayer, &dwMsgCount);
                if FAILED(hr)
                    break;
                if (m_fIsHost)
                {   // Also host message may exist
                    hr = m_pIDP->GetMessageCount(m_idHost, &dwHostMsgCount);
                    if FAILED(hr)
                        break;
                }
                for (UINT i = 0; i < dwHostMsgCount + dwMsgCount; i++) {
                    m_Message.m_dwLength = CCommMessage::MAX_BUFFER_LENGTH;
                    hr = m_pIDP->Receive(&m_idFrom, &m_idTo, DPRECEIVE_ALL,
                        m_Message.m_aBuffer, &m_Message.m_dwLength);
                    if SUCCEEDED(hr) 
                    {
                        if (m_idTo == m_idPlayer)
                        {   // New message for the player (client)
                            HandleClientMessage ();
                            INC_TOTAL_CLIENT_MESSAGES_RECEIVED;
                        }
                        else if (m_fIsHost && m_idTo == m_idHost)
                        {   // New message for the host
                            HandleHostMessage();
                            INC_TOTAL_HOST_MESSAGES_RECEIVED;
                        }
                        else
                            ASSERT (FALSE);

                        INC_TOTAL_BYTES_RECEIVED (m_Message.m_dwLength);
                        UPDATE_BIGGEST_PACKET_RECEIVED (m_Message.m_dwLength);
                        UPDATE_SMALLEST_PACKET_RECEIVED(m_Message.m_dwLength);
                        UPDATE_RECEIVE;
                    }   // End of successful message received
                    else
                    {
                        INC_RECEIVE_ERRORS;
                    }
                } 
            }
            break;

        case WAIT_OBJECT_1: // new message to send (from the outgoing queue):
            {   
                CMessage msg;
                // Empty outgoing queue:
                for (int count = m_OutgoingMsgQ.GetQueueSize(); count > 0; count--)
                {
                    // Get uncompressed message:
                    if (! m_OutgoingMsgQ.Dequeue(msg))  
                        // TODO: although we were signaled after enqueue, we still encounter
                        // empty Q occasionly.
                        break;
                    // Send message over net to game host:
                    m_Message.m_dwLength = msg.GetBuffer(m_Message.m_aBuffer);

#if defined (NET_MON_SYNC) || defined (NET_MON_GAME)
                    if (msg.GetType() != CMessage::CHECK_SUM)   // Not checksum message
                    {
                        NET_GAME_TRACE (("Client sending to host %s, size=%d (msg time = %d), game time = %d",
                                        msg.GetName(), m_Message.m_dwLength, msg.GetTime(), 
                                        TANKS_APP->m_gTimer.GetLocalTime()))
                    }
                    else
                    {
                        NET_SYNC_TRACE (("Client sending checksum to host, size=%d (game time = %d)",
                                         m_Message.m_dwLength, msg.GetTime()));
                    }
#endif // defined (NET_MON_SYNC) || defined (NET_MON_GAME)

                    HRESULT hr = m_pIDP->Send( m_idPlayer,
                                               m_idHost, 
                                               DPSEND_GUARANTEED,
                                               m_Message.m_aBuffer,
                                               m_Message.m_dwLength);
                    if (!SUCCEEDED(hr))
                    {   // Can't send message
                        TRACE ("\tERROR: Client cannot send message to host !!!\n");
                        INC_CLIENT_SEND_ERRORS;
                    }
                    else
                    {   // Message sent successfully
                        INC_TOTAL_BYTES_SENT (m_Message.m_dwLength);
                        INC_TOTAL_CLIENT_MESSAGES_SENT;
                        UPDATE_BIGGEST_PACKET_SENT (m_Message.m_dwLength);
                        UPDATE_SMALLEST_PACKET_SENT (m_Message.m_dwLength);
                        UPDATE_SEND;
                    }
                }   // End while
            }   // End case block
            break;

        case WAIT_OBJECT_2: // we should quit thread:
            m_bEndThread = TRUE;
            break;

        case WAIT_FAILED: {
            CString strErr;

            strErr.Format("CommManager thread failed with error code: %08X",
                GetLastError());
            AfxMessageBox(strErr);
                          }
        default:
            m_bEndThread = TRUE;
        }
    }   // end of major comm. manager loop

    // Ask the incoming message queue to stop setting the enqueue event on every enqueue.
    m_OutgoingMsgQ.SetNotificationEvent (NULL);
    // Terminate the enqueue event
    CloseHandle (hEnqueueEvent);

    return 0;
}

void
CCommManager::HandleClientMessage()
{
    // First lets take care of system msgs, such as NEWPLAYER..
    if (DPID_SYSMSG==m_idFrom) 
    {
        switch (((DPMSG_GENERIC*)&m_Message.m_aBuffer[0])->dwType) 
        {
            case DPSYS_CREATEPLAYERORGROUP: // a new player is joining
                // Store player name in the array (where ?)
                NET_SYS_TRACE (("Client got Create player or group (player ID = %d)",
                    ((DPMSG_CREATEPLAYERORGROUP*)m_Message.m_aBuffer)->dpId));
                break;
            case DPSYS_DESTROYPLAYERORGROUP:
                if (((DPMSG_DESTROYPLAYERORGROUP*)&m_Message.m_aBuffer[0])->dpId == m_idHost)   
                {   // this means the host closed the session
                    DisplayMessageAndEndGame (IDS_LOST_SEVER_CONNECTION);
                }
                NET_SYS_TRACE (("Client got Destroy player or group (player id = %d",
                                ((DPMSG_DESTROYPLAYERORGROUP*)&m_Message.m_aBuffer[0])->dpId));
                break;
            case DPSYS_SESSIONLOST: // We can't proceed playing - stop game:
                DisplayMessageAndEndGame (IDS_LOST_SEVER_CONNECTION);
                NET_SYS_TRACE (("Client got session lost"));
                break;
            default:
                // Disregard other DirectPlay system messages
                NET_SYS_TRACE (("Client got unhandled message (Msg ID = %d)", 
                                ((DPMSG_GENERIC*)&m_Message.m_aBuffer[0])->dwType));
                break;

        }
        return;
    }

    m_pClient->HandleMessage();
}

    
void
CCommManager::HandleHostMessage()
{
    // First lets take care of system msgs, such as NEWPLAYER..
    if (DPID_SYSMSG==m_idFrom) {
        switch (((DPMSG_GENERIC*)&m_Message.m_aBuffer[0])->dwType) 
        {
            case DPSYS_CREATEPLAYERORGROUP: 
                { // a new player is joining
                    DPID idFrom = ((DPMSG_CREATEPLAYERORGROUP*)m_Message.m_aBuffer)->dpId;

                    NET_SYS_TRACE (("Server got Create player or group (player ID = %d)",idFrom));

                    if (idFrom==m_idHost)   // We received new player about our own object
                        return;
                    // Respond with ACK:
                    m_Message.m_aBuffer[0] = ACK;
                    m_Message.m_dwLength = 1;
                    SendAsHost(idFrom); // As host
                }
                break;
            case DPSYS_DESTROYPLAYERORGROUP: 
                { // a player just left the session:
                    DPID idFrom = ((DPMSG_CREATEPLAYERORGROUP*)m_Message.m_aBuffer)->dpId;
                    NET_SYS_TRACE (("Server got Destroy player or group (player ID = %d)",idFrom));
                    // Notify other players ?
                    // Remove the ID from the array:
                    m_CSHost.Lock();
                    m_pHost->RemovePlayerFromArray(idFrom);
                    m_CSHost.Unlock();
                }
                break;
            case DPSYS_SESSIONLOST: // We can't proceed playing - stop game:
                NET_SYS_TRACE (("Server got Session lost"));
                DisplayMessageAndEndGame (IDS_LOST_SEVER_CONNECTION);
                break;
            default:
                // Disregard other DirectPlay system messages
                NET_SYS_TRACE (("Server got unhandled message (Msg ID = %d)", 
                                ((DPMSG_GENERIC*)&m_Message.m_aBuffer[0])->dwType));
                break;
        }
        return;
    }
    m_CSHost.Lock();
    m_pHost->HandleMessage( m_idFrom,               // Send the player ID of the sender
                            m_idFrom == m_idPlayer);// If this is the local player, it's the judge
    m_CSHost.Unlock();
}

void
CCommManager::SendAsHost(DPID idTo)
{
    ASSERT (m_pIDP);
    
    // Always called by host:
    NET_GAME_TRACE (("Host sending %s to player ID %d (size=%d) at time %lu, msg.time=%lu",
                    CMessage::GetName (m_Message.m_aBuffer[0]),
                    idTo,
                    m_Message.m_dwLength,
                    TANKS_APP->m_gTimer.GetLocalTime(),
                    *((PDWORD)&m_Message.m_aBuffer[1])));
    HRESULT hr = m_pIDP->Send( m_idHost,
                               idTo, 
                               DPSEND_GUARANTEED,
                               m_Message.m_aBuffer,
                               m_Message.m_dwLength);
    if (!SUCCEEDED(hr))
    {   // Can't send message
        TRACE ("\tERROR: Host cannot send message to client !!!\n");
        INC_HOST_SEND_ERRORS;
    }
    else
    {   // Message sent successfully
        INC_TOTAL_BYTES_SENT (m_Message.m_dwLength);
        INC_TOTAL_HOST_MESSAGES_SENT;
        UPDATE_BIGGEST_PACKET_SENT (m_Message.m_dwLength);
        UPDATE_SMALLEST_PACKET_SENT (m_Message.m_dwLength);
        UPDATE_SEND;
    }
}

void 
CCommManager::NotifyBonusEaten (UINT uTankID)
{   
    // This function is called by a tank object in this machine.
    // It is called only if we're the host machine => the prime judge of the game.
    // A tank eats a bonus and wishes to notify all the players (including himself)
    // of this happy event.

    ASSERT (IsHost());  // Only a tank on a host machine can issue this call
    
    CMessage::MessageData Params;
        // Create bonus eating notification message:
    Params.BonusParam.Type = BONUS_NONE; // Bonus is removed, not added
    Params.BonusParam.LifeSpan = uTankID;
        // Send it to the comm. managers outgoing queue - it'll spread to all from there.
        // The host player will receive it and will spread it to all players.
    TANKS_APP->m_gOutgoingMsgQueue.Enqueue (CMessage::ADD_BONUS, Params);
}

void 
CCommManager::NotifyNewBonus (BonusType typ, DWORD ttl, CPoint &loc)
{   
    // This function is called by the game manager in the host machine.
    // It is called only if we're the host machine => the prime judge of the game.
    // It's time to tell all of the game managers (including the local one)
    // that a new bonus is added.

    ASSERT (IsHost());  // Only the game manager on a host machine can issue this call
    
    CMessage::MessageData Params;
        // Create bonus eating notification message:
    Params.BonusParam.Type = typ;
    Params.BonusParam.LifeSpan = ttl / 1000;    // Convert ttl to seconds
    Params.BonusParam.XPos = WORD(loc.x);
    Params.BonusParam.YPos = WORD(loc.y);
        // Send it to the comm. managers outgoing queue - it'll spread to all from there.
        // The host player will receive it and will spread it to all players.
    TANKS_APP->m_gOutgoingMsgQueue.Enqueue (CMessage::ADD_BONUS, Params);
}

void 
CCommManager::NotifyExplodingTank (UINT uTankID)
{   // This function is called by a tank object in this machine.
    // It is called only if we're the host machine => the prime judge of the game.
    // A tank reaches the conclusion he's dying now wishes to notify all the 
    // players (including himself) of this sad event.

//    ASSERT (IsHost());  // Only a tank on a host machine can issue this call
    
    CMessage::MessageData Params;
        // Create tank removal notification message:
    Params.TankRemoveParam.ID = BYTE(uTankID);
    Params.TankRemoveParam.Explode = TRUE;
        // Send it to the comm. managers outgoing queue - it'll spread to all from there
        // The host player will receive it and will spread it to all players.
    TANKS_APP->m_gOutgoingMsgQueue.Enqueue (CMessage::REMOVE_TANK, Params);
}

void 
CCommManager::NotifyCheckSum (CMessage::MessageData &msgdata)
{   // This function is called by a the game manager in this machine.
    // It is called when the game manager wishes to send a game check sum
    m_OutgoingMsgQ.Enqueue (CMessage::CHECK_SUM, msgdata);
    // We take advantage of the fact that this method is called every sec.
    // to trigger the zombie check:
    if (m_pHost)
        m_pHost->ChkForZombies();
}

void
CCommManager::NotifyChatMsg (LPCSTR szMsg)
{
    CMessage::MessageData Params;
    strcpy (Params.ChatParam.Msg, szMsg);
    Params.ChatParam.TankID = BYTE(TANKS_APP->m_gGameManager.GetLocalTankID());
    Params.ChatParam.idFrom = m_idPlayer;
    m_OutgoingMsgQ.Enqueue (CMessage::CHAT, Params);
}


void 
CCommManager::DisplayMessageAndEndGame (UINT uMsgID)
{
    if (m_bLostServerConnection)
        return;
    m_bLostServerConnection = TRUE;
    if (0 != uMsgID)
        AfxMessageBox (uMsgID, MB_OK | MB_ICONHAND);
            // Simulate a user menu selection of "Stop Game":
    ::PostMessage(TANKS_APP->m_pMainWnd->m_hWnd, WM_COMMAND, ID_STOP_GAME, 0);
}

BOOL 
CCommManager::GetPlayerInfo (
                             UINT ind, 
                             BOOL &bIsJudge, 
                             CString &cstrName, 
                             DWORD &dwDuration, 
                             DWORD &dwRTT)
{
    m_CSHost.Lock();
    BOOL bRes = FALSE;
    if (m_pHost)
    {   // Server active
        bRes = m_pHost->GetPlayerInfo (ind, dwDuration, dwRTT);
        if (bRes)
        {   // Successfully queried server
            DWORD dwDPID = m_pHost->GetDirectPlayID (ind);
            bIsJudge = (m_idPlayer == dwDPID);
            char szBuf[1024];
            DWORD dwSize = 1024;
            HRESULT hres = m_pIDP->GetPlayerData (dwDPID, szBuf, &dwSize, 0);
            cstrName = (DP_OK == hres) ? szBuf : "";
        }
    }
    m_CSHost.Unlock();
    return bRes;
}

void
CCommManager::GetPlayerName (DPID id, CString& cstrName)
{
    m_CSHost.Lock();
    char szBuf[256];
    DWORD dwSize = 256;
    HRESULT hres = m_pIDP->GetPlayerData (id, szBuf, &dwSize, 0);
    cstrName = (DP_OK == hres && dwSize) ? szBuf : "Unknown";
    m_CSHost.Unlock();
}

BOOL CCommManager::KillPlayer (UINT uTankID)
{
    BOOL bRes = FALSE;
    m_CSHost.Lock();
    if (m_pHost)
    {
        ASSERT (uTankID < MAX_TANKS);
        NotifyExplodingTank (uTankID);
        bRes = TRUE;
    }
    m_CSHost.Unlock();
    return bRes;
}

#ifdef GATHER_NETWORK_STATS

void 
CCommManager::UpdateReceive ()
{
    DWORD dwCurTime = GetTickCount();
    DWORD dwWindowSize = dwCurTime - m_dwLastSecTimeRecv;
    if (dwWindowSize < 1000)
        return;
    m_dwLastSecTimeRecv = dwCurTime;
    DWORD dwAmountInWindow = m_dwTotalBytesReceived - m_dwLastRecvTotal;
    m_dwLastRecvTotal = m_dwTotalBytesReceived;
    DWORD dwBPS = DWORD(double(dwAmountInWindow) * double(1000.0) / double(dwWindowSize));
    m_dwMaxBytesReceivedInLastSecond = max (m_dwMaxBytesReceivedInLastSecond, dwBPS);
}

void 
CCommManager::UpdateSend ()
{
    DWORD dwCurTime = GetTickCount();
    DWORD dwWindowSize = dwCurTime - m_dwLastSecTimeSend;
    if (dwWindowSize < 1000)
        return;
    m_dwLastSecTimeSend = dwCurTime;
    DWORD dwAmountInWindow = m_dwTotalBytesSent - m_dwLastSentTotal;
    m_dwLastSentTotal = m_dwTotalBytesSent;
    DWORD dwBPS = DWORD(double(dwAmountInWindow) * double(1000.0) / double(dwWindowSize));
    m_dwMaxBytesSentInLastSecond = max (m_dwMaxBytesSentInLastSecond, dwBPS);
}

void
CCommManager::TraceNetworkStats ()
{
    TRACE ( "\n\n\t\t\t**** Network statistics ****\n");

    DWORD dwNow = GetTickCount();
    DWORD dwTotTime = dwNow - m_dwSessionStartTime;

    TRACE ( "Session time = %.3f secs.\n", 
                double(dwTotTime) / 1000.0);

    TRACE ( "Send:\n");

    DWORD dwTotMsgs = m_dwTotalClientMsgsSent + m_dwTotalHostMsgsSent;

    TRACE ( "\tTotal bytes sent = %d (biggest packet size = %d, smallest packet size = %d, avg size = %d)\n",
                m_dwTotalBytesSent, 
                m_dwBiggestSentPacketSize, 
                m_dwSmallestSentPacketSize,
                dwTotMsgs ? DWORD(double(m_dwTotalBytesSent) / double(dwTotMsgs)) : 0);

    TRACE ( "\tTotal messages sent = %d (client messages = %d, host messages = %d)\n",
                dwTotMsgs, 
                m_dwTotalClientMsgsSent, 
                m_dwTotalHostMsgsSent);

    TRACE ( "\tTotal send errors = %d (client errors = %d, host errors = %d)\n",
                m_dwClientSendErrors + m_dwHostSendErrors, 
                m_dwClientSendErrors, 
                m_dwHostSendErrors);

    TRACE ( "\tMaximum bytes sent per second = %d (avg = %d)\n",
                m_dwMaxBytesSentInLastSecond,
                dwTotTime ? DWORD(double(m_dwTotalBytesSent) / (double(dwTotTime) / double(1000.0))) : 0);

    TRACE ( "Receive:\n");

    dwTotMsgs = m_dwTotalClientMsgsReceived + m_dwTotalHostMsgsReceived;

    TRACE ( "\tTotal bytes received = %d (biggest packet size = %d, smallest packet size = %d, avg size = %d)\n",
                m_dwTotalBytesReceived, 
                m_dwBiggestReceivedPacketSize, 
                m_dwSmallestReceivedPacketSize,
                dwTotMsgs ? DWORD(double(m_dwTotalBytesReceived) / double(dwTotMsgs)) : 0);               

    TRACE ( "\tTotal messages received = %d (client messages = %d, host messages = %d)\n",
                dwTotMsgs, 
                m_dwTotalClientMsgsReceived, 
                m_dwTotalHostMsgsReceived);

    TRACE ( "\tTotal receive errors = %d\n",
                m_dwReceiveErrors);

    TRACE ( "\tMaximum bytes received per second = %d (avg = %d)\n\n",
                m_dwMaxBytesReceivedInLastSecond,
                dwTotTime ? DWORD(double(m_dwTotalBytesReceived) / (double(dwTotTime) / double(1000.0))) : 0);
}


#endif  // GATHER_NETWORK_STATS
