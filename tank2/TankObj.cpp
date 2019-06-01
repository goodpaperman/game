// TankObj.cpp: implementation of the CTankObj class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "tank.h"
#include "TankObj.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#include "math.h"
#include "TankDlg.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//class CTankDlg ; 
CTankObj::CTankObj() : m_position(CPointf(TANK_SIZE,TANK_SIZE)), m_radius(TANK_SIZE), 
						m_routine(CLine()), m_launchRoutine(CLine()),  m_state(defence), 
						m_type(companion), m_rank(fast), m_life(TANK_LIFE), m_step(2), 
						m_fireRange(100), m_attackID(0), /*m_attackPt(CPoint(1,1)), */
						m_interval(20), m_turnInterval(2), m_turnCounter(0), 
						m_launchCounter(20), m_bodyAngle(0), m_gunAngle(0),
						m_delta(10*PI/180), m_bSel(FALSE), m_bPrepared(FALSE)
{

}
CTankObj::CTankObj( 
		CPointf position, 
		UINT type/* = companion*/, 
		UINT rank/* = slow*/, 
		float fireRange/* = 100*/, 
		UINT state/* = defence*/, 
		float radius/* = 10*/, 
		CLine routine/* = CLine()*/, 
		float life/* = TANK_LIFE*/, 
		float step/* = 2 */, 
		UINT attackID/* = 0*/, 
//		CPoint attackPt/* = CPoint(1, 1)*/, 
		UINT interval/* = 20*/, 
		UINT turnInterval/* =2*/,
		double bodyAngle/* = 0 */, 
		double gunAngle/* = 0 */, 
		double delta/* = 10*PI/180 */,
		CLine launchRoutine/* = CLine()*/, 
		BOOL bSelect/* = FALSE*/,
		BOOL bPrepared/*=FALSE*/)
{
	m_position = position ; 
	m_type = (TYPE)type ; 
	m_rank = (RANK)rank ; 
	m_state = (STATE)state ; 
	m_radius = radius ; 
	m_routine = routine ; 
	m_life = life ; 
	m_step = step ; 
	m_fireRange = fireRange ; 
	m_attackID = attackID ; 
//	m_attackPt = attackPt ; 
	m_interval = interval ; 
	m_launchCounter = interval ; //launch at once prepare
	m_turnInterval = turnInterval ; 
	m_turnCounter = 0 ;			//don't turn at once prepare
	m_bodyAngle = bodyAngle ; 
	m_gunAngle = gunAngle ; 
	m_delta = delta ; 
	m_launchRoutine = launchRoutine ; 
	m_bSel = bSelect ; 
	m_bPrepared = bPrepared ; 
}

CTankObj::~CTankObj()
{

}

//ȡ���ڿڵ㣬���ڻ����ڹ�
CPoint CTankObj::GetGunDirection()
{
	CPointf point ; 
	point.x = m_position.x + m_radius*cos(m_gunAngle) ; 
	point.y = m_position.y + m_radius*sin(m_gunAngle) ; 
	return point.GetPoint() ; 
}

//caculate wheel lines 
void CTankObj::GetWheel(CLine *left, CLine *right)
{
	double angle ; 
	angle = m_bodyAngle + PI/2 ; 
	CheckAngle(angle) ; 
	CLine diameter ; 
	diameter.SetLine(m_position, angle, TANK_SIZE) ; 
	left->SetLine(diameter.m_start, m_bodyAngle, TANK_SIZE) ; 
	right->SetLine(diameter.m_end, m_bodyAngle, TANK_SIZE) ; 
}

//����̹���н�·��, �����ӵ�·��
void CTankObj::SetRoutine(CPointf &point, BOOL bGun)
{
	if( bGun )
	{
		m_launchRoutine.SetLine(m_position, point) ; 
	}
	else
	{
		m_routine.SetLine(m_position, point) ; 
	}
}

