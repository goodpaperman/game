/*****************************************************************************
*                                                                             
*   GameObject.cpp                                                            
*                                                                             
*   Electrical Engineering Faculty - Software Lab                             
*   Spring semester 1998                                                      
*                                                                             
*   Tanks game                                                                
*                                                                             
*   Module description: Implements the GameObject hierarchy :
*                                   GameObject (abstract)
*                                      /   \
*                           MovingObject   ExplodingObject
*                       All game objects (tanks, shells, etc.) are derived from 
*                       one or more (multiple inheritance) of these classes.
*                       
*                                                                             
*   Authors: Eran Yariv - 28484475                                           
*            Moshe Zur  - 24070856                                           
*                                                                            
*                                                                            
*   Date: 23/09/98                                                           
*                                                                            
******************************************************************************/
#include <stdafx.h>
#include <math.h>
#include <GameObject.h>
#include <GameConsts.h>
#include <Tanks.h>

CGameObject::CGameObject () :
    m_ListPosition (NULL),
    m_UpdateRect (m_Pos, m_Size),
    m_GlobalImageManager (TANKS_APP->m_gImageManager),
    m_GlobalObjsList (TANKS_APP->m_gGameManager.ExposeObjects())
{}


/*------------------------------------------------------------------------------

  Function: GetMapPosition

  Purpose:  Returns the objects current position on game board (map).
            The object can be entirely seen (fully in map), partialy seen
            (some portion of the object is out of map), and unseen.

  Input:    pos - the suggested position of the object on the map.

  Output:   Status of the object visiblity given the suggested position.

  Remarks:  


------------------------------------------------------------------------------*/
WORD CGameObject::GetMapPosition (CPoint &pos)
{
    WORD wRes = 0;
    CRect ObjRect (pos, m_Size),
          TmpRect,
          MapRect (0, 0, MAP_WIDTH-1, MAP_HEIGHT-1);

    CBitMask *myBitMask = m_GlobalImageManager.ExposeBitMask(GetImage());
    if (NULL != myBitMask)
    {
        // There's a better rectangle for the object
        CRect &ActualRect = myBitMask->GetActualRect();
        int x1 = pos.x + ActualRect.left,
            y1 = pos.y + ActualRect.top;
        ObjRect.SetRect (x1, y1, x1 + ActualRect.Width(), y1 + ActualRect.Height());
    }

    if (ObjRect.left >= 0 && ObjRect.right < MAP_WIDTH) 
        wRes |= X_FULL_IN_MAP;      // Map fully contains the object in the X axis
    else if (TmpRect.IntersectRect (ObjRect, MapRect) != 0)
        wRes |= X_PARTIAL_IN_MAP;   // Map partailly contains the object in the X axis
    else 
        wRes |= X_NOT_IN_MAP;      // Map doesn't contains the object in the X axis

    if (ObjRect.top >= 0 && ObjRect.bottom < MAP_HEIGHT) 
        wRes |= Y_FULL_IN_MAP;      // Map fully contains the object in the Y axis
    else if (TmpRect.IntersectRect (ObjRect, MapRect) != 0)
        wRes |= Y_PARTIAL_IN_MAP;   // Map partailly contains the object in the Y axis
    else 
        wRes |= Y_NOT_IN_MAP;      // Map doesn't contains the object in the Y axis

    return wRes;
}

/*------------------------------------------------------------------------------

  Function: CollidesWith

  Purpose:  Check if our object collides with the given object.

  Input:    pOtherObj - the pointer to the object to be checked against.

  Output:   TRUE if objects collide.

  Remarks:  Some objects contains bitmasks for more accurate collision detection
            (by pixel resolution), but more time consuming. For objects w/o 
            bitmasks the object rectangles are checked for intersection.
------------------------------------------------------------------------------*/
BOOL CGameObject::CollidesWith (CGameObject *pOtherObj)
{
    CRect IntersecRect = pOtherObj->CGameObject::GetUpdateRectangle();

    IntersecRect &= CGameObject::GetUpdateRectangle();

    if (IntersecRect.IsRectEmpty()) 
    {   // no further calculations needed - rects don't intersect:
        return FALSE;
    }
    // check if bit masks of objects collides: (check is ignored if both bit masks are null)
    CBitMask *myBitMask = m_GlobalImageManager.ExposeBitMask(GetImage());
    CBitMask *otherBitMask = m_GlobalImageManager.ExposeBitMask(pOtherObj->GetImage());
    if (! myBitMask && ! otherBitMask) {
        return TRUE;
    }
    return BitMaskCollides (IntersecRect,
                            m_Pos, 
                            pOtherObj->GetPos(), 
                            myBitMask, 
                            otherBitMask);
}

