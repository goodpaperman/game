/*****************************************************************************
*                                                                             
*   TankObj.inl
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
CTankObj::~CTankObj()
{
}

inline UINT 
CTankObj::GetID ()
{
    return m_uTankID;
}

inline UINT 
CTankObj::GetShieldLevel ()
{
    return m_uShieldLevel;
}

inline UINT 
CTankObj::GetShellsCount ()
{
    return m_uShells;
}

inline UINT 
CTankObj::GetBulletsCount ()
{
    return m_uBullets;
}

inline UINT 
CTankObj::GetMinesCount ()
{
    return m_uMines;
}

inline UINT
CTankObj::SetShieldLevel (UINT uShield)
{
    UINT uOldShield = m_uShieldLevel;

    m_uShieldLevel = uShield;
    return uOldShield;
}

inline UINT
CTankObj::SetShellsCount (UINT uShells)
{
    UINT uOldShells = m_uShells;

    m_uShells = uShells;
    return uOldShells;
}

inline UINT
CTankObj::SetBulletsCount (UINT uBullets)
{
    UINT uOldBullets = m_uBullets;

    m_uBullets = uBullets;
    return uOldBullets;
}

inline UINT
CTankObj::SetMinesCount (UINT uMines)
{
    UINT uOldMines = m_uMines;

    m_uMines = uMines;
    return uOldMines;
}

inline HIMAGE 
CTankObj::GetImage()
{
    m_GlobalImageManager.UpdateImage (*m_pCurrentImage, m_bImageChanged);
    return *m_pCurrentImage;
}

inline GameObjectType 
CTankObj::GetType()
{
    return TANK;
}

inline ObjectHeightType 
CTankObj::GetHeight()
{
    return LOWER_LEVEL;
}

inline BOOL
CTankObj::IsLocal ()
{
    return m_bLocal;
}

inline void
CTankObj::RelinquishInput ()
{
    m_bUseManouverSet = FALSE;
}

inline CManouverSet&       
CTankObj::GetManouverSet () const
{
    return m_ManouverSet;
}

inline BOOL
CTankObj::IsExploding ()
{
    return (m_pCurrentImage == &m_hExplodedTankImage);
}


inline WORD                
CTankObj::GetStateCheckSum()
{   // Gets check sum (16 bits) of shield level, ammo level and fast fire rate
    return WORD(
            PackBits (WORD(m_uShieldLevel), 4)            +   // Pack shield  level to 4 bits ( 0.. 3)
           (PackBits (WORD(m_uShells),      4) <<  4)     +   // Pack shells  count to 4 bits ( 4.. 7)
           (PackBits (WORD(m_uBullets),     4) <<  8)     +   // Pack bullets count to 4 bits ( 8..11) 
           (PackBits (WORD(m_uMines),       3) << 12)     +   // Pack mines   count to 3 bits (12..14)
           (m_dwFireRateBonusLastTick ? 0x8000 : 0));   // Pack fast fire rate to a bit (15)
}

inline UINT
CTankObj::GetDirection ()
{
    return m_uDirectionIndex;
}

inline BOOL
CTankObj::IsZombie ()
{
    return m_bZombie;
}

inline void
CTankObj::SetZombie (BOOL b)
{
    m_bZombie = b;
        // Fix image to reflect zombie mode change:
    m_ImageManager.SetOverlay (m_hTankImage, m_bZombie ? &m_hZombieOverlay : NULL);
}

inline BOOL
CTankObj::GetFastFire()
{
    return (0 != m_dwFireRateBonusLastTick);
}