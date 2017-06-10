// EventDetail.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MFCApplication3.h"
#include "EventDetail.h"
#include "afxdialogex.h"


// EventDetail 대화 상자입니다.

IMPLEMENT_DYNAMIC(EventDetail, CDialogEx)

EventDetail::EventDetail(CWnd* pParent, CString person_type/*=NULL*/)
	: CDialogEx(IDD_EVENT_DETAIL, pParent)
{
	type = person_type;
}

EventDetail::~EventDetail()
{
}

void EventDetail::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST2, m_event_list);
}


BEGIN_MESSAGE_MAP(EventDetail, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &EventDetail::OnBnClickedButton1)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST2, &EventDetail::OnNMDblclkList2)
	ON_BN_CLICKED(IDC_BUTTON2, &EventDetail::OnBnClickedButton2)
	ON_NOTIFY(NM_CLICK, IDC_LIST2, &EventDetail::OnNMClickList2)
END_MESSAGE_MAP()


// EventDetail 메시지 처리기입니다.

BOOL EventDetail::OnInitDialog() {
	CDialogEx::OnInitDialog();
	CString event_num, event_name, discount, detail;

	// 데이터베이스와의 연결

	TRY{
		db_eventdetail.OpenEx(_T("DSN=UOS25;UID=UOS25;PWD=0000"));
	}
		CATCH(CException, e)
	{
		TCHAR errMSG[255];

		e->GetErrorMessage(errMSG, 255);
		AfxMessageBox(errMSG, MB_ICONERROR);


		if (db_eventdetail.IsOpen())
			db_eventdetail.Close();

	}
	END_CATCH

		m_Selected_listItem = -1;

	m_event_list.ModifyStyle(0, LVS_REPORT | LVS_SHOWSELALWAYS | LVS_SINGLESEL, 0);
	m_event_list.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	m_event_list.InsertColumn(0, L"이벤트 번호", LVCFMT_CENTER, 70);
	m_event_list.InsertColumn(1, L"이름", LVCFMT_CENTER, 150);
	m_event_list.InsertColumn(2, L"할인여부", LVCFMT_CENTER, 70);
	m_event_list.InsertColumn(3, L"내용", LVCFMT_CENTER, 250);

	// 자격이 없는 경우
	if (type.IsEmpty()) {
		GetDlgItem(IDC_BUTTON1)->ShowWindow(FALSE);
		GetDlgItem(IDC_BUTTON2)->ShowWindow(FALSE);
	}

	ShowData();
	return TRUE;  // return TRUE unless you set the focus to a control
}

void EventDetail::ShowData()
{
	CRecordset recSet(&db_eventdetail);
	CString sql, event_num, event_name, discount, detail;

	m_event_list.DeleteAllItems();

	sql.Format(L"SELECT EVENT_NUM, EVENT_NAME, DISCOUNT_AGREE, DETAIL FROM EVENT");
	recSet.Open(CRecordset::dynaset, sql);

	while (!recSet.IsEOF())
	{
		recSet.GetFieldValue(_T("EVENT_NUM"), event_num);
		recSet.GetFieldValue(_T("EVENT_NAME"), event_name);
		recSet.GetFieldValue(_T("DISCOUNT_AGREE"), discount);
		recSet.GetFieldValue(_T("DETAIL"), detail);

		// Insert itm into list
		int nListitm = m_event_list.InsertItem(0, event_num, 0);
		m_event_list.SetItem(nListitm, 1, LVFIF_TEXT, event_name, 0, 0, 0, NULL);
		m_event_list.SetItem(nListitm, 2, LVFIF_TEXT, discount, 0, 0, 0, NULL);
		m_event_list.SetItem(nListitm, 3, LVFIF_TEXT, detail, 0, 0, 0, NULL);

		//다음 레코드로 이동
		recSet.MoveNext();
	}
	recSet.Close();
}



void EventDetail::OnBnClickedButton1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (!type.IsEmpty()) {
		dlg_new_event = new NewEvent(this, NULL);
		dlg_new_event->Create(NewEvent::IDD);
		dlg_new_event->ShowWindow(SW_SHOW);
	}
}


void EventDetail::OnNMDblclkList2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>( pNMHDR );
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (!type.IsEmpty()) {
		if (pNMItemActivate->iItem != -1) {
			// 선택된 아이템의 인덱스 번호를 얻는다
			NM_LISTVIEW * pNMListView = (NM_LISTVIEW*) pNMHDR;
			int cur_idx = pNMListView->iItem;

			CString event_num;
			event_num = m_event_list.GetItemText(cur_idx, 0);

			// Open dialog using currently selected index num
			dlg_new_event = new NewEvent(this, event_num);
			dlg_new_event->Create(NewEvent::IDD);
			dlg_new_event->ShowWindow(SW_SHOW);
		}
		else
			MessageBox(L"이벤트를 선택하세요");
	}
	*pResult = 0;
}

void EventDetail::OnNMClickList2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>( pNMHDR );
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_Selected_listItem = pNMItemActivate->iItem;
	*pResult = 0;
}

void EventDetail::OnBnClickedButton2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (m_Selected_listItem < 0)
		MessageBox(L"이벤트를 선택하세요");
	else {
		CString  event_num, sql;
		event_num = m_event_list.GetItemText(m_Selected_listItem, 0);

		sql.Format(L"DELETE FROM EVENT WHERE EVENT_NUM='%s'", event_num);
		db_eventdetail.ExecuteSQL(sql);
		MessageBox(L"이벤트 삭제 완료");

		ShowData();
	}
}
