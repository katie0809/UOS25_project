#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "EventDetail.h"


// InfoProd ��ȭ �����Դϴ�.

class InfoProd : public CDialogEx
{
	DECLARE_DYNAMIC(InfoProd)

public:
	InfoProd(CWnd* pParent = NULL, CString code = NULL);   // ǥ�� �������Դϴ�.
	virtual ~InfoProd();
	CDatabase db_infoprod;
	EventDetail * dlg_event_detail;
	// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_INFO_PROD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnDeltaposSpin1(NMHDR *pNMHDR, LRESULT *pResult);
	CEdit m_infoprod_stock;
	CEdit m_infoprod_name;
	CEdit m_infoprod_price;
	CEdit m_infoprod_saleprice;
	CEdit m_infoprod_maker;
	CComboBox m_infoprod_sort;
	CComboBox m_infoprod_event;
	CSpinButtonCtrl m_infoprod_spin;
	CString prod_code;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton1();
};
