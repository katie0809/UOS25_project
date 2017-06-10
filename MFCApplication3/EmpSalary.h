#pragma once
#include "afxwin.h"


// EmpSalary ��ȭ �����Դϴ�.

class EmpSalary : public CDialogEx
{
	DECLARE_DYNAMIC(EmpSalary)

public:
	EmpSalary(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~EmpSalary();
	CDatabase db_empsalary;
	// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_EMP_SALARY };


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

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
