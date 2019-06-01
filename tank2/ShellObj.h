// ShellObj.h: interface for the CShellObj class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SHELLOBJ_H__9BA98072_7A5C_47DE_8742_E130FD8D1903__INCLUDED_)
#define AFX_SHELLOBJ_H__9BA98072_7A5C_47DE_8742_E130FD8D1903__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "line.h"
#define MAP_MAX 13			//13幅位图

class CShellObj  
{
public:
	CShellObj();
	CShellObj( CPointf point, 
			   UINT tankID = 0 , 
			   UINT type = companion ,  
//			   UINT rank = slow , 
			   CLine routine = CLine(),
			   UINT state = run , 
			   float radius = 3, 
			   float step = 30 , 
			   float range = 20 , 
			   float excution = 10, 
			   UINT mapID = 0 , 
			   UINT bExplode = FALSE);
	virtual ~CShellObj();

public :
	void DrawShell( CDC *pDC );
	void MoveTo();
	typedef enum {run, explode}STATE ; 
	typedef enum {companion, enemy}TYPE ; 
//	typedef enum {slow, normal, quick, fast}RANK ; 

	CPointf m_position ;		//位置
	float m_radius ;			//半径
	CLine m_routine ;		//行进路径
	UINT m_tankID ;			//所属坦克ID
	STATE m_state ;		//状态
	TYPE m_type ;		//类别
//	RANK m_rank ;		//等级
	float m_step ;			//前进步长
	float m_range ;			//杀伤半径
	float m_excution ;		//杀伤力
	UINT m_mapID ;			//爆炸位图ID
	BOOL m_explode ;		//是否爆炸
};

#endif // !defined(AFX_SHELLOBJ_H__9BA98072_7A5C_47DE_8742_E130FD8D1903__INCLUDED_)
