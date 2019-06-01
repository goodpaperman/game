/*****************************************************************************
*                                                                             
*   Bomb.h
*                                                                             
*   Electrical Engineering Faculty - Software Lab                             
*   Spring semester 1998                                                      
*                                                                             
*   Tanks game                                                                
*                                                                             
*   Module description: Interface of the bomb game object.
*                       
*                                                                             
*   Authors: Eran Yariv - 28484475                                           
*            Moshe Zur  - 24070856                                           
*                                                                            
*                                                                            
*   Date: 23/09/98                                                           
*                                                                            
******************************************************************************/
#ifndef _BOMB_H
#define _BOMB_H

#include <GameObject.h>
#include <GameManager.h>
#include <Tanks.h>
#include <GameBoard.h>

// The following pragma is here to tell the compiler we're using virtual inheritance to solve the
// classic diamond problem.
#pragma pointers_to_members(full_generality, virtual_inheritance)

// The following pragma is here to tell the compiler not to display the following warning message:
// warning C4250: 'CBomb' : inherits 'CExplodingGameObject::GetImage' via dominance.
//
// We get this warning because GetImage is defined as a pure virtual function in the abstract
// class CGameObject. CMovingGameObject inherits it but doesn't change it (keeps it pure virtual)
// and CExplodingGameObject inherits it and gives it a concrete implementation.
// Since CBomb inherits from CExplodingGameObject and CMovingGameObject (class diamond)
// the compiler tells us it chose to use the concrete implementation found in 
// CExplodingGameObject.
#pragma warning( disable : 4250 )

class CBomb: public CMovingGameObject, CExplodingGameObject
{
public:
    CBomb (CPoint& Origin, UINT uDirectionIndex);
    virtual ~CBomb () {}

    StateType           CalcState (DWORD dwCurTime);
    ObjectHeightType    GetHeight();                
    CReaction           React(CGameObject *pTo);    
    GameObjectType      GetType();                  
    CRect &             GetUpdateRectangle();       

private:
    HIMAGE              m_himgBomb;
    CPoint              m_Origin;
}; 

#include <Bomb.inl>


#pragma

#endif