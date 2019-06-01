/*****************************************************************************
*                                                                             
*   Dib.cpp                                                            
*                                                                             
*   Electrical Engineering Faculty - Software Lab                             
*   Spring semester 1998                                                      
*                                                                             
*   Tanks game                                                                
*                                                                             
*   Module description: Implements a DIB object.
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

//  dib.cpp
//

#include "stdafx.h"
#include "dib.h"
#include <windowsx.h>
#include <afxadv.h>

/////////////////////////////////////////////////////////////////////////////
// CDIB

IMPLEMENT_DYNAMIC(CDIB, CObject)


void CDIB::Free()
{
    // Make sure all member data that might have been allocated is freed.
    if (m_pBits)
    {
        GlobalFreePtr(m_pBits);
        m_pBits = NULL;
    }
    if (m_pBMI)
    {
        GlobalFreePtr(m_pBMI);
        m_pBMI = NULL;
    }
    if (m_pPalette)
    {
        m_pPalette->DeleteObject();
        delete m_pPalette;
        m_pPalette = NULL;
    }
}

/*************************************************************************
 *
 * Paint()
 *
 * Parameters:
 *
 * HDC hDC          - DC to do output to
 *
 * LPRECT lpDCRect  - rectangle on DC to do output to
 *
 * LPRECT lpDIBRect - rectangle of DIB to output into lpDCRect
 *
 * CPalette* pPal   - pointer to CPalette containing DIB's palette
 *
 * Return Value:
 *
 * BOOL             - TRUE if DIB was drawn, FALSE otherwise
 *
 * Description:
 *   Painting routine for a DIB.  Calls StretchDIBits() or
 *   SetDIBitsToDevice() to paint the DIB.  The DIB is
 *   output to the specified DC, at the coordinates given
 *   in lpDCRect.  The area of the DIB to be output is
 *   given by lpDIBRect.
 *
 ************************************************************************/
BOOL CDIB::Paint(CDC *pDC, LPRECT lpDCRect, LPRECT lpDIBRect) const
{
    if (!m_pBMI)
        return FALSE;

    CPalette *pOldPal = NULL;        // Previous palette

    // Get the DIB's palette, then select it into DC
    if (m_pPalette != NULL)
    {
        // Select as background since we have
        // already realized in forground if needed
        pOldPal = pDC->SelectPalette(m_pPalette, FALSE);
        pDC->RealizePalette();
    }

    /* Make sure to use the stretching mode best for color pictures */
    pDC->SetStretchBltMode(COLORONCOLOR);

    /* Determine whether to call StretchDIBits() or SetDIBitsToDevice() */
    CRect rFullDC;
    if (NULL == lpDCRect)
    {
        rFullDC.SetRect (0,0, Width() - 1, Height() - 1);
        lpDCRect = &rFullDC;
    }
    if (NULL == lpDIBRect)
    {
        rFullDC.SetRect (0,0, Width() - 1, Height() - 1);
        lpDIBRect = &rFullDC;
    }
    BOOL bSuccess;
    if ((RECTWIDTH(lpDCRect)  == RECTWIDTH(lpDIBRect)) &&
        (RECTHEIGHT(lpDCRect) == RECTHEIGHT(lpDIBRect)))
        bSuccess = ::SetDIBitsToDevice(
           pDC->m_hDC,                 // hDC
           lpDCRect->left,             // DestX
           lpDCRect->top,              // DestY
           RECTWIDTH(lpDCRect),        // nDestWidth
           RECTHEIGHT(lpDCRect),       // nDestHeight
           lpDIBRect->left,            // SrcX
           (int)Height() -
              lpDIBRect->top -
              RECTHEIGHT(lpDIBRect),   // SrcY
           0,                          // nStartScan
           (WORD)Height(),             // nNumScans
           m_pBits,                    // lpBits
           m_pBMI,                     // lpBitsInfo
           DIB_RGB_COLORS);            // wUsage
   else
      bSuccess = ::StretchDIBits(
        pDC->m_hDC,                   // hDC
        lpDCRect->left,               // DestX
        lpDCRect->top,                // DestY
        RECTWIDTH(lpDCRect),          // nDestWidth
        RECTHEIGHT(lpDCRect),         // nDestHeight
        lpDIBRect->left,              // SrcX
        lpDIBRect->top,               // SrcY
        RECTWIDTH(lpDIBRect),         // wSrcWidth
        RECTHEIGHT(lpDIBRect),        // wSrcHeight
        m_pBits,                      // lpBits
        m_pBMI,                       // lpBitsInfo
        DIB_RGB_COLORS,               // wUsage
        SRCCOPY);                     // dwROP

    /* Reselect old palette */
    if (pOldPal != NULL)
    {
        pDC->SelectPalette(pOldPal, TRUE);
    }
    return bSuccess;
}

