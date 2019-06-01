/*****************************************************************************
*                                                                             
*   ImageManager.cpp                                                            
*                                                                             
*   Electrical Engineering Faculty - Software Lab                             
*   Spring semester 1998                                                      
*                                                                             
*   Tanks game                                                                
*                                                                             
*   Module description: Implements the image manager object. Handle the game
*                       objects' images - load from the resource, calculate
*                       image bitmask, animate, and release the images at the 
*                       end.
*                       
*                                                                             
*   Authors: Eran Yariv - 28484475                                           
*            Moshe Zur  - 24070856                                           
*                                                                            
*                                                                            
*   Date: 23/09/98                                                           
*                                                                            
******************************************************************************/
#include <stdafx.h>
#include <ImageManager.h>
#include <GameConsts.h>
#include <BitmapTables.h>

void CImageManager::LoadImages ()
{
    ASSERT (!m_bCreated); // Called only once
    for (int uImgID=0; uImgID < IMG_LAST_INDEX; uImgID++)
        if (IMG_GAMEOVER != uImgID) // Conserve memory
            LoadSingleImage (ImageType(uImgID));
    m_bCreated=TRUE;
}            


void CImageManager::LoadSingleImage (ImageType uImgID)
{
    ASSERT (uImgID >=0 && uImgID < IMG_LAST_INDEX);

    // Traverse all images
    for (UINT uSumImgID = 0; 
         uSumImgID < m_ImagesInfo[uImgID].uNumSubImages; 
         uSumImgID++) 
        // Traverse all sub-images
        for (UINT uFrameID = 0;
             uFrameID < m_ImagesInfo[uImgID].SubImages[uSumImgID].uNumBitmaps;
             uFrameID++) 
        {
            // Traverse all bitmaps
            CDIB *pDIB = new CDIB();
            //TRACE("new dib %08x\n", pDIB);
            m_ImagesInfo[uImgID].SubImages[uSumImgID].Bitmaps[uFrameID].pDIB = pDIB;
            if (!pDIB->ReadFromResource(m_ImagesInfo[uImgID].SubImages[uSumImgID].Bitmaps[uFrameID].uBitmapID))
                AfxThrowResourceException();    // Can't load resource bitmap
            // Create bit mask if needed (pointer is initialized to a non-null value):
            if (m_ImagesInfo[uImgID].SubImages[uSumImgID].Bitmaps[uFrameID].pBitMask) {
                m_ImagesInfo[uImgID].SubImages[uSumImgID].Bitmaps[uFrameID].pBitMask = 
                    new CBitMask(*pDIB);
                //TRACE("new bitmask %08x\n", m_ImagesInfo[uImgID].SubImages[uSumImgID].Bitmaps[uFrameID].pBitMask);
            }
        }
}

void CImageManager::ReleaseSingleImage (ImageType uImgID)
{
    ASSERT (uImgID >=0 && uImgID < IMG_LAST_INDEX);
    // Traverse all images
    for (UINT uSumImgID = 0; 
         uSumImgID < m_ImagesInfo[uImgID].uNumSubImages; 
         uSumImgID++) 
        // Traverse all sub-images
        for (UINT uFrameID = 0;
             uFrameID < m_ImagesInfo[uImgID].SubImages[uSumImgID].uNumBitmaps;
             uFrameID++) 
        {
            // Traverse all bitmaps
            CDIB *&pDIB = 
				m_ImagesInfo[uImgID].SubImages[uSumImgID].Bitmaps[uFrameID].pDIB;
            if (NULL != pDIB)
            {
                delete pDIB;
                pDIB = NULL;
            }
            CBitMask *&pBitMask = 
				m_ImagesInfo[uImgID].SubImages[uSumImgID].Bitmaps[uFrameID].pBitMask;
            if (NULL != pBitMask)
            {
                delete pBitMask;
                pBitMask = NULL;
            }
        }
}

void CImageManager::ReloadMap ()
{
    ASSERT (m_bCreated); // Called only once
    CDIB *pDIB = m_ImagesInfo[IMG_BOARD].SubImages[0].Bitmaps[0].pDIB;
    ASSERT (NULL != pDIB);

    CDIB tmpDIB;
    BOOL bSuccess = tmpDIB.ReadFromResource (IDB_BOARD);
    ASSERT (bSuccess);
    if (!bSuccess)
        return;
    bSuccess = pDIB->CopyFrom (&tmpDIB);
    ASSERT (bSuccess);
}

CImageManager::~CImageManager ()
{
    if (!m_bCreated)
        return; // Never created
    for (int uImgID=IMG_TANK0; uImgID < IMG_LAST_INDEX; uImgID++)
        ReleaseSingleImage (ImageType(uImgID));
    m_bCreated=FALSE;
}

void CImageManager::DisplayImage (HIMAGE &himg, CDIB *pDstDIB, CPoint &pos)
{
    ASSERT (m_bCreated);
    ASSERT_VALID_HIMAGE (himg); // Costly call
    BitmapType *pCurBitmap = &m_ImagesInfo[himg.bImageType].SubImages[himg.bSubImgID].Bitmaps[himg.bFrame];
    pDstDIB->PasteCKRect (pCurBitmap->pDIB, 
                          pos.x + pCurBitmap->iXOffset, 
                          pos.y + pCurBitmap->iYOffset, 
                          TRANSP_COLOR);
    if (himg.pOverlay != NULL)
        DisplayImage (*himg.pOverlay, pDstDIB, pos);
}

void
CImageManager::UpdateImage (HIMAGE &himg, BOOL &bImageChanged)
{
    HIMAGE himgOldCopy = himg;

    ASSERT (m_bCreated);

    ASSERT_VALID_HIMAGE (himg); // Costly call

    DWORD dwFrameDelay = m_ImagesInfo[himg.bImageType].SubImages[himg.bSubImgID].dwFrameDelay;

    if (dwFrameDelay != 0) { 
        // Not a static image 

        DWORD dwNow = GetTickCount();

        if (0 == himg.dwStartFrameTime)  // Animation isn't started yet
        {
            himg.dwStartFrameTime = dwNow;   // Animation just started
        }
        else
        {   // Animation is on the way, calc new frame

            ASSERT (himg.dwStartFrameTime <= dwNow);
            int iNewFrame = int ((dwNow - himg.dwStartFrameTime) / dwFrameDelay);
            UINT uNumBitmaps = m_ImagesInfo[himg.bImageType].SubImages[himg.bSubImgID].uNumBitmaps;
            if (UINT(iNewFrame) >= uNumBitmaps)
            {   // Overlap occured
                if (m_ImagesInfo[himg.bImageType].SubImages[himg.bSubImgID].bCyclicAnim)
                {
                    // Animation is cyclic => simply restart
                    iNewFrame %= uNumBitmaps;
                    himg.bFrame = BYTE(iNewFrame);
                }
                else 
                {  // Animation is not cyclic => Display last frame and set end flag
                    himg.bFrame = BYTE(uNumBitmaps - 1);
                    himg.bAnimEnded = TRUE;
                }
            } // end of overlap
            else
                himg.bFrame = BYTE(iNewFrame);
        } // End of animation on the way case
        //TRACE ("Animation: timegap = %d, delay = %d, frame = %d\n",dwNow - himg.dwStartFrameTime, dwFrameDelay, himg.bFrame);
    } // End of animation image case
    if (FALSE == bImageChanged &&   // The calling object thinks its image hasn't changed
        0 != memcmp (&himgOldCopy, &himg, sizeof (HIMAGE))) // but the image handle has changed
        bImageChanged = TRUE;
}
