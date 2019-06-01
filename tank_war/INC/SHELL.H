/*****************************************************************************
*                                                                             
*   Shell.h
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
#ifndef _SHELL_H
#define _SHELL_H

#include <GameObject.h>
#include <GameManager.h>
#include <Tanks.h>
#include <GameBoard.h>

// The following pragma is here to tell the compiler we're using virtual inheritance to solve the
// classic diamond problem.
#pragma pointers_to_members(full_generality, virtual_inheritance)

// The following pragma is here to tell the compiler not to display the following warning message:
// warning C4250: 'CShell' : inherits 'CExplodingGameObject::GetImage' via dominance.
//
// We get this warning because GetImage is defined as a pure virtual function in the abstract
// class CGameObject. CMovingGameObject inherits it but doesn't change it (keeps it pure virtual)
// and CExplodingGameObject inherits it and gives it a concrete implementation.
// Since CShell inherits from CExplodingGameObject and CMovingGameObject (class diamond)
// the compiler tells us it chose to use the concrete implementation found in 
// CExplodingGameObject.
#pragma warning( disable : 4250 )


class CShell: public CMovingGameObject, CExplodingGameObject
{
public:
    CShell (UINT uXPos, UINT uYPos, UINT uDirectionIndex, UINT uParentTankID);
    virtual ~CShell () {}

    StateType           CalcState (DWORD dwCurTime);
    ObjectHeightType    GetHeight();                
    CReaction           React(CGameObject *pTo);    
    GameObjectType      GetType();                  
    CRect &             GetUpdateRectangle();       

private:
    HIMAGE              m_himgShell;
    UINT                m_uParentTankID;    // ID of shooting tank
}; 

#include <Shell.inl>


#pragma

#endif