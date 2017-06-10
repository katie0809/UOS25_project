#pragma once
#include "ManageOrder.h"
#include "ManageReturn.h"
#include "NewOrder.h"
#include "OrderConfirm.h"
#include "NewEmp.h"
#include "InfoEmp.h"
#include "NewProd.h"
#include "InfoProd.h"
#include "EventDetail.h"
#include "CodeList.h"
#include "EmpSalary.h"
#include "NewSale.h"
#include "SaleDetail.h"
#include "ManageAsset.h"
#include "NewDisuse.h"
#include "InfoDisuse.h"
#include "NewCustomer.h"

// CContentView 酉곗엯?덈떎.

class CContentView : public CListView
{
	DECLARE_DYNCREATE(CContentView)

protected:
	CContentView();          
	virtual ~CContentView();

/*
	Attributes
*/
public:
	int CUR_CONTENT;
	CString CUR_CODE;

	CListCtrl * m_list;
	CManageOrder * dlg_manage_order;
	NewOrder * dlg_new_order; 
	CManageReturn * dlg_manage_return;
	COrderConfirm * dlg_confirmed_order;
	CNewCustomer * dlg_customer;

	NewEmp * dlg_new_emp;
	InfoEmp * dlg_info_emp;
	NewProd * dlg_new_prod;
	InfoProd * dlg_info_prod;
	EmpSalary * dlg_emp_salary;
	EventDetail * dlg_event_detail;
	CodeList * dlg_code_list;
	NewDisuse * dlg_new_disuse;
	InfoDisuse * dig_info_disuse;

	NewSale * dlg_new_sale; //새 상품 추가 대화상자 인스턴스
	SaleDetail * dlg_sale_detail;
	ManageAsset * manageasset;

public:

#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	void DeleteContent(CListCtrl * m_list);
	virtual void OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/);
public:
	virtual void OnInitialUpdate();
	afx_msg void OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void OnCancelOrder();
	afx_msg void OnCancelReturn();
	afx_msg void OnNMClick(NMHDR *pNMHDR, LRESULT *pResult);
};


