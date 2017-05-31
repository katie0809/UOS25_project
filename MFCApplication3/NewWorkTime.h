#pragma once
#include "afxwin.h"


// NewWorkTime 대화 상자입니다.

class NewWorkTime : public CDialogEx
{
	DECLARE_DYNAMIC(NewWorkTime)

public:
	NewWorkTime(CWnd* pParent = NULL, CString code = NULL, CString date = NULL);   // 표준 생성자입니다.
	virtual ~NewWorkTime();
	CDatabase db_worktime;
	// 대화 상자 데이터입니다.
	enum { IDD = IDD_NEW_WORKTIME };


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

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
