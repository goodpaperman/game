/*****************************************************************************
*                                                                             
*   MsgQueue.inl
*                                                                             
*   Electrical Engineering Faculty - Software Lab                             
*   Spring semester 1998                                                      
*                                                                             
*   Tanks game                                                                
*                                                                             
*   Contents: Inline functions implementations.
*                                                                             
*   Authors: Eran Yariv - 28484475                                           
*            Moshe Zur  - 24070856                                           
*                                                                            
*                                                                            
*   Date: 23/09/98                                                           
*                                                                            
******************************************************************************/
inline BOOL
CMsgQueue::Enqueue (CMessage *pMsg, BOOL bSorted)
{
    ASSERT (pMsg->GetType() < CMessage::NUM_MSGS);
    return bSorted ? CQueue::Enqueue(pMsg, pMsg->GetTime()) : CQueue::Enqueue(pMsg);
}
    
inline BOOL 
CMsgQueue::Enqueue (CMessage::MessageType MsgType, CMessage::MessageData& MsgUnion, GameMessageType t)
{
    ASSERT (MsgType < CMessage::NUM_MSGS);
    CMessage *pMsg = new CMessage(MsgType, MsgUnion, t);
    ASSERT(pMsg);
    if (!pMsg)
    {
        AfxMessageBox(IDS_OUTOFMEMORY_ERR, MB_ICONSTOP | MB_OK, 0);
        abort();
    }
    return Enqueue(pMsg);
}

inline BOOL 
CMsgQueue::Dequeue (CMessage& Msg)
{
    CMessage *pDeadMsg;

    if (CQueue::Dequeue((CObject*&)pDeadMsg)) {
        // Copy message:
        Msg.SetType(pDeadMsg->GetType());
        ASSERT (Msg.GetType() < CMessage::NUM_MSGS);
        Msg.SetTime(pDeadMsg->GetTime()); 
        memcpy (&Msg.m_UnionData, &(pDeadMsg->m_UnionData), sizeof(CMessage::MessageData));
        // It's OK now to free the dead message:
        delete pDeadMsg;
        return TRUE;
    }
    return FALSE;
}
