/*****************************************************************************
*                                                                             
*   Bomb.cpp                                                            
*                                                                             
*   Electrical Engineering Faculty - Software Lab                             
*   Spring semester 1998                                                      
*                                                                             
*   Tanks game                                                                
*                                                                             
*   Module description: Implements the bomb object.
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
#include <Bomb.h>
#include <TankObj.h>

CBomb::CBomb (CPoint& Origin, UINT uDirectionIndex) :
    CMovingGameObject (         Origin.x, Origin.y,
                                BOMB_WIDTH, BOMB_HEIGHT, 
                                uDirectionIndex, 
                                BOMB_SPEED),

    CExplodingGameObject (      Origin.x, Origin.y,
                                BOMB_WIDTH, BOMB_HEIGHT,
                                BOMB_INTENSITY,
                                CImageManager::IMG_SHELL_EXPLODE),   // TODO: replace(?)
    m_Origin(Origin)
{
    m_himgBomb = m_GlobalImageManager.GetImage (CImageManager::IMG_BOMB);
    m_pCurImage = &m_himgBomb; // Override explosion image as current image
}

StateType 
CBomb::CalcState (DWORD dwCurTime)
{
    m_bImageChanged = FALSE;    // Assume no change since last CalcState
    if (m_bIsExploding)
    {
        if (IsExplosionOver()) // Bomb is exploding and after its last frame
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
    int iDistSqr = CalcNewPos (dwCurTime, FALSE);
    if (iDistSqr < 0) 
    {   // Out of map situation
        return STATE_DEAD;
    }
    if (m_GlobalImageManager.ImageEnded (m_himgBomb))
    {
        // Bomb hit's the ground
        Explode ();
    }
    return STATE_ALIVE;
}
        
CReaction
CBomb::React(CGameObject *pTo)
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
    return CReaction (CalcRelativeExplosionIntensity (pTo, MIN_BOMB_RADIUS, MAX_BOMB_RADIUS));
}


