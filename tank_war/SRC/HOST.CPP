/*****************************************************************************
*                                                                             
*   Host.cpp                                                            
*                                                                             
*   Electrical Engineering Faculty - Software Lab                             
*   Spring semester 1998                                                      
*                                                                             
*   Tanks game                                                                
*                                                                             
*   Module description: Handles the network messages directed to the host of 
*                       the game session. Acts as the game judge using the game
*                       manager of the local player on this machine.
*                       
*                                                                             
*   Authors: Eran Yariv - 28484475                                           
*            Moshe Zur  - 24070856                                           
*                                                                            
*                                                                            
*   Date: 23/09/98                                                           
*                                                                            
******************************************************************************/
#include "stdafx.h"
#include "Tanks.h"
#include "Host.h"

#include <time.h>
#include <math.h>

#ifdef GATHER_SYNC_STATS

    #define INC_SYNC_MSG(t)                     m_TanksSyncStats[(t)].dwSyncMsgsArrived++;
    #define INC_SYNC_MSG_SIZE(t,sz)             m_TanksSyncStats[(t)].dwTotSyncMsgSize+=(sz);
    #define INC_BONUS_MISMATCH(t)               m_TanksSyncStats[(t)].dwBonusMismatch++;
    #define INC_MINES_MISMATCH(t)               m_TanksSyncStats[(t)].dwMinesMismatch++;
    #define INC_ZOMBIE_MISMATCH(t)              m_TanksSyncStats[(t)].dwZombieMismatch++;
    #define INC_OTHER_TANK_STATUS_MISMATCH(t)   m_TanksSyncStats[(t)].dwOtherTankStatusMismatch++;
    #define INC_MISSING_TANK(t)                 m_TanksSyncStats[(t)].dwMissingTanks++;
    #define INC_EXTRA_TANKS(t)                  m_TanksSyncStats[(t)].dwExtraTanks++;
    #define INC_ZOMBIE_IN(t)                    m_TanksSyncStats[(t)].dwZombieIn++;
    #define INC_ZOMBIE_OUT(t)                   m_TanksSyncStats[(t)].dwZombieOut++;

    #define UPDATE_SYNC_MSG_TIME(t)             UpdateSyncMsgTime((t));
    #define ZERO_TANK_SYNC_STATS(t)             ZeroTankSyncStats((t));
    #define PRINT_TANK_SYNC_STATS(t)            PrintTankSyncStats((t));

#else   // !defined GATHER_SYNC_STATS

    #define INC_SYNC_MSG(t)    
    #define INC_SYNC_MSG_SIZE(t,sz)                 
    #define INC_BONUS_MISMATCH(t)               
    #define INC_MINES_MISMATCH(t)               
    #define INC_ZOMBIE_MISMATCH(t)
    #define INC_OTHER_TANK_STATUS_MISMATCH(t)   
    #define INC_MISSING_TANK(t)                 
    #define INC_EXTRA_TANKS(t)                  
    #define INC_ZOMBIE_IN(t)    
    #define INC_ZOMBIE_OUT(t)   

    #define UPDATE_SYNC_MSG_TIME(t)             
    #define ZERO_TANK_SYNC_STATS(t)             
    #define PRINT_TANK_SYNC_STATS(t)            

#endif // defined GATHER_SYNC_STATS


CHost::CHost (CCommManager &CommManager) : 
    m_CommManager(CommManager),
    m_GameManager(TANKS_APP->m_gGameManager),
    m_Timer (TANKS_APP->m_gTimer),
    m_Message(CommManager.ExposeMessage()),
    m_bComplexity (BYTE(TANKS_APP->GetStoredMapComplexity())),
#if defined (REAL_RANDOM)
    m_wSeed (WORD(time( NULL )))   // Randomness according to current time
#else
    m_wSeed (WORD(10))
#endif
{
    for (int i = 0; i < MAX_TANKS; i++)
    {
        m_aPlayersID[i] = CCommManager::INVALID_PLAYER_ID;
        m_JudgeViewOfTanks[i].Exists = FALSE;
        m_JudgeViewOfTanks[i].Zombie = FALSE;
        m_JudgeViewOfTanks[i].BadRTTCount = 0;
        m_JudgeViewOfTanks[i].LastRTTWasGood = TRUE;
    }
}

CHost::~CHost()
{
    for (UINT i = 0; i < MAX_TANKS; i++)
        if (CCommManager::INVALID_PLAYER_ID != m_aPlayersID[i])
            // Player still exists upon termination - display its sync stats
            PRINT_TANK_SYNC_STATS (i);
}


