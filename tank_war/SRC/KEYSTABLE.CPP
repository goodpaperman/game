/*****************************************************************************
*                                                                             
*   KeysTable.cpp                                                            
*                                                                             
*   Electrical Engineering Faculty - Software Lab                             
*   Spring semester 1998                                                      
*                                                                             
*   Tanks game                                                                
*                                                                             
*   Module description: Each player defines it's own set of keys to control the
*                       tank. The key table object loads the last mapping to
*                       a table, either from the registry file or from a default
*                       hard coded table (when playing for the 1st time).
*                       During the game, each key pressed is looked in the table,
*                       and if it's one of the control keys, it alters the 
*                       maneuver set object assigned to our local tank.
*                                                                             
*   Authors: Eran Yariv - 28484475                                           
*            Moshe Zur  - 24070856                                           
*                                                                            
*                                                                            
*   Date: 23/09/98                                                           
*                                                                            
******************************************************************************/
#include <stdafx.h>
#include "Tanks.h"
#include "KeysTable.h"
#include "GameConsts.h"

CKeysTable::~CKeysTable()
{
    TANKS_APP -> SetStoredRightKey (m_uKeysArr[0]);
    TANKS_APP -> SetStoredLeftKey (m_uKeysArr[1]);
    TANKS_APP -> SetStoredForwardKey (m_uKeysArr[2]);
    TANKS_APP -> SetStoredBackwardKey (m_uKeysArr[3]);
    TANKS_APP -> SetStoredShellKey (m_uKeysArr[4]);
    TANKS_APP -> SetStoredBulletKey (m_uKeysArr[5]);
    TANKS_APP -> SetStoredMineKey (m_uKeysArr[6]);
    TANKS_APP -> SetStoredAerialKey (m_uKeysArr[7]);
}

CKeysTable::CKeysTable (const CKeysTable& rhs)
{
    for (int i = 0; i < CManouverSet::MAX_MANOUVER_BIT; i++)
        m_uKeysArr[i] = rhs.m_uKeysArr[i];
}

/*------------------------------------------------------------------------------

  Function: SetKey

  Purpose:  Assigns a new control key to the indicated action.

  Input:    ind: Index of the action in the table.
            key: Virtual key code of the requested key.

  Output:   Return TRUE if key could be assigned.

  Remarks:  Method return FALSE in case the key is allready in use for a
            different action.
------------------------------------------------------------------------------*/
BOOL
CKeysTable::SetKey (int ind, UINT key)
{
    BOOL bRes = FALSE;
        // Checks index is in bounds:
    if (0 <= ind && CManouverSet::MAX_MANOUVER_BIT > ind)
    {
        BOOL Duplicated = FALSE;    // Indicates key is already in use
            // Check that key is unique:
        for (int i = 0; i < CManouverSet::MAX_MANOUVER_BIT && !Duplicated; i++)
            Duplicated = (m_uKeysArr[i] == key);

        if (!Duplicated)
        {
            m_uKeysArr[ind] = key;
            bRes = TRUE;
        }
    }
    return bRes;
}

void
CKeysTable::RestoreDefault()
{
    for (int i = 0; i < CManouverSet::MAX_MANOUVER_BIT; i++)
        m_uKeysArr[i] = DEFAULT_KEYS_SETTINGS[i];
}

void
CKeysTable::InitTable()
{
    m_uKeysArr[0] = TANKS_APP -> GetStoredRightKey (DEFAULT_KEYS_SETTINGS[0]);
    m_uKeysArr[1] = TANKS_APP -> GetStoredLeftKey (DEFAULT_KEYS_SETTINGS[1]);
    m_uKeysArr[2] = TANKS_APP -> GetStoredForwardKey (DEFAULT_KEYS_SETTINGS[2]);
    m_uKeysArr[3] = TANKS_APP -> GetStoredBackwardKey (DEFAULT_KEYS_SETTINGS[3]);
    m_uKeysArr[4] = TANKS_APP -> GetStoredShellKey (DEFAULT_KEYS_SETTINGS[4]);
    m_uKeysArr[5] = TANKS_APP -> GetStoredBulletKey (DEFAULT_KEYS_SETTINGS[5]);
    m_uKeysArr[6] = TANKS_APP -> GetStoredMineKey (DEFAULT_KEYS_SETTINGS[6]);
    m_uKeysArr[7] = TANKS_APP -> GetStoredAerialKey (DEFAULT_KEYS_SETTINGS[7]);
}

CKeysTable& 
CKeysTable::operator= (const CKeysTable& rhs)
{
    if (this != &rhs)   // Just copy arrays, if it's not the same object:
    {
        for (int i = 0; i < CManouverSet::MAX_MANOUVER_BIT; i++)
            m_uKeysArr[i] = rhs.m_uKeysArr[i];
    }
    return *this;
}