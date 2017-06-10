// NewDisuse.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MFCApplication3.h"
#include "NewDisuse.h"
#include "afxdialogex.h"


// NewDisuse 대화 상자입니다.

IMPLEMENT_DYNAMIC(NewDisuse, CDialogEx)

NewDisuse::NewDisuse(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_NEW_DISUSE, pParent)
{

}

NewDisuse::~NewDisuse()
{
}

void NewDisuse::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_newdisuse_prod);
	DDX_Control(pDX, IDC_LIST2, m_newdisuse_disuse);
}


BEGIN_MESSAGE_MAP(NewDisuse, CDialogEx)
	ON_BN_CLICKED(IDOK, &NewDisuse::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON1, &NewDisuse::OnBnClickedButton1)
	ON_NOTIFY(LVN_ENDLABELEDIT, IDC_LIST2, &NewDisuse::OnEndlabeleditList2)
	ON_NOTIFY(NM_CLICK, IDC_LIST2, &NewDisuse::OnClickList2)
	ON_BN_CLICKED(IDC_BUTTON2, &NewDisuse::OnBnClickedButton2)
END_MESSAGE_MAP()


// NewDisuse 메시지 처리기입니다.

BOOL NewDisuse::OnInitDialog() {
	CDialogEx::OnInitDialog();

	// 데이터베이스와의 연결

	TRY
	{
		db_newdisuse.OpenEx(_T("DSN=UOS25;UID=UOS25;PWD=0000"));
	}
		CATCH(CException, e)
	{
		TCHAR errMSG[255];

		e->GetErrorMessage(errMSG, 255);
		AfxMessageBox(errMSG, MB_ICONERROR);


		if (db_newdisuse.IsOpen())
			db_newdisuse.Close();

	}
	END_CATCH

		m_newdisuse_prod.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_CHECKBOXES | LVS_EX_GRIDLINES);
	m_newdisuse_prod.ModifyStyle(0, LVS_REPORT | LVS_SHOWSELALWAYS | LVS_SINGLESEL, 0);

	m_newdisuse_prod.InsertColumn(0, L"상품번호", LVCFMT_CENTER, 150);
	m_newdisuse_prod.InsertColumn(1, L"상품이름", LVCFMT_CENTER, 100);
	m_newdisuse_prod.InsertColumn(2, L"제조사", LVCFMT_CENTER, 100);
	m_newdisuse_prod.InsertColumn(3, L"재고", LVCFMT_CENTER, 50);
	m_newdisuse_prod.InsertColumn(4, L"유통기한", LVCFMT_CENTER, 80);

	m_newdisuse_disuse.ModifyStyle(0, LVS_REPORT | LVS_SHOWSELALWAYS | LVS_SINGLESEL | LVS_EDITLABELS, 0);
	m_newdisuse_disuse.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);;

	m_newdisuse_disuse.InsertColumn(0, L"폐기수량", LVCFMT_CENTER, 100);
	m_newdisuse_disuse.InsertColumn(1, L"상품번호", LVCFMT_CENTER, 150);
	m_newdisuse_disuse.InsertColumn(2, L"상품이름", LVCFMT_CENTER, 100);
	m_newdisuse_disuse.InsertColumn(3, L"재고", LVCFMT_CENTER, 50);

	m_selected_item = -1;
	ShowData();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void NewDisuse::ShowData() {
	CRecordset recSet(&db_newdisuse);
	CString sql, prod_num, name, maker, amount, expiration;

	sql.Format(L"SELECT PROD_NUM, PROD_NAME, PROD_MAKER, PROD_STOCK_AMOUNT, PROD_EXPIRATION FROM PRODUCT");
	recSet.Open(CRecordset::dynaset, sql);

	// 코드 목록 출력
	while (!recSet.IsEOF())
	{
		recSet.GetFieldValue(_T("PROD_NUM"), prod_num);
		recSet.GetFieldValue(_T("PROD_NAME"), name);
		recSet.GetFieldValue(_T("PROD_MAKER"), maker);
		recSet.GetFieldValue(_T("PROD_STOCK_AMOUNT"), amount);
		recSet.GetFieldValue(_T("PROD_EXPIRATION"), expiration);

		// Insert itm into list
		int nListitm = m_newdisuse_prod.InsertItem(0, prod_num, 0);
		m_newdisuse_prod.SetItem(nListitm, 1, LVFIF_TEXT, name, 0, 0, 0, NULL);
		m_newdisuse_prod.SetItem(nListitm, 2, LVFIF_TEXT, maker, 0, 0, 0, NULL);
		m_newdisuse_prod.SetItem(nListitm, 3, LVFIF_TEXT, amount, 0, 0, 0, NULL);
		if (expiration == '0')
			expiration.Format(L"X");
		m_newdisuse_prod.SetItem(nListitm, 4, LVFIF_TEXT, expiration, 0, 0, 0, NULL);

		//다음 레코드로 이동
		recSet.MoveNext();
	}
	recSet.Close();
}

