/*****************************************************************************
*                                                                             
*   Queue.h
*                                                                             
*   Electrical Engineering Faculty - Software Lab                             
*   Spring semester 1998                                                      
*                                                                             
*   Tanks game                                                                
*                                                                             
*   Module description: Implements the queue base class.
*                       
*                                                                             
*   Authors: Eran Yariv - 28484475                                           
*            Moshe Zur  - 24070856                                           
*                                                                            
*                                                                            
*   Date: 23/09/98                                                           
*                                                                            
******************************************************************************/
#ifndef __CQUEUE_H__
#define __CQUEUE_H__

/*-------------------------- INCLUDE SECTION---------------------------------*/
#include <afxmt.h>

/*------------------------ EXCEPTION SECTION --------------------------------*/

/*------------------------ DEFINITION SECTION -------------------------------*/

class CQueue
{
public:

#define KILL_Q_MSG      ((CObject*)NULL)    

    CQueue (BOOL bBlocking) :
        m_bBlocking(bBlocking),
        m_bQueueIsDead (FALSE),
        m_hEnqueueEvent (NULL),
        m_DispatchSem (0 /* Initial counter */, LONG_MAX /* there are INFINITY counters */)
    {}
    
    virtual ~CQueue()   
    { 
        CloseQueue();
    }

    BOOL Enqueue (CObject* pNewElement);    // Insert new object into the queue

    BOOL Enqueue (CObject* pNewElement, DWORD dwKey); // Insert new object to sorted Q

    BOOL Dequeue(CObject*&);                // Remove an object out of the queue

    int GetQueueSize();

    void SetNotificationEvent (HANDLE);

    void Empty();

private: 

    void CloseQueue ();

    CObList             m_Queue;            // The queue's list

    CCriticalSection    m_QueueCS;          // use to synchronize between the queue's actions
    CSemaphore          m_DispatchSem;      // use to synchronize between the queue's content and reader
    
    BOOL                m_bQueueIsDead,     // TRUE if KILL_Q_MSG was dequeued
                        m_bBlocking;        // TRUE if enqueue operation should wait until 
                                            // there are items in the queue
    HANDLE              m_hEnqueueEvent;    // Signalled when enqueue if performed.
};                 


#endif