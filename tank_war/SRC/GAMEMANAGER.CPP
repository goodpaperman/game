/*****************************************************************************
*                                                                             
*   GameManager.cpp                                                            
*                                                                             
*   Electrical Engineering Faculty - Software Lab                             
*   Spring semester 1998                                                      
*                                                                             
*   Tanks game                                                                
*                                                                             
*   Module description: The core of the game - implements the main game loop.
*                       Holds the list of objects participating in the game.
*                       In every game iteration, all the objects on the list
*                       are refreshed, and the game display is updated.
*                       More details on the algorithms in use are described below.
*                                                                             
*   Authors: Eran Yariv - 28484475                                           
*            Moshe Zur  - 24070856                                           
*                                                                            
*                                                                            
*   Date: 23/09/98                                                           
*                                                                            
******************************************************************************/
#include "stdafx.h"
#include <afxmt.h>
#include "tanks.h"
#include "DIB.h"
#include "GameManager.h"
#include "ImageManager.h"
#include "MsgQueue.h"
#include "Bonus.h"
#include "GameBoard.h"
#include "TankObj.h"
#include "Shell.h"
#include "Bullet.h"
#include "Mine.h"
#include "Bomber.h"
#include "Message.h"
#include "GameOver.h"


CGameManager::CGameManager (UINT uFreq) :
        m_Timer(TANKS_APP->m_gTimer),
        m_ImageManager(TANKS_APP->m_gImageManager),
        m_MsgQueue(TANKS_APP->m_gIncomingMsgQueue),
        m_CommManager(TANKS_APP->m_gCommManager),
        m_dwBonusTimeout (0)
{
    VERIFY (SetFrequency (uFreq));
}

BOOL CGameManager::SetFrequency (UINT uFreq)
{
    if (uFreq < MIN_RENDER_FREQ || uFreq > MAX_RENDER_FREQ)
        return FALSE;
    m_uFreq = uFreq;
    m_uMilliSleep = UINT (double(1000.0) / double(m_uFreq));
    return TRUE;
}

UINT CGameManager::ThreadEntry (LPVOID /*lpParam*/)
{
    SetPriority (GAME_MANAGER_THREAD_PRIORITY);
    m_MapHWnd = TANKS_APP->m_gHwndMap;              // Get handle to map (Windows handle)
	ASSERT (NULL != m_MapHWnd);
    m_dwChkSumSignal = 0;                           // Don't send check sum right away
    m_iNumTanks = 0;                                // Initially, no tanks.
    m_iLocalTankID = -1;                            // Initially, no local tank

    HDC dc = ::GetDC (m_MapHWnd);
    m_BackBuffer.CreateEmpty (MAP_WIDTH, MAP_HEIGHT);// Create back (off-screen) buffer
    m_BackBuffer.GetPaletteFromResourceBitmap (IDB_BULLET);
    TANKS_APP->m_gDrawDIB.SetPalette (*m_BackBuffer.m_pPalette);
    TANKS_APP->m_gDrawDIB.Realize (dc, FALSE);
    ::ReleaseDC (m_MapHWnd, dc);

    MultiRectGameTechnique ();                      // Do the game main loop
    DestroyObjects ();                              // Kill list of objects
    return 0;
}

void CGameManager::DestroyObjects ()
{
    m_GameObjsList.KillList();

    m_TanksCS.Lock();
    for (int i=0; i<MAX_TANKS; i++)
        m_pTanks[i] = NULL;
    m_TanksCS.Unlock();

    m_BonusCS.Lock();
    m_pBonus = NULL;
    m_BonusCS.Unlock();

    m_iNumTanks = 0;
}

#pragma warning (disable : 4701)
/* warning C4701: local variable 'CurObjHImg' may be used without having been initialized
   God damn it, trust me on this one - I know what I'm doing here....
*/

