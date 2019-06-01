/*****************************************************************************
*                                                                             
*   Timer.cpp                                                            
*                                                                             
*   Electrical Engineering Faculty - Software Lab                             
*   Spring semester 1998                                                      
*                                                                             
*   Tanks game                                                                
*                                                                             
*   Module description: Calculates the time delta between the local time and 
*                       the host machine local time.
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
#include <Timer.h>
#include <math.h>

/****************** CTimer **********************/

CTimer::CTimer (BOOL bForceLowResTimers) : m_bFirstSample (TRUE)
{
    LARGE_INTEGER   liFreq;
    if (!bForceLowResTimers && QueryPerformanceFrequency( &liFreq ))
    {   // User didn't force us to use low resoultion timer and 
        // the high res timer is supported by the system
        ASSERT (liFreq.QuadPart > 0);
        m_llFreq = liFreq.QuadPart / LONGLONG (1000);
        m_pActualSampleFunction = this->SampleLocalTimeWithPerfCounters;
    }
    else
    {
        m_pActualSampleFunction = GetTickCount;
    }
}

DWORD CALLBACK
CTimer::SampleLocalTimeWithPerfCounters()
{
    LARGE_INTEGER li;    
    QueryPerformanceCounter (&li);
    return DWORD (li.QuadPart / m_llFreq);
}

LONGLONG CTimer::m_llFreq;

/****************** CVanJacobsonTimer **********************/

/*------------------------------------------------------------------------------

  Function: UpdateDelta

  Purpose:  Updates the delta between the local timer (on this machine) and the 
            host machine timer, using the Van-Jacobson algo. (used in TCP protocol)
            based on former samples. This algo. gives more damped results than the
            smoothed average algo..

  Input:    dwRemoteTime - a new delta sample calculated from last message.

  Output:   Returns last sample of local time.

  Remarks:  

------------------------------------------------------------------------------*/
DWORD
CVanJacobsonTimer::UpdateDelta (DWORD dwRemoteTime)
{
    DWORD dwCurLocalTime = GetLocalTime ();
    LONG lCurDelta = LONG(dwCurLocalTime) - LONG (dwRemoteTime);

    if (m_bFirstSample)
    {
        m_lLastDelta = lCurDelta;
        m_dPrevSRTT = double(lCurDelta);
        m_dPrevSDEV = 0.0;
    }
    else
    {
        double dSRTT = (1.0 - VJ_g) * double(m_dPrevSRTT) + VJ_g * double(lCurDelta);
        double dERR  = double(lCurDelta) - double(m_dPrevSRTT);
        double dSDEV = (1.0 - VJ_h) * m_dPrevSDEV + VJ_h * fabs(dERR);
        double dRTO  = dSRTT + VJ_f * dSDEV;

        m_lLastDelta = LONG(dRTO);

        // Update previous values:
        m_dPrevSRTT = dSRTT;
        m_dPrevSDEV = dSDEV;
    }
    return dwCurLocalTime;   // Return sampled local time
}

/****************** CSmoothedAverageTimer **********************/

CSmoothedAverageTimer::CSmoothedAverageTimer  (BOOL bForceLowResTimers) :
    CTimer (bForceLowResTimers),
    m_dPrevDelta (0.0)
{}

/*------------------------------------------------------------------------------

  Function: UpdateDelta

  Purpose:  Updates the delta between the local timer (on this machine) and the 
            host machine timer, using the smoothed average algo..

  Input:    dwRemoteTime - a new delta sample calculated from last message.

  Output:   Returns last sample of local time.

  Remarks:  

------------------------------------------------------------------------------*/
DWORD
CSmoothedAverageTimer::UpdateDelta (DWORD dwRemoteTime)
// Returns last sample of local time, updates m_lLastDelta 
{
    DWORD dwCurLocalTime = GetLocalTime ();
    LONG lCurDelta = LONG(dwCurLocalTime) - LONG (dwRemoteTime);

    if (!m_bFirstSample)
    {   // Not first time we get a sample
        m_dPrevDelta = ALPHA * m_dPrevDelta + ONE_MINUS_ALPHA * double(lCurDelta);
    }
    else 
    {   // First time we get a sample
        m_bFirstSample = FALSE; // Turn it off
        m_dPrevDelta = lCurDelta;
    }

    m_lLastDelta = LONG (m_dPrevDelta);
    return dwCurLocalTime;   // Return sampled local time
}

