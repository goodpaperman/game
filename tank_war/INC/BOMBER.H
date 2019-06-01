/*****************************************************************************
*                                                                             
*   Bomber.h
*                                                                             
*   Electrical Engineering Faculty - Software Lab                             
*   Spring semester 1998                                                      
*                                                                             
*   Tanks game                                                                
*                                                                             
*   Module description: Interface of the bomber game object.
*                       
*                                                                             
*   Authors: Eran Yariv - 28484475                                           
*            Moshe Zur  - 24070856                                           
*                                                                            
*                                                                            
*   Date: 23/09/98                                                           
*                                                                            
******************************************************************************/
#ifndef _BOMBER_OBJECT_H_
#define _BOMBER_OBJECT_H_

#include "GameConsts.h"
#include "GameObject.h"
#include "ImageManager.h"
#include "ManouverSet.h"
#include "Timer.h"
#include "GameObjectsList.h"
#include "MsgQueue.h"
#include "TankObj.h"
#include "Bomb.h"

class CBomber : public CMovingGameObject
{
public:
    CBomber (CTankObj *pParent);    // Creates a bomber is setup mode
    CBomber (UINT uDirInd,          // Creates a bomber in flight mode
             UINT uXPos,
             UINT uYPos,
             DWORD dwInitFlightTime);
    ~CBomber ();

    StateType           CalcState (DWORD dwCurTime);
    ObjectHeightType    GetHeight();
    HIMAGE              GetImage();
    CReaction           React(CGameObject *pTo);
    GameObjectType      GetType();
    BOOL                IsLocal ();

private:

    typedef enum {
        BOMBER_STATE_SETUP,        // Setup mode
        BOMBER_STATE_FLYING,       // Flying mode
        BOMBER_STATE_DEAD          // Dead now
    } BomberStateType;

        // images:
    HIMAGE          m_hImage;           // handle to a bitmap of a bomber
        // keyboard:                    
    CManouverSet  * m_pManouverSet;     // points to manouver set in game manager
        // manouvers:                   
    DWORD           m_dwLastMoveTick;   // last time a we moved (in setup) or dropped a bomb (in flying)
        // properties:
    CTankObj      * m_pFather;                
    BOOL            m_bLocal;           // Are we a local or remote bomber?
    BomberStateType m_State;
    DWORD           m_dwInitialTime;    // Flight start time 
    CPoint          m_InitialPos;       // Flight start point
    UINT            m_uEdgePos;         // Position on map edge
        // aliases:
    CMsgQueue     & m_MsgQueue;
    TIMER_CLASS   & m_Timer;

    CPoint CalcEdgePos ();
    void   FixEdgeDirection ();
    BOOL   FindBombPos (CPoint &ResPos, DWORD dwTimeGap);
    void   SendBomberMsg(); 

};

#include "Bomber.inl"

#endif