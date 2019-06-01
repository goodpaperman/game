// MapCore.cpp: implementation of the CMapCore class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "..\include\MapCore.h"
#include "Math.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMapCore::CMapCore()
{
	m_map = NULL ;
	m_row = 0 ;
	m_col = 0 ;
	m_bValid = FALSE ;
	m_bRandomBlank = TRUE ;
}

CMapCore::~CMapCore()
{
	ClearMap() ;
}

BOOL CMapCore::InitMap(int row, int col)
{
	if(row<=2 || col<=2 || row>100 || col>100)
	{
		m_bValid = FALSE ;
		TRACE("Invalid row[%d] or col[%d].\n", row, col) ;
		return FALSE ;
	}

	m_row = row ;
	m_col = col ;
	m_map = new int [m_row * m_col] ;

	for(int i=0; i<m_row * m_col ; i++)
		m_map[i] = i ;

	m_bValid = TRUE ;
	return TRUE ;
}

void CMapCore::RandomMap()
{
	if(m_bValid)
	{
		srand((unsigned)time(NULL)); 
		int count = m_row*m_col ;
		int *tmp = new int[count] ; 
		
		for(int i=0; i<count ; i++)
		{
			tmp[i] = i ;
		}

		if(!m_bRandomBlank)
		{
			count-- ;
		}

		for(i=0; i<count; i++)
		{
			int num = -1 ;
			num = rand() % count ;
			while(tmp[num] == -1)
			{
				num = rand() % count ;
			}
			m_map[i] = tmp[num] ;

			tmp[num] = -1 ;
		}

		delete []tmp ;
	}
	else
		TRACE("Invalid.\n") ;
}

Point CMapCore::IndexToPoint(int index)
{
	if(!m_bValid || index<0 || index>=m_row*m_col)
	{
		TRACE("Invalid index.\n") ;
		return Point(0, 0) ;
	}

	Point pt ; 
	pt.row = index / m_col ;
	pt.col = index % m_col ; 
	return pt ;
}

int   CMapCore::PointToIndex(Point pt)
{
	return PointToIndex(pt.row, pt.col) ;
}

int   CMapCore::PointToIndex(int row, int col)
{
	int index = row * m_col + col ;
	if(!m_bValid || index<0 || index>=m_row * m_col)
	{
		TRACE("Invalid point.\n") ;
		return 0 ;
	}
	else 
		return index ;

}

BOOL CMapCore::SwitchBlank(Direction direct)
{
	if(!m_bValid)
	{
		TRACE("Invalid.\n") ;
		return FALSE;
	}

	return SwitchGrid(GetBlankPoint(), direct) ;
}

BOOL CMapCore::SwitchBlank(Point pt)
{
	if(!m_bValid)
	{
		TRACE("Invalid.\n") ;
		return FALSE;
	}

	Point blank = GetBlankPoint() ;
	if(IsNabour(blank, pt))
		return SwitchGrid(blank, pt) ;
	else 
		return FALSE ;
}

BOOL CMapCore::SwitchGrid(Point pt0, Point pt1)
{
	if(!m_bValid)
	{
		TRACE("Invalid.\n") ;
		return FALSE;
	}

	if(pt0.row<0 || pt0.row>=m_row || pt0.col<0 || pt0.col>=m_col)
	{
		TRACE("Invalid Point (%d, %d).\n", pt0.row, pt0.col) ;
		return FALSE;
	}

	if(pt1.row<0 || pt1.row>=m_row || pt1.col<0 || pt1.col>=m_col)
	{
		TRACE("Invalid Point (%d, %d).\n", pt1.row, pt1.col) ;
		return FALSE;
	}


	int index0 = PointToIndex(pt0) ;
	int index1 = PointToIndex(pt1) ;

	if(index0<0 || index0>=m_row*m_col)
	{
		TRACE("Invalid Index %d.\n", index0) ;
		return FALSE;
	}

	if(index1<0 || index1>=m_row * m_col) 
	{
		TRACE("Invalid Index %d.\n", index1) ;
		return FALSE ;
	}

	int tmp = m_map[index0] ;
	m_map[index0] = m_map[index1] ;
	m_map[index1] = tmp ;
	return TRUE ;
}

