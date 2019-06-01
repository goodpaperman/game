/*****************************************************************************
*                                                                             
*   Mine.h
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
#ifndef _MINE_H
#define _MINE_H

#include <GameObject.h>
#include <TankObj.h>

class CMine : public CExplodingGameObject
{
public:
    CMine (UINT uXPos, UINT uYPos);
    virtual ~CMine () {}

    StateType           CalcState (DWORD dwCurTime);
    ObjectHeightType    GetHeight();
    CReaction           React(CGameObject *pTo);
    GameObjectType      GetType();
    void                Kill();

private:
    HIMAGE              m_himgNormal;
    DWORD               m_dwStartTime;
};


#include <Mine.inl>

#endif