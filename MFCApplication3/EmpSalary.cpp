// EmpSalary.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MFCApplication3.h"
#include "EmpSalary.h"
#include "afxdialogex.h"


// EmpSalary 대화 상자입니다.

IMPLEMENT_DYNAMIC(EmpSalary, CDialogEx)

EmpSalary::EmpSalary(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_EMP_SALARY, pParent)
{

}

EmpSalary::~EmpSalary()
{
}

void EmpSalary::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, 1020, m_empsalary_emp);
	DDX_Control(pDX, 1026, m_empsalary_year);
	DDX_Control(pDX, 1025, m_empsalary_month);
	DDX_Control(pDX, 1019, m_empsalary_time);
	DDX_Control(pDX, 1021, m_empsalary_salary);
}


BEGIN_MESSAGE_MAP(EmpSalary, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &EmpSalary::OnBnClickedButton1)
	ON_BN_CLICKED(IDOK, &EmpSalary::OnBnClickedOk)
	ON_CBN_SELCHANGE(1026, &EmpSalary::OnCbnSelchangeCombo3)
	ON_CBN_SELCHANGE(1025, &EmpSalary::OnCbnSelchangeCombo2)
	ON_CBN_SELCHANGE(1020, &EmpSalary::OnCbnSelchangeCombo1)
END_MESSAGE_MAP()


// EmpSalary 메시지 처리기입니다.



// EmpSalary 메시지 처리기입니다.

