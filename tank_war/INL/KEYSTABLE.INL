/*****************************************************************************
*                                                                             
*   KeysTable.inl
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
CKeysTable::CKeysTable()
{}

inline UINT
CKeysTable::GetKey (int ind) const
{
    ASSERT (0 <= ind && CManouverSet::MAX_MANOUVER_BIT > ind);
    return m_uKeysArr[ind];
}