/*************************************************************************
 *
 * CreatePalette()
 *
 * Return Value:
 *
 * TRUE if succesfull, FALSE otherwise
 *
 * Description:
 *
 * This function creates a palette from a DIB by allocating memory for the
 * logical palette, reading and storing the colors from the DIB's color table
 * into the logical palette, creating a palette from this logical palette,
 * and then returning the palette's handle. This allows the DIB to be
 * displayed using the best possible colors (important for DIBs with 256 or
 * more colors).
 *
 ************************************************************************/
BOOL CDIB::CreatePalette()
{
    if (!m_pBMI)
        return FALSE;

   //get the number of colors in the DIB
   WORD wNumColors = NumColors();

   if (wNumColors != 0)
   {
        // allocate memory block for logical palette
        HANDLE hLogPal = ::GlobalAlloc(GHND, sizeof(LOGPALETTE) + sizeof(PALETTEENTRY)*wNumColors);

        // if not enough memory, clean up and return NULL
        if (hLogPal == 0)
            return FALSE;

        LPLOGPALETTE lpPal = (LPLOGPALETTE)::GlobalLock((HGLOBAL)hLogPal);

        // set version and number of palette entries
        lpPal->palVersion = PALVERSION;
        lpPal->palNumEntries = (WORD)wNumColors;

        for (int i = 0; i < (int)wNumColors; i++)
        {
            lpPal->palPalEntry[i].peRed = m_pBMI->bmiColors[i].rgbRed;
            lpPal->palPalEntry[i].peGreen = m_pBMI->bmiColors[i].rgbGreen;
            lpPal->palPalEntry[i].peBlue = m_pBMI->bmiColors[i].rgbBlue;
            lpPal->palPalEntry[i].peFlags = 0;
        }

        /* create the palette and get handle to it */
        if (m_pPalette)
        {
            m_pPalette->DeleteObject();
            delete m_pPalette;
        }
        m_pPalette = new CPalette;
        BOOL bResult = m_pPalette->CreatePalette(lpPal);
        ::GlobalUnlock((HGLOBAL) hLogPal);
        ::GlobalFree((HGLOBAL) hLogPal);
        return bResult;
    }

    return TRUE;
}


/*************************************************************************
 *
 * NumColors()
 *
 * Return Value:
 *
 * WORD             - number of colors in the color table
 *
 * Description:
 *
 * This function calculates the number of colors in the DIB's color table
 * by finding the bits per pixel for the DIB (whether Win3.0 or other-style
 * DIB). If bits per pixel is 1: colors=2, if 4: colors=16, if 8: colors=256,
 * if 24, no colors in color table.
 *
 ************************************************************************/
WORD CDIB::NumColors() const
{
    if (!m_pBMI)
        return 0;

    WORD wBitCount;  // DIB bit count

    /*  The number of colors in the color table can be less than 
     *  the number of bits per pixel allows for (i.e. lpbi->biClrUsed
     *  can be set to some value).
     *  If this is the case, return the appropriate value.
     */

    DWORD dwClrUsed;

    dwClrUsed = m_pBMI->bmiHeader.biClrUsed;
    if (dwClrUsed != 0)
        return (WORD)dwClrUsed;

    /*  Calculate the number of colors in the color table based on
     *  the number of bits per pixel for the DIB.
     */
    wBitCount = m_pBMI->bmiHeader.biBitCount;

    /* return number of colors based on bits per pixel */
    switch (wBitCount)
    {
        case 1:
            return 2;

        case 4:
            return 16;

        case 8:
            return 256;

        default:
            return 0;
    }
}

