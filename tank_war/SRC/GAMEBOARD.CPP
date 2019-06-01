/*****************************************************************************
*                                                                             
*   GameBoard.cpp                                                            
*                                                                             
*   Electrical Engineering Faculty - Software Lab                             
*   Spring semester 1998                                                      
*                                                                             
*   Tanks game                                                                
*                                                                             
*   Module description: Implements the game board game object.
*                       
*                                                                             
*   Authors: Eran Yariv - 28484475                                           
*            Moshe Zur  - 24070856                                           
*                                                                            
*                                                                            
*   Date: 23/09/98                                                           
*                                                                            
******************************************************************************/

#include "stdafx.h"
#include <stdlib.h>
#include <ImageManager.h>
#include <Tanks.h>
#include <GameBoard.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/*------------------------------------------------------------------------------

  Function: React

  Purpose:  Returns the game board reaction to the given game object. Since the
            game board is a passive object, the reaction can only be blocked or
            non-blocked, and the collision detection is done for every map block
            the object overlaps.

  Input:    pTo - the game object we are reacting to

  Output:   The Reaction object

  Remarks:  
------------------------------------------------------------------------------*/
CReaction 
CGameBoard::React(CGameObject *pTo)
{
    switch (pTo->GetType()) 
    {   // We only react to moving object in the lower level and upper level:
        case TANK:
        case SHELL:
        case BULLET:
            {
                CPoint &ObjPos = pTo->GetPos();
                CSize &ObjSize = pTo->GetDimensions();
                // Calc the indexes of the map blocks the object is overlapping:
                UINT ul = ObjPos.x / MAP_BLOCK_SCALE,
                     ur = (ObjPos.x + ObjSize.cx -1) / MAP_BLOCK_SCALE,
                     ut = ObjPos.y / MAP_BLOCK_SCALE,
                     ub = (ObjPos.y + ObjSize.cy -1) / MAP_BLOCK_SCALE;

                TerrainType ter = TERR_EMPTY;
                // Init CurrPos and CurrSize to hold the rect we are currently checking against:
                CPoint CurrPos;
                CSize CurrSize(MAP_BLOCK_SCALE, MAP_BLOCK_SCALE);
                UINT x, y;
                // Check against every rect we are overlapping:
                for (x = ul, CurrPos.x = ul * MAP_BLOCK_SCALE; 
                     x <= ur; 
                     x++, CurrPos.x += MAP_BLOCK_SCALE)
                    for (y = ut, CurrPos.y = ut * MAP_BLOCK_SCALE;
                         y <= ub;
                         y++, CurrPos.y += MAP_BLOCK_SCALE) {
                        //CRect TempRect(CurrPos, CurrSize);
                        if (CollidesWith(pTo, CRect(CurrPos, CurrSize))) {
                            ter = max (ter, ReadMap (x,y));
                        }
                    }

                return CReaction (0, ter, BONUS_NONE);
            }

        default:
            return 0;
    }
}


BOOL 
CGameBoard::PickWallSeed (UINT &x, UINT &y)
{
    UINT uMaxTries= MAP_X_CELLS * MAP_Y_CELLS * 3;
    while (uMaxTries-- > 0) 
    {
        x = TANK_BLOCK_WIDTH + Rand (MAP_X_CELLS - 1 - (TANK_BLOCK_WIDTH * 2));
        y = TANK_BLOCK_HEIGHT + Rand (MAP_Y_CELLS - 1 - (TANK_BLOCK_HEIGHT * 2));
        BOOL bFoundPlace = TRUE;
        for (int i = -TANK_BLOCK_WIDTH; i <= TANK_BLOCK_WIDTH && bFoundPlace ; i++)
            for (int j = -TANK_BLOCK_HEIGHT; j <= TANK_BLOCK_HEIGHT && bFoundPlace; j++)
                if (TERR_EMPTY != ReadMap(UINT(int(x)+i), UINT(int(y)+j)))
                    bFoundPlace = FALSE;
        if (bFoundPlace)
            return TRUE;    // Success
    }
    return FALSE;   // Failure
}


