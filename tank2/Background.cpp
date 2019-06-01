// Background.cpp : implementation file
//

#include "stdafx.h"
#include "tank.h"
#include "Background.h"
#include "TankDlg.h"
//class CTankDlg ; 

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBackground



#define CheckAngle(an) \
{\
	if( (an)>2*PI )\
	{\
		(an) -= 2*PI ; \
	}\
	if( (an)<0 )\
	{\
		(an) += 2*PI ;\
	}\
}
//#include "constant.h"
#include "math.h"
int CBackground::m_height ; 
int CBackground::m_width ; 
ShellList CBackground::m_shells ; 
TankQueue CBackground::m_tanks ; 

CBackground::CBackground() : lbDown(FALSE), msMove(FALSE), nCreate(0)
{
}

CBackground::~CBackground()
{
}


BEGIN_MESSAGE_MAP(CBackground, CStatic)
	//{{AFX_MSG_MAP(CBackground)
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBackground message handlers

void CBackground::InitBackground()
{
	SetSize() ; 
	//创建用于显示的位图
	CClientDC WinDC(this) ;
//	BITMAPFILEHEADER bmfHeader ; 
	
	//创建256标准色的调色板
	lpLogPal = (LPLOGPALETTE)new BYTE[sizeof(LOGPALETTE)+256*sizeof(PALETTEENTRY)] ; 
	lpLogPal->palVersion = 0x0300 ; 
	lpLogPal->palNumEntries = 256 ; 
	memcpy(lpLogPal->palPalEntry, rgbStd256, sizeof(RGBQUAD)*256) ; 
	VERIFY(m_Palette.CreatePalette(lpLogPal)) ; 

	//创建位图
	CSize size = GetSize() ;	//取得显示范围大小
	lpbi = (LPBITMAPINFO)new BYTE[sizeof(BITMAPINFOHEADER)+(256*sizeof(RGBQUAD))] ; 
	lpbi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER) ; 
	lpbi->bmiHeader.biWidth = size.cx ; 
	lpbi->bmiHeader.biHeight = size.cy ; 
	lpbi->bmiHeader.biPlanes = 1 ; 
	lpbi->bmiHeader.biBitCount = 8 ; 
	lpbi->bmiHeader.biCompression = BI_RGB ; 
	lpbi->bmiHeader.biSizeImage = WIDTHBYTES((DWORD)size.cx*8)*size.cy ; 
	lpbi->bmiHeader.biXPelsPerMeter = 0 ; 
	lpbi->bmiHeader.biYPelsPerMeter = 0 ; 
	lpbi->bmiHeader.biClrUsed = 0 ; 
	lpbi->bmiHeader.biClrImportant = 0 ; 

	memcpy(lpbi->bmiColors, rgbStd256, sizeof(RGBQUAD)*256) ; 

	HBITMAP hBitmap = CreateDIBSection( WinDC.m_hDC, lpbi, 
					DIB_RGB_COLORS, (void **)&m_pBits, NULL, 0) ; 
	if (hBitmap==NULL)
	{
		delete [](BYTE *)lpLogPal ; 
		delete [](BYTE *)lpbi ; 
		lpLogPal = NULL ; 
		lpbi = NULL ; 
		AfxMessageBox("CPreview::InitView():创建显示位图失败！") ; 
		return ; 
	}
	m_Bitmap.Attach(hBitmap) ; 

	InitObject() ; 
}