/*------------------------------------------------------------------------------

  Function: HandleRequestTank

  Purpose:  Handles the tank request message send by a new game player.
            Position the new tank in a vacant and safe (far enough from all 
            other tanks) place, and maps a vacant ID for the player (the 
            requested ID if still vacant). The ADD_TANK message is then send to 
            all game players. 
            This is also a trigger for the ADD_BOARD message to the new player.

  Input:    bReqTankID - the players requested ID (defines the tanks color).

  Output:   None.

  Remarks:  
------------------------------------------------------------------------------*/
void
CHost::HandleRequestTank (BYTE bReqTankID)
{
    CMessage::MessageData MsgData;
    //
    //  Prepare add tank message
    //
    int x,y,dir;
        // Find a safe and vacant position, and point the tank toward the 
        // center of the map:
    LocateNewTankPos (x,y,dir);
    MsgData.TankParam.XPos = x;
    MsgData.TankParam.YPos = y;
    MsgData.TankParam.Direction = dir;
        // Get a vacant ID:
    MsgData.TankParam.ID = MapAndGetAvailTankID(bReqTankID); 
    MsgData.TankParam.Local = FALSE;
    MsgData.TankParam.ShieldLevel = TANK_INIT_SHIELD_LEVEL;
    MsgData.TankParam.Shells = TANK_INIT_SHELLS;
    MsgData.TankParam.Bullets = TANK_INIT_BULLETS;
    MsgData.TankParam.Mines = TANK_INIT_MINES;
    MsgData.TankParam.FireRateBonusSecsLeft = 0;
    MsgData.TankParam.Bomber = FALSE;
    MsgData.TankParam.FastFire = FALSE;
    CMessage Msg(CMessage::ADD_TANK, MsgData, SERVER_MSG);

    m_Message.m_dwLength = Msg.GetBuffer(m_Message.m_aBuffer);
    //
    // Send add tank message
    //
    SendToAllButSender ();    // All players get add remote tank
    // Requesting tank get add local tank:
    Msg.m_UnionData.TankParam.Local = TRUE;
    Msg.GetBuffer(m_Message.m_aBuffer);

    ReplyToSender ();
    PlayerJoinsGame (MsgData.TankParam.ID);
    //
    //  Add board message:
    //
    MsgData.BoardParam.Seed = m_wSeed;
    MsgData.BoardParam.Complexity = m_bComplexity;
    CMessage BoardMsg (CMessage::ADD_BOARD, MsgData, SERVER_MSG);
    m_Message.m_dwLength = BoardMsg.GetBuffer(m_Message.m_aBuffer);

    ReplyToSender ();
}


