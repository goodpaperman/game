// Line.cpp: implementation of the CLine class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "tank.h"
#include "Line.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#include "constant.h"
#include "math.h"

CLine::CLine() : m_end(CPointf(0,0)), m_start(CPointf(0, 0))
{
}

CLine::~CLine()
{

}


void CLine::SetLine(CPointf &start, CPointf &end)
{
	m_start = start ; 
	m_end = end ; 
	CalcAngle() ; 
}

void CLine::SetLine(CPointf &center, double angle, double length)
{
	float dx, dy ; 
	dx = (float)length*cos(angle) ; 
	dy = (float)length*sin(angle) ; 
	m_start.x = center.x - dx ; 
	m_start.y = center.y - dy ; 
	m_end.x = center.x + dx ; 
	m_end.y = center.y + dy ; 
	m_angle = angle ; 
}

//计算直线倾角
double CLine::CalcAngle()
{
    double alfa ; 
    double dx, dy ; 
    dx = m_end.x - m_start.x ; 
    dy = m_end.y - m_start.y ; 
    if( fabs(dx)<FLOAT_ZERO )
    {
        if( fabs(dy)<FLOAT_ZERO )
        {
            alfa = 0 ; 
        }
        else if( dy>0 )
        {
            alfa = PI/2 ; 
        }
        else
        {
            alfa = 3*PI/2 ; 
        }
    }
    else
    {
        alfa = atan2(dy, dx) ; 
        if( alfa<0 )
        {
            if( dy>0 )  //第二象限
            {
                alfa += PI ; 
            }
            else        //第四象限
            {
                alfa += 2*PI ; 
            }
        }
        else
        {
            if( dy<0 )  //第三象限
            {
                alfa += PI ; 
            }
        }
    }
    
    m_angle = alfa ; 
    return alfa ; 
}

//计算两端点距离
double CLine::Distance()
{
	double dis = (m_end.x-m_start.x)*(m_end.x-m_start.x)
				 + (m_end.y-m_start.y)*(m_end.y-m_start.y) ;
	return sqrt(dis) ;
}

//根据步长，计算下一个移动点。
CPointf CLine::GetNextStep(float step)
{
	float dx, dy ; 
	dx = (float)step*cos(m_angle) ; 
	dy = (float)step*sin(m_angle) ; 
//	GetWhole(dx) ; 
//	GetWhole(dy) ;  
	CPointf des ; 
	des.x = m_start.x + dx ; 
	des.y = m_start.y + dy ; 
	return des ; 
}

/*
//取整操作
void CLine::GetWhole(double &x)
{
	if( x>=0 )
	{
		x = ceil(x) ; 
	}
	else
	{
		x = floor(x) ; 
	}
}
*/

double CLine::PointToLine(CPointf &point)
{
//	CalcAngle() ; 
	return abs((m_start.x-point.x)*sin(m_angle)-(m_start.y-point.y)*cos(m_angle)) ; 
}

BOOL CLine::PointInLine(CPointf &point)
{
	CLine temp; 
	temp.SetLine(m_start, point) ; 
	temp.CalcAngle() ; 
	double dis = fabs(temp.m_angle-m_angle) ; 
	if( dis>=PI/2 && dis<=3*PI/2 )
	{
//		TRACE("point in line : FALSE\n") ; 
		return FALSE ; 
	}
//	TRACE("point in line : TRUE\n") ; 
	return TRUE ; 
}

double CLine::CheckAngle(double an)
{
    if( an>2*PI )
    {
        an -= 2*PI ; 
    }
    if( an<0 )
    {
        an += 2*PI ;
    }
    return an ; 
}
