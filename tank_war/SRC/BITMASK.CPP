/*****************************************************************************
*                                                                             
*   BitMask.cpp                                                            
*                                                                             
*   Electrical Engineering Faculty - Software Lab                             
*   Spring semester 1998                                                      
*                                                                             
*   Tanks game                                                                
*                                                                             
*   Module description: Each game object has one or more bitmaps. In order to 
*                       calculate collision detection between 2 objects, we 
*                       need to find which part of the object's bitmap is 
*                       overlapping the other object's bitmap. 
*                       If we find at least one pixel overlapping,
*                       that is not a background colored pixel,
*                       than we have a collision. To save the effort of 
*                       comparing bitmaps of 24 bpp, we make a mask for each
*                       bitmap, of only 1 byte per pixel - a pixel w/ value
*                       0 is a background (transparent) one, and w/ value 1 is
*                       part of the game object.
*                       
*                                                                             
*   Authors: Eran Yariv - 28484475                                           
*            Moshe Zur  - 24070856                                           
*                                                                            
*                                                                            
*   Date: 23/09/98                                                           
*                                                                            
******************************************************************************/
#include "stdafx.h"
#include "BitMask.h"
#include "GameConsts.h"

/*------------------------------------------------------------------------------

  Function: Constructor.

  Purpose:  Constructs a bit mask for the associated bitmap.

  Input:    dib: A game object bitmap.

  Output:   None.

  Remarks:  Along with the bit mask, we calculate the borders of the object 
            actuall image inside the bitmap.

------------------------------------------------------------------------------*/
CBitMask::CBitMask(CDIB& dib) : m_Size(dib.Size()), m_BitMask(new BYTE[m_Size.cy * m_Size.cx])
{
    ASSERT(m_BitMask);
    PPIXEL Bits = (PPIXEL)dib.m_pBits;
    BYTE *Masks = m_BitMask;
    m_ActualRect.top = m_ActualRect.left = LONG_MAX; 
    m_ActualRect.right = m_ActualRect.bottom = LONG_MIN; 
    for (int x = 0; x < m_Size.cx; x++)
        for (int y = 0; y < m_Size.cy; y++)
        {
            *Masks++ = (*Bits++ != TRANSP_COLOR);
            if (TRANSP_COLOR != dib.ColorAt (x,y))
            {   // Real pixel
                m_ActualRect.top = min (m_ActualRect.top, y);
                m_ActualRect.left = min (m_ActualRect.left, x);
                m_ActualRect.right = max (m_ActualRect.right, x);
                m_ActualRect.bottom = max (m_ActualRect.bottom, y);
            }
        }
}

