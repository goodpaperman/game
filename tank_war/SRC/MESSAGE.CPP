/*****************************************************************************
*                                                                             
*   Message.cpp                                                            
*                                                                             
*   Electrical Engineering Faculty - Software Lab                             
*   Spring semester 1998                                                      
*                                                                             
*   Tanks game                                                                
*                                                                             
*   Module description: Messages are send between the game threads (in process)
*                       and over the network between the host and players.
*                       We use pool allocation techniques to make message 
*                       allocation fast and bit fields to compact the size of 
*                       messages when send over the net.
*                                                                             
*   Authors: Eran Yariv - 28484475                                           
*            Moshe Zur  - 24070856                                           
*                                                                            
*                                                                            
*   Date: 23/09/98                                                           
*                                                                            
******************************************************************************/
#include "stdafx.h"
#include "Message.h"
#include "Tanks.h"

#pragma pack (push)
#pragma pack (1)

CPool<CMessage> CMessage::m_Pool;       // The message pool. We overload the new and
                                        // delete methods to use the pools fast allocation
                                        // and deallocation methods.

#define VAR_LEN                         0xFF     // Variable message length
#define TNK_CHK_SUM_TO_TANK_ID(pb)      (BYTE(( ((PBYTE(pb))[2]) >> 2) & 0x03))
#define MSG_HEADER_SIZE                 5   // Msg type (1 Byte) + Msg time (DWORD)

static BYTE MsgSizeArr[] =              // Messages size look up table:
{
    MSG_HEADER_SIZE + sizeof (BonusParamTag),           // ADD_BONUS 
                                                        // (See BonusParamTag for description)
    MSG_HEADER_SIZE + sizeof (TankParamTag),            // ADD_TANK  
                                                        // (See TankParamTag for description)
    MSG_HEADER_SIZE + sizeof (BoardParamTag),           // ADD_BOARD 
                                                        // (See BoardParamTag for description)
    MSG_HEADER_SIZE + sizeof (BomberParamTag),          // ADD_BOMBER 
                                                        // (See BomberParamTag for description)
    MSG_HEADER_SIZE + sizeof (TankRemoveParamTag),      // REMOVE_TANK 
                                                        // (See TankRemoveParamTag for description)
    MSG_HEADER_SIZE + sizeof (ManouverSetParamTag),     // MANOUVER_SET 
                                                        // (See ManouverSetParamTag for description)
    MSG_HEADER_SIZE + sizeof (TankStatusParamTag),      // SET_TANK_STATUS 
                                                        // (See TankStatusParamTag for description)
    MSG_HEADER_SIZE + sizeof (TankPosParamTag),         // SET_TANK_POS
                                                        // (See TankPosParamTag for description)
    MSG_HEADER_SIZE + sizeof (TankZombieParamTag),      // SET_TANK_ZOMBIE
                                                        // (See TankZombieParamTag for description)
    MSG_HEADER_SIZE + sizeof (TankRequestParamTag),     // REQUEST_TANK
                                                        // (See TankRequestParamTag for description)
    VAR_LEN,                                            // CHECK_SUM
                                                        // (See SingleTankChkSumType for description)
    VAR_LEN                                             // CHAT
};

BYTE         * CMessage::m_MsgSizeArr = MsgSizeArr;
TIMER_CLASS  * CMessage::m_pTimer = NULL;


/*------------------------------------------------------------------------------

  Function: Constructor.

  Purpose:  Constructor for messages created internally (not from network)

  Input:    type: Type of message to be constructed.
            data: Data of message to be constructed.
            typ:  Flag to indicate if message contains our local game time
                  (internal message between threads) or the host game time
                  (external message sent over the net).

------------------------------------------------------------------------------*/
CMessage::CMessage (MessageType type, 
                    union MessageData& data, 
                    GameMessageType typ) : 
    m_Type (type), 
    m_UnionData (data)
{   
    if (NULL == m_pTimer)
        m_pTimer = &(TANKS_APP->m_gTimer);

    memcpy (&m_UnionData, &data, sizeof(union MessageData));
    if (PLAYER_MSG == typ)
    {   // Message created internally (not from network) and is directed out.
        // This only happens for clients.
        // Convert to server's time (game time)
        m_dwMsgTime = m_pTimer->GetRemoteTime ();
    }
    else 
        // Just store local time as message time:
        m_dwMsgTime = m_pTimer->GetLocalTime();
}

