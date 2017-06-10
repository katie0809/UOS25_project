#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "NewSortCode.h"

// CodeList 대화 상자입니다.

class CodeList : public CDialogEx
{
	DECLARE_DYNAMIC(CodeList)

public:
	CodeList(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CodeList();
	CDatabase db_codelist;
	NewSortCode *dlg_new_sort_code;
	// 대화 상자 데이터입니다.
	enum { IDD = IDD_CODE_LIST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

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
