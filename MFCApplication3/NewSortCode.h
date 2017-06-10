#pragma once
#include "afxwin.h"


// NewSortCode 대화 상자입니다.

class NewSortCode : public CDialogEx
{
	DECLARE_DYNAMIC(NewSortCode)

public:
	NewSortCode(CWnd* pParent = NULL, CString code = NULL);   // 표준 생성자입니다.
	virtual ~NewSortCode();
	CDatabase db_newsortcode;
	// 대화 상자 데이터입니다.
	enum { IDD = IDD_NEW_SORT_CODE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

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
