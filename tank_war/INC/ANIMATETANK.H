/*****************************************************************************
*                                                                             
*   AnimateTank.h
*                                                                             
*   Electrical Engineering Faculty - Software Lab                             
*   Spring semester 1998                                                      
*                                                                             
*   Tanks game                                                                
*                                                                             
*   Module description: Interface of the AnimateTank class.
*                       
*                                                                             
*   Authors: Eran Yariv - 28484475                                           
*            Moshe Zur  - 24070856                                           
*                                                                            
*                                                                            
*   Date: 23/09/98                                                           
*                                                                            
******************************************************************************/
#if !defined(AFX_ANIMATETANK_H__E52AF544_E83A_11D1_9738_B56BC683A70E__INCLUDED_)
#define AFX_ANIMATETANK_H__E52AF544_E83A_11D1_9738_B56BC683A70E__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// AnimateTank.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAnimateTank window

class CAnimateTank : public CAnimateCtrl
{
// Construction
public:
    CAnimateTank();
    virtual ~CAnimateTank() {}

    BOOL IsSelected ()  { return m_bSelected; }
    void Select ();     
    void Unselect ();

    BOOL Init(int index, CWnd *);

private:
    BOOL m_bSelected;
};


#endif // !defined(AFX_ANIMATETANK_H__E52AF544_E83A_11D1_9738_B56BC683A70E__INCLUDED_)