BOOL CMapCore::SwitchGrid(Point pt, Direction direct)
{
	if(!m_bValid)
	{
		TRACE("Invalid.\n") ;
		return FALSE;
	}

	if(pt.row<0 || pt.row>=m_row || pt.col<0 || pt.col>=m_col)
	{
		TRACE("Invalid Point[%d, %d].", pt.row, pt.col) ;
		return FALSE;
	}

	if(!CanMove(pt, direct))
		return FALSE;


	Point pt2 = pt ;
	switch(direct)
	{
	case left :
		pt2.col --; 
		break ;
	case right :
		pt2.col ++; 
		break ;
	case up :
		pt2.row-- ;
		break ;
	case down :
		pt2.row++ ;
		break ;
	default :
		break ;
	}

	int index0 = PointToIndex(pt) ;
	int index1 = PointToIndex(pt2) ;

	if(index0<0 || index0>=m_row*m_col)
	{
		TRACE("Invalid Index %d.\n", index0) ;
		return FALSE;
	}

	if(index1<0 || index1>=m_row * m_col) 
	{
		TRACE("Invalid Index %d.\n", index1) ;
		return FALSE;
	}

	int tmp = m_map[index0] ;
	m_map[index0] = m_map[index1] ;
	m_map[index1] = tmp ;
	return TRUE ;

}

BOOL CMapCore::IsNabour(Point pt0, Point pt1)
{
	if(!m_bValid)
	{
		TRACE("Invalid.\n") ;
		return FALSE ;
	}
	
	if(pt0.row<0 || pt0.row>=m_row || pt0.col<0 || pt0.col>=m_col)
	{
		TRACE("Invalid Point (%d, %d).\n", pt0.row, pt0.col) ;
		return FALSE ;
	}

	if(pt1.row<0 || pt1.row>=m_row || pt1.col<0 || pt1.col>=m_col)
	{
		TRACE("Invalid Point (%d, %d).\n", pt1.row, pt1.col) ;
		return FALSE ;
	}

	if(pt0.row==pt1.row)
	{
		if(abs(pt0.col-pt1.col)==1)
			return TRUE ;
		else
			return FALSE ;
	}
	else if(pt0.col == pt1.col)
	{
		if(abs(pt0.row-pt1.row)==1)
			return TRUE ;
		else 
			return FALSE ;
	}
	return FALSE ;
}

/*
BOOL CMapCore::IsNabour(int index0, int index1)
{
	if(index0<0 || index0>=m_row*m_col)
	{
		TRACE("Invalid Index %d.\n", index0) ;
		return FALSE ;
	}

	if(index1<0 || index1>=m_row * m_col) 
	{
		TRACE("Invalid Index %d.\n", index1) ;
		return FALSE ;
	}

	Point pt0 = IndexToPoint(index0) ;
	Point pt1 = IndexToPoint(index1) ;

	return IsNabour(pt0, pt1) ;
}
*/
BOOL CMapCore::GetDirectionFromPoint(Point pt0, Point pt1, Direction *pDirect)
{
	if(!m_bValid)
	{
		TRACE("Invalid.\n") ;
		return FALSE ;
	}

	if(pt0.row<0 || pt0.row>=m_row || pt0.col<0 || pt0.col>=m_col)
	{
		TRACE("Invalid Point (%d, %d).\n", pt0.row, pt0.col) ;
		return FALSE ;
	}

	if(pt1.row<0 || pt1.row>=m_row || pt1.col<0 || pt1.col>=m_col)
	{
		TRACE("Invalid Point (%d, %d).\n", pt1.row, pt1.col) ;
		return FALSE ;
	}

	if(pt0.row==pt1.row)
	{
		int result = pt0.col - pt1.col ;
		if(abs(result)==1)
		{
			if(result>0)
				*pDirect = left ;
			else
				*pDirect = right ;

			return TRUE ;
		}
		else
			return FALSE ;
	}
	else if(pt0.col == pt1.col)
	{
		int result = pt0.row - pt1.row ;
		if(abs(result)==1)
		{
			if(result>0)
				*pDirect = up ;
			else
				*pDirect = down ;

			return TRUE ;
		}
		else 
			return FALSE ;
	}
	return FALSE ;
}


