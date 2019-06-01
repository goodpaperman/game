/*****************************************************************************
*                                                                             
*   MyEdit.cpp                                                            
*                                                                             
*   Electrical Engineering Faculty - Software Lab                             
*   Spring semester 1998                                                      
*                                                                             
*   Tanks game                                                                
*                                                                             
*   Module description: Implements an Edit box object that responds to all key
*                       strokes. Used in the keyboard setting dialog, to catch
*                       all key that can be used to control the tank.    
*                                                                             
*   Authors: Eran Yariv - 28484475                                           
*            Moshe Zur  - 24070856                                           
*                                                                            
*                                                                            
*   Date: 23/09/98                                                           
*                                                                            
******************************************************************************/
#include "stdafx.h"
#include "MyEdit.h"


BOOL CMyEdit::PreTranslateMessage(MSG* pMsg)
{
    BOOL bIgnore = FALSE;   // When this flag is true, the special key is not passed to
                            // the application (keys like Return or arrows), to avoid 
                            // cursor movements and mischief behavior.
	if(pMsg->message == WM_SYSKEYDOWN || pMsg->message == WM_KEYDOWN)
    {       // Yep, some key was pressed down alright:
        UINT nKey = pMsg->wParam;
            // Handle text in CEdit control, if key is appropiate
        if (SetKeyName (nKey))
        {   // Check that key is unique, and sets it
            if ( m_pKeys->SetKey(m_iIndex, nKey) ) 
            {
                UpdateText();
            } else  // Sound a beep notifying the key pressed is mapped to another control:
            {
                MessageBeep(MB_ICONEXCLAMATION);
            }
        }
            // Check for trouble makers:
        bIgnore = ( nKey == VK_RETURN  || 
                    nKey == VK_END     ||
                    nKey == VK_DOWN    ||
                    nKey == VK_UP      ||
                    nKey == VK_LEFT    ||
                    nKey == VK_RIGHT );

    }
    if (bIgnore)
        return TRUE;
	return CEdit::PreTranslateMessage(pMsg);
}

/*------------------------------------------------------------------------------

  Function: SetKeyName

  Purpose:  Finds the given key's name, and return TRUE if key is appropiate.

  Input:    nKey: Virtual code of key pressed by user.

  Output:   return TRUE if the key pressed is supported as control key.

  Remarks:  We don't support keys that change the normal windows behavior, such as
            TAB, ALT etc.
------------------------------------------------------------------------------*/
BOOL
CMyEdit::SetKeyName (UINT nKey)
{
    char cGroup1[] = ";=,-./`"; // The codes for these keys are sequents:
    char cGroup2[] = "[\\]'";

    m_cstrKeyName.Empty();   // Reference string, to help find out if key is appropiate

    switch(nKey)
	{	// Handle all playable keys, including non-alpha-numeric keys:
	case VK_CONTROL:
		m_cstrKeyName = "CONTROL";
		break;
		
	case VK_SHIFT:
		m_cstrKeyName = "SHIFT";
		break;
		
	case VK_BACK:
		m_cstrKeyName = "BACKSPACE";
		break;
		
	case VK_RETURN:
		m_cstrKeyName = "ENTER";
		break;
		
	case VK_LEFT:
		m_cstrKeyName = "Left Arrow";
		break;
		
	case VK_RIGHT:
		m_cstrKeyName = "Right Arrow";
		break;
		
	case VK_UP:
		m_cstrKeyName = "Up Arrow";
		break;
		
	case VK_DOWN:
		m_cstrKeyName = "Down Arrow";
		break;
		
	case VK_SPACE:
		m_cstrKeyName = "Spacebar";
		break;

	case VK_MULTIPLY:
		m_cstrKeyName = "NUM *";
		break;

	case VK_DIVIDE:
		m_cstrKeyName = "NUM /";
		break;

	case VK_SUBTRACT:
		m_cstrKeyName = "NUM -";
		break;

	case VK_ADD:
		m_cstrKeyName = "NUM +";
		break;

	case VK_DECIMAL:
		m_cstrKeyName = "NUM .";
		break;

    case VK_NUMPAD0:
    case VK_NUMPAD1:
    case VK_NUMPAD2:
    case VK_NUMPAD3:
    case VK_NUMPAD4:
    case VK_NUMPAD5:
    case VK_NUMPAD6:
    case VK_NUMPAD7:
    case VK_NUMPAD8:
    case VK_NUMPAD9:
        m_cstrKeyName.Format ("NUM %d", nKey - VK_NUMPAD0);
        break;

    case VK_F2:
    case VK_F3:
    case VK_F4:
    case VK_F5:
    case VK_F6:
    case VK_F7:
    case VK_F8:
    case VK_F9:
    case VK_F11:
    case VK_F12:
        m_cstrKeyName.Format ("FUNC %d", nKey - VK_F1 + 1);
        break;

    case VK_PRIOR:
        m_cstrKeyName = "Page Up";
        break;

    case VK_NEXT:
        m_cstrKeyName = "Page Down";
        break;

    case VK_HOME:
        m_cstrKeyName = "Home";
        break;

    case VK_END:
        m_cstrKeyName = "End";
        break;

    case VK_INSERT:
        m_cstrKeyName = "Insert";
        break;

    case VK_DELETE:
        m_cstrKeyName = "Delete";
        break;

    case 186:   // ;
    case 187:   // =
    case 188:   // ,
    case 189:   // -
    case 190:   // .
    case 191:   // /
    case 192:   // `
        m_cstrKeyName = cGroup1[nKey - 186];
        break;

    case 219:   // [
    case 220:   // (\)
    case 221:   // ]
    case 222:   // '
        m_cstrKeyName = cGroup2[nKey - 219];
        break;

    case VK_CLEAR:
        m_cstrKeyName = "NUM 5"; // Infact you get this key if numlock is off, but who cares
        break;

    default:
        if ((47 < nKey && 58 > nKey) ||     // isalnum give us trouble, so we check it manually
            (64 < nKey && 91 > nKey)) 
        {
			m_cstrKeyName.Format ("%c", nKey);
        }
		break;
	}
    return (!m_cstrKeyName.IsEmpty());
}

