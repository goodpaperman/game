/*****************************************************************************
*                                                                             
*   GameOver.h
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
#ifndef _GAMEOVER_H_
#define _GAMEOVER_H_

#include "GameObject.h"

class CGameOver : public CGameObject
{
public:
    CGameOver ();
    ~CGameOver();

    StateType           CalcState (DWORD dwCurTime);
    ObjectHeightType    GetHeight();
    HIMAGE              GetImage();
    CReaction           React(CGameObject *pTo);
    GameObjectType      GetType();

private:
    HIMAGE    m_hImage;
};

#include "GameOver.inl"


#endif