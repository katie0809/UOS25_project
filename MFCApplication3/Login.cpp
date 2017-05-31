// Login.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MFCApplication3.h"
#include "Login.h"
#include "afxdialogex.h"


// CLogin 대화 상자입니다.

IMPLEMENT_DYNAMIC(CLogin, CDialogEx)

CLogin::CLogin(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_LOGIN, pParent)
{

}

CLogin::~CLogin()
{
}

void CLogin::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, s_usercode);
}


BEGIN_MESSAGE_MAP(CLogin, CDialogEx)
	ON_BN_CLICKED(IDC_LOGIN, &CLogin::OnBnClickedLogin)
END_MESSAGE_MAP()


// CLogin 메시지 처리기입니다.


BOOL CLogin::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.



	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CLogin::OnBnClickedLogin()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString usercode;
	s_usercode.GetWindowTextW(usercode);

	if (usercode == "0000")
	{
		SYSTEM_USER = USER_EMPLOYEE;
		OnOK();
	}
	else if (usercode == "1234")
	{
		SYSTEM_USER = USER_MANAGER;
		OnOK();
	}
	else
	{
		MessageBox(L"잘못된 사용자 계정입니다");
		s_usercode.SetWindowTextW(L"");
	}
	
}
