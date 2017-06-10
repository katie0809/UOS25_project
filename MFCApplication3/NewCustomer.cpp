// NewCustomer.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MFCApplication3.h"
#include "NewCustomer.h"
#include "afxdialogex.h"


// CNewCustomer 대화 상자입니다.

IMPLEMENT_DYNAMIC(CNewCustomer, CDialogEx)

CNewCustomer::CNewCustomer(int num, CWnd* pParent)
	: CDialogEx(IDD_NEW_MEMBER, pParent)
{
	member_num.Format(L"%010d", num);
}

CNewCustomer::~CNewCustomer()
{
}

void CNewCustomer::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, mem_name);
	DDX_Control(pDX, IDC_EDIT2, mem_phone);
	DDX_Control(pDX, IDC_STATIC6, mem_num);
}


BEGIN_MESSAGE_MAP(CNewCustomer, CDialogEx)
	ON_BN_CLICKED(IDOK, &CNewCustomer::OnBnClickedOk)
END_MESSAGE_MAP()


// CNewCustomer 메시지 처리기입니다.


void CNewCustomer::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	CString name, phone;
	
	mem_name.GetWindowTextW(name);
	mem_phone.GetWindowTextW(phone);

	CDatabase db_member;

	TRY
	{
		db_member.OpenEx(_T("DSN=UOS25;UID=UOS25;PWD=0000"));
	}
		CATCH(CException, e)
	{
		TCHAR errMSG[255];

		e->GetErrorMessage(errMSG, 255);
		AfxMessageBox(errMSG, MB_ICONERROR);


		if (db_member.IsOpen())
			db_member.Close();

	}
	END_CATCH

	CString SQL;
	SQL.Format(L"INSERT INTO MEMBER(MEMBER_NUM, NAME, PHONE, MILEAGE) VALUES('%s', '%s', '%s', 0)", member_num, name, phone);
	
	db_member.ExecuteSQL(SQL);
	db_member.Close();

	SetDlgItemText(IDC_STATIC6, member_num);

	Sleep(1500);
	CDialogEx::OnOK();
}
