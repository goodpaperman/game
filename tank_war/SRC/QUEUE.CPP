/*****************************************************************************
*                                                                             
*   Queue.cpp                                                            
*                                                                             
*   Electrical Engineering Faculty - Software Lab                             
*   Spring semester 1998                                                      
*                                                                             
*   Tanks game                                                                
*                                                                             
*   Module description: Implements a multi threaded safe queue, using a linked
*                       list to hold the data. A semaphor is used to signal the
*                       thread for a new incoming data.
*                                                                             
*   Authors: Eran Yariv - 28484475                                           
*            Moshe Zur  - 24070856                                           
*                                                                            
*                                                                            
*   Date: 23/09/98                                                           
*                                                                            
******************************************************************************/
#include "stdafx.h"
#include "queue.h"
#include "Message.h"

/***************************************************************************

   Function:   CloseQueue

   Purpose:    Empty the queue and delete all it's elements.

   Input:      None.

   Output:     None.

****************************************************************************/
void CQueue::CloseQueue()
{
    Empty();
    m_bQueueIsDead = TRUE;
}
	


/***************************************************************************

   Function:   GetQueueSize

   Purpose:    Get the current total number of items in the queue

   Input:      None.

   Output:     Number of items in the queue.

****************************************************************************/
int CQueue::GetQueueSize()
{
    int nQueueSize;

    if (m_bQueueIsDead) // close Q flag is set
        return 0;   // Size of dead queue is zero
    
    m_QueueCS.Lock();		// Lock the option of accessing the queue

    nQueueSize = m_Queue.GetCount();

    m_QueueCS.Unlock();	// Release the option of accessing the queue
    return nQueueSize;
}


/***************************************************************************

   Function:   Enqueue

   Purpose:    Insert a given element into the queue.

   Input:      Pointer to the new element.

   Output:     TRUE upon success, otherwise - FALSE.

****************************************************************************/
BOOL CQueue::Enqueue(CObject* pNewElement)	
{
    if (m_bQueueIsDead) // close Q flag is set
        return FALSE;   // Cannot add messages to a dead queue

	m_QueueCS.Lock();		// Lock the option of accessing the queue

    if (m_bQueueIsDead) // close Q flag is set
        return FALSE;   // Cannot add messages to a dead queue

    if (pNewElement == KILL_Q_MSG)
        m_bQueueIsDead = TRUE;
    else
    {
        m_Queue.AddTail(pNewElement);	// Insert the new alement at the tail of the queue
    }
    if(m_bBlocking)  // should wait until there are elements in the queue
	    m_DispatchSem.Unlock();	// Update the dispatch semaphore - the queue is not empty
        
    m_QueueCS.Unlock();	// Release the option of accessing the queue

    if (NULL != m_hEnqueueEvent)
    {
        SetEvent (m_hEnqueueEvent);
    }

	return TRUE;
}


/***************************************************************************

   Function:   Dequeue

   Purpose:    Get the head object out of the queue.

   Input:      Return object address.

   Output:     BOOL - FALSE if there are no objects in the queue,
               so that no object removed from the list, otherewise - TRUE.

   Remarks:	   The head object removed out of the queue but not deleted.

****************************************************************************/
BOOL CQueue::Dequeue(CObject*& pRes)	
{

    if (m_bQueueIsDead) // close Q flag is set
    {
        pRes = KILL_Q_MSG;
        return TRUE;
    }

    if(m_bBlocking) {// should wait until there are elements in the queue
	    m_DispatchSem.Lock();	// wait until there are objects in the queue

        if (m_bQueueIsDead) { // close Q flag was set while were sleeping on the locked semaphore
            pRes = KILL_Q_MSG;
            return TRUE;
        }
    }

	m_QueueCS.Lock();		// Lock the option of accessing the queue

    if (m_bQueueIsDead) { // Close Q flag was set while were sleeping on the locked semaphore
        pRes = KILL_Q_MSG;
        return TRUE;
    }

    if(m_Queue.IsEmpty()){   // No items in the queue
		m_QueueCS.Unlock();
        return FALSE;
	}

	pRes = m_Queue.RemoveHead ();	// Get the queue's head object and remove it
	m_QueueCS.Unlock();	// Release the option of accessing the queue
    return TRUE;
}

void 
CQueue::SetNotificationEvent (HANDLE h)
{
    m_hEnqueueEvent = h;
}

void
CQueue::Empty()
{
	CObject* pElement;

	m_QueueCS.Lock();		// Lock the option of accessing the queue

    for (POSITION pos = m_Queue.GetHeadPosition(); pos != NULL; ) // walk through the queue
    {
        pElement = m_Queue.GetNext(pos);
        
        if(pElement) // not the kill Q message
        {
            delete(pElement); // throw exception
            m_DispatchSem.Unlock ();
        }
    }
    m_Queue.RemoveAll(); 

	ASSERT(m_Queue.IsEmpty()); // check if the queue is empty after removing the elements
    
	m_QueueCS.Unlock();	// Release the option of accessing the queue
}

BOOL CQueue::Enqueue(CObject* pNewElement, DWORD dwKey)	
{
    if (m_bQueueIsDead) // close Q flag is set
        return FALSE;   // Cannot add messages to a dead queue

	m_QueueCS.Lock();		// Lock the option of accessing the queue

    if (m_bQueueIsDead) // close Q flag is set
        return FALSE;   // Cannot add messages to a dead queue

    if (pNewElement == KILL_Q_MSG)
        m_bQueueIsDead = TRUE;
    else
    {
        BOOL fInsertDone = FALSE;   // Was an item inserted
        for (POSITION pos = m_Queue.GetHeadPosition(); pos; ) 
        {
            POSITION CurPos = pos;
            if (((CMessage*)m_Queue.GetNext(pos))->GetTime() > dwKey) 
            {
                m_Queue.InsertBefore(CurPos, pNewElement);
                fInsertDone = TRUE;
                break;
            }
        }
        if (! fInsertDone)  // empty loop, or new message has the highest key:
            m_Queue.AddTail(pNewElement);	// Insert the new alement at the tail of the queue
    }
    if(m_bBlocking)  // should wait until there are elements in the queue
	    m_DispatchSem.Unlock();	// Update the dispatch semaphore - the queue is not empty
        
    m_QueueCS.Unlock();	// Release the option of accessing the queue

    if (NULL != m_hEnqueueEvent)
    {
        SetEvent (m_hEnqueueEvent);
    }
	return TRUE;
}

