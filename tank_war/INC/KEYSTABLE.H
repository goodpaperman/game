/*****************************************************************************
*                                                                             
*   KeysTable.h
*                                                                             
*   Electrical Engineering Faculty - Software Lab                             
*   Spring semester 1998                                                      
*                                                                             
*   Tanks game                                                                
*                                                                             
*   Module description: Implements the table of keys in use to control the 
*                       tank. Maps between the virtual key code and the action.
*                       
*                                                                             
*   Authors: Eran Yariv - 28484475                                           
*            Moshe Zur  - 24070856                                           
*                                                                            
*                                                                            
*   Date: 23/09/98                                                           
*                                                                            
******************************************************************************/
#ifndef _KEYS_TABLE_H_
#define _KEYS_TABLE_H_

#include <ManouverSet.h>

class CKeysTable
{
public:
    CKeysTable();                       
    ~CKeysTable();                      // Store default keys to registry
    CKeysTable (const CKeysTable& rhs); // Copy ctor

    CKeysTable& operator= (const CKeysTable& rhs);

    UINT GetKey (int ind) const;
    BOOL SetKey (int ind, UINT key);    // Checks key's uniqness

    void RestoreDefault();              // Get default keys from Game consts
    void InitTable();                   // Get keys' bindings from registry

private:
    UINT m_uKeysArr[CManouverSet::MAX_MANOUVER_BIT];
    BOOL m_fInitDone;                   // Set to true after RestoreDefault has accomplished
};

#include <KeysTable.inl>

#endif