void CTankObj::DrawTank(CDC *pDC, UINT id)
{ 
	if( m_life<=0 && !CTankDlg::bShowDead )
	{
		return ; 
	}

 	CBrush *pOldBrush ; 
 	CPen *pOldPen ;
	pOldBrush = (CBrush *)pDC->SelectStockObject(NULL_BRUSH) ; 
	pOldPen = (CPen *)pDC->SelectStockObject(BLACK_PEN) ; 

	CBrush brushBlue(RGB(0, 0, 255)) ; 
	CBrush brushRed(RGB(255, 0, 0)) ; 
	CBrush brushGreen(RGB(0, 255, 0)) ; 
	CBrush brushYellow(RGB(224, 224, 64)) ; 

	//̹����
	CRect rect ; 
	rect.left = m_position.x - m_radius ; 
	rect.right = m_position.x + m_radius ; 
	rect.top = m_position.y - m_radius ; 
	rect.bottom = m_position.y + m_radius ; 
	pDC->Ellipse(rect) ;

	//ѡ��״̬
	if( m_bSel )
	{
		rect.InflateRect(3, 3) ; 
		CPoint end ;
		end.x = rect.left ; 
		end.y = rect.top + m_radius*0.6 ; 
		pDC->MoveTo(rect.TopLeft()) ; 
		pDC->LineTo(end) ; 
		end.y = rect.top ; 
		end.x = rect.left + m_radius*0.6 ; 
		pDC->MoveTo(rect.TopLeft()) ; 
		pDC->LineTo(end) ; 

		end.x = rect.left ; 
		end.y = rect.bottom - m_radius*0.6 ; 
		pDC->MoveTo(rect.left, rect.bottom) ; 
		pDC->LineTo(end) ; 
		end.y = rect.bottom ; 
		end.x = rect.left + m_radius*0.6 ; 
		pDC->MoveTo(rect.left, rect.bottom) ; 
		pDC->LineTo(end) ; 
		
		end.x = rect.right ; 
		end.y = rect.top + m_radius*0.6 ; 
		pDC->MoveTo(rect.right, rect.top) ; 
		pDC->LineTo(end) ; 
		end.y = rect.top ; 
		end.x = rect.right - m_radius*0.6 ; 
		pDC->MoveTo(rect.right, rect.top) ; 
		pDC->LineTo(end) ; 

		end.x = rect.right ; 
		end.y = rect.bottom - m_radius*0.6 ; 
		pDC->MoveTo(rect.right, rect.bottom) ; 
		pDC->LineTo(end) ; 
		end.y = rect.bottom ; 
		end.x = rect.right - m_radius*0.6 ; 
		pDC->MoveTo(rect.right, rect.bottom) ; 
		pDC->LineTo(end) ; 
	}

	//����
	CRect top ; 
	top.left = m_position.x - 3 ; 
	top.right = m_position.x + 3 ; 
	top.top = m_position.y - 3 ; 
	top.bottom = m_position.y + 3 ;
	if( m_type == companion )
	{		
		pDC->FillRect(&top, &brushBlue) ;   
	}
	else
	{
		pDC->FillRect(&top, &brushRed) ;   
	}

	//�ڹ�
	pDC->MoveTo(m_position.GetPoint()) ; 
	pDC->LineTo(GetGunDirection()) ; 

	//����
	CLine left, right ; 
	GetWheel(&left, &right) ; 
	pDC->MoveTo(left.m_start.GetPoint()) ; 
	pDC->LineTo(left.m_end.GetPoint()) ; 
	pDC->MoveTo(right.m_start.GetPoint()) ; 
	pDC->LineTo(right.m_end.GetPoint()) ; 

	//�󷽵���
	{
		CLine axis, baffle ; 
		axis.SetLine(m_position, m_bodyAngle, TANK_SIZE+1) ; 
		double baffleAngle = m_bodyAngle+PI/2 ; 
		CheckAngle(baffleAngle) ; 
		baffle.SetLine(axis.m_start, baffleAngle, TANK_SIZE/2) ; 
		pDC->MoveTo(baffle.m_start.GetPoint()) ; 
		pDC->LineTo(baffle.m_end.GetPoint()) ; 
	}

	//����
	if( m_life>0 )
	{
		CRect life ; 
		life.left = m_position.x - m_radius ; 
		life.top = m_position.y + m_radius ; 
		life.bottom = life.top + 4 ; 
		life.right = life.left + 2*m_radius ;
 		pDC->Rectangle(life) ; 
		
		float quotiety = (m_life>100?100:m_life)/TANK_LIFE ; //m_life may be greater than 100
		if( quotiety>0.66 )
		{
			life.right = life.left + 2*m_radius*(quotiety-0.66)*3 ; 
			life.InflateRect(-1, -1) ; 
			pDC->FillRect(life, &brushGreen) ; 
		}
		else if( quotiety>0.33 )
		{
			life.right = life.left + 2*m_radius*(quotiety-0.33)*3 ; 
			life.InflateRect(-1, -1) ; 
			pDC->FillRect(life, &brushYellow) ; 
		}
		else if( quotiety>1e-2 )
		{
			life.right = life.left + 2*m_radius*quotiety*3 ; 
			life.InflateRect(-1, -1) ; 
			pDC->FillRect(life, &brushRed) ; 
		}
	}

	//���
	if( CTankDlg::bShowID)
	{
		CRect rtID ; 
		rtID.left = m_position.x - m_radius ; 
		rtID.top = m_position.y + m_radius + 4 ; 
		rtID.bottom = rtID.top + 15 ; 
		rtID.right = rtID.left + 2*m_radius ;
		CString strID ;
		strID.Format("%d", id) ; 
		pDC->DrawText(strID, rtID, DT_CENTER) ; 
	}
	

	//�ȼ�
	CRect rank ;
	rank.right = m_position.x + m_radius ; 
	rank.left = rank.right - RANK_SIZE ; 
	rank.top = m_position.y - m_radius ; 
	rank.bottom = rank.top + RANK_SIZE ; 

	CBrush rankBrush(RGB(255, 0, 0)) ; 
	switch( m_rank )		//�˴�������break�������
	{
		case fast :
		{
			CRect fast = rank ; 
			fast.top += 2*RANK_SIZE + 2 ; 
			fast.bottom += 2*RANK_SIZE + 2 ; 
			pDC->FillRect(fast, &rankBrush) ; 
		}
		case quick :
		{
			CRect quick = rank ; 
			quick.top += 1*RANK_SIZE + 1 ; 
			quick.bottom += 1*RANK_SIZE + 1 ; 
			pDC->FillRect(quick, &rankBrush) ; 
		}
		case normal :
		{
			CRect normal = rank ; 
			pDC->FillRect(normal, &rankBrush) ; 
		}
		case slow :
		default :
			break ;
	}

	//·��
	if( CTankDlg::bShowRoutine && m_state==move && m_life>0 )
	{
		pDC->MoveTo(m_routine.m_start.GetPoint()) ; 
		pDC->LineTo(m_routine.m_end.GetPoint()) ; 
	}

	//����Ŀ��
	if( CTankDlg::bShowAttackDes && m_state==attack && m_life>0 && m_bPrepared)
	{
		pDC->MoveTo(m_routine.m_start.GetPoint()) ; 
		pDC->LineTo(m_routine.m_end.GetPoint()) ; 
		CTankObj *pTank = CBackground::m_tanks.GetAt(m_attackID) ; 
		pDC->MoveTo(m_routine.m_end.GetPoint()) ; 
		pDC->LineTo(pTank->m_position.GetPoint()) ; 
	}

	//������Χ
	if( CTankDlg::bShowFireRange )
	{
		CRect range ; 
		range.left = m_position.x - m_fireRange ; 
		range.right = m_position.x + m_fireRange ; 
		range.top = m_position.y - m_fireRange ; 
		range.bottom = m_position.y + m_fireRange ; 
		pDC->Ellipse(range) ; 
	}

 	pDC->SelectObject(pOldBrush) ; 
 	pDC->SelectObject(pOldPen) ; 
}