/*------------------------------------------------------------------------------

  Function: MultiRectGameTechnique

  Purpose:  Main game loop. Every iteration of the loop, game objects recalc
            their status, and the display is refreshed. Every Iteration lasts a 
            constant period of time, to allow a steady frame rate of the display, 
            using the sleep system call, to stay low on CPU use.

  Input:    None.

  Output:   None.

  Remarks:  The algorithm in use is explained in details in the Programmers's
            manual section in the game's help files.
            

------------------------------------------------------------------------------*/
void 
CGameManager::MultiRectGameTechnique ()
{
#ifdef GATHER_RENDERING_STATS
    DWORD dwLoopCount=0;
    DWORD dwStartTime = GetTickCount();
    int lTotalTimeLeft = 0,                 // For statistics only !
        lMaxTimeLeft = 0,                   // For statistics only !
        lMinTimeLeft = LONG(m_uMilliSleep), // For statistics only !
        iMaxObjs = 0;                       // For statistics only !
#endif  // GATHER_RENDERING_STATS
    BOOL bImageChanged;

    typedef struct {
        HIMAGE      himg;
        CPoint      ObjectPos;
    } UpdateArrayUnit;

    UpdateArrayUnit UpdateArray[MAX_POSSIBLE_OBJECTS];

    UINT uArrIndex;

    LONG lSleepTime;         // Time left to sleep (may be negative if loop is too slow)

    m_bRefreshAll = TRUE;

        // Initially, empty the message queue to add the board object to the game objects list
    EmptyMsgQ(dwStartTime);
        // First, create the constant board in the back-buffer
    LIST_POS lp = m_GameObjsList.GetHeadPosition();
    CGameObject* pBoard = m_GameObjsList.GetNext(lp);
    ASSERT (pBoard->GetType() == BOARD);
    CDIB *pBoardDIB = m_ImageManager.ExposeDIB (pBoard->GetImage());
        // Copy the board directly - no transparency
    m_BackBuffer.CopyFrom (pBoardDIB);
    while (!m_bEndThread)
    {   // Loop while game lasts
            // Get sample time
        DWORD dwCurTime = m_Timer.GetLocalTime();
        DWORD dwLoopStart = dwCurTime;

        AttemptToSendGameChecksum ();

        if (m_CommManager.IsHost())
            TryToAddBonus (dwCurTime);

            // Empty object list requests queue
        bImageChanged = EmptyMsgQ(dwCurTime);

            // Init update rectangle
        m_UpdateRegionRect.SetRectEmpty();

            // Init updates array
        uArrIndex = 0;

        //
        // First pass: loop the objects 
        //

            // Walk over game objects and update rectangle
            // Skip the board:
        LIST_POS lp = m_GameObjsList.GetHeadPosition();
        CGameObject* pGameObj = m_GameObjsList.GetNext(lp);
        ASSERT (pGameObj->GetType() == BOARD);

            // Loop the other (transparent non-board) objects:
        for ( pGameObj = m_GameObjsList.GetNext(lp);
              pGameObj;
              pGameObj = m_GameObjsList.GetNext(lp) )
        {
            StateType CurObjState = pGameObj->CalcState(dwCurTime);
            HIMAGE CurObjHImg;

                // Get new position from current object
            CPoint CurObjPos = pGameObj->GetPos();

                // Get update rectangle from current object
            CRect CurObjUpdateRect = pGameObj -> GetUpdateRectangle();
                // Update region to be union of all rectangles
            m_UpdateRegionRect |= CurObjUpdateRect;

            if (STATE_DEAD == CurObjState)
            {       // Object is no longer with us:
                BOOL fIsMine = FALSE;
                if (pGameObj->GetType() == MINE)
                {
                    fIsMine = TRUE;
                }
                else if (pGameObj->GetType() == TANK)
                {
                    m_TanksCS.Lock();
                    // A tank is dying, remove it from fast pointer array
                    ASSERT (NULL != m_pTanks[pGameObj->GetID()]);   // Tank must still exist
                    int iID = pGameObj->GetID();
                    if (m_iLocalTankID == iID)
                    {   // Local tank is removed
                        m_CommManager.NotifyExplodingTank(m_iLocalTankID);
                        m_iLocalTankID = -1;
                    }
                    m_pTanks[iID] = NULL;
                    m_iNumTanks --;
                    ASSERT (m_iNumTanks >= 0);
                    m_TanksCS.Unlock();
                }
                else if (pGameObj->GetType() == BONUS)
                {
                    m_BonusCS.Lock();
                    // A bonus is dying, remove it from fast pointer
                    if (m_pBonus == pGameObj)
                        // If the current bonus is dying (not a newer one)
                        m_pBonus = NULL;
                    m_BonusCS.Unlock();
                }
                if (fIsMine)
                    m_MinesCS.Lock();
                m_GameObjsList.RemoveObject(pGameObj);
                if (fIsMine)
                    m_MinesCS.Unlock();
                    // If object is removed, map image has changed    
                bImageChanged = TRUE;
            } 
            else 
            {       // STATE_ALIVE: Update update rect size:
                CurObjHImg = pGameObj->GetImage();  // This call can change the bImageChanged of the object
                if (!bImageChanged) 
                        // No change detected yet....
                        // Ask the current object if it changed image
                    bImageChanged = pGameObj->HasImageChanged();
            }
                // Clear back-buffer from previous frame 
            m_BackBuffer.CopyRectFrom (pBoardDIB, 
                                       CurObjUpdateRect.left, 
                                       CurObjUpdateRect.top, 
                                       CurObjUpdateRect.Width(), 
                                       CurObjUpdateRect.Height(), 
                                       CurObjUpdateRect.left, 
                                       CurObjUpdateRect.top);
                // If the object is still alive, add it to the array:
            if (CurObjState == STATE_ALIVE)
            {
                ASSERT (uArrIndex < MAX_POSSIBLE_OBJECTS);
                UpdateArray[uArrIndex].himg = CurObjHImg;
                UpdateArray[uArrIndex].ObjectPos.x = CurObjPos.x;
                UpdateArray[uArrIndex++].ObjectPos.y = CurObjPos.y;
            } 
        }
        
#ifdef GATHER_RENDERING_STATS
        iMaxObjs = max (iMaxObjs, (int)uArrIndex);  // For statistics only!
#endif  // GATHER_RENDERING_STATS

        //
        // Second pass, clear the array
        //

        if (bImageChanged || m_bRefreshAll)
        {
            // The game image has changed - m_UpdateRegionRect holds the change rectangle
            for (UINT i = 0; i < uArrIndex; i++)
                // While there are objects to update in the array
            {
                    // Update back buffer:
                m_ImageManager.DisplayImage (UpdateArray[i].himg, 
                                             &m_BackBuffer, 
                                             UpdateArray[i].ObjectPos);
            }

            if (m_bRefreshAll) 
            {   // First time around ?
                m_bRefreshAll = FALSE; // No more !
                    // Update entire map to display all terrain
                m_UpdateRegionRect.SetRect (0,0, MAP_WIDTH , MAP_HEIGHT);
            }
            // Dump it (flip)
            DumpBackBufferToScreen ();
        }

        lSleepTime = LONG(m_uMilliSleep) - LONG(m_Timer.GetLocalTime() - LONG(dwLoopStart));

#ifdef GATHER_RENDERING_STATS
            // Update counters, timers and statistics:
        dwLoopCount++;
        lTotalTimeLeft += lSleepTime;                    // For statistics only !
        lMaxTimeLeft = max (lMaxTimeLeft, lSleepTime);   // For statistics only !
        lMinTimeLeft = min (lMinTimeLeft, lSleepTime);   // For statistics only !
#endif

        if (lSleepTime > 1)  // Do we need to sleep at all?
            Sleep (lSleepTime);
    }   // End of main rendering loop


#ifdef GATHER_RENDERING_STATS
        // Debug performance dump:
    TRACE ( "\n\t\t\t**** Rendering statistics ****\n");
    TRACE ("%d loops per second\n",
            dwLoopCount / ((GetTickCount() - dwStartTime) / 1000));
    TRACE ("Avg. time left in loop = %f millisecs. Min = %d millisecs, Max = %d millisecs\n",
            double(lTotalTimeLeft) / double(dwLoopCount), lMinTimeLeft, lMaxTimeLeft);
    TRACE ("Max concurrent objects = %d\n\n\n", iMaxObjs);
#endif
}

