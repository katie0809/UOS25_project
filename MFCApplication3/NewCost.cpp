// NewCost.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "MFCApplication3.h"
#include "NewCost.h"
#include "afxdialogex.h"


// NewCost ��ȭ �����Դϴ�.

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
	today.Format(L"%04d�� %02d��", cTime.GetYear(), cTime.GetMonth());


	switch (curPos) {
	case 0: {
		detail_edit.SetWindowTextW(today + L"�Ӵ�� : (����)");
		break;
	}
	case 1: {
		detail_edit.SetWindowTextW(today + L"������ : (����)");
		break;
	}
	case 2: {
		detail_edit.SetWindowTextW(today + L"��Ÿ��� : (����)");
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

	MessageBox(L"���������� ��� �Ϸ�!");

	CDialogEx::OnOK();
}


