#pragma once
#include "afxcmn.h"


// EventDetail ��ȭ �����Դϴ�.

class EventDetail : public CDialogEx
{
	DECLARE_DYNAMIC(EventDetail)

public:
	EventDetail(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~EventDetail();
	CDatabase db_eventdetail;
	// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_EVENT_DETAIL };


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CListCtrl m_event_list;
};