#pragma warning (default : 4701)

    
/*------------------------------------------------------------------------------

  Function: DumpBackBufferToScreen

  Purpose:  Switched between the back buffer and the front buffer.

  Input:    None.

  Output:   None.

  Remarks:  See game manager algorithm for details.

------------------------------------------------------------------------------*/
void CGameManager::DumpBackBufferToScreen ()
{
    HDC dc = ::GetDC (m_MapHWnd);

    int iWidth  = m_UpdateRegionRect.Width(),
        iHeight = m_UpdateRegionRect.Height();

    m_UpdateRegionRect.top = MAP_HEIGHT - m_UpdateRegionRect.top - iHeight;

    TANKS_APP->m_gDrawDIB.DrawDib ( 
        &m_BackBuffer,                  // Source image
        dc,                             // Device context
        m_UpdateRegionRect.left,        // Destination leftmost corner
        m_UpdateRegionRect.top,         // Destination topmost corner        
        iWidth,                         // Destination width
        iHeight,                        // Destination height
        m_UpdateRegionRect.left,        // Source leftmost corner 
        m_UpdateRegionRect.top,         // Source topmost corner  
        iWidth,                         // Source width
        iHeight,                        // Source height
        0                               // Flags - nothing
    );
    ::ReleaseDC (m_MapHWnd, dc);
    ::ValidateRect (m_MapHWnd, NULL);
}

