/*****************************************************************************
*                                                                             
*   Bonus.inl
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
CBonus::~CBonus()
{}

inline StateType           
CBonus::CalcState (DWORD dwCurTime)
{
    if ((BONUS_NONE == m_Type) ||               // Either the bonus is taken already or,
        (m_uLifeSpan < dwCurTime - m_uStartTime))   // it's life span is exhausted
        return STATE_DEAD;
    return STATE_ALIVE;
}

inline ObjectHeightType    
CBonus::GetHeight()
{
    return LOWER_LEVEL;
}

inline HIMAGE
CBonus::GetImage()
{
    m_GlobalImageManager.UpdateImage (m_hImage, m_bImageChanged);
    return m_hImage;
}

inline CReaction           
CBonus::React(CGameObject *pTo)
{
    if (TANK == pTo->GetType() && CollidesWith(pTo)) { // The first tank gets it all:
        BonusType OldType = m_Type;
        m_Type = BONUS_NONE;
        return CReaction(0, TERR_EMPTY, OldType);
    }
    return CReaction(); // By default bonus type is BONUS_NONE
}

inline GameObjectType      
CBonus::GetType()
{
    return BONUS;
}

inline BonusType           
CBonus::GetBonusType ()
{
    return m_Type;
}

inline void
CBonus::Kill ()
{
    m_Type = BONUS_NONE;    // Notify dead state
}
