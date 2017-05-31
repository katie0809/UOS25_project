
#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "OrderConfirm.h"
#include <vector>
#include <string>
using namespace std;

// CManageOrder ��ȭ �����Դϴ�.

class CManageOrder : public CDialogEx
{
	DECLARE_DYNAMIC(CManageOrder)

/*
	Attributes

*/
public:
	class CContentView * p_contentView;
	CString current_code; // � �ֹ������ �ҷ��;� �ϴ���~
	CDatabase db_order;
	CString order_id;
	int dbitmcnt; // �ֹ� Ȯ�� ����� ��ü �ֹ��� ����
	vector<vector<int> > chked_itm; // �ֹ� �� �������� üũ�� �������� ������ �����ϴ� ���͹迭

									
/*
	Operations
*/
public:
	void ShowData(CDatabase & db_order);

public:
	CManageOrder(CWnd * pParent, CString selected_itm);


	// ǥ�� �������Դϴ�.
	virtual ~CManageOrder();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_MANAGE_ORDER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

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
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
};
