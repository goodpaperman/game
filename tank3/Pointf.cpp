// Pointf.cpp: implementation of the CPointf class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "tank.h"
#include "Pointf.h"
#include "math.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPointf::CPointf()
{
  x = 0.0 ; 
  y = 0.0 ; 
}

CPointf::~CPointf()
{

}

CPointf::CPointf(float x1, float y1)
{
	x = x1 ; 
	y = y1 ; 
}

CPointf::CPointf(CPoint point) 
{
	x = (float)point.x ; 
	y = (float)point.y ; 
}

void CPointf::operator=(CPointf &pointf)
{
	x = pointf.x ; 
	y = pointf.y ; 
}

CPointf CPointf::operator+(CPointf &pointf)
{
	CPointf re ; 
	re.x = x + pointf.x ; 
	re.y = y + pointf.y ; 
	return re ; 
}

void CPointf::operator+=(CPointf &pointf)
{
	CPointf re ; 
	x += pointf.x ; 
	y += pointf.y ; 
}

CPointf CPointf::operator-(CPointf &pointf)
{
	CPointf re ; 
	re.x = x - pointf.x ; 
	re.y = y - pointf.y ; 
	return re ; 
}

void CPointf::operator-=(CPointf &pointf)
{
	CPointf re ; 
	x -= pointf.x ; 
	y -= pointf.y ; 
}

BOOL CPointf::operator==(CPointf &pointf)
{
	if( x-pointf.x<1e-2 && y-pointf.y<1e-2)
	{
		return TRUE ; 
	}
	return FALSE ; 
}

CPoint CPointf::GetPoint()
{
	CPoint re ; 
	re.x = round(x) ; 
	re.y = round(y) ; 
	return re ; 
}

//value should be greater than 0 
int CPointf::round(float value)
{
	double digit = fabs(value - (int)value) ; 
	if(digit>=0.5)
	{
		return (int)value+1 ; 
	}
	else
	{
		return (int)value ; 
	}
}
