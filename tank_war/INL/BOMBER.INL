/*****************************************************************************
*                                                                             
*   Bomber.inl
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
CBomber::~CBomber()
{
}

inline HIMAGE 
CBomber::GetImage()
{
    return m_hImage;
}

inline GameObjectType 
CBomber::GetType()
{
    return BOMBER;
}

inline ObjectHeightType 
CBomber::GetHeight()
{
    return SKY_LEVEL;
}

inline BOOL
CBomber::IsLocal ()
{
    return m_bLocal;
}

inline CReaction           
CBomber::React(CGameObject *)
{
    return CReaction ();    // Indifferent object
}
