/*****************************************************************************
*                                                                             
*   GameObjectsList.inl
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
inline CGameObjectsList::CGameObjectsList ()
{
    m_aArrays[GROUND_LEVEL].Init(1);      // Game board only
    m_aArrays[LOWER_LEVEL].Init(MAX_POSSIBLE_OBJECTS);
    m_aArrays[HIGHER_LEVEL].Init(MAX_POSSIBLE_OBJECTS);
    m_aArrays[SKY_LEVEL].Init(MAX_POSSIBLE_OBJECTS); // Airplanes and game-over objects
}


inline CGameObjectsList::~CGameObjectsList ()
{
    KillList();
}

inline void
CGameObjectsList::KillList ()
{
    for (int i = 0; i < 4; i++)
        m_aArrays[i].Empty();
}

inline void
CGameObjectsList::AddObject (CGameObject *pGameObj)
{
    ASSERT(pGameObj);
    Freeze();
    VERIFY(m_aArrays[pGameObj->GetHeight()].Add(pGameObj));
    Thaw();
}

inline void 
CGameObjectsList::RemoveObject (CGameObject *pGameObj)
{
    ASSERT(pGameObj);
    Freeze();
    VERIFY(m_aArrays[pGameObj->GetHeight()].Remove(pGameObj));
    Thaw();
}

inline int 
CGameObjectsList::GetObjectsCount () const
{
    return  m_aArrays[0].GetObjectsCount() +
            m_aArrays[1].GetObjectsCount() +
            m_aArrays[2].GetObjectsCount() +
            m_aArrays[3].GetObjectsCount();
}

inline LIST_POS
CGameObjectsList::GetHeadPosition ()
{
    LIST_POS lp;

    lp.ind = 0;
    lp.pos = m_aArrays[0].GetHeadPosition();

    return lp;
}

inline CGameObject* 
CGameObjectsList::GetNext (LIST_POS& lp)
{
    if (lp.ind < 0 || lp.ind > 3)
        return NULL;

    CGameObject* RetVal = m_aArrays[lp.ind].GetNext(lp.pos);

    if (! RetVal) {     // We have reached current array's top
        while (++lp.ind < 4)  // Advance to next non-empty array, if possible
        {
            lp.pos = m_aArrays[lp.ind].GetHeadPosition();
            RetVal = m_aArrays[lp.ind].GetNext(lp.pos);
            if (RetVal)
                // Success
                return RetVal;
        }
    }
    return RetVal;
}

inline void
CGameObjectsList::Freeze()
{
    m_CS.Lock();
}

inline void
CGameObjectsList::Thaw()
{
    m_CS.Unlock();
}