void CBackground::InitObject()
{
	SetTimer(TIMER_SLOW, TIME_SLOW, NULL) ; 
	SetTimer(TIMER_NORMAL, TIME_NORMAL, NULL) ; 
	SetTimer(TIMER_QUICK, TIME_QUICK, NULL) ; 
	SetTimer(TIMER_FAST, TIME_FAST, NULL) ; 
	SetTimer(TIMER_EXPLODE, TIME_EXPLODE, NULL) ; 

/*	m_tanks.SetSize(7) ; 
	CTankObj *pTank = new CTankObj(CPoint(20, 80)) ; 
	m_tanks.SetAt(0, pTank) ; 
	pTank = new CTankObj(CPoint(20, 20), 1, 0, 0) ; 
	m_tanks.SetAt(1, pTank) ; 
	pTank = new CTankObj(CPoint(20, 40), 1, 0, 1) ; 
	m_tanks.SetAt(2, pTank) ; 
	pTank = new CTankObj(CPoint(20, 60), 1, 0, 1) ; 
	m_tanks.SetAt(3, pTank) ; 
	pTank = new CTankObj(CPoint(40, 60), 1, 0, 2) ; 
	m_tanks.SetAt(4, pTank) ; 
	pTank = new CTankObj(CPoint(40, 20), 1, 0, 2) ; 
	m_tanks.SetAt(5, pTank) ; 
	pTank = new CTankObj(CPoint(500, 400), 1, 0, 3) ; 
	m_tanks.SetAt(6, pTank) ; 
*/
}

void CBackground::OnDestroy() 
{
	CStatic::OnDestroy();
	
	// TODO: Add your message handler code here
/*
	KillTimer(TIMER_SLOW) ; 
	KillTimer(TIMER_NORMAL) ; 
	KillTimer(TIMER_QUICK) ; 
	KillTimer(TIMER_FAST) ; 
	KillTimer(TIMER_EXPLODE) ; 
*/
	Pause(TRUE) ; 
	Restart() ; 
	if( lpLogPal!=NULL )
	{
		delete [](BYTE *)lpLogPal ; 
	}
	if( lpbi!=NULL )
	{
		delete [](BYTE *)lpbi ; 
	}
	m_Bitmap.DeleteObject() ; 
}

CSize CBackground::GetSize()
{
	CSize size(m_width, m_height) ; 
//	TRACE("\n****Background size is : %d, %d", size.cx, size.cy) ; 
	return size ; 
}

void CBackground::SetSize()
{
	CRect rect ; 
	GetClientRect(&rect) ; 
	m_width = rect.Width() ; 
	m_height = rect.Height() ; 
}
void CBackground::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CDC MemDC ; 
	VERIFY( MemDC.CreateCompatibleDC(&dc) ) ; 

	CPalette *m_pOldPalette = dc.SelectPalette(&m_Palette, FALSE) ; 
	dc.RealizePalette() ; 
	CBitmap *m_pOldBitmap = MemDC.SelectObject(&m_Bitmap) ; 
	
	//将图形输入到位图上.
	DrawWithoutFlicker(&MemDC) ; 
	//OnPaint只是简单的输出已经绘制好的位图。
	dc.BitBlt(0, 0, m_width, m_height, &MemDC, 0, 0, SRCCOPY) ; 

	MemDC.SelectObject(m_pOldBitmap) ; 
	dc.SelectPalette(m_pOldPalette, TRUE) ; 	// Do not call CStatic::OnPaint() for painting messages
	
	// Do not call CStatic::OnPaint() for painting messages
}


void CBackground::DrawWithoutFlicker(CDC *pDC)
{
// 	BITMAP bm ; 
// 	CBitmap bmp ; 
//	HBITMAP hImage ; 
// 	CBitmap *pOldbmp ; 
 	CBrush *pOldBrush ; 
 	CPen *pOldPen ; 
 
 	CDC memDC ; 
 	memDC.CreateCompatibleDC(pDC) ; 
 
//	pDC->SetMapMode(MM_TWIPS) ; 
//	pDC->SetViewportOrg(CPoint(0, m_height)) ; 

 	//刷新屏幕背景
 	pOldBrush = (CBrush *)pDC->SelectStockObject(WHITE_BRUSH) ; 
 	pOldPen = (CPen *)pDC->SelectStockObject(WHITE_PEN) ; 
 	pDC->Rectangle(0, 0, m_width, m_height) ; 
 	pDC->SelectObject(pOldBrush) ; 
 	pDC->SelectObject(pOldPen) ; 

	POSITION pos = m_shells.GetHeadPosition() ; 
	POSITION posAfter = pos ; 
	while( pos!=NULL )
	{
		m_shells.GetNext(posAfter) ; 
		CShellObj *pShell = m_shells.GetAt(pos) ; 		
		if( pShell->m_explode )
		{
			delete pShell ; 
			m_shells.RemoveAt(pos) ; 
		}
		else
		{
			pShell->DrawShell(pDC) ; 
		}
		pos = posAfter ; 
	}
//	TRACE("SHELLS : %d\n", m_shells.GetCount()) ; 

	for( int i=0 ; i<m_tanks.GetSize() ; i++ )
	{
		m_tanks.GetAt(i)->DrawTank(pDC,i) ; 
	}

	pDC->DrawFocusRect(&m_focus) ; 

}


