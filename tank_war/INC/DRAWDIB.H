/*****************************************************************************
*                                                                             
*   DrawDib.h
*                                                                             
*   Electrical Engineering Faculty - Software Lab                             
*   Spring semester 1998                                                      
*                                                                             
*   Tanks game                                                                
*                                                                             
*   Module description: interface for the CDrawDib class.
*                       
*                                                                             
*   Authors: Eran Yariv - 28484475                                           
*            Moshe Zur  - 24070856                                           
*                                                                            
*                                                                            
*   Date: 23/09/98                                                           
*                                                                            
******************************************************************************/
//////////////////////////////////////////////////////////////////////
//
//  Creator : El Barto (ef00@luc.ac.be)
//  Location : http://www.luc.ac.be/~ef00/ebgfx
//  Date : 09-04-98
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DRAWDIB_H__B1E2F1A3_CCA7_11D1_91CA_0020AFF82585__INCLUDED_)
#define AFX_DRAWDIB_H__B1E2F1A3_CCA7_11D1_91CA_0020AFF82585__INCLUDED_

#include  <vfw.h>
#include "DIB.h"
#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CDrawDib  
{
public:
    HDRAWDIB m_hDrawDib;

    HDRAWDIB Open ();
    BOOL Close ();
    
    BOOL Begin ( HDC hdc, int dxDest, int dyDest,
        LPBITMAPINFOHEADER lpbi, int dxSrc, int dySrc, UINT wFlags );
    BOOL End ();

    BOOL Draw ( HDC hdc, int xDst, int yDst,
        int dxDst, int dyDst, LPBITMAPINFOHEADER lpbi, LPVOID lpBits,
        int xSrc, int ySrc, int dxSrc, int dySrc, UINT wFlags );
    BOOL DrawDib ( CDIB *Dib, HDC hdc, int xDst, int yDst,
        int dxDst, int dyDst, UINT wFlags );
    BOOL DrawDib ( CDIB *Dib, HDC hdc, int xDst, int yDst, int dxDst, int dyDst, 
                   int xSrc, int ySrc, int dxSrc, int dySrc,
                   UINT wFlags );
    
    LPVOID GetBuffer ( LPBITMAPINFOHEADER lpbi,
        DWORD dwSize, DWORD dwFlags );
    
    BOOL ProfileDisplay ( LPBITMAPINFOHEADER lpbi );

    HPALETTE GetPalette ();
    BOOL SetPalette ( HPALETTE hpal );
    BOOL ChangePalette (int iStart, int iLen, LPPALETTEENTRY lppe );
    UINT Realize ( HDC hdc, BOOL fBackground );

    BOOL Start ( LONG rate );
    BOOL Stop ();
    BOOL Time ( LPDRAWDIBTIME lpddtime );

    CDrawDib ();
    virtual ~CDrawDib ();
};

#endif // !defined(AFX_DRAWDIB_H__B1E2F1A3_CCA7_11D1_91CA_0020AFF82585__INCLUDED_)
