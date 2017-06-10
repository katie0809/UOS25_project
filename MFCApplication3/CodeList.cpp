// CodeList.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "MFCApplication3.h"
#include "CodeList.h"
#include "afxdialogex.h"


// CodeList ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CodeList, CDialogEx)

CodeList::CodeList(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_CODE_LIST, pParent)
	, m_codelist_radio(0)
{

}

CodeList::~CodeList()
{
}

void CodeList::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_codelist_list);
	DDX_Radio(pDX, IDC_RADIO1, (int &) m_codelist_radio);
	DDX_Control(pDX, IDC_EDIT1, m_codelist_select);
}


BEGIN_MESSAGE_MAP(CodeList, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CodeList::OnBnClickedButton1)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_RADIO1, IDC_RADIO5, CodeList::SetRadioStatus)
	ON_BN_CLICKED(IDC_BUTTON2, &CodeList::OnBnClickedButton2)
	ON_NOTIFY(NM_CLICK, IDC_LIST1, &CodeList::OnNMClickList1)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &CodeList::OnNMDblclkList1)
END_MESSAGE_MAP()


// CodeList �޽��� ó�����Դϴ�.

BOOL CodeList::OnInitDialog() {
	CDialogEx::OnInitDialog();

	// �����ͺ��̽����� ����

	TRY
	{
		db_codelist.OpenEx(_T("DSN=UOS25;UID=UOS25;PWD=0000"));
	}
		CATCH(CException, e)
	{
		TCHAR errMSG[255];

		e->GetErrorMessage(errMSG, 255);
		AfxMessageBox(errMSG, MB_ICONERROR);


		if (db_codelist.IsOpen())
			db_codelist.Close();

	}
	END_CATCH


		m_codelist_list.ModifyStyle(0, LVS_REPORT | LVS_SHOWSELALWAYS | LVS_SINGLESEL, 0);
	m_codelist_list.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	m_codelist_list.InsertColumn(0, L"�����ڵ�", LVCFMT_CENTER, 100);
	m_codelist_list.InsertColumn(1, L"���������ڵ�", LVCFMT_CENTER, 100);
	m_codelist_list.InsertColumn(2, L"���г���", LVCFMT_CENTER, 100);

	m_selected_item = -1;
	sql_list.Format(L"SELECT SORT_CODE, HIGH_SORT_CODE, SORT_DETAIL FROM SORT_CODE WHERE HIGH_SORT_CODE IS NOT NULL ORDER BY HIGH_SORT_CODE DESC, SORT_CODE DESC;");
	m_codelist_select.SetWindowTextW(L"��ü�ڵ带 �����Ͽ����ϴ�.");
	ShowData();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}

void CodeList::ShowData() {
	CRecordset recSet(&db_codelist);
	CString sort_code, high_sort_code, detail;

	m_codelist_list.DeleteAllItems();

	recSet.Open(CRecordset::dynaset, sql_list);

	// �ڵ� ��� ���
	while (!recSet.IsEOF())
	{
		recSet.GetFieldValue(_T("SORT_CODE"), sort_code);
		recSet.GetFieldValue(_T("HIGH_SORT_CODE"), high_sort_code);
		recSet.GetFieldValue(_T("SORT_DETAIL"), detail);

		// Insert itm into list
		int nListitm = m_codelist_list.InsertItem(0, sort_code, 0);
		m_codelist_list.SetItem(nListitm, 1, LVFIF_TEXT, high_sort_code, 0, 0, 0, NULL);
		m_codelist_list.SetItem(nListitm, 2, LVFIF_TEXT, detail, 0, 0, 0, NULL);

		//���� ���ڵ�� �̵�
		recSet.MoveNext();
	}
	recSet.Close();
}


