// Game.cpp: implementation of the CGame class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MapGame.h"
#include "..\include\Game.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGame::CGame()
{
    m_hWnd = NULL;
    m_state = invalid ;
    m_grid.cx = 0 ;
	m_grid.cy = 0 ;
	m_step = 0 ;
	m_time = 0 ;
}

CGame::~CGame()
{
    m_hWnd = NULL;
    m_state = invalid ;
    m_grid.cx = 0 ;
	m_grid.cy = 0 ;	
	m_step = 0 ;
	m_time = 0 ;
}


BOOL CGame::InitGame(int row, int col, HWND hwnd, SIZE size)
{
	if(hwnd == NULL)
	{
		TRACE("Invalid handle %d", hwnd) ;
		return FALSE ;
	}
	m_hWnd = hwnd ;

	if(size.cx==0 || size.cy==0)
	{
		TRACE("Invalid size(%d, %d)", size.cx, size.cy); 
		return FALSE ;
	}
	m_grid = size ;

	if(!m_core.InitMap(row, col))
	{
		TRACE("InitMap failure.") ;
		return FALSE ;
	}
	m_state = run ;
	Run() ;
	return TRUE ;
}

int CGame::GetRow()
{
	if(m_state == run)
		return m_core.GetRow() ;
	else 
		return 0 ;
}

int CGame::GetCol()
{
	if(m_state == run)
		return m_core.GetCol() ;
	else 
		return 0 ;
}

void CGame::Run()
{
	if(m_state == run || m_state == win)
	{
		m_core.RandomMap() ;
		m_step = 0 ;
		m_time = CTime::GetCurrentTime().GetTime() ;
		m_state = run ;
	}
}

void CGame::Clear() 
{
	if(m_state == run)
	{
		m_core.ClearMap() ;
		m_state = invalid ;
	}
}

void CGame::MoveTo(POINT pt)
{
	if(m_state == run)
	{
		Point point = PToP(pt) ;
		Point pointBk = m_core.GetBlankPoint() ;

		if(m_core.SwitchBlank(point))
		{
			m_step++ ;
			ClearRedo() ;
			Direction direct ; 
			if(m_core.GetDirectionFromPoint(point, pointBk, &direct))
				m_undo.AddTail(direct) ;
			else
				TRACE("Warning : Add direction : %d to undo list failure.", direct) ;

			::SendMessage(m_hWnd, WM_REFRESH, 0, 0) ;
		}

		if(m_core.IsWin())
		{
			::SendMessage(m_hWnd, WM_WIN, 0, 0) ;
			m_state = win ;
		}
	}
}


void CGame::MoveTo(Direction direct)
{
	if(m_state == run)
	{
		Direction newDirect = ReverseDirection(direct) ;
		if(m_core.SwitchBlank(newDirect))
		{
			m_step++ ;
			ClearRedo() ;
			m_undo.AddTail(direct) ;

			::SendMessage(m_hWnd, WM_REFRESH, 0, 0) ;
		}

		if(m_core.IsWin())
		{
			::SendMessage(m_hWnd, WM_WIN, 0, 0) ;
			m_state = win ;
		}
	}
} 

//Reverse user direction to kernel direction.
Direction CGame::ReverseDirection(Direction direct)
{
	Direction newDirect ;
	switch(direct)
	{
	case left :
		newDirect = right ;
		break ;
	case right :
		newDirect = left ;
		break ;
	case up :
		newDirect = down ;
		break ;
	case down :
		newDirect = up ;
		break ;
	default :
		TRACE("Unkown direction: %d", direct) ;
		break ;
	}
	return newDirect ;
}

RECT CGame::GetGrid(int row, int col)
{
	RECT rect ;
	rect.left = 0 ;
	rect.right = 0 ;
	rect.top = 0 ;
	rect.bottom = 0 ;

	if(m_state!=run)
		return rect ;

	if(row<0 || row>=m_core.GetRow())
	{
		TRACE("Invalid row %d.", row) ;
		return rect ;
	}

	if(col<0 || col>=m_core.GetCol())
	{
		TRACE("Invalid col %d.", col) ;
		return rect ;
	}

	rect.left = col * m_grid.cx ;
	rect.right = rect.left + m_grid.cx ;
	rect.top = row * m_grid.cy ;
	rect.bottom = rect.top + m_grid.cy ;
	return rect ;
}

int CGame::GetGridValue(int row, int col)
{
	if(m_state == run)
	{
		return m_core.GetValue(m_core.PointToIndex(row, col)) ;
	}
	return 0 ;
}

RECT CGame::GetImageGrid(int row, int col) 
{
	RECT rect ;
	rect.left = 0 ;
	rect.right = 0 ;
	rect.top = 0 ;
	rect.bottom = 0 ;

	if(m_state!=run)
		return rect ;

	if(row<0 || row>=m_core.GetRow())
	{
		TRACE("Invalid row %d.", row) ;
		return rect ;
	}

	if(col<0 || col>=m_core.GetCol())
	{
		TRACE("Invalid col %d.", col) ;
		return rect ;
	}

	int value = m_core.GetValue(m_core.PointToIndex(row, col)) ;
	Point pt = m_core.IndexToPoint(value) ;

	rect = PToR(pt) ;
	return rect ;
}


