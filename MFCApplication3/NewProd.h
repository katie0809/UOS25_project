#pragma once
#include "afxwin.h"
#include "EventDetail.h"


// NewProd ��ȭ �����Դϴ�.

class NewProd : public CDialogEx
{
	DECLARE_DYNAMIC(NewProd)

public:
	NewProd(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~NewProd();
	CDatabase db_newprod;
	EventDetail * dlg_event_detail;
	// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_NEW_PROD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
//	virtual BOOL OnInitDialog();
	void ShowData();
	CEdit m_newprod_name;
	CEdit m_newprod_price;
	CComboBox m_newprod_sort;
	CComboBox m_newprod_event;
	afx_msg void OnBnClickedOk();
	CEdit m_newprod_saleprice;
	CEdit m_newprod_maker;
	afx_msg void OnBnClickedButton1();
	CEdit m_newprod_expiration;
	virtual BOOL OnInitDialog();
};