/*------------------------------------------------------------------------------

  Function: ValueCloseEnough

  Purpose:  A hueristic comparison function. If one of the values is smaller 
            then the minimal tolerance allowed, then perform an accurate 
            comparison, otherwise - allways return "equal".

  Input:    val1, val2 - the values to be compared
            MinTolerance - the value which underneath, an accurate comparison
                           should be performed.

  Output:   Return TRUE if values are "equal"

  Remarks:  Used to decide if a remote player needs an update after sending its
            game checksum.
------------------------------------------------------------------------------*/
BOOL 
CHost::ValueCloseEnough (int val1, int val2, int MinTolerance)
{
    if ((val1 < MinTolerance) || (val2 < MinTolerance))
        return (val1 == val2);
    return TRUE;
}

    
/*------------------------------------------------------------------------------

  Function: HandleCheckSum

  Purpose:  Handles the check sum message. The message contains the remote 
            player's game status, and is checked against the host game status.
            Updates are send only when the mismatch affects the remote player's
            game severely.

  Input:    msg - the decoded check sum message
            bJudgeReporting - flag indicating the sender is the local player on
                              the host machine (nick name - judge)

  Output:   None.

  Remarks:  The reporting tank's position is dispatched to all player 
            immediatly.
------------------------------------------------------------------------------*/
void 
CHost::HandleCheckSum (CMessage &msg, BOOL bJudgeReporting)
{
    int i;  // For loops
    int iReportingTank = LookupTankID (m_idFrom);
        // This flag indicates that the host of the session is still working,
        // but its tank was destroyed, means we can ignore some parts of the
        // message:
    BOOL bDeadHostReport = msg.m_UnionData.CheckSumParam.DeadHostReport;

    ASSERT (!bDeadHostReport || bJudgeReporting);

    if (!bDeadHostReport)
    {
        UPDATE_SYNC_MSG_TIME (iReportingTank);
        INC_SYNC_MSG (iReportingTank);
        INC_SYNC_MSG_SIZE (iReportingTank,
                           sizeof (FixedSizeChkSumType) + 
                           msg.m_UnionData.CheckSumParam.NumberOfTanks * 
                            sizeof (SingleTankChkSumType));
    }

    // Update the judge's view of other tanks existance:
    for (i=0; i<MAX_TANKS; i++)
        m_JudgeViewOfTanks[i].Exists = m_GameManager.IsAlive (i);

    ASSERT(bJudgeReporting ||   // Either dead judge reports or reporting tank exists
           msg.m_UnionData.CheckSumParam.TanksChkSums[iReportingTank].TankExists);


    if (!bDeadHostReport)
    {   // The reporting tank is alive -
        // 1st we store it's ammo counts and FF:
        m_JudgeViewOfTanks[iReportingTank].Shells = 
            msg.m_UnionData.CheckSumParam.TanksChkSums[iReportingTank].Shells;
        m_JudgeViewOfTanks[iReportingTank].Bullets = 
            msg.m_UnionData.CheckSumParam.TanksChkSums[iReportingTank].Bullets;
        m_JudgeViewOfTanks[iReportingTank].Mines = 
            msg.m_UnionData.CheckSumParam.TanksChkSums[iReportingTank].Mines;
        m_JudgeViewOfTanks[iReportingTank].FastFire = 
            msg.m_UnionData.CheckSumParam.TanksChkSums[iReportingTank].FastFire;

        // Than we send to all other it's position:
        SendTankPos(iReportingTank,
                    msg.m_UnionData.CheckSumParam.XPos,
                    msg.m_UnionData.CheckSumParam.YPos);
    }
    // Finally we compare the reporting tank's view of others' ammo counts:
    for (i = 0; i<MAX_TANKS; i++) {
        // Skip reporting tank
        if (i == iReportingTank)
            continue;
        // Update reporting tank with tanks he isn't aware off:
        if (!m_JudgeViewOfTanks[i].Exists &&
            msg.m_UnionData.CheckSumParam.TanksChkSums[i].TankExists)
        {
            SendRemoveTank(i);
            INC_EXTRA_TANKS(i)
            continue;
        } else
        if (m_JudgeViewOfTanks[i].Exists &&
            !msg.m_UnionData.CheckSumParam.TanksChkSums[i].TankExists)
        {
            SendAddTank(i);
            INC_MISSING_TANK(i)
            continue;
        }
        // Now both the reporter and the judge agree upon tanks existance / inexistance
        if (!m_JudgeViewOfTanks[i].Exists)
            continue;
        // TODO: this comparison may be to strict, since we ignore the
        // reporting time of the counter. We need to consider a more tolerant
        // comparison.
        if (!ValueCloseEnough (m_JudgeViewOfTanks[i].Shells,
                               msg.m_UnionData.CheckSumParam.TanksChkSums[i].Shells,
                               3) ||
            !ValueCloseEnough (m_JudgeViewOfTanks[i].Bullets,
                               msg.m_UnionData.CheckSumParam.TanksChkSums[i].Bullets,
                               6) ||
            !ValueCloseEnough (m_JudgeViewOfTanks[i].Mines,
                               msg.m_UnionData.CheckSumParam.TanksChkSums[i].Mines,
                               2) ||
            (m_JudgeViewOfTanks[i].FastFire != 
             msg.m_UnionData.CheckSumParam.TanksChkSums[i].FastFire)
             )
        {
            SendTankStatus(i);
            INC_OTHER_TANK_STATUS_MISMATCH(i)
        }
        // Update zombies:
        if (m_JudgeViewOfTanks[i].Zombie != 
                msg.m_UnionData.CheckSumParam.TanksChkSums[i].Zombie)
        {
            SendTankZombie(i, m_JudgeViewOfTanks[i].Zombie, FALSE);
            INC_ZOMBIE_MISMATCH (i)
        }
    }

    // A judge gets to set a few things and other just follow.
    // A judge is the nickname of the local player on the server's machine.
    if (bJudgeReporting)
    {   // The judge sets the following game checksums for the others to compare with:
        // Save current bonus status:
        m_JudgeViewOfBonus = msg.m_UnionData.CheckSumParam.ActiveBonusType;
        // Save current game sectors mine statuses:
        memcpy (m_JudgeViewOfMines, 
                msg.m_UnionData.CheckSumParam.MinesSectorsChkSum, 
                sizeof(BYTE) * (MAX_SECTOR + 1));
    }
    else
    {   // Non-judge, compare with judge settings
        // Compare current bonus status:
        if (msg.m_UnionData.CheckSumParam.ActiveBonusType != 
            m_JudgeViewOfBonus)
        {   // Bonus type mismatch
            SendBonusStatus();
            INC_BONUS_MISMATCH (iReportingTank);
        }
        // Compare current game sectors mine statuses:
        for (i=0; i <= MAX_SECTOR; i++)
            if (m_JudgeViewOfMines[i] !=
                msg.m_UnionData.CheckSumParam.MinesSectorsChkSum[i])
            {   // Sector i mines mismatch
                SendMinesStatus (i);
                INC_MINES_MISMATCH (iReportingTank);
            }
    }
}

/*------------------------------------------------------------------------------

  Function: LocateNewTankPos

  Purpose:  Finds a position for the new tank. Make sure the rectangle is vacant
            and that the distance from other tanks is suitable. Calculates the 
            tanks direction to point to the center of the map.

  Input:    XPos,YPos, Dir - output params.

  Output:   None.

  Remarks:  
------------------------------------------------------------------------------*/
void
CHost::LocateNewTankPos(int &XPos, int &YPos, int &Dir)
{
#define PI              double( 3.14159265359 )
#define MAX_TRIES        10
#define MIN_SQR_DIST     (100 * 100)

    DWORD dwMaxDist = 0;
    CSize size(TANK_WIDTH, TANK_HEIGHT);
    CPoint BestPos;

    for (int i=0; i<MAX_TRIES; i++)
    {   // Start looking
        CPoint pos;

        TANKS_APP->m_gGameManager.FindVacantRect(size, pos);

        DWORD dwCurDist = TANKS_APP->m_gGameManager.GetMinDistanceFromTanks (pos);
        if (MIN_SQR_DIST < dwCurDist)
        {   // We found a spot that satisfies us - stop looking
            BestPos = pos;
            break;
        }
        if (dwMaxDist < dwCurDist)
        {   // Found better position
            dwMaxDist = dwCurDist;
            BestPos = pos;
        }
    }

    XPos = BestPos.x;
    YPos = BestPos.y;
    // Let's place all tanks facing the center of the board:
    double dx = XPos - (MAP_WIDTH / 2);
    double dy = YPos - (MAP_HEIGHT / 2);
    if (0.0 == dx)
    {   // Either point up or down
        Dir = (dy > 0.0) ? 18 : 6;
    }
    double alpha = atan2 (dy, dx) * 180.0 / PI;     // -180 <= alpha <= 180

    Dir = (MAX_DIRECTIONS - int(alpha / 15.0)) % MAX_DIRECTIONS;
    ASSERT (Dir >= 0 && Dir < MAX_DIRECTIONS);
    if (Dir < 0 || Dir >= MAX_DIRECTIONS)
        Dir = 0;
}

