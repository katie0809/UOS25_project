// NewEvent.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MFCApplication3.h"
#include "NewEvent.h"
#include "afxdialogex.h"


// NewEvent 대화 상자입니다.

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


// NewEvent 메시지 처리기입니다.

BOOL NewEvent::OnInitDialog() {
	CDialogEx::OnInitDialog();

	// 데이터베이스와의 연결

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

	// 수정인지 추가인지
	if (event_num.IsEmpty())
		m_newevent_discount.SetCurSel(0);
	else {
		ShowData();
		GetDlgItem(IDOK)->SetWindowText(TEXT("수정"));
	}
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void NewEvent::ShowData() {
	CRecordset recSet(&db_newevent);
	CString sql, name, discount, detail, s_discount;

	sql.Format(L"SELECT EVENT_NAME, DISCOUNT_AGREE, DETAIL FROM EVENT WHERE EVENT_NUM = '%s'", event_num);
	recSet.Open(CRecordset::dynaset, sql);

	// 이벤트 정보 입력 (초기화)
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
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CRecordset recSet(&db_newevent);
	CString sql, name, detail, discount, event_num_string;
	int new_event_num;

	this->m_newevent_discount.GetLBText(this->m_newevent_discount.GetCurSel(), discount);
	m_newevent_name.GetWindowTextW(name);
	m_newevent_detail.GetWindowTextW(detail);

	// 이벤트 추가
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
			MessageBox(L"이벤트 추가 완료");
			CDialogEx::OnOK();
		}
		else
			MessageBox(L"모든 항목을 입력해 주십시오");
	}
	else { // 이벤트 수정
		if (!name.IsEmpty() && !detail.IsEmpty()) {

			sql.Format(L"UPDATE EVENT SET EVENT_NAME='%s', DISCOUNT_AGREE='%s', DETAIL='%s' WHERE EVENT_NUM='%s'", name, discount, detail, event_num);
			db_newevent.ExecuteSQL(sql);
			MessageBox(L"이벤트 수정 완료");
			CDialogEx::OnOK();
		}
		else
			MessageBox(L"모든 항목을 입력해 주십시오");
	}
	CDialogEx::OnOK();
}