void CBackground::OnLButtonDown(UINT uFlags, CPoint point)
{
//	TRACE("\n***%d, %d", point.x, point.y) ; 
//	TRACE("\n***%d, %d", point.x, point.y) ; 
//	BOOL bAttack = uFlags & MK_CONTROL ; 
	CPointf pointf = CorrectCoordinate(CPointf(point)) ; 

	CTankObj *pTank ;
	switch( nCreate )
	{
		case 0 :
			pTank = new CTankObj(pointf, 0, CTankDlg::tankRank, 100+CTankDlg::tankFireRange) ; 
			m_tanks.Add(pTank) ; 
			Invalidate() ; 
			break ; 
		case 1 :
			pTank = new CTankObj(pointf, 1, CTankDlg::tankRank, 100+CTankDlg::tankFireRange) ; 
			m_tanks.Add(pTank) ; 
			Invalidate() ; 
			break ; 
		default :
			lbDown = TRUE ; 
			msMove = FALSE ; 
			m_focus.TopLeft() = point ; 
			m_focus.BottomRight() = point ; 
			break ; 
	}
}

void CBackground::OnLButtonUp(UINT uFlags, CPoint point)
{
	if( nCreate<2 )
	{
		return ;
	}
	CPointf pointf = CorrectCoordinate(CPointf(point)) ; 
	BOOL bCtrl = uFlags & MK_CONTROL ; 
	BOOL bShift = uFlags & MK_SHIFT ; 

	int i ; 
	lbDown = FALSE ; 
	if( msMove )		//select by rectangle
	{
		CRect focus = m_focus ; 
		m_focus.TopLeft() = CPoint(0, 0) ; 
		m_focus.BottomRight() = CPoint(0, 0) ; 
		OnPaint() ; 
		if( !bShift )
		{
			RemoveSelection() ; 
		}
		for( i=0 ; i<m_tanks.GetSize() ; i++ )
		{
			CTankObj *pTank = m_tanks.GetAt(i) ; 
			if( pTank->m_type==CTankObj::companion
				&& pTank->m_life>0
				&& pTank->SelectByRect(focus))
			{
				m_tanksSel.Add(pTank) ; 
				pTank->m_bSel = TRUE ; 
			}
		}

	}
	else if( bCtrl )		//force_attack
	{
		for( i=0 ; i<m_tanksSel.GetSize() ; i++ )
		{
			CTankObj *pTank = m_tanksSel.GetAt(i) ; 
			pTank->m_state = CTankObj::force_attack ; 
			pTank->SetRoutine(pointf, TRUE) ; 
			pTank->m_bPrepared = FALSE ; 
		}
	}
	else
	{
		for( i=0 ; i<m_tanks.GetSize() ; i++ )
		{
			CTankObj *pTank = m_tanks.GetAt(i) ; 
			if( pTank->m_life>0 && pTank->HitTest(pointf) )
			{
				if( pTank->m_type==CTankObj::companion
					&& pTank->m_life>0)	//select by click
				{
					if( !bShift )
					{
						RemoveSelection() ; 
					}
					m_tanksSel.Add(pTank) ; 
					pTank->m_bSel = TRUE ; 
				}
				else										//attack
				{
					for( int j=0 ; j<m_tanksSel.GetSize() ; j++ )
					{
						CTankObj *pTank = m_tanksSel.GetAt(j) ; 
						pTank->m_state = CTankObj::attack ; 
						pTank->m_attackID = i ; 
						pTank->m_bPrepared = FALSE ; 
					}
				}
				return ; 
			}
		}
		//move
		for( i=0 ; i<m_tanksSel.GetSize() ; i++ )
		{
			CTankObj *pTank = m_tanksSel.GetAt(i) ; 
			pTank->m_state = CTankObj::move ; 
			pTank->SetRoutine(ScatterDestination(pointf, m_tanksSel.GetSize(), i)) ;
		}
	}
}