//��֤�Ƕ���0-2PI��Χ�����ڼ���
void CTankObj::CheckAngle(double &an)
{
	if( an>2*PI )
	{
		an -= 2*PI ; 
	}
	if( an<0 )
	{
		an += 2*PI ;
	}
}

//ת��̹���ڹܶ�׼Ŀ�ĵأ���ת��̹����������ֵ��ʾ�Ƿ�ת�����
BOOL CTankObj::TurnAbout(BOOL bGun)
{
	double angle, alfa, beta ; 
	if( bGun ) 
	{
		alfa = m_launchRoutine.m_angle ; 
		beta = fabs(alfa-m_gunAngle) ; 
		angle = m_gunAngle ; 
	}
	else
	{
		alfa = m_routine.m_angle ; 
		beta = fabs(alfa-m_bodyAngle) ; 
		angle = m_bodyAngle ; 
	}

	if( beta<m_delta )
	{
		angle = alfa ; 
		if( bGun )
		{
			m_gunAngle = angle ; 
		}
		else
		{
			m_bodyAngle = angle ; 
		}
		return TRUE ;
	}
	else if( beta<PI )
	{
		if( alfa>angle )
		{
			angle += m_delta ; 
		}
		else
		{
			angle -= m_delta ; 
		}
	}
	else
	{
		if( alfa>angle )
		{
			angle -= m_delta ; 
		}
		else
		{
			angle += m_delta ; 
		}
	}
	CheckAngle(angle) ; 
	if( bGun )
	{
		m_gunAngle = angle ; 
	}
	else
	{
		double temp = m_bodyAngle - angle ; 
		m_bodyAngle = angle ; 
		m_gunAngle -= temp ;			//�ڹ�������ת��
		CheckAngle(m_gunAngle) ; 
	}
//	TRACE("\n&&& TurnAbout : %f", m_angle/PI*180) ; 
	return FALSE ; //launch need this false to make sure launch after aim
//	return TRUE ; //turn when move
}

