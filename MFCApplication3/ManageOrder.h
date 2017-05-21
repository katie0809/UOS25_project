
#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CManageOrder 대화 상자입니다.

class CManageOrder : public CDialogEx
{
	DECLARE_DYNAMIC(CManageOrder)

/*
	Attributes
*/
public:
	class CContentView * p_contentView;
	CString current_date; // 어떤 날짜의 주문목록을 불러와야 하는지~
	CDatabase db_order;
	int order_id;
/*
	Operations
*/
public:
	void ShowData(CDatabase & db_order);

public:
	CManageOrder(CWnd* pParent = NULL);
	CManageOrder(CWnd * pParent, int selected_itm);
	// 표준 생성자입니다.
	virtual ~CManageOrder();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_MANAGE_ORDER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CCheckListBox m_orderList;
	afx_msg void OnBnClickedReturn();
	afx_msg void OnBnClickedConfirm();
	afx_msg void OnBnClickedOk();
	CListCtrl m_orderlist;
};