CPointf CBackground::ScatterDestination(CPointf start, int num, int id)
{
	CPointf point = start ; 
	int right, bottom ; 
	int size = sqrt(num) ; 
	if( num>size*size )
	{
		size++ ; 
	}
//	TRACE("SIZE : %d\n", size) ; 
	right = point.x + (size-1)*TANK_SIZE*2 + TANK_SIZE ; 
	bottom = point.y + num/size*TANK_SIZE*2 + TANK_SIZE ; 
//	TRACE("RIGHT: %d, BOTTOM: %d, WIDTH: %d, HEIGHT: %d\n", right, bottom, m_width, m_height) ; 
	if( right>m_width )
	{
		point.x -= right-m_width ; 
	}
	if( bottom>m_height )
	{
		point.y -= bottom-m_height ; 
	}

	CPointf pos ; 
	pos.x = point.x + 2*TANK_SIZE*(id%size) ; 
	pos.y = point.y + 2*TANK_SIZE*(id/size) ; 
	return pos ; 
}

void CBackground::OnMouseMove(UINT uFlags, CPoint point)
{
	CPointf pointf = CorrectCoordinate(CPointf(point)) ; 
	CPoint pt = pointf.GetPoint() ; 
//	BOOL bAttack = uFlags & MK_CONTROL ; 
	if( lbDown )
	{
		msMove = TRUE ; 
		OnPaint() ; 
		if( pt.x < m_focus.left ) 
		{
			m_focus.left = pt.x ; 
		}
		if( pt.x > m_focus.right )
		{
			m_focus.right = pt.x ; 
		}
		if( pt.y < m_focus.top )
		{
			m_focus.top = pt.y ; 
		}
		if( pt.y > m_focus.bottom )
		{
			m_focus.bottom = pt.y ; 
		}
		OnPaint() ; 
	}
/*	else 
	{
		for( int i=0 ; i<m_tanks.GetSize() ; i++ )
		{
			CTankObj *pTank = m_tanks.GetAt(i) ; 
			if( pTank->m_life>0 && pTank->HitTest(point) )
			{
				m_curCross = AfxGetApp()->LoadCursor(IDC_CURSOR_CROSS) ;
				SetCursor(m_curCross) ; 
				ShowCursor(TRUE) ; 
				return ; 
			}
		}
	}
*/
}

void CBackground::OnRButtonDown(UINT uFlags, CPoint point)
{
	if( nCreate<2 )
	{
		nCreate++ ;
	}
	else
	{
		RemoveSelection() ; 
	}
}

void CBackground::RemoveSelection()
{
	for( int i=0 ; i<m_tanksSel.GetSize() ; i++ )
	{
		CTankObj *pTank = m_tanksSel.GetAt(i) ; 
		pTank->m_bSel = FALSE ; 
	}
	m_tanksSel.RemoveAll() ; 
}

//修正坐标值，防止坦克越界
CPointf CBackground::CorrectCoordinate(CPointf &point)
{
	CRect correct(0, 0, m_width, m_height) ; 
	correct.InflateRect(-TANK_SIZE, -TANK_SIZE) ; 
//	if( !PtInRect(&correct, point.GetPoint()) )
	{
		if( point.x < correct.left )
		{
			point.x = correct.left ; 
		}
		if( point.x > correct.right )
		{
			point.x = correct.right ; 
		}
		if( point.y < correct.top )
		{
			point.y = correct.top ;
		}
		if( point.y > correct.bottom ) 
		{
			point.y = correct.bottom ; 
		}
	}
	return point ; 
}

