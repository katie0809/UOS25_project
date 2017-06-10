#pragma once
#include "afxwin.h"


// NewEmp ��ȭ �����Դϴ�.

class NewEmp : public CDialogEx
{
	DECLARE_DYNAMIC(NewEmp)

public:
	NewEmp(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~NewEmp();
	CDatabase db_newemp;
	// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_NEWEMP };


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

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
