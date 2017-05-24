
#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include <vector>
#include <string>
using namespace std;

// CManageOrder 대화 상자입니다.

class CManageOrder : public CDialogEx
{
	DECLARE_DYNAMIC(CManageOrder)

/*
	Attributes

*/
public:
	class CContentView * p_contentView;
	CString current_code; // 어떤 주문목록을 불러와야 하는지~
	CDatabase db_order;
	CString order_id;
	int dbitmcnt; // 주문 확인 당시의 전체 주문의 개수
	vector<vector<int> > chked_itm; // 주문 상세 내역에서 체크된 아이템의 내용을 저장하는 벡터배열

									
/*
	Operations
*/
public:
	void ShowData(CDatabase & db_order);

public:
	CManageOrder(CWnd* pParent = NULL);
	CManageOrder(CWnd * pParent, CString selected_itm);

	// 표준 생성자입니다.
	virtual ~CManageOrder();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_MANAGE_ORDER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	void ModifyStyle();
	afx_msg void OnBnClickedReturn();
	afx_msg void OnBnClickedConfirm();
	afx_msg void OnBnClickedOk();
	CListCtrl m_orderlist;
	afx_msg void OnBnClickedReorder();
	CListCtrl m_returnList;
	CListCtrl m_reorderList;
	CListCtrl m_orderList;
	afx_msg void OnEndlabeleditReorderList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEndlabeleditReturnList(NMHDR *pNMHDR, LRESULT *pResult);
};
