#pragma once
#include "afxwin.h"


// ManageWage ��ȭ �����Դϴ�.

class ManageWage : public CDialogEx
{
	DECLARE_DYNAMIC(ManageWage)

public:
	ManageWage(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~ManageWage();
	enum { IDD = IDD_DIALOG3 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

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
