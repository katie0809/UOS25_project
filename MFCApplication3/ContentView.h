#pragma once
#include "ManageOrder.h"
#include "NewOrder.h"
// CContentView ë·°ì…?ˆë‹¤.

class CContentView : public CListView
{
	DECLARE_DYNCREATE(CContentView)

protected:
	CContentView();           // ?™ì  ë§Œë“¤ê¸°ì— ?¬ìš©?˜ëŠ” protected ?ì„±?ì…?ˆë‹¤.
	virtual ~CContentView();

/*
	Attributes
*/
public:
	int CUR_CONTENT;
	CString current_date;
	CListCtrl * m_list;
	CManageOrder * dlg_manage_order; //	
	NewOrder * dlg_new_order; // ??ì£¼ë¬¸ ì¶”ê? ?¤ì´?¼ë¡œê·?ì§€???¬ì¸??
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


