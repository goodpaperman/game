/*****************************************************************************
*                                                                             
*   Dib.inl
*                                                                             
*   Electrical Engineering Faculty - Software Lab                             
*   Spring semester 1998                                                      
*                                                                             
*   Tanks game                                                                
*                                                                             
*   Contents: Inline functions implementations.
*                                                                             
*   Authors: Eran Yariv - 28484475                                           
*            Moshe Zur  - 24070856                                           
*                                                                            
*                                                                            
*   Date: 23/09/98                                                           
*                                                                            
******************************************************************************/



/*************************************************************************
 *
 * IsValid()
 *
 * Return Value:
 *
 * BOOL            - Is the DIB initialized?
 *
 * Description:
 *
 * This function test for a valid DIB. Invalid DIBs cannot be used
 *
 ************************************************************************/
inline BOOL CDIB::IsValid() const 
{ 
    return (m_pBMI != NULL); 
}

/*************************************************************************
 *
 * Invalidate()
 *
 * Return Value:
 *
 * void
 *
 * Description:
 *
 * Invalidates a DIB - makes it unusable.
 *
 ************************************************************************/
inline void CDIB::Invalidate() 
{ 
    Free();
}

/*************************************************************************
 *
 * FindPixel()
 *
 * Return Value:
 *
 * PPIXEL            - Pixel pointer into bytes structure
 *
 * Description:
 *
 * This function returns the correct pixel in the bitmap structure
 * at a given point.
 *
 ************************************************************************/
inline PPIXEL CDIB::FindPixel (UINT x, UINT y)       const
{
    return PPIXEL(&m_pBits[x + y * WIDTHBYTES((m_pBMI->bmiHeader.biWidth) << 3)]);
}

inline CDIB::CDIB()
{
    m_pBMI = NULL;
    m_pBits = NULL;
    m_pPalette = NULL;
}

inline CDIB::~CDIB()
{
    Free();
}

/*************************************************************************
 *
 * Width()
 *
 * Return Value:
 *
 * DWORD            - width of the DIB
 *
 * Description:
 *
 * This function gets the width of the DIB from the BITMAPINFOHEADER
 * width field 
 *
 ************************************************************************/
inline DWORD CDIB::Width() const
{
    if (!m_pBMI)
        return 0;

    /* return the DIB width */
    return m_pBMI->bmiHeader.biWidth;
}


/*************************************************************************
 *
 * Height()
 *
 * Return Value:
 *
 * DWORD            - height of the DIB
 *
 * Description:
 *
 * This function gets the height of the DIB from the BITMAPINFOHEADER
 * height field 
 *
 ************************************************************************/

inline DWORD CDIB::Height() const
{
    if (!m_pBMI)
        return 0;
    
    /* return the DIB height */
    return m_pBMI->bmiHeader.biHeight;
}

/*************************************************************************
 *
 * Size()
 *
 * Return Value:
 *
 * CSize            - Size of the DIB
 *
 * Description:
 *
 * This function gets the size of the DIB from the BITMAPINFOHEADER
 * height and width field 
 *
 ************************************************************************/
inline CSize CDIB::Size() const
{
    return CSize (Width(), Height());
}

/*************************************************************************
 *
 * PaletteSize()
 *
 * Return Value:
 *
 * WORD             - size of the color palette of the DIB
 *
 * Description:
 *
 * This function gets the size required to store the DIB's palette by
 * multiplying the number of colors by the size of an RGBQUAD 
 *
 ************************************************************************/
inline WORD CDIB::PaletteSize() const
{
    if (!m_pBMI)
        return 0;
    return WORD(NumColors() * sizeof(RGBQUAD));
}

inline PIXEL & CDIB::ColorAt (UINT uX, UINT uY)
{
    ASSERT (uX < Width());
    ASSERT (uY < Height());
    ASSERT (NumColors() <= 256);
    return *FindPixel(uX, uY);
}


#ifdef _DEBUG
inline void CDIB::Dump(CDumpContext& dc) const
{
    CObject::Dump(dc);
}
#endif