void CBackground::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	switch( nIDEvent )
	{
		case TIMER_SLOW	:
		{
//			TRACE("\n***Timer: %d", nIDEvent) ; 
			for( int i=0 ; i<m_tanks.GetSize() ; i++ )
			{
				CTankObj *pTank = m_tanks.GetAt(i) ; 
				if( pTank->m_rank == CTankObj::slow && pTank->m_life>0)
				{
					switch( pTank->m_state )
					{
						case CTankObj::move :
							pTank->MoveTo(i) ; 
							break ; 
						case CTankObj::force_attack :
							pTank->ForceAttack(i) ; 
							break ; 
						case CTankObj::defence :
							pTank->Defence(i) ; 
							break ; 
						case CTankObj::attack :
							pTank->Attack(i) ; 
							break ; 
						default :
							break ;
					}
				}
			}
			break ;
		}
		case TIMER_NORMAL :
		{
//			TRACE("\n***Timer: %d", nIDEvent) ; 
			for( int i=0 ; i<m_tanks.GetSize() ; i++ )
			{
				CTankObj *pTank = m_tanks.GetAt(i) ; 
				if( pTank->m_rank == CTankObj::normal && pTank->m_life>0)
				{
					switch( pTank->m_state )
					{
						case CTankObj::move :
							pTank->MoveTo(i) ; 
							break ; 
						case CTankObj::force_attack :
							pTank->ForceAttack(i) ; 
							break ; 
						case CTankObj::defence :
							pTank->Defence(i) ; 
							break ; 
						case CTankObj::attack :
							pTank->Attack(i) ; 
							break ; 
						default :
							break ;
					}
				}
			}
			break ;
		}
		case TIMER_QUICK :
		{
//			TRACE("\n***Timer: %d", nIDEvent) ; 
			for( int i=0 ; i<m_tanks.GetSize() ; i++ )
			{
				CTankObj *pTank = m_tanks.GetAt(i) ; 
				if( pTank->m_rank == CTankObj::quick && pTank->m_life>0)
				{
					switch( pTank->m_state )
					{
						case CTankObj::move :
							pTank->MoveTo(i) ; 
							break ; 
						case CTankObj::force_attack :
							pTank->ForceAttack(i) ; 
							break ; 
						case CTankObj::defence :
							pTank->Defence(i) ; 
							break ; 
						case CTankObj::attack :
							pTank->Attack(i) ; 
							break ; 
						default :
							break ;
					}
				}
			}
			break ;
		}
		case TIMER_FAST	:
		{
//			TRACE("\n***Timer: %d", nIDEvent) ; 
			for( int i=0 ; i<m_tanks.GetSize() ; i++ )
			{
				CTankObj *pTank = m_tanks.GetAt(i) ; 
				if( pTank->m_rank == CTankObj::fast && pTank->m_life>0)
				{
					switch( pTank->m_state )
					{
						case CTankObj::move :
							pTank->MoveTo(i) ; 
							break ; 
						case CTankObj::force_attack :
							pTank->ForceAttack(i) ; 
							break ; 
						case CTankObj::defence :
							pTank->Defence(i) ; 
							break ; 
						case CTankObj::attack :
							pTank->Attack(i) ; 
							break ; 
						default :
							break ;
					}
				}
			}
			break ;
		}
		case TIMER_EXPLODE :
		{
//			TRACE("\n***Timer: %d", nIDEvent) ; 
			POSITION pos = m_shells.GetHeadPosition() ; 
			while( pos!=NULL )
			{
				CShellObj *pShell = m_shells.GetAt(pos) ; 
				switch( pShell->m_state )
				{
					case CShellObj::run :
						pShell->MoveTo() ; 
						break ; 
					case CShellObj::explode : 
						if( pShell->m_mapID<MAP_SIZE )
						{
							pShell->m_mapID++ ; 
						}
						break ; 
					default :
						break ; 
				}
				m_shells.GetNext(pos) ; 
			}
			break ;
		}
		default :
			break ;
	}
	Battle() ; 
	Invalidate() ; 
	CStatic::OnTimer(nIDEvent);
}


