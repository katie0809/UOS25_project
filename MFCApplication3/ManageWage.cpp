// ManageWage.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MFCApplication3.h"
#include "ManageWage.h"
#include "afxdialogex.h"


// ManageWage 대화 상자입니다.

IMPLEMENT_DYNAMIC(ManageWage, CDialogEx)

ManageWage::ManageWage(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG3, pParent)
{

}

ManageWage::~ManageWage()
{
}

void ManageWage::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, employee_combo);
	DDX_Control(pDX, IDC_TITLE_TEXT, title_text);
	DDX_Control(pDX, IDC_WORKTIME_TEXT, worktime_text);
	DDX_Control(pDX, IDC_WAGE_TEXT, wage_text);
}


BEGIN_MESSAGE_MAP(ManageWage, CDialogEx)
	ON_CBN_SELCHANGE(IDC_COMBO1, &ManageWage::OnCbnSelchangeCombo1)
	ON_BN_CLICKED(IDOK, &ManageWage::OnBnClickedOk)
END_MESSAGE_MAP()


//detail 추가명 : 이름
void ManageWage::OnCbnSelchangeCombo1()
{
	int index = employee_combo.GetCurSel();
	employee_combo.GetLBText(index, name);


	//직급 찾기
	CRecordset rec(&db_wage);
	CString titleSQL;
	titleSQL.Format(L"select EMP_CODE, RANK from EMPLOYEE where NAME = '%s'", name);
	rec.Open(CRecordset::dynaset, titleSQL);
	rec.GetFieldValue(_T("RANK"), title);
	rec.GetFieldValue(_T("EMP_CODE"), emp_code);
	title_text.SetWindowTextW(title);

	//직급에 해당하는 시급 찾기
	CRecordset rec2(&db_wage);
	titleSQL.Format(L"select SORT_DETAIL from SORT_CODE where SORT_CODE_C = '%s'", title);
	rec2.Open(CRecordset::dynaset, titleSQL);
	rec2.GetFieldValue(_T("SORT_DETAIL"), the_wage);

	//마지막 지급 날짜 확인 : F04와 이름을 기준으로 FUNDS에서 찾는다.
	//있을 경우 해당 날 이후로의 WORK_HISTORY의 시간 도합, 없을 경우 모든 WORK_HISTORY의 도합
	CRecordset rec3(&db_wage);
	titleSQL.Format(L"select FUNDS_DATE from FUNDS where FUNDS_SORT = 'F04' and FUNDS_DETAIL_NUM = '%s' order by FUNDS_DATE desc", name);
	rec3.Open(CRecordset::dynaset, titleSQL);



	if (rec3.IsEOF()) {
		CRecordset rec4(&db_wage);
		titleSQL.Format(L"select WORKTIME from WORK_HISTORY where EMP_CODE = '%s'", emp_code);
		rec4.Open(CRecordset::dynaset, titleSQL);
		int working_time(0);

		while (!rec4.IsEOF()) {
			CString temp_time;
			rec4.GetFieldValue(_T("WORKTIME"), temp_time);
			working_time += _ttoi(temp_time);
			rec4.MoveNext();
		}

		work_hours.Format(_T("%d"), working_time);
		worktime_text.SetWindowTextW(work_hours);

		working_time *= _ttoi(the_wage);
		total_wage.Format(_T("%d"), working_time);
		wage_text.SetWindowTextW(total_wage);
	}
	else {
		rec3.GetFieldValue(_T("FUNDS_DATE"), last_date);

		CRecordset rec5(&db_wage);
		titleSQL.Format(L"select WORKTIME from WORK_HISTORY where EMP_CODE = '%s' and WORK_DATE >= '%s'", emp_code, last_date);
		rec5.Open(CRecordset::dynaset, titleSQL);
		int working_time(0);

		while (!rec5.IsEOF()) {
			CString temp_time;
			rec5.GetFieldValue(_T("WORKTIME"), temp_time);
			working_time += _ttoi(temp_time);
			rec5.MoveNext();
		}

		work_hours.Format(_T("%d"), working_time);
		worktime_text.SetWindowTextW(work_hours);

		working_time *= _ttoi(the_wage);
		total_wage.Format(_T("%d"), working_time);
		wage_text.SetWindowTextW(total_wage);
	}

}


BOOL ManageWage::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	TRY
	{
		db_wage.OpenEx(_T("DSN=UOS25;UID=UOS25;PWD=0000"));
	}
		CATCH(CException, e)
	{
		TCHAR errMSG[255];

		e->GetErrorMessage(errMSG, 255);
		AfxMessageBox(errMSG, MB_ICONERROR);


		if (db_wage.IsOpen())
			db_wage.Close();

	}
	END_CATCH


		CRecordset recSet(&db_wage);
	CString cate_name;
	CString sql_str = ( L"select NAME from EMPLOYEE" );
	recSet.Open(CRecordset::dynaset, sql_str);

	while (!recSet.IsEOF()) {
		recSet.GetFieldValue(_T("NAME"), cate_name);
		employee_combo.AddString(cate_name);
		recSet.MoveNext();
	}

	return TRUE;
}


void ManageWage::OnBnClickedOk()
{
	CString todayHHMMSS;
	CTime cTime2 = CTime::GetCurrentTime();
	todayHHMMSS.Format(L"%04d%02d%02d%02d%02d%02d", cTime2.GetYear(), cTime2.GetMonth(), cTime2.GetDay(), cTime2.GetHour(), cTime2.GetMinute(), cTime2.GetSecond());

	CString addWageSQL;
	addWageSQL.Format(L"insert into FUNDS(FUNDS_DATE, FUNDS_SORT_CODE, PRICE, FUNDS_DETAIL_NUM) values ('%s', 'F04', '-%s', '%s')", todayHHMMSS, total_wage, name);
	db_wage.ExecuteSQL(addWageSQL);

	MessageBox(L"직원 급여 정산 완료!");
	CDialogEx::OnOK();
}
