// EventDetail.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MFCApplication3.h"
#include "EventDetail.h"
#include "afxdialogex.h"


// EventDetail 대화 상자입니다.

IMPLEMENT_DYNAMIC(EventDetail, CDialogEx)

EventDetail::EventDetail(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_EVENT_DETAIL, pParent)
{

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
END_MESSAGE_MAP()


// EventDetail 메시지 처리기입니다.

BOOL EventDetail::OnInitDialog() {
	CDialogEx::OnInitDialog();
	CRecordset recSet(&db_eventdetail);
	CString sql, event_code, event_name, discount, detail;

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

		m_event_list.InsertColumn(0, L"이벤트 코드", LVCFMT_CENTER, 70);
	m_event_list.InsertColumn(1, L"이름", LVCFMT_CENTER, 150);
	m_event_list.InsertColumn(2, L"할인여부", LVCFMT_CENTER, 70);
	m_event_list.InsertColumn(3, L"내용", LVCFMT_CENTER, 150);

	sql.Format(L"SELECT EVENT_CODE, EVENT_NAME, DISCOUNT_AGREE, DETAIL FROM EVENT");
	recSet.Open(CRecordset::dynaset, sql);

	while (!recSet.IsEOF())
	{
		recSet.GetFieldValue(_T("EVENT_CODE"), event_code);
		recSet.GetFieldValue(_T("EVENT_NAME"), event_name);
		recSet.GetFieldValue(_T("DISCOUNT_AGREE"), discount);
		recSet.GetFieldValue(_T("DETAIL"), detail);

		// Insert itm into list
		int nListitm = m_event_list.InsertItem(0, event_code, 0);
		m_event_list.SetItem(nListitm, 1, LVFIF_TEXT, event_name, 0, 0, 0, NULL);
		m_event_list.SetItem(nListitm, 2, LVFIF_TEXT, discount, 0, 0, 0, NULL);
		m_event_list.SetItem(nListitm, 3, LVFIF_TEXT, detail, 0, 0, 0, NULL);

		//다음 레코드로 이동
		recSet.MoveNext();
	}
	recSet.Close();


	return TRUE;  // return TRUE unless you set the focus to a control
}
