#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include <vector>
#include <string>
using namespace std;

// NewOrder 대화 상자입니다.

class NewOrder : public CDialogEx
{
	DECLARE_DYNAMIC(NewOrder)

public:
	NewOrder(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~NewOrder();
	CDatabase db_neworder;
	vector<vector<CString> > itmData;
// 대화 상자 데이터입니다.
	enum { IDD = IDD_NEW_ORDER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	void ShowData(CDatabase & db_neworder);
	afx_msg void OnBnClickedOrder();
	afx_msg void OnBnClickedOk();
	CListCtrl m_newOrder;
	afx_msg void OnEndlabeleditNewOrder(NMHDR *pNMHDR, LRESULT *pResult);
};
