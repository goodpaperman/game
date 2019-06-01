/*****************************************************************************
*                                                                             
*   TankObj.h
*                                                                             
*   Electrical Engineering Faculty - Software Lab                             
*   Spring semester 1998                                                      
*                                                                             
*   Tanks game                                                                
*                                                                             
*   Module description: Implements the tank game object.
*                       
*                                                                             
*   Authors: Eran Yariv - 28484475                                           
*            Moshe Zur  - 24070856                                           
*                                                                            
*                                                                            
*   Date: 23/09/98                                                           
*                                                                            
******************************************************************************/
#ifndef _TANK_OBJECT_H_
#define _TANK_OBJECT_H_

#include "GameConsts.h"
#include "GameObject.h"
#include "ImageManager.h"
#include "ManouverSet.h"
#include "Timer.h"
#include "GameObjectsList.h"
#include "MsgQueue.h"
#include "TanksDlg.h"
#include "CyclicBuf.h"

typedef struct {
    int iBulletXOffset,
        iBulletYOffset,
        iShellXOffset,
        iShellYOffset,
        iMineXOffset,
        iMineYOffset;
} DirOffsetType;

struct PosEntry {
    PosEntry (DWORD dwTime = -1, UINT uXPos = -1, UINT uYPos = -1) :
        m_dwTime (dwTime), m_uXPos(uXPos), m_uYPos(uYPos) {};

    DWORD m_dwTime;
    UINT  m_uXPos;
    UINT  m_uYPos;
};

class CTankObj : public CMovingGameObject
{
public:
    CTankObj (  UINT uID, UINT uXPos, UINT uYPos, UINT uDirIndex,
                BOOL bLocal = FALSE, 
                UINT uShieldLevel = TANK_INIT_SHIELD_LEVEL, 
                UINT uShells = TANK_INIT_SHELLS,
                UINT uBullets = TANK_INIT_BULLETS, 
                UINT uMines = TANK_INIT_MINES, 
                BYTE bFireRateBonusSecsLeft = 0   );
    ~CTankObj ();

    UINT                GetID ();
    UINT                GetShieldLevel ();
    UINT                GetShellsCount ();
    UINT                GetBulletsCount ();
    UINT                GetMinesCount ();

    UINT                SetShieldLevel (UINT uShield);
    UINT                SetShellsCount (UINT uShells);
    UINT                SetBulletsCount (UINT uBullets);
    UINT                SetMinesCount (UINT uMines);

    StateType           CalcState (DWORD dwCurTime);
    ObjectHeightType    GetHeight();
    HIMAGE              GetImage();
    CRect             & GetUpdateRectangle();

    BOOL                GetFastFire();
    CReaction           React(CGameObject *pTo);
    GameObjectType      GetType();
    BOOL                IsLocal ();

    BOOL                IsZombie ();
    void                SetZombie (BOOL);

        // Ask tank to relinquish input manouver set and ignore user requests
    void                RelinquishInput ();     
        // Ask tank to regain input manouver set and obey user requests
    void                RegainInput (BOOL bBomberLaunched);         

    CManouverSet&       GetManouverSet () const;

    void                EatBonus (BonusType, DWORD dwEatTime);
    BOOL                IsExploding ();
    void                Kill(); // Start exploding the tank

    UINT                GetDirection ();    // Get current tank direction

    WORD                GetStateCheckSum(); // Gets check sum (16 bits) of shield level, ammo level and fast fire rate
    void                GetStatus(CMessage::MessageData &MsgData);
    void                GetStatusAndPos(CMessage::MessageData &MsgData);

    void                SetPos (DWORD dwTime, int XPos, int YPos);
    void                SetStatus(CMessage::MessageData &MsgData);

private:
        // images:
    HIMAGE              m_hTankImage;               // handle to a bitmap of a live tank
    HIMAGE              m_hExplodedTankImage;       // handle to a bitmap of an exploding tank
    HIMAGE              m_hZombieOverlay;           // Zombie overlay image    
    HIMAGE            * m_pCurrentImage;            // points to one of the handles above
        // keyboard:                    
    CManouverSet      & m_ManouverSet;              // points to manouver set in game manager
        // manouvers:                   
    DWORD               m_dwLastBulletFiredTick;    // last time a bullet was fired

    BOOL                BulletsEnabled(DWORD);      // function to control the bullet fire rate
    void                FireBullet(DWORD);          // fuction to handle a fired bullet
    DWORD               m_dwLastShellFiredTick;     // last time a shell was fired
    DWORD               m_dwLastMineDropTick;       // last time mine was dropped
    BOOL                MinesEnabled(DWORD);        // Controls the mine fire rate

    BOOL                ShellsEnabled(DWORD);       // function to control the shell fire rate
    void                FireShell(DWORD);           // function to handle a fired shell
    BOOL                DropMine(DWORD);            // function to handle a dropped mine
    void                LaunchBomber();             // function to handle launching a new bomber


    WORD                PackBits (WORD wVal, WORD wNumBits);    // Get best representation with a given number of bits


    DWORD               m_dwFireRateBonusLastTick;  // the last tick for the fire rate bonus to remain active
    DWORD               m_dwBulletFireDelay;        // current delay between bullets
    DWORD               m_dwShellFireDelay;         // current delay between shells
    DWORD               m_dwLastRotationTime;
        // properties:
    UINT                m_uTankID;
    UINT                m_uShieldLevel;
    UINT                m_uShells;
    UINT                m_uBullets;
    UINT                m_uMines;
    BOOL                m_bLocal;                   // Are we a local or remote tank?
    BOOL                m_bUseManouverSet;          // Do we have control of the manouver set?
    BOOL                m_bBomberAvail;             // Do we have a bomber at our disposal?
    BOOL                m_bBomberInSetup;           // Did we just launch a bomber in setup mode?
    BOOL                m_bZombie;                  // Is tank zombie (slow network connection)?
        // aliases:
    CTanksDlg         * m_pDlgWnd;
    CImageManager     & m_ImageManager;
    CGameObjectsList  & m_ObjList;
    CMsgQueue         & m_MsgQueue;
    CCommManager      & m_CommManager;
    TIMER_CLASS       & m_Timer;

    static const DirOffsetType m_SpawnOffsets [];

    // Manouver set posting to the server:
    CManouverSet        m_PrevManouverSet; // Previous manouver set
    void                SendManouverSet ( CManouverSet &ms );

    // Setting a new position is tricky, doing it only after a mismatch checksum:
    BOOL                m_bForceNewPos;   // Are we forced with a new position and direction ?
    int                 m_iNewForcedXPos;
    int                 m_iNewForcedYPos;
    UINT                m_uNewForcedDir;

    // This cyclic buffer holds the history table of the recent tanks positions:
    CCyclicBuffer<PosEntry, MAX_POS_TABLE> m_PosTable;
};

#include "TankObj.inl"

#endif