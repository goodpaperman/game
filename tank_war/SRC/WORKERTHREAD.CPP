/*****************************************************************************
*                                                                             
*   WorkerThread.cpp                                                            
*                                                                             
*   Electrical Engineering Faculty - Software Lab                             
*   Spring semester 1998                                                      
*                                                                             
*   Tanks game                                                                
*                                                                             
*   Module description: Abstract base class, defining methods to launch and 
*                       stop a worker thread. All worker threads in our
*                       application derive from this class.
*                                                                             
*   Authors: Eran Yariv - 28484475                                           
*            Moshe Zur  - 24070856                                           
*                                                                            
*                                                                            
*   Date: 23/09/98                                                           
*                                                                            
******************************************************************************/
#include <stdafx.h>
#include "WorkerThread.h"


/*------------------------------------------------------------------------------

  Function: StartWorkerThread

  Purpose:  Starts the main worker function - ThreadEntry. We make sure only a
            single instance of the thread is running.

  Input:    Pointer to the thread's params.
  Output:   None.

  Remarks:  Since we want ThreadEntry to be a virtual function, we can't start
            the thread from this function. Instead we call the static function
            InvokeThreadEntry, passing to it the pointer to our class. This way
            we can invoke the dynamically binded thread entry, and pass the 
            params (as class private members).

------------------------------------------------------------------------------*/
void 
CWorkerThread::StartWorkerThread (LPVOID pvParam)
{
    if (m_bThreadIsRunning) // Only one worker thread is handled at a time !
        return;

    m_bEndThread = FALSE;   // This flag is checked by thread, making it TRUE ends thread.
    m_pvParam = pvParam;

    CWinThread* pThread = AfxBeginThread( InvokeThreadEntry, this );

    if (pThread) 
    {
        m_hThread = pThread -> m_hThread;
        m_bThreadIsRunning = TRUE;
    }
}

/*------------------------------------------------------------------------------

  Function: EndWorkerThread

  Purpose:  Stops the thread, by changing the appropiate flag.

  Input:    bWaitForTermination : If flag is on we wait on the thread terminate
            event.
  Output:   None.

  Remarks:  In some cases, the thread main loop be blocked for a long time
            before reaching the flag. In this cases the derived class may 
            override this method in order to use their own signal to terminate.
------------------------------------------------------------------------------*/
void 
CWorkerThread::EndWorkerThread (BOOL bWaitForTermination)
{
    if (m_bThreadIsRunning)
    {
        m_bEndThread = TRUE;    // Notify thread to die
        if (bWaitForTermination)
            WaitForSingleObject ( m_hThread, INFINITE );
        m_bThreadIsRunning = FALSE;
    }
}

/*------------------------------------------------------------------------------

  Function: InvokeThreadEntry

  Purpose:  Static function passed to AfxBeginThread. It's purpose is to invoke
            the working thread main loop - ThreadEntry.

  Input:    pvParam : Pointer to the derived class.
  Output:   None.

  Remarks:  See explanation at StartWorkerThread.
------------------------------------------------------------------------------*/
UINT 
CWorkerThread::InvokeThreadEntry (LPVOID pvParam)
{
    CWorkerThread *pWorkerThread = (CWorkerThread*)pvParam;

    return pWorkerThread ? pWorkerThread -> ThreadEntry (pWorkerThread -> m_pvParam)
        : 0;
}

