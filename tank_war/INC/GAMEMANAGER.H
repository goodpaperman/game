/*****************************************************************************
*                                                                             
*   GameManager.h
*                                                                             
*   Electrical Engineering Faculty - Software Lab                             
*   Spring semester 1998                                                      
*                                                                             
*   Tanks game                                                                
*                                                                             
*   Module description: Implements the manager of the game objects.
*                       
*                                                                             
*   Authors: Eran Yariv - 28484475                                           
*            Moshe Zur  - 24070856                                           
*                                                                            
*                                                                            
*   Date: 23/09/98                                                           
*                                                                            
******************************************************************************/
#ifndef _GAME_MANAGER_H
#define _GAME_MANAGER_H


#include <WorkerThread.h>
#include <GameObject.h>
#include <GameObjectsList.h>
#include <ManouverSet.h>
#include <GameConsts.h>
#include <Timer.h>
#include <MsgQueue.h>
#include <DrawDib.h>

class CTankObj;     // Pre-declaration
class CBonus;       // Pre-declaration
class CCommManager; // Pre-declaration

class CGameManager : public CWorkerThread
{
public:
    CGameManager (UINT uFreq = DEFAULT_RENDER_FREQ);
    virtual ~CGameManager ();

    void                BeginGame ();
    void                EndGame ();

    void                AddObject (CGameObject *pGameObj);
    void                RemoveObject (CGameObject *pGameObj);

    CGameObjectsList  & ExposeObjects();
    void                GetBonusState(CMessage::MessageData& MsgData);
    BOOL                GetTankStatus(int iTankID, CMessage::MessageData& MsgData);
    BOOL                GetTankStatusAndPos(int iTankID, CMessage::MessageData& MsgData);
    DWORD               GetMinesInSector (UINT uSector, DWORD *pAllMinesInSector);
    void                SetMinesInSector (UINT uSector, DWORD dwNumMines, DWORD *pAllMinesInSector);

    BOOL                IsRectVacant (CRect&);             // Check that the rectangle is vacant
    BOOL                IsPositionValid (CGameObject*);    // Check that the object's position is vacant

        // Frequency specifies frames per second
    UINT                GetFrequency ();
    BOOL                SetFrequency (UINT);

    void                RefreshDisplay (); // Ask the game manager to refresh entire map display on next frame

    void                FindVacantRect (CSize&, CPoint &);   // Finds a point for a rect in the board which is vacant
    DWORD               GetMinDistanceFromTanks (CPoint &);  // Returns min distance from all active tanks

    BOOL                IsAlive (UINT uTankID);    // Returns TRUE if TankID appears in array

    int                 GetLocalTankID() const;    // Return the ID of the local tank (-1 if none)

    CManouverSet m_ManouverSets [MAX_TANKS]; // The manouver set for each tank

private:
    // Methods:
    UINT                ThreadEntry (LPVOID lpParam=0);    // main loop of the game
    BOOL                EmptyMsgQ(DWORD);
    void                DumpBackBufferToScreen ();
    void                DestroyObjects();
    void                AttemptToSendGameChecksum ();      // If it is time to send the check sum, do so.

        /* The following two methods represent two different techniques to display images.

           SingleRectGameTechnique is a technique in which the board (constant terrain and obstacles) 
           is fully copied to the back buffer in every game loop.
           The objects are then pasted (transparently) over the board and when all the objects
           are traversed (and if there was a change in the scene) the entire back-buffer is copied to
           the screen.

           MultiRectGameTechnique is a technique in which the board (constant terrain and obstacles) 
           is copied to the back buffer in rectangles.
           The objects are traversed twice.
           One the first loop, the update rectangle of each object and its image ID are collected.
           For each object the board map is copied (only in the object's rectangle) to the 
           back-buffer. The update rect and the image ID are stored in an array.

           The second loop traverses that array. The objects themselves are copied (transparently)
           to the back buffer.
           When the array is over, and if there was a change in the scene, the entire back-buffer 
           is copied to the screen.

           The Threadentry function calls either one of these techniques.
        */
    //void              SingleRectGameTechnique ();  // Currently out of use for performance reasons !!!
    void                MultiRectGameTechnique ();

    // Message handlers:
    void                AddShell        (CMessage &);
    void                AddBullet       (CMessage &);
    void                AddMine         (CMessage &);
    void                AddBonus        (CMessage &, DWORD);
    void                AddTank         (CMessage &);
    void                RemoveTank      (CMessage &);
    void                AddBoard        (CMessage &);
    void                AddBomber       (CMessage &, DWORD);
    void                SetTankStatus   (CMessage &);
    void                SetTankPos      (CMessage &);
    void                SetTankZombie   (CMessage &);

    // Members:
    CGameObjectsList m_GameObjsList; // dynamic list of participating game objects

    // aliases to global objects:
    TIMER_CLASS   &m_Timer;
    CImageManager& m_ImageManager;
    CMsgQueue&     m_MsgQueue;
    CCommManager  &m_CommManager;

    HWND           m_MapHWnd;               // Window handle of map region (for DrawDIB)
    CRect          m_UpdateRegionRect;      // Update rectangle of last frame
    UINT           m_uFreq,                 // Refresh frequency
                   m_uMilliSleep;           // Refresh wait time (time to sleep between frames).
    CDIB           m_BackBuffer;            // Back (off-screen) buffer
    BOOL           m_bRefreshAll;           // Refresh entire map on next frame ?

    CTankObj      *m_pTanks[MAX_TANKS];     // Points to tank objects (for quick reference)
    int            m_iLocalTankID;          // ID of local tank
    int            m_iNumTanks;             // Counter of current number of tanks
    CCriticalSection m_TanksCS;             // Enable safe pointer access to m_pTanks

    CBonus        *m_pBonus;                // Points to single bonus object (for quick reference)
    DWORD          m_dwBonusTimeout;        // Timeout until a new bonus is generated
    DWORD          m_dwChkSumSignal;        // If changed, a new checksum should be sent
    void           TryToAddBonus (DWORD);   
    BonusType      m_LastBonusType;         // Last type of bonus in use
    CCriticalSection m_BonusCS;             // Enable safe pointer access to m_pBonus
    CCriticalSection m_MinesCS;             // Enables safe multithreaded access to all the mines
};

#include <Tanks.h>
// Inline sections:
#include <GameManager.inl>

#endif
