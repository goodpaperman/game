// Pointf.h: interface for the CPointf class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_POINTF_H__F8D58E51_EC33_4252_87FA_4D47198D533A__INCLUDED_)
#define AFX_POINTF_H__F8D58E51_EC33_4252_87FA_4D47198D533A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CPointf  
{
public:
	CPointf();
	virtual ~CPointf();
public :
	int round(float value);
	CPointf(float x1, float y1);
	CPointf(CPoint point) ; 
	void operator=(CPointf &pointf) ; 
	CPointf operator+(CPointf &pointf) ; 
	void operator+=(CPointf &pointf) ; 
	CPointf operator-(CPointf &pointf) ; 
	void operator-=(CPointf &pointf) ; 
	BOOL operator==(CPointf &pointf) ; 
	CPoint GetPoint() ; 
	float x ; 
	float y ; 
};

#endif // !defined(AFX_POINTF_H__F8D58E51_EC33_4252_87FA_4D47198D533A__INCLUDED_)
