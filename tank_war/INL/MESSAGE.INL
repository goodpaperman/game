/*****************************************************************************
*                                                                             
*   Message.inl
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
inline void    
CMessage::SetTime (DWORD dwTime)
{
    m_dwMsgTime = dwTime;
}

inline DWORD   
CMessage::GetTime () const
{
    return m_dwMsgTime;
}

inline CMessage::MessageType 
CMessage::GetType () const
{
    return m_Type;
}

inline void        
CMessage::SetType ( MessageType typ)
{
    m_Type = typ;
}
