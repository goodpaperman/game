/*****************************************************************************
*                                                                             
*   Bonus.h
*                                                                             
*   Electrical Engineering Faculty - Software Lab                             
*   Spring semester 1998                                                      
*                                                                             
*   Tanks game                                                                
*                                                                             
*   Module description: Interface of the bonus game object.
*                       
*                                                                             
*   Authors: Eran Yariv - 28484475                                           
*            Moshe Zur  - 24070856                                           
*                                                                            
*                                                                            
*   Date: 23/09/98                                                           
*                                                                            
******************************************************************************/
#ifndef _BONUS_H_
#define _BONUS_H_

#include "GameObject.h"

class CBonus : public CGameObject
{
public:
    CBonus (BonusType type, CPoint& pos, DWORD lifespan, DWORD StartTime);
    ~CBonus();

    StateType           CalcState (DWORD dwCurTime);
    ObjectHeightType    GetHeight();
    HIMAGE              GetImage();
    CReaction           React(CGameObject *pTo);
    GameObjectType      GetType();
    BonusType           GetBonusType ();
    void                Kill();

private:
    BonusType m_Type;
    HIMAGE    m_hImage;
    DWORD     m_uLifeSpan;
    DWORD     m_uStartTime;
};

#include "Bonus.inl"


#endif