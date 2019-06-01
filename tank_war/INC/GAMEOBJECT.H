/*****************************************************************************
*                                                                             
*   GameObject.h
*                                                                             
*   Electrical Engineering Faculty - Software Lab                             
*   Spring semester 1998                                                      
*                                                                             
*   Tanks game                                                                
*                                                                             
*   Module description: Implements the game object class hierarchy.
*                       
*                                                                             
*   Authors: Eran Yariv - 28484475                                           
*            Moshe Zur  - 24070856                                           
*                                                                            
*                                                                            
*   Date: 23/09/98                                                           
*                                                                            
******************************************************************************/
#ifndef _GAME_OBJECT_H
#define _GAME_OBJECT_H

#include <ImageManager.h>
#include <GameConsts.h>
#include <Reaction.h>

class CGameObjectsList; // Pre-declaration

typedef struct {
    double dXDir, dYDir;
} DirectionType;

typedef struct {
    double dXPos, dYPos;
} PrecisePosType;

typedef enum {STATE_DEAD, STATE_ALIVE} StateType;

typedef enum {TANK, BOARD, SHELL, 
              BULLET, MINE, BOMB, 
              BOMBER, BONUS, GAMEOVER} GameObjectType;

typedef enum {GROUND_LEVEL,     // Add object at ground level (lowest) - there's room for only one object there.
              LOWER_LEVEL,      // Add object above ground level 
              HIGHER_LEVEL,     // Add object above lower level
              SKY_LEVEL         // Add object at sky level (highest) - there's room for only one object there.
             } ObjectHeightType;

class CGameObject : public CObject
{
public:

typedef enum {  X_FULL_IN_MAP       =   0x01,   // Map fully contains the object in the X axis
                X_PARTIAL_IN_MAP    =   0x02,   // Map partailly contains the object in the X axis
                X_NOT_IN_MAP        =   0x04,   // Map doesn't contains the object in the X axis
                Y_FULL_IN_MAP       =   0x08,   // Map fully contains the object in the Y axis
                Y_PARTIAL_IN_MAP    =   0x10,   // Map partailly contains the object in the Y axis
                Y_NOT_IN_MAP        =   0x20    // Map doesn't contains the object in the Y axis
             } MapPositionFlags;

    CGameObject ();

    void SetPos (UINT uXPos, UINT uYPos);
    void SetSize (UINT uXSize, UINT uYSize);

    virtual ~CGameObject () {}

    virtual StateType           CalcState (DWORD dwCurTime)     = 0;
    virtual CPoint &            GetPos();
    virtual ObjectHeightType    GetHeight()                     = 0;
    virtual HIMAGE              GetImage()                 = 0;
    virtual CReaction           React(CGameObject *pTo)         = 0;
    virtual GameObjectType      GetType()                       = 0;
    virtual void                Kill();
    virtual CRect &             GetUpdateRectangle();
    virtual CSize &             GetDimensions();
        // This method is here only for the tank objects:
    virtual                     UINT GetID ();

    BOOL                        HasImageChanged ();
    BYTE                        GetSector();        // Gets sector on map
    BYTE                        GetPosCheckSum();   // Get position packed into a byte

        // Returns bits representing flags from MapPositionFlags
    WORD                        GetMapPosition (CPoint &pos); 
    void                        SetListPosition (POSITION);
    POSITION                    GetListPosition ();
    void                        SetArrayPosition (CGameObject**);
    CGameObject**               GetArrayPosition ();
    BOOL                        CollidesWith (CGameObject *);
    BOOL                        CollidesWith (CGameObject* pOtherObj, CRect& MyRect);

protected:
    CPoint          m_Pos;                  // Object position
    CSize           m_Size;                 // Object size
    POSITION        m_ListPosition;         // Object position in objects list
    CGameObject**   m_ppArrayPosition;      // Object position in objects array
    CRect           m_UpdateRect;           // Update rectangle
    BOOL            m_bImageChanged;        // Indicate a change in display

    BOOL BitMaskCollides (CRect& IntersectRect, CPoint& myPos, CPoint& otherPos,
                          CBitMask* myBitMask, CBitMask* otherBitMask);

        // The following refrences are shortcuts to global structures:
    CImageManager      &m_GlobalImageManager;
    CGameObjectsList   &m_GlobalObjsList;
};

/* ------------------------- Moving object ------------------------- */

class CMovingGameObject : virtual public CGameObject
{
public:

    CMovingGameObject (UINT uXPos, UINT uYPos, UINT uXSize, UINT uYSize,
                       UINT uDirectionIndex = 12, 
                       double dVelocity = 0.0);

    virtual ~CMovingGameObject () {}

        // Calculates new position. Returns move distance (squared) or -1 if out of map
    int             CalcNewPos(DWORD dwCurTime, BOOL bMustBeFullyInMap = TRUE);
    void            SaveMovement ();
    void            UndoMovement ();
    void            StopMovement ();

    virtual CRect & GetUpdateRectangle();

    void            SetNewPos (CPoint &);
    void            SetNewPos (int x, int y);

protected:

    UINT                        m_uDirectionIndex;  // Direction in m_dDirectionsArr

    static const DirectionType  m_dDirectionsArr[]; // all possible directions
    DWORD                       m_uLastTick;        // Holds the last time tick the tank state was updated
    double                      m_dVelocity;        // Velocity in pixels per second

    BOOL                        CalcPosAtTime (CPoint &StartPos,
                                               DWORD dwTimeGap,
                                               CPoint &ResPos,
                                               BOOL bMustBeFullyInMap = TRUE);

private:
    PrecisePosType              m_PrecPos,          // Precise position
                                m_PrecPosCopy,      // Copy of precise position (for SaveMovement / UndoMovement)
                                m_PrevPrecPos,      // Previous precise position
                                m_PrevPrecPosCopy;  // Copy of previous precise position (for SaveMovement / UndoMovement)
    CPoint                      m_PosCopy;          // Copy of integer position
    static const double         m_cdThousandth;     // Constant 1/1000 - for faster calcs.

};




/* ------------------------- Exploding object ------------------------- */

class CExplodingGameObject : virtual public CGameObject
{
public:

    CExplodingGameObject (UINT uXPos, UINT uYPos, UINT uXSize, UINT uYSize,
                          UINT uMaxIntensity,
                          CImageManager::ImageType uExplosionImageIndex,
                          BOOL InitiallyExploding = FALSE);

    virtual ~CExplodingGameObject() {}

    virtual HIMAGE  GetImage();
    virtual CRect & GetUpdateRectangle();

    UINT    CalcRelativeExplosionIntensity (CGameObject *pOtherObject, UINT MinRadius, UINT MaxRadius);
    void    Explode ();    // Set explosion to true and switch image
    BOOL    IsExplosionOver (); // Are we exploding and after last frame ?
    BOOL    CheckAndSetTankNotification (UINT uTankID);   // TankID = 0..3

protected:

    BOOL            m_bIsExploding,                 // Are we exploding now ?
                    m_bNotifiedTanks[MAX_TANKS];    // What tanks did we hit already?

    HIMAGE          m_himgExplode,                  // Explosion image
                   *m_pCurImage;                    // Current image
    UINT            m_uMaxIntensity;                // Maximal intensity
};


// Inline sections:
#include <GameObject.inl>

#endif