/*****************************************************************************
*                                                                             
*   TankObj.cpp                                                            
*                                                                             
*   Electrical Engineering Faculty - Software Lab                             
*   Spring semester 1998                                                      
*                                                                             
*   Tanks game                                                                
*                                                                             
*   Module description: Implements the characteristics of the tank object.
*                                                                             
*   Authors: Eran Yariv - 28484475                                           
*            Moshe Zur  - 24070856                                           
*                                                                            
*                                                                            
*   Date: 23/09/98                                                           
*                                                                            
******************************************************************************/
#include "stdafx.h"
#include <math.h>
#include "tanks.h"
#include "TankObj.h"
#include "GameManager.h"
#include "Bullet.h"
#include "Shell.h"
#include "Mine.h"
#include "Message.h"
#include "Bomber.h"

// Regular delay between shells and bullets:
const UINT BULLET_FIRE_DELAY        =   UINT(1000.0 / double(BULLET_FIRE_RATE));    
const UINT SHELL_FIRE_DELAY         =   UINT(1000.0 / double(SHELL_FIRE_RATE));
// Extra short delay, when bonus is effective:
const UINT RAPID_BULLET_FIRE_DELAY  =   UINT(1000.0 / double(BULLET_FIRE_RATE)) / FIRE_RATE_BONUS;
const UINT RAPID_SHELL_FIRE_DELAY   =   UINT(1000.0 / double(SHELL_FIRE_RATE)) / FIRE_RATE_BONUS;

const DirOffsetType CTankObj::m_SpawnOffsets [24] =
{ //   Bullet    Shell     Mine
    {   0, 20,  -6,  17,  37,  16 },      //   0     ( <-- )
    {   1, 25,  -4,  22,  37,   8 },      //  15
    {   2, 29,  -3,  28,  35,   3 },      //  30
    {   6, 33,   2,  32,  30,  -3 },      //  45
    {   9, 36,   5,  36,  26,  -5 },      //  60
    {  15, 38,  11,  38,  21,  -7 },      //  75
    {  20, 38,  17,  38,  16,  -9 },      //  90     (  up )
    {  25, 38,  23,  38,   9,  -6 },      // 105     
    {  30, 36,  28,  36,   4,  -4 },      // 120     
    {  32, 32,  32,  32,  -2,   1 },      // 135     
    {  36, 29,  36,  27,  -5,   2 },      // 150    
    {  37, 24,  37,  22,  -7,  10 },      // 165     
    {  37, 18,  37,  16,  -9,  15 },      // 180     ( --> )     
    {  37, 13,  37,  10,  -7,  20 },      // 195     
    {  36,  9,  36,   5,  -4,  26 },      // 210     
    {  32,  5,  32,   1,  -2,  30 },      // 225     
    {  28,  2,  26,  -2,   3,  33 },      // 240     
    {  24,  0,  22,  -5,   9,  36 },      // 255     
    {  17, -1,  15,  -5,  15,  38 },      // 270     ( down )     
    {  13, -1,  10,  -6,  20,  38 },      // 285   
    {   8,  2,   5,  -2,  26,  36 },      // 300   
    {   4,  5,   0,   1,  32,  31 },      // 315   
    {   2,  9,  -3,   6,  34,  26 },      // 330   
    {   0, 14,  -5,  11,  37,  21 }       // 345   
};

const PosEntry UNUSED_POS_ENTRY;