/*************************************************************************
 *
 * Save()
 *
 * Saves the specified DIB into the specified CFile.  The CFile
 * is opened and closed by the caller.
 *
 * Parameters:
 *
 * CFile& file - open CFile used to save DIB
 *
 * Return value: Number of saved bytes or CFileException
 *
 *************************************************************************/
DWORD CDIB::Save(CFile& file)
{
    BITMAPFILEHEADER bmfHdr; // Header for Bitmap file
    DWORD dwDIBSize;

    if (m_pBMI == NULL)
        return 0;

    // Fill in the fields of the file header

    // Fill in file type (first 2 bytes must be "BM" for a bitmap)
    bmfHdr.bfType = DIB_HEADER_MARKER;  // "BM"

    // Calculating the size of the DIB is a bit tricky (if we want to
    // do it right).  The easiest way to do this is to call GlobalSize()
    // on our global handle, but since the size of our global memory may have
    // been padded a few bytes, we may end up writing out a few too
    // many bytes to the file (which may cause problems with some apps).
    //
    // So, instead let's calculate the size manually (if we can)
    //
    // First, find size of header plus size of color table.  Since the
    // first DWORD in both BITMAPINFOHEADER and BITMAPCOREHEADER conains
    // the size of the structure, let's use this.
    dwDIBSize = *(LPDWORD)&m_pBMI->bmiHeader + PaletteSize();  // Partial Calculation

    // Now calculate the size of the image
    dwDIBSize += CalcImageSize ();

    // Calculate the file size by adding the DIB size to sizeof(BITMAPFILEHEADER)
    bmfHdr.bfSize = dwDIBSize + sizeof(BITMAPFILEHEADER);
    bmfHdr.bfReserved1 = 0;
    bmfHdr.bfReserved2 = 0;

    /*
     * Now, calculate the offset the actual bitmap bits will be in
     * the file -- It's the Bitmap file header plus the DIB header,
     * plus the size of the color table.
     */
    bmfHdr.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + m_pBMI->bmiHeader.biSize + PaletteSize();

    // Write the file header
    file.Write((LPSTR)&bmfHdr, sizeof(BITMAPFILEHEADER));
    DWORD dwBytesSaved = sizeof(BITMAPFILEHEADER); 

    // Write the DIB header
    UINT nCount = sizeof(BITMAPINFO) + (NumColors()-1)*sizeof(RGBQUAD);
    dwBytesSaved += nCount; 
    file.Write(m_pBMI, nCount);
    
    // Write the DIB bits
    DWORD dwBytes = m_pBMI->bmiHeader.biBitCount * Width();
  // Calculate the number of bytes per line
    if (dwBytes%32 == 0)
        dwBytes /= 8;
    else
        dwBytes = dwBytes/8 + (32-dwBytes%32)/8 + (((32-dwBytes%32)%8 > 0) ? 1 : 0); 
    nCount = dwBytes * Height();
    dwBytesSaved += nCount; 
    file.WriteHuge(m_pBits, nCount);

    return dwBytesSaved;
}


