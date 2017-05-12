
// MFCApplication3.h : MFCApplication3 응용 프로그램에 대한 주 헤더 파일
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH에 대해 이 파일을 포함하기 전에 'stdafx.h'를 포함합니다."
#endif

#include "resource.h"       // 주 기호입니다.


// CMFCApplication3App:
// 이 클래스의 구현에 대해서는 MFCApplication3.cpp을 참조하십시오.
//
class CMenuView;
class CMFCApplication3App : public CWinApp
{
public:
	CMFCApplication3App();


/*
	Attributes
*/
public:
	CDatabase db_menu;
	CMenuView * p_menuView;
/*
	Operations
*/
public:
	BOOL ShowMenu();		// 왼쪽 트리 메뉴를 보여주는 함수
	BOOL OpenDatabase();	// 데이터베이스에 연결해주는 함수


// 재정의입니다.
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// 구현입니다.
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CMFCApplication3App theApp;
