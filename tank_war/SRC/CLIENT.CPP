/*****************************************************************************
*                                                                             
*   Client.cpp                                                            
*                                                                             
*   Electrical Engineering Faculty - Software Lab                             
*   Spring semester 1998                                                      
*                                                                             
*   Tanks game                                                                
*                                                                             
*   Module description: Implements the client handling of network messages.
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
#include "Client.h"
#include "TanksDlg.h"

CClient::CClient (CCommManager &CommManager) : 
    m_CommManager(CommManager),
    m_IncomingMsgQ(TANKS_APP->m_gIncomingMsgQueue),
    m_GameManager(TANKS_APP->m_gGameManager),
    m_Message(CommManager.ExposeMessage())
{
    m_hHostIDEvent          = CreateEvent (NULL, FALSE, FALSE, NULL);
    m_hAddLocalTankEvent    = CreateEvent (NULL, FALSE, FALSE, NULL);
    m_hAddBoardEvent        = CreateEvent (NULL, FALSE, FALSE, NULL);
}

CClient::~CClient ()
{
    CloseHandle (m_hHostIDEvent);
    CloseHandle (m_hAddLocalTankEvent);
    CloseHandle (m_hAddBoardEvent);
}

/*------------------------------------------------------------------------------

  Function: WaitForGameProperties

  Purpose:  Called when the client begins a new game, to negotiate with the game
            host and retrieve the game properties - player's tank ID and game 
            board properties.

  Input:    None.

  Output:   Return TRUE on success.

  Remarks:  The DirectPlay::CreatePlayer method is send to the game host w/o 
            knowing it's DirectPlay player ID. The "protocol" we are using to 
            negotiate w/ the host for the game properties is also responsible
            for generating the host player ID (to be used when sending messages)
------------------------------------------------------------------------------*/
BOOL
CClient::WaitForGameProperties()
{
    // Wait for Host ID to arrive as response to our CreatePlayer msg:
    int nResponse = WaitForSingleObject(m_hHostIDEvent, COMM_TIME_OUT);

    if (WAIT_OBJECT_0 != nResponse)
        return FALSE;
        // At this stage we know that m_idHost is valid.
    // Send Request for Tank ID
    CMessage::MessageData Params;
        // Request new tank:
    UINT uTankID = TANKS_APP->GetStoredPreferedTankID();
    Params.TankRequestParam.bID = BYTE(uTankID);

    TANKS_APP->m_gOutgoingMsgQueue.Enqueue (CMessage::REQUEST_TANK, Params);

    // Wait for AddTank and AddBoard to arrive
    HANDLE aHandles[2] = { m_hAddLocalTankEvent, m_hAddBoardEvent };

    nResponse = WaitForMultipleObjects (2, aHandles, TRUE, COMM_TIME_OUT);
    return (WAIT_TIMEOUT != nResponse);
}

/*------------------------------------------------------------------------------

  Function: HandleMessage

  Purpose:  Handles the messages send from the game host to our client.

  Input:    None.

  Output:   None.

  Remarks:  1. The message is stored in the CommManager to reduce stack 
               operations.
            2. We treat a client on the host machine the same as a remote 
               client (thanks to the help of DirectPlay abstraction...)
------------------------------------------------------------------------------*/
void
CClient::HandleMessage()
{
    // Check if that's our host saying ACK:
    if ((1==m_Message.m_dwLength) && (CCommManager::ACK==m_Message.m_aBuffer[0])) 
    {   // Notify our CommManager of the Host ID (current m_idFrom):
        m_CommManager.SetHostID();
        // We can release the WaitForGameProperties method from it's wait:
        SetEvent(m_hHostIDEvent);
        NET_GAME_TRACE (("Client got ping from host"));
        // Nothing else left to do here, the rest is done in GetGameProperties:
        return;
    }

    if (CMessage::SET_MINES == m_Message.m_aBuffer[0])
    {   // Special case: host sending mines per sector information - variable
        // lenght message that may be very big !!!
        DWORD AllMinesInSector[MAX_MINES_PER_SECTOR];
        int iSector;

        // Retrieve array of positions with the data of all the existing mines 
        // in the given sector:
        DWORD dwMinesFound = CMessage::DecodeSectorMines (&(m_Message.m_aBuffer[1]),
                                                          m_Message.m_dwLength - 1,
                                                          iSector, 
                                                          AllMinesInSector);
        // Tell the local game manager the correct mines setting in the sector.
        // Do it now - don't use any queue: this message if it was put in a queue,
        // would get very big - we don't want to waste that space.
        m_GameManager.SetMinesInSector (iSector, dwMinesFound, AllMinesInSector);
        return;
    }

    CMessage *pMsg =  new CMessage(m_Message.m_aBuffer, 
                        (BYTE)m_Message.m_dwLength, PLAYER_MSG);
    ASSERT(pMsg);
    if (!pMsg)
    {
        AfxMessageBox(IDS_OUTOFMEMORY_ERR, MB_ICONSTOP | MB_OK, 0);
        abort();
    }
    switch (pMsg->GetType())
    {   // Catch special messages and handle accordingly:
        case CMessage::ADD_BOARD:
            // We should catch the 2 initializing messages, 
            // to end GetGameProperties:
            NET_GAME_TRACE (("Client got ADD_BOARD from host (game time = %d)",
                pMsg->GetTime ()));
            SetEvent(m_hAddBoardEvent);
            break;

        case CMessage::ADD_TANK:
            NET_GAME_TRACE (("Client got ADD_TANK (%s) from host "
                "(game time = %d)",
                pMsg->m_UnionData.TankParam.Local ? "local" : "remote", 
                pMsg->GetTime ()));
            if (pMsg->m_UnionData.TankParam.Local)
                SetEvent(m_hAddLocalTankEvent);
            break;

        case CMessage::MANOUVER_SET:
            NET_GAME_TRACE (("Client got MANOUVER_SET from host"
                "(game time = %d)", pMsg->GetTime ()));
            // We should copy the manouver set of the sending tank to the 
            // global array:
            CopyManouverSet();
            // We don't enqueue the message, so we have delete the 
            // message manually:
            delete pMsg;
            // This message isn't handled by the game manager - 
            // we should return before the enqueue:
            return;

        case CMessage::CHAT:
            // Update chat dialog
            ((CTanksDlg*)TANKS_APP->m_pMainWnd)->MarshalChatMsg(
                pMsg->m_UnionData.ChatParam.idFrom,
                pMsg->m_UnionData.ChatParam.TankID,
                pMsg->m_UnionData.ChatParam.Msg);
            delete pMsg;
            // This message isn't handled by the game manager - 
            // we should return before the enqueue:
            return;

        default:
            NET_GAME_TRACE (("Client got message %s from host "
                            "(game time = %d)", 
                            pMsg->GetName(), pMsg->GetTime ()));
            break;
    }
    // At last, we put the message (as is) in the queue towards the 
    // game manager:
    m_IncomingMsgQ.Enqueue(pMsg, TRUE);
}

void
CClient::CopyManouverSet()
{
    // Copy the manouver set from message to global manouver set array
    CMessage Msg (m_Message.m_aBuffer, (BYTE)m_Message.m_dwLength, PLAYER_MSG);

    // Set manouver set:
    TANKS_APP->m_gManouverSets[Msg.m_UnionData.ManouverSetParam.TankID].SetAll(
        Msg.m_UnionData.ManouverSetParam.ManouverSet);

    // Set last direction:
    TANKS_APP->m_guRemoteTanksDirs[Msg.m_UnionData.ManouverSetParam.TankID] =
        Msg.m_UnionData.ManouverSetParam.Direction;
}

