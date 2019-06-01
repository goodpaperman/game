// MapCore.h: interface for the CMapCore class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAPCORE_H__F422E1D3_3E0F_430F_9F90_3F15AD2AE5FB__INCLUDED_)
#define AFX_MAPCORE_H__F422E1D3_3E0F_430F_9F90_3F15AD2AE5FB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <iostream.h>
#include <iomanip.h>

// Point.row & Point.col are 0-based integers.
typedef struct _declspec(dllexport) tagPoint
{
	tagPoint()	{row=0; col=0 ;}
	tagPoint(int r, int c) { row=r; col=c; }

	int row ;
	int col ;
}Point ;

typedef enum tagDirection
{
	left, 
	right, 
	up, 
	down
}Direction ;

class _declspec(dllexport) CMapCore  
{
public:
	CMapCore();
	virtual ~CMapCore();

	BOOL InitMap(int row, int col) ;
	void RandomMap() ;

	Point IndexToPoint(int index) ;
	int   PointToIndex(Point pt) ;
	int   PointToIndex(int row, int col) ;

	//direction here mean blank grid to other grid
	//reverse with user direction.
	BOOL SwitchBlank(Direction direct) ;
	BOOL SwitchBlank(Point pt) ;
	BOOL SwitchGrid(Point pt0, Point pt1) ;
	BOOL SwitchGrid(Point pt, Direction direct) ;
	BOOL IsNabour(Point pt0, Point pt1) ;
	BOOL GetDirectionFromPoint(Point pt0, Point pt1, Direction *pDirect) ;
	BOOL CanMove(Point pt, Direction direct) ;
	BOOL IsWin() ;

	int GetRow(); 
	int GetCol() ;
	int GetValue(int index); 
	Point GetBlankPoint() ;
	int GetBlankIndex() ;
	void PrintMap(BOOL bCout = FALSE) ;
	void ClearMap() ;

public :
	BOOL m_bRandomBlank ;

private :
	// row & col are 1-based integers.
	int m_row, m_col ;
	int *m_map ;
	BOOL m_bValid ;
};

//extern "C" _declspec(dllexport) CMapCore* CreateObject() ;
//extern "C" EXPORT CMapCore* CreateObject() ;
#endif // !defined(AFX_MAPCORE_H__F422E1D3_3E0F_430F_9F90_3F15AD2AE5FB__INCLUDED_)
