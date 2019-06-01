/*****************************************************************************
*                                                                             
*   SheildStatus.cpp
*                                                                             
*   Electrical Engineering Faculty - Software Lab                             
*   Spring semester 1998                                                      
*                                                                             
*   Tanks game                                                                
*                                                                             
*   Module description: Implements the shield status object that is displayed
*                       in the main dialog.
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
#include "ShieldStatus.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(CShieldStatus, CButton)
	//{{AFX_MSG_MAP(CShieldStatus)
	ON_WM_QUERYNEWPALETTE()
	ON_WM_PALETTECHANGED()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CShieldStatus::CShieldStatus():
        m_uPrevShieldLevel (0),
        m_uCurShieldLevel (100)
{
    m_DIB.CreateEmpty (SHIELD_STATUS_WIDTH, MAP_HEIGHT);
    m_DIB.GetPaletteFromResourceBitmap (IDB_BULLET);
}

void CShieldStatus::SetLevel (UINT uShieldLevel)
{
    ASSERT (uShieldLevel <= 100);
    m_uCurShieldLevel = uShieldLevel;
    if (m_uCurShieldLevel != m_uPrevShieldLevel)
        InvalidateRect (NULL, FALSE);
}


enum {  BLOCK_SIZE = 7,
        GAP_SIZE   = 2
     };

CDIB *CShieldStatus::GetImage ()
{
    if (m_uCurShieldLevel != m_uPrevShieldLevel)
    {   // Level has changed - recreate image
        int iNumBars = (MAP_HEIGHT + GAP_SIZE) / (BLOCK_SIZE + GAP_SIZE);
        int iColorStep = int(float(255) / float(iNumBars));
        iNumBars = int (float(iNumBars) * float (m_uCurShieldLevel) / float(100));

        int iCurColor = 0;

        m_DIB.FillSolidColor (0,0,0); // Clear to background
        for (int i=0; i < iNumBars; i++)
        {
            m_DIB.FillRect (0, 
                            i*(BLOCK_SIZE + GAP_SIZE), 
                            SHIELD_STATUS_WIDTH,
                            BLOCK_SIZE, 
                            255 - iCurColor, 0, iCurColor);
            iCurColor += iColorStep;
        }
        m_uPrevShieldLevel = m_uCurShieldLevel;
    }
    return &m_DIB;
}

