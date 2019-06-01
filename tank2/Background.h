#if !defined(AFX_BACKGROUND_H__38650C5E_5F3D_4616_913F_8DA1A2BDE860__INCLUDED_)
#define AFX_BACKGROUND_H__38650C5E_5F3D_4616_913F_8DA1A2BDE860__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Background.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBackground window

#define WIDTHBYTES(bits) ((((bits)+31)/32)*4)
//256标准色
static RGBQUAD rgbStd256[256] =                 
{ 
	{0, 0, 0, 0}, {0, 0, 128, 0}, {0, 128, 0, 0}, {0, 128, 128, 0}, 
	{128, 0, 0, 0}, {128, 0, 128, 0}, {128, 128, 0, 0}, {192, 192, 192, 0}, 
	{192, 220, 192, 0}, {240, 202, 166, 0}, {238, 238, 238, 0}, {221, 221, 221, 0}, 
	{204, 204, 204, 0}, {187, 187, 187, 0}, {170, 170, 170, 0}, {153, 153, 153, 0}, 
	
	{136, 136, 136, 0}, {119, 119, 119, 0}, {102, 102, 102, 0}, {85, 85, 85, 0}, 
	{68, 68, 68, 0}, {51, 51, 51, 0}, {34, 34, 34, 0}, {17, 17, 17, 0}, 
	{204, 255, 255, 0}, {153, 255, 255, 0}, {102, 255, 255, 0}, {51, 255, 255, 0}, 
	{255, 204, 255, 0}, {204, 204, 255, 0}, {153, 204, 255, 0}, {102, 204, 255, 0}, 
	
	{51, 204, 255, 0}, {0, 204, 255, 0}, {255, 153, 255, 0}, {204, 153, 255, 0}, 
	{153, 153, 255, 0}, {102, 153, 255, 0}, {51, 153, 255, 0}, {0, 153, 255, 0}, 
	{255, 102, 255, 0}, {204, 102, 255, 0}, {153, 102, 255, 0}, {102, 102, 255, 0}, 
	{51, 102, 255, 0}, {0, 102, 255, 0}, {255, 51, 255, 0}, {204, 51, 255, 0}, 
	
	{153, 51, 255, 0}, {102, 51, 255, 0}, {51, 51, 255, 0}, {0, 51, 255, 0}, 
	{204, 0, 255, 0}, {153, 0, 255, 0}, {102, 0, 255, 0}, {51, 0, 255, 0}, 
	{255, 255, 204, 0}, {204, 255, 204, 0}, {153, 255, 204, 0}, {102, 255, 204, 0}, 
	{51, 255, 204, 0}, {0, 255, 204, 0}, {255, 204, 204, 0}, {153, 204, 204, 0}, 
//16	
	{102, 204, 204, 0}, {51, 204, 204, 0}, {0, 204, 204, 0}, {255, 153, 204, 0}, 
	{204, 153, 204, 0}, {153, 153, 204, 0}, {102, 153, 204, 0}, {51, 153, 204, 0}, 
	{0, 153, 204, 0}, {255, 102, 204, 0}, {204, 102, 204, 0}, {153, 102, 204, 0},
	{102, 102, 204, 0}, {51, 102, 204, 0}, {0, 102, 204, 0}, {255, 51, 204, 0},  
	
	{204, 51, 204, 0}, {153, 51, 204, 0}, {102, 51, 204, 0}, {51, 51, 204, 0}, 
	{0, 51, 204, 0}, {255, 0, 204, 0}, {204, 0, 204, 0}, {153, 0, 204, 0}, 
	{102, 0, 204, 0}, {51, 0, 204, 0}, {255, 255, 153, 0}, {204, 255, 153, 0}, 
	{153, 255, 153, 0}, {102, 255, 153, 0}, {51, 255, 153, 0}, {0, 255, 153, 0}, 
	
	{255, 204, 153, 0}, {204, 204, 153, 0}, {153, 204, 153, 0}, {102, 204, 153, 0},
	{51, 204, 153, 0}, {0, 204, 153, 0}, {255, 153, 153, 0}, {204, 153, 153, 0}, 
	{102, 153, 153, 0}, {51, 153, 153, 0}, {0, 153, 153, 0}, {255, 102, 153, 0}, 
	{204, 102, 153, 0}, {153, 102, 153, 0}, {102, 102, 153, 0}, {51, 102, 153, 0},  
	
	{0, 102, 153, 0}, {255, 51, 153, 0}, {204, 51, 153, 0}, {153, 51, 153, 0}, 
	{102, 51, 153, 0}, {51, 51, 153, 0}, {0, 51, 153, 0}, {255, 0, 153, 0}, 
	{204, 0, 153, 0}, {153, 0, 153, 0}, {102, 0, 153, 0}, {51, 0, 153, 0}, 
	{255, 255, 102, 0}, {204, 255, 102, 0}, {153, 255, 102, 0}, {102, 255, 102, 0}, 
//32
	{51, 255, 102, 0}, {0, 255, 102, 0}, {255, 204, 102, 0}, {204, 204, 102, 0}, 
	{153, 204, 102, 0}, {102, 204, 102, 0}, {51, 204, 102, 0}, {0, 204, 102, 0}, 
	{255, 153, 102, 0}, {204, 153, 102, 0}, {153, 153, 102, 0}, {102, 153, 102, 0}, 
	{51, 153, 102, 0}, {0, 153, 102, 0}, {255, 102, 102, 0}, {204, 102, 102, 0}, 
	
	{153, 102, 102, 0}, {51, 102, 102, 0}, {0, 102, 102, 0}, {255, 51, 102, 0}, 
	{204, 51, 102, 0}, {153, 51, 102, 0}, {102, 51, 102, 0}, {51, 51, 102, 0}, 
	{0, 51, 102, 0}, {255, 0, 102, 0}, {204, 0, 102, 0}, {153, 0, 102, 0}, 
	{102, 0, 102, 0}, {51, 0, 102, 0}, {255, 255, 51, 0}, {204, 255, 51, 0}, 
	
	{153, 255, 51, 0}, {102, 255, 51, 0}, {51, 255, 51, 0}, {0, 255, 51, 0}, 
	{255, 204, 51, 0}, {204, 204, 51, 0}, {153, 204, 51, 0}, {102, 204, 51, 0}, 
	{51, 204, 51, 0}, {0, 204, 51, 0}, {255, 153, 51, 0}, {204, 153, 51, 0}, 
	{153, 153, 51, 0}, {102, 153, 51, 0}, {51, 153, 51, 0}, {0, 153, 51, 0}, 
	
	{255, 102, 51, 0}, {204, 102, 51, 0}, {153, 102, 51, 0}, {102, 102, 51, 0}, 
	{51, 102, 51, 0}, {0, 102, 51, 0}, {255, 51, 51, 0}, {204, 51, 51, 0}, 
	{153, 51, 51, 0}, {102, 51, 51, 0}, {0, 51, 51, 0}, {255, 0, 51, 0}, 
	{204, 0, 51, 0}, {153, 0, 51, 0}, {102, 0, 51, 0}, {51, 0, 51, 0}, 
//48
	{204, 255, 0, 0}, {153, 255, 0, 0}, {102, 255, 0, 0}, {51, 255, 0, 0}, 
	{255, 204, 0, 0}, {204, 204, 0, 0}, {153, 204, 0, 0}, {102, 204, 0, 0}, 
	{51, 204, 0, 0}, {255, 153, 0, 0}, {204, 153, 0, 0}, {153, 153, 0, 0}, 
	{102, 153, 0, 0}, {0, 0, 238, 0}, {0, 0, 221, 0}, {0, 0, 204, 0}, 
	
	{0, 0, 187, 0}, {0, 0, 170, 0}, {0, 0, 153, 0}, {0, 0, 136, 0}, 
	{0, 0, 119, 0}, {0, 0, 102, 0}, {0, 0, 85, 0}, {0, 0, 68, 0}, 
	{0, 0, 51, 0}, {0, 0, 34, 0}, {0, 0, 17, 0}, {0, 238, 0, 0}, 
	{0, 221, 0, 0}, {0, 204, 0, 0}, {0, 187, 0, 0}, {0, 170, 0, 0}, 
	
	{0, 153, 0, 0}, {0, 136, 0, 0}, {0, 119, 0, 0}, {0, 102, 0, 0}, 
	{0, 85, 0, 0}, {0, 68, 0, 0}, {0, 51, 0, 0}, {0, 34, 0, 0}, 
	{0, 17, 0, 0}, {238, 0, 0, 0}, {221, 0, 0, 0}, {204, 0, 0, 0}, 
	{187, 0, 0, 0}, {170, 0, 0, 0}, {153, 0, 0, 0}, {136, 0, 0, 0}, 
	
	{119, 0, 0, 0}, {102, 0, 0, 0}, {85, 0, 0, 0}, {68, 0, 0, 0}, 
	{51, 0, 0, 0}, {34, 0, 0, 0}, {240, 251, 255, 0}, {164, 160, 160, 0}, 
	{128, 128, 128, 0}, {0, 0, 255, 0}, {0, 255, 0, 0}, {0, 255, 255, 0},
	{255, 0, 0, 0}, {255, 0, 255, 0}, {255, 255, 0, 0}, {255, 255, 255, 0}
//64
} ; 


