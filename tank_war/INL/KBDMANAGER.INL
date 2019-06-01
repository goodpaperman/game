/*****************************************************************************
*                                                                             
*   KbdManager.inl
*                                                                             
*   Electrical Engineering Faculty - Software Lab                             
*   Spring semester 1998                                                      
*                                                                             
*   Tanks game                                                                
*                                                                             
*   Contents: Inline functions implementations.
*                                                                             
*   Authors: Eran Yariv - 28484475                                           
*            Moshe Zur  - 24070856                                           
*                                                                            
*                                                                            
*   Date: 23/09/98                                                           
*                                                                            
******************************************************************************/
inline
CKbdManager::CKbdManager() :
m_pManouverSet(NULL)
{}

inline
CKbdManager::~CKbdManager()
{}

inline void
CKbdManager::SetManouverSet(CManouverSet *pManouverSet)
{
    ASSERT(pManouverSet);
    m_pManouverSet = pManouverSet;
}

inline void
CKbdManager::InitKeysTable()
{
    m_Keys.InitTable();
}

inline BOOL
CKbdManager::SetKey(UINT uKey)
{
    if (! m_pManouverSet)
        return FALSE;

    for (UINT ind = 0; ind < CManouverSet::MAX_MANOUVER_BIT; ind++)
    {   
        if (uKey == m_Keys.GetKey(ind))
        {
                // Set relevant manouver bit
            m_pManouverSet -> SetBit (ind);
            return TRUE;
        }
    }
    return FALSE;
}

inline BOOL
CKbdManager::UnsetKey(UINT uKey)
{
    if (! m_pManouverSet)
        return FALSE;

    for (UINT ind = 0; ind < CManouverSet::MAX_MANOUVER_BIT; ind++)
    {   
        if (uKey == m_Keys.GetKey(ind))
        {
                // Set relevant manouver bit
            m_pManouverSet -> UnsetBit (ind);
            return TRUE;
        }
    }
    return FALSE;
}
