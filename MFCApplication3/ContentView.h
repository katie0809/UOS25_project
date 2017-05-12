#pragma once
#include "ManageOrder.h"
#include "NewOrder.h"
#include "SalesView.h"
// CContentView 뷰입니다.

class CContentView : public CListView
{
	DECLARE_DYNCREATE(CContentView)

protected:
	CContentView();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CContentView();

/*
	Attributes
*/
public:
	int CUR_CONTENT;
	CString current_date;
	CListCtrl * m_list;
	CSalesView * sales_view;
	CManageOrder * dlg_manage_order; //	주문 관리 다이얼로그 지시 포인터
	NewOrder * dlg_new_order; // 새 주문 추가 다이얼로그 지시 포인터
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
};


