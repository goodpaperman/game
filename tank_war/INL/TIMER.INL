/*****************************************************************************
*                                                                             
*   Timer.inl
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

/****************** CTimer **********************/

inline DWORD 
CTimer::GetRemoteTime ()
{
    return DWORD(LONG(GetLocalTime()) - m_lLastDelta);
}

inline DWORD 
CTimer::GetLocalTime (DWORD dwRemoteTime)
{   // Converts remote time (server's game timer) to local time (client's time)
    // and returns local time    
    return UpdateDelta (dwRemoteTime);
}


inline DWORD
CTimer::GetLocalTime ()
{
    return m_pActualSampleFunction ();
}

/****************** CVanJacobsonTimer **********************/

inline 
CVanJacobsonTimer::CVanJacobsonTimer  (BOOL bForceLowResTimers) :
    CTimer (bForceLowResTimers),
    m_dPrevSRTT (0.0),
    m_dPrevSDEV (0.0)
{}

