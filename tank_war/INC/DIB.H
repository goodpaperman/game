/*****************************************************************************
*                                                                             
*   Dib.h
*                                                                             
*   Electrical Engineering Faculty - Software Lab                             
*   Spring semester 1998                                                      
*                                                                             
*   Tanks game                                                                
*                                                                             
*   Module description: Interface for the CDib class.
*                       
*                                                                             
*   Authors: Eran Yariv - 28484475                                           
*            Moshe Zur  - 24070856                                           
*                                                                            
*                                                                            
*   Date: 23/09/98                                                           
*                                                                            
******************************************************************************/

/////////////////////////////////////////////////////////////////////////////
// Copyright (C) 1998 by Jorge Lodos
// All rights reserved
//
// Distribute and use freely, except:
// 1. Don't alter or remove this notice.
// 2. Mark the changes you made
//
// Send bug reports, bug fixes, enhancements, requests, etc. to:
//    lodos@cigb.edu.cu
/////////////////////////////////////////////////////////////////////////////

// dib.h

#ifndef _INC_DIB
#define _INC_DIB

/* DIB constants */
#define PALVERSION   0x300

/* Dib Header Marker - used in writing DIBs to files */
#define DIB_HEADER_MARKER   ((WORD) ('M' << 8) | 'B')

/* DIB Macros*/
#define RECTWIDTH(lpRect)     ((lpRect)->right - (lpRect)->left)
#define RECTHEIGHT(lpRect)    ((lpRect)->bottom - (lpRect)->top)

// WIDTHBYTES performs DWORD-aligning of DIB scanlines.  The "bits"
// parameter is the bit count for the scanline (biWidth * biBitCount),
// and this macro returns the number of DWORD-aligned bytes needed
// to hold those bits.

#define WIDTHBYTES(bits)    (((bits) + 31) / 32 * 4)

typedef BYTE PIXEL, *PPIXEL;

class CDIB : public CObject
{
    DECLARE_DYNAMIC(CDIB)

// Constructors
public:
    CDIB();

// Attributes
    LPBYTE m_pBits;
    LPBITMAPINFO m_pBMI;
    CPalette* m_pPalette;

    DWORD           Width()     const;
    DWORD           Height()    const;
    CSize           Size()      const;
    WORD            NumColors() const;
    BOOL            IsValid()   const;

// Operations
    BOOL            ReadFromResource (UINT);
    PIXEL&          ColorAt (UINT uX, UINT uY);
    void            FillSolidColor (BYTE R, BYTE G, BYTE B);
    void            FillRect (int x, int y, int w, int h, int R, int G, int B );
    BOOL            CreateEmpty (UINT uXSize, UINT uYSize);
    BOOL            CreateRotated (
                        CDIB *pSrc, 
                        UINT uAngle /* 0, 90, 180 or 270 only */, 
                        BOOL bFlipHoriz, 
                        BOOL bFlipVert);
    BOOL            Paint(CDC *pDC, LPRECT p1=NULL, LPRECT p2=NULL) const;
    HGLOBAL         CopyToHandle();
    DWORD           Save(CFile& file);
    DWORD           Read(CFile& file);
    DWORD           ReadFromHandle(HGLOBAL hGlobal);
    BOOL            CopyFrom (CDIB *pSrc);
    BOOL            CopyPalette (CDIB *pSrc);
    BOOL            CopyPalette (CPalette *pSrc);
    BOOL            GetPaletteFromResourceBitmap (UINT uResID);
    void            PasteCKRect (   CDIB *Dib, 
                                    int x, 
                                    int y, 
                                    PIXEL ColorKey);
    BOOL            CopyRectFrom (  CDIB *pSrcDIB, 
                                    int SrcX, 
                                    int SrcY, 
                                    UINT SrcWidth, 
                                    UINT SrcHeight,
                                    int DstX, int DstY); 

    void            Invalidate();

    virtual void    Serialize(CArchive& ar);

// Implementation
    virtual         ~CDIB();

protected:
    BOOL            CreatePalette();
    DWORD           CalcImageSize();
    WORD            PaletteSize()               const;
    void            Free();
    PPIXEL          FindPixel(UINT x, UINT y)   const;

public:
#ifdef _DEBUG
    virtual void    Dump(CDumpContext& dc)      const;
#endif
};

#include "dib.inl"


#endif //!_INC_DIB