/*------------------------------------------------------------------------------

  Function: EmptyMsgQ

  Purpose:  Empties the incoming message queue. Messages arrived either internally
            from game objects (e.g. a tank may send add bullet message) or
            externally from the host.

  Input:    dwLoopStartTime: Current loop time (local game time).

  Output:   

  Remarks:   Returns TRUE if any message was processed.

------------------------------------------------------------------------------*/
BOOL
CGameManager::EmptyMsgQ(DWORD dwLoopStartTime)
{
    CMessage Msg;

    while (m_MsgQueue.Dequeue(Msg))
    {   // While there are still messages to process:
        switch (Msg.GetType())
        {
        //
        // Internal messages (Game objects to game manager)
        //
            case CMessage::ADD_OBJECT:      // Add a general object
                AddObject ( Msg.m_UnionData.pGameObj );
                break;
            case CMessage::ADD_GAME_OVER:   // Add game over message
                AddObject (new CGameOver);
                break;
            case CMessage::ADD_SHELL:       // Add a shell
                AddShell (Msg);
                break;
            case CMessage::ADD_BULLET:      // Add a bullet
                AddBullet (Msg);
                break;
            case CMessage::ADD_MINE:        // Add a mine
                AddMine (Msg);
                break;
        //
        // Incoming messages (From server, through the comm manager to the game manager)
        //
            case CMessage::ADD_BONUS: 
                AddBonus (Msg, dwLoopStartTime);
                break;
            case CMessage::ADD_TANK:
                AddTank (Msg);
                break;
            case CMessage::REMOVE_TANK:
                RemoveTank (Msg);
                break;
            case CMessage::ADD_BOARD: 
                AddBoard (Msg);
                break;
            case CMessage::ADD_BOMBER:
                AddBomber (Msg, dwLoopStartTime);
                break;
            case CMessage::SET_TANK_STATUS:
                SetTankStatus (Msg);
                break;
            case CMessage::SET_TANK_POS:
                SetTankPos (Msg);
                break;
            case CMessage::SET_TANK_ZOMBIE:
                SetTankZombie (Msg);
                break;
            default:    // this message isn't for me - bail out:
                ASSERT(FALSE);
        }
    }
    return TRUE;    // Something always changes
}

/*------------------------------------------------------------------------------

  Function: FindVacantRect

  Purpose:  Returns a postion of a vacant rectangle on the game board.

  Input:    size: Required size of a vacant rectangle.
            pt: Position of vacant rectangle found.

  Output:   None.

  Remarks:  This function must succeed (or else it's caught in an endless loop).
            
------------------------------------------------------------------------------*/
void
CGameManager::FindVacantRect(CSize& size, CPoint &pt)
{
        // First check if list is empty - means we are positioning the first tank.
        // For that purpose we preserve the (0,0) location on every board.
        // Reason: the fist REQUEST_TANK message may arrive before the game manager 
        // gets the game board.
    if (m_GameObjsList.GetObjectsCount() == 0) {
        pt.x = 0; pt.y = 0;
        return;
    }
    CRect rect;
    do {
        rect.left   = rand() % (MAP_WIDTH - size.cx + 1);
        rect.top    = rand() % (MAP_HEIGHT - size.cy + 1);
        rect.right  = rect.left + size.cx - 1;
        rect.bottom = rect.top + size.cy - 1;
    } while (!IsRectVacant(rect));
    // Got solution
    pt.x = rect.left;
    pt.y = rect.top;
}

/*------------------------------------------------------------------------------

  Function: GetMinDistanceFromTanks

  Purpose:  Returns the minimal distance from the given point to the tanks in game.
            Method is used when placing a new tank on the game board.

  Input:    pt: The position in inspection.

  Output:   The minimal squared distance to the tanks in game, or MAX_DWORD if no
            tanks is playing.

------------------------------------------------------------------------------*/
DWORD
CGameManager::GetMinDistanceFromTanks (CPoint &pt)
{
    if (0 == m_GameObjsList.GetObjectsCount())
        return MAX_DWORD;   // No objects

    DWORD dwMinDist = MAX_DWORD;
    m_TanksCS.Lock();
    for (int i=0; i<MAX_TANKS; i++)
        if (NULL != m_pTanks[i])
        {   // Found a tank
            CPoint &ptTank = m_pTanks[i]->GetPos();
            DWORD dwCurDist = (ptTank.x - pt.x) * (ptTank.x - pt.x) +
                              (ptTank.y - pt.y) * (ptTank.y - pt.y);
            dwMinDist = min (dwMinDist, dwCurDist);
        }
    m_TanksCS.Unlock();
    return dwMinDist;
}

/*------------------------------------------------------------------------------

  Function: IsRectVacant

  Purpose:  Check that the given game board rectangle is clear of game object.

  Input:    rect: The rectangle in inspection.

  Output:   Return TRUE if rectangle is vacant.

------------------------------------------------------------------------------*/
BOOL
CGameManager::IsRectVacant(CRect& rect)
{
    LIST_POS lp = m_GameObjsList.GetHeadPosition();
    // Make sure list is untouched during search:
    m_GameObjsList.Freeze();
    // First, check if board has that rectangle vacant:
    CGameObject* pGameObj = m_GameObjsList.GetNext(lp);  // GameBoard
    ASSERT(BOARD == pGameObj->GetType());

    if (! ((CGameBoard*)pGameObj)->IsRectVacant(rect) ) {
        m_GameObjsList.Thaw();
        return FALSE;
    }

    CRect TstRect;
    // Now, check with all other objects in LOWER_LEVEL:
    for ( pGameObj = m_GameObjsList.GetNext(lp);
          pGameObj && HIGHER_LEVEL > pGameObj->GetHeight();
          pGameObj = m_GameObjsList.GetNext(lp) ) {
              // On the first intersection - stop and exit:
            if ( TstRect.IntersectRect(pGameObj->CGameObject::GetUpdateRectangle(), rect) )
            {
                m_GameObjsList.Thaw();
                return FALSE;
            }
    }
    m_GameObjsList.Thaw();
    return TRUE;
}

