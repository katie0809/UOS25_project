#pragma once
#include "afxwin.h"


// InfoDisuse ��ȭ �����Դϴ�.

class InfoDisuse : public CDialogEx
{
	DECLARE_DYNAMIC(InfoDisuse)

public:
	InfoDisuse(CWnd* pParent = NULL, CString date = NULL, CString num = NULL);   // ǥ�� �������Դϴ�.
	virtual ~InfoDisuse();
	CDatabase db_infodisuse;
	// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_INFO_DISUSE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

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