/*************************************************************************

  Function:  Read (CFile&)

   Purpose:  Reads in the specified DIB file into a global chunk of
             memory.

   Returns:  Number of read bytes.

*************************************************************************/
DWORD CDIB::Read(CFile& file)
{
    // Ensures no memory leaks will occur
    Free();
    
    BITMAPFILEHEADER bmfHeader;

    // Go read the DIB file header and check if it's valid.
    if (file.Read((LPSTR)&bmfHeader, sizeof(bmfHeader)) != sizeof(bmfHeader))
        return 0;
    if (bmfHeader.bfType != DIB_HEADER_MARKER)
        return 0;
    DWORD dwReadBytes = sizeof(bmfHeader);

    // Allocate memory for DIB
    m_pBMI = (LPBITMAPINFO)GlobalAllocPtr(GHND, bmfHeader.bfOffBits-sizeof(BITMAPFILEHEADER) + 256*sizeof(RGBQUAD));
    if (m_pBMI == 0)
        return 0;

    // Read header.
    if (file.Read(m_pBMI, bmfHeader.bfOffBits-sizeof(BITMAPFILEHEADER)) != (UINT)(bmfHeader.bfOffBits-sizeof(BITMAPFILEHEADER)))
    {
        GlobalFreePtr(m_pBMI);
        m_pBMI = NULL;
        return 0;
    }
    dwReadBytes += bmfHeader.bfOffBits-sizeof(BITMAPFILEHEADER);

    DWORD dwLength = file.GetLength();
    // Go read the bits.
    m_pBits = (LPBYTE)GlobalAllocPtr(GHND, dwLength - bmfHeader.bfOffBits);
    if (m_pBits == 0)
    {
        GlobalFreePtr(m_pBMI);
        m_pBMI = NULL;
        return 0;
    }
    
    if (file.ReadHuge(m_pBits, dwLength-bmfHeader.bfOffBits) != (dwLength - bmfHeader.bfOffBits))
    {
        GlobalFreePtr(m_pBMI);
        m_pBMI = NULL;
        GlobalFreePtr(m_pBits);
        m_pBits = NULL;
        return 0;
    }
    dwReadBytes += dwLength - bmfHeader.bfOffBits;
    CreatePalette();
    return dwReadBytes;
}

//////////////////////////////////////////////////////////////////////////
//// Clipboard support

//---------------------------------------------------------------------
//
// Function:   CopyToHandle
//
// Purpose:    Makes a copy of the DIB to a global memory block.  Returns
//             a handle to the new memory block (NULL on error).
//
// Returns:    Handle to new global memory block.
//
//---------------------------------------------------------------------

HGLOBAL CDIB::CopyToHandle()
{
    CSharedFile file;
    try
    {
        if (Save(file)==0)
            return 0;
    }
    catch (CFileException* e)
    {
        e->Delete();
        return 0;
    }
        
    return file.Detach();
}

//---------------------------------------------------------------------
//
// Function:   ReadFromHandle
//
// Purpose:    Initializes from the given global memory block.  
//
// Returns:    Number of read bytes.
//
//---------------------------------------------------------------------

DWORD CDIB::ReadFromHandle(HGLOBAL hGlobal)
{
    CSharedFile file;
    file.SetHandle(hGlobal, FALSE);
    DWORD dwResult = Read(file);
    file.Detach();
    return dwResult;
}

//////////////////////////////////////////////////////////////////////////
//// Serialization support

void CDIB::Serialize(CArchive& ar) 
{
    CFile* pFile = ar.GetFile();
    ASSERT(pFile != NULL);
    if (ar.IsStoring())
    {   // storing code
        Save(*pFile);
    }
    else
    {   // loading code
        Read(*pFile);
    }
}

//////////////////////////////////////////////////////////////////////////
//// Resource support

BOOL CDIB::ReadFromResource (UINT uResID)
{
    HINSTANCE   hInst   = AfxGetInstanceHandle();
    CString     cstrResName;
    //
    // Find the bitmap resource
    //
    cstrResName.Format ("#%d",uResID);
    HRSRC hResInfo = FindResource(hInst, cstrResName, "MY_BITMAP");
    if (!hResInfo)
    {
        TRACE("CDIB: Failed to find resource %d.\n", uResID);
        return FALSE;
    }
    //
    // Load the bitmap resource
    //
    HANDLE hRes = LoadResource(hInst, hResInfo);
    if (!hRes)
    {
        TRACE("CDIB: Failed to load resource %d.\n", uResID);
        return FALSE;
    }
    //
    // Lock the bitmap resource
    //
    PBYTE lpRes = (PBYTE)LockResource(hRes);
    if (!lpRes)
    {
        TRACE("CDIB: Failed to lock resource.\n");
        FreeResource(hRes);
        return FALSE;
    }
    DWORD dwResSize = SizeofResource (hInst, hResInfo);
    CMemFile memFile (lpRes, dwResSize);
    DWORD dwBytesRead = Read (memFile);
    FreeResource (hRes);
    return (dwResSize == dwBytesRead);
}       

//////////////////////////////////////////////////////////////////////////
//// Misc support

