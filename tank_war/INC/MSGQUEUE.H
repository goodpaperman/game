/*****************************************************************************
*                                                                             
*   MsgQueue.h
*                                                                             
*   Electrical Engineering Faculty - Software Lab                             
*   Spring semester 1998                                                      
*                                                                             
*   Tanks game                                                                
*                                                                             
*   Module description: Implements the message queues used between the game
*                       manager and the communication manager.
*                       
*                                                                             
*   Authors: Eran Yariv - 28484475                                           
*            Moshe Zur  - 24070856                                           
*                                                                            
*                                                                            
*   Date: 23/09/98                                                           
*                                                                            
******************************************************************************/
#ifndef _MESSAGE_QUEUE_H_
#define _MESSAGE_QUEUE_H_

#include "queue.h"
#include "GameObject.h"
#include "Message.h"
#include "resource.h"

class CMsgQueue : public CQueue
{
public:
    CMsgQueue (BOOL bBlocking = TRUE) : CQueue(bBlocking) {}

    // Enqueue an already allocated message:
    BOOL Enqueue (CMessage *pMsg, BOOL bSorted = FALSE);
    // Alloc new message and insert into Q
    BOOL Enqueue (CMessage::MessageType MsgType, CMessage::MessageData& MsgUnion, GameMessageType = PLAYER_MSG);
    // Get message from Q, copy to pMsg, and delete message:
    BOOL Dequeue (CMessage& pMsg);
};

#include "MsgQueue.inl"

#endif