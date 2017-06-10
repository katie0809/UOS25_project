#pragma once
#include "afxwin.h"


// InfoDisuse 대화 상자입니다.

class InfoDisuse : public CDialogEx
{
	DECLARE_DYNAMIC(InfoDisuse)

public:
	InfoDisuse(CWnd* pParent = NULL, CString date = NULL, CString num = NULL);   // 표준 생성자입니다.
	virtual ~InfoDisuse();
	CDatabase db_infodisuse;
	// 대화 상자 데이터입니다.
	enum { IDD = IDD_INFO_DISUSE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	void ShowData();
	CEdit m_infodisuse_date;
	CEdit m_infodisuse_prodnum;
	CEdit m_infodisuse_name;
	CEdit m_infodisuse_maker;
	CEdit m_infodisuse_amount;
	CString disuse_date, prod_num;
	afx_msg void OnBnClickedButton1();
};
