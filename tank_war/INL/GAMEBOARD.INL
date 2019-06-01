/*****************************************************************************
*                                                                             
*   GameBoard.inl
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
inline CGameBoard::CGameBoard () :
    m_EmptyRect (0,0,0,0),
    m_pMap (new BYTE[MAP_X_CELLS * MAP_Y_CELLS]),
    m_bPrevDir (1)
{
    SetPos (0,0);
    SetSize (MAP_WIDTH, MAP_HEIGHT);
}

inline CGameBoard::~CGameBoard ()
{
    delete [] m_pMap;
}

inline StateType 
CGameBoard::CalcState (DWORD)
{
    return STATE_ALIVE;
}

inline CPoint &
CGameBoard::GetPos()
{
    return m_Pos;
}

inline ObjectHeightType
CGameBoard::GetHeight()
{
    return GROUND_LEVEL;
}

inline HIMAGE 
CGameBoard::GetImage ()
{
    return m_hImage;
}

inline GameObjectType 
CGameBoard::GetType ()
{
    return BOARD;
}

inline CRect &
CGameBoard::GetUpdateRectangle()
{
    return m_EmptyRect;
}

inline CSize &
CGameBoard::GetDimensions ()
{
    return m_Size;
}

inline TerrainType 
CGameBoard::ReadMap (UINT x, UINT y)
{
    if (!InBoard(x, y))
        return TERR_BLOCKED;
    return (TerrainType)m_pMap[x + y * MAP_X_CELLS];
}

inline void 
CGameBoard::WriteMap (UINT x, UINT y, TerrainType ter)
{
    ASSERT (InBoard(x, y) && ter <= TERR_BLOCKED);
    m_pMap[x + y * MAP_X_CELLS] = BYTE(ter);
}

inline UINT 
CGameBoard::Rand (UINT uMax)
{   // Returns random number between 0..uMax-1
    ASSERT (uMax < RAND_MAX);
    return rand() % uMax;
}

inline UINT 
CGameBoard::BlockSize (UINT uPixelSize)
{
    return ((uPixelSize % MAP_BLOCK_SCALE) == 0) ?
           (uPixelSize / MAP_BLOCK_SCALE ) :
           (1 + uPixelSize / MAP_BLOCK_SCALE);
}            

inline BOOL CGameBoard::InBoard (UINT x, UINT y)
{
    return (x < MAP_X_CELLS && y < MAP_Y_CELLS);
}
