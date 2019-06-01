/*****************************************************************************
*                                                                             
*   Bonus.cpp                                                            
*                                                                             
*   Electrical Engineering Faculty - Software Lab                             
*   Spring semester 1998                                                      
*                                                                             
*   Tanks game                                                                
*                                                                             
*   Module description: Implements the bonus object.
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
#include "Tanks.h"
#include "Bonus.h"

CBonus::CBonus (BonusType type, CPoint& pos, DWORD lifespan, DWORD StartTime) : 
    m_Type(type),
    m_uLifeSpan(lifespan)
{
    m_uStartTime = StartTime;
    m_Pos = pos;
    m_Size.cx = BONUS_WIDTH;
    m_Size.cy = BONUS_HEIGHT;
    CImageManager::ImageType ImageType = CImageManager::IMG_LAST_INDEX;
    switch (type) {
        case BONUS_SHELLS:
            ImageType = CImageManager::IMG_BONUS_SHELLS;
            break;           
        case BONUS_BULLETS:
            ImageType = CImageManager::IMG_BONUS_BULLETS;
            break;
        case BONUS_MINES:
            ImageType = CImageManager::IMG_BONUS_MINES;
            break;
        case BONUS_BOMBER:
            ImageType = CImageManager::IMG_BONUS_BOMBER;
            break;
        case BONUS_FIRE_RATE:
            ImageType = CImageManager::IMG_BONUS_FIRERATE;
            break;
        case BONUS_SHIELD:
            ImageType = CImageManager::IMG_BONUS_SHIELD;
            break;
        default:    
            ASSERT (FALSE);
    }
    m_hImage = m_GlobalImageManager.GetImage (ImageType);
}

