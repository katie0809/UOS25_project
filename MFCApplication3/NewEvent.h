#pragma once
#include "afxwin.h"


// NewEvent 대화 상자입니다.

class NewEvent : public CDialogEx
{
	DECLARE_DYNAMIC(NewEvent)

public:
	NewEvent(CWnd* pParent = NULL, CString num = NULL);   // 표준 생성자입니다.
	virtual ~NewEvent();
	CDatabase db_newevent;
	// 대화 상자 데이터입니다.
	enum { IDD = IDD_NEW_EVENT };


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	void ShowData();
	CEdit m_newevent_name;
	CComboBox m_newevent_discount;
	CEdit m_newevent_detail;
	CString event_num;
	afx_msg void OnBnClickedOk();
};