//�ƶ�̹�˵�Ŀ�ĵ�
void CTankObj::MoveTo(int id)
{
	m_routine.m_start = m_position ; 
	m_routine.CalcAngle() ; 
	if( m_routine.Distance()<m_step ) //arrive
	{
		m_position = m_routine.m_end ; 
		if(m_state==move)		//attack don't
		{
			m_state = defence ; 
		}
		return ; 
	}
	else //if( TurnAbout() ) turn when move 
	{
		m_turnCounter++ ; 
		if(m_turnCounter>m_turnInterval)
		{
			TurnAbout() ; 
			m_turnCounter = 0 ; 
		}
		if( m_routine.Distance()<m_step )
		{
			m_position = m_routine.m_end ; 
			if(m_state==move)		//attack don't
			{
				m_state = defence ; 
			}
		}
		else 
		{
			m_routine.m_angle = m_bodyAngle ; //next step is base on current angle.not destination angle .
			m_position = m_routine.GetNextStep(m_step) ; 	
			if( CTankDlg::bMoveAttack ) 
			{
				Defence(id) ; 
			}
		}
	}
}


void CTankObj::ForceAttack(int index)
{
	m_launchRoutine.m_start = m_position ; 
	m_launchRoutine.CalcAngle() ; 
	if( m_launchRoutine.Distance()<=m_fireRange )
	{
		Launch(index) ; 
	}
	else
	{		
		if(m_bPrepared)
		{
			CLine attackDis ; 
			attackDis.m_start = m_routine.m_end ; 
			attackDis.m_end = m_launchRoutine.m_end ; 
			if( attackDis.Distance()>m_fireRange )
			{
				m_routine.m_end = PrepareAttack(FALSE) ; //recaculate attack position 
			}
		}
		else
		{
			m_bPrepared = TRUE ; 
			m_routine.m_end = PrepareAttack(TRUE) ; //recaculate attack position and angle
		}
		MoveTo(index) ; 
	}
}

