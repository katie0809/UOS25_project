#pragma once
#include "afxwin.h"
#include "stdafx.h"

// CLogin ��ȭ �����Դϴ�.

class CLogin : public CDialogEx
{
	DECLARE_DYNAMIC(CLogin)

public:
	CLogin(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CLogin();

// ��ȭ ���� �������Դϴ�.
//#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LOGIN };
//#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CEdit s_usercode;
	afx_msg void OnBnClickedLogin();
};
