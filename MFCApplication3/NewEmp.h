#pragma once
#include "afxwin.h"


// NewEmp 대화 상자입니다.

class NewEmp : public CDialogEx
{
	DECLARE_DYNAMIC(NewEmp)

public:
	NewEmp(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~NewEmp();
	CDatabase db_newemp;
	// 대화 상자 데이터입니다.
	enum { IDD = IDD_NEWEMP };


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	void ShowData();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();

	CListCtrl m_newEmp;
	/*	CComboBox m_newemp_rank;
	CComboBox m_newemp_sex;
	CEdit m_newemp_name;
	CEdit m_newemp_age;
	CEdit m_newemp_phone;
	CEdit m_newemp_bank;*/
	CEdit m_newemp_name;
	CComboBox m_newemp_rank;
	CEdit m_newemp_age;
	CComboBox m_newemp_sex;
	CEdit m_newemp_phone;
	CEdit m_newemp_bank;
};
