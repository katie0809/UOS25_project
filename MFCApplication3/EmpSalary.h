#pragma once
#include "afxwin.h"


// EmpSalary 대화 상자입니다.

class EmpSalary : public CDialogEx
{
	DECLARE_DYNAMIC(EmpSalary)

public:
	EmpSalary(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~EmpSalary();
	CDatabase db_empsalary;
	// 대화 상자 데이터입니다.
	enum { IDD = IDD_EMP_SALARY };


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	void ShowData();
	CComboBox m_empsalary_emp;
	CComboBox m_empsalary_month;
	CEdit m_empsalary_time;
	CEdit m_empsalary_salary;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedOk();
	CComboBox m_empsalary_year;
	afx_msg void OnCbnSelchangeCombo3();
	afx_msg void OnCbnSelchangeCombo2();
	afx_msg void OnCbnSelchangeCombo1();
};