CPointf CTankObj::PrepareAttack(BOOL bRecaculateAngle)
{
//	m_launchRoutine.CalcAngle() ; 
	CLine attackLine ; 
	if( bRecaculateAngle )
	{
		double alfa = acos(m_fireRange/m_launchRoutine.Distance()) ;
		TRACE("alfa : %f; ", alfa/PI*180) ; 
		alfa = fabs(2*alfa) ; 
		if( alfa<0.01)	//near the fireRange alfa is very little.
		{
			alfa = PI ;
		}
//		TRACE("alfa : %f; ", alfa/2/PI*180) ; 
		srand((UINT)time(NULL)) ;
		float angleRandom = rand()%((int)(alfa*100)) ; 
		angleRandom /= 100 ; 
		angleRandom -= alfa/2 ; 
		double angle = m_launchRoutine.CalcAngle() ; 
		TRACE("randomAngle : %f, angle : %f\n", angleRandom/PI*180, 360-angle/PI*180) ;
		angle += angleRandom + PI ; //angle������ڷ����߶��ԣ����Դ�ֵ�Ա������߼��㣬�����һ��PI��ƫ�ơ�
		CheckAngle(angle) ; 
		attackLine.m_start = m_launchRoutine.m_end ; 
		attackLine.m_angle = angle ;  
	}
	else 
	{
		attackLine.m_start = m_launchRoutine.m_end ; 
		attackLine.m_end = m_routine.m_end ; 
		attackLine.CalcAngle() ; 
	}
	return attackLine.GetNextStep(m_fireRange-1) ; 
	//m_launchRoutine.GetNextStep(m_launchRoutine.Distance()-m_fireRange+2) ; 
}

void CTankObj::Launch(int id)
{
	if( TurnAbout(TRUE) )
	{
		m_launchCounter++ ; 
		if( m_launchCounter>=m_interval )
		{
			srand((UINT)time(NULL)) ; 
			int x, y ; 
			if( m_state==defence 
				|| m_state==force_attack 
				|| m_state==attack)
			{
				x = rand()%7 - 3 ; 
				y = rand()%7 - 3 ; 
			}
			else		//move defence 
			{
				float dis = m_launchRoutine.Distance() ; 
				dis = 4*TANK_SIZE*dis/m_fireRange ; 
				x = rand()%(int)dis - dis/2 ; 
				y = rand()%(int)dis - dis/2 ; 
			}
//			TRACE("X : %d, Y : %d\n", x, y ) ; 
			m_launchRoutine.m_end.x += x ; 
			m_launchRoutine.m_end.y += y ; 
			m_launchRoutine.CalcAngle() ; 
			CShellObj *shell = new CShellObj(GetGunDirection(),
											 id, 
											 m_type, 
											 m_launchRoutine) ; 
			CBackground::m_shells.AddTail(shell) ; 
			m_launchCounter = 0 ; 
			if( m_state==attack || m_state==defence )
			{
				//m_attackID is set at Defence .
				CTankObj *pTank = CBackground::m_tanks.GetAt(m_attackID) ; 
				if( pTank->m_state==defence && (pTank->m_type==enemy || CTankDlg::bAutoAttack) )
				{
					int ID = pTank->GetNearestTank(1) ; 
					if( ID==-1)  //������Χ��û�е���
					{
						pTank->m_attackID = id ; 
						pTank->m_state = attack ; 
						pTank->m_bPrepared = FALSE ; //need to prepare attack position
						pTank->GroupAction(m_attackID) ; 
					}
				}
			}
		}
	}
}

BOOL CTankObj::HitTest(CPointf &point)
{
	CLine dis ; 
	dis.SetLine(m_position, point) ; 
	if( dis.Distance()<=m_radius )
	{
		return TRUE ; 
	}
	else
	{
		return FALSE ; 
	}
}

