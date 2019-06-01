/*****************************************************************************
*                                                                             
*   BonusStatus.h
*                                                                             
*   Electrical Engineering Faculty - Software Lab                             
*   Spring semester 1998                                                      
*                                                                             
*   Tanks game                                                                
*                                                                             
*   Module description: Interface of the CBonusStatus class.
*                       
*                                                                             
*   Authors: Eran Yariv - 28484475                                           
*            Moshe Zur  - 24070856                                           
*                                                                            
*                                                                            
*   Date: 23/09/98                                                           
*                                                                            
******************************************************************************/
#if !defined(AFX_BONUSSTATUS_H__B239E761_E897_11D1_9738_A0C11B38D128__INCLUDED_)
#define AFX_BONUSSTATUS_H__B239E761_E897_11D1_9738_A0C11B38D128__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// BonusStatus.h : header file
//

#include <DIB.h>
#include <GameConsts.h>

/////////////////////////////////////////////////////////////////////////////
// CBonusStatus window

class CBonusStatus : public CButton
{
// Construction
public:
    CBonusStatus();
    void Create ();
    virtual ~CBonusStatus() {}
    void SetAerialSupport (BOOL bOn);
    void SetFastFireRate (BOOL bOn);
    
    CDIB *GetImage ();

private:

    BOOL m_bAerial,
         m_bFastFire,
         m_bChangeDisplay;
    CDIB m_DIB,
         m_DIBBomb,
         m_DIBFastFire;

    UINT m_uFastFireXPos,
         m_uFastFireYPos,
         m_uBombXPos,
         m_uBombYPos;

protected:
    //{{AFX_MSG(CAmmoStatus)
    //}}AFX_MSG

    DECLARE_MESSAGE_MAP()

};


#endif // !defined(AFX_BONUSSTATUS_H__B239E761_E897_11D1_9738_A0C11B38D128__INCLUDED_)
