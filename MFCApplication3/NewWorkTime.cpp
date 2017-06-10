// Work_Time.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "MFCApplication3.h"
#include "NewWorkTime.h"
#include "afxdialogex.h"


// Work_Time ��ȭ �����Դϴ�.

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


// Work_Time �޽��� ó�����Դϴ�.

BOOL NewWorkTime::OnInitDialog() {
	CDialogEx::OnInitDialog();
	CString year, month, day, hour, minute;

	// �����ͺ��̽����� ����

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

	// ���� �Ǵ� �߰��� ����
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

		GetDlgItem(IDOK)->SetWindowText(TEXT("����"));
	}

	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


void NewWorkTime::OnBnClickedOk()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

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

	// ���� �Ǵ� �߰��� ����
	if (worktime_date.IsEmpty()) {
		if (year.IsEmpty() == FALSE && month.IsEmpty() == FALSE && day.IsEmpty() == FALSE && hour.IsEmpty() == FALSE) {

			sql.Format(L"insert into WORK_HISTORY(WORKTIME_DATE, EMP_NUM, WORKTIME) values ( '%s', '%s', '%s')", date, emp_num, time);
			db_worktime.ExecuteSQL(sql);
			MessageBox(L"�ٹ��̷��� ����Ǿ����ϴ�.");

			CDialogEx::OnOK();
		}
		else
			MessageBox(L"��� �׸��� �Է��� �ֽʽÿ�");

	}
	else {
		sql.Format(L"UPDATE WORK_HISTORY SET WORKTIME_DATE='%s', WORKTIME='%s' WHERE EMP_NUM='%s' AND WORKTIME_DATE='%s'", date, time, emp_num, worktime_date);
		db_worktime.ExecuteSQL(sql);
		MessageBox(L"�ٹ��̷� ���� �Ϸ�");
		CDialogEx::OnOK();
	}
}


void NewWorkTime::OnBnClickedButton1()
{
	CString sql;

	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	sql.Format(L"DELETE FROM WORK_HISTORY WHERE EMP_NUM='%s' AND WORKTIME_DATE='%s' ", emp_num, worktime_date);
	db_worktime.ExecuteSQL(sql);
	MessageBox(L"�ٹ��̷� ���� �Ϸ�");
	CDialogEx::OnOK();
}
