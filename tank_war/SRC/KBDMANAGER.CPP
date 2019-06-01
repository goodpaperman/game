/*****************************************************************************
*                                                                             
*   KbdManager.cpp                                                            
*                                                                             
*   Electrical Engineering Faculty - Software Lab                             
*   Spring semester 1998                                                      
*                                                                             
*   Tanks game                                                                
*                                                                             
*   Module description: The keyboard manager holds the player control keys in
*                       use. It allows the user to assign his own keys to the
*                       different tank actions, check for conflicts, and store
*                       the table for future use in the registry file.
*                                                                             
*   Authors: Eran Yariv - 28484475                                           
*            Moshe Zur  - 24070856                                           
*                                                                            
*                                                                            
*   Date: 23/09/98                                                           
*                                                                            
******************************************************************************/
#include <stdafx.h>
#include <KbdManager.h>
#include "keymappingdlg.h"

void
CKbdManager::SetKbdMapping()
{
    // Make a copy of current set of keys, and pass it to dlg:
    CKeysTable NewKeys = m_Keys;
    // Create Keys configuration dlg:
	CKeyMappingDlg dlg(&NewKeys);

	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)  // Replace the current settings with the new ones:
	{
        m_Keys = NewKeys;
	}
}

/*------------------------------------------------------------------------------

  Function: RefreshManouverSet

  Purpose:  Refresh the local tank's maneuver set after each use, to deal with
            situation of focus lost of the main game dialog.

  Input:    None.

  Output:   None.

  Remarks:  When the game loose it's focus, the WM_KEY_UP message may be missed,
            thus causing the tank to repeat it's last maneuver set. To eliminate
            this phenomena, we check the key board state after every use of the
            maneuver set by the local tank.

------------------------------------------------------------------------------*/
void
CKbdManager::RefreshManouverSet()
{
    ASSERT(m_pManouverSet);

    for (int i = 0; i < 8; i++)
    {
        UINT uKeyCode = m_Keys.GetKey(i);

        if (! GetAsyncKeyState(uKeyCode))   // Key is up or nother window got the kbd input
            m_pManouverSet -> UnsetBit (i);
    }
}