CTankObj::CTankObj ( UINT uID, UINT uXPos, UINT uYPos, UINT uDirIndex, BOOL bLocal, UINT uShieldLevel, UINT uShells, 
                     UINT uBullets, UINT uMines, BYTE bFireRateBonusSecsLeft) :
        CMovingGameObject(uXPos, uYPos, TANK_WIDTH, TANK_HEIGHT, uDirIndex, double(TANK_MAX_VELOCITY)),
        m_uTankID(uID),
        m_uShieldLevel(uShieldLevel),
        m_uShells(uShells),
        m_uBullets(uBullets),
        m_uMines(uMines),
        m_pDlgWnd ((CTanksDlg*)(TANKS_APP->m_pMainWnd)),
        m_ManouverSet(TANKS_APP->m_gManouverSets[uID & 0x3]),
        m_ImageManager(TANKS_APP->m_gImageManager),
        m_ObjList(TANKS_APP->m_gGameManager.ExposeObjects()),
        m_MsgQueue(TANKS_APP->m_gIncomingMsgQueue),
        m_CommManager(TANKS_APP->m_gCommManager),
        m_Timer(TANKS_APP->m_gTimer),
        m_dwLastBulletFiredTick(0),
        m_dwLastShellFiredTick(0),
        m_dwLastMineDropTick(0),
        m_dwBulletFireDelay(BULLET_FIRE_DELAY),
        m_dwShellFireDelay(SHELL_FIRE_DELAY),
        m_bLocal (bLocal),
        m_dwLastRotationTime (0),
        m_bUseManouverSet (TRUE),
        m_bBomberInSetup (FALSE),
        m_bBomberAvail (FALSE),
        m_bForceNewPos (FALSE),
        m_bZombie (FALSE),
        m_PosTable (UNUSED_POS_ENTRY)
{
    ASSERT (uID < MAX_TANKS);
    
    m_hTankImage = m_ImageManager.GetImage ((CImageManager::ImageType)uID);
    m_hExplodedTankImage = m_ImageManager.GetImage (CImageManager::IMG_TANK_EXPLODE);
    m_hZombieOverlay = m_ImageManager.GetImage (CImageManager::IMG_TANK_ZOMBIE);

    // Test code  **************:
    m_ImageManager.RotateImage (m_hTankImage, m_uDirectionIndex);
    // End of test code **************
    m_pCurrentImage = &m_hTankImage;

    m_dwFireRateBonusLastTick = DWORD(bFireRateBonusSecsLeft) * 1000 + GetTickCount();

    if (m_bLocal) {
        m_pDlgWnd->SetShieldLevel (m_uShieldLevel);
        m_pDlgWnd->SetShellsCount (m_uShells);
        m_pDlgWnd->SetBulletsCount (m_uBullets);
        m_pDlgWnd->SetMinesCount (m_uMines);
        m_pDlgWnd->SetAerialSupport (FALSE);
        m_pDlgWnd->SetFastFireRate (m_dwFireRateBonusLastTick != 0);
    }
}

CReaction
CTankObj::React(CGameObject *pTo)
{
    ASSERT(pTo);

    CReaction res;

    switch (pTo->GetType())
    {
    case TANK:      // detect collision, and block the other object movement:
    case SHELL:     
    case BULLET:    
    case BOMB:
        if (CollidesWith(pTo))
        {
            res = CReaction(0, HIT_TANK, BONUS_NONE, m_uTankID);
        }
        break;
    }
    return res;
}

#pragma warning (disable : 4701)
/* warning C4701: local variable 'CurObjHImg', 'himgLastImage', 'uLastDir'
   may be used without having been initialized
   God damn it, trust me on this one - I know what I'm doing here....
*/