/*------------------------------------------------------------------------------

  Function: MapAndGetAvailTankID

  Purpose:  Get the new tank's ID. Try to satisfy the player's request.

  Input:    bReqTankID - the requested tank ID.

  Output:   The new tanks ID.

  Remarks:  
------------------------------------------------------------------------------*/
BYTE
CHost::MapAndGetAvailTankID(BYTE bReqTankID)
{
    BYTE bInd = bReqTankID; // Assume we take the request

    if (m_aPlayersID[bInd] != CCommManager::INVALID_PLAYER_ID) {
        // We can't satisfy the request - lets find a vacant place:
        for (int i = 0; i < MAX_TANKS; i++)
            if (CCommManager::INVALID_PLAYER_ID == m_aPlayersID[i])
                break;

        ASSERT(i != MAX_TANKS);     // We must have a vacant place
        if (i == MAX_TANKS)         // In case of emergency - take any place:
            i = bReqTankID;

        bInd = BYTE(i);
    }

    m_aPlayersID[bInd] = m_idFrom;
    return bInd;
}

int  
CHost::LookupTankID (DPID id)
{   // Returns 0..MAX_TANKS-1 or -1 (no found) or -2 (host)
    if (id == m_CommManager.GetHostID())
        return -2; //Host
    for (UINT i = 0; i < MAX_TANKS; i++)
        if (id == m_aPlayersID[i])
            return int(i);

    return -1; // Not found
}


/*------------------------------------------------------------------------------

  Function: HandleMessage

  Purpose:  Handle message send to the game host from a game player (either 
            remote or local).

  Input:    idFrom - the DirectPlay ID of the sending player
            bFromJudge - flag indicating the message was send by the local tank.

  Output:   None.

  Remarks:  
------------------------------------------------------------------------------*/
void
CHost::HandleMessage(DPID idFrom, BOOL bFromJudge)
{
        // Save message originator locally for reply options:
    m_idFrom = idFrom;

        // Decode the buffer into a message:
    CMessage Msg(m_Message.m_aBuffer, (BYTE)m_Message.m_dwLength, SERVER_MSG);

        // Update table of player's RTTs:
    UpdatePlayerRTT (Msg.GetTime());

    switch (Msg.GetType()) {
        case CMessage::MANOUVER_SET:// All players get the manouver set (excluding the sender)
            // One of the tanks is dispatching his manouver set notification to all:
            // The message contains the data that should be
            // send to all other tanks except the sender of this message.
            // The message params are exactly the same, so we simply re-dispatch it!
            NET_GAME_TRACE (("Host got MANOUVER_SET from tank %d (game time = %d)", 
                            LookupTankID(m_idFrom), Msg.GetTime ()));
            SendToAllButSender();    
            break;

        case CMessage::REMOVE_TANK: // Tell all the players a tank dies
            NET_GAME_TRACE (("Host got REMOVE_TANK from tank %d (game time = %d)", 
                            LookupTankID(m_idFrom), Msg.GetTime ()));
            SendToAllButSender ();
            break;
            
        case CMessage::ADD_BONUS:   // Tell all the players a bonus is added / eaten
            // Set message time to current game time:
            *(PDWORD(&m_Message.m_aBuffer[1])) = TANKS_APP->m_gTimer.GetLocalTime();
            NET_GAME_TRACE (("Host got ADD_BONUS from tank %d (game time = %d)", 
                            LookupTankID(m_idFrom), *(PDWORD(&m_Message.m_aBuffer[1]))));
            SendToAll ();
            break;

        case CMessage::ADD_BOMBER:  // Tell all the players a bomber is in the air
            NET_GAME_TRACE (("Host got ADD_BOMBER from tank %d (game time = %d)", 
                            LookupTankID(m_idFrom), Msg.GetTime ()));
            SendToAll ();
            break;

        case CMessage::REQUEST_TANK:    // A new tank (player) is joining the game session:
            NET_GAME_TRACE (("Host got REQUEST_TANK from tank %d (game time = %d)", 
                            LookupTankID(m_idFrom), Msg.GetTime ()));
            HandleRequestTank (BYTE(Msg.m_UnionData.TankRequestParam.bID));
            break;

        case CMessage::CHECK_SUM:       // Check sum message from a remote or local player
            NET_SYNC_TRACE (("Host got CHECK_SUM from tank %d (game time = %d)", 
                            LookupTankID(m_idFrom), Msg.GetTime ()));
            HandleCheckSum (Msg, bFromJudge);
            break;

        case CMessage::CHAT:            // Dispatch msg to all players except sender:
            SendToAll();
            break;

        default:
            NET_GAME_TRACE (("Host got unknown game message from tank %d (game time = %d)", 
                            LookupTankID(m_idFrom), Msg.GetTime ()));
            ASSERT(FALSE);
    }
}

