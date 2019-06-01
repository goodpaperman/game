/*****************************************************************************
*                                                                             
*   GameBoard.h
*                                                                             
*   Electrical Engineering Faculty - Software Lab                             
*   Spring semester 1998                                                      
*                                                                             
*   Tanks game                                                                
*                                                                             
*   Module description: Implements the game board object.
*                       
*                                                                             
*   Authors: Eran Yariv - 28484475                                           
*            Moshe Zur  - 24070856                                           
*                                                                            
*                                                                            
*   Date: 23/09/98                                                           
*                                                                            
******************************************************************************/
#if !defined _GAME_BOARD_H
#define _GAME_BOARD_H

#if _MSC_VER >= 1000
#pragma once
#endif 

#include <GameObject.h>
#include <GameConsts.h>



class CGameBoard : public CGameObject
{
public:
	CGameBoard();
	virtual ~CGameBoard();

    void                GenerateBoard (UINT uRandSeed,          // Random seed
                                       UINT uComplexityLevel    // Complexity level [1..20]
                                      );

    StateType           CalcState (DWORD dwCurTime);
    CPoint &            GetPos();
    ObjectHeightType    GetHeight();
    HIMAGE              GetImage(); // Call after a call to GenerateBoard only!
    CReaction           React(CGameObject *pTo);
    GameObjectType      GetType();
    CRect &             GetUpdateRectangle();
    CSize &             GetDimensions();

    BOOL                IsRectVacant(CRect&);   // return TRUE if rect is vacant

private:

    enum {  MAP_BLOCK_SCALE                     = 10,
            MAP_X_CELLS                         = MAP_WIDTH / MAP_BLOCK_SCALE,
            MAP_Y_CELLS                         = MAP_HEIGHT / MAP_BLOCK_SCALE,
            DIRECTION_UP                        = 0x01,
            DIRECTION_LEFT                      = 0x02,
            DIRECTION_RIGHT                     = 0x04,
            DIRECTION_DOWN                      = 0x08,
            DIRECTION_UPLEFT                    = 0x10,
            DIRECTION_UPRIGHT                   = 0x20,
            DIRECTION_DOWNLEFT                  = 0x40,
            DIRECTION_DOWNRIGHT                 = 0x80,
         };



    TerrainType         ReadMap (UINT x, UINT y);
    void                WriteMap (UINT x, UINT y, TerrainType);
    UINT                Rand (UINT uMax);
    BOOL                PickWallSeed (UINT &x, UINT &y);
    BYTE                GetNextWallBlockOptions (UINT x, UINT y);
    void                PickNextWallBlock (UINT &x, UINT &y, BYTE bDirectionOptions);
    void                ErectWalls (UINT uComplexityLevel);
    void                PlaceObstacle (UINT uComplexityLevel, UINT uImageID, TerrainType);
    BOOL                FindAndMarkBlockLocation (UINT &x, UINT &y, 
                                                  UINT uBlocksWidth, UINT uBlocksHeight, TerrainType);
    UINT                BlockSize (UINT uPixelSize);
    BOOL                InBoard (UINT x, UINT y);


    HIMAGE    m_hImage;
    CRect     m_EmptyRect;  // To return at GetUpdateRectangle
    BYTE     *m_pMap; 
    BYTE      m_bPrevDir;

};

#include <GameBoard.inl>



#endif