BOOL CMapCore::CanMove(Point pt, Direction direct)
{
	if(pt.row<0 || pt.row>=m_row || pt.col<0 || pt.col>=m_col)
	{
		TRACE("Invalid Point[%d, %d].", pt.row, pt.col) ;
		return FALSE ;
	}

	switch(direct)
	{
	case left :
		if(pt.col == 0)
			return FALSE ;
		break ;
	case right :
		if(pt.col == m_col-1)
			return FALSE ;
		break ;
	case up :
		if(pt.row == 0)
			return FALSE ;
		break ;
	case down :
		if(pt.row == m_row-1)
			return FALSE ;
		break ;
	default :
		break ;
	}

	return TRUE ;
}

/*
BOOL CMapCore::CanMove(int index, Direction direct)
{
	if(index<0 || index>=m_row*m_col)
	{
		TRACE("Invalid Index %d.", index) ;
		return FALSE ;
	}
	return CanMove(IndexToPoint(index), direct) ;
}
*/

BOOL CMapCore::IsWin()
{
	if(!m_bValid)
	{
		TRACE("Invalid.\n");
		return FALSE ;
	}

	for(int i=0; i<m_row*m_col; i++)
	{
		if(m_map[i]!=i)
			return FALSE ;
	}

	return TRUE ;
}


int CMapCore::GetRow()
{
	if(m_bValid)
		return m_row ;
	else 
	{
		TRACE("Invalid.\n") ;
		return 0 ;
	}
}

int CMapCore::GetCol()
{
	if(m_bValid)
		return m_col ;
	else 
	{
		TRACE("Invalid.\n") ;
		return 0 ;
	}
}

int CMapCore::GetValue(int index)
{
	if(!m_bValid || index<0 || index>=m_row * m_col)
	{
		TRACE("Invalid.\n") ;
		return 0 ;
	}
	else
		return m_map[index] ;
}

Point CMapCore::GetBlankPoint()
{
	if(m_bValid)
	{
		Point blank = Point(0, 0) ;
		for(int i=0; i<m_row * m_col; i++)
		{
			if(m_map[i] == m_row*m_col - 1)
				blank = IndexToPoint(i) ;
		}
		return blank ;
	}
	else
	{
		TRACE("Invalid.\n") ;
		return Point(0, 0) ;
	}
}

int CMapCore::GetBlankIndex()
{
	return PointToIndex(GetBlankPoint()) ;
}

void CMapCore::PrintMap(BOOL bCout)
{
	if(!m_bValid)
	{
		TRACE("Invalid.\n") ;
		return ;
	}

	if(!bCout)
	{
		for(int i=0; i<m_row; i++)
		{
			TRACE("%d: ", i+1) ;
			for(int j=0; j<m_col; j++)
			{
				TRACE("%-5d|", m_map[i*m_col+j]) ;
			}
			TRACE("\n") ;
		}
	}
	else
	{
		for(int i=0; i<m_row; i++)
		{
			cout<<i+1<<":    " ;
			CString string ;
			for(int j=0; j<m_col; j++)
			{
				if(m_map[i*m_col + j] == m_row * m_col -1)
					string.Format("%5s", "X") ;
				else
					string.Format("%5d", m_map[i*m_col + j]) ;
				cout<<string;
			}
			cout<<endl  ;
		}
	}
		
}

void CMapCore::ClearMap()
{
	if(m_bValid && m_map!=NULL && AfxIsValidAddress(m_map, m_row * m_col))
	{
		delete []m_map ;
		m_map = NULL ;
	}

	m_bValid = FALSE ;
	m_row = 0 ;
	m_col = 0 ;
}