BOOL EmpSalary::OnInitDialog() {
	CDialogEx::OnInitDialog();

	// 데이터베이스와의 연결

	TRY
	{
		db_empsalary.OpenEx(_T("DSN=UOS25;UID=UOS25;PWD=0000"));
	}
		CATCH(CException, e)
	{
		TCHAR errMSG[255];

		e->GetErrorMessage(errMSG, 255);
		AfxMessageBox(errMSG, MB_ICONERROR);


		if (db_empsalary.IsOpen())
			db_empsalary.Close();

	}
	END_CATCH

		ShowData();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void EmpSalary::ShowData() {
	CRecordset recSet(&db_empsalary);
	CString sql, emp_num, name, rank, emp_string, year, month;

	sql.Format(L"SELECT EMP_NUM, NAME, RANK_SORT_CODE FROM EMPLOYEE WHERE RANK_SORT_CODE IN ('사장', '매니저', '야간직원', '주간직원') ORDER BY EMP_NUM DESC");
	recSet.Open(CRecordset::dynaset, sql);

	// 직원 목록 추가
	while (!recSet.IsEOF())
	{
		recSet.GetFieldValue(_T("EMP_NUM"), emp_num);
		recSet.GetFieldValue(_T("NAME"), name);
		recSet.GetFieldValue(_T("RANK_SORT_CODE"), rank);

		emp_string.Format(L"%s.%s.%s", emp_num, name, rank);
		m_empsalary_emp.AddString(emp_string);

		//다음 레코드로 이동
		recSet.MoveNext();
	}
	recSet.Close();

	// 2000~2030년 추가
	for (int i = 2000; i<2030; i++) {
		year.Format(L"%d", i);
		m_empsalary_year.AddString(year);
	}

	// 1~12월 추가
	for (int i = 1; i < 13; i++) {
		month.Format(L"%02d", i);
		m_empsalary_month.AddString(month);
	}

}


void EmpSalary::OnBnClickedButton1()
{
	CRecordset recSet(&db_empsalary);
	CString sql, emp_num, rank, year, month, funds_sort, date_start, date_end, s_hour, s_minute, s_salary;
	int hour, minute, salary;

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// 월급확인

	// 정보 받아오기
	this->m_empsalary_emp.GetLBText(this->m_empsalary_emp.GetCurSel(), emp_num);
	this->m_empsalary_year.GetLBText(this->m_empsalary_year.GetCurSel(), year);
	this->m_empsalary_month.GetLBText(this->m_empsalary_month.GetCurSel(), month);

	if (emp_num.IsEmpty() == FALSE && year.IsEmpty() == FALSE && month.IsEmpty() == FALSE) {

		AfxExtractSubString(rank, emp_num, 2, '.');
		AfxExtractSubString(emp_num, emp_num, 0, '.');
		funds_sort.Format(L"%s%s%s", emp_num, year, month);

		sql.Format(L"SELECT * FROM FUNDS WHERE FUNDS_DETAIL_NUM = '%s'", funds_sort);
		recSet.Open(CRecordset::dynaset, sql);
		if (!recSet.IsEOF()) {
			recSet.Close();
			MessageBox(L"이미 월급을 지급하였습니다.");
		}
		else { // 존재하지 않는 경우 (해당 달의 월급을 지급하지 않은 경우)
			recSet.Close();

			hour = 0;
			minute = 0;
			salary = 0;
			date_start.Format(L"%s%s00", year, month);
			date_end.Format(L"%s%s32", year, month);

			sql.Format(L"SELECT WORKTIME FROM WORK_HISTORY WHERE EMP_NUM='%s' AND WORKTIME_DATE BETWEEN '%s' AND '%s'", emp_num, date_start, date_end);
			recSet.Open(CRecordset::dynaset, sql);

			// 총 근무시간 구하기
			while (!recSet.IsEOF())
			{
				recSet.GetFieldValue(_T("WORKTIME"), s_hour);
				AfxExtractSubString(s_minute, s_hour, 1, '.');
				AfxExtractSubString(s_hour, s_hour, 0, '.');

				if (s_minute == '5' && minute == 5) {
					minute = 0;
					hour++;
				}
				else if (s_minute == '5')
					minute = 5;

				hour = hour + _ttoi(s_hour);

				//다음 레코드로 이동
				recSet.MoveNext();
			}
			recSet.Close();
			s_hour.Format(L"%d.%d", hour, minute);
			m_empsalary_time.SetWindowTextW(s_hour);

			// 월급 계산하기
			sql.Format(L"SELECT SORT_DETAIL FROM SORT_CODE WHERE SORT_CODE = '%s'", rank);
			recSet.Open(CRecordset::dynaset, sql);
			if (!recSet.IsEOF()) {
				recSet.GetFieldValue(_T("SORT_DETAIL"), s_salary);
				if (minute == 5)
					salary = _ttoi(s_salary) / 2;
				salary = salary + ( _ttoi(s_salary) *hour );
			}
			recSet.Close();
			s_salary.Format(L"%d", salary);
			m_empsalary_salary.SetWindowTextW(s_salary);
		}
	}
	else
		MessageBox(L"모든 항목을 선택해주세요.");
}


void EmpSalary::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// 월급지급
	CRecordset recSet(&db_empsalary);
	CString sql, emp_num, year, month, sort_code, sort_detail, salary, today;
	CTime cTime;

	// 정보 받아오기
	this->m_empsalary_emp.GetLBText(this->m_empsalary_emp.GetCurSel(), emp_num);
	this->m_empsalary_year.GetLBText(this->m_empsalary_year.GetCurSel(), year);
	this->m_empsalary_month.GetLBText(this->m_empsalary_month.GetCurSel(), month);
	m_empsalary_salary.GetWindowTextW(salary);

	if (salary.IsEmpty() == FALSE && salary != '0') {

		sql.Format(L"SELECT SORT_CODE FROM SORT_CODE WHERE SORT_DETAIL = '직원월급'");
		recSet.Open(CRecordset::dynaset, sql);
		if (!recSet.IsEOF()) {
			recSet.GetFieldValue(_T("SORT_CODE"), sort_code);
		}
		recSet.Close();

		AfxExtractSubString(emp_num, emp_num, 0, '.');
		sort_detail.Format(L"%s%s%s", emp_num, year, month);
		cTime = CTime::GetCurrentTime(); // 현재 날짜 및 시간
		today.Format(L"%04d%02d%02d%02d%02d%02d", cTime.GetYear(), cTime.GetMonth(), cTime.GetDay(), cTime.GetHour(), cTime.GetMinute(), cTime.GetSecond());

		sql.Format(L"INSERT INTO FUNDS(FUNDS_DATE, FUNDS_DETAIL_NUM, PRICE, FUNDS_SORT_CODE) values ('%s', '%s', '%d', '%s')", today, sort_detail, _ttoi(salary), sort_code);
		db_empsalary.ExecuteSQL(sql);
		MessageBox(L"월급지급 완료");
		CDialogEx::OnOK();
	}
	else if (salary.IsEmpty() == TRUE)
		MessageBox(L"지급을 했거나 선택 항목을 변경했을 경우 다시 월급확인을 해주십시오");
	else
		MessageBox(L"지급할 월급이 없습니다.");
}


void EmpSalary::OnCbnSelchangeCombo3()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_empsalary_time.SetWindowTextW(L"");
	m_empsalary_salary.SetWindowTextW(L"");
}


void EmpSalary::OnCbnSelchangeCombo2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_empsalary_time.SetWindowTextW(L"");
	m_empsalary_salary.SetWindowTextW(L"");
}


void EmpSalary::OnCbnSelchangeCombo1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_empsalary_time.SetWindowTextW(L"");
	m_empsalary_salary.SetWindowTextW(L"");
}
