/*****************************************************************************
*                                                                             
*   WorkerThread.inl
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
inline
CWorkerThread::CWorkerThread () : m_bThreadIsRunning(FALSE)
    {}

inline
CWorkerThread::~CWorkerThread () 
{
    if (m_bThreadIsRunning) {
        EndWorkerThread(TRUE);
    }
}

inline BOOL
CWorkerThread::SetPriority (int prio)
{
    if (!m_bThreadIsRunning)
        return FALSE;   // Thread not running
    return SetThreadPriority (m_hThread, prio);
}
