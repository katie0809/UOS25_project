#pragma once
#include "afxwin.h"


// CNewCustomer 대화 상자입니다.

class CNewCustomer : public CDialogEx
{
	DECLARE_DYNAMIC(CNewCustomer)

public:
	CNewCustomer(int count, CWnd* pParent);   // 표준 생성자입니다.
	virtual ~CNewCustomer();

// 대화 상자 데이터입니다.
//#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_NEW_MEMBER };
//#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CString member_num;
	CEdit mem_name;
	CEdit mem_phone;
	CStatic mem_num;
	afx_msg void OnBnClickedOk();
};
