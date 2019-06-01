/*****************************************************************************
*                                                                             
*   GameOver.inl
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
inline StateType           
CGameOver::CalcState (DWORD /*dwCurTime*/)
{
    return STATE_ALIVE;
}

inline ObjectHeightType    
CGameOver::GetHeight()
{
    return HIGHER_LEVEL;
}

inline HIMAGE
CGameOver::GetImage()
{
    m_GlobalImageManager.UpdateImage (m_hImage, m_bImageChanged);
    return m_hImage;
}

inline CReaction           
CGameOver::React(CGameObject *  /*pTo*/)
{
    return CReaction(); // No reaction
}

inline GameObjectType      
CGameOver::GetType()
{
    return GAMEOVER;
}

inline CGameOver::~CGameOver()
{
    m_GlobalImageManager.ReleaseGameOverBitmaps();
}