/*------------------------------------------------------------------------------

  Function: Constructor.

  Purpose:  Constructor for messages arriving from network (not created internally)

  Input:    pData: Pointer to data buffer.
            bDataSize:  Size of data buffer.
            typ: Flag indicating if message time is global (host) or local 
                 (any player).

------------------------------------------------------------------------------*/
CMessage::CMessage (PBYTE pData, BYTE bDataSize, GameMessageType typ)
{
    if (NULL == m_pTimer)
        m_pTimer = &(TANKS_APP->m_gTimer);

        // Get message ID
    BYTE bType = BYTE((pData[0]) & 0x000F);
        // Make sure the message has a valid type
    ASSERT (bType < sizeof (MsgSizeArr) / sizeof(MsgSizeArr[0]));
        // Make sure the data fits the size according to its type
    ASSERT ((VAR_LEN == m_MsgSizeArr[bType]) ||     // Variable length or (ignored)
            (bDataSize == m_MsgSizeArr[bType]));    // exact length
        // Message type ok - store it.
    m_Type = MessageType(bType);

    pData++;
        // Read time from message
    PDWORD pTime = PDWORD(pData);
    if (PLAYER_MSG == typ)
    { // Requested by player (coming from server), convert to local time
        m_dwMsgTime = m_pTimer->GetLocalTime (*pTime);
    }
    else
    { // Requested by server (coming from a client to server), already in server's time
        m_dwMsgTime = *pTime;
    }

    pData += sizeof(DWORD);
        // Handle decoding of incoming message according to type
    switch (m_Type)
    {   
        case ADD_BONUS:                 // Server => Client
            DecodeAddBonus (pData);     
            break;
        case ADD_TANK:                  // Server => Client
            DecodeAddTank (pData);
            break;
        case ADD_BOARD:                 // Server => Client
            DecodeAddBoard (pData);
            break;
        case ADD_BOMBER:                // Server => Client
            DecodeAddBomber (pData);
            break;
        case REMOVE_TANK:               // Server => Client
            DecodeRemoveTank (pData);
            break;
        case MANOUVER_SET:              // Server <=> Client
            DecodeManouverSet (pData);
            break;
        case REQUEST_TANK:              // Client => Server
            DecodeRequestTank (pData);
            break;
        case CHECK_SUM:                 // Client => Server;
            DecodeCheckSum (pData);
            break;
        case SET_TANK_STATUS:            // Server => Client
            DecodeSetTankStatus (pData);
            break;
        case SET_TANK_POS:              // Server => Client
            DecodeSetTankPos (pData);
            break;
        case SET_TANK_ZOMBIE:              // Server => Client
            DecodeSetTankZombie (pData);
            break;
        case CHAT:
            DecodeChat(pData);
            break;
        default:
            ASSERT (FALSE);
            break;
    }
    UNREFERENCED_PARAMETER(bDataSize);
}