/*------------------------------------------------------------------------------

  Function: CalcState

  Purpose:  The main Tank object function. Called every game loop, the function
            collects the tank's maneuver, calculates the new position, and gathers
            the other game objects reaction to the new position. Upon return the
            tanks status (shield, ammo, bonuses) are updated.

  Input:    The local game time of the current game loop.

  Output:   Tank's new state.

  Remarks:  There's a difference between local and remote tanks behavior:
            The local tanks are allowed to set their state to dead, they need to
            update the game display (ammo etc.) and to dispatch the maneuver that
            was in use. Remote tanks don't need to do the above.

------------------------------------------------------------------------------*/
StateType 
CTankObj::CalcState (DWORD dwCurTime)
{ 
    m_bImageChanged = FALSE;    // Assume no change since last CalcState

    if (m_pCurrentImage == &m_hExplodedTankImage) 
    {   // if state==exploded - advance explosion or terminate
        if (m_ImageManager.ImageEnded(*m_pCurrentImage))  // That's it - we are done :-(
        {   // Explosion is over
            if (m_bLocal)   // It's the local tank that dies
            {
                // Go to game over animation mode
                m_pDlgWnd->GameOver ();
            }
            return STATE_DEAD;
        }
        return STATE_ALIVE; // Still exploding
    } 

    if (m_bZombie && !m_bForceNewPos)
        return STATE_ALIVE; // Zombie tank reacts to nothing !!

    // From here on starts the normal tank behaviour:
    
    CManouverSet ms;        // Detect manouver set (for network messages)
    BOOL bTryToRotate;      // Did the user dare to rotate ?
    HIMAGE himgLastImage;   // Current image, for undo purposes
    UINT uLastDir;          // Current rotation index, for undo purposes
    UINT ManouverSet;

    if (m_bUseManouverSet)
    {   // Can we use the manouver set to react to the user's whim
        //
        // Save movement params (for Undo option)
        //
        SaveMovement ();
        //
        //  handle manouver set:
        //
        ManouverSet = m_ManouverSet.GetAll();   
            // Each time we call GetAll on the MS we need to reset its values:
        TANKS_APP->m_gKbdManager.RefreshManouverSet();

        //
        // Calc new direction:
        //
        bTryToRotate = FALSE;              
        himgLastImage = m_hTankImage;
        uLastDir = m_uDirectionIndex;

        //
        // In cases of remote tanks, we occasionally get an absolute
        // direction from the net.
        //
        if (! m_bLocal && TANKS_APP->m_guRemoteTanksDirs[m_uTankID] != INVALID_DIRECTION)
        {   // We just got an absolute direction from a remote tank
            m_uDirectionIndex = TANKS_APP->m_guRemoteTanksDirs[m_uTankID];

            TANKS_APP->m_guRemoteTanksDirs[m_uTankID] = INVALID_DIRECTION;

            bTryToRotate = TRUE;
        } 
        else 
        {    
            // Normal game play - Either a local tank, or a remote w/o absolute direction:
            if (ManouverSet & CManouverSet::TURN_RIGHT_MASK)
            {   // Rotate right
                ms.SetBit (CManouverSet::TURN_RIGHT);   
                if (dwCurTime - m_dwLastRotationTime >= TANK_ROTATION_DELAY) 
                {   // enough time has passed sine last rotation
                    m_uDirectionIndex = (m_uDirectionIndex + 1) % MAX_DIRECTIONS;
                    bTryToRotate = TRUE;
                    ASSERT (m_uDirectionIndex < MAX_DIRECTIONS);
                } 
            }
            else if (ManouverSet & CManouverSet::TURN_LEFT_MASK)
            {   // Rotate left
                ms.SetBit (CManouverSet::TURN_LEFT);
                if (dwCurTime - m_dwLastRotationTime >= TANK_ROTATION_DELAY)
                {   // enough time has passed sine last rotation
                    m_uDirectionIndex = (m_uDirectionIndex == 0) ? 
                        (MAX_DIRECTIONS - 1) : (m_uDirectionIndex - 1);
                    bTryToRotate = TRUE;
                    ASSERT (m_uDirectionIndex < MAX_DIRECTIONS);
                }
            }
        } // end of local tank or remote w/o absolute direction

        if (bTryToRotate)
        {   // There's an attmept to rotate - Rotate the image
            m_ImageManager.RotateImage (m_hTankImage, m_uDirectionIndex);
            // Check if we're out of the map now
            if (GetMapPosition(m_Pos) != (X_FULL_IN_MAP | Y_FULL_IN_MAP)) 
            {   // Out of map situation - undo rotation
                m_uDirectionIndex = uLastDir;
                m_hTankImage = himgLastImage;
                ms.UnsetBit (CManouverSet::TURN_RIGHT);   // Cancel roations
                ms.UnsetBit (CManouverSet::TURN_LEFT);
            }
            else
            {   // Rotation o.k. - apply changes
                m_dwLastRotationTime = dwCurTime;
                m_bImageChanged = TRUE;
            }
        } // End of rotation attempt

        //
        // Calc new position: 
        //
        BOOL bTryToMove = FALSE;
        if (m_bForceNewPos)
        {   // We got a forced position and direction (due to checksum mismatch)
            SetNewPos (m_iNewForcedXPos, m_iNewForcedYPos);
            m_bForceNewPos = FALSE;       // Turn it off
            m_bImageChanged = TRUE;
        } 
        else 
        {   // Normal game play
            if (ManouverSet & CManouverSet::FORWARD_MASK) 
            {   // Move forward
                m_dVelocity = fabs(m_dVelocity);
                ms.SetBit (CManouverSet::FORWARD);
                bTryToMove = TRUE;
            } else if (ManouverSet & CManouverSet::BACKWARD_MASK) 
            {   // Move backwards
                m_dVelocity = -1.0 * fabs(m_dVelocity);
                ms.SetBit (CManouverSet::BACKWARD);
                bTryToMove = TRUE;
            }
            else
                StopMovement ();    // No movement chosen
            if (bTryToMove)
            {   // Tank tried to move
                if (CalcNewPos(dwCurTime) < 0)
                {   // 
                    // Failed to move - out of map case
                    //
                    UndoMovement ();
                    if (m_uDirectionIndex != uLastDir) 
                    {   // If we moved AND rotated, cancel rotation as well.
                        m_uDirectionIndex = uLastDir;
                        m_hTankImage = himgLastImage;
                    }
                    ms.UnsetBit (CManouverSet::FORWARD);      // Cancel movements:
                    ms.UnsetBit (CManouverSet::BACKWARD);
                    ms.UnsetBit (CManouverSet::TURN_RIGHT);   // Cancel roations
                    ms.UnsetBit (CManouverSet::TURN_LEFT);
                    m_bImageChanged = FALSE;
                }
                else
                {   
                    // Tries to move and successded (so far, collision not tested yet)
                    m_bImageChanged = TRUE;
                }
            }   // end of movement attempt
        } // end of normal game play
    }   // end of if (m_bUseManouverSet)
    //
    // handle reaction:
    //
    CReaction Reaction = m_ObjList.GetGameReaction (this);
    if (m_CommManager.IsHost() && (BONUS_NONE != Reaction.GetBonusType()))
        //
        // If this the host machine and a tank just hit a bonus, tell all the other
        // players (including this player) that the bonus has just been eaten.
        // This will cause a ADD_BONUS message to arrive with the Tank ID in 
        // the LifeSpan. The game manager, upon receipt of such a message will call 
        // the EatBonus method for the correct tank object.
        //
        m_CommManager.NotifyBonusEaten (m_uTankID);
    //
    // update state (shield level, image)
    //
    UINT ExplosionReact = Reaction.GetExplosionIntensity();
    if (ExplosionReact) 
    {   // We are hit:
        m_uShieldLevel -= ExplosionReact;    
        if (LONG(m_uShieldLevel) < 0)
		{
            // We are hit badly - that's the end:
			if (m_bLocal)	// If it's a remote tank, we have to
							// wait for the judge decision...
				Kill();
			else
				m_uShieldLevel = 0;
		}
        if (m_bLocal)
            // If it's a local tank, update shield status.
            m_pDlgWnd->SetShieldLevel (m_uShieldLevel);
    }
    //
    // update position:
    //
    TerrainType TerrainReact = Reaction.GetTerrainDifficulty();
    switch (TerrainReact)
    {
        case TERR_BLOCKED:
        case HIT_TANK:
            // we are blocked - rewind the new position:
            UndoMovement ();
            if (m_uDirectionIndex != uLastDir) {
                // There was also a rotation, undo it.
                m_uDirectionIndex = uLastDir;
                m_hTankImage = himgLastImage;
            }
            m_bImageChanged = FALSE;
                // Cancel all movements and rotations:
            ms.UnsetBit (CManouverSet::TURN_RIGHT);
            ms.UnsetBit (CManouverSet::TURN_LEFT);
            ms.UnsetBit (CManouverSet::FORWARD);
            ms.UnsetBit (CManouverSet::BACKWARD);
            break;
        case TERR_EMPTY:
            m_dVelocity = TANK_MAX_VELOCITY;
            break;
        case TERR_75SPEED:
            m_dVelocity = TANK_75_VELOCITY;
            break;
        case TERR_50SPEED:
            m_dVelocity = TANK_50_VELOCITY;
            break;
        case TERR_25SPEED:
            m_dVelocity = TANK_25_VELOCITY;
            break;
        default:
            ASSERT (FALSE);
            break;
    }

    if (m_bUseManouverSet)
    {   // Can we use the manouver set to react to the user's whim
        //
        // check the fire controls, and spawn bullet/shell/mine if needed:
        //
        if ((ManouverSet & CManouverSet::FIRE_SHELL_MASK) && // Fire shell button is pressed and
            m_uShells)                                       // we have shells 
        {
            if (ShellsEnabled(dwCurTime))    // we can fire a new shell
                FireShell(dwCurTime);
            ms.SetBit (CManouverSet::FIRE_SHELL);
        }

        if ((ManouverSet & CManouverSet::FIRE_BULLET_MASK) &&   // Fire bullet button is pressed and
            m_uBullets)                                         // we have bullets and
        {
            if (BulletsEnabled(dwCurTime))  // we can fire a new bullet
                FireBullet(dwCurTime);
            ms.SetBit (CManouverSet::FIRE_BULLET);
        }

        if ((ManouverSet & CManouverSet::DROP_MINE_MASK) &&     // Drop mine button is pressed and
            m_uMines)                                           // we have mines
        {
            if (MinesEnabled(dwCurTime)) // we can fire a new mine 
                // Try to drop a mine (may fail if there's no vacant room on the map)
                DropMine(dwCurTime);
            
            ms.SetBit (CManouverSet::DROP_MINE);
        }

        if ((ManouverSet & CManouverSet::AERIAL_SUPPORT_MASK) &&    // Bomber button is pressed and
            !m_bBomberInSetup &&                                    // we don't already have a bomber in setup mode and    
            m_bBomberAvail)                                         // we have the bomber icon on
        {
            LaunchBomber();
            // Notice, we don't set the bomber bit in the ms. Only when the bomber starts to fly
            // (after its setup mode) it sends ADD_BOMBER to all other players.
        }
    }


    // Before we leave - update the last tick calculated:
    // Check the fire rate bonus expiration time:
    if (m_dwFireRateBonusLastTick && (m_dwFireRateBonusLastTick <= dwCurTime)) 
    {   // turn off the fire rate bonus (timeout):
        m_dwBulletFireDelay = BULLET_FIRE_DELAY;
        m_dwShellFireDelay = SHELL_FIRE_DELAY;
        m_dwFireRateBonusLastTick = 0;
        if (m_bLocal)
            // Turn of the icon
            m_pDlgWnd->SetFastFireRate (FALSE);
    }
    SendManouverSet (ms);   // Notify other tanks on the network of our actions.

    // Store newly calculated position in table:
    PosEntry NewPos(dwCurTime, m_Pos.x, m_Pos.y);
    m_PosTable.AddHead(NewPos);

    return STATE_ALIVE; 
}