void NewDisuse::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString sql, date, prod_num, amount, stock_amount;

	if (m_newdisuse_disuse.GetItemCount() > 0) {
		for (int i = 0; i < m_newdisuse_disuse.GetItemCount(); i++)
		{
			amount = m_newdisuse_disuse.GetItemText(i, 0);
			prod_num = m_newdisuse_disuse.GetItemText(i, 1);
			stock_amount = m_newdisuse_disuse.GetItemText(i, 3);
			CTime cTime = CTime::GetCurrentTime(); // 현재 날짜 및 시간
			date.Format(L"%04d%02d%02d%02d%02d%02d", cTime.GetYear(), cTime.GetMonth(), cTime.GetDay(), cTime.GetHour(), cTime.GetMinute(), cTime.GetSecond());

			if (amount != '0') {
				sql.Format(L"INSERT INTO DISUSE(DISUSE_DATE, PROD_NUM, DISUSE_AMOUNT) values ('%s', '%s', '%d')", date, prod_num, _ttoi(amount));
				db_newdisuse.ExecuteSQL(sql);

				sql.Format(L"UPDATE PRODUCT SET PROD_STOCK_AMOUNT='%d' WHERE PROD_NUM='%s'", ( _ttoi(stock_amount) ) - ( _ttoi(amount) ), prod_num);
				db_newdisuse.ExecuteSQL(sql);
			}
		}
		MessageBox(L"폐기항목 추가 완료");
		CDialogEx::OnOK();
	}
	else
		MessageBox(L"폐기항목을 추가해 주십시오.");
}

// 폐기물품추가
void NewDisuse::OnBnClickedButton1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString prod_num, name, amount, zero;

	m_newdisuse_disuse.DeleteAllItems();

	int nCount = m_newdisuse_prod.GetItemCount();
	for (int i = 0; i<nCount; i++)
	{
		BOOL a = m_newdisuse_prod.GetCheck(i);
		if (a == TRUE)
		{
			prod_num = m_newdisuse_prod.GetItemText(i, 0);
			name = m_newdisuse_prod.GetItemText(i, 1);
			amount = m_newdisuse_prod.GetItemText(i, 3);

			// Insert itm into list
			int nListitm = m_newdisuse_disuse.InsertItem(0, L"0", 0);
			m_newdisuse_disuse.SetItem(nListitm, 1, LVFIF_TEXT, prod_num, 0, 0, 0, NULL);
			m_newdisuse_disuse.SetItem(nListitm, 2, LVFIF_TEXT, name, 0, 0, 0, NULL);
			m_newdisuse_disuse.SetItem(nListitm, 3, LVFIF_TEXT, amount, 0, 0, 0, NULL);
		}
	}
}


void NewDisuse::OnEndlabeleditList2(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>( pNMHDR );
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	CString stock_amount, disuse_amount; // 상품의 재고, 수정된 수량
	int SelectedIdx, _stock_amount, _disuse_amount;

	SelectedIdx = m_newdisuse_disuse.GetSelectionMark();
	stock_amount = m_newdisuse_disuse.GetItemText(SelectedIdx, 3);
	_stock_amount = _ttoi(stock_amount);
	_disuse_amount = _ttoi(pDispInfo->item.pszText);


	// 원래 주문한 양보다 수정된 값이 크면 수정을 막는다
	if (_stock_amount < _disuse_amount)
		MessageBox(L"폐기 수량은 재고 수량보다 작아야 합니다.");
	else {
		m_newdisuse_disuse.SetItemText(SelectedIdx, 0, pDispInfo->item.pszText);
	}
	*pResult = 0;
}


void NewDisuse::OnClickList2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>( pNMHDR );
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_selected_item = pNMItemActivate->iItem;
	*pResult = 0;
}


void NewDisuse::OnBnClickedButton2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_newdisuse_disuse.DeleteItem(m_selected_item);
	m_selected_item = -1;
}
