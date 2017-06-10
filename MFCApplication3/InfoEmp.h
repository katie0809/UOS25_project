#pragma once
#include "afxwin.h"
#include "NewWorkTime.h"
#include "afxcmn.h"


// InfoEmp ��ȭ �����Դϴ�.

class InfoEmp : public CDialogEx
{
	DECLARE_DYNAMIC(InfoEmp)

public:
	InfoEmp(CWnd* pParent = NULL, CString selected_emp = NULL);   // ǥ�� �������Դϴ�.
	virtual ~InfoEmp();
	CDatabase db_infoemp;
	NewWorkTime * dlg_new_work_time;
	// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_INFO_EMP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	void ShowData(CDatabase & db_infoemp);
	CEdit m_infoemp_name;
	CComboBox m_infoemp_rank;
	CEdit m_infoemp_age;
	CComboBox m_infoemp_sex;
	CEdit m_infoemp_phone;
	CEdit m_infoemp_bank;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButton1();
	CListCtrl m_worktime_list;
	CString rank_out, emp_num;
	afx_msg void OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult);
};
