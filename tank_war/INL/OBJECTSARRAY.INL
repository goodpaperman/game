/*****************************************************************************
*                                                                             
*   ObjectsArray.inl
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
inline CObjectsArray::CObjectsArray () :
m_uSize(0),
m_Array(NULL),
m_lplpTop(NULL)
{
}

inline void
CObjectsArray::Init (UINT uSize)
{
    m_uSize = uSize;
    // Ensure array size is bigger than 0, and allocation succeeded:
    ASSERT(m_uSize);
    m_Array = new CGameObject*[m_uSize];
    m_lplpTop = m_Array;
    m_lplpLast = &m_Array[m_uSize-1];
}

inline CObjectsArray::~CObjectsArray()
{
    delete [] m_Array;
}

inline ARR_POS
CObjectsArray::GetHeadPosition()
{
    return m_Array;     // Initialized to NULL on empty array
}

inline CGameObject* 
CObjectsArray::GetNext(ARR_POS& pos)
{
    CGameObject *pObj = NULL;

    if (pos < m_lplpTop && pos >= m_Array)   // If legal and we can still advance
        pObj = *pos++;

    return pObj;
}

inline BOOL 
CObjectsArray::Add(CGameObject *pGameObj)
{
    if (m_lplpTop <= m_lplpLast) {  // Is there a place ?
        ASSERT(pGameObj);
        pGameObj->SetArrayPosition(m_lplpTop);   // Store position for fast removal
        *m_lplpTop++ = pGameObj;    // Store pointer and advance
        return TRUE;
    }
    return FALSE;
}

inline BOOL 
CObjectsArray::Remove(CGameObject *pGameObj)
{
    // Object stores his position in list:
    ASSERT(pGameObj);
    CGameObject **lplpDelObj = pGameObj->GetArrayPosition();
    ASSERT(lplpDelObj && lplpDelObj>=m_Array && lplpDelObj<m_lplpTop);
    
    // Replace last entry with the one just evacuated:
    *lplpDelObj = *(--m_lplpTop);   // This will also move top one entry down
    
    // Update the newly shuffled object's position:
    (*lplpDelObj)->SetArrayPosition(lplpDelObj);

    // And ofcourse - delete object
    delete pGameObj;

    return TRUE;
}

inline BOOL 
CObjectsArray::IsEmpty()
{
    return (m_lplpTop == m_Array);
}

inline int 
CObjectsArray::GetObjectsCount() const
{
    return (DWORD(m_lplpTop) - DWORD(m_Array)) / sizeof(CGameObject*);
}

inline void
CObjectsArray::Empty()
{
    for (CGameObject** p = m_Array; p < m_lplpTop; p++)
        delete (*p);
    m_lplpTop = m_Array;
}
