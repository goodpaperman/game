/*****************************************************************************
*                                                                             
*   GameManager.inl
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
CGameManager::~CGameManager()
{
    EndGame();
}

inline void
CGameManager::BeginGame()
{
    StartWorkerThread();
}

inline void
CGameManager::EndGame()
{
    EndWorkerThread(TRUE);
}

inline void 
CGameManager::RemoveObject (CGameObject *pGameObj)
{
    ASSERT (pGameObj);
    m_GameObjsList.RemoveObject(pGameObj);  // will also delete the allocated memory
}

inline void
CGameManager::AddObject (CGameObject *pGameObj)
{
    ASSERT (pGameObj);
    m_GameObjsList.AddObject(pGameObj);
}

inline CGameObjectsList &
CGameManager::ExposeObjects()
{
    return m_GameObjsList;
}

inline UINT 
CGameManager::GetFrequency ()
{
    return m_uFreq;
}

inline void
CGameManager::RefreshDisplay ()
{
    m_bRefreshAll = TRUE;
}

inline BOOL
CGameManager::IsAlive (UINT uTankID)
{
    ASSERT(uTankID < MAX_TANKS);
    return m_pTanks[uTankID] != NULL;
}

inline int
CGameManager::GetLocalTankID() const
{
    return m_iLocalTankID;
}