#pragma warning (default : 4701)


inline BOOL
CTankObj::BulletsEnabled(DWORD dwCurTime)
{
    return (dwCurTime - m_dwLastBulletFiredTick >= m_dwBulletFireDelay);
}
 
inline BOOL
CTankObj::MinesEnabled(DWORD dwCurTime)
{
    return (dwCurTime - m_dwLastMineDropTick >= MINE_FIRE_DELAY);
}


inline BOOL 
CTankObj::ShellsEnabled(DWORD dwCurTime)
{
    return (dwCurTime - m_dwLastShellFiredTick >= m_dwShellFireDelay);
}

void
CTankObj::FireShell(DWORD dwCurTime)
{
    // update flags:
    m_uShells--;
    m_dwLastShellFiredTick = dwCurTime;
    // spawn shell and send message to game mananger:
    CMessage::MessageData Params;
    Params.ShellParam.wXPos = WORD(m_SpawnOffsets[m_uDirectionIndex].iShellXOffset + m_Pos.x);
    Params.ShellParam.wYPos = WORD(m_SpawnOffsets[m_uDirectionIndex].iShellYOffset + m_Pos.y);
    Params.ShellParam.bDirectionIndex = BYTE(m_uDirectionIndex);
    Params.ShellParam.bParentTankID = BYTE(m_uTankID);

    VERIFY (m_MsgQueue.Enqueue(CMessage::ADD_SHELL, Params));
    if (m_bLocal)
    {
        // Update display
        m_pDlgWnd->SetShellsCount(m_uShells);
    }
    // Play the sound
    TANKS_APP->m_gSoundManager.Play(CSoundManager::FIRE_SHELL);
}

