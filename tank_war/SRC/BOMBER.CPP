/*****************************************************************************
*                                                                             
*   Bomber.cpp                                                            
*                                                                             
*   Electrical Engineering Faculty - Software Lab                             
*   Spring semester 1998                                                      
*                                                                             
*   Tanks game                                                                
*                                                                             
*   Module description: Implements the bomber object. That's a unique game 
*                       object - when in use it catches the keyboard input,
*                       enabling the player to position and direct it on board,
*                       and afterward when launched it acts like the other 
*                       moving game objects.
*                       
*                                                                             
*   Authors: Eran Yariv - 28484475                                           
*            Moshe Zur  - 24070856                                           
*                                                                            
*                                                                            
*   Date: 23/09/98                                                           
*                                                                            
******************************************************************************/
#include <stdafx.h>
#include <bomber.h>
#include <Bomb.h>

/*------------------------------------------------------------------------------

  Function: Constructor

  Purpose:  Contructs the bomber on a local machine (enabling player to control
            the start position and direction).

  Input:    pParent - pointer to the tank that owns (launches) the bomber.

  Output:   None.

  Remarks:  
------------------------------------------------------------------------------*/
CBomber::CBomber (CTankObj *pParent) :
    CMovingGameObject (MAP_WIDTH - BOMBER_WIDTH / 2,        // XPos
                       MAP_HEIGHT / 2 - BOMBER_HEIGHT / 2,  // YPos
                       BOMBER_WIDTH,                        // XSize
                       BOMBER_HEIGHT,                       // YSize
                       0,                                   // Direction index
                       BOMBER_SPEED),                       // Velocity 
    m_pFather (pParent),
    m_bLocal (TRUE),
    m_MsgQueue(TANKS_APP->m_gIncomingMsgQueue),
    m_Timer(TANKS_APP->m_gTimer),
    m_dwLastMoveTick (0),
    m_State (BOMBER_STATE_SETUP),
    m_uEdgePos (UINT(1.5 * float (EDGE_TICKS)))
{
    m_hImage = m_GlobalImageManager.GetImage (CImageManager::IMG_BOMBER);
    if (!m_GlobalObjsList.IsObjectValid(m_pFather))
    {
        // Father tank is dead now
        m_pFather = NULL;
        m_State = BOMBER_STATE_DEAD;
        m_pManouverSet = NULL;
    }
    else
    {   // Wa have a legal father tank
        m_pManouverSet = &m_pFather->GetManouverSet();
        m_pFather->RelinquishInput();
    }
}            

/*------------------------------------------------------------------------------

  Function: Constructor

  Purpose:  Contructs the bomber on a remote machine (a player on another 
            machine was launching the bomber, we are only displaying it).

  Input:    uDirInd - the bombers direction
            uXPos,uYPos - the bombers position
            dwInitFlightTime - the game time the bomber was launched

  Output:   None.

  Remarks:  
------------------------------------------------------------------------------*/
CBomber::CBomber (  UINT uDirInd,
                    UINT uXPos,
                    UINT uYPos,
                    DWORD dwInitFlightTime) :
    CMovingGameObject (uXPos,           // XPos
                       uYPos,           // YPos
                       BOMBER_WIDTH,    // XSize
                       BOMBER_HEIGHT,   // YSize
                       uDirInd,         // Direction index
                       BOMBER_SPEED),   // Velocity 
    m_pFather (NULL),
    m_bLocal (FALSE),
    m_MsgQueue(TANKS_APP->m_gIncomingMsgQueue),
    m_Timer(TANKS_APP->m_gTimer),
    m_dwLastMoveTick (dwInitFlightTime),
    m_State (BOMBER_STATE_FLYING),
    m_pManouverSet (NULL),
    m_dwInitialTime (dwInitFlightTime),
    m_InitialPos (CPoint (uXPos, uYPos))
{
    m_hImage = m_GlobalImageManager.GetImage (CImageManager::IMG_BOMBER);
    m_GlobalImageManager.RotateImage (m_hImage, m_uDirectionIndex);
}

