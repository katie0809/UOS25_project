#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "ManageOrder.h"

// CManageReturn 대화 상자입니다.

class CManageReturn : public CDialogEx
{
	DECLARE_DYNAMIC(CManageReturn)

public:
	CManageReturn(CWnd* pParent = NULL);   // 표준 생성자입니다.
	CManageReturn(CWnd* pParent, CString retId);
	virtual ~CManageReturn();

	/*
	Attributes
	*/
	CString ret_id;
	CString ret_type;
	CDatabase db_ret;
	CManageOrder * dlg_manage_order;
// 대화 상자 데이터입니다.
// #ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_RETURN };
// #endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

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
