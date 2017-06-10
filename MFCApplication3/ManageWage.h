#pragma once
#include "afxwin.h"


// ManageWage 대화 상자입니다.

class ManageWage : public CDialogEx
{
	DECLARE_DYNAMIC(ManageWage)

public:
	ManageWage(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~ManageWage();
	enum { IDD = IDD_DIALOG3 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CComboBox employee_combo;
	CStatic title_text;
	CStatic worktime_text;
	CStatic wage_text;
	afx_msg void OnCbnSelchangeCombo1();
	virtual BOOL OnInitDialog();
	CDatabase db_wage;

	CString name;
	CString title;
	CString work_hours;
	CString the_wage;
	CString emp_code;
	CString last_date;
	CString total_wage;


	afx_msg void OnBnClickedOk();
};
