/*****************************************************************************
*                                                                             
*   Reaction.h
*                                                                             
*   Electrical Engineering Faculty - Software Lab                             
*   Spring semester 1998                                                      
*                                                                             
*   Tanks game                                                                
*                                                                             
*   Module description: Implements the game reaction object.
*                       
*                                                                             
*   Authors: Eran Yariv - 28484475                                           
*            Moshe Zur  - 24070856                                           
*                                                                            
*                                                                            
*   Date: 23/09/98                                                           
*                                                                            
******************************************************************************/
#ifndef _REACTION_H
#define _REACTION_H

typedef enum {  TERR_EMPTY  = 0,
                TERR_75SPEED,
                TERR_50SPEED,
                TERR_25SPEED,
                TERR_BLOCKED,       // Hit block area (wall)
                HIT_TANK            // Hit block area (tank)
             } TerrainType;

typedef enum {
    BONUS_NONE    = 0,
    BONUS_SHELLS,
    BONUS_BULLETS,
    BONUS_MINES,
    BONUS_BOMBER,
    BONUS_FIRE_RATE,
    BONUS_SHIELD,
    NUM_BONUSES
} BonusType;



class CReaction
{
public:

    CReaction (UINT uExplosionIntensity = 0,
               TerrainType uTerrainDifficulty = TERR_EMPTY,
               BonusType = BONUS_NONE,
               UINT TankID = 0);

    virtual ~CReaction() {}

    UINT            GetExplosionIntensity();
    TerrainType     GetTerrainDifficulty();
    BonusType       GetBonusType ();
    UINT            GetTankID ();

private:
    WORD    m_wReactionBits;
                    /*  Bits  0.. 5  - Explosion intensity (0..63)
                        Bits  6.. 8  - Terrain difficulty level.
                                       Values defined in GameBoard.h as TERR_XXX + HIT_TANK
                        Bits  9..11  - Bonus type (0=None, 1=Shells Ammo., 
                                       2 = Bullets ammo., 3=Mines, 
                                       4=Bomber, 5=Fire rate, 6=Shield)
                        Bits 12..14  - Tank ID (in case of HIT_TANK)
                    */


    enum {  REACTION_EXPLOSION_MASK     = 0x03F,
            REACTION_TERRAIN_MASK       = 0x1C0,
            REACTION_TERRAIN_SHIFT      = 0x6,
            REACTION_BONUS_MASK         = 0xE00,
            REACTION_BONUS_SHIFT        = 0x9,
            REACTION_TANKID_MASK        = 0x7000,
            REACTION_TANKID_SHIFT       = 0xC
         };
};

#include <Reaction.inl>

#endif