/*------------------------------------------------------------------------------

  Function: TryToAddBonus

  Purpose:  Attempts to add a new bonus object to the game.

  Input:    dwCurTime: Current game time.

  Output:   None.

  Remarks:  A new bonus is added iff the former one has expired. This way we 
            ensure only one bonus is available at a time.

------------------------------------------------------------------------------*/
void 
CGameManager::TryToAddBonus (DWORD dwCurTime)
{
    if (dwCurTime < m_dwBonusTimeout)
        return;     // Not time yet
    ASSERT (m_CommManager.IsHost());    // Non-host machines don't create bonuses
    // Pick bonus timeout
    DWORD ttl    = BONUS_MIN_LIFESPAN + BONUS_LIFESPAN_RANGE * (rand () % 5);  // sleep between 1~3 min. in 30 sec. jumps.
    // Pick bonus type
    BonusType bt = BonusType(1 + (rand() % BONUS_SHIELD));
    // Pick bonus location
    CPoint loc;
    
    FindVacantRect (CSize (BONUS_WIDTH, BONUS_HEIGHT) ,loc);
    // Post a new bonus message:
    m_CommManager.NotifyNewBonus (bt, ttl, loc);

    // Calc time in the future to spawn a new bonus
    m_dwBonusTimeout = dwCurTime + ttl + 2000;
}


/*------------------------------------------------------------------------------

  Function: AttemptToSendGameChecksum

  Purpose:  Sends a check sum representing our local player's game status.
            The host compares the check sum with it's own game status, and
            send updates on mismatches found.

  Input:    None.

  Output:   None.

  Remarks:  The method is called every loop iteration, but execution is completed
            only once every CHKSUM_TIME_GAP_BITS ms.
------------------------------------------------------------------------------*/
void
CGameManager::AttemptToSendGameChecksum ()
{   // This function must be called only from within the gamer manager game loop
    if ((m_Timer.GetLocalTime () >> CHKSUM_TIME_GAP_BITS) <= m_dwChkSumSignal)
        // There's still time
        return;

    if ((-1 == m_iLocalTankID) &&       // No local tank. The game is in GameOver mode.
        !m_CommManager.IsHost())        // Not a judge
        // Game will be over when the user presses any key => send nothing!
        return;

    if (0 == m_dwChkSumSignal)
    {   // First call
        m_dwChkSumSignal = m_Timer.GetLocalTime () >> CHKSUM_TIME_GAP_BITS;
        return;
    }

    m_dwChkSumSignal++; // The checksum for this time-slot is starting

    // Time to check-sum and send

    // Compose checksum message
    CMessage::MessageData msg;

        // Clear all tanks checks sums
    memset (msg.CheckSumParam.TanksChkSums,
            0,
            sizeof (CheckSumParamTag::TankCheckSumParamTag) * MAX_TANKS);

    m_TanksCS.Lock();
    // Get local tanks position:
    if (m_iLocalTankID >= 0)
    {   // Local tank is alive
        CPoint pos = m_pTanks[m_iLocalTankID]->GetPos();
        msg.CheckSumParam.XPos = WORD(pos.x);
        msg.CheckSumParam.YPos = WORD(pos.y);
        msg.CheckSumParam.DeadHostReport = FALSE;
    }
    else
    {   // Reporting tank is host and host is dead
        msg.CheckSumParam.DeadHostReport = TRUE;  // Indicate dead host report
    }
    // Get other info on all tanks, inc. local:
    for (int i=0; i<MAX_TANKS; i++)
        if (m_pTanks[i] != NULL)
        {   // i'th tank exists
            msg.CheckSumParam.TanksChkSums[i].TankExists = TRUE;
            msg.CheckSumParam.TanksChkSums[i].FastFire = m_pTanks[i]->GetFastFire();
            msg.CheckSumParam.TanksChkSums[i].Bullets = WORD(m_pTanks[i]->GetBulletsCount());
            msg.CheckSumParam.TanksChkSums[i].Shells = WORD(m_pTanks[i]->GetShellsCount());
            msg.CheckSumParam.TanksChkSums[i].Mines = WORD(m_pTanks[i]->GetMinesCount());
            msg.CheckSumParam.TanksChkSums[i].Zombie = m_pTanks[i]->IsZombie();
        }

    m_TanksCS.Unlock();
    msg.CheckSumParam.NumberOfTanks = BYTE(m_iNumTanks);

    // Now, the mines:

        // Clear all partial sector checksums:
    memset (msg.CheckSumParam.MinesSectorsChkSum, 
            0,
            sizeof (BYTE) * (MAX_SECTOR + 1));
    
        // Start scanning the list of objects.
    m_MinesCS.Lock();
    LIST_POS lp = m_GameObjsList.GetHeadPosition();
    CGameObject* pGameObj = m_GameObjsList.GetNext(lp);  // GameBoard
    ASSERT(BOARD == pGameObj->GetType());

        // Continue (after the board) and scan for mines (only lower level):
    for ( pGameObj = m_GameObjsList.GetNext(lp);
          pGameObj && (HIGHER_LEVEL > pGameObj->GetHeight());
          pGameObj = m_GameObjsList.GetNext(lp) ) 
    {
        // This is a lower level object
        if (pGameObj->GetType() != MINE)
            continue;

        // This is a mine - nothing can stop us now.
        BYTE bSector = pGameObj->GetSector();
        ASSERT (bSector <= MAX_SECTOR);
        msg.CheckSumParam.MinesSectorsChkSum[bSector] = 
            BYTE(msg.CheckSumParam.MinesSectorsChkSum[bSector] + pGameObj->GetPosCheckSum());
    }
    m_MinesCS.Unlock();
    // Finally, do the bonus flag:
    m_BonusCS.Lock();
    if (m_pBonus != NULL)
        msg.CheckSumParam.ActiveBonusType = m_pBonus->GetBonusType();
    else 
        msg.CheckSumParam.ActiveBonusType = BONUS_NONE;
    m_BonusCS.Unlock();

    // Send it all through the comm. manager
    m_CommManager.NotifyCheckSum (msg);
}