/*------------------------------------------------------------------------------

  Function: CollidesWith

  Purpose:  Check if our object collides with the given object.

  Input:    pOtherObj - the pointer to the object to be checked against.
            MyRect - a specified rectangle on the game board object to be 
            checked against.

  Output:   TRUE if objects collide.

  Remarks:  This method is called from game board only. 
            Thus we know there is no bit mask for the other object and that 
            UpdateRect is no good for us.
------------------------------------------------------------------------------*/
BOOL CGameObject::CollidesWith (CGameObject* pOtherObj, CRect& MyRect)
{
    CRect IntersecRect = pOtherObj->CGameObject::GetUpdateRectangle();

    IntersecRect &= MyRect;

    if (IntersecRect.IsRectEmpty()) 
    {   // no further calculations needed - rects don't intersect:
        return FALSE;
    }
    // check if there is a bit mask:
    if (m_GlobalImageManager.ExposeBitMask(pOtherObj->GetImage())) {
        return BitMaskCollides (IntersecRect,
                                MyRect.TopLeft(),
                                pOtherObj->GetPos(),
                                NULL,
                                m_GlobalImageManager.ExposeBitMask(pOtherObj->GetImage()));
    }
    return TRUE;
}

/*------------------------------------------------------------------------------

  Function: BitMaskCollides

  Purpose:  Test if the given bitmasks collides inside the given rectangle.

  Input:    IntersectRect - The intersection rectangle of the objects that 
                            overlaps.
            myPos - The position of one object on the map
            otherPos - the position of the other object on the map
            myBitMask - the bitmask of one object (may be null)
            otherBitMask - the bitmask of the other object (may be null)

  Output:   TRUE if the two bitmasks collides inside the region of the 
            intersection rectangle.

  Remarks:  At least on of the bitmask must exist (otherwise this code wasn't
            inuse).
------------------------------------------------------------------------------*/
BOOL 
CGameObject::BitMaskCollides (CRect& IntersectRect, CPoint& myPos, CPoint& otherPos,
                              CBitMask* myBitMask, CBitMask* otherBitMask)
{
    CPoint myOffset = IntersectRect.TopLeft() - myPos;
    CPoint otherOffset = IntersectRect.TopLeft() - otherPos;

    ASSERT(myBitMask || otherBitMask);  // at this stage at least one bit mask exists

    if (myBitMask && otherBitMask) 
    {    // both object have bitmasks:
        for (int i = 0; i < IntersectRect.Width(); i++) {
            for (int j = 0; j < IntersectRect.Height(); j++) {
                if (myBitMask->GetMaskAt(i + myOffset.x, j + myOffset.y) &&
                    otherBitMask->GetMaskAt(i + otherOffset.x, j + otherOffset.y))
                    return TRUE;
            }
        }
        return FALSE;
    }
    // Only one object contains a bit mask, so we assume the missing bitmask is
    // all ones:
    CBitMask *BitMask;
    CPoint Offset;
    if (myBitMask) {
        BitMask = myBitMask;
        Offset = myOffset;
    } else {
        BitMask = otherBitMask;
        Offset = otherOffset;
    }
    for (int i = Offset.x; i < Offset.x + IntersectRect.Width(); i++) {
        for (int j = Offset.y; j < Offset.y + IntersectRect.Height(); j++) {
            if (BitMask->GetMaskAt(i, j))
                return TRUE;
        }
    }
    return FALSE;
}


/* ------------------------- Moving object ------------------------- */

