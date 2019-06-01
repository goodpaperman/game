/*****************************************************************************
*                                                                             
*   Pool.h
*                                                                             
*   Electrical Engineering Faculty - Software Lab                             
*   Spring semester 1998                                                      
*                                                                             
*   Tanks game                                                                
*                                                                             
*   Module description: CPool provides a memory pool template for fast 
*                       allocation/deallocation of T objects; Used for game
*                       message passed in process.
*                       
*                                                                             
*   Authors: Eran Yariv - 28484475                                           
*            Moshe Zur  - 24070856                                           
*                                                                            
*                                                                            
*   Date: 23/09/98                                                           
*                                                                            
******************************************************************************/
#ifndef _POOL_H_
#define _POOL_H_

//
//  Purpose: CPool provides a memory pool for fast allocation/deallocation of T objects;
//  Usage:   Class T should declare CPool<T> object as a static member (preferably private),
//           and override it's default new and delete operators to call CPool::Alloc and 
//           CPool::Free instead.
//

#define ELEMENTS_PER_BLOCK  512

template <class T>      // class T must provide access to a T* pNext member.
class CPool
{
public:
    CPool () {}     
    ~CPool () {
        // TODO: delete ALL allocated blocks
        ::operator delete (m_pHeadOfBlocksList);
    }


    static void *Alloc (size_t uElementSize);
    static void  Free (void *pDeadElement, size_t uElementSize);

private:
    static T *m_pHeadOfFreeList;
    static T *m_pHeadOfBlocksList;
};


template <class T>
T *CPool<T>::m_pHeadOfFreeList = NULL;

template <class T>
T *CPool<T>::m_pHeadOfBlocksList = NULL;

template <class T>
void *CPool<T>::Alloc (size_t uElementSize)
{
    if (uElementSize != sizeof (T)) 
    {   // send request of the wrong size to ::operator new()
        ASSERT(FALSE);
        //return ::operator new (uElementSize);
        return NULL;
    }

    T *p = m_pHeadOfFreeList;      // try to take an element from the head of list

    if (p)                          // if p is valid just move head of list to next free element
        m_pHeadOfFreeList = p->pNext;

    else {
        // The free list is empty. Allocate a block of memory for ELEMENTS_PER_BLOCK elements:
        
        T *pNewBlock = static_cast<T*>(::operator new (ELEMENTS_PER_BLOCK * sizeof (T)));
        
        // for now - just store a pointer to this single allocated block
        // TODO: add block to list of blocks; meanwhile make sure we don't start another block
        ASSERT(m_pHeadOfBlocksList==NULL);
        if (m_pHeadOfBlocksList)
        {
            return NULL;
        }
        m_pHeadOfBlocksList = pNewBlock;

        // form a new free list by linking the memory chunks together; skip the zeroth element
        // because you'll return that to the caller of operator new

        for (int i = 1; i < ELEMENTS_PER_BLOCK-1; i++)
            pNewBlock[i].pNext = &pNewBlock[i+1];

        // terminate the linked list with a null pointer

        pNewBlock[ELEMENTS_PER_BLOCK-1].pNext = NULL;

        // set p to front of list, m_pHeadOfFreeList to chunk immediately following

        p = pNewBlock;
        m_pHeadOfFreeList = &pNewBlock[1];
    }

    return p;
}

template <class T>
void CPool<T>::Free (void *pDeadElement, size_t uElementSize)
{
    if (pDeadElement == NULL)           // default behavior of ::operator delete()
        return;

    if (uElementSize != sizeof (T)) 
    {   // send request of the wrong size to ::operator delete()
        ASSERT(FALSE);
        //::operator delete(pDeadElement);
        return;
    }

    // cast void pointer to element pointer

    T *pCarcass = static_cast<T*>(pDeadElement);

    // add element to head free list

    pCarcass->pNext = m_pHeadOfFreeList;

    m_pHeadOfFreeList = pCarcass;
}

#endif