void
CGameManager::GetBonusState(CMessage::MessageData& MsgData)
{
    m_BonusCS.Lock();
    if (m_pBonus) 
    {   // Bonus exists
        MsgData.BonusParam.Type = m_pBonus->GetBonusType();
        CPoint pos(m_pBonus->GetPos());
        MsgData.BonusParam.XPos = pos.x;
        MsgData.BonusParam.YPos = pos.y;
        MsgData.BonusParam.LifeSpan = BONUS_MIN_LIFESPAN + BONUS_LIFESPAN_RANGE;   // Max life span
    }
    else
    {   // Bonus doesn't exist
        MsgData.BonusParam.Type = BONUS_NONE;
        MsgData.BonusParam.LifeSpan = MAX_TANKS;    // No tank has eaten the last bonus
    }
    m_BonusCS.Unlock();
}

BOOL 
CGameManager::GetTankStatus(int iTankID, CMessage::MessageData& MsgData)
{
    BOOL bRes = FALSE;
    m_BonusCS.Lock();
    ASSERT(iTankID < MAX_TANKS);
    if (m_pTanks[iTankID]) {
        bRes = TRUE;
        m_pTanks[iTankID]->GetStatus(MsgData);
    }
    m_BonusCS.Unlock();
    return bRes;
}

BOOL 
CGameManager::GetTankStatusAndPos(int iTankID, CMessage::MessageData& MsgData)
{
    BOOL bRes = FALSE;
    m_BonusCS.Lock();
    ASSERT(iTankID < MAX_TANKS);
    if (m_pTanks[iTankID]) {
        bRes = TRUE;
        m_pTanks[iTankID]->GetStatusAndPos(MsgData);
    }
    m_BonusCS.Unlock();
    return bRes;
}

DWORD
CGameManager::GetMinesInSector (UINT uSector, DWORD *pAllMinesInSector)
{
    DWORD dwCount = 0;
    m_MinesCS.Lock();
    LIST_POS lp = m_GameObjsList.GetHeadPosition();
    CGameObject* pGameObj = m_GameObjsList.GetNext(lp);  // GameBoard
    ASSERT(BOARD == pGameObj->GetType());

        // Continue (after the board) and scan for mines (only lower level):
    for ( pGameObj = m_GameObjsList.GetNext(lp);
          pGameObj && (HIGHER_LEVEL > pGameObj->GetHeight());
          pGameObj = m_GameObjsList.GetNext(lp) ) 
    {
        // This is a lower level object
        if (pGameObj->GetType() != MINE)
            continue;
        // This is a mine - check for sector
        if (pGameObj->GetSector() != uSector)
            continue;   // Mine is not of requested sector
        // This is a mine of requested sector - add it
        
        pAllMinesInSector[dwCount++] = MAKELONG (pGameObj->GetPos().x, pGameObj->GetPos().y);

    }
    m_MinesCS.Unlock();
    return dwCount;
}