/*------------------------------------------------------------------------------

  Function: CalcNewPos

  Purpose:  Calculates the moving object current position depending on its speed, 
            the time passed since we last updated its position, and a flag 
            indicating that object must be fully inside map.

  Input:    dwCurTime - Current game time
            bMustBeFullyInMap - flag forcing object to be fully in map

  Output:   Returns the distance passed (squared) or -1 if out of map.

  Remarks:  
------------------------------------------------------------------------------*/
int CMovingGameObject::CalcNewPos (DWORD dwCurTime, BOOL bMustBeFullyInMap)
{
    if (m_uLastTick == 0)   // Called for the first time
        m_uLastTick = dwCurTime;

    DWORD dwTimeGap = dwCurTime - m_uLastTick;
    m_uLastTick = dwCurTime;
    double dDistanceRatio = m_dVelocity * double (dwTimeGap) * m_cdThousandth,
           dXDist = dDistanceRatio * m_dDirectionsArr[m_uDirectionIndex].dXDir,
           dYDist = dDistanceRatio * m_dDirectionsArr[m_uDirectionIndex].dYDir;

    PrecisePosType NewPrecPos;

    NewPrecPos.dXPos = m_PrecPos.dXPos + dXDist;
    NewPrecPos.dYPos = m_PrecPos.dYPos + dYDist;

    CPoint NewIntPos (int (NewPrecPos.dXPos+0.5), int (NewPrecPos.dYPos+0.5));
    if (bMustBeFullyInMap)
    {   // Check if object is fully in map
        if (GetMapPosition(NewIntPos) != (X_FULL_IN_MAP | Y_FULL_IN_MAP)) 
        {   // Out of map situation
            return -1;
        }
    }
    else 
    { // check if object is partially in map
        WORD fl = GetMapPosition(NewIntPos);
        if ((X_NOT_IN_MAP & fl) || (Y_NOT_IN_MAP & fl)) 
        {   // Out of map situation
            return -1;
        }
    }
    // Now we know the move is legal:
    m_PrevPrecPos = m_PrecPos;
    m_PrecPos = NewPrecPos;
    m_Pos = NewIntPos;
    return int((dXDist * dXDist) + (dYDist * dYDist));
}

/*------------------------------------------------------------------------------

  Function: CalcPosAtTime

  Purpose:  Calculates the moving object new position depending on its speed, 
            the time passed its previous position and the prev. pos. and a 
            flag indicating that object must be fully inside map.

  Input:    StartPos - the previous position
            dwTimeGap - the time period the object was moving since the 
                        previous position.
            ResPos - the new position [OUT]
            bMustBeFullyInMap - flag forcing object to be fully in map

  Output:   Returns TRUE if new position is contained in map.

  Remarks:  
------------------------------------------------------------------------------*/
BOOL                        
CMovingGameObject::CalcPosAtTime (CPoint &StartPos,
                                  DWORD dwTimeGap,
                                  CPoint &ResPos,
                                  BOOL bMustBeFullyInMap)
{
    double dDistanceRatio = m_dVelocity * double (dwTimeGap) * m_cdThousandth,
           dXDist = dDistanceRatio * m_dDirectionsArr[m_uDirectionIndex].dXDir,
           dYDist = dDistanceRatio * m_dDirectionsArr[m_uDirectionIndex].dYDir;
    ResPos.x = int (double(StartPos.x) + dXDist + 0.5);
    ResPos.y = int (double(StartPos.y) + dYDist + 0.5);
    if (bMustBeFullyInMap)
    {   // Check if object is fully in map
        if (GetMapPosition(ResPos) != (X_FULL_IN_MAP | Y_FULL_IN_MAP)) 
        {   // Out of map situation
            return FALSE;
        }
    }
    else 
    { // check if object is partially in map
        WORD fl = GetMapPosition(ResPos);
        if ((X_NOT_IN_MAP & fl) || (Y_NOT_IN_MAP & fl)) 
        {   // Out of map situation
            return FALSE;
        }
    }
    return TRUE;
}

void                        
CMovingGameObject::SetNewPos (int x, int y)
{
    m_PrevPrecPos = m_PrecPos;
    SetPos (x, y);
    m_PrecPos.dXPos = x;
    m_PrecPos.dYPos = y;
}


const double CMovingGameObject::m_cdThousandth = double(0.001);

