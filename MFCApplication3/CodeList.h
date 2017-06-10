#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "NewSortCode.h"

// CodeList ��ȭ �����Դϴ�.

class CodeList : public CDialogEx
{
	DECLARE_DYNAMIC(CodeList)

public:
	CodeList(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CodeList();
	CDatabase db_codelist;
	NewSortCode *dlg_new_sort_code;
	// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_CODE_LIST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	void ShowData();
	CListCtrl m_codelist_list;
	afx_msg void OnBnClickedButton1();
	afx_msg void SetRadioStatus(UINT value);
	int m_codelist_radio, m_selected_item;
	CEdit m_codelist_select;
	afx_msg void OnBnClickedButton2();
	CString sql_list;
	afx_msg void OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult);
};
