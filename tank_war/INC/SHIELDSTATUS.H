/*****************************************************************************
*                                                                             
*   ShieldStatus.h
*                                                                             
*   Electrical Engineering Faculty - Software Lab                             
*   Spring semester 1998                                                      
*                                                                             
*   Tanks game                                                                
*                                                                             
*   Module description: Interface of the CShieldStatus class.
*                       
*                                                                             
*   Authors: Eran Yariv - 28484475                                           
*            Moshe Zur  - 24070856                                           
*                                                                            
*                                                                            
*   Date: 23/09/98                                                           
*                                                                            
******************************************************************************/
#if !defined(AFX_SHIELDSTATUS_H__B239E761_E897_11D1_9738_A0C11B38D128__INCLUDED_)
#define AFX_SHIELDSTATUS_H__B239E761_E897_11D1_9738_A0C11B38D128__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// ShieldStatus.h : header file
//

#include <DIB.h>
#include <GameConsts.h>

/////////////////////////////////////////////////////////////////////////////
// CShieldStatus window

class CShieldStatus : public CButton
{
// Construction
public:
	CShieldStatus();
	virtual ~CShieldStatus() {}
    void SetLevel (UINT uShieldLevel);
    CDIB *GetImage ();
private:
    UINT m_uPrevShieldLevel,        // 0%..100%
         m_uCurShieldLevel;
    CDIB m_DIB;

protected:
	//{{AFX_MSG(CAmmoStatus)
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};


#endif // !defined(AFX_SHIELDSTATUS_H__B239E761_E897_11D1_9738_A0C11B38D128__INCLUDED_)