void 
CGameBoard::PickNextWallBlock (UINT &x, UINT &y, BYTE bDirectionOptions)
{
    ASSERT (bDirectionOptions); // You gotta give me something to choose from.....
    BYTE b = m_bPrevDir;    
    if (0 == Rand(8))
        b = BYTE(1 << (Rand(4))); // b = 1, 2, 4, or 8
    for (;;)
    {
        if ((b & bDirectionOptions) == DIRECTION_UP)
        {
            y--;
            m_bPrevDir = b;
            return;
        }
        if ((b & bDirectionOptions) == DIRECTION_DOWN)
        {
            y++;
            m_bPrevDir = b;
            return;
        }
        if ((b & bDirectionOptions) == DIRECTION_LEFT)
        {
            x--;
            m_bPrevDir = b;
            return;
        }
        if ((b & bDirectionOptions) == DIRECTION_RIGHT)
        {
            x++;
            m_bPrevDir = b;
            return;
        }
        b = BYTE(1 << (Rand(4))); // b = 1, 2, 4, or 8
    }
}


BYTE 
CGameBoard::GetNextWallBlockOptions (UINT x, UINT y)
{
    BYTE bRes = 0;
    int i,j;
    BOOL bOK;

    // Check the up option:
    bOK = FALSE;
    if (x     >= TANK_BLOCK_WIDTH  && x     <= (MAP_X_CELLS - 1 - TANK_BLOCK_WIDTH) &&
        (y-1) >= TANK_BLOCK_HEIGHT && (y-1) <= (MAP_Y_CELLS - 1 - TANK_BLOCK_HEIGHT))
    {
        bOK = TRUE;
        for (i = -TANK_BLOCK_WIDTH; i <= TANK_BLOCK_WIDTH && bOK; i++)
            for (j = -(TANK_BLOCK_HEIGHT + 1) ; j <= -1 && bOK; j++)
                if (TERR_EMPTY != ReadMap(UINT(int(x)+i), UINT(int(y)+j)))
                    bOK = FALSE;
        if (bOK)
            bRes |= DIRECTION_UP;
    }            

    // Check the down option:
    bOK = FALSE;
    if (x     >= TANK_BLOCK_WIDTH  && x     <= (MAP_X_CELLS - 1 - TANK_BLOCK_WIDTH) &&
        (y+1) >= TANK_BLOCK_HEIGHT && (y+1) <= (MAP_Y_CELLS - 1 - TANK_BLOCK_HEIGHT))
    {
        bOK = TRUE;
        for (i = -TANK_BLOCK_WIDTH; i <= TANK_BLOCK_WIDTH && bOK; i++)
            for (j = 1 ; j <= (TANK_BLOCK_HEIGHT + 1) && bOK; j++)
                if (TERR_EMPTY != ReadMap(UINT(int(x)+i), UINT(int(y)+j)))
                    bOK = FALSE;
        if (bOK)
            bRes |= DIRECTION_DOWN;
    }            

    // Check the left option:
    bOK = FALSE;
    if ((x-1) >= TANK_BLOCK_WIDTH  && (x-1) <= (MAP_X_CELLS - 1 - TANK_BLOCK_WIDTH) &&
        y     >= TANK_BLOCK_HEIGHT && y     <= (MAP_Y_CELLS - 1 - TANK_BLOCK_HEIGHT))
    {
        bOK = TRUE;
        for (i = -(TANK_BLOCK_WIDTH + 1); i <= -1 && bOK; i++)
            for (j = -TANK_BLOCK_HEIGHT ; j <= TANK_BLOCK_HEIGHT && bOK; j++)
                if (TERR_EMPTY != ReadMap(UINT(int(x)+i), UINT(int(y)+j)))
                    bOK = FALSE;
        if (bOK)
            bRes |= DIRECTION_LEFT;
    }            

    // Check the right option:
    bOK = FALSE;
    if ((x+1) >= TANK_BLOCK_WIDTH  && (x+1) <= (MAP_X_CELLS - 1 - TANK_BLOCK_WIDTH) &&
        y     >= TANK_BLOCK_HEIGHT && y     <= (MAP_Y_CELLS - 1 - TANK_BLOCK_HEIGHT))
    {
        bOK = TRUE;
        for (i = 1; i <= (TANK_BLOCK_WIDTH + 1) && bOK; i++)
            for (j = -TANK_BLOCK_HEIGHT ; j <= TANK_BLOCK_HEIGHT && bOK; j++)
                if (TERR_EMPTY != ReadMap(UINT(int(x)+i), UINT(int(y)+j)))
                    bOK = FALSE;
        if (bOK)
            bRes |= DIRECTION_RIGHT;
    }            

    return bRes;
}