#include <afxtempl.h>
#include "TankObj.h"
#include "ShellObj.h"

class CTankObj ; 
class CShellObj ; 

typedef CArray<CTankObj*, CTankObj*> TankQueue ;		//所有坦克
typedef CList<CShellObj*, CShellObj*> ShellList ;		//所有炮弹

#define TIMER_SLOW		1		//移动定时:慢300ms
#define TIMER_NORMAL	2		//移动定时:正常200ms
#define TIMER_QUICK		3		//移动定时:快150ms
#define TIMER_FAST		4		//移动定时:急速100ms
#define TIMER_EXPLODE	5		//爆炸定时:40ms

#define TIME_SLOW		300		//移动定时:慢300ms
#define TIME_NORMAL		200		//移动定时:正常200ms
#define TIME_QUICK		150		//移动定时:快150ms
#define TIME_FAST		100		//移动定时:急速100ms
#define TIME_EXPLODE	40		//爆炸定时:40ms


class CBackground : public CStatic
{
// Construction
public:
	CBackground();

// Attributes
public:

	//显示范围
	static int m_width ; 
	static int m_height ; 

	//位图(用于显示)
	CPalette m_Palette ;
	CBitmap m_Bitmap ; 
	BYTE *m_pBits ; 
	LPLOGPALETTE lpLogPal ; 
	LPBITMAPINFO lpbi ; 

