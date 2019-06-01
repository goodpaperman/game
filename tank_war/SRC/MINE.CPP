/*****************************************************************************
*                                                                             
*   Mine.cpp                                                            
*                                                                             
*   Electrical Engineering Faculty - Software Lab                             
*   Spring semester 1998                                                      
*                                                                             
*   Tanks game                                                                
*                                                                             
*   Module description: Implements the mine game object.
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
#include <Mine.h>
#include <Tanks.h>


CMine::CMine (UINT uXPos, UINT uYPos) :
    CExplodingGameObject (uXPos, uYPos, MINE_WIDTH, MINE_HEIGHT,
                          MINE_INTENSITY, CImageManager::IMG_SHELL_EXPLODE),
    m_dwStartTime (0)
{
    m_himgNormal = m_GlobalImageManager.GetImage (CImageManager::IMG_MINE);
    m_pCurImage = &m_himgNormal;
}

StateType CMine::CalcState (DWORD dwCurTime)
{
    m_bImageChanged = FALSE;    // Assume no change since last CalcState
    if (m_dwStartTime == 0) // Start time not set yet
        m_dwStartTime = dwCurTime;

    if ((dwCurTime - m_dwStartTime) >= MINE_EXPIRATION ||   // Mine expires (time-out)
        IsExplosionOver())                                  // Mine is exploding and after its last frame
    {
        return STATE_DEAD;
    }
    else 
    {
        return STATE_ALIVE;
    }
}

CReaction 
CMine::React(CGameObject *pTo)
{
    if ((pTo->GetType() != TANK && pTo->GetType() != MINE) ||   // We react only to tanks and mines !
        !CollidesWith (pTo))                                    // We react only on collision !
        return CReaction(); // No reaction

        // New mines that collides with us are blocked :
    if (pTo->GetType() == MINE)
        return CReaction (0, TERR_BLOCKED, BONUS_NONE);

        // Now we know a tank hits us !!!
    UINT uTankID = pTo->GetID();
    if (CheckAndSetTankNotification(uTankID))
            // We already exploded on this tank before
        return CReaction(); // No reaction

    // OK - this is a new tank !!!
    Explode ();
        // Play sound
    TANKS_APP->m_gSoundManager.Play(CSoundManager::MINE_EXPLODE);
    return CReaction (m_uMaxIntensity);
}

