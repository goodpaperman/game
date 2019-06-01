/*****************************************************************************
*                                                                             
*   Client.h
*                                                                             
*   Electrical Engineering Faculty - Software Lab                             
*   Spring semester 1998                                                      
*                                                                             
*   Tanks game                                                                
*                                                                             
*   Module description: Interface for the client module of the communication
*                       manager.
*                       
*                                                                             
*   Authors: Eran Yariv - 28484475                                           
*            Moshe Zur  - 24070856                                           
*                                                                            
*                                                                            
*   Date: 23/09/98                                                           
*                                                                            
******************************************************************************/
#ifndef _CLIENT_H_
#define _CLIENT_H_

#include "CommManager.h"

class CClient
{
public:
    CClient(CCommManager &CommManager);
    ~CClient();

    void HandleMessage();

    BOOL WaitForGameProperties();

private:

    void CopyManouverSet();

    CCommManager  & m_CommManager;          // Alias to the comm. manager
    CMsgQueue     & m_IncomingMsgQ;         // Alias to the incoming msg queue
    CGameManager  & m_GameManager;          // Alias to the local game manager

    CCommMessage&   m_Message;

    HANDLE          m_hAddLocalTankEvent;
    HANDLE          m_hAddBoardEvent;
    HANDLE          m_hHostIDEvent;
};

#endif