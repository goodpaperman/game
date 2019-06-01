/*****************************************************************************
*                                                                             
*   AmmoStatus.h
*                                                                             
*   Electrical Engineering Faculty - Software Lab                             
*   Spring semester 1998                                                      
*                                                                             
*   Tanks game                                                                
*                                                                             
*   Module description: Interface of the CAmmoStatus class.
*                       
*                                                                             
*   Authors: Eran Yariv - 28484475                                           
*            Moshe Zur  - 24070856                                           
*                                                                            
*                                                                            
*   Date: 23/09/98                                                           
*                                                                            
******************************************************************************/
#if !defined(AFX_AMMOSTATUS_H__B239E761_E897_11D1_9738_A0C11B38D128__INCLUDED_)
#define AFX_AMMOSTATUS_H__B239E761_E897_11D1_9738_A0C11B38D128__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// AmmoStatus.h : header file
//

#include <DIB.h>
#include <GameConsts.h>

/////////////////////////////////////////////////////////////////////////////
// CAmmoStatus window

class CAmmoStatus : public CButton
{
// Construction
public:
	CAmmoStatus();
    void Create ();
	virtual ~CAmmoStatus() {}
    void SetShellsCount (UINT);
    void SetBulletsCount (UINT);
    void SetMinesCount (UINT);
    
    CDIB *GetImage ();

private:

    void WriteNumber (UINT u, UINT x, UINT y);

    UINT m_uPrevShellsCount,
         m_uCurShellsCount,
         m_uPrevBulletsCount,
         m_uCurBulletsCount,
         m_uPrevMinesCount,
         m_uCurMinesCount,
         m_uShellIconYPos,
         m_uBulletIconYPos,
         m_uMineIconYPos;
    CDIB m_DIB,
         m_DIBDigits;
	// Generated message map functions
protected:
	//{{AFX_MSG(CAmmoStatus)
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

};


#endif // !defined(AFX_AMMOSTATUS_H__B239E761_E897_11D1_9738_A0C11B38D128__INCLUDED_)