void
CTankObj::FireBullet(DWORD dwCurTime)
{
    // update flags:
    m_uBullets--;
    m_dwLastBulletFiredTick = dwCurTime;
    // spawn bullet and send message to game mananger:
    CMessage::MessageData Params;
    Params.BulletParam.wXPos = WORD(m_SpawnOffsets[m_uDirectionIndex].iBulletXOffset + m_Pos.x);
    Params.BulletParam.wYPos = WORD(m_SpawnOffsets[m_uDirectionIndex].iBulletYOffset + m_Pos.y);
    Params.BulletParam.bDirectionIndex = BYTE(m_uDirectionIndex);
    Params.BulletParam.bParentTankID = BYTE(m_uTankID);

    VERIFY (m_MsgQueue.Enqueue(CMessage::ADD_BULLET, Params));
    if (m_bLocal)
    {
        // Update display
        m_pDlgWnd->SetBulletsCount(m_uBullets);
    }
    // Play sound
    TANKS_APP->m_gSoundManager.Play(CSoundManager::FIRE_BULLET);
}

BOOL
CTankObj::DropMine(DWORD dwCurTime)
{
    CMine tmpMine (m_SpawnOffsets[m_uDirectionIndex].iMineXOffset + m_Pos.x,
                   m_SpawnOffsets[m_uDirectionIndex].iMineYOffset + m_Pos.y);
    CReaction Reaction = m_ObjList.GetGameReaction (&tmpMine);
        // check that we can spawn:
    if ((Reaction.GetTerrainDifficulty() == TERR_EMPTY) &&                   // board is empty
        (tmpMine.GetMapPosition(tmpMine.GetPos()) == (X_FULL_IN_MAP | Y_FULL_IN_MAP))) // and fully in map
    {
        // update flags:
        m_uMines--;
        m_dwLastMineDropTick = dwCurTime;
        // spawn mine and send message to game mananger:
        CMessage::MessageData Params;

        Params.MineParam.wXPos = WORD(tmpMine.GetPos().x);
        Params.MineParam.wYPos = WORD(tmpMine.GetPos().y);

        VERIFY (m_MsgQueue.Enqueue(CMessage::ADD_MINE, Params));

        if (m_bLocal)
        {
            // Update display
            m_pDlgWnd->SetMinesCount(m_uMines);
        }

        return TRUE;    // Mine dropped
    }
    return FALSE;   // Mine cannot be dropped
}

