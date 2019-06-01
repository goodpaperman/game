/*****************************************************************************
*                                                                             
*   MyEdit.h
*                                                                             
*   Electrical Engineering Faculty - Software Lab                             
*   Spring semester 1998                                                      
*                                                                             
*   Tanks game                                                                
*                                                                             
*   Module description: Implements edit box control that handles all keyboard
*                       inputs (for the keyboard mapping dialog).
*                       
*                                                                             
*   Authors: Eran Yariv - 28484475                                           
*            Moshe Zur  - 24070856                                           
*                                                                            
*                                                                            
*   Date: 23/09/98                                                           
*                                                                            
******************************************************************************/
#ifndef _MYEDIT_H_
#define _MYEDIT_H_

#include <KeysTable.h>

class CMyEdit : public CEdit
{
public:

    CMyEdit::CMyEdit (CKeysTable* pKeys, int ind) : 
      CEdit(), m_pKeys(pKeys), m_iIndex(ind)
    {
        ASSERT( pKeys );  // Dialog must receive a valid key table
        ASSERT( SetKeyName (pKeys->GetKey(ind)) );  // Make sure key table isn't corrupted:
                                                    // the key must be a legitimate one
    }
    
    BOOL PreTranslateMessage(MSG* pMsg);    // We overload this function to take care of
                                            // all keys passed to CEdit

    void UpdateText ()  // Synchronize string with keytable, and update control's caption
    {
            // Assert that key code in table is allways legitimate:
        ASSERT( SetKeyName (m_pKeys->GetKey(m_iIndex)) );
        SetWindowText( m_cstrKeyName);
    }

private:
    // Methods
    BOOL SetKeyName (UINT nKey);
    // Members
    CString     m_cstrKeyName;  // Temp place to hold the key's name as appears to user
    CKeysTable* m_pKeys;        // Points to the current key table being configured
    int         m_iIndex;       // Holds the index in the keys' table corresponding 
                                //  with the control
};

#endif