void
CHost::RemovePlayerFromArray(DPID idPlayer)
{
    int i = LookupTankID (idPlayer);
    ASSERT (i >= 0);
    if (i < 0)  // This should never happen
        return;

    PRINT_TANK_SYNC_STATS (i);

    m_aPlayersID[i] = CCommManager::INVALID_PLAYER_ID;
    if (m_GameManager.IsAlive (i)) 
    {    // Non-gracefull player disconnection happaned
        CMessage::MessageData Params;
            // Create tank removal notification message:
        Params.TankRemoveParam.ID = BYTE(i);
        Params.TankRemoveParam.Explode = TRUE;
        // Compose message from data:
        CMessage Msg(CMessage::REMOVE_TANK, Params, SERVER_MSG);
        // Pack message to buffer:
        m_Message.m_dwLength = Msg.GetBuffer(m_Message.m_aBuffer);
        SendToAll ();
    }
}

void
CHost::SendToAllButSender()
{
    for (UINT i = 0; i < MAX_TANKS; i++)
        if ((m_aPlayersID[i] != m_idFrom) &&
            (m_aPlayersID[i] != CCommManager::INVALID_PLAYER_ID)) {
                m_CommManager.SendAsHost (m_aPlayersID[i]);
        }
}

void
CHost::SendToAll ()
{
    for (UINT i = 0; i < MAX_TANKS; i++)
        if (m_aPlayersID[i] != CCommManager::INVALID_PLAYER_ID) 
        {   // A valid player
                m_CommManager.SendAsHost (m_aPlayersID[i]);    // Send as host
        }
}

void 
CHost::SendBonusStatus()
{
    CMessage::MessageData MsgData;

    // Get Bonus state from GameManager:
    m_GameManager.GetBonusState(MsgData);
    // Compose new message from data:
    CMessage Msg(CMessage::ADD_BONUS, MsgData, SERVER_MSG);
    // Pack message to buffer:
    m_Message.m_dwLength = Msg.GetBuffer(m_Message.m_aBuffer);
    // Send to last reported:
    ReplyToSender();
}

void 
CHost::SendAddTank(int iTankID)
{
    CMessage::MessageData MsgData;
    // Get tank's state and position:
    BOOL bRes = m_GameManager.GetTankStatusAndPos(iTankID, MsgData);

    if (!bRes)
        return; // Tank just died between our fingers...
    // Under no-circumstances should a remote player get it's local tanks this way:
    MsgData.TankParam.Local = FALSE;
    CMessage Msg(CMessage::ADD_TANK, MsgData, SERVER_MSG);
    // Pack message to buffer:
    m_Message.m_dwLength = Msg.GetBuffer(m_Message.m_aBuffer);
    // Send to last reporter:
    ReplyToSender();
}

void
CHost::SendTankPos 
(   int iTankID,                // Setting position of tank ID
    int nXPos,                  // X Pos
    int nYPos)                  // Y Pos
{
    CMessage::MessageData MsgData;

    MsgData.TankPosParam.XPos = nXPos;
    MsgData.TankPosParam.YPos = nYPos;
    MsgData.TankPosParam.ID   = iTankID;
    // Compose message from data:
    CMessage Msg(CMessage::SET_TANK_POS, MsgData, SERVER_MSG);
    // Pack message to buffer:
    m_Message.m_dwLength = Msg.GetBuffer(m_Message.m_aBuffer);
    // Send to all other tanks, except reporter:
    SendToAllButSender();
}

void
CHost::SendTankZombie(int iTankID, BOOL bZombie, BOOL bToAll)
{
    CMessage::MessageData MsgData;

    MsgData.TankZombieParam.ID = BYTE(iTankID);
    MsgData.TankZombieParam.Zombie = BYTE(bZombie);
    // Compose message from data:
    CMessage Msg(CMessage::SET_TANK_ZOMBIE, MsgData, SERVER_MSG);
    // Pack message to buffer:
    m_Message.m_dwLength = Msg.GetBuffer(m_Message.m_aBuffer);
    if (bToAll) 
    {   // Send to all tanks:
        SendToAll();
    } else
    {
        ReplyToSender();
    }
}

