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

	CPointf m_position ;		//λ��
	float m_radius ;			//�뾶
	CLine m_routine ;		//�н�·��
	CLine m_launchRoutine ; //����·��
	STATE m_state ;			//״̬
	TYPE m_type ;			//���
	RANK m_rank ;			//�ȼ�
	float m_life ;			//����ֵ
	float m_step ;			//ǰ������
	float m_fireRange ;		//�����뾶
	UINT m_attackID ;		//����̹��ID
//	CPoint m_attackPt ;		//ǿ�ƹ�����
	UINT m_interval ;		//������(ʱ��Ƭ)
	UINT m_launchCounter ;	//�������
	UINT m_turnInterval ;   //ת����(ʱ��Ƭ)
	UINT m_turnCounter ;    //ת�����
	double m_bodyAngle ;	//����Ƕ�
	double m_gunAngle ;		//�ڹܽǶ�
	double m_delta ;		//ת���Ƕ�
	BOOL m_bSel ;				//��ѡ��
	BOOL m_bPrepared ;		//׼�����˹���λ��?
};

#endif // !defined(AFX_TANKOBJ_H__A7F0D421_80FD_4616_9EB1_61C83D8F89B9__INCLUDED_)
