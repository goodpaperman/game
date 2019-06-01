/*****************************************************************************
*                                                                             
*   ManouverSet.inl
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
inline
CManouverSet::CManouverSet () : 
    m_uManouverSet(0)                          // Clear set
    {}

inline
CManouverSet::~CManouverSet ()
    {}

inline void 
CManouverSet::SetAll (BYTE NewManouverSet) 
{ 
    m_uManouverSet = BYTE(NewManouverSet & BYTE(MANOUVER_SET_MASK));    // Mask input to retain bounds
}

inline BYTE
CManouverSet::GetAll()
{
    return m_uManouverSet;
}
    
inline void 
CManouverSet::Clear ()
{
    m_uManouverSet = 0;
}

inline void
CManouverSet::UnsetBit(UINT uInd)
{
    UINT uMask = ~(1 << uInd);
    m_uManouverSet &= uMask;
}