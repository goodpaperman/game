/*****************************************************************************
*                                                                             
*   SoundManager.cpp
*                                                                             
*   Electrical Engineering Faculty - Software Lab                             
*   Spring semester 1998                                                      
*                                                                             
*   Tanks game                                                                
*                                                                             
*   Module description: Manages the DirectSound objects.
*                       
*                                                                             
*   Authors: Eran Yariv - 28484475                                           
*            Moshe Zur  - 24070856                                           
*                                                                            
*                                                                            
*   Date: 23/09/98                                                           
*                                                                            
******************************************************************************/
#include "stdafx.h"
#include "mmsystem.h"
#include "SoundManager.h"

LPCSTR CSoundManager::m_aszSoundFileNames[] =
{
    "FIRE_BULLET", 
    "FIRE_SHELL", 
    "SHELL_EXPLODE",
    "MINE_EXPLODE",
    "TANK_EXPLODE",
    "DROP_BOMB",
    "PICK_BONUS",
    "GAME_OVER"
};

CSoundManager::CSoundManager() :
m_pDirectSound(NULL),
m_bMuteMode(FALSE)
{}

/*------------------------------------------------------------------------------

  Function: Init

  Purpose:  Initialize DirectSound objects.

  Input:    hWnd - Handle to main window

  Output:   Return TRUE if initializtion succeeded.

  Remarks:  
------------------------------------------------------------------------------*/
BOOL
CSoundManager::Init(HWND hWnd)
{
    ASSERT (! m_pDirectSound);
    if (m_pDirectSound)
        m_pDirectSound->Release();
    // Create DirectSound object
    if (DirectSoundCreate (NULL, &m_pDirectSound, NULL) == DS_OK)
    {
        // Set cooperative level for DirectSound. Normal means our
        // sounds will be silenced when our window loses input focus.
        if (m_pDirectSound->SetCooperativeLevel (hWnd, //AfxGetMainWnd()->m_hWnd,
                DSSCL_NORMAL) != DS_OK)
        {
            m_pDirectSound->Release();
            m_pDirectSound = NULL;
            return FALSE;
        }
    } else
    {
        m_pDirectSound = NULL;
        return FALSE;
    }
    // Load each wav files into DirectSound buffers:
    for (UINT i = 0; i < LAST_SOUND_TYPE; i++)
    {
        LoadWavToBuffer((SoundType)i);
    }

    return TRUE;
}

CSoundManager::~CSoundManager()
{
    // Release DirectSound interface (auto. release the sec. buffers):
    if (m_pDirectSound)
        m_pDirectSound->Release();
}

/*------------------------------------------------------------------------------

  Function: Play

  Purpose:  Play the 2nd buffer specified.

  Input:    ind - Index to 2nd buffer.

  Output:   None.

  Remarks:  Restarts to play buffer if called while buffer is playing.
------------------------------------------------------------------------------*/
void
CSoundManager::Play(CSoundManager::SoundType ind)
{
    // Check if play is anabled:
    if (m_bMuteMode || !m_pDirectSound)
        return;

    ASSERT (ind < LAST_SOUND_TYPE);
    if (ind >= LAST_SOUND_TYPE)
        return;

    // Play buffer(ind):
    if (m_aSoundBuffers[ind])
    {
//        m_aSoundBuffers[ind]->Stop();
        m_aSoundBuffers[ind]->SetCurrentPosition(0);
        m_aSoundBuffers[ind]->Play(0, 0, 0);
    }
}