void
CTankObj::LaunchBomber()
{
    ASSERT (m_bLocal);
    ASSERT (m_bBomberAvail);
    ASSERT (!m_bBomberInSetup);

    CBomber *pBomber = new CBomber (this);
    // spawn mine and send message to game mananger:
    CMessage::MessageData Params;
    Params.pGameObj = pBomber;
    VERIFY (m_MsgQueue.Enqueue(CMessage::ADD_OBJECT, Params));
    m_bBomberInSetup = TRUE;
    m_bBomberAvail = FALSE;
    m_pDlgWnd->SetAerialSupport (FALSE);
}

void
CTankObj::RegainInput (BOOL bBomberLaunched)
{
    m_bUseManouverSet = TRUE;
    m_bBomberInSetup = FALSE;
    if (!bBomberLaunched)
    {   // Bomber canceled - re-enable it
        m_bBomberAvail = TRUE;
        m_pDlgWnd->SetAerialSupport (TRUE);
    }
}

void 
CTankObj::SendManouverSet ( CManouverSet &ms )
{
    if (!m_bLocal)
        return; // Don't send manouver set for remote tanks
    if (ms.GetAll() == m_PrevManouverSet.GetAll())
        // No change => Don't send
        return;

    CMessage::MessageData Params;
        // Create manouver set notification message:
    Params.ManouverSetParam.TankID = BYTE(m_uTankID);
    Params.ManouverSetParam.ManouverSet = ms.GetAll();
    Params.ManouverSetParam.Direction = BYTE(m_uDirectionIndex);
        // Send it
    TANKS_APP->m_gOutgoingMsgQueue.Enqueue (CMessage::MANOUVER_SET, Params);
        // Update last manouver set sent over the network
    m_PrevManouverSet = ms;
}

