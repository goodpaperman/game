/*****************************************************************************
*                                                                             
*   Timer.h                                                            
*                                                                             
*   Electrical Engineering Faculty - Software Lab                             
*   Spring semester 1998                                                      
*                                                                             
*   Tanks game                                                                
*                                                                             
*   Module description: Implements the time calculation used to synch the 
*                       local machine time with the host machine time.
*                       
*                                                                             
*   Authors: Eran Yariv - 28484475                                           
*            Moshe Zur  - 24070856                                           
*                                                                            
*                                                                            
*   Date: 23/09/98                                                           
*                                                                            
******************************************************************************/
#ifndef TIMER_H
#define TIMER_H

#include "stdafx.h"

/*  If VAN_JACOBSON is defined, the Van-Jacobon technique is used to calc the delta
    between the server time (remote) and the client time (local).
    (as in TCP round-trip delay estimation algorithms)

    Otherwise, regular smoothed average is used to calc the delta
    between the server time (remote) and the client time (local).


    Refer to Dr. Dobbs #283, March 98, pp. 127-130 for more details.
*/

class CTimer
{
public:

    CTimer              (BOOL bForceLowResTimers = FALSE);   // If bForceLowResTimers, GetTickCount is used
    virtual             ~CTimer () {}

    DWORD               GetRemoteTime ();
    DWORD               GetLocalTime (DWORD dwRemoteTime);
    DWORD               GetLocalTime ();

protected:

    virtual DWORD       UpdateDelta (DWORD) = 0;
    LONG                m_lLastDelta;       // Delta = LocalTime - RemoteTime
    BOOL                m_bFirstSample;

private:
    static DWORD CALLBACK   SampleLocalTimeWithPerfCounters();
    static LONGLONG     m_llFreq;
    DWORD               (CALLBACK* m_pActualSampleFunction) ();
};

class CVanJacobsonTimer : public CTimer
{
public:
    CVanJacobsonTimer   (BOOL bForceLowResTimers = FALSE);   // If bForceLowResTimers, GetTickCount is used
    virtual             ~CVanJacobsonTimer() {}
private:
    virtual DWORD       UpdateDelta (DWORD);

    #define VJ_g        double(0.125)
    #define VJ_h        double(0.250)
    #define VJ_f        double(4.000)

    double              m_dPrevSRTT;        // Previous Smoothed Round-Trip Time
    double              m_dPrevSDEV;        // Previous standard deviation
};

class CSmoothedAverageTimer : public CTimer
{
public:
    CSmoothedAverageTimer (BOOL bForceLowResTimers = FALSE);   // If bForceLowResTimers, GetTickCount is used
    virtual             ~CSmoothedAverageTimer() {}
private:
    virtual DWORD       UpdateDelta (DWORD);

    #define ALPHA           double(0.5)
    #define ONE_MINUS_ALPHA double (1.0 - ALPHA)

    double                          m_dPrevDelta;
};


// Inline sections:
#include <Timer.inl>

// The TIMER_CLASS macro is used to specify the type of timer in use throughout
// the program. It can be either CSmoothedAverageTimer or CVanJacobsonTimer.
#define TIMER_CLASS         CSmoothedAverageTimer     

#endif
