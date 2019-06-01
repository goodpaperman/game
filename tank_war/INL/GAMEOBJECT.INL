/*****************************************************************************
*                                                                             
*   GameObject.inl
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
inline void CGameObject::SetListPosition (POSITION pos)
{
    m_ListPosition = pos;
}

inline POSITION CGameObject::GetListPosition ()
{
    return m_ListPosition;
}

inline void CGameObject::SetArrayPosition (CGameObject** pos)
{
    m_ppArrayPosition = pos;
}

inline CGameObject** CGameObject::GetArrayPosition ()
{
    return m_ppArrayPosition;
}

inline CRect &
CGameObject::GetUpdateRectangle()
{
    return (m_UpdateRect = CRect(m_Pos, m_Size));
}

inline CPoint &
CGameObject::GetPos ()
{
    return m_Pos;
}

inline CSize &
CGameObject::GetDimensions ()
{
    return m_Size;
}

inline UINT
CGameObject::GetID()
{
    // We shouldn't reach this point - this method is just a place holder for the tank object.
    ASSERT(FALSE);  
    return 0;
}

inline void
CGameObject::Kill ()
{
    ASSERT (FALSE); // Must be overridden by derived class if function is to be used.
}

inline void 
CGameObject::SetPos (UINT uXPos, UINT uYPos)
{
    m_Pos.x = uXPos;
    m_Pos.y = uYPos;
    m_bImageChanged = TRUE;
}

inline void 
CGameObject::SetSize (UINT uXSize, UINT uYSize)
{
    m_Size.cx = uXSize;
    m_Size.cy = uYSize;
    m_bImageChanged = TRUE;
}

inline BOOL
CGameObject::HasImageChanged ()
{
    return m_bImageChanged;
}

inline BYTE                
CGameObject::GetSector()
{   // Returns screen sector (0..MAX_SECTOR)

    // Since the X position is 0..MAP_WIDTH-1, it takes MAP_WIDTH_BITS bits to represent on each axis.
    // Shifting (MAP_WIDTH_BITS-2) bits to the right leaves the 2 MSBs intact.
    // Summing the X and Y axis this way gives us the MAX_SECTOR possible sectors.
    return BYTE(((m_Pos.x >> (MAP_WIDTH_BITS-2)) << 2) | (m_Pos.y >> (MAP_HEIGHT_BITS-2)));
}

inline BYTE
CGameObject::GetPosCheckSum()
{   // Returns position checksum within a sector

    // Since the sector number already tells us about the 2 MSBs of each axis.
    // We are left with the MAP_WIDTH_BITS-2 LSBs. 
    // Summing them up on both axis gives a rather unique
    // checksum (with a BYTE overflow).

    // Since this is a descent compiler, the complex expressions below will be consts at run-time.
    return BYTE (BYTE(m_Pos.x & ((1 << (MAP_WIDTH_BITS  - 2)) - 1)) + 
                 BYTE(m_Pos.y & ((1 << (MAP_HEIGHT_BITS - 2)) - 1)));
}

/* ------------------------- Moving object ------------------------- */

inline CMovingGameObject::CMovingGameObject (  UINT uXPos, UINT uYPos, UINT uXSize, UINT uYSize,
                                               UINT uDirectionIndex, 
                                               double dVelocity):
    m_uDirectionIndex (uDirectionIndex),
    m_dVelocity (dVelocity),
    m_uLastTick (0)
{
    SetPos (uXPos, uYPos);
    SetSize (uXSize, uYSize);
    m_PrevPrecPos.dXPos = m_PrecPos.dXPos = m_Pos.x;
    m_PrevPrecPos.dYPos = m_PrecPos.dYPos = m_Pos.y;
}


inline CRect &
CMovingGameObject::GetUpdateRectangle()
{
    CRect PrevRect (CPoint (int(m_PrevPrecPos.dXPos + 0.5), int(m_PrevPrecPos.dYPos + 0.5)), m_Size),
          CurRect  (m_Pos, m_Size);
          
    m_UpdateRect.UnionRect ( PrevRect, CurRect);
    return m_UpdateRect;
}

inline void 
CMovingGameObject::SaveMovement ()
{
    m_PrecPosCopy = m_PrecPos;   
    m_PrevPrecPosCopy = m_PrevPrecPos;
    m_PosCopy = m_Pos;
}

inline void 
CMovingGameObject::UndoMovement ()
{
    m_PrecPos = m_PrecPosCopy;   
    m_PrevPrecPos = m_PrevPrecPosCopy;
    m_Pos = m_PosCopy;
    StopMovement ();
}

inline void 
CMovingGameObject::StopMovement ()
{
    m_uLastTick = 0;
}        

inline void                        
CMovingGameObject::SetNewPos (CPoint &pos)
{
    SetNewPos (pos.x ,pos.y);
}



/* ------------------------- Exploding object ------------------------- */



inline HIMAGE CExplodingGameObject::GetImage()
{
    m_GlobalImageManager.UpdateImage (*m_pCurImage, m_bImageChanged);
    return *m_pCurImage;
}

inline CRect &
CExplodingGameObject::GetUpdateRectangle()
{
    CSize size;
    CPoint offset;

    m_GlobalImageManager.GetImageSize(*m_pCurImage, size);
    m_GlobalImageManager.GetImageOffset(*m_pCurImage, offset);

    m_UpdateRect = CRect (m_Pos + offset, size);
    return m_UpdateRect;
}

inline void
CExplodingGameObject::Explode ()
{
    m_bIsExploding = TRUE;
    m_pCurImage = &m_himgExplode;
}

inline BOOL
CExplodingGameObject::CheckAndSetTankNotification (UINT uTankID)
{
    ASSERT (uTankID < MAX_TANKS);
    BOOL bState = m_bNotifiedTanks[uTankID];
    m_bNotifiedTanks[uTankID] = TRUE;
    return bState;
}

inline BOOL
CExplodingGameObject::IsExplosionOver ()
{
    return (m_bIsExploding &&    // Mine is exploding and after its last frame
            m_GlobalImageManager.ImageEnded ( m_himgExplode ));
}

