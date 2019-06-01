/*****************************************************************************
*                                                                             
*   CommManager.inl
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
CCommManager::IsConnected()
{
    return m_fIsConnected;
}

inline BOOL 
CCommManager::IsHost()
{
    return m_fIsHost;
}

inline void
CCommManager::EndThread(BOOL bWait)
{
    VERIFY(SetEvent(m_hQuitEvent));
    EndWorkerThread(bWait);
}

inline CCommMessage& 
CCommManager::ExposeMessage()
{
    return m_Message;
}

inline void 
CCommManager::SetHostID()
{
    // Make sure we don't override existing HostID in case of client:
    ASSERT(m_fIsHost || m_idHost == INVALID_PLAYER_ID);
    m_idHost = m_idFrom;
}
