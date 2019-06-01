/*****************************************************************************
*                                                                             
*   BonusStatus.cpp
*                                                                             
*   Electrical Engineering Faculty - Software Lab                             
*   Spring semester 1998                                                      
*                                                                             
*   Tanks game                                                                
*                                                                             
*   Module description: Implements the bonus status object displayed on the 
*                       main dialog.
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
#include "Tanks.h"
#include "BonusStatus.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(CBonusStatus, CButton)
    //{{AFX_MSG_MAP(CBonusStatus)
    ON_WM_QUERYNEWPALETTE()
    ON_WM_PALETTECHANGED()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

CBonusStatus::CBonusStatus():
         m_bFastFire (FALSE),
         m_bAerial (FALSE),
         m_bChangeDisplay (TRUE)
{
    m_DIB.CreateEmpty (AMMO_STATUS_WIDTH, MAP_HEIGHT - AMMO_STATUS_HEIGHT);
    m_DIB.GetPaletteFromResourceBitmap (IDB_BULLET);
}

void CBonusStatus::Create ()
{
    m_DIBBomb.ReadFromResource (IDB_BOMB_ICON);
    m_DIBFastFire.ReadFromResource (IDB_FASTFIRE);
    m_uFastFireXPos = (AMMO_STATUS_WIDTH / 2) - (m_DIBFastFire.Width() / 2);
    m_uFastFireYPos = MAP_HEIGHT - AMMO_STATUS_HEIGHT - m_DIBFastFire.Height();
    m_uBombXPos = (AMMO_STATUS_WIDTH / 2) - (m_DIBBomb.Width() / 2);
    m_uBombYPos = m_uFastFireYPos - 10 - m_DIBBomb.Height();
}


void CBonusStatus::SetAerialSupport (BOOL bOn)
{
    m_bAerial = bOn;
    m_bChangeDisplay = TRUE;
    InvalidateRect (NULL, FALSE);
}

void CBonusStatus::SetFastFireRate (BOOL bOn)
{
    m_bFastFire = bOn;
    m_bChangeDisplay = TRUE;
    InvalidateRect (NULL, FALSE);
}

CDIB *CBonusStatus::GetImage ()
{
    if (m_bChangeDisplay)
    {   // Something has changed - recreate image
        m_DIB.FillSolidColor (0,0,0);
        if (m_bFastFire)
            m_DIB.CopyRectFrom (&m_DIBFastFire, 
                                0, 
                                0, 
                                m_DIBFastFire.Width(), 
                                m_DIBFastFire.Height(), 
                                m_uFastFireXPos, 
                                m_uFastFireYPos);
        if (m_bAerial)
            m_DIB.CopyRectFrom (&m_DIBBomb, 
                                0, 
                                0, 
                                m_DIBBomb.Width(), 
                                m_DIBBomb.Height(), 
                                m_uBombXPos, 
                                m_uBombYPos);
        m_bChangeDisplay = FALSE;
    }
    return &m_DIB;
}
