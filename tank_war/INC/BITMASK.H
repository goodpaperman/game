/*****************************************************************************
*                                                                             
*   BitMask.h
*                                                                             
*   Electrical Engineering Faculty - Software Lab                             
*   Spring semester 1998                                                      
*                                                                             
*   Tanks game                                                                
*                                                                             
*   Module description: Interface of the bitmask class.
*                       
*                                                                             
*   Authors: Eran Yariv - 28484475                                           
*            Moshe Zur  - 24070856                                           
*                                                                            
*                                                                            
*   Date: 23/09/98                                                           
*                                                                            
******************************************************************************/
#ifndef _BIT_MASK_H_
#define _BIT_MASK_H_

#include "Dib.h"

class CBitMask
{
public:
    CBitMask (CDIB &);
    ~CBitMask();

    BOOL GetMaskAt(int,int);

    CRect &GetActualRect ();
private:
    CSize   m_Size;       // should be initialized before the array is allocated!!
    CRect   m_ActualRect;
    BYTE   *m_BitMask;
};

#include "BitMask.inl"

#endif