void
CGameManager::SetMinesInSector (UINT uSector, DWORD dwNumMines, DWORD *pAllMinesInSector)
{
    m_MinesCS.Lock();
    LIST_POS lp = m_GameObjsList.GetHeadPosition();
    CGameObject* pGameObj = m_GameObjsList.GetNext(lp);  // GameBoard
    ASSERT(BOARD == pGameObj->GetType());

    DWORD dwNewMines = dwNumMines;

        // Continue (after the board) and scan for mines (only lower level):
    for ( pGameObj = m_GameObjsList.GetNext(lp);
          pGameObj && (HIGHER_LEVEL > pGameObj->GetHeight());
          pGameObj = m_GameObjsList.GetNext(lp) ) 
    {
        // This is a lower level object
        if (pGameObj->GetType() != MINE)
            continue;
        // This is a mine - check for sector
        if (pGameObj->GetSector() != uSector)
            continue;   // Mine is not of requested sector
        // This is a mine of requested sector - find it in the list
        BOOL bMineFound = FALSE;
        for (DWORD i=0; i < dwNumMines; i++)
            if ((pGameObj->GetPos().x == LOWORD (pAllMinesInSector[i])) &&
                (pGameObj->GetPos().y == HIWORD (pAllMinesInSector[i])))
            {
                // The mine is found in the list:
                pAllMinesInSector[i] = MAX_DWORD;   // Mark it out
                dwNewMines--;                       // This is not a new mine
                bMineFound = TRUE;
                break;                              // Stop searching the list
            }
        if (!bMineFound)
        {
            // Mine is not in the list but is in the game - Kill it
            pGameObj->Kill();
        }
    }
    m_MinesCS.Unlock();
    // Now the list contains exactly dwNewMines new mines to be added.
    if (0 == dwNewMines)
        return; // Quit now if there are no new mines to add

    for (DWORD i=0; i < dwNumMines; i++)
        if (pAllMinesInSector[i] != MAX_DWORD)
        {   // Mine is in the list and not marked => Add it using a message to the incoming queue
            CMessage::MessageData Params;
            Params.MineParam.wXPos = LOWORD (pAllMinesInSector[i]);
            Params.MineParam.wYPos = HIWORD (pAllMinesInSector[i]);
            VERIFY (m_MsgQueue.Enqueue(CMessage::ADD_MINE, Params));
            if (--dwNewMines == 0)
                // No more new mines, stop scanning the list and exit
                break;
        }
}

// Message handlers:

void 
CGameManager::AddBonus (CMessage &Msg, DWORD dwLoopStartTime)
{
    if (BONUS_NONE == Msg.m_UnionData.BonusParam.Type)
    { // Bonus is being removed - Tank ID is in LifeSpan (MAX_TANKS for no tank)
        if (Msg.m_UnionData.BonusParam.LifeSpan < MAX_TANKS)
        {   // Some tank ate the bonus, tell him about that
            m_TanksCS.Lock();
            if (m_pTanks[Msg.m_UnionData.BonusParam.LifeSpan] != NULL)
                // This tank is alive on this machine - feed the bonus to the tank
                m_pTanks[Msg.m_UnionData.BonusParam.LifeSpan]->EatBonus (m_LastBonusType, 
                                                                                Msg.GetTime());
            m_TanksCS.Unlock();
        }
        m_BonusCS.Lock();
        if (NULL != m_pBonus)
        {   // Bonus didn't timeout yet
            m_pBonus->Kill();   // Remove this bonus object
            m_pBonus = NULL;    // Deref (make room for new bonus right away)    
        }
        m_BonusCS.Unlock();
    }
    else
    { // Bonus is being added
        m_BonusCS.Lock();
        if (NULL != m_pBonus)
            // Make sure there's only one bonus
            m_pBonus->Kill();   // Remove this bonus object
        m_pBonus = new CBonus (
            BonusType(Msg.m_UnionData.BonusParam.Type),
            CPoint (Msg.m_UnionData.BonusParam.XPos, 
                    Msg.m_UnionData.BonusParam.YPos),
            1000L * Msg.m_UnionData.BonusParam.LifeSpan, // Convert from secs to msecs.
            dwLoopStartTime);
        AddObject (m_pBonus);
        m_LastBonusType = BonusType(Msg.m_UnionData.BonusParam.Type);
        m_BonusCS.Unlock();
    }
}

void
CGameManager::AddTank (CMessage &Msg)
{
    m_TanksCS.Lock();
    // The following assertion was removed:
    // Reason:  If this is a client (non-judge) and it connects to a server,
    //          the server sends ADD_TANK after the client first check sum report.
    //          The ADD_TANK may not be fully digested until it's time to send
    //          another checksum report, which will report the tank as still missing
    //          and will cause another ADD_TANK with the same tank ID.
//            ASSERT (NULL == m_pTanks[Msg.m_UnionData.TankParam.ID]);   // Can't add same tank twice
    if (NULL == m_pTanks[Msg.m_UnionData.TankParam.ID])
    {   // First time this tank is seen
        m_pTanks[Msg.m_UnionData.TankParam.ID] = new CTankObj (
            Msg.m_UnionData.TankParam.ID,
            Msg.m_UnionData.TankParam.XPos,
            Msg.m_UnionData.TankParam.YPos,
            Msg.m_UnionData.TankParam.Direction,
            Msg.m_UnionData.TankParam.Local,
            Msg.m_UnionData.TankParam.ShieldLevel,
            Msg.m_UnionData.TankParam.Shells,
            Msg.m_UnionData.TankParam.Bullets,
            Msg.m_UnionData.TankParam.Mines,
            Msg.m_UnionData.TankParam.FireRateBonusSecsLeft);  // @@ fix last parameter @@@
        AddObject (m_pTanks[Msg.m_UnionData.TankParam.ID]);
        m_iNumTanks++;
        ASSERT (m_iNumTanks <= MAX_TANKS);
    }
    else
    {   
        //
        // An already existing tank was added. Ignore, next checksum will fix tank properties
        //
    }

    // If this our local tank - attach the keyboard to the manouver set:
    if (Msg.m_UnionData.TankParam.Local)
    {   // Local tank
        TANKS_APP->m_gKbdManager.SetManouverSet 
            (&(TANKS_APP->m_gManouverSets[Msg.m_UnionData.TankParam.ID]));
        m_iLocalTankID = Msg.m_UnionData.TankParam.ID;
    }
    else
    {   // Adding a remote tank, make sure the manouver set is clear
        TANKS_APP->m_gManouverSets[Msg.m_UnionData.TankParam.ID].Clear();
    }
    m_TanksCS.Unlock();
}