void CBackground::Battle()
{
	int i, j ; 

	//防止坦克相互倾轧
	for( i=0 ; i<m_tanks.GetSize() ; i++ )
	{
		for( j=i+1 ; j<m_tanks.GetSize() ; j++ )
		{
			AvoidCrush(i, j) ; 
		}
	}

	//防止越界
	for( i=0 ; i<m_tanks.GetSize() ; i++ )
	{
		CTankObj *pTank = m_tanks.GetAt(i) ; 
		CorrectCoordinate(pTank->m_position) ; 
	}

	//shells explode caculation
	POSITION pos = m_shells.GetHeadPosition() ; 
	while( pos!=NULL )
	{
		CShellObj *pShell = m_shells.GetAt(pos) ; 
		if( pShell->m_mapID==MAP_SIZE )
		{
			pShell->m_explode = TRUE ; 
			LostLife( pShell ) ; 
		}
		m_shells.GetNext(pos) ; 
	}
}


void CBackground::LostLife(CShellObj *pShell)
{
	CLine distance ; 
	distance.m_start = pShell->m_position ; 
	for( int i=0 ; i<m_tanks.GetSize() ; i++ )
	{
		CTankObj *pTank = m_tanks.GetAt(i) ; 
		if( pTank->m_life>0 && pShell->m_tankID!=i)	//tank do not hurt himself
		{
			distance.m_end = pTank->m_position ; 
			double dis = distance.Distance() ; 
			if( dis<=pShell->m_range*0.2 )
			{
				pTank->m_life -= pShell->m_excution * 1.0 ; 
			}
			else if( dis<=pShell->m_range*0.5 )
			{
				pTank->m_life -= pShell->m_excution * 0.6 ; 
			}
			else if( dis<=pShell->m_range*0.7 )
			{
				pTank->m_life -= pShell->m_excution * 0.4 ; 
			}
			else if( dis<=pShell->m_range*1.0 )
			{
				pTank->m_life -= pShell->m_excution * 0.2 ; 
			}
			if( pTank->m_life<=0 )	//who kill the tank will be ranked
			{
//				pTank->m_life = 0 ; 
				if( pTank->m_bSel )
				{
					pTank->m_bSel = FALSE ; 
				}
				CTankObj *pWin = m_tanks.GetAt(pShell->m_tankID) ; 
				if( pWin->m_life>0 )
				{
					pWin->m_fireRange += 10 ; 
					if( pWin->m_interval>10 )
					{
						pWin->m_interval-- ; 
					}
					if( pWin->m_turnInterval>0 )
					{
						pWin->m_turnInterval-- ;  
					}
					switch( pTank->m_rank )
					{
						case slow :
							pWin->m_life += 10 ; 
							break ; 
						case normal :
							pWin->m_life += 20 ; 
							break ; 
						case quick :
							pWin->m_life += 30 ; 
							break ; 
						case fast :
							pWin->m_life += 40 ; 
							break ; 
					}
//					if( pWin->m_life>100 )		m_life can be greater than 100
//					{
//						pWin->m_life = 100 ; 
//					}
					if( pWin->m_delta<10*PI/180 )
					{
						pWin->m_delta += PI/180 ;
					}
					
					if( pWin->m_rank==CTankObj::fast )	
					{
						int id = pWin->GetNearestTank(0) ; 
						if( id!=-1 )
						{
							pWin = m_tanks.GetAt(id) ; 
						}
					}
					if( pWin->m_rank<CTankObj::fast )
					{
						pWin->m_rank = (CTankObj::RANK)(pWin->m_rank + 1) ;
					}
				}
			}
		}
	}
}