const DirectionType CMovingGameObject::m_dDirectionsArr[MAX_DIRECTIONS] = {
    { -1.0            , 0.0             },  // 0
    { -0.9659258262891, 0.2588190451025 },  // 15
    { -0.8660254037844, 0.5             },  // 30
    { -0.7071067811865, 0.7071067811865 },  // 45
    { -0.5            , 0.8660254037844 },  // 60
    { -0.2588190451025, 0.9659258262891 },  // 75
    {  0.0            , 1.0             },  // 90
    {  0.2588190451025, 0.9659258262891 },  // 105
    {  0.5            , 0.8660254037844 },  // 120
    {  0.7071067811865, 0.7071067811865 },  // 135
    {  0.8660254037844, 0.5             },  // 150
    {  0.9659258262891, 0.2588190451025 },  // 165
    {  1.0            , 0.0             },  // 180
    {  0.9659258262891,-0.2588190451025 },  // 195
    {  0.8660254037844,-0.5             },  // 210
    {  0.7071067811865,-0.7071067811865 },  // 225
    {  0.5            ,-0.8660254037844 },  // 240
    {  0.2588190451025,-0.9659258262891 },  // 255
    {  0.0            ,-1.0             },  // 270
    { -0.2588190451025,-0.9659258262891 },  // 285
    { -0.5            ,-0.8660254037844 },  // 300
    { -0.7071067811865,-0.7071067811865 },  // 315
    { -0.8660254037844,-0.5             },  // 330
    { -0.9659258262891,-0.2588190451025 }   // 345
};


/* ------------------------- Exploding object ------------------------- */


CExplodingGameObject::CExplodingGameObject 
     (UINT uXPos, UINT uYPos, UINT uXSize, UINT uYSize,
      UINT uMaxIntensity,
      CImageManager::ImageType uExplosionImageIndex,
      BOOL InitiallyExploding) :
    m_uMaxIntensity (uMaxIntensity),
    m_bIsExploding (InitiallyExploding)
{
    m_himgExplode = m_GlobalImageManager.GetImage (uExplosionImageIndex);
    m_pCurImage = &m_himgExplode;    // override this in your ctor
    SetPos (uXPos, uYPos);
    SetSize (uXSize, uYSize);
        
    ASSERT (uMaxIntensity > 0);
    for (int i=0; i<MAX_TANKS; i++)
        m_bNotifiedTanks[i]=FALSE;  // No tank was notified about an explosion
}

/*------------------------------------------------------------------------------

  Function: CalcRelativeExplosionIntensity

  Purpose:  Calculates the explosion intensity depending on the distance between
            our object and the given object, and the min and max radiuses of the
            explosition's impact.

  Input:    pOtherObject - pointer to the object we are exploding
            MinRadius - the minimal explosion impact radius: below this radius
                        the impact is maximal
            MaxRadius - the maximal explostion imapct radius: above this radius
                        there is no impact

  Output:   The explosion intensity

  Remarks:  If the distance between the objects is in the range {min, max}, the
            intensity is calculated relative to the distance.


------------------------------------------------------------------------------*/
UINT 
CExplodingGameObject::CalcRelativeExplosionIntensity (CGameObject *pOtherObject, 
                                                      UINT MinRadius, UINT MaxRadius)
{
    CPoint OtherPos  = pOtherObject->GetPos ();
    CSize  OtherSize = pOtherObject->GetDimensions();
    int iXDist = (m_Pos.x + m_Size.cx / 2) - (OtherPos.x + (OtherSize.cx) / 2),
        iYDist = (m_Pos.y + m_Size.cy / 2) - (OtherPos.y + (OtherSize.cy) / 2),
        iDist = int (sqrt (double(iXDist * iXDist) + double(iYDist * iYDist)) + 0.5);

    ASSERT (MaxRadius > MinRadius);

    if (iDist <= int(MinRadius))
        // We're below minimum radius - return full blast power
        return m_uMaxIntensity;

    if (iDist >= int(MaxRadius))
        // We're behind max radius - no impact
        return 0;

    // Otherwise , we're between min and max radiuses. Return relative impact

    return UINT (double (MaxRadius - iDist) * double (m_uMaxIntensity) /
                 double (MaxRadius - MinRadius)); 
}