BOOL CDIB::CreateEmpty (UINT uXSize, UINT uYSize)
{
    Free ();

    m_pBMI = (LPBITMAPINFO)GlobalAllocPtr(GHND, sizeof(BITMAPINFOHEADER) + 256*sizeof(RGBQUAD));
    if (NULL == m_pBMI)
        return FALSE;
    // Clear palette to black
    memset (m_pBMI->bmiColors, 120, 256 * sizeof(RGBQUAD));
    DWORD dwBmBitsSize = WIDTHBYTES(uXSize*((DWORD)8)) * uYSize;

    m_pBMI->bmiHeader.biSize = sizeof (BITMAPINFOHEADER);
    m_pBMI->bmiHeader.biWidth = uXSize;
    m_pBMI->bmiHeader.biHeight = uYSize;
    m_pBMI->bmiHeader.biPlanes = 1;
    m_pBMI->bmiHeader.biBitCount = 8;
    m_pBMI->bmiHeader.biCompression = BI_RGB;
    m_pBMI->bmiHeader.biSizeImage = dwBmBitsSize;
    m_pBMI->bmiHeader.biXPelsPerMeter = 100;
    m_pBMI->bmiHeader.biYPelsPerMeter = 100;
    m_pBMI->bmiHeader.biClrUsed = 256;
    m_pBMI->bmiHeader.biClrImportant = 256;
    m_pBits = (LPBYTE)GlobalAllocPtr(GHND, dwBmBitsSize);
    if (NULL == m_pBits)
    {
        GlobalFreePtr (m_pBMI);
        return FALSE;
    }
    // Clear image to black
    memset (m_pBits, 0, dwBmBitsSize);
    return TRUE;
}

DWORD CDIB::CalcImageSize()
{
    if ((m_pBMI->bmiHeader.biCompression == BI_RLE8) || (m_pBMI->bmiHeader.biCompression == BI_RLE4))
    {
        // It's an RLE bitmap, we can't calculate size, so trust the
        // biSizeImage field
    }
    else
    {
        DWORD dwBmBitsSize;  // Size of Bitmap Bits only
        // It's not RLE, so size is Width (DWORD aligned) * Height
        dwBmBitsSize = WIDTHBYTES((m_pBMI->bmiHeader.biWidth)*((DWORD)m_pBMI->bmiHeader.biBitCount)) * 
                                   m_pBMI->bmiHeader.biHeight;
        // Now, since we have calculated the correct size, why don't we
        // fill in the biSizeImage field (this will fix any .BMP files which
        // have this field incorrect).
        m_pBMI->bmiHeader.biSizeImage = dwBmBitsSize;
    }
    return m_pBMI->bmiHeader.biSizeImage;
}


void CDIB::FillSolidColor (BYTE R, BYTE G, BYTE B)
{
    DWORD dwImageSize = CalcImageSize();
    if (256<=NumColors())
    {   // Palette based DIB
        PIXEL ColorInd = PIXEL(m_pPalette->GetNearestPaletteIndex(RGB(R, G, B)));
        memset (m_pBits, ColorInd, dwImageSize);
    }
    // We don't support non-palette DIBs for now
}

