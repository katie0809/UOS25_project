#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include <vector>


// SaleDetail 대화 상자입니다.

class SaleDetail : public CDialogEx
{
	DECLARE_DYNAMIC(SaleDetail)

public:
	SaleDetail(CWnd* pParent, CString sale_num);   // 표준 생성자입니다.
	virtual ~SaleDetail();
	enum { IDD = IDD_SALE_DETAIL };


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	CListCtrl saledetail_list;
	CButton close_btn;
	CButton return_btn;
	CString sale_num; // 새롭게 뜬 대화상자의 모든 레코들의 공통 sale_num
	CString return_sale_num;
	CDatabase db_saledetail;


	// Overriding Methods
	virtual BOOL OnInitDialog();
	void ShowData(CDatabase& db_saledetail);
	void ModifyStyle();

	// View Clicked Methods
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();

	// Methods returning each code
	void SetReturnCode(CString * return_code);
	void SetSaleNum(CString * sale_num);
	int return_flag; // 1 : can return the items, 0 : items are already returned(can't click return btn)
};
