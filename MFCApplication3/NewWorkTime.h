#pragma once
#include "afxwin.h"


// NewWorkTime ��ȭ �����Դϴ�.

class NewWorkTime : public CDialogEx
{
	DECLARE_DYNAMIC(NewWorkTime)

public:
	NewWorkTime(CWnd* pParent = NULL, CString code = NULL, CString date = NULL);   // ǥ�� �������Դϴ�.
	virtual ~NewWorkTime();
	CDatabase db_worktime;
	// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_NEW_WORKTIME };


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CComboBox m_worktime_year;
	CComboBox m_worktime_month;
	CComboBox m_worktime_day;
	CComboBox m_worktime_hour;
	CComboBox m_worktime_minute;
	CString emp_code, worktime_date;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButton1();
};
