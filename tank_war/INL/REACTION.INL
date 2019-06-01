/*****************************************************************************
*                                                                             
*   Reaction.inl
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
inline CReaction::CReaction (UINT uExplosionIntensity,
                             TerrainType TerrainDifficulty,
                             BonusType bt,
                             UINT TankID) :
    m_wReactionBits (WORD(uExplosionIntensity                            | 
                          (TerrainDifficulty << REACTION_TERRAIN_SHIFT)  |
                          (bt << REACTION_BONUS_SHIFT)                   |
                          (TankID << REACTION_TANKID_SHIFT)))
{}


inline UINT CReaction::GetExplosionIntensity ()
{
    return UINT(m_wReactionBits & REACTION_EXPLOSION_MASK);
}

inline TerrainType CReaction::GetTerrainDifficulty  ()
{
    return TerrainType((m_wReactionBits & REACTION_TERRAIN_MASK) 
                >> REACTION_TERRAIN_SHIFT);
}

inline BonusType CReaction::GetBonusType ()
{ 
    return BonusType((m_wReactionBits & REACTION_BONUS_MASK) 
                >> REACTION_BONUS_SHIFT);
}

inline UINT CReaction::GetTankID ()
{ 
    return UINT((m_wReactionBits & REACTION_TANKID_MASK) 
                >> REACTION_TANKID_SHIFT);
}

