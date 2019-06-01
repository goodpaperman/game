/*****************************************************************************
*                                                                             
*   KbdManager.h
*                                                                             
*   Electrical Engineering Faculty - Software Lab                             
*   Spring semester 1998                                                      
*                                                                             
*   Tanks game                                                                
*                                                                             
*   Module description: Handles the setting of the tanks keyboard controls.
*                       
*                                                                             
*   Authors: Eran Yariv - 28484475                                           
*            Moshe Zur  - 24070856                                           
*                                                                            
*                                                                            
*   Date: 23/09/98                                                           
*                                                                            
******************************************************************************/
#ifndef _KBD_MANAGER_H_
#define _KBD_MANAGER_H_

#include <ManouverSet.h>
#include <KeysTable.h>

class CKbdManager
{
public:
    CKbdManager();
    ~CKbdManager();
    
    void SetKbdMapping (void);  // Launch the dialog to let the user set key mappings
    void SetManouverSet(CManouverSet *pManouverSet);
    void InitKeysTable();       // Read settings from registry / game consts
    void RefreshManouverSet();	// Called after local tank received the current
                                // manouver set, to unset keys when window is no longer in
                                // focus.
    
    BOOL SetKey(UINT);
    BOOL UnsetKey(UINT);
    
private:
    CManouverSet *m_pManouverSet;
    CKeysTable m_Keys;   // Holds the virtual code for every manouver
};

#include "KbdManager.inl"

#endif