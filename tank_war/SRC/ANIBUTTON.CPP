/*****************************************************************************
*                                                                             
*   AniButton.cpp                                                            
*                                                                             
*   Electrical Engineering Faculty - Software Lab                             
*   Spring semester 1998                                                      
*                                                                             
*   Tanks game                                                                
*                                                                             
*   Module description: Implements the animated buttons used in the dialogs.
*                       
*                                                                             
*   Authors: Eran Yariv - 28484475                                           
*            Moshe Zur  - 24070856                                           
*                                                                            
*                                                                            
*   Date: 23/09/98                                                           
*                                                                            
******************************************************************************/
////////////////////////////////////////////////////////////////
// CAniButton Animated button
// (c) Oscar  oscarko@cyberus.ca
// 


#include "stdafx.h"
#include "AniButton.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAniButton

CAniButton::CAniButton()
{
    m_nAniID = 0;
    m_bPlaying = FALSE;
}

CAniButton::~CAniButton()
{
}


BEGIN_MESSAGE_MAP(CAniButton, CButton)
    //{{AFX_MSG_MAP(CAniButton)
    ON_WM_MOUSEMOVE()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CAniButton::Create( LPCTSTR lpszCaption, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID )
{
    BOOL m_bSucess = CButton::Create( lpszCaption,dwStyle,rect, pParentWnd, nID );
    return m_bSucess;
}

/////////////////////////////////////////////////////////////////////////////
// CAniButton message handlers

void CAniButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
    CRect rect;
    GetClientRect(rect);

    if (!::IsWindow(m_AnimateCtrl))
    {
       m_AnimateCtrl.Create(WS_CHILD |WS_VISIBLE,rect,this,0);
       m_AnimateCtrl.Open(m_nAniID);
       m_AnimateCtrl.GetClientRect(rect);

       VERIFY(SetWindowPos(NULL, -1, -1, rect.Width()+4, rect.Height()+4,
        SWP_NOMOVE|SWP_NOZORDER|SWP_NOREDRAW|SWP_NOACTIVATE));

       rect.OffsetRect(2,2);
       
       m_AnimateCtrl.MoveWindow(rect); 
       //m_AnimateCtrl.Play(0,-1,1);
    }

    CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
    UINT nState = lpDrawItemStruct->itemState;
    CRect buttonRect;
    GetClientRect(buttonRect);

    if (IsWindowEnabled())
        nState &= ~ODS_DISABLED;
    else
        nState |= ODS_DISABLED;

    DrawButton(pDC, nState, buttonRect);
}

void CAniButton::LoadAVI(UINT nAniID)
{
    m_nAniID =nAniID;
}

//Draw the button borders
void CAniButton::DrawButton(CDC* pDC, UINT nState, CRect rect)
{
    COLORREF upCol,downCol,edgeCol;
    BOOL bRevers = FALSE;

    upCol = downCol = edgeCol = RGB(0,0,0);

    if ((nState & ODS_SELECTED) == ODS_SELECTED)
    {
        //  Pressed 
        upCol=  RGB(0,60,255);
        downCol=RGB(0,120,255);
        edgeCol=RGB(0,60,0);
        bRevers = TRUE;

    }
    else if ((nState & ODS_FOCUS) == ODS_FOCUS)
    {
        // In focus
        upCol=  RGB(0,20,80);
        downCol=RGB(0,40,80);
        edgeCol=RGB(0,20,0);
    }
    else if ((nState & ODS_DISABLED) == ODS_DISABLED)
    {
        // Disabled -  in the future maybe
    }
    else
    {   // Normal
        upCol=  RGB(0,0,0);
        downCol=RGB (0,0,0);
        edgeCol=RGB(0,0,0);
    }

    CPen* pOldPen = NULL;

    BOOL pen1Created;
    CPen pen1;
    BOOL pen2Created;
    CPen pen2;

    pen1Created = pen1.CreatePen(PS_SOLID, 1, upCol);
    if (pen1Created)  
        pOldPen = pDC->SelectObject( &pen1 );

    pDC->MoveTo(1,rect.Height()-1);
    pDC->LineTo(1,1);                                                             
    pDC->LineTo(rect.Width()-1,1);
    pDC->MoveTo(0,rect.Height()-1);
    pDC->LineTo(0,0);                                                             
    pDC->LineTo(rect.Width()-1,0);

    pen2Created = pen2.CreatePen(PS_SOLID, 1, downCol);
    if (pen2Created)  
    {
        pDC->SelectObject( &pen2 );
    }
    
    if (pen1Created) pen1.DeleteObject();
        pen1Created = FALSE;

    pDC->MoveTo(rect.Width()-1,0);
    pDC->LineTo(rect.Width()-1,rect.Height()-1);
    pDC->LineTo(0,rect.Height()-1);
    pDC->MoveTo(rect.Width()-2,1);
    pDC->LineTo(rect.Width()-2,rect.Height()-2);
    pDC->LineTo(0,rect.Height()-2);

    if (pen2Created) pen2.DeleteObject();
        pen2Created = FALSE;

    pen1Created = pen1.CreatePen(PS_SOLID, 1, edgeCol);
    if (pen1Created) 
        pOldPen = pDC->SelectObject( &pen1 );

    if (bRevers)
    {
        pDC->MoveTo(1,rect.Height()-2);
        pDC->LineTo(1,1);                                                             
        pDC->LineTo(rect.Width()-2,1);
    }
    else
    {
        pDC->MoveTo(rect.Width()-1,0);
        pDC->LineTo(rect.Width()-1,rect.Height()-1);
        pDC->LineTo(-1,rect.Height()-1);
    }

    if (pen1Created) pen1.DeleteObject();
        pen1Created = FALSE;

    if (pOldPen != NULL) pDC->SelectObject( pOldPen );

}


void CAniButton::OnMouseMove(UINT nFlags, CPoint point) 
{

    ClientToScreen(&point);
    CRect rcWindow;
    GetWindowRect(rcWindow);
    BOOL bNewMouseOverButton = rcWindow.PtInRect(point);

    unsigned long nROnly = ES_READONLY;

    BOOL bTest = (GetStyle() &  nROnly) != nROnly;

    if (bNewMouseOverButton && IsWindowEnabled() && bTest)
    {
        if (::IsWindow(m_AnimateCtrl) && !m_bPlaying)
        {
            m_AnimateCtrl.Play(0,UINT(-1),1);
            m_bPlaying = TRUE;
            SetCapture();
        }
    }
    else
    {
        m_bPlaying = FALSE;
        ReleaseCapture();
    }
    CButton::OnMouseMove(nFlags, point);
}