/*------------------------------------------------------------------------------

  Function: LoadWavToBuffer

  Purpose:  Do all the dirty job - open the wav file and load it into its buffer.

  Input:    ind - Index to 2nd buffer.

  Output:   None.

  Remarks:  
------------------------------------------------------------------------------*/
void
CSoundManager::LoadWavToBuffer(CSoundManager::SoundType ind)
{
    HMMIO           hmmio       = NULL;
    MMCKINFO        mmckinfoParent;
    MMCKINFO        mmckinfoSubchunk;
    DWORD           dwFmtSize;
    LPCSTR          szFileName;
    HANDLE          hFormat     = NULL;
    WAVEFORMATEX   *pFormat     = NULL;
    HANDLE          hData       = NULL;
    HPSTR           lpData      = NULL;
    BYTE           *pbData      = NULL;
    BYTE           *pbData2     = NULL;
    DWORD           dwDataSize;
    WORD            wBlockSize;
    DWORD           dwLength;
    DWORD           dwLength2;

    m_aSoundBuffers[ind] = NULL;
    szFileName = m_aszSoundFileNames[ind];
    //
    //  Load WAVE files from resource
    //
    HANDLE      hRes    = NULL;
    HANDLE      hBuffer = NULL;
    LPSTR       lpRes   = NULL;
    HRSRC       hResInfo;
    HINSTANCE   hInst   = AfxGetInstanceHandle();
    MMIOINFO    mmioInfo;

    // Find the wav resource
    hResInfo = FindResource(hInst, (LPSTR)szFileName, "WAVE");
    if (!hResInfo)
    {
        TRACE("SoundManager: Failed to find resource file %s.\n", szFileName);
        return;
    }

    // Load the wav resource
    hRes = LoadResource(hInst, hResInfo);
    if (!hRes)
    {
        TRACE("SoundManager: Failed to load resource file %s.\n", szFileName);
        return;
    }

    // Lock the wav resource
    lpRes = (LPSTR)LockResource(hRes);
    if (!lpRes)
    {
        TRACE("SoundManager: Failed to lock resource.\n");
        goto CLEANUP;
    }

    //
    //  Read the wav file into memory:
    //

    // Prepare memory for reading using buffered I/O:
    ZeroMemory (&mmioInfo, sizeof(mmioInfo));
    mmioInfo.fccIOProc = FOURCC_MEM;
    mmioInfo.pchBuffer = (LPSTR)lpRes;
    mmioInfo.cchBuffer = SizeofResource(hInst, hResInfo);
    hBuffer = GlobalAlloc (LMEM_MOVEABLE, mmioInfo.cchBuffer);
    if (!hBuffer)
    {
        TRACE("SoundManager: Out of memory.\n");
        goto CLEANUP;
    }
    mmioInfo.pchBuffer = (LPSTR) GlobalLock(hBuffer);
    if (!mmioInfo.pchBuffer)
    {
        TRACE("SoundManager: Failed to lock memory for buffer.\n");
        goto CLEANUP;
    }
    memcpy (mmioInfo.pchBuffer, lpRes, mmioInfo.cchBuffer);

    if (!mmioInfo.cchBuffer)
    {
        TRACE("SoundManager: Failed to get resource size (Error=%d).\n", 
            GetLastError());
        goto CLEANUP;
    }

    hmmio = mmioOpen(NULL, &mmioInfo, MMIO_READWRITE /*| MMIO_ALLOCBUF*/);
    if(!hmmio)
    {
        TRACE("SoundManager: Failed to open file %s.\n", szFileName);
        return;
    }

    // Locate a 'RIFF' chunk with a 'WAVE' form type
    // to make sure it's a WAVE file.
    mmckinfoParent.fccType = mmioFOURCC('W', 'A', 'V', 'E');
    if (mmioDescend(hmmio, (LPMMCKINFO) &mmckinfoParent, NULL, MMIO_FINDRIFF))
    {
        TRACE("SoundManager: This is not a WAVE file.\n");
        goto CLEANUP;
    }

    // Now, find the format chunk (form type 'fmt '). It should be
    // a subchunk of the 'RIFF' parent chunk.
    mmckinfoSubchunk.ckid = mmioFOURCC('f', 'm', 't', ' ');
    if (mmioDescend(hmmio, &mmckinfoSubchunk, &mmckinfoParent,
        MMIO_FINDCHUNK))
    {
        TRACE("SoundManager: WAVE file is corrupted.\n");
        goto CLEANUP;
    }

    // Get the size of the format chunk, allocate and lock memory for it.
    dwFmtSize = mmckinfoSubchunk.cksize;
    hFormat = GlobalAlloc(LMEM_MOVEABLE, LOWORD(dwFmtSize));
    if (!hFormat)
    {
        TRACE("SoundManager: Out of memory.\n");
        goto CLEANUP;
    }
    pFormat = (WAVEFORMATEX *) GlobalLock(hFormat);
    if (!pFormat)
    {
        TRACE("SoundManager: Failed to lock memory for format chunk.\n");
        goto CLEANUP;
    }

    // Read the format chunk.
    if (mmioRead(hmmio, (HPSTR) pFormat, dwFmtSize) != (LONG) dwFmtSize)
    {
        TRACE("SoundManager: Failed to read format chunk.\n");
        goto CLEANUP;
    }

    // Make sure it's a PCM file.
    if (pFormat->wFormatTag != WAVE_FORMAT_PCM)
    {
        TRACE("SoundManager: The file is not a PCM file.\n");
        goto CLEANUP;
    }

    // Ascend out of the format subchunk.
    mmioAscend(hmmio, &mmckinfoSubchunk, 0);

    // Find the data subchunk.
    mmckinfoSubchunk.ckid = mmioFOURCC('d', 'a', 't', 'a');
    if (mmioDescend(hmmio, &mmckinfoSubchunk, &mmckinfoParent,
        MMIO_FINDCHUNK))
    {
        TRACE("SoundManager: WAVE file has no data chunk.\n");
        goto CLEANUP;
    }

    // Get the size of the data subchunk.
    dwDataSize = mmckinfoSubchunk.cksize;
    if (dwDataSize == 0L)
    {
        TRACE("SoundManager: The data chunk has no data.\n");
        goto CLEANUP;
    }


    // Save block alignment info for later use.
    wBlockSize = pFormat->nBlockAlign;


    // Allocate and lock memory for the waveform data.
    hData = GlobalAlloc(GMEM_MOVEABLE , dwDataSize );
                       // GMEM_SHARE is not needed on 32 bits
    if (!hData)
    {
        TRACE("SoundManager: Out of memory.\n");
        goto CLEANUP;
    }
    lpData = (char *)GlobalLock(hData);
    if (!lpData)
    {
        TRACE("SoundManager: Failed to lock memory for data chunk.\n");
        goto CLEANUP;
    }

    // Read the waveform data subchunk.
    if(mmioRead(hmmio, (HPSTR) lpData, dwDataSize) != (LONG) dwDataSize)
    {
        TRACE("SoundManager: Failed to read data chunk.\n");
        goto CLEANUP;
    }

    //
    //  Create DirectSoundBuffer:
    //

    DSBUFFERDESC dsbd;
    
    //DS Now create the secondary buffer
    memset(&dsbd, 0, sizeof(DSBUFFERDESC));
    dsbd.dwSize       = sizeof(DSBUFFERDESC);
    dsbd.dwFlags      = DSBCAPS_CTRLDEFAULT ;
    dsbd.dwBufferBytes= dwDataSize; // size of the audio data in bytes
    dsbd.lpwfxFormat  = pFormat;    
    if (m_pDirectSound->CreateSoundBuffer(
        &dsbd, 
        &m_aSoundBuffers[ind],
        NULL) != 0)
    {
        TRACE("SoundManager: Direct Sound Buffer Creation Failed\n");
        goto CLEANUP;
    }
    ASSERT (m_aSoundBuffers[ind]);    

    //
    //  Copy data to buffer:
    //

    if (m_aSoundBuffers[ind]->Lock
                  ( 0,                      // the position to start to lock
                    dwDataSize,             // number of bytes to lock
                    (PVOID *)&pbData,       // pbData will point to the memory area
                    &dwLength,              // number of bytes to use under pbData
                    (PVOID *)&pbData2,      // not used since no wrap around
                    &dwLength2,             // not used since no wrap around
                    0L) != DS_OK)
    {
        TRACE("SoundManager: Sound buffer Lock Failed.\n");
        goto CLEANUP;
    }

    // Now we can copy data into the area by, say, memcpy function
    memcpy(pbData, lpData, dwLength);

    // Unlock it
    if (m_aSoundBuffers[ind]->Unlock(pbData, dwLength, NULL, 0) != DS_OK)
    {
        TRACE("SoundManager: Sound buffer Unlock Failed.\n");
    }

CLEANUP:
    if (hRes)
        FreeResource(hRes);

    if (hFormat)
    {
        LocalUnlock( hFormat );
        LocalFree( hFormat );
    }
    if (hData)
    {
        GlobalUnlock( hData );
        GlobalFree( hData );
    }
    if (hBuffer)
    {
        GlobalUnlock( hBuffer );
        GlobalFree( hBuffer );
    }
    if (hmmio)
        mmioClose(hmmio, 0);

    return;
}