BOOL CDIB::CopyRectFrom (
    CDIB *pSrcDIB, 
    int SrcX, 
    int SrcY, 
    UINT SrcWidth, 
    UINT SrcHeight,
    int DstX, int DstY
)
{
    if ((DstX >= int(Width())) || (DstY >= int(Height())))
        return FALSE;     // Completely out of region
    if ((DstX + int(SrcWidth) < 0) || (DstY + int(SrcHeight)) < 0)
        return FALSE;     // Completely out of region
    if (NumColors() != pSrcDIB->NumColors())
        return FALSE;     // Must have same color depth
    if (NumColors() > 256)
        return FALSE;     // We currently don't support DIBs without palettes
    if (DstX < 0)
    {
        SrcWidth += DstX;
        SrcX -= DstX;
        DstX = 0;
    }
    if (DstY < 0)
    {
        SrcHeight += DstY;
        SrcY -= DstY;
        DstY = 0;
    }

    DWORD dwSrcDIBWidth = pSrcDIB->Width();
    DWORD dwSrcDIBHeight = pSrcDIB->Height();
    DWORD dwMyWidth = Width();

    ASSERT ((SrcX < int(dwSrcDIBWidth)) &&
            (SrcY < int(dwSrcDIBHeight)));
    ASSERT ((SrcX >= 0) && (SrcY >= 0));

    SrcWidth = min (SrcWidth, UINT(dwMyWidth) - DstX);
    SrcWidth = min (SrcWidth, UINT(dwSrcDIBWidth) - SrcX);
    SrcHeight = min (SrcHeight, UINT(Height()) - DstY);
    SrcHeight = min (SrcHeight, UINT(dwSrcDIBHeight) - SrcY);

    PPIXEL pSrc = pSrcDIB->FindPixel(SrcX, SrcY);
    PPIXEL pDst = FindPixel(DstX, DstY);

    DWORD SrcInc = WIDTHBYTES(dwSrcDIBWidth << 3);
    DWORD DstInc = WIDTHBYTES(dwMyWidth << 3);

    for (UINT uCurHeight = 0; uCurHeight < SrcHeight; uCurHeight++)
    {
        memcpy (pDst, pSrc, SrcWidth);
        pDst+= DstInc;
        pSrc+= SrcInc;
    }
    return TRUE;
}

BOOL CDIB::CopyFrom (CDIB *pSrc)
{
    DWORD dwWidth = Width(),
          dwHeight= Height();
    if (NULL == pSrc)
        return FALSE;   // Bad Src
    if ((dwWidth != pSrc->Width()) ||
        (dwHeight != pSrc->Height()) ||
        (NumColors() != pSrc->NumColors())
       )
        return FALSE;   // Bad size or color depth
    memcpy (m_pBits, pSrc->m_pBits, m_pBMI->bmiHeader.biSizeImage);
    return TRUE;
}

void CDIB::PasteCKRect (CDIB *Dib, int x, int y, PIXEL ColorKey)
{
    // Clip Rect
    int px,py,dx,dy,
        mw = Width(),         // My width
        mh = Height(),        // My height
        ow = Dib->Width(),    // Other's width
        oh = Dib->Height();   // Other's height
    if (x >= 0)
    {
        px = x;
        dx = ((x + ow) < mw) ? ow : mw - x;
    }
    else
    {
        px = 0;
        dx = x + (((x + ow) < mw) ? ow : mw - x);
    }
    if (y >= 0)
    {
        py = y;
        dy = ((y + oh) < mh) ? oh : mh - y;
    }
    else
    {
        py = 0;
        dy = y + (((y + oh) < mh) ? oh : mh - y);
    }    
    // If Nothing to Paste return
    if ( (dx <= 0) || (dy <= 0) )
        return;
    // Prepare Buffer Addresses
    PPIXEL src = Dib->FindPixel (px-x, py-y);
    PPIXEL dst = FindPixel (px, py);
    DWORD SrcInc = WIDTHBYTES(ow << 3);
    DWORD DstInc = WIDTHBYTES(mw << 3);
    // Do Paste
    while ( dy-- )
    {
        for ( int i=0; i<dx; i++ )
            if (src[i] != ColorKey)
                dst[i]=src[i];
        src+=SrcInc;
        dst+=DstInc;
    }
}

void CDIB::FillRect (int x, int y, int w, int h, int R, int G, int B )
{
    // Clip Rect
    int iWidth = int(Width());
    int iHeight = int(Height());
    int px=(x>=0) ? x : 0;
    int py=(y>=0) ? y : 0;
    int dx=((x+w)<iWidth) ? w : iWidth-x;
    int dy=((y+h)<iHeight) ? h : iHeight-y;
    dx=(x>=0) ? dx : dx + x;
    dy=(y>=0) ? dy : dy + y;
    // If Nothing to Fill return
    if ( (dx<=0) || (dy<=0) )
        return;
    // Prepare Buffer Address
    PPIXEL dst = FindPixel (px, py);
    PIXEL ColorInd = PIXEL(m_pPalette->GetNearestPaletteIndex(RGB(R, G, B)));
    DWORD DstInc = WIDTHBYTES(iWidth << 3);
    // Do Fill
    while ( dy-- )
    {
        memset (dst, ColorInd, dx);
        dst+=DstInc;
    }
}

