/*****************************************************************************
*                                                                             
*   ManouverSet.h
*                                                                             
*   Electrical Engineering Faculty - Software Lab                             
*   Spring semester 1998                                                      
*                                                                             
*   Tanks game                                                                
*                                                                             
*   Module description: Implements the tanks manouver set (movement and fire
*                       control).
*                       
*                                                                             
*   Authors: Eran Yariv - 28484475                                           
*            Moshe Zur  - 24070856                                           
*                                                                            
*                                                                            
*   Date: 23/09/98                                                           
*                                                                            
******************************************************************************/
#ifndef MANOUVER_SET_H
#define MANOUVER_SET_H

#include <afxmt.h>

class CManouverSet
{
public:

enum {TURN_RIGHT     ,
      TURN_LEFT      ,
      FORWARD        ,
      BACKWARD       ,
      FIRE_SHELL     ,
      FIRE_BULLET    ,
      DROP_MINE      ,
      AERIAL_SUPPORT ,
      MAX_MANOUVER_BIT };

enum {TURN_RIGHT_MASK     = 1 << TURN_RIGHT,
      TURN_LEFT_MASK      = 1 << TURN_LEFT,
      FORWARD_MASK        = 1 << FORWARD,
      BACKWARD_MASK       = 1 << BACKWARD,
      FIRE_SHELL_MASK     = 1 << FIRE_SHELL,
      FIRE_BULLET_MASK    = 1 << FIRE_BULLET,
      DROP_MINE_MASK      = 1 << DROP_MINE,
      AERIAL_SUPPORT_MASK = 1 << AERIAL_SUPPORT,
      MANOUVER_SET_MASK   = (1 << MAX_MANOUVER_BIT) - 1 };

    CManouverSet ();
    virtual ~CManouverSet ();

    void SetAll (BYTE /* New set */);
    void SetBit (UINT /* Index */);
    void UnsetBit (UINT);
    BYTE GetAll();
    void Clear ();

private:
    BYTE    m_uManouverSet;

    static const UINT MOVE_COLLISION;
    static const UINT TURN_COLLISION;
};

// Inline sections:
#include <ManouverSet.inl>

#endif

