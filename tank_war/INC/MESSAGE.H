/*****************************************************************************
*                                                                             
*   Message.h
*                                                                             
*   Electrical Engineering Faculty - Software Lab                             
*   Spring semester 1998                                                      
*                                                                             
*   Tanks game                                                                
*                                                                             
*   Module description: Implements the message class used in process and on 
*                       the network.
*                       
*                                                                             
*   Authors: Eran Yariv - 28484475                                           
*            Moshe Zur  - 24070856                                           
*                                                                            
*                                                                            
*   Date: 23/09/98                                                           
*                                                                            
******************************************************************************/
#ifndef _MESSAGE_H_
#define _MESSAGE_H_

#include "GameObject.h"
#include "Pool.h"
#include "Timer.h"

#pragma pack (push)
#pragma pack (1)

typedef enum
{
    SERVER_MSG,         // Server is requesting encoding / decoding of message
    PLAYER_MSG          // Server is requesting encoding / decoding of message    
} GameMessageType;        

// Every type of message with params should declare them bellow, and include them in the union:
struct BonusParamTag {
    DWORD         Type        : 3;    // Type
    DWORD         XPos        : 9;    // X pos
    DWORD         YPos        : 9;    // Y pos
    DWORD         LifeSpan    :11;    // TTL (seconds)
};

struct CheckSumParamTag {   // Encoded to variable length
    BYTE        NumberOfTanks;       // Current number of tanks
    BOOL        DeadHostReport;      // Host player is dead but host is alive
    BonusType   ActiveBonusType;     // Is there an active bonus? What's the type?
    WORD        XPos;                // Reporting tank's position
    WORD        YPos;
    struct TankCheckSumParamTag {    // A tank check sum for each tank
        BOOL    TankExists;          // Does tank exist?               
        BOOL    Zombie;              // Zombie flag
        BOOL    FastFire;            // Fast fire rate flag
        WORD    Bullets;
        WORD    Shells;
        WORD    Mines;
    }  TanksChkSums[MAX_TANKS];
    BYTE        MinesSectorsChkSum [MAX_SECTOR + 1];    // A mines sector check sum for each map sector
};

struct BulletParamTag { // Internal - never encoded
    WORD wXPos;
    WORD wYPos;
    BYTE bDirectionIndex;
    BYTE bParentTankID;
};

struct ShellParamTag { // Internal - never encoded
    WORD wXPos;
    WORD wYPos;
    BYTE bDirectionIndex;
    BYTE bParentTankID;
};

struct MineParamTag { // Internal - never encoded
    WORD wXPos;
    WORD wYPos;
};

struct TankParamTag {
    DWORD ID                      : 2;        // Tank ID
    int   XPos                    :10;        // X pos  (signed: -512..511)
    int   YPos                    :10;        // Y pos  (signed: -512..511)
    DWORD Local                   : 1;        // Is local?
    DWORD ShieldLevel             : 7;        // Shield level
    DWORD Bomber                  : 1;        // Bomber available ?
    DWORD FastFire                : 1;        // FastFireRate available - end of DWORD 1

    DWORD Shells                  : 9;        // Shells count
    DWORD Bullets                 : 9;        // Bullets count    
    DWORD Mines                   : 9;        // Mines count    
    DWORD Direction               : 5;        // Direction (0..23)     - end of DWORD 2

    BYTE FireRateBonusSecsLeft;               // Fast fire rate time left (in seconds)
};

struct TankStatusParamTag {
    BYTE bShieldLevel           : 7;
    BYTE bFastFire              : 1;	// End of byte 1
    BYTE bFireRateBonusSecsLeft : 8;	// End of byte 2
    BYTE bID                    : 2;
    BYTE bBomber                : 1;
    BYTE bZombie                : 1;
	BYTE bReserved1				: 4;	// End of byte 3
    DWORD dwShells              : 9;
    DWORD dwBullets             : 9;
    DWORD dwMines               : 9;    // End of byte 7
};

struct TankPosParamTag {
    DWORD ID            : 2;
    int   XPos          :10;    // X pos  (signed: -512..511)
    int   YPos          :10;    // Y pos  (signed: -512..511)
    DWORD Reserved      :10;    // End of DWORD
};

struct TankZombieParamTag {
    BYTE ID            : 2;
    BYTE Zombie        : 1;
    BYTE Reserved      : 5;
};