/*------------------------------------------------------------------------------

  Function: GetBuffer

  Purpose:  Spools the message contents to buffer, minimizing buffer size by 
            using bit fields to store the message contents.

  Input:    pRes: Pointer to buffer.

  Output:   Return the buffer length.

  Remarks:  Each type of message has an encoding method to convert data to
            compact structure w/o loosing information.

------------------------------------------------------------------------------*/
DWORD        
CMessage::GetBuffer (PBYTE pRes)
{
        // Store the type
    pRes[0] = BYTE(m_Type);
    pRes++;
        // Store the remote time
    *((PDWORD)pRes) = m_dwMsgTime;
    pRes += sizeof (DWORD);
        // Handle encoding of message according to type
    switch (m_Type)
    {   
        case REQUEST_TANK:          // Client => Server
            return EncodeRequestTank (pRes);
        case ADD_BONUS:             // Server => Client
            return EncodeAddBonus (pRes);
        case ADD_TANK:              // Server => Client
            return EncodeAddTank (pRes);
        case ADD_BOARD:             // Server => Client
            return EncodeAddBoard (pRes);
        case ADD_BOMBER:            // Server => Client
            return EncodeAddBomber (pRes);
        case REMOVE_TANK:           // Server => Client
            return EncodeRemoveTank (pRes);
        case MANOUVER_SET:          // Server <=> Client
            return EncodeManouverSet (pRes);
        case CHECK_SUM:             // Client => Server
            return EncodeCheckSum (pRes);
        case SET_TANK_STATUS:        // Server => Client
            return EncodeSetTankStatus (pRes);
        case SET_TANK_POS:          // Server => Client
            return EncodeSetTankPos (pRes);
        case SET_TANK_ZOMBIE:       // Server => Client
            return EncodeSetTankZombie (pRes);
        case CHAT:
            return EncodeChat(pRes);
        default:
            ASSERT (FALSE);
            return 0;
    }
}


// Encoding & decoding functions:
// ------------------------------

DWORD            
CMessage::EncodeAddBomber (PBYTE pData) 
{
    // Validate data to encode
    ASSERT (m_UnionData.BomberParam.Direction < 24);
    *((BomberParamTag *)pData) = m_UnionData.BomberParam;
    return m_MsgSizeArr[ADD_BOMBER];
}

void            
CMessage::DecodeAddBomber (PBYTE pData)
{
    m_UnionData.BomberParam = *((BomberParamTag *)pData);
    ASSERT (m_UnionData.BomberParam.Direction < 24);
}

DWORD            
CMessage::EncodeRequestTank (PBYTE pData) const
{
    // Validate data to encode
    ASSERT (m_UnionData.TankRequestParam.bID < 4);
    *pData = m_UnionData.TankRequestParam.bID;
    return m_MsgSizeArr[REQUEST_TANK];
}

void            
CMessage::DecodeRequestTank (PBYTE pData)
{
    m_UnionData.TankRequestParam.bID = *pData;
    ASSERT (m_UnionData.TankRequestParam.bID < 4);
}


DWORD            
CMessage::EncodeManouverSet (PBYTE pData) const
{
    // Validate data to encode
    ASSERT (m_UnionData.ManouverSetParam.Direction < 24);
    *((ManouverSetParamTag *)pData) = m_UnionData.ManouverSetParam;
    return m_MsgSizeArr[MANOUVER_SET];
}

void            
CMessage::DecodeManouverSet (PBYTE pData)
{
    m_UnionData.ManouverSetParam = *((ManouverSetParamTag *)pData);
    ASSERT (m_UnionData.ManouverSetParam.Direction < 24);
}

DWORD            
CMessage::EncodeAddBonus (PBYTE pData) const
{
    ASSERT (m_UnionData.BonusParam.XPos < 512);
    ASSERT (m_UnionData.BonusParam.YPos < 512);
    ASSERT (BonusType(m_UnionData.BonusParam.Type) < NUM_BONUSES);

    *((BonusParamTag *)pData) = m_UnionData.BonusParam;
    return m_MsgSizeArr[ADD_BONUS];
}

void            
CMessage::DecodeAddBonus (PBYTE pData)
{
    m_UnionData.BonusParam = *((BonusParamTag *)pData);
    ASSERT (BonusType(m_UnionData.BonusParam.Type) < NUM_BONUSES);
}


DWORD            
CMessage::EncodeAddTank (PBYTE pData) const
{
    ASSERT (m_UnionData.TankParam.ShieldLevel < 101);
    ASSERT (m_UnionData.TankParam.Direction < 24);

    *((TankParamTag*)pData) = m_UnionData.TankParam;
    return m_MsgSizeArr[ADD_TANK];
}