BOOL CTankObj::SelectByRect(CRect &Rect)
{
	CRect tankRect ; 
	tankRect.left = m_position.x - m_radius ; 
	tankRect.right = m_position.x + m_radius ; 
	tankRect.top = m_position.y - m_radius ; 
	tankRect.bottom = m_position.y + m_radius ; 
	CRect intersect ; 
	intersect.IntersectRect(&tankRect, &Rect) ;
	if( intersect.IsRectNull() )
	{
		return FALSE ; 
	}
	else
	{
		return TRUE ; 
	}
}

void CTankObj::Defence(int id)
{
	int attackID = GetNearestTank(1) ; 
	if( attackID!=-1 )
	{
		if(m_state==defence||m_state==move) //attackID can not be changed when attack
		{
			m_attackID = attackID ; //used by Launch.
		}
		CTankObj *pTank = CBackground::m_tanks.GetAt(attackID) ; 
		m_launchRoutine.SetLine(m_position, pTank->m_position) ; 
		Launch(id) ; 
	}
	else if( CTankDlg::bAutoAttack && /*m_type==enemy &&*/ m_state==defence)	//enemy active stochastic
	{
		static int seed ; 
		srand((UINT)time(NULL)+seed) ;
		seed = rand() ; 
//		TRACE("SEED : %d\n" , seed) ; 
		if( seed%679==0 )//1769
		{
			if( seed%191!=0 )//91
			{
				int enemyID = GetNearestTank(3) ; //attack
				if( enemyID!=-1)
				{
					m_attackID = enemyID ; 
					m_state = attack ; 
					m_bPrepared = FALSE ; //need to prepare attack position
					GroupAction(id) ; 
				}
			}
			else		//move
			{
				CPoint des ; 
				srand((UINT)time(NULL)+seed) ; 
				des.x = rand()%(CBackground::m_width - 2*TANK_SIZE) + TANK_SIZE ; 
				des.y = rand()%(CBackground::m_height - 2*TANK_SIZE) + TANK_SIZE ; 
				m_routine.SetLine(m_position, CPointf(des)) ; 
				m_state = move ; 
				GroupAction(id) ; 
			}
		}
	}
}


void CTankObj::Attack(int id)
{
	CTankObj *pDes = CBackground::m_tanks.GetAt(m_attackID) ; 
	if( pDes->m_life<=0 )
	{
		m_state = defence ; 
		return ; 
	}
	m_launchRoutine.SetLine(m_position, pDes->m_position) ;  
	if( m_launchRoutine.Distance()<=m_fireRange )
	{
		m_routine.m_end = m_position ; 
		Launch(id) ; 
	}
	else
	{		
		if(m_bPrepared)
		{
			CLine attackDis ; 
			attackDis.m_start = m_routine.m_end ; 
			attackDis.m_end = m_launchRoutine.m_end ; 
			if( attackDis.Distance()>m_fireRange )
			{
				m_routine.m_end = PrepareAttack(FALSE) ; //recaculate attack position 
			}
		}
		else
		{
			m_bPrepared = TRUE ; 
			m_routine.m_end = PrepareAttack(TRUE) ; //recaculate attack position and angle
		}
		MoveTo(id) ; 
	}
}

BOOL CTankObj::IsMove()
{
	return ( m_state!=defence && !(m_routine.m_end==m_position) ) ; 
}


