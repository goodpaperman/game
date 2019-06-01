/*****************************************************************************
*                                                                             
*   Bullet.inl
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
CBullet::GetHeight()
{
    return HIGHER_LEVEL;
}

inline HIMAGE
CBullet::GetImage()
{
    m_GlobalImageManager.UpdateImage (m_himgBullet, m_bImageChanged);
    return m_himgBullet;
}

inline GameObjectType
CBullet::GetType()
{
    return BULLET;
}