CPoint 
CBomber::CalcEdgePos ()
{
    CPoint res;

    ASSERT (m_uEdgePos < (4 * EDGE_TICKS));

    if (((m_uEdgePos >= (3 * EDGE_TICKS)) && 
        (m_uEdgePos <= (4 * EDGE_TICKS - 1))) || 
        (0 == m_uEdgePos))
        // Left pane
        res.x = - BOMBER_WIDTH / 2;
    else if ((m_uEdgePos >= EDGE_TICKS) && (m_uEdgePos <= (2 * EDGE_TICKS)))
        // Right pane
        res.x = MAP_WIDTH - BOMBER_WIDTH / 2;
    else if ((m_uEdgePos >= 1) && (m_uEdgePos <= (EDGE_TICKS - 1)))
        // Top pane
        res.x = ((MAP_WIDTH / EDGE_TICKS) * m_uEdgePos) - BOMBER_WIDTH / 2;
    else if ((m_uEdgePos >= (2 * EDGE_TICKS + 1)) && 
        (m_uEdgePos <= (3 * EDGE_TICKS - 1)))
        // Bottom pane
        res.x = ((MAP_WIDTH / EDGE_TICKS) * ((3 * EDGE_TICKS) - m_uEdgePos)) - 
            BOMBER_WIDTH / 2;

    if ((m_uEdgePos >= (3 * EDGE_TICKS + 1)) && 
        (m_uEdgePos <= (4 * EDGE_TICKS - 1)))
        // Left pane
        res.y = ((MAP_HEIGHT / EDGE_TICKS) * 
            (m_uEdgePos - (3 * EDGE_TICKS))) - BOMBER_HEIGHT / 2;
    else if ((m_uEdgePos >= (EDGE_TICKS + 1)) && 
        (m_uEdgePos <= (2 * EDGE_TICKS - 1)))
        // Right pane
        res.y = ((MAP_HEIGHT / EDGE_TICKS) * ((2 * EDGE_TICKS) - m_uEdgePos)) -
            BOMBER_HEIGHT / 2;
    else if ((m_uEdgePos >= 0) && (m_uEdgePos <= EDGE_TICKS))
        // Top pane
        res.y = MAP_HEIGHT - BOMBER_HEIGHT / 2;
    else if ((m_uEdgePos >= (2 * EDGE_TICKS)) && 
        (m_uEdgePos <= (3 * EDGE_TICKS)))
        // Bottom pane
        res.y = - BOMBER_HEIGHT / 2;
    return res;
}

void
CBomber::FixEdgeDirection ()
{
    BOOL bTop    = (m_uEdgePos >= 0) && (m_uEdgePos <= EDGE_TICKS),
         bLeft   = ((m_uEdgePos >= (3 * EDGE_TICKS)) && (m_uEdgePos <= (4 * EDGE_TICKS - 1))) || (0 == m_uEdgePos),
         bRight  = (m_uEdgePos >= EDGE_TICKS) && (m_uEdgePos <= (2 * EDGE_TICKS)), 
         BBottom = (m_uEdgePos >= (2 * EDGE_TICKS)) && (m_uEdgePos <= (3 * EDGE_TICKS));

    ASSERT (m_uEdgePos < (4 * EDGE_TICKS));
    ASSERT (m_uDirectionIndex < MAX_DIRECTIONS);

    UINT u90Deg = MAX_DIRECTIONS / 4;

    if (bTop && (m_uDirectionIndex < (2 * u90Deg + 1)))
    {   // fix dir
        if (m_uDirectionIndex < u90Deg)
            m_uDirectionIndex = (MAX_DIRECTIONS - 1);
        else
            m_uDirectionIndex = 2 * u90Deg + 1;
    }
    if (bLeft && ((m_uDirectionIndex < (u90Deg + 1)) || (m_uDirectionIndex > (3 * u90Deg - 1))))
    {   // fix dir
        if (m_uDirectionIndex < (u90Deg + 1))
            m_uDirectionIndex = u90Deg + 1;
        else
            m_uDirectionIndex = (3 * u90Deg - 1);
    }
    if (bRight && (m_uDirectionIndex < (3 * u90Deg + 1)) && (m_uDirectionIndex > (u90Deg - 1)))
    {   // fix dir
        if (m_uDirectionIndex < (2 * u90Deg))
            m_uDirectionIndex = u90Deg - 1;
        else
            m_uDirectionIndex = (3 * u90Deg + 1);
    }
    if (BBottom && ((m_uDirectionIndex > (2 * u90Deg - 1)) || (m_uDirectionIndex < 1)))
    {   // fix dir
        if (0 == m_uDirectionIndex)
            m_uDirectionIndex = 1;
        else if (m_uDirectionIndex < (3 * u90Deg))
            m_uDirectionIndex = 2 * u90Deg - 1;
        else
            m_uDirectionIndex = 1;
    }
}


