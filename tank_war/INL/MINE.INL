/*****************************************************************************
*                                                                             
*   Mine.inl
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
inline ObjectHeightType 
CMine::GetHeight()
{
    return LOWER_LEVEL;
}

inline GameObjectType
CMine::GetType()
{
    return MINE;
}

inline void
CMine::Kill ()
{
    m_dwStartTime = 1;  // This will cause the mine to automatically expire 
                        // on next call to CMine::CalcState
}
