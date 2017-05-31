// Login.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "MFCApplication3.h"
#include "Login.h"
#include "afxdialogex.h"


// CLogin ��ȭ �����Դϴ�.

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


// CLogin �޽��� ó�����Դϴ�.


BOOL CLogin::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.



	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


void CLogin::OnBnClickedLogin()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
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
		MessageBox(L"�߸��� ����� �����Դϴ�");
		s_usercode.SetWindowTextW(L"");
	}
	
}