void CodeList::SetRadioStatus(UINT value)
{
	UpdateData(TRUE);
	switch (m_codelist_radio)
	{
	case 0:
		sql_list.Format(L"SELECT SORT_CODE, HIGH_SORT_CODE, SORT_DETAIL FROM SORT_CODE WHERE HIGH_SORT_CODE IS NOT NULL ORDER BY HIGH_SORT_CODE DESC, SORT_CODE DESC;");
		m_codelist_select.SetWindowTextW(L"��ü�ڵ带 �����Ͽ����ϴ�.");
		break;
	case 1:
		sql_list.Format(L"SELECT SORT_CODE, HIGH_SORT_CODE, SORT_DETAIL FROM SORT_CODE WHERE HIGH_SORT_CODE=(SELECT SORT_CODE FROM SORT_CODE WHERE SORT_DETAIL='�����ڵ�') ORDER BY SORT_CODE DESC;");
		m_codelist_select.SetWindowTextW(L"�����ڵ带 �����Ͽ����ϴ�.");
		break;
	case 2:
		sql_list.Format(L"SELECT SORT_CODE, HIGH_SORT_CODE, SORT_DETAIL FROM SORT_CODE WHERE HIGH_SORT_CODE=(SELECT SORT_CODE FROM SORT_CODE WHERE SORT_DETAIL='���з�') ORDER BY SORT_CODE DESC;");
		m_codelist_select.SetWindowTextW(L"���з��� �����Ͽ����ϴ�.");
		break;
	case 3:
		sql_list.Format(L"SELECT SORT_CODE, HIGH_SORT_CODE, SORT_DETAIL FROM SORT_CODE WHERE HIGH_SORT_CODE=(SELECT SORT_CODE FROM SORT_CODE WHERE SORT_DETAIL='�����ڵ�') ORDER BY SORT_CODE DESC;");;
		m_codelist_select.SetWindowTextW(L"�����ڵ带 �����Ͽ����ϴ�.");
		break;
	case 4:
		sql_list.Format(L"SELECT SORT_CODE, HIGH_SORT_CODE, SORT_DETAIL FROM SORT_CODE WHERE HIGH_SORT_CODE=(SELECT SORT_CODE FROM SORT_CODE WHERE SORT_DETAIL='�ڱ��ڵ�') ORDER BY SORT_CODE DESC;");
		m_codelist_select.SetWindowTextW(L"�ڱ��ڵ带 �����Ͽ����ϴ�.");
		break;
	}
	ShowData();
}

void CodeList::OnBnClickedButton2()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if (m_selected_item < 0)
		MessageBox(L"�����ڵ带 �����ϼ���");
	else {
		CString  sort_code, sql;
		sort_code = m_codelist_list.GetItemText(m_selected_item, 0);

		sql.Format(L"DELETE FROM SORT_CODE WHERE SORT_CODE='%s'", sort_code);
		db_codelist.ExecuteSQL(sql);
		MessageBox(L"�����ڵ� ���� �Ϸ�");

		ShowData();
	}
}


void CodeList::OnBnClickedButton1()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	dlg_new_sort_code = new NewSortCode(this, NULL);
	dlg_new_sort_code->Create(NewSortCode::IDD);
	dlg_new_sort_code->ShowWindow(SW_SHOW);
}


void CodeList::OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>( pNMHDR );
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	if (pNMItemActivate->iItem != -1) {
		// ���õ� �������� �ε��� ��ȣ�� ��´�
		NM_LISTVIEW * pNMListView = (NM_LISTVIEW*) pNMHDR;
		int cur_idx = pNMListView->iItem;

		CString sort_code;
		sort_code = m_codelist_list.GetItemText(cur_idx, 0);

		// Open dialog using currently selected index num
		dlg_new_sort_code = new NewSortCode(this, sort_code);
		dlg_new_sort_code->Create(NewSortCode::IDD);
		dlg_new_sort_code->ShowWindow(SW_SHOW);
	}

	*pResult = 0;
}


void CodeList::OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>( pNMHDR );
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_selected_item = pNMItemActivate->iItem;
	*pResult = 0;
}