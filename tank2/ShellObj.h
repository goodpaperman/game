// ShellObj.h: interface for the CShellObj class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SHELLOBJ_H__9BA98072_7A5C_47DE_8742_E130FD8D1903__INCLUDED_)
#define AFX_SHELLOBJ_H__9BA98072_7A5C_47DE_8742_E130FD8D1903__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "line.h"
#define MAP_MAX 13			//13��λͼ

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

	CPointf m_position ;		//λ��
	float m_radius ;			//�뾶
	CLine m_routine ;		//�н�·��
	UINT m_tankID ;			//����̹��ID
	STATE m_state ;		//״̬
	TYPE m_type ;		//���
//	RANK m_rank ;		//�ȼ�
	float m_step ;			//ǰ������
	float m_range ;			//ɱ�˰뾶
	float m_excution ;		//ɱ����
	UINT m_mapID ;			//��ըλͼID
	BOOL m_explode ;		//�Ƿ�ը
};

#endif // !defined(AFX_SHELLOBJ_H__9BA98072_7A5C_47DE_8742_E130FD8D1903__INCLUDED_)
