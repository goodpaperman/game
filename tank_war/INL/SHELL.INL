/*****************************************************************************
*                                                                             
*   Shell.inl
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
CShell::GetHeight()
{
    return HIGHER_LEVEL;
}


inline GameObjectType
CShell::GetType()
{
    return SHELL;
}

inline CRect &
CShell::GetUpdateRectangle()
{
    if (m_bIsExploding)
        return CExplodingGameObject::GetUpdateRectangle();
    else
        return CMovingGameObject::GetUpdateRectangle();
}

