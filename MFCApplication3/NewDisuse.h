#pragma once
#include "afxcmn.h"


// NewDisuse 대화 상자입니다.

class NewDisuse : public CDialogEx
{
	DECLARE_DYNAMIC(NewDisuse)

public:
	NewDisuse(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~NewDisuse();
	CDatabase db_newdisuse;
	// 대화 상자 데이터입니다.
	enum { IDD = IDD_NEW_DISUSE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

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