void 
CGameBoard::ErectWalls (UINT uComplexityLevel)
{
    CDIB dibWallBlock;      // 1 map block of TERR_BLOCKED

    UINT uMaxWalls = 6 + Rand (uComplexityLevel);       // Max number of walls
         

    dibWallBlock.ReadFromResource (IDB_WALLBLOCK);
    CDIB *pDIB = TANKS_APP->m_gImageManager.ExposeDIB (m_hImage);

    while (uMaxWalls--) 
    {   // While there are still wall to erect
        UINT x,y, uCurWallLength = 0,
             uMaxWallLenght = 6 + 2 * Rand (uComplexityLevel);  // Max wall length
        BOOL bWallDeadEnd = FALSE;

        if (FALSE == PickWallSeed (x, y))
            return; // Can't create more walls        
        while (!bWallDeadEnd)
        {
            if (++uCurWallLength >= uMaxWallLenght)
                bWallDeadEnd = TRUE;   // Length exceeded => fake deadend => stop wall
            WriteMap (x,y, TERR_BLOCKED);
            pDIB->PasteCKRect (&dibWallBlock, 
                               x * MAP_BLOCK_SCALE, 
                               y * MAP_BLOCK_SCALE, 
                               TRANSP_COLOR);
                // Get optional directions for next wall block
            BYTE bDirectionOptions = GetNextWallBlockOptions (x,y);
            if (bDirectionOptions == 0) // Dead end
                bWallDeadEnd = TRUE;
            else
                // Pick direction from options    
                PickNextWallBlock (x, y, bDirectionOptions);
        }
    }
}

BOOL 
CGameBoard::FindAndMarkBlockLocation (UINT &x, UINT &y, 
                                           UINT uBlocksWidth, UINT uBlocksHeight,
                                           TerrainType ter)
{
    UINT uMaxTries= (MAP_X_CELLS - 2) * (MAP_Y_CELLS - 2) * 3;
    while (uMaxTries-- > 0) 
    {
        BOOL bOccupied = FALSE;

        x = 1 + Rand (MAP_X_CELLS - uBlocksWidth - 1);
        y = 1 + Rand (MAP_Y_CELLS - uBlocksHeight - 1);
        for (int u = -1; u < int(uBlocksWidth+1) && !bOccupied; u++)
            for (int v = -1; v < int(uBlocksHeight+1) && !bOccupied; v++)
                if (TERR_EMPTY != ReadMap (UINT(x) + u, UINT(y) + v))
                    bOccupied = TRUE;   // Sorry, it's occupied
        if (!bOccupied)
        {
            // Found free spot
            for (UINT z = 0; z < uBlocksWidth; z++)
                for (UINT w = 0; w < uBlocksHeight; w++)
                    WriteMap (x + z, y + w, ter);
            return TRUE;    // Success
        }
    }
    return FALSE;
}

