/*****************************************************************************
*                                                                             
*   ManouverSet.cpp                                                            
*                                                                             
*   Electrical Engineering Faculty - Software Lab                             
*   Spring semester 1998                                                      
*                                                                             
*   Tanks game                                                                
*                                                                             
*   Module description: Each tank is assigned a Maneuver Set describing the user
*                       key board input in terms of maneuvers - move forward, 
*                       backward, turn etc. The maneuver set object resolve any 
*                       conflicts that may occur when pressing keys that control
*                       contradicting maneuvers, and is send over the net and
*                       to the tanks or bomber objects.
*                                                                             
*   Authors: Eran Yariv - 28484475                                           
*            Moshe Zur  - 24070856                                           
*                                                                            
*                                                                            
*   Date: 23/09/98                                                           
*                                                                            
******************************************************************************/
#include "stdafx.h"
#include "ManouverSet.h"

const UINT CManouverSet::MOVE_COLLISION = FORWARD_MASK | BACKWARD_MASK;
const UINT CManouverSet::TURN_COLLISION = TURN_RIGHT_MASK | TURN_LEFT_MASK;

void 
CManouverSet::SetBit (UINT ManouverBit)
{
    UINT uMask = (1 << ManouverBit) & MANOUVER_SET_MASK; // Calc mask, to set new val

        // Check for turn key collisions:
    if ( ((uMask | m_uManouverSet) & // Suppose we set the bit, does it collide ?
            TURN_COLLISION) == TURN_COLLISION ) 
    {
        m_uManouverSet &= ~TURN_COLLISION;  // Clear both bits - conflict is settled
        return;
    } 
        // Check for move key collisions:
    if ( ((uMask | m_uManouverSet) & // Suppose we set the bit, does it collide ?
            MOVE_COLLISION) == MOVE_COLLISION )
    {
        m_uManouverSet &= ~MOVE_COLLISION;  // Clear both bits - conflict is settled
        return;
    } 
        // it's now safe to set bit:
    m_uManouverSet |= uMask;
}

