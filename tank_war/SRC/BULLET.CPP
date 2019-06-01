/*****************************************************************************
*                                                                             
*   Bullet.cpp                                                            
*                                                                             
*   Electrical Engineering Faculty - Software Lab                             
*   Spring semester 1998                                                      
*                                                                             
*   Tanks game                                                                
*                                                                             
*   Module description: Implements the bullet object.
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
#include <Bullet.h>

CBullet::CBullet (UINT uXPos, UINT uYPos, UINT uDirectionIndex, UINT uParentTankID) :
    CMovingGameObject (uXPos, uYPos, BULLET_WIDTH, BULLET_HEIGHT, 
                       uDirectionIndex, BULLET_SPEED),
    m_Death (DEATH_NOTDEAD),
    m_bExplodedOnTank (FALSE),
    m_uParentTankID (uParentTankID),
    m_uWaitToTellTank (0)
{
    m_himgBullet = m_GlobalImageManager.GetImage (CImageManager::IMG_BULLET);
    m_Origin = m_Pos;
}

StateType 
CBullet::CalcState (DWORD dwCurTime)
{
    m_bImageChanged = FALSE;    // Assume no change since last CalcState
    if (m_Death != DEATH_NOTDEAD &&     // Bullet is dead and
        m_Death != DEATH_HITTANK)       // not waiting to tell that to any tank
    {
        return STATE_DEAD;
    }
    if (DEATH_HITTANK == m_Death)
    {   // We are dead because we hit a tank
        if (m_bExplodedOnTank)
        {   // and he knows it
            return STATE_DEAD;
        }
        else
        {   // He doesn't know it yet...
            if (m_uWaitToTellTank++ < (DEFAULT_RENDER_FREQ / 2))
            {   // If less than half a second passed and the tank
                // still doesn't know it was hit, wait on
                return STATE_ALIVE;
            }
            else
            {   // Otherwise, we can't just hang in one spot.
                // Kill the bullet.
                return STATE_DEAD;
            }
        }
    }
    // We're still alive here....

    // Try to advance...
    int iDistSqr = CalcNewPos (dwCurTime);
    if (iDistSqr < 0) 
    {   // Out of map situation
        m_Death = DEATH_OUTOFSCREEN;
        return STATE_DEAD;
    }

    // Recalc iDistSqr to be distance from point of fire
    int iXDist = m_Origin.x - m_Pos.x,
        iYDist = m_Origin.y - m_Pos.y;
    iDistSqr = iXDist * iXDist + iYDist * iYDist;

    if (BULLET_DISTANCE_SQR <= iDistSqr)
    {   // Distance expired situation
        m_Death = DEATH_OUTOFRANGE;
        return STATE_DEAD;
    }

    // See what the rest of the world thinks about our movement
    CReaction react = m_GlobalObjsList.GetGameReaction (this);
    TerrainType ter = react.GetTerrainDifficulty();

    if ((ter < TERR_BLOCKED) ||  
            // Cool - didn't hit a thing or ...
        ((HIT_TANK == ter) && (react.GetTankID() == m_uParentTankID)))
            // Hit a tank but it's our father
    {
        m_bImageChanged = TRUE; // We moved a bit
        return STATE_ALIVE;
    }
    if (ter == TERR_BLOCKED)
    {   // Hit a wall
        m_Death = DEATH_HITWALL;
        return STATE_DEAD;
    }
    // Now we're in the (ter == HIT_TANK) situation
    m_Death = DEATH_HITTANK;
    m_uHitTankID = react.GetTankID();   // Store the tank ID we hit
    return STATE_ALIVE; // We can't die until the tank get notified we hit it.
}
        
CReaction
CBullet::React(CGameObject *pTo)
{
    if ((m_Death == DEATH_NOTDEAD) ||   // Either we're still flying
        (pTo->GetType() != TANK)   ||   // or we're not reacting to tanks
        m_bExplodedOnTank)              // or we already exploded
        return CReaction(); // empty reaction

    // Otherwise, we're in STATE_ALIVE and DEATH_HITTANK and this is a tank
    ASSERT (DEATH_HITTANK == m_Death);
    ASSERT (m_uHitTankID < MAX_TANKS);

    if ((TANK == pTo->GetType()) &&     // We're being asked to react to a tank and
        (pTo->GetID() == m_uHitTankID)) // this tank is the tank we hit
    {   // Now, this is the tank we're hitting
        m_bExplodedOnTank = TRUE;   // Finally !!!
        return CReaction (BULLET_INTENSITY);
    }

    return CReaction ();    // This is not the tank we're hitting
}