void
CHost::SendTankStatus(int iTankID)
{
    CMessage::MessageData MsgData;

    // Get tank's status:
    BOOL bRes = m_GameManager.GetTankStatus(iTankID, MsgData);

    if (! bRes)
        return;
    // Override the judge's game-manager ammo counters and FF flag, with
    // data from the judge's tables:
    MsgData.TankParam.Shells    = m_JudgeViewOfTanks[iTankID].Shells;
    MsgData.TankParam.Bullets   = m_JudgeViewOfTanks[iTankID].Bullets;
    MsgData.TankParam.Mines     = m_JudgeViewOfTanks[iTankID].Mines;
    MsgData.TankParam.FastFire  = 
        m_JudgeViewOfTanks[iTankID].FastFire;

    // Under no-circumstances should a remote player get it's local tanks this way:
    MsgData.TankParam.Local = FALSE;
    // Compose message from data:
    CMessage Msg(CMessage::SET_TANK_STATUS, MsgData, SERVER_MSG);
    // Pack message to buffer:
    m_Message.m_dwLength = Msg.GetBuffer(m_Message.m_aBuffer);
    // Send to last reporter:
    ReplyToSender();
}

void
CHost::SendRemoveTank(int iTankID)
{
    CMessage::MessageData MsgData;

    MsgData.TankRemoveParam.ID = BYTE(iTankID);
    MsgData.TankRemoveParam.Explode = TRUE;
    // Compose message from data:
    CMessage Msg(CMessage::REMOVE_TANK, MsgData, SERVER_MSG);
    // Pack message to buffer:
    m_Message.m_dwLength = Msg.GetBuffer(m_Message.m_aBuffer);
    // Send to last reporter:
    ReplyToSender();
}

void CHost::SendMinesStatus(int iSector)
{
    /*  NOTICE: This is a special case. Since the information on all the mines in
        a given sector is of variable length and may get very big, we don't construct
        a structure for it in the MessageData union like we do for all the other
        message type.
        Instead, we make the encoding in-place directly to the DPlay output buffer
        (using a static member function of CMessage) and don't keep any message structure.
    */
    DWORD AllMinesInSector[MAX_MINES_PER_SECTOR];

    // Fill array of positions with the data of all the existing mines in the given sector
    DWORD dwMinesFound = m_GameManager.GetMinesInSector (iSector, AllMinesInSector);

    m_Message.m_aBuffer[0] = CMessage::SET_MINES;   // Set message type in 1st byte
 
    m_Message.m_dwLength = 1 +  // The message type takes 1 byte
        CMessage::EncodeSectorMines (&m_Message.m_aBuffer[1],  
                                     iSector, 
                                     dwMinesFound, 
                                     AllMinesInSector);
    // Send to last reporter:
    ReplyToSender();
}


void
CHost::PlayerJoinsGame (UINT uID)
{
    memset (&m_PlayersRTT[uID], 0, sizeof (PlayerRTT));
    m_PlayersRTT[uID].dwPlayerJoinTime  = 
    m_PlayersRTT[uID].dwLastMsgTime     = GetTickCount();
    m_PlayersRTT[uID].dwPlayerMinRTT = MAX_DWORD; 
    ZERO_TANK_SYNC_STATS (uID);
}

/*------------------------------------------------------------------------------

  Function: UpdatePlayerRTT

  Purpose:  Calculates the round trip time from our host to the reporting player.
            Check if the zombie status should be updated.

  Input:    dwMsgTime - the time stamp of the message

  Output:   None.

  Remarks:  A tank becomes zombie if 5 sequential messages have RTT larger then
            the maximal allowed (this parameter is configurable via the 
            registry, and is set to 1.5 sec. by default).


------------------------------------------------------------------------------*/
void 
CHost::UpdatePlayerRTT (DWORD dwMsgTime)
{
    int i = LookupTankID (m_idFrom);
    if (0 > i)
        // Tank ID not associated yet   
        return;

    DWORD dwNow = m_Timer.GetLocalTime();
    DWORD dwCurLat = (dwNow >= dwMsgTime) ?
        (dwNow - dwMsgTime) : (dwMsgTime - dwNow);

    m_PlayersRTT[i].dwLastMsgTime = dwNow;
    m_PlayersRTT[i].dwPlayerCurRTT = dwCurLat;
    
    // TRACE ("          Now = %d, MsgTime = %d,  RTT = %d\n", dwNow, dwMsgTime, dwCurLat);

#ifdef GATHER_SYNC_STATS
    m_PlayersRTT[i].dwPlayerMaxRTT = max (
        m_PlayersRTT[i].dwPlayerMaxRTT, dwCurLat);
    m_PlayersRTT[i].dwPlayerMinRTT = min (
        m_PlayersRTT[i].dwPlayerMinRTT, dwCurLat);
    m_PlayersRTT[i].dwPlayerTotalRTTs += dwCurLat;
    m_PlayersRTT[i].dwTotMsgs++;
    m_PlayersRTT[i].adwPlayerRTTCounts[min (7, dwCurLat >> 7)]++;
    m_PlayersRTT[i].adwPlayerRTTTotals[min (7, dwCurLat >> 7)] += dwCurLat;
#endif // GATHER_SYNC_STATS

    //
    // Check for zombies:
    //
    BOOL bZombie = m_JudgeViewOfTanks[i].Zombie;    // Store current state

    if (m_JudgeViewOfTanks[i].Zombie)
    {   // Check if zombie's condition is improving:
        if (dwCurLat < TANKS_APP->GetMaxRTT())                   // RTT is valid again
        {
            if (m_JudgeViewOfTanks[i].LastRTTWasGood)   // if the former one was too -
                m_JudgeViewOfTanks[i].BadRTTCount--;    // dec. counter, or else
            else                                        // reset counter as we begin a new series
                m_JudgeViewOfTanks[i].BadRTTCount = MAX_BAD_RTT_COUNT - 1;
        }
        // If there were MAX_BAD_RTT_COUNT good RTTs in a row - 
        // tank is back in business:
        if (m_JudgeViewOfTanks[i].BadRTTCount <= 0)
        {
            m_JudgeViewOfTanks[i].BadRTTCount = 0;
            bZombie = FALSE;
        }
    } else
    {   // Check if tank needs to become zombie:
        if (dwCurLat > TANKS_APP->GetMaxRTT())                       // RTT is too big
        {
            if (! m_JudgeViewOfTanks[i].LastRTTWasGood)     // if the former one was too
                m_JudgeViewOfTanks[i].BadRTTCount++;        // inc. counter, or else
            else                                            // reset counter as a new series starts
                m_JudgeViewOfTanks[i].BadRTTCount = 1;
        }
        // If there were MAX_BAD_RTT_COUNT bad RTTs in a row - 
        // tank is out of business:
        if (m_JudgeViewOfTanks[i].BadRTTCount >= MAX_BAD_RTT_COUNT)
        {
            bZombie = TRUE;
        }
    }
    // Update last RTT evaluation:
    m_JudgeViewOfTanks[i].LastRTTWasGood = (dwCurLat < TANKS_APP->GetMaxRTT());
    
    if (bZombie != m_JudgeViewOfTanks[i].Zombie)    // There is a status change:
    {
        m_JudgeViewOfTanks[i].Zombie = bZombie;

        if (bZombie)
        {
            INC_ZOMBIE_IN (i)
        }
        else
        {
            INC_ZOMBIE_OUT (i)
        }
        NET_SYNC_TRACE (("Tank %d is %s zombie: "
                         "current latency = %d (max = %d),"
                         " bad RTT count = %d\n",
                         i, 
                         bZombie ? "becoming" : "stopping being", 
                         dwCurLat, 
                         m_PlayersRTT[i].dwPlayerMaxRTT,
                         m_JudgeViewOfTanks[i].BadRTTCount));
        SendTankZombie (i, bZombie, TRUE /* Send to all */);
    }

}