struct BoardParamTag {
    DWORD Seed        : 16;       // Random seed
    DWORD Complexity  :  5;       // Complexity level
    DWORD Reserved    :  9;       // Reserved for DWORD alignment
};

struct BomberParamTag {
    int   Xpos      :10;    // X pos (out of map enabled)  (signed: -512..511)
    int   Ypos      :10;    // Y pos (out of map enabled)  (signed: -512..511)
    DWORD Direction : 5;    // Direction (0..23)
    DWORD Reserved  : 7;    // Reserved (for DWORD padding)
};

struct TankRequestParamTag {
    BYTE    bID;
};

struct TankRemoveParamTag {
    BYTE    ID      : 2;    // Tank ID
    BYTE    Explode : 1;    // Explodes ?
};

struct ManouverSetParamTag {
    BYTE TankID       :  2;     // Tank ID
    BYTE Direction    :  5;     // Direction (0..23)
    BYTE Reserved     :  1;     // Reserved (for BYTE padding)
    BYTE ManouverSet;           // ManouverSet Data
};

struct ChatParamTag {
    BYTE  TankID;
    DWORD idFrom;
    char Msg[MAX_CHAT_MSG_LEN+1];
};

typedef struct {
    BYTE    NumberOfTanks   : 3;    // Current number of tanks ([0..MAX_TANKS])
    BYTE    ActiveBonusType : 3;    // Is there an active bonus? What's the type?
    BYTE    DeadHostReport  : 1;    // Host is dead but reports game status    
    BYTE    Reserved        : 1;
    WORD    XPos;                   // Reporting tanks pos
    WORD    YPos;
} CheckSumHeaderType;

typedef struct {
    BYTE    LoNibble       : 4;     // Low nibble
    BYTE    HiNibble       : 4;     // High nibble
} DoubleSectorChkSumType;

typedef struct {
    CheckSumHeaderType      Header;
    DoubleSectorChkSumType  MinesSectorsChkSum[8];
} FixedSizeChkSumType;

typedef struct {
    DWORD   TankID          :  2;
    DWORD   Zombie          :  1;   // Zombie flag
    DWORD   FastFire        :  1;
    DWORD   Bullets         :  9;
    DWORD   Shells          :  9;
    DWORD   Mines           :  9;
    DWORD   Reserved        :  1;
} SingleTankChkSumType;


class CMessage : public CObject
{
public:

    enum MessageType 
    {// Code            Description                         Direction (client's view)
     // --------------- ----------------------------------- ---------------------------
        ADD_BONUS,      // Add bonus object                 Incoming / Outgoing (*)
        ADD_TANK,       // Add tank (remote or local)       Incoming
        ADD_BOARD,      // Add board object                 Incoming
        ADD_BOMBER,     // Add bomber object                Incoming / Outgoing
        REMOVE_TANK,    // Remove tank (remote or local)    Incoming / Outgoing (*)
        MANOUVER_SET,   // Update manouver set              Incoming / Outgoing
        SET_TANK_STATUS,// Set existing tank state          Incoming
        SET_TANK_POS,   // Sets a tank position             Incoming
        SET_TANK_ZOMBIE,// Sets tank's zombie flag          Incoming
        REQUEST_TANK,   // Request a local tank             Outgoing
        CHECK_SUM,      // Notify host of current state     Outgoing
        CHAT,           // A string send to/from player     Incoming / Outgoing
        SET_MINES,      // Sets the mines in a sector       Incoming - special treatment 
                        // see remarks in 
                        // CHost::SendMinesStatus and
                        // in CClient::HandleMessage

        ADD_OBJECT,     // Add other objects                Internal
        ADD_SHELL,      // Add a shell                      Internal
        ADD_BULLET,     // Add a bullet                     Internal
        ADD_MINE,       // Add a mine                       Internal
        ADD_GAME_OVER,  // Add game-over animation          Internal

        NUM_MSGS        // Total number of messages         --------

        // (*) means the message is outgoing from the playaer on the host machine to the host player.
    };

    union MessageData 
    {
        CGameObject                 *pGameObj;
        struct BonusParamTag        BonusParam;
        struct ShellParamTag        ShellParam;
        struct BulletParamTag       BulletParam;
        struct MineParamTag         MineParam;
        struct TankParamTag         TankParam;
        struct BoardParamTag        BoardParam;
        struct BomberParamTag       BomberParam;
        struct TankRequestParamTag  TankRequestParam;
        struct ManouverSetParamTag  ManouverSetParam;
        struct TankRemoveParamTag   TankRemoveParam;
        struct CheckSumParamTag     CheckSumParam;
        struct TankStatusParamTag   TankStatusParam;
        struct TankPosParamTag      TankPosParam;
        struct TankZombieParamTag   TankZombieParam;
        struct ChatParamTag         ChatParam;
    } m_UnionData;

