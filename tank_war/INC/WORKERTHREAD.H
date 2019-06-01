/*****************************************************************************
*                                                                             
*   WorkerThread.h
*                                                                             
*   Electrical Engineering Faculty - Software Lab                             
*   Spring semester 1998                                                      
*                                                                             
*   Tanks game                                                                
*                                                                             
*   Module description: Interface for the worker thread base class.
*                       
*                                                                             
*   Authors: Eran Yariv - 28484475                                           
*            Moshe Zur  - 24070856                                           
*                                                                            
*                                                                            
*   Date: 23/09/98                                                           
*                                                                            
******************************************************************************/
#ifndef _WORKER_THREAD_H
#define _WORKER_THREAD_H

class CWorkerThread
{
public:
    CWorkerThread ();
    virtual ~CWorkerThread ();

    BOOL SetPriority (int prio);

protected:
    void StartWorkerThread (LPVOID pvParam = 0);
    void EndWorkerThread (BOOL bWaitForTermination);

    static UINT     InvokeThreadEntry (LPVOID pParam);  // Used as shell only, to invoke the thread
    virtual UINT    ThreadEntry (LPVOID pParam = 0) = 0;

    LPVOID          m_pvParam;               // Hold param for thread proc.
    BOOL            m_bEndThread;            // Should the thread die? 
    
private:
    HANDLE m_hThread;
    BOOL   m_bThreadIsRunning;      // Is the thread running?
};           
        

// Inline sections:
#include <WorkerThread.inl>

#endif
