#pragma once
#include "afxwin.h"


// CNewCustomer ��ȭ �����Դϴ�.

class CNewCustomer : public CDialogEx
{
	DECLARE_DYNAMIC(CNewCustomer)

public:
	CNewCustomer(int count, CWnd* pParent);   // ǥ�� �������Դϴ�.
	virtual ~CNewCustomer();

// ��ȭ ���� �������Դϴ�.
//#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_NEW_MEMBER };
//#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	CString member_num;
	CEdit mem_name;
	CEdit mem_phone;
	CStatic mem_num;
	afx_msg void OnBnClickedOk();
};
