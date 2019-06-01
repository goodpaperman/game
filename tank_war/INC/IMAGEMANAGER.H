/*****************************************************************************
*                                                                             
*   ImageManager.h
*                                                                             
*   Electrical Engineering Faculty - Software Lab                             
*   Spring semester 1998                                                      
*                                                                             
*   Tanks game                                                                
*                                                                             
*   Module description: Handle the images of all the game objects.
*                       
*                                                                             
*   Authors: Eran Yariv - 28484475                                           
*            Moshe Zur  - 24070856                                           
*                                                                            
*                                                                            
*   Date: 23/09/98                                                           
*                                                                            
******************************************************************************/
#ifndef _IMAGE_MANAGER_H
#define _IMAGE_MANAGER_H

#include <DIB.h>
#include <BitMask.h>

#define HIMAGE CImageManager::ImageIdentifierType

class CImageManager 
{
public:

typedef enum {  IMG_TANK0 = 0,      //  Tank of type 0 (White) 
                                    //      15 degrees rotations (24 bitmaps)
                IMG_TANK1,          //  Tank of type 1 (Blue)
                                    //      15 degrees rotations (24 bitmaps)
                IMG_TANK2,          //  Tank of type 2 (Red)
                                    //      15 degrees rotations (24 bitmaps)
                IMG_TANK3,          //  Tank of type 3 (Green)
                                    //      15 degrees rotations (24 bitmaps)
                IMG_TANK_EXPLODE,   //  Tank explosion 
                                    //      (16 frames)
                IMG_SHELL,          //  Sheel image
                                    //      15 degrees rotations (24 bitmaps)
                IMG_SHELL_EXPLODE,  //  Shell explosion 
                                    //      (28 frames)
                IMG_BULLET,         //  Bullet 
                                    //      (1 bitmap)    
                IMG_BOARD,          //  Empty board
                                    //      (1 bitmap)
                IMG_MINE,           //  Mine 
                                    //      (2 frames ,cyclic animation, 1.00 seconds per cycle)
                IMG_BONUS_SHELLS,   //  Shells bonus 
                                    //      (20 frames, cyclic animation, 2.00 seconds per cycle)
                IMG_BONUS_BULLETS,  //  Bullets bonus 
                                    //      (20 frames, cyclic animation, 2.00 seconds per cycle)
                IMG_BONUS_MINES,    //  Mines bonus 
                                    //      (9 frames, cyclic animation, 2.00 seconds per cycle)
                IMG_BONUS_BOMBER,   //  Areal support bonus 
                                    //      (20 frames, cyclic animation, 2.00 seconds per cycle)
                IMG_BONUS_FIRERATE, //  Faster fire rate bonus 
                                    //      (20 frames, cyclic animation, 2.00 seconds per cycle)
                IMG_BONUS_SHIELD,   //  Shield repair bonus 
                                    //      (9 frames, cyclic animation, 2.00 seconds per cycle)
                IMG_GAMEOVER,       //  Game-over sign
                                    //      (60 frames, cyclic animation, 4.00 seconds per cycle)
                IMG_BOMBER,         //  Bomber plane
                                    //      15 degrees rotations (24 bitmaps)
                IMG_BOMB,           //  Dropping bomb
                                    //      (11 frames, 2.00 seconds)
                IMG_TANK_ZOMBIE,    //  Zombie tank (Gray)
                                    //      15 degrees rotations (24 bitmaps)
                IMG_LAST_INDEX      //  For compilation purposes (keep it last)
            } ImageType;

    typedef struct _ImageIdentifierType {
        BYTE  bImageType;        // Type of image
        BYTE  bSubImgID;         // Sub image ID
        BYTE  bFrame;            // Animation frame
        BOOL  bAnimEnded;        // Animation is expired
        DWORD dwStartFrameTime;  // Start time of animation
        struct _ImageIdentifierType *pOverlay;  // Pointer to optional overlay
    } ImageIdentifierType;

    CImageManager ();
    virtual ~CImageManager ();

    void   LoadImages();        // Creation - must be called only once and after the main window
                                //            is constructed !!!
    HIMAGE      GetImage (ImageType);
    void        UpdateImage (HIMAGE &, BOOL &);
    void        DisplayImage (HIMAGE &himg, CDIB *pDstDIB, CPoint &pos);
    void        RotateImage (HIMAGE &, UINT uAngle);
    BOOL        ImageEnded (HIMAGE &);
    CDIB       *ExposeDIB (HIMAGE &);
    void        GetImageSize (HIMAGE &, CSize &);
    void        GetImageOffset (HIMAGE &, CPoint &);
    CBitMask   *ExposeBitMask(HIMAGE &);
    void        SetOverlay (HIMAGE &, HIMAGE *p = NULL);
    HIMAGE     *GetOverlay(HIMAGE &);
    void        ReloadMap();        // Reload map image from file (for a new game session)

    // The following two functions are here for memory consumption purposes:

    void        LoadGameOverBitmaps ();
    void        ReleaseGameOverBitmaps ();
private:

/*  O.K. - We hold a list of images (m_ImagesInfo). Each item on the list
    is of ImageInfoType which is itself a list of subimages (SubImageType).
    For example, a tank image has 24 sub images (different rotation angles).
    Every subimage can be one or more frames of animation.
    So, every sub image holds animation info (time, cyclic etc.) and a list of bitmaps.
    Every bitmap holds the resource bitmap ID and a DIB buffer for the data.
    The list is static and is composed at compile-time.

    Only the DIB buffer in the list is dynamic and is initialized and loaded 
    on run-time construction.

    The image handle (HIMAGE) is actually a structure that points into the list.
    It holds the following data:
        1. Image index (Image type)
        2. Sub image index (for example, tank angle)
        3. Bitmap ID (animation frame)
        4. Animation expiration flag (for non-cyclic animation images)
        5. Last animation frame time


*/

typedef struct {
    UINT            uBitmapID;      // ID of resource bitmap
    CDIB           *pDIB;           // DIB data of bitmap
    CBitMask       *pBitMask;       // Bit mask of DIB 
    int             iXOffset,       // Offset from original point
                    iYOffset;
} BitmapType;

typedef struct {
    UINT            uNumBitmaps;    // Number of frames (bitmaps)
    BitmapType     *Bitmaps;        // Bitmaps of the sub-image
    BOOL            bCyclicAnim;    // Is the animation cyclic (repeatative)?
    DWORD           dwFrameDelay;   // Time gap (in millisecs) between the animation frames (0 = static image)
} SubImageType;

typedef struct {
    UINT            uNumSubImages;  // Number of sub-images
    SubImageType   *SubImages;      // Sub-images
} ImageInfoType;

#ifdef _DEBUG
    void AssertValidHImage (HIMAGE &himg);
#endif

    void LoadSingleImage (ImageType);
    void ReleaseSingleImage (ImageType);

    BOOL                    m_bCreated;
    static ImageInfoType    m_ImagesInfo[IMG_LAST_INDEX];
    static BitmapType       m_Bitmaps[];
    static SubImageType     m_SubImages[];
};

#ifdef _DEBUG
    #define ASSERT_VALID_HIMAGE(h)      AssertValidHImage(h)
#else
    #define ASSERT_VALID_HIMAGE(h)
#endif

// Inline sections:
#include <ImageManager.inl>

#endif
