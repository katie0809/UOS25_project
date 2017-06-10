#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include <vector>
#include <string>
using namespace std;

// NewCost ��ȭ �����Դϴ�.

class NewCost : public CDialogEx
{
	DECLARE_DYNAMIC(NewCost)

public:
	NewCost(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~NewCost();
	enum { IDD = IDD_NEW_COST };
	CDatabase db_cost;
	CString price;
	CString detail;


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	CComboBox category_combo;
	CEdit price_edit;
	CEdit detail_edit;
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
};
