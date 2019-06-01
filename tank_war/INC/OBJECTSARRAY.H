/*****************************************************************************
*                                                                             
*   ObjectsArray.h
*                                                                             
*   Electrical Engineering Faculty - Software Lab                             
*   Spring semester 1998                                                      
*                                                                             
*   Tanks game                                                                
*                                                                             
*   Module description: Implements an array of game objects sharing the same
*                       z-order. Used by the GameObjectsList class.
*                       
*                                                                             
*   Authors: Eran Yariv - 28484475                                           
*            Moshe Zur  - 24070856                                           
*                                                                            
*                                                                            
*   Date: 23/09/98                                                           
*                                                                            
******************************************************************************/
#ifndef _OBJECTS_ARRAY_H_
#define _OBJECTS_ARRAY_H_

#include "GameObject.h"

typedef CGameObject**     ARR_POS;

class CObjectsArray
{
public:
    CObjectsArray ();
    virtual ~CObjectsArray();

    void Init (UINT uSize);

    BOOL Add(CGameObject *);
    BOOL Remove(CGameObject *);

    ARR_POS GetHeadPosition();
    CGameObject* GetNext(ARR_POS&);

    BOOL IsEmpty();
    int GetObjectsCount() const;

    void Empty();                   // Deletes all pointer in array

private:
    UINT m_uSize;                   // Total number of entries
    CGameObject** m_Array;          // Allocated array
                                    
    ARR_POS m_lplpTop;              // Pointer to next empty entry
    ARR_POS m_lplpLast;             // Pointer to last available entry (&m_Array[m_uSize-1])
};

#include "ObjectsArray.inl"

#endif