#pragma once
#include "afxwin.h"


// NewEvent ��ȭ �����Դϴ�.

class NewEvent : public CDialogEx
{
	DECLARE_DYNAMIC(NewEvent)

public:
	NewEvent(CWnd* pParent = NULL, CString num = NULL);   // ǥ�� �������Դϴ�.
	virtual ~NewEvent();
	CDatabase db_newevent;
	// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_NEW_EVENT };


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

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
