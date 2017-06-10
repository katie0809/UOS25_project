#pragma once
#include "afxwin.h"


// NewSortCode ��ȭ �����Դϴ�.

class NewSortCode : public CDialogEx
{
	DECLARE_DYNAMIC(NewSortCode)

public:
	NewSortCode(CWnd* pParent = NULL, CString code = NULL);   // ǥ�� �������Դϴ�.
	virtual ~NewSortCode();
	CDatabase db_newsortcode;
	// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_NEW_SORT_CODE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	void ShowData();
	CEdit m_newsortcode_code;
	CComboBox m_newsortcode_high;
	CEdit m_newsortcode_detail;
	afx_msg void OnBnClickedOk();
	CString sort_code;
};
