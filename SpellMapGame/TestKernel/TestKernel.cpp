// TestKernel.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "windows.h"
#include <iostream.h>
#include "..\include\MapCore.h"


void Usage()
{
	cout<<"Selection:"<<endl ;
	cout<<" 1:Move Up."<<endl ;
	cout<<" 2:Move Down."<<endl ;
	cout<<" 3:Move Left."<<endl ;
	cout<<" 4:Move Right."<<endl ;
	cout<<"-1:Quit."<<endl ;
}

int main(int argc, char* argv[])
{
	printf("Hello World!\n");
	cout<<"Test Start."<<endl ;

	CMapCore core ; 
	core.InitMap(3, 3) ;
	core.m_bRandomBlank = FALSE ;
	core.RandomMap() ;
	core.PrintMap(TRUE) ;

	Point pt = core.GetBlankPoint() ;
	cout<<pt.row<<","<<pt.col<<endl ;

	bool bQuit = false ;
	do
	{
		Usage() ;
		int result ;
		cin>>result; 
		switch(result)
		{
		case -1:
			bQuit = true ;
			break ;
		case 1:
			core.SwitchBlank(up) ;
			break ;
		case 2:
			core.SwitchBlank(down) ;
			break ;
		case 3:
			core.SwitchBlank(left) ;
			break ;
		case 4 :
			core.SwitchBlank(right) ;
			break ;
		default :
			Usage() ;
			break ;
		}

		core.PrintMap(TRUE) ;
		if(core.IsWin())
		{
			cout<<"Congratulations! You Win."<<endl ;
			break ;
		}
	}while(!bQuit);

	cout<<"Test Stop."<<endl ;
	return 0;
}