Point CGame::PToP(POINT pt)
{
	Point point(0, 0) ;
	if(m_state != run)
		return point ;

	int row, col ; 
	row = pt.y / m_grid.cy ;
	col = pt.x / m_grid.cx ;
	
	if(row<0 || row>=m_core.GetRow())
	{
		TRACE("Invalid row %d.", row) ;
		return point ;
	}

	if(col<0 || col>=m_core.GetCol())
	{
		TRACE("Invalid col %d.", col) ;
		return point ;
	}

	point.row = row ;
	point.col = col ;
	return point ;
}

RECT CGame::PToR(Point pt)
{
	RECT rect ;
	rect.left = 0 ;
	rect.right = 0 ;
	rect.top = 0 ;
	rect.bottom = 0 ;

	if(m_state != run)
		return rect ;

	if(pt.row<0 || pt.row>=m_core.GetRow())
	{
		TRACE("Invalid Point(%d, %d)", pt.row, pt.col) ;
		return rect ;
	}

	if(pt.col<0 || pt.col>=m_core.GetCol())
	{
		TRACE("Invalid Point(%d, %d)", pt.row, pt.col) ;
		return rect ;
	}

	rect.left = pt.col * m_grid.cx ;
	rect.right = rect.left + m_grid.cx ;
	rect.top = pt.row * m_grid.cy ;
	rect.bottom = rect.top + m_grid.cy ;
	return rect ;
}


void CGame::PrintMap(BOOL bCout)
{
	if(m_state == run)
	{
		m_core.PrintMap(bCout) ;
		if(bCout)
		{
			cout<<"Step : "<<m_step<<endl ;
			CTimeSpan t(GetTime()); 
			cout<<"Time : "<<t.GetHours()<<":"<<t.GetMinutes()<<":"<<t.GetSeconds()<<endl ;
			cout<<"Undo List: " ;
			POSITION pos = m_undo.GetTailPosition() ;
			while(pos != NULL)
			{
				Direction direct = m_undo.GetPrev(pos) ;
				switch(direct)
				{
				case 0:
					cout<<"left, "  ;
					break ;
				case 1:
					cout<<"right, " ;
					break ;
				case 2:
					cout<<"up, "; 
					break ;
				case 3:
					cout<<"down, "; 
					break ;
				default :
					break ;
				}
			}
			cout<<endl;
			
			cout<<"Redo List: " ;
			pos = m_redo.GetTailPosition() ;
			while(pos != NULL)
			{
				Direction direct = m_undo.GetPrev(pos) ;
				switch(direct)
				{
				case 0:
					cout<<"left, "  ;
					break ;
				case 1:
					cout<<"right, " ;
					break ;
				case 2:
					cout<<"up, "; 
					break ;
				case 3:
					cout<<"down, "; 
					break ;
				default :
					break ;
				}
			}
			cout<<endl; 
		}
		else
		{
			TRACE("Step : %d\n", m_step) ;
			CTimeSpan t(GetTime()) ;
			TRACE("Time : %d:%d:%d\n", t.GetHours(), t.GetMinutes(), t.GetSeconds()) ;
			TRACE("Undo List: ") ;
			POSITION pos = m_undo.GetTailPosition() ;
			while(pos != NULL)
			{
				Direction direct = m_undo.GetPrev(pos) ;
				switch(direct)
				{
				case 0:
					TRACE("left, ")  ;
					break ;
				case 1:
					TRACE("right, ") ;
					break ;
				case 2:
					TRACE("up, "); 
					break ;
				case 3:
					TRACE("down, "); 
					break ;
				default :
					break ;
				}
			}
			TRACE("\n");
			
			TRACE("Redo List: ") ;
			pos = m_redo.GetTailPosition() ;
			while(pos != NULL)
			{
				Direction direct = m_undo.GetPrev(pos) ;
				switch(direct)
				{
				case 0:
					TRACE("left, ")  ;
					break ;
				case 1:
					TRACE("right, ") ;
					break ;
				case 2:
					TRACE("up, "); 
					break ;
				case 3:
					TRACE("down, "); 
					break ;
				default :
					break ;
				}
			}
			TRACE("\n"); 
		}
	}
}


void CGame::Undo()
{
	if(m_state != run)
		return;

	if(!CanUndo())
		return ; 

	Direction direct = m_undo.RemoveTail() ;		//user direct
	if(m_core.SwitchBlank(direct))					//undo : reverse 1st
	{												//convert to kernel : reverse 2nd
		m_redo.AddTail(ReverseDirection(direct)) ;
		m_step++ ;
		::SendMessage(m_hWnd, WM_REFRESH, 0, 0) ;
	}
}

void CGame::Redo()
{
	if(m_state != run)
		return;

	if(!CanRedo())
		return ; 

	Direction direct = m_redo.RemoveTail() ;	//redo : reverse 1st
	if(m_core.SwitchBlank(direct))				//convert to kernel : reverse 2nd
	{
		m_undo.AddTail(ReverseDirection(direct)) ;
		m_step++ ;
		::SendMessage(m_hWnd, WM_REFRESH, 0, 0) ;
	}
}

BOOL CGame::CanRedo()
{
	if(m_state != run)
		return FALSE ;

	return m_redo.GetCount()>0 ;
}

BOOL CGame::CanUndo()
{
	if(m_state != run)
		return FALSE ;

	return m_undo.GetCount()>0 ;
}

void CGame::ClearRedo()
{
	if(m_state != run)
		return;

	for(int i=0; i<m_redo.GetCount(); i++)
	{
		m_redo.RemoveTail() ;
	}
}

int CGame::GetStepCount()
{
	return m_step ;
}

long CGame::GetTime() 
{
	return CTime::GetCurrentTime().GetTime() - m_time ;
}