StateType           
CBomber::CalcState (DWORD dwCurTime)
{
    m_bImageChanged = FALSE;    // Assume no change since last CalcState
    if (BOMBER_STATE_DEAD == m_State)
        return STATE_DEAD;

    if (BOMBER_STATE_FLYING == m_State)
    {   // Flying now
        int iDistSqr = CalcNewPos (dwCurTime, FALSE);
        if (iDistSqr < 0) 
        {   // Out of map situation
            m_State = BOMBER_STATE_DEAD;
            return STATE_DEAD;
        }
        int iNumBombs = (dwCurTime - m_dwLastMoveTick) / BOMBS_TIME_GAP;
        while (iNumBombs--)
        {   // More bombs to drop
            m_dwLastMoveTick += BOMBS_TIME_GAP;
            CPoint BombPos;
            // Find out position of bomb at new time
            if (!FindBombPos (BombPos, m_dwLastMoveTick - m_dwInitialTime))
            { // Out of map
                m_State = BOMBER_STATE_DEAD;
                return STATE_DEAD;
            }
            // Drop the bomb
            CBomb *pBomb = new CBomb (BombPos, m_uDirectionIndex);
            // spawn bomb and send message to game mananger:
            CMessage::MessageData Params;
            Params.pGameObj = pBomb;
            VERIFY (m_MsgQueue.Enqueue(CMessage::ADD_OBJECT, Params));
            // Play sound
            TANKS_APP->m_gSoundManager.Play(CSoundManager::DROP_BOMB);
        }
        m_bImageChanged = TRUE;
        return STATE_ALIVE;
    }

    // Setup mode:

    if (!AfxIsValidAddress(m_pFather, sizeof (CTankObj)))
    {
        // Father tank is dead
        m_State = BOMBER_STATE_DEAD;
        return STATE_DEAD;
    }

    if (0 == m_dwLastMoveTick) {
        // First time
        m_dwLastMoveTick = dwCurTime;
        m_bImageChanged = TRUE;
        m_pManouverSet->UnsetBit (CManouverSet::AERIAL_SUPPORT);
    }

    UINT ManouverSet = m_pManouverSet->GetAll();
        // Each time we call GetAll on the MS we need to reset its values:
    TANKS_APP->m_gKbdManager.RefreshManouverSet();

    if (dwCurTime - m_dwLastMoveTick >= BOMBER_ROTATION_DELAY)
    {   // Can we move / rotate ?
        BOOL bMoveMade = FALSE;
        if (ManouverSet & CManouverSet::TURN_RIGHT_MASK) 
        {   // Rotate right
            m_uDirectionIndex = (m_uDirectionIndex + 1) % MAX_DIRECTIONS;
            bMoveMade = TRUE;
        }
        else if (ManouverSet & CManouverSet::TURN_LEFT_MASK) 
        {   // Rotate left
            m_uDirectionIndex = (m_uDirectionIndex == 0) ? (MAX_DIRECTIONS - 1) : (m_uDirectionIndex - 1);
            bMoveMade = TRUE;
        }
        if (ManouverSet & CManouverSet::FORWARD_MASK) 
        {   // Move forward
            m_uEdgePos = (m_uEdgePos + 1) % (4 * EDGE_TICKS);
            bMoveMade = TRUE;
        } else if (ManouverSet & CManouverSet::BACKWARD_MASK) 
        {   // Move backwards
            m_uEdgePos = (0 == m_uEdgePos) ? (4 * EDGE_TICKS - 1) : m_uEdgePos - 1;
            bMoveMade = TRUE;
        }
        if (bMoveMade)
        {
            m_dwLastMoveTick = dwCurTime;
            SetNewPos (CalcEdgePos());
            FixEdgeDirection ();
            m_GlobalImageManager.RotateImage (m_hImage, m_uDirectionIndex);
            m_bImageChanged = TRUE;
        }
    }
    if (ManouverSet & (CManouverSet::FIRE_SHELL_MASK | CManouverSet::FIRE_BULLET_MASK)) 
    {   // Launch bomber-> Send ADD_BOMBER to the server, and kill current bobmer.
        // The server will send ADD_BOMBER to all players (and us too) an we'll create a new
        // bomber object as a response.    
        m_pFather->RegainInput (TRUE);
//        m_State = BOMBER_STATE_FLYING;
//        m_InitialPos = m_Pos;
//        m_dwLastMoveTick = m_dwInitialTime = dwCurTime;
        m_pManouverSet->UnsetBit (CManouverSet::FIRE_SHELL);
        m_pManouverSet->UnsetBit (CManouverSet::FIRE_BULLET);
        SendBomberMsg ();
        return STATE_DEAD;  // Kill this object, a new bomber (flying) will be created soon
                            // when the game manager receives ADD_BOMBER from the server.
    }
    if (ManouverSet & (CManouverSet::DROP_MINE_MASK | CManouverSet::AERIAL_SUPPORT_MASK)) 
    {   // Cancel bomber
        m_pFather->RegainInput (FALSE);
        m_State = BOMBER_STATE_DEAD;
        m_pManouverSet->UnsetBit (CManouverSet::AERIAL_SUPPORT);
        m_pManouverSet->UnsetBit (CManouverSet::DROP_MINE);
        return STATE_DEAD;
    }

    return STATE_ALIVE;
}

BOOL
CBomber::FindBombPos (CPoint &ResPos, DWORD dwTimeGap)
{
    CalcPosAtTime (m_InitialPos, dwTimeGap, ResPos, FALSE);
    // Fix position to look as if the bomb is dropped from behind the bomber
    double radius = BOMBER_WIDTH / 2;

    ResPos.x += (int(radius) - int (radius * m_dDirectionsArr[m_uDirectionIndex].dXDir) - 9);
    ResPos.y += (int(radius) - int (radius * m_dDirectionsArr[m_uDirectionIndex].dYDir) - 9);

    WORD fl = GetMapPosition(ResPos);
    if ((X_NOT_IN_MAP & fl) || (Y_NOT_IN_MAP & fl)) 
    {   // Out of map situation
        return FALSE;
    }
    return TRUE;
}

void
CBomber::SendBomberMsg ()
{
    CMessage::MessageData Params;
        // Create bomber notification message:
    Params.BomberParam.Xpos = m_Pos.x;
    Params.BomberParam.Ypos = m_Pos.y;
    Params.BomberParam.Direction = m_uDirectionIndex;
        // Send it
    TANKS_APP->m_gOutgoingMsgQueue.Enqueue (CMessage::ADD_BOMBER, Params);
}
