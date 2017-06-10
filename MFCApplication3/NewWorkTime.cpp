// Work_Time.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MFCApplication3.h"
#include "NewWorkTime.h"
#include "afxdialogex.h"


// Work_Time 대화 상자입니다.

IMPLEMENT_DYNAMIC(NewWorkTime, CDialogEx)

NewWorkTime::NewWorkTime(CWnd* pParent, CString num, CString date/*=NULL*/)
	: CDialogEx(IDD_NEW_WORKTIME, pParent)
{
	emp_num = num;
	worktime_date = date;
}

NewWorkTime::~NewWorkTime()
{
}


void NewWorkTime::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, 1020, m_worktime_year);
	DDX_Control(pDX, 1025, m_worktime_month);
	DDX_Control(pDX, 1026, m_worktime_day);
	DDX_Control(pDX, 1027, m_worktime_hour);
	DDX_Control(pDX, 1028, m_worktime_minute);
}


BEGIN_MESSAGE_MAP(NewWorkTime, CDialogEx)
	ON_BN_CLICKED(IDOK, &NewWorkTime::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON1, &NewWorkTime::OnBnClickedButton1)
END_MESSAGE_MAP()


// Work_Time 메시지 처리기입니다.

BOOL NewWorkTime::OnInitDialog() {
	CDialogEx::OnInitDialog();
	CString year, month, day, hour, minute;

	// 데이터베이스와의 연결

	TRY
	{
		db_worktime.OpenEx(_T("DSN=UOS25;UID=UOS25;PWD=0000"));
	}
		CATCH(CException, e)
	{
		TCHAR errMSG[255];

		e->GetErrorMessage(errMSG, 255);
		AfxMessageBox(errMSG, MB_ICONERROR);


		if (db_worktime.IsOpen())
			db_worktime.Close();

	}
	END_CATCH


		for (int i = 2000; i<2030; i++) {
			year.Format(_T("%d"), i);
			m_worktime_year.AddString(year);
		}

	for (int i = 1; i < 13; i++) {
		month.Format(_T("%02d"), i);
		m_worktime_month.AddString(month);
	}

	for (int i = 1; i < 32; i++) {
		day.Format(_T("%02d"), i);
		m_worktime_day.AddString(day);
	}

	for (int i = 1; i < 25; i++) {
		hour.Format(_T("%2d"), i);
		m_worktime_hour.AddString(hour);
	}

	for (int i = 0; i <2; i++) {
		minute.Format(_T("%d"), i * 30);
		m_worktime_minute.AddString(minute);
	}

	// 수정 또는 추가에 따라
	if (worktime_date.IsEmpty()) {
		m_worktime_year.SetCurSel(0);
		m_worktime_month.SetCurSel(0);
		m_worktime_day.SetCurSel(0);
		m_worktime_hour.SetCurSel(0);
		m_worktime_minute.SetCurSel(0);
		GetDlgItem(IDC_BUTTON1)->ShowWindow(FALSE);
	}
	else {

		CRecordset recSet(&db_worktime);
		CString sql, time;
		sql.Format(L"SELECT WORKTIME FROM WORK_HISTORY WHERE EMP_NUM='%s' AND WORKTIME_DATE='%s'", emp_num, worktime_date);
		recSet.Open(CRecordset::dynaset, sql);

		if (!recSet.IsEOF()) {
			recSet.GetFieldValue(_T("WORKTIME"), time);
		}
		recSet.Close();

		year = worktime_date.Left(4);
		month = worktime_date.Mid(4, 2);
		day = worktime_date.Right(2);
		AfxExtractSubString(hour, time, 0, '.');
		AfxExtractSubString(minute, time, 1, '.');

		for (int i = 0; i < 30; i++) {
			CString combo_year;
			m_worktime_year.GetLBText(i, combo_year);
			if (combo_year == year) {
				m_worktime_year.SetCurSel(i);
				break;
			}
		}

		for (int i = 0; i < 12; i++) {
			CString combo_month;
			m_worktime_month.GetLBText(i, combo_month);
			if (combo_month == month) {
				m_worktime_month.SetCurSel(i);
				break;
			}
		}

		for (int i = 0; i < 31; i++) {
			CString combo_day;
			m_worktime_day.GetLBText(i, combo_day);
			if (combo_day == day) {
				m_worktime_day.SetCurSel(i);
				break;
			}
		}

		for (int i = 0; i < 24; i++) {
			CString combo_hour;
			m_worktime_hour.GetLBText(i, combo_hour);
			if (combo_hour == hour) {
				m_worktime_hour.SetCurSel(i);
				break;
			}
		}

		if (minute == '0')
			m_worktime_minute.SetCurSel(0);
		else
			m_worktime_minute.SetCurSel(1);

		GetDlgItem(IDOK)->SetWindowText(TEXT("수정"));
	}

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void NewWorkTime::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	CString sql, year, month, day, hour, minute, date, time;

	this->m_worktime_year.GetLBText(this->m_worktime_year.GetCurSel(), year);
	this->m_worktime_month.GetLBText(this->m_worktime_month.GetCurSel(), month);
	this->m_worktime_day.GetLBText(this->m_worktime_day.GetCurSel(), day);
	this->m_worktime_hour.GetLBText(this->m_worktime_hour.GetCurSel(), hour);

	date.Format(L"%4s%2s%2s", year, month, day);

	if (m_worktime_minute.GetCurSel() == 0)
		time.Format(L"%2s.0", hour);
	else
		time.Format(L"%2s.5", hour);

	// 수정 또는 추가에 따라
	if (worktime_date.IsEmpty()) {
		if (year.IsEmpty() == FALSE && month.IsEmpty() == FALSE && day.IsEmpty() == FALSE && hour.IsEmpty() == FALSE) {

			sql.Format(L"insert into WORK_HISTORY(WORKTIME_DATE, EMP_NUM, WORKTIME) values ( '%s', '%s', '%s')", date, emp_num, time);
			db_worktime.ExecuteSQL(sql);
			MessageBox(L"근무이력이 저장되었습니다.");

			CDialogEx::OnOK();
		}
		else
			MessageBox(L"모든 항목을 입력해 주십시오");

	}
	else {
		sql.Format(L"UPDATE WORK_HISTORY SET WORKTIME_DATE='%s', WORKTIME='%s' WHERE EMP_NUM='%s' AND WORKTIME_DATE='%s'", date, time, emp_num, worktime_date);
		db_worktime.ExecuteSQL(sql);
		MessageBox(L"근무이력 수정 완료");
		CDialogEx::OnOK();
	}
}


void NewWorkTime::OnBnClickedButton1()
{
	CString sql;

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	sql.Format(L"DELETE FROM WORK_HISTORY WHERE EMP_NUM='%s' AND WORKTIME_DATE='%s' ", emp_num, worktime_date);
	db_worktime.ExecuteSQL(sql);
	MessageBox(L"근무이력 삭제 완료");
	CDialogEx::OnOK();
}
