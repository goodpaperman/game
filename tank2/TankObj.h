// TankObj.h: interface for the CTankObj class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TANKOBJ_H__A7F0D421_80FD_4616_9EB1_61C83D8F89B9__INCLUDED_)
#define AFX_TANKOBJ_H__A7F0D421_80FD_4616_9EB1_61C83D8F89B9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "line.h"
#include "constant.h"
#include "background.h"
#include "shellobj.h"

class CTankObj  
{
public:
	CTankObj();
	CTankObj( 
		CPointf position , 
		UINT type = companion, 
		UINT rank = slow, 
		float fireRange = 100, 
		UINT state = defence, 
		float radius = 10, 
		CLine routine = CLine(), 
		float life = TANK_LIFE, 
		float step = 2 , 
		UINT attackID = 0, 
//		CPoint attackPt = CPoint(1, 1), 
		UINT interval = 20, 
		UINT turnInterval = 2, 
//		UINT counter = 0 ; 
		double bodyAngle = 0 , 
		double gunAngle = 0 , 
		double delta = 10*PI/180 ,
		CLine launchRoutine = CLine(), 
		BOOL bSelect = FALSE, 
		BOOL bPrepared = FALSE);
	virtual ~CTankObj();

public :
	void GroupAction(int id);
	int GetNearestTank( UINT Flag, UINT param=0);
	BOOL IsMove();
	void Attack(int id );
	void Defence( int id);
	BOOL SelectByRect( CRect &Rect );
	BOOL HitTest( CPointf &point );
	void Launch(int id);
	CPointf PrepareAttack(BOOL bRecaculateAngle);
	void ForceAttack(int index);
	void GetWheel( CLine *left, CLine *right );
	void CheckAngle( double &an );
	BOOL TurnAbout(BOOL bGun = FALSE);
	void MoveTo(int id);
	void SetRoutine( CPointf &point, BOOL bGun = FALSE );
	CPoint GetGunDirection();
	void DrawTank(CDC *pDC,UINT id);
	typedef enum {stay = 0, defence, move, attack, force_attack}STATE ; 
	typedef enum {companion = 0, enemy}TYPE ; 
	typedef enum {slow = 0, normal, quick, fast}RANK ; 

	CPointf m_position ;		//位置
	float m_radius ;			//半径
	CLine m_routine ;		//行进路径
	CLine m_launchRoutine ; //发射路径
	STATE m_state ;			//状态
	TYPE m_type ;			//类别
	RANK m_rank ;			//等级
	float m_life ;			//生命值
	float m_step ;			//前进步长
	float m_fireRange ;		//攻击半径
	UINT m_attackID ;		//攻击坦克ID
//	CPoint m_attackPt ;		//强制攻击点
	UINT m_interval ;		//发射间隔(时间片)
	UINT m_launchCounter ;	//发射计数
	UINT m_turnInterval ;   //转弯间隔(时间片)
	UINT m_turnCounter ;    //转弯计数
	double m_bodyAngle ;	//车身角度
	double m_gunAngle ;		//炮管角度
	double m_delta ;		//转动角度
	BOOL m_bSel ;				//被选中
	BOOL m_bPrepared ;		//准备好了攻击位置?
};

#endif // !defined(AFX_TANKOBJ_H__A7F0D421_80FD_4616_9EB1_61C83D8F89B9__INCLUDED_)