void
CTankObj::EatBonus (BonusType t, DWORD dwEatTime)
{
    switch (t)
    {
        case BONUS_SHELLS:
            m_uShells = min (MAX_STATUS_VALUE, m_uShells + TANK_BONUS_SHELLS);
            if (m_bLocal)
                m_pDlgWnd->SetShellsCount(m_uShells);
            break;
        case BONUS_BULLETS:
            m_uBullets = min (MAX_STATUS_VALUE, m_uBullets + TANK_BONUS_BULLETS);
            if (m_bLocal)
                m_pDlgWnd->SetBulletsCount(m_uBullets);
            break;
        case BONUS_MINES:
            m_uMines = min (MAX_STATUS_VALUE, TANK_BONUS_MINES + m_uMines);
            if (m_bLocal)
                m_pDlgWnd->SetMinesCount(m_uMines);
            break;
        case BONUS_BOMBER:
            if (m_bLocal)
                m_pDlgWnd->SetAerialSupport (TRUE);
            m_bBomberAvail = TRUE;
            break;
        case BONUS_FIRE_RATE:
            if (!m_dwFireRateBonusLastTick) {    // We don't already own the bonus
                m_dwBulletFireDelay = RAPID_BULLET_FIRE_DELAY;
                m_dwShellFireDelay = RAPID_SHELL_FIRE_DELAY;
            }
            m_dwFireRateBonusLastTick = dwEatTime + FIRE_RATE_BONUS_DURATION;
            if (m_bLocal)
                m_pDlgWnd->SetFastFireRate (TRUE);
            break;
        case BONUS_SHIELD:
            m_uShieldLevel = min (100, m_uShieldLevel + TANK_BONUS_SHIELD);
            if (m_bLocal)
                m_pDlgWnd->SetShieldLevel (m_uShieldLevel);
            break;
        default:
            ASSERT (FALSE); // Unsupported bonus type
            break;
    }
    // Play sound
    TANKS_APP->m_gSoundManager.Play(CSoundManager::PICK_BONUS);
}


WORD 
CTankObj::PackBits (WORD wVal, WORD wNumBits)
{
     // If the value can be fully represented by wNumBits bits, return it:
    if (wVal < (1 << wNumBits))
        return wVal;
    // First, find the position of the most significant bit :
    int iPos=15;
    WORD wValBackup = wVal;
    while (!(wVal & 0x8000))
    {   
        wVal = WORD(wVal << 1);
        iPos--;
    }
    // Now, simply shift right so that wNumBits MSBits fit into the result
    return WORD(wValBackup >> (iPos - wNumBits + 1));
}

void
CTankObj::GetStatus(CMessage::MessageData &MsgData)
{
    MsgData.TankStatusParam.bID =                   BYTE(m_uTankID);
    MsgData.TankStatusParam.bShieldLevel =          (BYTE)m_uShieldLevel;
    MsgData.TankStatusParam.dwShells =              m_uShells;
    MsgData.TankStatusParam.dwBullets =             m_uBullets;
    MsgData.TankStatusParam.dwMines =               m_uMines;
    MsgData.TankStatusParam.bFireRateBonusSecsLeft = 
                        BYTE(m_dwFireRateBonusLastTick - GetTickCount());
    MsgData.TankStatusParam.bBomber =               BYTE(m_bBomberAvail);
    MsgData.TankStatusParam.bFastFire =             (0 != m_dwFireRateBonusLastTick);
    MsgData.TankStatusParam.bZombie =               BYTE(m_bZombie);
}

/*------------------------------------------------------------------------------

  Function: SetPos

  Purpose:  Calculates tanks position, and tries to enforce it, next time we calc
            the tanks state. This function is used for REMOTE tanks only, when
            receiving updates from the tank's "owner" (a player in this game session
            that controls this tank).

  Input:    dwTime: Local game time in which the tanks position was recorded.
            XPos, YPos: Tank's position.

  Output:   None.

  Remarks:  Since this information arrived from the player that owns this tank,
            the time when the info was recored is at the past in our local game
            time. That means we can't just set the tanks position to the new one.
            Instead we keep a record of the tanks last positions and time in a
            table, and look through this table to find the closest available entry.
            If no such entry is found - the update is abandoned, other wise, we
            take the position offset between our record and the update we received, 
            and use it to correct the current tank's position.

------------------------------------------------------------------------------*/
void                
CTankObj::SetPos (DWORD dwTime, int XPos, int YPos)
{
    // Force a new position and direction (will take effect only on next call to CalcState)
    m_bForceNewPos = TRUE;

    // First we need to search the history table for a position near the time given:
    int ind = -1, min = INT_MAX;
    for (int i = 0; i < MAX_POS_TABLE; i++) {
        PosEntry posEntry = m_PosTable[i];
        if (DWORD(-1) == posEntry.m_dwTime)
            break;  // we covered all entries in use, and reached unused entry - just leave

        int delta = abs(posEntry.m_dwTime - dwTime);
        if (min > delta) {
            min = delta;
            ind = i;
        }
    }
    // Get rid of extreme points:
    // - Table is still empty (ind was never updated)
    // - The best entry found is still not relevant to the time stamp reported:
    if ((-1 == ind) ||
        ((UINT)abs(m_PosTable[ind].m_dwTime - dwTime) > TANKS_APP->GetMaxRTT()))
    {
        if (-1 == ind)
            NET_SYNC_TRACE(("TankObj::SetPos - empty table\n"))
        if (-1 != ind)
            NET_SYNC_TRACE(("TankObj::SetPos - time delta %d\n", \
                m_PosTable[ind].m_dwTime - dwTime))
        m_bForceNewPos = FALSE;
        return;
    }
    // Now we calc the offset from our position to tanks real position
    int dx = m_PosTable[ind].m_uXPos - XPos;
    int dy = m_PosTable[ind].m_uYPos - YPos;
    // And finally we apply the offset to tank's current position:
    m_iNewForcedXPos = m_Pos.x - dx;
    m_iNewForcedYPos = m_Pos.y - dy;
}


