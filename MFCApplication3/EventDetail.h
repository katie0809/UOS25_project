#pragma once
#include "afxcmn.h"
#include "NewEvent.h"


// EventDetail 대화 상자입니다.

class EventDetail : public CDialogEx
{
	DECLARE_DYNAMIC(EventDetail)

public:
	EventDetail(CWnd* pParent = NULL, CString person_type = NULL);   // 표준 생성자입니다.
	virtual ~EventDetail();
	CDatabase db_eventdetail;
	NewEvent * dlg_new_event;
	// 대화 상자 데이터입니다.
	enum { IDD = IDD_EVENT_DETAIL };


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	void ShowData();
	CListCtrl m_event_list;
	CString type;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnNMDblclkList2(NMHDR *pNMHDR, LRESULT *pResult);
	int m_Selected_listItem;
	afx_msg void OnBnClickedButton2();
	afx_msg void OnNMClickList2(NMHDR *pNMHDR, LRESULT *pResult);
};
