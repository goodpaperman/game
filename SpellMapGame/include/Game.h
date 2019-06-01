// Game.h: interface for the CGame class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GAME_H__4A9C652F_DD0F_44E1_99B8_9254E4838268__INCLUDED_)
#define AFX_GAME_H__4A9C652F_DD0F_44E1_99B8_9254E4838268__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MapCore.h"
#include "AfxTempl.h"

#define     WM_WIN              WM_USER + 100 
#define     WM_REFRESH          WM_USER + 101


typedef enum tagState
{
	invalid,
    run,
	win
}State ;

typedef CList<Direction, Direction> StepList ;

class _declspec(dllexport) CGame  
{
public:
	CGame();
	virtual ~CGame();

    BOOL InitGame(int row, int col, HWND hwnd, SIZE size) ;
	int GetRow() ;
	int GetCol() ;

	//direction here mean other grid to blank grid,
	//same with user direction.
	void Run() ;
	void Clear() ;
    void MoveTo(POINT pt) ;
    void MoveTo(Direction direct) ; 
	RECT GetGrid(int row, int col) ;
	int  GetGridValue(int row, int col) ;
	RECT GetImageGrid(int row, int col) ;

	void Undo() ;
	void Redo() ;
	BOOL CanRedo() ;
	BOOL CanUndo() ;
	void ClearRedo() ;
	int GetStepCount() ;
	long GetTime() ;

	void PrintMap(BOOL bCout = FALSE) ;

	Point PToP(POINT pt) ;
	RECT  PToR(Point pt) ;

private :
	Direction ReverseDirection(Direction direct) ;

private :
    CMapCore m_core ;
    HWND m_hWnd ;
    State m_state ;
    SIZE m_grid ;
	int m_step ;
	long m_time ;

	//direction in list is user point of view.
	StepList m_undo ;
	StepList m_redo ;
};

#endif // !defined(AFX_GAME_H__4A9C652F_DD0F_44E1_99B8_9254E4838268__INCLUDED_)