void
CGameManager::RemoveTank (CMessage &Msg)
{
    m_TanksCS.Lock();
    CTankObj *pTank = m_pTanks[Msg.m_UnionData.TankRemoveParam.ID];
    if (pTank &&                    // If tank exists and
        !pTank->IsExploding())      // it is not already exploding
    {
        pTank->Kill();
    }
    m_TanksCS.Unlock();
}

void
CGameManager::AddBoard (CMessage &Msg)
{
    CGameBoard *pGameBoard = new CGameBoard ();
    pGameBoard->GenerateBoard (
        Msg.m_UnionData.BoardParam.Seed,
        Msg.m_UnionData.BoardParam.Complexity);
    AddObject (pGameBoard);
}

void
CGameManager::AddShell (CMessage &Msg)
{
    AddObject(new CShell (
        Msg.m_UnionData.ShellParam.wXPos,
        Msg.m_UnionData.ShellParam.wYPos,
        Msg.m_UnionData.ShellParam.bDirectionIndex,
        Msg.m_UnionData.ShellParam.bParentTankID));
}

void
CGameManager::AddBullet (CMessage &Msg)
{
    AddObject (new CBullet (
        Msg.m_UnionData.BulletParam.wXPos,
        Msg.m_UnionData.BulletParam.wYPos,
        Msg.m_UnionData.BulletParam.bDirectionIndex,
        Msg.m_UnionData.BulletParam.bParentTankID));
}

void
CGameManager::AddBomber (CMessage &Msg, DWORD dwLoopStartTime)
{
    // Create and add a new flying bomber
    AddObject (new CBomber (Msg.m_UnionData.BomberParam.Direction,
                            Msg.m_UnionData.BomberParam.Xpos,
                            Msg.m_UnionData.BomberParam.Ypos,
                            dwLoopStartTime));
}

void
CGameManager::AddMine (CMessage &Msg)
{
    m_MinesCS.Lock();
    AddObject (new CMine (Msg.m_UnionData.MineParam.wXPos,
                          Msg.m_UnionData.MineParam.wYPos));
    m_MinesCS.Unlock();
}


void
CGameManager::SetTankStatus (CMessage &Msg)
{
    ASSERT(Msg.m_UnionData.TankStatusParam.bID < MAX_TANKS);
    if (m_pTanks[Msg.m_UnionData.TankStatusParam.bID])
    {   // Tank exists:
        // Set its status:
        m_pTanks[Msg.m_UnionData.TankStatusParam.bID]->SetStatus(Msg.m_UnionData);
    }
}

void
CGameManager::SetTankPos (CMessage &Msg)
{
    ASSERT(Msg.m_UnionData.TankPosParam.ID < MAX_TANKS);
    if (m_pTanks[Msg.m_UnionData.TankPosParam.ID])
    {   // Tank exists:
        // TODO: The following assert fails, that the reason it's marked out:
        //ASSERT (Msg.m_UnionData.TankPosParam.ID != UINT(m_iLocalTankID)); 
        // This msg shouldn't be send back to sender
        if (Msg.m_UnionData.TankPosParam.ID == UINT(m_iLocalTankID))
            return;
        m_pTanks[Msg.m_UnionData.TankPosParam.ID]->
            SetPos (Msg.GetTime(),
                    Msg.m_UnionData.TankPosParam.XPos,  // Set its position
                    Msg.m_UnionData.TankPosParam.YPos); // according the time stamp
    }
}

void
CGameManager::SetTankZombie (CMessage &Msg)
{
    ASSERT(Msg.m_UnionData.TankZombieParam.ID < MAX_TANKS);
    if (m_pTanks[Msg.m_UnionData.TankZombieParam.ID])  // Tank exists:
        m_pTanks[Msg.m_UnionData.TankZombieParam.ID]->
            SetZombie (Msg.m_UnionData.TankZombieParam.Zombie);
}