    CMessage    () {}
    CMessage    (MessageType type, union MessageData& data, GameMessageType);
    CMessage    (PBYTE, BYTE, GameMessageType);   // Create a message from a buffer and convert to local time

    virtual ~CMessage() {}

    void        SetTime (DWORD);                // Set message time (after translation to/from local)
    DWORD       GetTime () const;               // Get message time

    MessageType GetType () const;               // Get message type
    void        SetType ( MessageType );   

    DWORD       GetBuffer (PBYTE);              // Copy message to comm. buffer, return length

#ifdef NET_MON_GAME
    char       *GetName();                      // Get message name
    static char*GetName (BYTE);                 // Get message name of a given message ID 
#endif

    // used for the pool:

    void* operator new (size_t size)
    {
        return m_Pool.Alloc (size);
    }

    void operator delete (void *p, size_t size)
    {
        m_Pool.Free(p, size);
        return;
    }

#if _MFC_VER > 0x0421
    // Visual C++ version 6.0 and above (_MFC_VER >= 0x0600) complain if the 
    // other form of delete operator (not formerly supported in Visual C++ 5.0 and below)
    // does not exist.
    void operator delete (void *p)
    {
        m_Pool.Free(p, sizeof(CMessage));
        return;
    }
#endif

    static DWORD EncodeSectorMines (PBYTE buf, int iSector, DWORD dwMinesFound,  DWORD *pAllMinesInSector);
    static DWORD DecodeSectorMines (PBYTE buf, DWORD dwBufferSize, int &iSector, DWORD *pAllMinesInSector);

private:
    CMessage *pNext;        // for objects on free list
    static CPool<CMessage> m_Pool;
    friend CPool<CMessage>; // provide access for CPool to pNext private member

    DWORD           m_dwMsgTime;            // Time of message
    MessageType     m_Type;                 // Type of message

    // Encoding functions:
    DWORD           EncodeAddBomber (PBYTE);            // Client => Server && Server => Client
    DWORD           EncodeRequestTank (PBYTE) const;    // Client => Server
    DWORD           EncodeCheckSum (PBYTE) const;       // Client => Server
    DWORD           EncodeAddTank (PBYTE) const;        // Server => Client
    DWORD           EncodeManouverSet (PBYTE) const;    // Client => Server && Server => Client
    DWORD           EncodeAddBonus (PBYTE) const;       // Server => Client
    DWORD           EncodeAddBoard (PBYTE) const;       // Server => Client
    DWORD           EncodeRemoveTank (PBYTE) const;     // Server => Client
    DWORD           EncodeSetTankStatus (PBYTE) const;  // Server => Client
    DWORD           EncodeSetTankPos (PBYTE) const;     // Server => Client
    DWORD           EncodeSetTankZombie (PBYTE) const;  // Server => Client
    DWORD           EncodeChat (PBYTE) const;           // Server => Client && Client => Server


    // Decoding functions:
    void            DecodeAddBomber (PBYTE);            // Client => Server && Server => Client
    void            DecodeRequestTank (PBYTE);          // Client => Server
    void            DecodeCheckSum (PBYTE);             // Client => Server
    void            DecodeAddTank (PBYTE);              // Server => Client
    void            DecodeManouverSet (PBYTE);          // Client => Server && Server => Client
    void            DecodeAddBonus (PBYTE);             // Server => Client
    void            DecodeAddBoard (PBYTE);             // Server => Client
    void            DecodeRemoveTank (PBYTE);           // Server => Client
    void            DecodeSetTankStatus (PBYTE);        // Server => Client
    void            DecodeSetTankPos (PBYTE);           // Server => Client
    void            DecodeSetTankZombie (PBYTE);        // Server => Client
    void            DecodeChat (PBYTE);                 // Server => Client && Client => Server

    static BYTE         *m_MsgSizeArr;       // Static messages sizes table
    static TIMER_CLASS  *m_pTimer;           // Pointer to global timer
};

#include <Message.inl>

#pragma pack (pop)

#endif