BOOL CDIB::CreateRotated (CDIB *pSrc, UINT uAngle /* 0, 90, 180 or 270 only */, BOOL bFlipHoriz, BOOL bFlipVert)
{
    ASSERT (uAngle == 0 || uAngle == 90 || uAngle == 180 || uAngle == 270);

    UINT uSrcWidth = pSrc->Width();
    UINT uSrcHeight= pSrc->Height();
    UINT uDstWidth;
    UINT uDstHeight;

    if (uAngle == 180)
        return CreateRotated (pSrc, 0, !bFlipHoriz, !bFlipVert);
    if (uAngle == 270)
        return CreateRotated (pSrc, 90, !bFlipHoriz, !bFlipVert);

    BOOL bSuccess;
    if (uAngle == 0)
    {
        bSuccess = CreateEmpty (uSrcWidth, uSrcHeight);
        uDstWidth = uSrcWidth;
        uDstHeight = uSrcHeight;
    }
    else
    {
        bSuccess = CreateEmpty (uSrcHeight, uSrcWidth);
        uDstWidth = uSrcHeight;
        uDstHeight = uSrcWidth;
    }
    if (!bSuccess)
        return FALSE;   // Can't create

    for (int x = 0; x < int(uDstWidth); x++)
        for (int y = 0; y < int(uDstHeight); y++)
            switch (uAngle)
            {
                case   0:
                    ColorAt (x, y) = pSrc->ColorAt ((bFlipHoriz ? uDstWidth-1-x : x), (bFlipVert ? uDstHeight-1-y : y));
                    break;
                case 90:
                    ColorAt (x, y) = 
                        pSrc->ColorAt ((bFlipVert  ? uSrcWidth-1-y : y), (bFlipHoriz ? x : ( uSrcHeight-1-x )));
                    break;
                default:
                    ASSERT (FALSE);
            }
    return TRUE;
}                        

BOOL CDIB::CopyPalette (CDIB *pSrc)
{
    if ((NULL == pSrc) || (NULL == pSrc->m_pPalette))
        return FALSE;   // Bad source or source has no palette
    BOOL bRes = CopyPalette (pSrc->m_pPalette);
    if (bRes)
    {
        memcpy (m_pBMI->bmiColors, pSrc->m_pBMI->bmiColors, 256 * sizeof (RGBQUAD));
    }
    return bRes;
}

BOOL CDIB::CopyPalette (CPalette *pPal)
{
    if (NULL == pPal)
        return FALSE;
    if (m_pPalette)
    {
        m_pPalette->DeleteObject();
        delete m_pPalette;
        m_pPalette = NULL;
    }
    UINT uNumColors = pPal->GetEntryCount();
    // allocate memory block for logical palette
    HANDLE hLogPal = ::GlobalAlloc(GHND, sizeof(LOGPALETTE) + sizeof(PALETTEENTRY)*uNumColors);
    // if not enough memory, clean up and return NULL
    if (hLogPal == 0)
        return FALSE;
    LPLOGPALETTE lpPal = (LPLOGPALETTE)::GlobalLock((HGLOBAL)hLogPal);
    // set version and number of palette entries
    lpPal->palVersion = PALVERSION;
    lpPal->palNumEntries = (WORD)uNumColors;
    UINT uCount = pPal->GetPaletteEntries(0, uNumColors, lpPal->palPalEntry);
    if (uCount != uNumColors)
    {
        ::GlobalUnlock((HGLOBAL) hLogPal);
        ::GlobalFree((HGLOBAL) hLogPal);
        return FALSE;
    }
    m_pPalette = new CPalette;
    BOOL bResult = m_pPalette->CreatePalette(lpPal);
    ::GlobalUnlock((HGLOBAL) hLogPal);
    ::GlobalFree((HGLOBAL) hLogPal);
    return bResult;
}

BOOL CDIB::GetPaletteFromResourceBitmap (UINT uResID)
{
    CDIB tmpDIB;

    if (!tmpDIB.ReadFromResource(uResID))
        return FALSE;
    return CopyPalette (&tmpDIB);
}

