// NewCost.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MFCApplication3.h"
#include "NewCost.h"
#include "afxdialogex.h"


// NewCost 대화 상자입니다.

IMPLEMENT_DYNAMIC(NewCost, CDialogEx)

NewCost::NewCost(CWnd* pParent)
	: CDialogEx(IDD_NEW_COST, pParent)
{

}


NewCost::~NewCost()
{
}

void NewCost::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, category_combo);
	DDX_Control(pDX, IDC_EDIT2, price_edit);
	DDX_Control(pDX, IDC_EDIT3, detail_edit);
}


BEGIN_MESSAGE_MAP(NewCost, CDialogEx)
	ON_CBN_SELCHANGE(IDC_COMBO1, &NewCost::OnCbnSelchangeCombo1)
	ON_BN_CLICKED(IDOK, &NewCost::OnBnClickedOk)
END_MESSAGE_MAP()


BOOL NewCost::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	TRY
	{
		db_cost.OpenEx(_T("DSN=UOS25;UID=UOS25;PWD=0000"));
	}
		CATCH(CException, e)
	{
		TCHAR errMSG[255];

		e->GetErrorMessage(errMSG, 255);
		AfxMessageBox(errMSG, MB_ICONERROR);


		if (db_cost.IsOpen())
			db_cost.Close();

	}
	END_CATCH


		return TRUE;
}


void NewCost::OnCbnSelchangeCombo1()
{
	int curPos = category_combo.GetCurSel();

	CString today;
	CTime cTime = CTime::GetCurrentTime();
	today.Format(L"%04d년 %02d월", cTime.GetYear(), cTime.GetMonth());


	switch (curPos) {
	case 0: {
		detail_edit.SetWindowTextW(today + L"임대료 : (내용)");
		break;
	}
	case 1: {
		detail_edit.SetWindowTextW(today + L"공과금 : (내용)");
		break;
	}
	case 2: {
		detail_edit.SetWindowTextW(today + L"기타비용 : (내용)");
		break;
	}
	}
}


void NewCost::OnBnClickedOk()
{
	CString todayHHMMSS;
	CTime cTime2 = CTime::GetCurrentTime();
	todayHHMMSS.Format(L"%04d%02d%02d%02d%02d%02d", cTime2.GetYear(), cTime2.GetMonth(), cTime2.GetDay(), cTime2.GetHour(), cTime2.GetMinute(), cTime2.GetSecond());


	price_edit.GetWindowTextW(price);
	detail_edit.GetWindowTextW(detail);

	CRecordset recSet(&db_cost);
	CString strSQL;
	strSQL.Format(L"insert into FUNDS(FUNDS_DATE, FUNDS_SORT_CODE, PRICE, FUNDS_DETAIL_NUM) values ('%s', 'F03', '-%s', '%s')", todayHHMMSS, price, detail);
	db_cost.ExecuteSQL(strSQL);

	MessageBox(L"유지관리비 등록 완료!");

	CDialogEx::OnOK();
}


