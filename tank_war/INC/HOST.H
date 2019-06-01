/*****************************************************************************
*                                                                             
*   Host.h
*                                                                             
*   Electrical Engineering Faculty - Software Lab                             
*   Spring semester 1998                                                      
*                                                                             
*   Tanks game                                                                
*                                                                             
*   Module description: Implements the communication module that handles 
*                       messages send to the game host.
*                       
*                                                                             
*   Authors: Eran Yariv - 28484475                                           
*            Moshe Zur  - 24070856                                           
*                                                                            
*                                                                            
*   Date: 23/09/98                                                           
*                                                                            
******************************************************************************/
#ifndef _HOST_H_
#define _HOST_H_

#include "CommManager.h"

class CHost
{
public:
    CHost(CCommManager &CommManager);
    ~CHost();

    void HandleMessage(DPID idFrom, BOOL bFromJudge);

    void RemovePlayerFromArray(DPID idPlayer);

    int  LookupTankID (DPID);   // Returns 0..MAX_TANKS-1 or -1 (no found) or -2 (host)

    BOOL GetPlayerInfo (UINT ind, DWORD &Duration, DWORD &RTT);
    DPID GetDirectPlayID (UINT uPlayerID);

    void ChkForZombies();       // Check if there are tanks that we didn't hear from
                                // for a long time

private:
    void HandleRequestTank (BYTE bReqTankID);
    void HandleCheckSum (CMessage &, BOOL bJudgeReporting);

    void SendBonusStatus();
    void SendAddTank(int iTankID);
    void SendTankPos(int iTankID, int nXPos, int nYPos);
    void SendTankStatus(int iTankID);
    void SendRemoveTank(int iTankID);
    void SendTankZombie(int iTankID, BOOL bZombie, BOOL bToAll);
    void SendMinesStatus(int iSector);

    void LocateNewTankPos(int &XPos, int &YPos, int &Dir);
    BYTE MapAndGetAvailTankID(BYTE bReqTankID);
    BOOL ValueCloseEnough (int val1, int val2, int MinTolerance);

    void SendToAllButSender();          // Send the message stored in m_Message to all tanks but the sender
    void SendToAll ();                  // Send the message stored in m_Message to all tanks.
    void ReplyToSender();               // Send the message stored in m_Message to the sender

    typedef struct JudgeViewOfTankTag
    {
        BOOL    Exists;         // Does tank exist?
        BOOL    Zombie;         // Zombie flag
        BOOL    FastFire;
        BOOL    LastRTTWasGood;
        DWORD   Shells;
        DWORD   Bullets;
        DWORD   Mines;
        int     BadRTTCount;
    } JudgeViewOfTank;

    JudgeViewOfTank m_JudgeViewOfTanks[MAX_TANKS];          // Judge's view on tanks states
    BYTE            m_JudgeViewOfMines[MAX_SECTOR + 1];     // Judge's view of mines
    BonusType       m_JudgeViewOfBonus;                     // Judge's view of current bonus

    CCommManager&   m_CommManager;                          // Alias to the comm. manager
    CGameManager&   m_GameManager;                          // Alias to the game  manager
    CCommMessage&   m_Message;                              // Alias to the comm. manager's message buffer
    TIMER_CLASS    &m_Timer;                                // Alias to global timer

    DPID            m_aPlayersID[MAX_TANKS];                // Map TankID to player DPID
    DPID            m_idFrom;                               // Sender of currently handled message 

    WORD            m_wSeed;                                // Game random seed
    BYTE            m_bComplexity;                          // Stored game complexity

    typedef struct _PlayerRTT
    {
#ifdef GATHER_SYNC_STATS
        DWORD dwPlayerMaxRTT;           // Maximal time RTT witnessed with this player
        DWORD dwPlayerMinRTT;           // Minimal time RTT witnessed with this player
        DWORD dwPlayerTotalRTTs;        // Sum of all RTTS (for average statistics)
        DWORD adwPlayerRTTCounts[8];    // For histogram of RTTs
        DWORD adwPlayerRTTTotals[8];    // For histogram of RTTs
        DWORD dwTotMsgs;                // Total number of messages RTTs are calculated for
#endif // GATHER_SYNC_STATS
        DWORD dwPlayerCurRTT;           // Last time RTT witnessed with this player
        DWORD dwPlayerJoinTime;         // The local host time of joining the game
        DWORD dwLastMsgTime;            // The local host time of last msg from player
    } PlayerRTT;

    PlayerRTT   m_PlayersRTT [MAX_TANKS];

    void PlayerJoinsGame (UINT uID);
    void UpdatePlayerRTT (DWORD dwGameTime);

#ifdef GATHER_SYNC_STATS
    typedef struct _TankSyncStats
    {
        DWORD dwStartTime;                  // Session start time for the player
        DWORD dwSyncMsgsArrived;            // Number of sync messages arriving from tank
        DWORD dwTotSyncMsgSize;             // Total size of all sync messages
        DWORD dwZombieIn;                   // Number of times tank turned zombie
        DWORD dwZombieOut;                  // Number of times tank turned un-zombie
        DWORD dwBonusMismatch;              // Number of bonus mismatches
        DWORD dwMinesMismatch;              // Mines checksum mismatch
        DWORD dwZombieMismatch;             // Zombie state mismatch
        DWORD dwOtherTankStatusMismatch;    // Mismatch in tank status (not reporting tank)
        DWORD dwMissingTanks;               // Number of tanks missing (yielding SendAddTank)
        DWORD dwExtraTanks;                 // Number of tanks still alive (yielding SendRemoveTank)

        DWORD dwSmallestTimeGap;            // Smallest time gap between two sync reports
        DWORD dwBiggestTimeGap;             // Biggest time gap between two sync reports
        DWORD dwTotalGaps, dwTotalGapsCnt;

        DWORD dwLastReportTime;             // Arrival time of last sync message

    } TankSyncStats;

    TankSyncStats   m_TanksSyncStats[MAX_TANKS];

    void ZeroTankSyncStats (UINT uTankID);
    void PrintTankSyncStats (UINT uTankID);
    void UpdateSyncMsgTime (UINT uTankID);

#endif // GATHER_SYNC_STATS
};

#include <host.inl>

#endif