void            
CMessage::DecodeAddTank (PBYTE pData)
{
    m_UnionData.TankParam = *((TankParamTag*)pData);
    ASSERT (m_UnionData.TankParam.ShieldLevel < 101);
    ASSERT (m_UnionData.TankParam.Direction < 24);
}

DWORD            
CMessage::EncodeAddBoard (PBYTE pData) const
{
    ASSERT (m_UnionData.BoardParam.Seed < 0xFFFF);
    ASSERT (m_UnionData.BoardParam.Complexity < 0x001F);

    *((BoardParamTag *)pData) = m_UnionData.BoardParam;
    return m_MsgSizeArr[ADD_BOARD];
}

void            
CMessage::DecodeAddBoard (PBYTE pData)
{
    m_UnionData.BoardParam = *((BoardParamTag *)pData);
}

DWORD            
CMessage::EncodeRemoveTank (PBYTE pData) const
{
    *((TankRemoveParamTag *)pData) = m_UnionData.TankRemoveParam;
    return m_MsgSizeArr[REMOVE_TANK];
}

void            
CMessage::DecodeRemoveTank (PBYTE pData)
{
    m_UnionData.TankRemoveParam = *((TankRemoveParamTag *)pData);
}


DWORD           
CMessage::EncodeSetTankStatus (PBYTE pBuffer) const
{
    *(TankStatusParamTag*)pBuffer = m_UnionData.TankStatusParam;
    return m_MsgSizeArr[SET_TANK_STATUS];
}

void
CMessage::DecodeSetTankStatus (PBYTE pBuffer)
{
    m_UnionData.TankStatusParam = *((TankStatusParamTag *)pBuffer);
}

DWORD
CMessage::EncodeSetTankPos (PBYTE pBuffer) const
{
    *(TankPosParamTag*)pBuffer = m_UnionData.TankPosParam;
    return m_MsgSizeArr[SET_TANK_POS];
}

void
CMessage::DecodeSetTankPos (PBYTE pBuffer)
{
    m_UnionData.TankPosParam = *(TankPosParamTag*)pBuffer;
}

DWORD
CMessage::EncodeSetTankZombie (PBYTE pBuffer) const
{
    *(TankZombieParamTag*)pBuffer = m_UnionData.TankZombieParam;
    return m_MsgSizeArr[SET_TANK_ZOMBIE];
}

void
CMessage::DecodeSetTankZombie (PBYTE pBuffer)
{
    m_UnionData.TankZombieParam = *(TankZombieParamTag*)pBuffer;
}

DWORD            
CMessage::EncodeCheckSum (PBYTE pData) const
{
    ASSERT (m_UnionData.CheckSumParam.NumberOfTanks <= 4);
    ASSERT ((m_UnionData.CheckSumParam.ActiveBonusType >= BONUS_NONE) &&
            (m_UnionData.CheckSumParam.ActiveBonusType < NUM_BONUSES));

    PBYTE pb = pData;
    // First, do fixed part:
    FixedSizeChkSumType *p = (FixedSizeChkSumType *)pb;
    p->Header.NumberOfTanks     = m_UnionData.CheckSumParam.NumberOfTanks;
    p->Header.DeadHostReport    = BYTE(m_UnionData.CheckSumParam.DeadHostReport ? 1 : 0);
    p->Header.ActiveBonusType   = BYTE(m_UnionData.CheckSumParam.ActiveBonusType);
    p->Header.XPos              = m_UnionData.CheckSumParam.XPos;
    p->Header.YPos              = m_UnionData.CheckSumParam.YPos;
        // Encode sector mines checksums:
    int sect = 0;
    for (int i=0; i<8; i++)
    {
        p->MinesSectorsChkSum[i].LoNibble = BYTE((m_UnionData.CheckSumParam.MinesSectorsChkSum[sect++]) & 0x0F);
        p->MinesSectorsChkSum[i].HiNibble = BYTE((m_UnionData.CheckSumParam.MinesSectorsChkSum[sect++]) & 0x0F);
    }

    // Encode tanks - variable part:

    pb += sizeof (FixedSizeChkSumType);
    for (i=0; i<4; i++)
        if (m_UnionData.CheckSumParam.TanksChkSums[i].TankExists)
        {   // i'th tank exists
            // Tank's encoding includes tank's ID
            SingleTankChkSumType *p = (SingleTankChkSumType *)pb;

            p->TankID   = i;
            p->Zombie   = m_UnionData.CheckSumParam.TanksChkSums[i].Zombie;
            p->Shells   = m_UnionData.CheckSumParam.TanksChkSums[i].Shells;
            p->Bullets  = m_UnionData.CheckSumParam.TanksChkSums[i].Bullets;
            p->Mines    = m_UnionData.CheckSumParam.TanksChkSums[i].Mines;
            p->FastFire = m_UnionData.CheckSumParam.TanksChkSums[i].FastFire;
            pb += sizeof (SingleTankChkSumType);
        }

    // Return true (varaible) message size    
    DWORD dwMsgSize = sizeof (FixedSizeChkSumType) + 
                      m_UnionData.CheckSumParam.NumberOfTanks * sizeof (SingleTankChkSumType);
    ASSERT (pb == pData+dwMsgSize);
    return dwMsgSize + 5;   // Including message header
}

