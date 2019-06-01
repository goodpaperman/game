/*****************************************************************************
*                                                                             
*   SoundManager.h
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
#ifndef _SOUND_MANAGER_H_
#define _SOUND_MANAGER_H_

#include <dsound.h>

class CSoundManager
{
public:
    enum SoundType 
    { 
        FIRE_BULLET, 
        FIRE_SHELL, 
        SHELL_EXPLODE,
        MINE_EXPLODE, 
        TANK_EXPLODE,
        DROP_BOMB,
        PICK_BONUS,
        GAME_OVER,
        LAST_SOUND_TYPE
    };
    CSoundManager();
    ~CSoundManager();

    BOOL Init (HWND);
    void Play (SoundType);
    void SetMuteMode (BOOL bMuteMode) { m_bMuteMode = bMuteMode; }

private:
    // members:
    LPDIRECTSOUND       m_pDirectSound;
    LPDIRECTSOUNDBUFFER m_aSoundBuffers[LAST_SOUND_TYPE];
    static LPCSTR       m_aszSoundFileNames[LAST_SOUND_TYPE];
    BOOL                m_bMuteMode;
    //methods:
    void LoadWavToBuffer(SoundType);
};

#endif