void 
CGameBoard::PlaceObstacle (UINT uComplexityLevel, 
                           UINT uImageID, 
                           TerrainType ter)
{
    CDIB dibBlocks[7];    // maps of block - 7 variations
    
        // Create all possible block variations:
    dibBlocks[0].ReadFromResource (uImageID);
    dibBlocks[1].CreateRotated (&dibBlocks[0], 90, FALSE, FALSE);
    dibBlocks[2].CreateRotated (&dibBlocks[0],180, FALSE, FALSE);
    dibBlocks[3].CreateRotated (&dibBlocks[0],270, FALSE, FALSE);
    dibBlocks[4].CreateRotated (&dibBlocks[0],  0, TRUE , FALSE);
    dibBlocks[5].CreateRotated (&dibBlocks[0],  0, FALSE, TRUE );
    dibBlocks[6].CreateRotated (&dibBlocks[0],  0, TRUE , TRUE );

    CDIB *pDIB = TANKS_APP->m_gImageManager.ExposeDIB (m_hImage);

    UINT uMaxBlocks = 2 + UINT(0.33 * float(1 + Rand (uComplexityLevel)));
    while (uMaxBlocks--)
    {
            // Pick block image variation
        UINT uBlockImage = Rand (7);   
        UINT x,y, 
             uBlockWidth  = BlockSize(dibBlocks[uBlockImage].Width()),
             uBlockHeight = BlockSize(dibBlocks[uBlockImage].Height());

        if (FALSE == FindAndMarkBlockLocation (x, y, uBlockWidth, uBlockHeight, ter))
            uMaxBlocks = 0;  // Stop placing ponds
        else
            pDIB->PasteCKRect (&dibBlocks[uBlockImage], 
                             x * MAP_BLOCK_SCALE, 
                             y * MAP_BLOCK_SCALE, 
                             TRANSP_COLOR);
    }
}
            

/*------------------------------------------------------------------------------

  Function: GenerateBoard

  Purpose:  Generates this game board depending on the complexity level, and on
            the seed of the random engine. This seed characterize the game board
            thus the same board is generated each time we use the same seed.

  Input:    uRandSeed - the random engine seed number
            uComplexityLevel - the complexity level of this board

  Output:   None.

  Remarks:  The game complexity is read from the registry.
            Passing the same seed and the complexity level to every game client 
            generates the board.

------------------------------------------------------------------------------*/
void 
CGameBoard::GenerateBoard (UINT uRandSeed, UINT uComplexityLevel)
{
        // obstacles of 75% speed are yet to be implemented....
    srand (uRandSeed);
    ASSERT (uComplexityLevel <= 20 && uComplexityLevel > 0);
        // Ask for board background to be loaded
    TANKS_APP->m_gImageManager.ReloadMap();
    m_hImage = TANKS_APP->m_gImageManager.GetImage (CImageManager::IMG_BOARD);

        // Start with an empty map (no obstacles)
    for (UINT x = 0; x < MAP_X_CELLS; x++)
        for (UINT y = 0; y < MAP_Y_CELLS; y++)
            WriteMap (x,y, TERR_EMPTY);

    PlaceObstacle (uComplexityLevel, IDB_POND, TERR_25SPEED);
    PlaceObstacle (uComplexityLevel, IDB_BOULDERS, TERR_50SPEED);
    ErectWalls (uComplexityLevel);
}

/*------------------------------------------------------------------------------

  Function: IsRectVacant

  Purpose:  Check if the given rectangle intersect with any background object on
            the map.

  Input:    rect - the rectangle to be check.

  Output:   TRUE if the rectangle is vacant = contains no walls or obstacles.

  Remarks:  The background objects consists of blocks - so we check the blocks
            that the given rectangle intersects.


------------------------------------------------------------------------------*/
BOOL
CGameBoard::IsRectVacant(CRect& rect)
{
        // Calc the indexes of the map blocks the rectangle intersects:
    UINT ul = rect.left / MAP_BLOCK_SCALE,
         ur = rect.right / MAP_BLOCK_SCALE,
         ut = rect.top / MAP_BLOCK_SCALE,
         ub = rect.bottom / MAP_BLOCK_SCALE;

    for (UINT x = ul; x <= ur; x++)
        for (UINT y = ut; y <= ub; y++)
            if (ReadMap (x, y) != TERR_EMPTY)
                return FALSE;

    return TRUE;
}