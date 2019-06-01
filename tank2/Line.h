//  Line.h: interface for the CLine class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LINE_H__232AD0E1_4AF0_461D_B6FE_716229125605__INCLUDED_)
#define AFX_LINE_H__232AD0E1_4AF0_461D_B6FE_716229125605__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "pointf.h"

class CLine  
{
public:
	CLine();
	virtual ~CLine();
public :
	double CheckAngle(double an);
	BOOL PointInLine(CPointf &point);
	double PointToLine( CPointf &point);
	void SetLine( CPointf &center, double angle , double length);
//	static void GetWhole( double &x );
	CPointf GetNextStep( float step );
	double Distance();
	double CalcAngle();
	void SetLine( CPointf &start, CPointf &end);
	CPointf m_start ; 
	CPointf m_end  ;
public :
	double m_angle ; 
};

#endif // !defined(AFX_LINE_H__232AD0E1_4AF0_461D_B6FE_716229125605__INCLUDED_)
