/*****************************************************************************
*                                                                             
*   gameObjectsList.h
*                                                                             
*   Electrical Engineering Faculty - Software Lab                             
*   Spring semester 1998                                                      
*                                                                             
*   Tanks game                                                                
*                                                                             
*   Module description: Implements the global game objects lists.
*                       
*                                                                             
*   Authors: Eran Yariv - 28484475                                           
*            Moshe Zur  - 24070856                                           
*                                                                            
*                                                                            
*   Date: 23/09/98                                                           
*                                                                            
******************************************************************************/
#ifndef _GAME_OBJECTS_LIST_H
#define _GAME_OBJECTS_LIST_H

#include <afxmt.h>
#include <ObjectsArray.h>

typedef struct { BYTE ind; ARR_POS pos; }     LIST_POS;

class CGameObjectsList
{
public:
    CGameObjectsList ();
    virtual ~CGameObjectsList ();

    void KillList ();
    void AddObject (CGameObject *);
    void RemoveObject (CGameObject *);

    CReaction GetGameReaction (CGameObject *);

    int GetObjectsCount () const;
    LIST_POS GetHeadPosition ();
    CGameObject *GetNext (LIST_POS&);

    BOOL IsObjectValid (CGameObject *);     // Checks the pointer points to a living object
    
    void Freeze();                          // Freeze the list from being changed (add || remove)
    void Thaw();                            // Undo freeze

private:
    CObjectsArray m_aArrays[4];             // One array for each z-depth level
    CCriticalSection m_CS;                  // Sync mechanism
};


// Inline sections:
#include <GameObjectsList.inl>

#endif