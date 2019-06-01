// ShellObj.cpp: implementation of the CShellObj class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "tank.h"
#include "ShellObj.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CShellObj::CShellObj() : m_position(CPointf(0, 0)), m_radius(3), m_routine(CLine()), 
						 m_tankID(0), m_state(run), /*m_rank(slow),*/ m_type(companion), 
						 m_step(30), m_range(20), m_excution(10), m_mapID(0), 
						 m_explode(FALSE)
{

}

CShellObj::~CShellObj()
{

}

CShellObj::CShellObj(  CPointf point, 
					   UINT tankID/* = 0 */, 
					   UINT type/* = companion */, 
//					   UINT rank/* = fast */, 
					   CLine routine/* = CLine()*/, 
					   UINT state/* = run*/, 
					   float radius/* = 3*/, 
					   float step/* = 30 */, 
					   float range/* = 30 */, 
					   float excution/* = 10*/, 
					   UINT mapID/* = 0 */, 
					   UINT bExplode/* = FALSE*/)
{
	m_position = point ; 
	m_radius = radius ; 
	m_routine = routine ; 
	m_tankID = tankID ; 
	m_state = (STATE)state ; 
	m_type = (TYPE)type ; 
//	m_rank = (RANK)rank ; 
	m_step = step ; 
	m_range = range ; 
	m_excution = excution ; 
	m_mapID = mapID ; 
	m_explode = bExplode ; 
}

void CShellObj::MoveTo()
{
	m_routine.m_start = m_position ; 
	m_routine.CalcAngle() ; 
	if( m_routine.Distance()<m_step ) //arrive
	{
		m_position = m_routine.m_end ; 
		m_state = explode ; 
		return ; 
	}
	else
	{
		m_position = m_routine.GetNextStep(m_step) ; 
	}
}

void CShellObj::DrawShell(CDC *pDC)
{
 	CBrush *pOldBrush ; 
 	CPen *pOldPen ;
	pOldBrush = (CBrush *)pDC->SelectStockObject(NULL_BRUSH) ; 
	pOldPen = (CPen *)pDC->SelectStockObject(BLACK_PEN) ; 

	//µ¯ÌåÒÆ¶¯
	if( m_state==run )
	{
		CRect rect ; 
		rect.left = m_position.x - m_radius ; 
		rect.right = m_position.x + m_radius ; 
		rect.top = m_position.y - m_radius ; 
		rect.bottom = m_position.y + m_radius ; 
		pDC->Ellipse(rect) ;
	}
	else		//±¬Õ¨
	{
		CDC memDC ; 
		CBitmap bmp ; 
		CBitmap *pOldbmp ; 
		BITMAP bm ; 
		memDC.CreateCompatibleDC(pDC) ; 

 		CString filename ; 
 		filename.Format("%d", m_mapID) ; 
 		if( m_mapID<10 )
 		{
 			filename = ".\\graphics\\shell_explode_0" + filename ; 
 		}
 		else 
 		{
 			filename = ".\\graphics\\shell_explode_" + filename ; 
 		}
 		filename += ".bmp" ; 
 		HBITMAP hImage = (HBITMAP)LoadImage(NULL, filename, IMAGE_BITMAP, 
 					0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE ) ; 
 		if( !hImage )
 		{
 			DWORD lastError = GetLastError() ; 
 			TRACE("CShellObj::DrawShell : Load Image Failed!, code : %d\n", lastError) ; 
 			return ; 
 		}
 		bmp.Attach(hImage) ; 
 		bmp.GetBitmap(&bm) ; 
 
 		pOldbmp = memDC.SelectObject(&bmp) ; 
 		pDC->BitBlt( m_position.x-bm.bmWidth/2, m_position.y-bm.bmHeight/2, bm.bmWidth, bm.bmHeight, &memDC, 0, 0, SRCCOPY ) ; 
 		memDC.SelectObject(pOldbmp) ; 
 		bmp.DeleteObject() ; 
	}

 	pDC->SelectObject(pOldBrush) ; 
 	pDC->SelectObject(pOldPen) ; 
}
