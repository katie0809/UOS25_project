// InfoEmp.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "MFCApplication3.h"
#include "InfoEmp.h"
#include "afxdialogex.h"


// InfoEmp ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(InfoEmp, CDialogEx)

InfoEmp::InfoEmp(CWnd* pParent, CString selected_emp/*=NULL*/)
	: CDialogEx(IDD_INFO_EMP, pParent)
{
	emp_code = selected_emp;
}

InfoEmp::~InfoEmp()
{
}

void InfoEmp::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, 1019, m_infoemp_name);
	DDX_Control(pDX, 1026, m_infoemp_sex);
	DDX_Control(pDX, 1022, m_infoemp_phone);
	DDX_Control(pDX, 1025, m_infoemp_rank);
	DDX_Control(pDX, 1020, m_infoemp_age);
	DDX_Control(pDX, 1023, m_infoemp_bank);
	DDX_Control(pDX, 1000, m_worktime_list);
}


BEGIN_MESSAGE_MAP(InfoEmp, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &InfoEmp::OnBnClickedButton1)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &InfoEmp::OnNMDblclkList1)
END_MESSAGE_MAP()


// InfoEmp �޽��� ó�����Դϴ�.


BOOL InfoEmp::OnInitDialog() {
	CDialogEx::OnInitDialog();

	// �����ͺ��̽����� ����

	TRY
	{
		db_infoemp.OpenEx(_T("DSN=UOS25;UID=UOS25;PWD=0000"));
	}
		CATCH(CException, e)
	{
		TCHAR errMSG[255];

		e->GetErrorMessage(errMSG, 255);
		AfxMessageBox(errMSG, MB_ICONERROR);


		if (db_infoemp.IsOpen())
			db_infoemp.Close();

	}
	END_CATCH

		// �������� ����
		m_infoemp_name.SetLimitText(10);
	m_infoemp_age.SetLimitText(2);
	m_infoemp_phone.SetLimitText(11);
	m_infoemp_bank.SetLimitText(20);

	ShowData(db_infoemp);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}

void InfoEmp::ShowData(CDatabase & db_infoemp) {
	CRecordset recSet(&db_infoemp);
	CString sql, db_code, name, rank, age, sex, phone, bank, combo_rank, date, time;

	sql.Format(L"SELECT * FROM EMPLOYEE WHERE EMP_CODE='%s'", emp_code);
	recSet.Open(CRecordset::dynaset, sql);

	if (!recSet.IsEOF())
	{
		recSet.GetFieldValue(_T("RANK"), rank);
		recSet.GetFieldValue(_T("NAME"), name);
		recSet.GetFieldValue(_T("AGE"), age);
		recSet.GetFieldValue(_T("SEX"), sex);
		recSet.GetFieldValue(_T("PHONE"), phone);
		recSet.GetFieldValue(_T("BANK"), bank);
	}
	recSet.Close();

	sql.Format(L"SELECT SORT_CODE FROM SORT_CODE WHERE HIGH_SORT_CODE=(SELECT SORT_CODE FROM SORT_CODE WHERE SORT_DETAIL='�����ڵ�')");
	recSet.Open(CRecordset::dynaset, sql);
	while (!recSet.IsEOF())
	{
		recSet.GetFieldValue(_T("SORT_CODE"), combo_rank);
		m_infoemp_rank.AddString(combo_rank);

		//���� ���ڵ�� �̵�
		recSet.MoveNext();
	}
	recSet.Close();

	m_infoemp_sex.AddString(_T("����"));
	m_infoemp_sex.AddString(_T("����"));

	// �� ����
	if (sex == 'M')
		m_infoemp_sex.SetCurSel(0);
	else
		m_infoemp_sex.SetCurSel(1);

	for (int i = 0; i < 4; i++) {
		m_infoemp_rank.GetLBText(i, combo_rank);
		if (combo_rank == rank) {
			m_infoemp_rank.SetCurSel(i);
			break;
		}
	}

	m_infoemp_name.SetWindowTextW(name);
	m_infoemp_age.SetWindowTextW(age);
	m_infoemp_phone.SetWindowTextW(phone);
	m_infoemp_bank.SetWindowTextW(bank);

	// �ٹ��̷� worktime
	m_worktime_list.ModifyStyle(0, LVS_REPORT | LVS_SHOWSELALWAYS | LVS_SINGLESEL, 0);
	m_worktime_list.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	m_worktime_list.InsertColumn(0, L"��¥", LVCFMT_CENTER, 150);
	m_worktime_list.InsertColumn(1, L"���� �ð�", LVCFMT_CENTER, 150);

	sql.Format(L"SELECT WORKTIME_DATE, WORKTIME FROM WORKHISTORY WHERE EMP_CODE='%s'", emp_code);
	recSet.Open(CRecordset::dynaset, sql);
	while (!recSet.IsEOF())
	{
		recSet.GetFieldValue(_T("WORKTIME_DATE"), date);
		recSet.GetFieldValue(_T("WORKTIME"), time);


		// Insert itm into list
		int nListitm = m_worktime_list.InsertItem(0, date, 0);
		m_worktime_list.SetItem(nListitm, 1, LVFIF_TEXT, time, 0, 0, 0, NULL);

		//���� ���ڵ�� �̵�
		recSet.MoveNext();
	}
	recSet.Close();

	// ����� ���
	if (rank == "���") {
		GetDlgItem(IDC_BUTTON1)->ShowWindow(FALSE);
		GetDlgItem(IDOK)->SetWindowText(TEXT("����"));
		rank_out.Format(L"���");
	}
}

