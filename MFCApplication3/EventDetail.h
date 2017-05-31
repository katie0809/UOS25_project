#pragma once
#include "afxcmn.h"


// EventDetail 대화 상자입니다.

class EventDetail : public CDialogEx
{
	DECLARE_DYNAMIC(EventDetail)

public:
	EventDetail(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~EventDetail();
	CDatabase db_eventdetail;
	// 대화 상자 데이터입니다.
	enum { IDD = IDD_EVENT_DETAIL };


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CListCtrl m_event_list;
};
