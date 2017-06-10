#pragma once
#include "afxcmn.h"
#include "NewEvent.h"


// EventDetail ��ȭ �����Դϴ�.

class EventDetail : public CDialogEx
{
	DECLARE_DYNAMIC(EventDetail)

public:
	EventDetail(CWnd* pParent = NULL, CString person_type = NULL);   // ǥ�� �������Դϴ�.
	virtual ~EventDetail();
	CDatabase db_eventdetail;
	NewEvent * dlg_new_event;
	// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_EVENT_DETAIL };


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

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
