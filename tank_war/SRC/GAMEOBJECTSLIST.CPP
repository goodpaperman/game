/*****************************************************************************
*                                                                             
*   GameObjectsList.cpp                                                            
*                                                                             
*   Electrical Engineering Faculty - Software Lab                             
*   Spring semester 1998                                                      
*                                                                             
*   Tanks game                                                                
*                                                                             
*   Module description: Implements the global game object list.
*                       The list consists of 4 sub-lists, one for every game
*                       level (z-order): background, lower, higher and sky.
*                       
*                                                                             
*   Authors: Eran Yariv - 28484475                                           
*            Moshe Zur  - 24070856                                           
*                                                                            
*                                                                            
*   Date: 23/09/98                                                           
*                                                                            
******************************************************************************/
#include <stdafx.h>
#include <GameObjectsList.h>

/*------------------------------------------------------------------------------

  Function: GetGameReaction

  Purpose:  Collects the reaction of all the game objects to the new position of 
            the object in question.

  Input:    pObj - pointer to object we test the reaction upon.

  Output:   The sum of all game objects reaction to our object.

  Remarks:  This method is called by the game manager for every game object in 
            every iteration.
------------------------------------------------------------------------------*/
CReaction 
CGameObjectsList::GetGameReaction (CGameObject *pObj)
{
    UINT uMaxExplosionIntensity = 0;
    TerrainType MaxTerrainDifficulty = TERR_EMPTY;
    BonusType bt = BONUS_NONE;
    UINT uMaxTankID = 0;

    LIST_POS lp = GetHeadPosition(); 
    for (CGameObject* pCurObj = GetNext(lp); pCurObj; pCurObj = GetNext(lp))
    {
        if (pCurObj == pObj)    // Skip our self reaction
            continue;
        CReaction reaction = pCurObj->React (pObj);
        uMaxExplosionIntensity = max (uMaxExplosionIntensity, 
                                      reaction.GetExplosionIntensity ());
        MaxTerrainDifficulty  = max (MaxTerrainDifficulty, 
                                     reaction.GetTerrainDifficulty ());
        bt = max (bt, reaction.GetBonusType());  
        uMaxTankID = max (uMaxTankID, reaction.GetTankID());
    }

    return CReaction (uMaxExplosionIntensity,
                      MaxTerrainDifficulty,
                      bt,
                      uMaxTankID);
}


/*------------------------------------------------------------------------------

  Function: IsObjectValid

  Purpose:  Check if the given pointer is still an active game object.

  Input:    pObj - pointer to object in question.

  Output:   Return TRUE if object is still active.

  Remarks:  
------------------------------------------------------------------------------*/
BOOL 
CGameObjectsList::IsObjectValid (CGameObject *pObj)
{
    LIST_POS lp = GetHeadPosition();

    for (CGameObject *p = GetNext(lp); p; p = GetNext(lp))
        if (p == pObj)
            return TRUE;

    return FALSE;
}