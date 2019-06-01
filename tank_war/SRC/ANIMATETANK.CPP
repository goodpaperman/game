/*****************************************************************************
*                                                                             
*   AnimateTank.cpp
*                                                                             
*   Electrical Engineering Faculty - Software Lab                             
*   Spring semester 1998                                                      
*                                                                             
*   Tanks game                                                                
*                                                                             
*   Module description: Implements the animated tanks shown in the game setting
*                       dialog.
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
#include "AnimateTank.h"
#include <GameConsts.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CAnimateTank::CAnimateTank() :
        m_bSelected (FALSE),
        CAnimateCtrl()
{}

void CAnimateTank::Select ()
{
    m_bSelected = TRUE;
    Play (0,UINT(-1),UINT(-1));
}

void CAnimateTank::Unselect ()
{
    m_bSelected = FALSE;
    Stop();
    Seek(0);
}

BOOL CAnimateTank::Init(int index, CWnd *pParent) 
{
    SubclassDlgItem (IDC_ANIMATE_TANK1 + index - 1, pParent);

    if (!Open (IDR_AVI_TANK0 + index - 1))
        return FALSE;
    SetWindowPos (&wndTop, 10 + (index-1) * (10+TANK_ANIM_WIDTH), 10, 
          TANK_ANIM_WIDTH, TANK_ANIM_HEIGHT, 0);
    return TRUE;
}
