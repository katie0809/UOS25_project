// NewEvent.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "MFCApplication3.h"
#include "NewEvent.h"
#include "afxdialogex.h"


// NewEvent ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(NewEvent, CDialogEx)

NewEvent::NewEvent(CWnd* pParent, CString num /*=NULL*/)
	: CDialogEx(IDD_NEW_EVENT, pParent)
{
	event_num = num;
}

NewEvent::~NewEvent()
{
}

void NewEvent::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, 1019, m_newevent_name);
	DDX_Control(pDX, 1020, m_newevent_discount);
	DDX_Control(pDX, 1021, m_newevent_detail);
}


BEGIN_MESSAGE_MAP(NewEvent, CDialogEx)
	ON_BN_CLICKED(IDOK, &NewEvent::OnBnClickedOk)
END_MESSAGE_MAP()


// NewEvent �޽��� ó�����Դϴ�.

BOOL NewEvent::OnInitDialog() {
	CDialogEx::OnInitDialog();

	// �����ͺ��̽����� ����

	TRY
	{
		db_newevent.OpenEx(_T("DSN=UOS25;UID=UOS25;PWD=0000"));
	}
		CATCH(CException, e)
	{
		TCHAR errMSG[255];

		e->GetErrorMessage(errMSG, 255);
		AfxMessageBox(errMSG, MB_ICONERROR);


		if (db_newevent.IsOpen())
			db_newevent.Close();

	}
	END_CATCH

		m_newevent_discount.AddString(L"Y");
	m_newevent_discount.AddString(L"N");

	// �������� �߰�����
	if (event_num.IsEmpty())
		m_newevent_discount.SetCurSel(0);
	else {
		ShowData();
		GetDlgItem(IDOK)->SetWindowText(TEXT("����"));
	}
	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}

void NewEvent::ShowData() {
	CRecordset recSet(&db_newevent);
	CString sql, name, discount, detail, s_discount;

	sql.Format(L"SELECT EVENT_NAME, DISCOUNT_AGREE, DETAIL FROM EVENT WHERE EVENT_NUM = '%s'", event_num);
	recSet.Open(CRecordset::dynaset, sql);

	// �̺�Ʈ ���� �Է� (�ʱ�ȭ)
	if (!recSet.IsEOF())
	{
		recSet.GetFieldValue(_T("EVENT_NAME"), name);
		recSet.GetFieldValue(_T("DISCOUNT_AGREE"), discount);
		recSet.GetFieldValue(_T("DETAIL"), detail);

		m_newevent_name.SetWindowTextW(name);
		m_newevent_detail.SetWindowTextW(detail);

		for (int i = 0; i < m_newevent_discount.GetCount(); i++) {
			m_newevent_discount.GetLBText(i, s_discount);
			if (s_discount == discount)
				m_newevent_discount.SetCurSel(i);
		}
	}
	recSet.Close();
}


void NewEvent::OnBnClickedOk()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CRecordset recSet(&db_newevent);
	CString sql, name, detail, discount, event_num_string;
	int new_event_num;

	this->m_newevent_discount.GetLBText(this->m_newevent_discount.GetCurSel(), discount);
	m_newevent_name.GetWindowTextW(name);
	m_newevent_detail.GetWindowTextW(detail);

	// �̺�Ʈ �߰�
	if (event_num.IsEmpty()) {
		sql.Format(L"SELECT EVENT_NUM FROM EVENT ORDER BY EVENT_NUM DESC");
		recSet.Open(CRecordset::dynaset, sql);

		if (!recSet.IsEOF())
		{
			recSet.GetFieldValue(_T("EVENT_NUM"), event_num_string);
			event_num_string = event_num_string.Mid(1, 3);
			new_event_num = _ttoi(event_num_string);
			new_event_num++;
		}
		else
			new_event_num = 1;
		recSet.Close();

		if (!name.IsEmpty() && !detail.IsEmpty()) {

			event_num_string.Format(L"C%03d", new_event_num);
			sql.Format(L"INSERT INTO EVENT(EVENT_NUM, EVENT_NAME, DISCOUNT_AGREE, DETAIL) values ('%s', '%s', '%s', '%s')", event_num_string, name, discount, detail);
			db_newevent.ExecuteSQL(sql);
			MessageBox(L"�̺�Ʈ �߰� �Ϸ�");
			CDialogEx::OnOK();
		}
		else
			MessageBox(L"��� �׸��� �Է��� �ֽʽÿ�");
	}
	else { // �̺�Ʈ ����
		if (!name.IsEmpty() && !detail.IsEmpty()) {

			sql.Format(L"UPDATE EVENT SET EVENT_NAME='%s', DISCOUNT_AGREE='%s', DETAIL='%s' WHERE EVENT_NUM='%s'", name, discount, detail, event_num);
			db_newevent.ExecuteSQL(sql);
			MessageBox(L"�̺�Ʈ ���� �Ϸ�");
			CDialogEx::OnOK();
		}
		else
			MessageBox(L"��� �׸��� �Է��� �ֽʽÿ�");
	}
	CDialogEx::OnOK();
}
