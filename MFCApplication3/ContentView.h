#pragma once
#include "ManageOrder.h"
#include "ManageReturn.h"
#include "NewOrder.h"
#include "OrderConfirm.h"
// CContentView 뷰입?�다.

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


