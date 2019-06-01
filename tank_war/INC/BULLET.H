/*****************************************************************************
*                                                                             
*   Bullet.h
*                                                                             
*   Electrical Engineering Faculty - Software Lab                             
*   Spring semester 1998                                                      
*                                                                             
*   Tanks game                                                                
*                                                                             
*   Module description: Interface for the bullet game object.
*                       
*                                                                             
*   Authors: Eran Yariv - 28484475                                           
*            Moshe Zur  - 24070856                                           
*                                                                            
*                                                                            
*   Date: 23/09/98                                                           
*                                                                            
******************************************************************************/
#ifndef _BULLET_H
#define _BULLET_H

#include <GameObject.h>
#include <GameConsts.h>
#include <ImageManager.h>
#include <GameManager.h>
#include <Tanks.h>
#include <GameBoard.h>

class CBullet: public CMovingGameObject
{
public:
    CBullet (UINT uXPos, UINT uYPos, UINT uDirectionIndex, UINT uParentTankID);
    virtual ~CBullet () {}

    StateType           CalcState (DWORD dwCurTime);
    ObjectHeightType    GetHeight();                
    HIMAGE              GetImage();                 
    CReaction           React(CGameObject *pTo);    
    GameObjectType      GetType();                  

private:

    typedef enum {  
            DEATH_OUTOFSCREEN,          // Bullet just flew out the window
            DEATH_HITWALL,              // Bullet hits a wall
            DEATH_HITTANK,              // Mission acomplished
            DEATH_OUTOFRANGE,           // Range exceeded
            DEATH_NOTDEAD
         } BulletDeathType;      

    BulletDeathType     m_Death;            // Bullet's death state
    HIMAGE              m_himgBullet;       // Bullet's image handle
    BOOL                m_bExplodedOnTank;  // Did we tell a tank we hit it yet ?
    CPoint              m_Origin;           // Point of start
    UINT                m_uParentTankID;    // ID of shooting tank
    UINT                m_uWaitToTellTank;  // Counter until tank knows bullet hits it
    UINT                m_uHitTankID;       // ID of tank bullet hits

    /*  When a bullet flies it may hit a tank.
        If this is the case, the bullet discovers it in its CalcState(...) call.
        It goes into DEATH_HITTANK state and stores the tank ID of the tank it
        hits in m_uHitTankID.
        It then waits until this tank calls React(...).
        When that tank calls React(...) it returns the hit reaction and 
        sets m_bExplodedOnTank to TRUE.
        The next call to CalcState(...) causes the bullet to die.
        If the tank didn't call React(...) for that bullet (the tank might be dead)
        the bullet waits for about 0.5 second and then kills itself.
    */
};

#include <Bullet.inl>

#endif