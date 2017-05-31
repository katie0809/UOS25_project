#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "ManageOrder.h"

// CManageReturn ��ȭ �����Դϴ�.

class CManageReturn : public CDialogEx
{
	DECLARE_DYNAMIC(CManageReturn)

public:
	CManageReturn(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	CManageReturn(CWnd* pParent, CString retId);
	virtual ~CManageReturn();

	/*
	Attributes
	*/
	CString ret_id;
	CString ret_type;
	CDatabase db_ret;
	COrderConfirm * dlg_manage_order;
// ��ȭ ���� �������Դϴ�.
// #ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_RETURN };
// #endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_returnList;
	virtual BOOL OnInitDialog();
	void ShowData(CDatabase &db);
	CListBox m_retType;
	CListBox m_mommyCode;
	CStatic s_retType;
	afx_msg void OnLbnSelchangeList1();
	afx_msg void OnBnClickedSave();
	afx_msg void OnEndlabeleditReturn(NMHDR *pNMHDR, LRESULT *pResult);
};
