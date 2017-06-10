// CodeList.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MFCApplication3.h"
#include "CodeList.h"
#include "afxdialogex.h"


// CodeList 대화 상자입니다.

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


// CodeList 메시지 처리기입니다.

BOOL CodeList::OnInitDialog() {
	CDialogEx::OnInitDialog();

	// 데이터베이스와의 연결

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

	m_codelist_list.InsertColumn(0, L"구분코드", LVCFMT_CENTER, 100);
	m_codelist_list.InsertColumn(1, L"상위구분코드", LVCFMT_CENTER, 100);
	m_codelist_list.InsertColumn(2, L"구분내용", LVCFMT_CENTER, 100);

	m_selected_item = -1;
	sql_list.Format(L"SELECT SORT_CODE, HIGH_SORT_CODE, SORT_DETAIL FROM SORT_CODE WHERE HIGH_SORT_CODE IS NOT NULL ORDER BY HIGH_SORT_CODE DESC, SORT_CODE DESC;");
	m_codelist_select.SetWindowTextW(L"전체코드를 선택하였습니다.");
	ShowData();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CodeList::ShowData() {
	CRecordset recSet(&db_codelist);
	CString sort_code, high_sort_code, detail;

	m_codelist_list.DeleteAllItems();

	recSet.Open(CRecordset::dynaset, sql_list);

	// 코드 목록 출력
	while (!recSet.IsEOF())
	{
		recSet.GetFieldValue(_T("SORT_CODE"), sort_code);
		recSet.GetFieldValue(_T("HIGH_SORT_CODE"), high_sort_code);
		recSet.GetFieldValue(_T("SORT_DETAIL"), detail);

		// Insert itm into list
		int nListitm = m_codelist_list.InsertItem(0, sort_code, 0);
		m_codelist_list.SetItem(nListitm, 1, LVFIF_TEXT, high_sort_code, 0, 0, 0, NULL);
		m_codelist_list.SetItem(nListitm, 2, LVFIF_TEXT, detail, 0, 0, 0, NULL);

		//다음 레코드로 이동
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
		m_codelist_select.SetWindowTextW(L"전체코드를 선택하였습니다.");
		break;
	case 1:
		sql_list.Format(L"SELECT SORT_CODE, HIGH_SORT_CODE, SORT_DETAIL FROM SORT_CODE WHERE HIGH_SORT_CODE=(SELECT SORT_CODE FROM SORT_CODE WHERE SORT_DETAIL='범주코드') ORDER BY SORT_CODE DESC;");
		m_codelist_select.SetWindowTextW(L"범주코드를 선택하였습니다.");
		break;
	case 2:
		sql_list.Format(L"SELECT SORT_CODE, HIGH_SORT_CODE, SORT_DETAIL FROM SORT_CODE WHERE HIGH_SORT_CODE=(SELECT SORT_CODE FROM SORT_CODE WHERE SORT_DETAIL='고객분류') ORDER BY SORT_CODE DESC;");
		m_codelist_select.SetWindowTextW(L"고객분류를 선택하였습니다.");
		break;
	case 3:
		sql_list.Format(L"SELECT SORT_CODE, HIGH_SORT_CODE, SORT_DETAIL FROM SORT_CODE WHERE HIGH_SORT_CODE=(SELECT SORT_CODE FROM SORT_CODE WHERE SORT_DETAIL='직급코드') ORDER BY SORT_CODE DESC;");;
		m_codelist_select.SetWindowTextW(L"직급코드를 선택하였습니다.");
		break;
	case 4:
		sql_list.Format(L"SELECT SORT_CODE, HIGH_SORT_CODE, SORT_DETAIL FROM SORT_CODE WHERE HIGH_SORT_CODE=(SELECT SORT_CODE FROM SORT_CODE WHERE SORT_DETAIL='자금코드') ORDER BY SORT_CODE DESC;");
		m_codelist_select.SetWindowTextW(L"자금코드를 선택하였습니다.");
		break;
	}
	ShowData();
}

void CodeList::OnBnClickedButton2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (m_selected_item < 0)
		MessageBox(L"구분코드를 선택하세요");
	else {
		CString  sort_code, sql;
		sort_code = m_codelist_list.GetItemText(m_selected_item, 0);

		sql.Format(L"DELETE FROM SORT_CODE WHERE SORT_CODE='%s'", sort_code);
		db_codelist.ExecuteSQL(sql);
		MessageBox(L"구분코드 삭제 완료");

		ShowData();
	}
}


void CodeList::OnBnClickedButton1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	dlg_new_sort_code = new NewSortCode(this, NULL);
	dlg_new_sort_code->Create(NewSortCode::IDD);
	dlg_new_sort_code->ShowWindow(SW_SHOW);
}


void CodeList::OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>( pNMHDR );
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	if (pNMItemActivate->iItem != -1) {
		// 선택된 아이템의 인덱스 번호를 얻는다
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
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_selected_item = pNMItemActivate->iItem;
	*pResult = 0;
}