/*****************************************************************************
*                                                                             
*   GameOver.cpp                                                            
*                                                                             
*   Electrical Engineering Faculty - Software Lab                             
*   Spring semester 1998                                                      
*                                                                             
*   Tanks game                                                                
*                                                                             
*   Module description: Implements the game over object.
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
#include "GameOver.h"
#include "Tanks.h"

CGameOver::CGameOver ()
{
    m_Pos.x = MAP_WIDTH / 2 - GAMEOVER_ANIM_WIDTH / 2;
    m_Pos.y = MAP_HEIGHT / 2 - GAMEOVER_ANIM_HEIGHT / 2;
    m_Size.cx = GAMEOVER_ANIM_WIDTH;
    m_Size.cy = GAMEOVER_ANIM_HEIGHT;
    m_GlobalImageManager.LoadGameOverBitmaps();
    m_hImage = m_GlobalImageManager.GetImage (CImageManager::IMG_GAMEOVER);
        // Play sound
    TANKS_APP->m_gSoundManager.Play(CSoundManager::GAME_OVER);
}