void            
CMessage::DecodeCheckSum (PBYTE pData)
{
    FixedSizeChkSumType *p = (FixedSizeChkSumType *)pData;
    m_UnionData.CheckSumParam.NumberOfTanks     = p->Header.NumberOfTanks;
    m_UnionData.CheckSumParam.DeadHostReport    = p->Header.DeadHostReport;
    m_UnionData.CheckSumParam.ActiveBonusType   = BonusType(p->Header.ActiveBonusType);
    m_UnionData.CheckSumParam.XPos = p->Header.XPos;
    m_UnionData.CheckSumParam.YPos = p->Header.YPos;
        // Decode sector mines checksums:
    int sect = 0;
    for (int i=0; i<8; i++)
    {
        m_UnionData.CheckSumParam.MinesSectorsChkSum[sect++] = p->MinesSectorsChkSum[i].LoNibble;
        m_UnionData.CheckSumParam.MinesSectorsChkSum[sect++] = p->MinesSectorsChkSum[i].HiNibble;
    }

    ASSERT (m_UnionData.CheckSumParam.NumberOfTanks <= 4);
    ASSERT ((m_UnionData.CheckSumParam.ActiveBonusType >= BONUS_NONE) &&
            (m_UnionData.CheckSumParam.ActiveBonusType < NUM_BONUSES));

    // Decode tanks - variable part:
    // Clear all tanks data
    memset (m_UnionData.CheckSumParam.TanksChkSums,
            0,
            sizeof (CheckSumParamTag::TankCheckSumParamTag) * MAX_TANKS);

    pData += sizeof (FixedSizeChkSumType);
    for (i=0; i<m_UnionData.CheckSumParam.NumberOfTanks; i++)
    {
        SingleTankChkSumType *p = (SingleTankChkSumType *)pData;
        BYTE bTankID = BYTE(p->TankID);  // Get tank ID
        ASSERT (bTankID < 4);
        m_UnionData.CheckSumParam.TanksChkSums[bTankID].TankExists  = TRUE;
        m_UnionData.CheckSumParam.TanksChkSums[bTankID].Zombie      = p->Zombie;
        m_UnionData.CheckSumParam.TanksChkSums[bTankID].Shells      = WORD(p->Shells);
        m_UnionData.CheckSumParam.TanksChkSums[bTankID].Bullets     = WORD(p->Bullets);
        m_UnionData.CheckSumParam.TanksChkSums[bTankID].Mines       = WORD(p->Mines);
        m_UnionData.CheckSumParam.TanksChkSums[bTankID].FastFire    = p->FastFire;
        pData += sizeof (SingleTankChkSumType);
    }
}