void
CTankObj::SetStatus(CMessage::MessageData &MsgData)
{
    // Make sure the message arrived at the right tank:
    ASSERT(MsgData.TankStatusParam.bID == m_uTankID);

    m_uShieldLevel  = MsgData.TankStatusParam.bShieldLevel;
    m_uShells       = MsgData.TankStatusParam.dwShells;
    m_uBullets      = MsgData.TankStatusParam.dwBullets;
    m_uMines        = MsgData.TankStatusParam.dwMines;
    SetZombie (MsgData.TankStatusParam.bZombie);

    m_dwFireRateBonusLastTick = MsgData.TankStatusParam.bFireRateBonusSecsLeft +
        GetTickCount();
    m_bBomberAvail  = MsgData.TankStatusParam.bBomber;

    if (MsgData.TankStatusParam.bFastFire)
    {   // Fast fire rate is set on:
        m_dwFireRateBonusLastTick = FIRE_RATE_BONUS_DURATION + GetTickCount();
    }
    if (m_bLocal)
    {   // Local tank - refresh display accordingly
        m_pDlgWnd->SetFastFireRate  (MsgData.TankStatusParam.bFastFire);
        m_pDlgWnd->SetAerialSupport (m_bBomberAvail);
        m_pDlgWnd->SetShieldLevel   (m_uShieldLevel);
        m_pDlgWnd->SetShellsCount   (m_uShells);
        m_pDlgWnd->SetBulletsCount  (m_uBullets);
        m_pDlgWnd->SetMinesCount    (m_uMines);
    }
}

void
CTankObj::GetStatusAndPos(CMessage::MessageData &MsgData)
{
    MsgData.TankParam.ID = m_uTankID;
    MsgData.TankParam.XPos = m_Pos.x;
    MsgData.TankParam.YPos = m_Pos.y;
    MsgData.TankParam.ShieldLevel = m_uShieldLevel;
    MsgData.TankParam.Shells = m_uShells;
    MsgData.TankParam.Bullets = m_uBullets;
    MsgData.TankParam.Mines = m_uMines;
    MsgData.TankParam.FireRateBonusSecsLeft = BYTE(m_dwFireRateBonusLastTick - GetTickCount());
    MsgData.TankParam.Bomber = m_bBomberAvail;
    MsgData.TankParam.FastFire = (0 != m_dwFireRateBonusLastTick);
    MsgData.TankParam.Direction = m_uDirectionIndex;
}

void
CTankObj::Kill ()
{
    m_uShieldLevel = 0;
    m_pCurrentImage = &m_hExplodedTankImage;
	if (m_bLocal)
		m_pDlgWnd->SetShieldLevel (m_uShieldLevel);
    // Play sound
    TANKS_APP->m_gSoundManager.Play(CSoundManager::TANK_EXPLODE);
}

CRect &
CTankObj::GetUpdateRectangle()
{
    if (m_pCurrentImage != &m_hExplodedTankImage)
        // Moving tank
        return CMovingGameObject::GetUpdateRectangle();
    else
    {   // Exploding tank
        CSize size;
        CPoint offset;

        m_GlobalImageManager.GetImageSize(*m_pCurrentImage, size);
        m_GlobalImageManager.GetImageOffset(*m_pCurrentImage, offset);

        m_UpdateRect = CRect (m_Pos + offset, size);
    }
    return m_UpdateRect;
}