/*------------------------------------------------------------------------------

  Function: ChkForZombies

  Purpose:  Another zombie check - this method is called by the game manager 
            every iteration, to check for tanks that have stopped reporting for
            a long period of time (MAX_COMM_MUTE_PERIOD)

  Input:    None.

  Output:   None.

  Remarks:  
------------------------------------------------------------------------------*/
void
CHost::ChkForZombies()
{
    DWORD dwNow = m_Timer.GetLocalTime();

    for (int i = 0; i < MAX_TANKS; i++)
    {
        if (CCommManager::INVALID_PLAYER_ID == m_aPlayersID[i])
            continue;
        // Check if time of last message from tank[i] arrived in reasonable time:
        if (((dwNow - m_PlayersRTT[i].dwLastMsgTime) > MAX_COMM_MUTE_PERIOD) &&
            !m_JudgeViewOfTanks[i].Zombie)
        {   // This player wasn't heard for too long time:
            m_JudgeViewOfTanks[i].Zombie = TRUE;
            INC_ZOMBIE_IN (i)

            NET_SYNC_TRACE (("Tank %d is becoming zombie"
                             " - last message arrived %d millisecs ago.\n",
                             i, dwNow - m_PlayersRTT[i].dwLastMsgTime));
                    
            SendTankZombie (i, TRUE, TRUE /* Send to all */);
        }
    }
}


BOOL 
CHost::GetPlayerInfo (UINT ind, DWORD &dwDuration, DWORD &dwRTT)
{
    ASSERT (ind < MAX_TANKS);
    if (CCommManager::INVALID_PLAYER_ID == m_aPlayersID[ind])
        return FALSE;   // Player not playing now
    dwDuration = GetTickCount() - m_PlayersRTT[ind].dwPlayerJoinTime;
    dwRTT = m_PlayersRTT[ind].dwPlayerCurRTT;
    return TRUE;
}

DPID
CHost::GetDirectPlayID (UINT uPlayerID)
{
    ASSERT (uPlayerID < MAX_TANKS);
    return m_aPlayersID[uPlayerID];
}


#ifdef GATHER_SYNC_STATS

void 
CHost::ZeroTankSyncStats (UINT uTankID)
{
    TRACE ("\tTank #%d is joining session, starting sync statistics for it.\n", uTankID);
    memset (&m_TanksSyncStats[uTankID], 0, sizeof (TankSyncStats));
    m_TanksSyncStats[uTankID].dwSmallestTimeGap = MAX_DWORD;
    m_TanksSyncStats[uTankID].dwStartTime = GetTickCount();
}