void InfoEmp::OnBnClickedOk()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CRecordset recSet(&db_infoemp);
	CString emp_code_string, name, rank, age, sex, phone, bank, sql;

	this->m_infoemp_rank.GetLBText(this->m_infoemp_rank.GetCurSel(), rank);
	m_infoemp_name.GetWindowTextW(name);
	m_infoemp_age.GetWindowTextW(age);
	this->m_infoemp_sex.GetLBText(this->m_infoemp_sex.GetCurSel(), sex);
	m_infoemp_phone.GetWindowTextW(phone);
	m_infoemp_bank.GetWindowTextW(bank);

	if (rank_out == "���") {
		CString sql;
		sql.Format(L"DELETE FROM EMPLOYEE WHERE EMP_CODE='%s'", emp_code);
		db_infoemp.ExecuteSQL(sql);
		MessageBox(L"���� ���� �Ϸ�");
		CDialogEx::OnOK();
	}
	else if (name.IsEmpty() == FALSE && rank.IsEmpty() == FALSE && age.IsEmpty() == FALSE && sex.IsEmpty() == FALSE && phone.IsEmpty() == FALSE && bank.IsEmpty() == FALSE) {

		if (sex == "����")
			sex = 'M';
		else
			sex = 'F';

		sql.Format(L"UPDATE EMPLOYEE SET RANK='%s', NAME='%s', AGE='%s', SEX='%s', PHONE='%s', BANK='%s' WHERE EMP_CODE='%s'", rank, name, age, sex, phone, bank, emp_code);
		db_infoemp.ExecuteSQL(sql);
		MessageBox(L"���� ���� �Ϸ�");
		CDialogEx::OnOK();
	}
	else
		MessageBox(L"��� �׸��� �Է��� �ֽʽÿ�");
}


void InfoEmp::OnBnClickedButton1()
{
	// �ٹ� �̷� �߰� ��ư
	dlg_new_work_time = new NewWorkTime(this, emp_code, NULL);
	dlg_new_work_time->Create(NewWorkTime::IDD);
	dlg_new_work_time->ShowWindow(SW_SHOW);
}

void InfoEmp::OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>( pNMHDR );
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	if (pNMItemActivate->iItem != -1) {
		// ���õ� �������� �ε��� ��ȣ�� ��´�
		NM_LISTVIEW * pNMListView = (NM_LISTVIEW*) pNMHDR;
		int cur_idx = pNMListView->iItem;

		CString WORKTIME_DATE;
		WORKTIME_DATE = m_worktime_list.GetItemText(cur_idx, 0);

		// Open dialog using currently selected index num
		dlg_new_work_time = new NewWorkTime(this, emp_code, WORKTIME_DATE);
		dlg_new_work_time->Create(NewWorkTime::IDD);
		dlg_new_work_time->ShowWindow(SW_SHOW);
	}
	else {}
	*pResult = 0;
}