	//物体
	static TankQueue m_tanks ; 
	static ShellList m_shells ; 
	TankQueue m_tanksSel ; 

	//mouse 
	BOOL lbDown ; 
	BOOL msMove ; 
	CRect m_focus ; 
	UINT nCreate ; //0-create friends , 1-create enemy 
	HCURSOR m_curCross ; 


// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBackground)
	//}}AFX_VIRTUAL

// Implementation
public:
	void AddTank();
	void Restart();
	void Pause( BOOL bPause);
	void MakeWay( CTankObj *from, CTankObj *to);
	void RemoveSelection();
	void OnRButtonDown( UINT uFlags, CPoint point );
	void OnMouseMove( UINT uFlags, CPoint point );
	void OnLButtonUp( UINT uFlags, CPoint point );
	void LostLife( CShellObj * pShell );
	BOOL AvoidCrush( UINT i, UINT j );
	static CPointf ScatterDestination( CPointf start, int num, int id);
	void Battle();
	CPointf CorrectCoordinate(CPointf &point);
	void SetSize();
	void OnLButtonDown(UINT uFlags, CPoint point );
	void InitObject();
	void DrawWithoutFlicker(CDC *pDC);
	CSize GetSize();
	void InitBackground();
	virtual ~CBackground();

	// Generated message map functions
protected:
	//{{AFX_MSG(CBackground)
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BACKGROUND_H__38650C5E_5F3D_4616_913F_8DA1A2BDE860__INCLUDED_)
