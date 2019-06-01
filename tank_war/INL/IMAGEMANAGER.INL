/*****************************************************************************
*                                                                             
*   ImageManager.inl
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
inline CImageManager::CImageManager () :
    m_bCreated (FALSE)
{}

#ifdef _DEBUG

inline void 
CImageManager::AssertValidHImage (HIMAGE &himg)
{
    ASSERT (himg.bImageType < IMG_LAST_INDEX);
    ASSERT (himg.bSubImgID < m_ImagesInfo[himg.bImageType].uNumSubImages);
    ASSERT (himg.bFrame < m_ImagesInfo[himg.bImageType].SubImages[himg.bSubImgID].uNumBitmaps);
}
#endif

inline HIMAGE 
CImageManager::GetImage (ImageType typ)
{
    ASSERT (m_bCreated);
        // Check for valid type
    ASSERT (typ >= 0 && typ < IMG_LAST_INDEX);
    HIMAGE res = {BYTE(typ),    // Type of image
                  0,            // Sub-image ID
                  0,            // Frame number
                  FALSE,        // Animation ended
                  0,            // Animation start time
                  NULL};        // No overlay
    return res;
}    
    
inline BOOL 
CImageManager::ImageEnded (HIMAGE &himg)
{
    ASSERT (m_bCreated);
    //ASSERT_VALID_HIMAGE (himg); // Costly call
    // If the following assertion fails, the function was called for a non-animating 
    // image or a repeatative animation image.
    ASSERT (!m_ImagesInfo[himg.bImageType].SubImages[himg.bSubImgID].bCyclicAnim &&  // Animation is acyclic
            m_ImagesInfo[himg.bImageType].SubImages[himg.bSubImgID].dwFrameDelay >0); // and has a limited time
    return himg.bAnimEnded;
}

inline void 
CImageManager::GetImageSize (HIMAGE &himg, CSize &size)
{
    ASSERT (m_bCreated);
    //ASSERT_VALID_HIMAGE (himg); // Costly call
    size = m_ImagesInfo[himg.bImageType].SubImages[himg.bSubImgID].Bitmaps[himg.bFrame].pDIB->Size();
}

inline void 
CImageManager::GetImageOffset (HIMAGE &himg, CPoint &offset)
{
    ASSERT (m_bCreated);
    //ASSERT_VALID_HIMAGE (himg); // Costly call
    BitmapType *pCurBitmap = &m_ImagesInfo[himg.bImageType].SubImages[himg.bSubImgID].Bitmaps[himg.bFrame];
    offset.x = pCurBitmap->iXOffset;
    offset.y = pCurBitmap->iYOffset;
}


inline void 
CImageManager::RotateImage (HIMAGE &himg, UINT uRotationIndex)
{   // Assuming sub-images are ordered by degrees, comprising a full circle.
    ASSERT (m_bCreated);
    //ASSERT_VALID_HIMAGE (himg); // Costly call
    ASSERT (m_ImagesInfo[himg.bImageType].uNumSubImages > uRotationIndex);   // Can't rotate with only one sub-image
    himg.bSubImgID = BYTE(uRotationIndex); 
}

inline CDIB *
CImageManager::ExposeDIB (HIMAGE &himg)
{
    ASSERT (m_bCreated);
    ASSERT_VALID_HIMAGE (himg); // Costly call
    return m_ImagesInfo[himg.bImageType].SubImages[himg.bSubImgID].Bitmaps[himg.bFrame].pDIB;
}

inline CBitMask *
CImageManager::ExposeBitMask(HIMAGE &himg)
{
    ASSERT (m_bCreated);
    ASSERT_VALID_HIMAGE (himg); // Costly call
    return m_ImagesInfo[himg.bImageType].SubImages[himg.bSubImgID].Bitmaps[himg.bFrame].pBitMask;
}

inline void
CImageManager::LoadGameOverBitmaps ()
{
    LoadSingleImage (IMG_GAMEOVER);
}

inline void
CImageManager::ReleaseGameOverBitmaps ()
{
    ReleaseSingleImage (IMG_GAMEOVER);
}

inline void
CImageManager::SetOverlay (HIMAGE &img, HIMAGE *pimg)
{
    img.pOverlay = pimg;
}
    
inline HIMAGE *
CImageManager::GetOverlay(HIMAGE &img)
{
    return img.pOverlay;
}

