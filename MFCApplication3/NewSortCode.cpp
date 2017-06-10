// NewSortCode.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "MFCApplication3.h"
#include "NewSortCode.h"
#include "afxdialogex.h"


// NewSortCode ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(NewSortCode, CDialogEx)

NewSortCode::NewSortCode(CWnd* pParent, CString code /*=NULL*/)
	: CDialogEx(IDD_NEW_SORT_CODE, pParent)
{
	sort_code = code;
}

NewSortCode::~NewSortCode()
{
}
void NewSortCode::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, 1019, m_newsortcode_code);
	DDX_Control(pDX, 1025, m_newsortcode_high);
	DDX_Control(pDX, 1020, m_newsortcode_detail);
}


BEGIN_MESSAGE_MAP(NewSortCode, CDialogEx)
END_MESSAGE_MAP()



// NewSortCode �޽��� ó�����Դϴ�.


BOOL NewSortCode::OnInitDialog() {
	CDialogEx::OnInitDialog();

	// �����ͺ��̽����� ����

	TRY
	{
		db_newsortcode.OpenEx(_T("DSN=UOS25;UID=UOS25;PWD=0000"));
	}
		CATCH(CException, e)
	{
		TCHAR errMSG[255];

		e->GetErrorMessage(errMSG, 255);
		AfxMessageBox(errMSG, MB_ICONERROR);


		if (db_newsortcode.IsOpen())
			db_newsortcode.Close();

	}
	END_CATCH

		ShowData();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}

void NewSortCode::ShowData() {
	CRecordset recSet(&db_newsortcode);
	CString sql, code, high_sort_code, detail, high_combo;

	sql.Format(L"SELECT SORT_CODE, SORT_DETAIL FROM SORT_CODE WHERE HIGH_SORT_CODE IS NULL;");
	recSet.Open(CRecordset::dynaset, sql);
	// �ڵ� ��� ���
	while (!recSet.IsEOF())
	{
		recSet.GetFieldValue(_T("SORT_CODE"), code);
		recSet.GetFieldValue(_T("SORT_DETAIL"), detail);

		high_combo.Format(L"%s.%s", code, detail);
		m_newsortcode_high.AddString(high_combo);

		//���� ���ڵ�� �̵�
		recSet.MoveNext();
	}
	recSet.Close();
	m_newsortcode_high.SetCurSel(0);

	// ������ ���
	if (!sort_code.IsEmpty()) {
		sql.Format(L"SELECT SORT_CODE, HIGH_SORT_CODE, SORT_DETAIL FROM SORT_CODE WHERE SORT_CODE= '%s';", sort_code);
		recSet.Open(CRecordset::dynaset, sql);

		// �ڵ� ��� ���
		if (!recSet.IsEOF())
		{
			recSet.GetFieldValue(_T("SORT_CODE"), code);
			recSet.GetFieldValue(_T("HIGH_SORT_CODE"), high_sort_code);
			recSet.GetFieldValue(_T("SORT_DETAIL"), detail);
		}
		recSet.Close();

		m_newsortcode_code.SetWindowTextW(code);
		m_newsortcode_detail.SetWindowTextW(detail);

		for (int i = 0; i < m_newsortcode_high.GetCount(); i++) {
			m_newsortcode_high.GetLBText(i, high_combo);
			if (high_combo == high_sort_code) {
				m_newsortcode_high.SetCurSel(i);
				break;
			}
		}
		GetDlgItem(IDOK)->SetWindowText(TEXT("����"));
	}

}

void NewSortCode::OnBnClickedOk()
{
	CString sql, code, high_sort_code, detail, high_combo;
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	m_newsortcode_code.GetWindowTextW(code);
	m_newsortcode_detail.GetWindowTextW(detail);
	this->m_newsortcode_high.GetLBText(this->m_newsortcode_high.GetCurSel(), high_sort_code);

	if (!code.IsEmpty() && !detail.IsEmpty()) {
		AfxExtractSubString(high_sort_code, high_sort_code, 0, '.');

		if (!sort_code.IsEmpty()) {
			sql.Format(L"UPDATE SORT_CODE SET SORT_CODE='%s', HIGH_SORT_CODE='%s', SORT_DETAIL='%s' WHERE SORT_CODE='%s'", code, high_sort_code, detail, sort_code);
			db_newsortcode.ExecuteSQL(sql);
			MessageBox(L"�����ڵ� ���� �Ϸ�");
			CDialogEx::OnOK();
		}
		else {
			sql.Format(L"INSERT INTO SORT_CODE(SORT_CODE, HIGH_SORT_CODE, SORT_DETAIL) values ('%s', '%s', '%s')", code, high_sort_code, detail);
			db_newsortcode.ExecuteSQL(sql);
			MessageBox(L"�����ڵ� �߰� �Ϸ�");
			CDialogEx::OnOK();
		}
	}
	else
		MessageBox(L"��� �׸��� �Է��Ͻʽÿ�.");
}