//Flag : 0 - nearest friend and rank < fast; 
//		 1 - nearest enemy in fireRange
//		 2 - nearest friend and id!=ID(param[i]) ; 
//		 3 - nearest enemy (egnore fireRange)
int CTankObj::GetNearestTank(UINT Flag, UINT param )
{
	typedef CArray<int,int> intArray ; 
	intArray *arrID ; 
	int id=-1 ; 
	double disMax ; 
	switch(Flag)
	{
		case 0:
		case 3 :
			disMax = 1000 ; 
			break ; 
		case 1 : 
			disMax = m_fireRange ; 
			break ; 
		case 2 :
			disMax = 1000 ; 
			arrID = (intArray*)param ; 
			break ; 
		default :
			break ;
	}

	for( int i=0 ; i<CBackground::m_tanks.GetSize() ; i++ )
	{
		CTankObj *pTank = CBackground::m_tanks.GetAt(i) ; 
		if( pTank == this )
		{
			continue ; 
		}
		switch( Flag )
		{
			case 0 :
			{
				if( pTank->m_type == m_type 
					&& pTank->m_life>0
					&& pTank->m_rank<CTankObj::fast)
				{
					CLine dis ; 
					dis.SetLine(pTank->m_position, m_position) ; 
					if( dis.Distance()<disMax )
					{
						disMax = dis.Distance() ; 
						id = i ;
					}
				}
				break ; 
			}
			case 1 :
			case 3 :
			{
				if( pTank->m_type!=m_type 
					&& pTank->m_life>0 )
				{
					CLine dis ; 
					dis.SetLine(m_position, pTank->m_position) ; 
					if( dis.Distance()<disMax )
					{
						id = i ; 
						disMax = dis.Distance() ; 
					}
				}
				break ;
			}
			case 2 :
			{
				BOOL bContinue = FALSE ; 
				for(int j=0 ; j<arrID->GetSize() ; j++ )
				{
					if( i==arrID->GetAt(j) )
					{
						bContinue = TRUE ; 
					}
				}
				if( !bContinue 
					&& pTank->m_type==m_type
					&& pTank->m_life>0 
					&& pTank->m_state==defence
					&& pTank->GetNearestTank(1)==-1)
				{
					CLine dis ; 
					dis.SetLine(pTank->m_position, m_position) ; 
					if( dis.Distance()<disMax )
					{
						disMax = dis.Distance() ; 
						id = i ;
					}
				}
				break ;
			}
			default :
				break ; 
		}
	}
/*	if( Flag==2)
	{
		TRACE("In getNearestTank\nGroup:") ; 
		for( i=0 ; i<arrID->GetSize(); i++)
		{		
			TRACE("%d,", arrID->GetAt(i)) ; 
		}
		TRACE("\n") ; 
	}
*/	return id ; 
}

void CTankObj::GroupAction(int id)
{
	srand((UINT)time(NULL)) ;
	int GROUPCOUNT = rand()%8 + 2 ; 		
	CArray<int,int> arrID ;
	int ID, i ; 
	arrID.SetSize(GROUPCOUNT) ; 
	for( i=0; i<GROUPCOUNT; i++)
	{
		arrID[i] = id ; 
	}
	for( i=0 ; i<GROUPCOUNT ; i++ )
	{
		ID = GetNearestTank(2, (UINT)&arrID) ; 
		if( ID!=-1 )
		{
			arrID[i] = ID ; 
		}
		else
		{
			break ;
		}
	}
	for( i=0 ; i<GROUPCOUNT ; i++ )
	{
		if(arrID[i]==id)
		{
			continue ;
		}
		CTankObj *pTank = CBackground::m_tanks.GetAt(arrID[i]) ; 
		switch(m_state)
		{
			case move :
			{
				pTank->m_routine.m_end = CBackground::ScatterDestination(m_routine.m_end, GROUPCOUNT+1, i+1) ; 
				pTank->m_state = move ; 
				break ; 
			}
			case attack :
			{
				pTank->m_attackID = m_attackID ; 
				pTank->m_state = attack ; 
				pTank->m_bPrepared = FALSE ; //need to prepare attack position
				break ; 
			}
			default :
				break ; 
		}
	}

	TRACE("in GroupAction\nID: %d Group:", id) ; 
	for( i=0 ; i<GROUPCOUNT; i++)
	{		
		TRACE("%d,", arrID[i]) ; 
	}
	TRACE("\n") ; 
}
