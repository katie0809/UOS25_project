#pragma once
#include "afxcmn.h"


// NewDisuse ��ȭ �����Դϴ�.

class NewDisuse : public CDialogEx
{
	DECLARE_DYNAMIC(NewDisuse)

public:
	NewDisuse(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~NewDisuse();
	CDatabase db_newdisuse;
	// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_NEW_DISUSE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	void ShowData();
	CListCtrl m_newdisuse_prod;
	CListCtrl m_newdisuse_disuse;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnEndlabeleditList2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnClickList2(NMHDR *pNMHDR, LRESULT *pResult);
	int m_selected_item;
	afx_msg void OnBnClickedButton2();
};