//防止坦克互相倾轧
BOOL CBackground::AvoidCrush(UINT i, UINT j)
{
	CTankObj *from, *to ; 
	from = m_tanks.GetAt(i) ; 
	to = m_tanks.GetAt(j) ; 
	if( from->m_life<=0 || to->m_life<=0 )
	{
		return TRUE ; 
	}
	CLine distance ; 
	distance.SetLine(from->m_position, to->m_position) ; 
	double dis = distance.Distance() ; 
	if( dis > from->m_radius + to->m_radius)
	{
		return TRUE ; 
	}
	
	if( from->IsMove() )
	{
		MakeWay(from, to) ; 
	}
	if( to->IsMove() )
	{
		MakeWay(to, from) ; 
	}

	distance.SetLine(from->m_position, to->m_position) ; 
	dis = distance.Distance() ; 
	if( dis > from->m_radius + to->m_radius)
	{
		return TRUE ; 
	}
	double delta = (from->m_radius + to->m_radius - dis)/2 ; 
	double angle = distance.CalcAngle() ; 
	double dx, dy ; 
	dx = delta*cos(angle) ; 
	dy = delta*sin(angle) ; 
//	CLine::GetWhole(dx) ; 
//	CLine::GetWhole(dy) ; 
	to->m_position.x += dx ; 
	to->m_position.y += dy ; 
	from->m_position.x -= dx ; 
	from->m_position.y -= dy ; 

	//分散目的地
	if( from->m_routine.Distance()<4*from->m_radius )
	{
		from->m_routine.m_end = from->m_position ; 
//		from->m_routine.CalcAngle() ; 
	}
	if( to->m_routine.Distance()<4*to->m_radius )
	{
		to->m_routine.m_end = to->m_position ; 
//		to->m_routine.CalcAngle() ; 
	}
	
	return FALSE ; 	
}



void CBackground::MakeWay(CTankObj *from, CTankObj *to)
{
	double dis = from->m_routine.PointToLine(to->m_position) ; 
	if( dis<from->m_radius*2 )
	{
		if( from->m_routine.PointInLine(to->m_position) )
		{
			double angle = from->m_routine.m_angle ; 
//			TRACE("angle : %f\n", angle) ; 
			angle += PI/2 ; 
			CheckAngle(angle) ; 
			CLine direct ; 
			direct.m_start = from->m_position ; 
			direct.m_angle = angle ; 
//			TRACE("angle : %f\n", angle) ;
//			TRACE("position : (%d, %d)\n", to->m_position.x, to->m_position.y ) ; 
			if(direct.PointInLine(to->m_position))
			{
//				direct.m_angle = angle ; 
				direct.m_start = to->m_position ; 
				to->m_position = direct.GetNextStep(1) ; 
			}
			else
			{
				angle -= PI ;
				CheckAngle(angle) ; 
//				TRACE("angle : %f\n", angle) ; 
				direct.m_angle = angle ; 
				direct.m_start = to->m_position ; 
				to->m_position = direct.GetNextStep(1) ; 
			}
//			TRACE("position : (%d, %d)\n", to->m_position.x, to->m_position.y ) ; 
		}
	}
}

void CBackground::Pause(BOOL bPause)
{
	if( bPause )
	{
		KillTimer(TIMER_SLOW) ; 
		KillTimer(TIMER_NORMAL) ; 
		KillTimer(TIMER_QUICK) ; 
		KillTimer(TIMER_FAST) ; 
		KillTimer(TIMER_EXPLODE) ; 
	}
	else
	{
		InitObject() ; 
	}
}

void CBackground::Restart()
{
	Pause(TRUE) ; 
	for( int i=0 ; i<m_tanks.GetSize() ; i++ )
	{
		delete m_tanks.GetAt(i) ; 
	}
	m_tanks.RemoveAll() ; 
	m_tanksSel.RemoveAll() ; 

	POSITION pos = m_shells.GetHeadPosition() ; 
	POSITION posAfter = pos ; 
	while( pos!=NULL )
	{
		m_shells.GetNext(posAfter) ; 
		CShellObj *pShell = m_shells.GetAt(pos) ; 
		delete pShell ; 
		pShell = NULL ; 
		m_shells.RemoveAt(pos) ; 
		pos = posAfter ; 
	}
/*
	POSITION pos = m_shells.GetHeadPosition() ; 
	while( pos!=NULL )
	{
		CShellObj *pShell = m_shells.GetAt(pos) ; 
		delete pShell ; 
		pShell = NULL ; 
		m_shells.RemoveAt(pos) ; 
		m_shells.GetNext(pos) ; 
	}

*/
	nCreate = 0 ; 
	Pause(FALSE) ; 
}

void CBackground::AddTank()
{
	nCreate = 0 ; 
}