DWORD
CMessage::EncodeChat (PBYTE pData) const
{
    *pData++ = m_UnionData.ChatParam.TankID;
    PDWORD pdw = (PDWORD)pData;
    *pdw++ = m_UnionData.ChatParam.idFrom;
    pData = (PBYTE)pdw;
    strcpy ((char *)pData, m_UnionData.ChatParam.Msg);
    return DWORD(sizeof (m_UnionData.ChatParam) -           // Entire message
                 sizeof (m_UnionData.ChatParam.Msg) +       // Leave const size
                 strlen(m_UnionData.ChatParam.Msg) + 1 +    // Variable string 
                 MSG_HEADER_SIZE);                          // Msg header
}
 
void
CMessage::DecodeChat (PBYTE pData)
{
    m_UnionData.ChatParam.TankID = *pData++;
    m_UnionData.ChatParam.idFrom = *((PDWORD)(pData));
    pData+=sizeof(DWORD);

    // Assert msg length :
    int len = strlen ((char *)pData);

    ASSERT (len <= MAX_CHAT_MSG_LEN);
    if (len > MAX_CHAT_MSG_LEN) 
    {   // Chop string at last available char:
        pData[MAX_CHAT_MSG_LEN] = '\0';
        len = MAX_CHAT_MSG_LEN;
    }
    strcpy (m_UnionData.ChatParam.Msg, (char *)pData);
}

/* Special case - static mines per sector encoding / decoding functions : */

DWORD 
CMessage::EncodeSectorMines (PBYTE pData, int iSector, DWORD dwMinesFound, DWORD *pAllMinesInSector)
{
    pData[0] = BYTE(iSector);
    pData[1] = BYTE(dwMinesFound);
    DWORD dwDataSize = dwMinesFound * sizeof (DWORD);
    memcpy (&pData[2], pAllMinesInSector, dwDataSize);
    return dwDataSize + 2;
}

DWORD 
CMessage::DecodeSectorMines (PBYTE pData, DWORD dwBufferSize, int &iSector, DWORD *pAllMinesInSector)
{
    ASSERT (dwBufferSize >= 2);
    iSector = int(pData[0]);

    DWORD dwMinesFound = DWORD(pData[1]);
    DWORD dwDataSize = dwMinesFound * sizeof (DWORD);
    ASSERT (dwBufferSize == dwDataSize + 2);
    memcpy (pAllMinesInSector, &pData[2], dwDataSize);
    return dwMinesFound;

    UNREFERENCED_PARAMETER(dwBufferSize);
}

/* End of special case - static mines per sector encoding / decoding functions */


#ifdef NET_MON_GAME
char *
CMessage::GetName()
{   // Get message name
    return CMessage::GetName (BYTE(m_Type));
}
char *
CMessage::GetName(BYTE bID)
{   // Get message name (static)
    switch (bID)
    {
        case ADD_BONUS:
            return "ADD_BONUS";
        case ADD_TANK:
            return "ADD_TANK";
        case ADD_BOARD:
            return "ADD_BOARD";
        case ADD_BOMBER:
            return "ADD_BOMBER";
        case REMOVE_TANK:
            return "REMOVE_TANK";
        case MANOUVER_SET:
            return "MANOUVER_SET";
        case SET_TANK_STATUS:
            return "SET_TANK_STATUS";
        case SET_TANK_POS:
            return "SET_TANK_POS";
        case REQUEST_TANK:
            return "REQUEST_TANK";
        case CHECK_SUM:
            return "CHECK_SUM";
        case ADD_OBJECT:
            return "ADD_OBJECT";
        case ADD_SHELL:
            return "ADD_SHELL";
        case ADD_BULLET:
            return "ADD_BULLET";
        case ADD_GAME_OVER:
            return "ADD_GAME_OVER";
        case 0xff:
            return "PING";
        default:
            return "unknown";
    }
}

#endif  NET_MON_GAME



#pragma pack (pop)