void 
CHost::PrintTankSyncStats (UINT uTankID)
{
    TRACE ( "\n\n\t\t\t**** Game sync statistics for tank %d ****\n",
                uTankID);
    TRACE ( "Total time player %d was active = %.3f secs\n",
                uTankID,
                double(GetTickCount() - m_TanksSyncStats[uTankID].dwStartTime) / 1000.0);
    TRACE ( "Sync messages arriving from tank %d = %d\n",
                uTankID, m_TanksSyncStats[uTankID].dwSyncMsgsArrived);
    TRACE ("Average size of sync message = %d\n",
                m_TanksSyncStats[uTankID].dwSyncMsgsArrived ?
                (uTankID, m_TanksSyncStats[uTankID].dwTotSyncMsgSize / 
                 m_TanksSyncStats[uTankID].dwSyncMsgsArrived) : 0);
    TRACE ( "Sync mismatches:\n");
    TRACE ( "\tBonus = %d, Mines = %d, Zombie = %d\n",
                m_TanksSyncStats[uTankID].dwBonusMismatch,
                m_TanksSyncStats[uTankID].dwMinesMismatch,
                m_TanksSyncStats[uTankID].dwZombieMismatch);
    TRACE ( "\tTanks status = %d\n",
                m_TanksSyncStats[uTankID].dwOtherTankStatusMismatch);
    TRACE ( "\tMissing tanks = %d, Extra tanks = %d\n",
                m_TanksSyncStats[uTankID].dwMissingTanks,
                m_TanksSyncStats[uTankID].dwExtraTanks);
    TRACE ( "Zombie:\n");
    TRACE ( "\tTank became zombie %d times, and came back alive %d times.\n",
                m_TanksSyncStats[uTankID].dwZombieIn,
                m_TanksSyncStats[uTankID].dwZombieOut);
    TRACE ( "Sync msgs timing:\n");
    TRACE ( "\tTime gap between reports: min = %d millisecs, max = %d, avg = %d\n",
                m_TanksSyncStats[uTankID].dwSmallestTimeGap,
                m_TanksSyncStats[uTankID].dwBiggestTimeGap,
                m_TanksSyncStats[uTankID].dwTotalGapsCnt ?
                    DWORD(double(m_TanksSyncStats[uTankID].dwTotalGaps) / 
                          double(m_TanksSyncStats[uTankID].dwTotalGapsCnt)) : 0);

    DWORD dwAvg = m_PlayersRTT[uTankID].dwPlayerTotalRTTs ? 
        DWORD(double(m_PlayersRTT[uTankID].dwPlayerTotalRTTs) /
              double(m_PlayersRTT[uTankID].dwTotMsgs)) : 0;

    TRACE ( "Round Trip Times:\n");
    TRACE ( "\tMax = %d msecs, Min = %d msecs , Avg = %d msecs\n",
                m_PlayersRTT[uTankID].dwPlayerMaxRTT,
                m_PlayersRTT[uTankID].dwPlayerMinRTT,
                dwAvg);
    TRACE ( "\tRTT histogram:\n" );
    for (int i = 0; i < sizeof(m_PlayersRTT[uTankID].adwPlayerRTTCounts) /
        sizeof (m_PlayersRTT[uTankID].adwPlayerRTTCounts[0]) - 1; i++)
    {
        TRACE ( "\tRTT between %d and %d was counted %d times (Avg = %d)\n",
            i << 7, 
            ((i + 1) << 7) - 1, 
            m_PlayersRTT[uTankID].adwPlayerRTTCounts[i],
            m_PlayersRTT[uTankID].adwPlayerRTTCounts[i] ?
            m_PlayersRTT[uTankID].adwPlayerRTTTotals[i] /
            m_PlayersRTT[uTankID].adwPlayerRTTCounts[i] : 0);
    }
    TRACE ( "\tRTT above %d was counted %d times (Avg = %d)\n\n",
            i << 7,
            m_PlayersRTT[uTankID].adwPlayerRTTCounts[i],
            m_PlayersRTT[uTankID].adwPlayerRTTCounts[i] ?
            m_PlayersRTT[uTankID].adwPlayerRTTTotals[i] /
            m_PlayersRTT[uTankID].adwPlayerRTTCounts[i] : 0);

    UNREFERENCED_PARAMETER(uTankID);
}

void 
CHost::UpdateSyncMsgTime (UINT uTankID)
{
    if (!m_TanksSyncStats[uTankID].dwLastReportTime)
    {   // First time around
        m_TanksSyncStats[uTankID].dwLastReportTime = GetTickCount();
        m_TanksSyncStats[uTankID].dwTotalGapsCnt = 0;
        m_TanksSyncStats[uTankID].dwTotalGaps = 0;
        return;
    }
    DWORD dwNow = GetTickCount();
    DWORD dwTimeGap = dwNow - m_TanksSyncStats[uTankID].dwLastReportTime;
    m_TanksSyncStats[uTankID].dwLastReportTime = dwNow;
    m_TanksSyncStats[uTankID].dwSmallestTimeGap = min (dwTimeGap,
        m_TanksSyncStats[uTankID].dwSmallestTimeGap);
    m_TanksSyncStats[uTankID].dwBiggestTimeGap = max (dwTimeGap,
        m_TanksSyncStats[uTankID].dwBiggestTimeGap);
    m_TanksSyncStats[uTankID].dwTotalGaps += dwTimeGap;
    m_TanksSyncStats[uTankID].dwTotalGapsCnt++;
}


#endif // defined GATHER_SYNC_STATS

