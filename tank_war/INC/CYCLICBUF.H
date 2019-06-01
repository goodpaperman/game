/*****************************************************************************
*                                                                             
*   CyclicBuf.h
*                                                                             
*   Electrical Engineering Faculty - Software Lab                             
*   Spring semester 1998                                                      
*                                                                             
*   Tanks game                                                                
*                                                                             
*   Module description: Template implementing a cyclic buffer.
*                       
*                                                                             
*   Authors: Eran Yariv - 28484475                                           
*            Moshe Zur  - 24070856                                           
*                                                                            
*                                                                            
*   Date: 23/09/98                                                           
*                                                                            
******************************************************************************/
#ifndef _CYCLIC_BUFFER_H
#define _CYCLIC_BUFFER_H

template <class T, UINT uInitialSize> 
class CCyclicBuffer
{
public:

    CCyclicBuffer ();
    CCyclicBuffer (const T val);
    virtual ~CCyclicBuffer() {}

    void Clear (const T val);           // Sets all elements to val
    T AddHead (const T val);            // Returns previous value (removed)

    T& operator[] (UINT i);             // Access the i'th element 
    T  operator[] (UINT i) const;       // Access the i'th element 

private:
    T       m_Buff[uInitialSize];
    UINT    m_uCurInd;
};

template <class T, UINT uInitialSize> 
inline 
CCyclicBuffer<T, uInitialSize>::CCyclicBuffer () :
    m_uCurInd (0)
{
    ASSERT (uInitialSize > 1);
}

template <class T, UINT uInitialSize> 
inline 
CCyclicBuffer<T, uInitialSize>::CCyclicBuffer (const T val) :
    m_uCurInd (0)
{
    ASSERT (uInitialSize > 1);
    Clear (val);
}

template <class T, UINT uInitialSize> 
inline 
void CCyclicBuffer<T, uInitialSize>::Clear (const T val) 
{
    for (UINT u=0; u<uInitialSize; u++)
        m_Buff[u] = val;        // Requires operator = from T
}

template <class T, UINT uInitialSize> 
inline 
T& CCyclicBuffer<T, uInitialSize>::operator [] (UINT i)
{
    ASSERT (i < uInitialSize);
    return m_Buff[(i + m_uCurInd) % uInitialSize];
}

template <class T, UINT uInitialSize> 
inline 
T CCyclicBuffer<T, uInitialSize>::operator [] (UINT i) const
{
    ASSERT (i < uInitialSize);
    return m_Buff[(i + m_uCurInd) % uInitialSize];
}


template <class T, UINT uInitialSize> 
T CCyclicBuffer<T, uInitialSize>::AddHead (const T val) 
{
    if (0 == m_uCurInd)
        m_uCurInd = uInitialSize - 1;
    else
        m_uCurInd--;
    T prevVal = m_Buff[m_uCurInd];
    m_Buff[m_uCurInd] = val;
    return prevVal;
}

#endif // _CYCLIC_BUFFER_H
