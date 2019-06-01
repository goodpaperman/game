/*****************************************************************************
*                                                                             
*   Shell.cpp                                                            
*                                                                             
*   Electrical Engineering Faculty - Software Lab                             
*   Spring semester 1998                                                      
*                                                                             
*   Tanks game                                                                
*                                                                             
*   Module description: Implements the shell game object.
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
#include <Shell.h>
#include <TankObj.h>
#include <SoundManager.h>

CShell::CShell (UINT uXPos, UINT uYPos, UINT uDirectionIndex, UINT uParentTankID) :
    CMovingGameObject (         uXPos, uYPos,
                                SHELL_WIDTH, SHELL_HEIGHT, 
                                uDirectionIndex, 
                                SHELL_SPEED),

    CExplodingGameObject (      uXPos, uYPos,
                                SHELL_WIDTH, SHELL_HEIGHT,
                                SHELL_INTENSITY,
                                CImageManager::IMG_SHELL_EXPLODE),
    m_uParentTankID (uParentTankID)
{
    m_himgShell = m_GlobalImageManager.GetImage (CImageManager::IMG_SHELL);
    m_GlobalImageManager.RotateImage (m_himgShell, uDirectionIndex);
    m_pCurImage = &m_himgShell; // Ovverride explosion image as current image
}

StateType 
CShell::CalcState (DWORD dwCurTime)
{
    m_bImageChanged = FALSE;    // Assume no change since last CalcState
    if (m_bIsExploding)
    {
        if (IsExplosionOver()) // Shell is exploding and after its last frame
        {
            return STATE_DEAD;
        }
        else 
        {
            return STATE_ALIVE;
        }
    }

    // We're flying high, we're flying high right to the sky......

    // Try to advance...
    int iDistSqr = CalcNewPos (dwCurTime);
    if (iDistSqr < 0) 
    {   // Out of map situation
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
        
        m_bImageChanged = TRUE;
        return STATE_ALIVE;
    }

        // Now we're either hitting a wall or a tank !!
    Explode ();
        // Play the explosion sound
    TANKS_APP->m_gSoundManager.Play(CSoundManager::SHELL_EXPLODE);
    return STATE_ALIVE;
}
        
CReaction
CShell::React(CGameObject *pTo)
{
    if (!m_bIsExploding ||          // We don't react until we explode
        pTo->GetType() != TANK)     // We react only to tanks !
    {
        return CReaction(); // No reaction
    }

    UINT uTankID = pTo->GetID();
    if (CheckAndSetTankNotification(uTankID))
            // We already exploded on this tank before
        return CReaction(); // No reaction

        // OK, this is a tank and it is the first time we explode on it ....
    return CReaction (CalcRelativeExplosionIntensity (pTo, MIN_SHELL_RADIUS, MAX_SHELL_RADIUS));
}


