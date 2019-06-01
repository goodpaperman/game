/*****************************************************************************
*                                                                             
*   AmmoStatus.cpp
*                                                                             
*   Electrical Engineering Faculty - Software Lab                             
*   Spring semester 1998                                                      
*                                                                             
*   Tanks game                                                                
*                                                                             
*   Module description: Implements the ammunition status object displayed in 
*                       the main dialog.
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
#include "AmmoStatus.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(CAmmoStatus, CButton)
	//{{AFX_MSG_MAP(CAmmoStatus)
	ON_WM_QUERYNEWPALETTE()
	ON_WM_PALETTECHANGED()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CAmmoStatus::CAmmoStatus():
         m_uPrevShellsCount (0xffff),
         m_uCurShellsCount (0),
         m_uPrevBulletsCount (0xffff),
         m_uCurBulletsCount (0),
         m_uPrevMinesCount (0xffff),
         m_uCurMinesCount (0)
{
    m_DIB.CreateEmpty (AMMO_STATUS_WIDTH, AMMO_STATUS_HEIGHT);
    m_DIB.GetPaletteFromResourceBitmap (IDB_BULLET);
}

void CAmmoStatus::Create ()
{
    CDIB DIBMineIcon,
         DIBBulletIcon,
         DIBShellIcon;

    int iTextWidth = DIGIT_HEIGHT * 3;

    m_DIB.FillSolidColor (0,0,0);
    DIBMineIcon.ReadFromResource (IDB_MINE_ICON);
    DIBBulletIcon.ReadFromResource (IDB_BULLET_ICON);
    DIBShellIcon.ReadFromResource (IDB_SHELL_ICON);
    m_DIBDigits.ReadFromResource (IDB_DIGITS);

    m_uShellIconYPos = AMMO_STATUS_HEIGHT - DIBShellIcon.Height();
    m_uBulletIconYPos = m_uShellIconYPos - (DIBBulletIcon.Height() + iTextWidth);
    m_uMineIconYPos = m_uBulletIconYPos - (DIBMineIcon.Height() + iTextWidth);

    m_DIB.CopyRectFrom (&DIBShellIcon, 
                        0, 
                        0,
                        DIBShellIcon.Width(),
                        DIBShellIcon.Height(), 
                        (AMMO_STATUS_WIDTH / 2) - (DIBShellIcon.Width() / 2),
                        m_uShellIconYPos);

    m_DIB.CopyRectFrom (&DIBBulletIcon, 
                        0, 
                        0,
                        DIBBulletIcon.Width(),
                        DIBBulletIcon.Height(), 
                        (AMMO_STATUS_WIDTH / 2) - (DIBBulletIcon.Width() / 2),
                        m_uBulletIconYPos);

    m_DIB.CopyRectFrom (&DIBMineIcon, 
                        0, 
                        0,
                        DIBMineIcon.Width(),
                        DIBMineIcon.Height(), 
                        (AMMO_STATUS_WIDTH / 2) - (DIBMineIcon.Width() / 2),
                        m_uMineIconYPos);
}

void CAmmoStatus::WriteNumber (UINT u, UINT x, UINT y)
{
    ASSERT (u < 1000);  // Max of 3 digits on display
    char sz[4];
    wsprintf (sz,"%3d",u);
    for (int i=0; i<3; i++)
    { 
        m_DIB.CopyRectFrom (&m_DIBDigits,
                            (sz[i] == ' ') ? (DIGIT_WIDTH * 10) : (DIGIT_WIDTH*(sz[i] - '0')),
                            0,
                            DIGIT_WIDTH,
                            DIGIT_HEIGHT,
                            x,y);
        x += (DIGIT_WIDTH + 2);
    }
}

void CAmmoStatus::SetShellsCount (UINT uCount)
{
    m_uCurShellsCount = uCount;
    if (m_uCurShellsCount != m_uPrevShellsCount)
        InvalidateRect (NULL, FALSE);
}

void CAmmoStatus::SetBulletsCount (UINT uCount)
{
    m_uCurBulletsCount = uCount;
    if (m_uCurBulletsCount != m_uPrevBulletsCount)
        InvalidateRect (NULL, FALSE);
}

void CAmmoStatus::SetMinesCount (UINT uCount)
{
    m_uCurMinesCount = uCount;
    if (m_uCurMinesCount != m_uPrevMinesCount)
        InvalidateRect (NULL, FALSE);
}


enum {  BLOCK_SIZE = 7,
        GAP_SIZE   = 2
     };

CDIB *CAmmoStatus::GetImage ()
{
    if (m_uCurShellsCount != m_uPrevShellsCount ||
        m_uCurBulletsCount != m_uPrevBulletsCount ||
        m_uCurMinesCount != m_uPrevMinesCount)
    {   // Something has changed - recreate image
        const int iNumberXPos = ((AMMO_STATUS_WIDTH / 2) - ((DIGIT_WIDTH * 3 + 4) / 2));

        WriteNumber (m_uCurShellsCount, iNumberXPos, m_uShellIconYPos - DIGIT_HEIGHT - 4);
        WriteNumber (m_uCurBulletsCount, iNumberXPos, m_uBulletIconYPos - DIGIT_HEIGHT - 4);
        WriteNumber (m_uCurMinesCount, iNumberXPos, m_uMineIconYPos - DIGIT_HEIGHT - 4);
        m_uPrevShellsCount = m_uCurShellsCount;
        m_uPrevBulletsCount = m_uCurBulletsCount;
        m_uPrevMinesCount = m_uCurMinesCount;
    }
    return &m_DIB;
}
