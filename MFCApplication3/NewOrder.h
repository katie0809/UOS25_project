#pragma once
#include "afxwin.h"


// NewOrder 대화 상자입니다.

class NewOrder : public CDialogEx
{
	DECLARE_DYNAMIC(NewOrder)

public:
	NewOrder(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~NewOrder();
	CDatabase db_neworder;

// 대화 상자 데이터입니다.
	enum { IDD = IDD_NEW_ORDER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	void ShowData(CDatabase & db_neworder);
	CCheckListBox m_neworderList;
	afx_msg void OnBnClickedOrder();
	afx_msg void OnBnClickedOk();
};
