#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// COrderConfirm 대화 상자입니다.

class COrderConfirm : public CDialogEx
{
	DECLARE_DYNAMIC(COrderConfirm)

public:
	COrderConfirm(CWnd* pParent = NULL);   // 표준 생성자입니다.
	COrderConfirm(CWnd* pParent, CString order_code);
	virtual ~COrderConfirm();
	CString order_id;
	CDatabase db_order;
// 대화 상자 데이터입니다.
// #ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CONFIRMED_ORDER };
// #endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	void ShowData(CDatabase & db_order);
	CListCtrl m_orderList;
	CEdit reorder_code;
	CEdit return_code;
	CListBox reorderCode;
	CListBox returnCode;
};
