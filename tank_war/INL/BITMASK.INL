/*****************************************************************************
*                                                                             
*   BitMask.inl
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
CBitMask::~CBitMask()
{
    delete [] m_BitMask;
}

inline BOOL
CBitMask::GetMaskAt(int x,int y)
{
    return m_BitMask[x + m_Size.cx * y];
}

inline CRect &
CBitMask::GetActualRect ()
{
    return m_ActualRect;
}

