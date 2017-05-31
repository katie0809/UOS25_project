// NewProd.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MFCApplication3.h"
#include "NewProd.h"
#include "afxdialogex.h"


// NewProd 대화 상자입니다.

IMPLEMENT_DYNAMIC(NewProd, CDialogEx)

NewProd::NewProd(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_NEW_PROD, pParent)
{

}

NewProd::~NewProd()
{
}

void NewProd::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, 1019, m_newprod_name);
	DDX_Control(pDX, 1020, m_newprod_price);
	DDX_Control(pDX, 1022, m_newprod_maker);
	DDX_Control(pDX, 1021, m_newprod_saleprice);
	DDX_Control(pDX, 1025, m_newprod_sort);
	DDX_Control(pDX, 1026, m_newprod_event);
}


BEGIN_MESSAGE_MAP(NewProd, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &NewProd::OnBnClickedButton1)
END_MESSAGE_MAP()


// NewProd 메시지 처리기입니다.


void NewProd::OnBnClickedButton1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


BOOL NewProd::OnInitDialog() {
	CDialogEx::OnInitDialog();
	// 데이터베이스와의 연결

	TRY
	{
		db_newprod.OpenEx(_T("DSN=UOS25;UID=UOS25;PWD=0000"));
	}
		CATCH(CException, e)
	{
		TCHAR errMSG[255];

		e->GetErrorMessage(errMSG, 255);
		AfxMessageBox(errMSG, MB_ICONERROR);


		if (db_newprod.IsOpen())
			db_newprod.Close();

	}
	END_CATCH

		// 글자제한 설정
		m_newprod_name.SetLimitText(30);
	m_newprod_price.SetLimitText(10);
	m_newprod_saleprice.SetLimitText(10);
	m_newprod_maker.SetLimitText(30);

	ShowData();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void NewProd::ShowData() {
	CRecordset recSet(&db_newprod);
	CString sql, sort_code, sort_detail, string;

	sql.Format(L"SELECT SORT_CODE, SORT_DETAIL FROM SORT_CODE WHERE HIGH_SORT_CODE=(SELECT SORT_CODE FROM SORT_CODE WHERE SORT_DETAIL='범주코드')");
	recSet.Open(CRecordset::dynaset, sql);

	while (!recSet.IsEOF())
	{
		recSet.GetFieldValue(_T("SORT_CODE"), sort_code);
		recSet.GetFieldValue(_T("SORT_DETAIL"), sort_detail);

		string.Format(L"%s.%s", sort_code, sort_detail);
		m_newprod_sort.AddString(string);

		//다음 레코드로 이동
		recSet.MoveNext();
	}
	recSet.Close();
	m_newprod_sort.SetCurSel(0);

	m_newprod_event.AddString(L"0.이벤트 없음");
	sql.Format(L"SELECT EVENT_CODE, EVENT_NAME FROM EVENT");
	recSet.Open(CRecordset::dynaset, sql);
	while (!recSet.IsEOF())
	{
		recSet.GetFieldValue(_T("EVENT_CODE"), sort_code);
		recSet.GetFieldValue(_T("EVENT_NAME"), sort_detail);

		string.Format(L"%s.%s", sort_code, sort_detail);
		m_newprod_event.AddString(string);

		//다음 레코드로 이동
		recSet.MoveNext();
	}
	recSet.Close();
	m_newprod_event.SetCurSel(0);
}

void NewProd::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CRecordset recSet(&db_newprod);
	CString sql, name, price, sale_price, maker, sort, event, prod_code_string;
	int prod_code;

	m_newprod_name.GetWindowTextW(name);
	m_newprod_price.GetWindowTextW(price);
	m_newprod_saleprice.GetWindowTextW(sale_price);
	m_newprod_maker.GetWindowTextW(maker);
	this->m_newprod_event.GetLBText(this->m_newprod_event.GetCurSel(), event);
	this->m_newprod_sort.GetLBText(this->m_newprod_sort.GetCurSel(), sort);

	sql.Format(L"SELECT PROD_CODE FROM PRODUCT ORDER BY PROD_CODE DESC");
	recSet.Open(CRecordset::dynaset, sql);

	if (!recSet.IsEOF())
	{
		recSet.GetFieldValue(_T("PROD_CODE"), prod_code_string);
		prod_code = _ttoi(prod_code_string);
		prod_code++;
	}
	else
		prod_code = 1;
	recSet.Close();

	if (name.IsEmpty() == FALSE && price.IsEmpty() == FALSE && sale_price.IsEmpty() == FALSE && maker.IsEmpty() == FALSE && sort.IsEmpty() == FALSE) {

		prod_code_string.Format(L"%d", prod_code);
		AfxExtractSubString(sort, sort, 0, '.');
		AfxExtractSubString(event, event, 0, '.');
		if (event == '0')
			event.Format(L"");

		sql.Format(L"INSERT INTO PRODUCT(PROD_CODE, PROD_NAME, PROD_MAKER, PROD_PRICE, PROD_SALE_PRICE, PROD_STOCK_AMOUNT, EVENT_CODE, PROD_CATG_CODE) values ('%s', '%s', '%s', '%d', '%d', '%d', '%s', '%s')", prod_code_string, name, maker, _ttoi(price), _ttoi(sale_price), 0, event, sort);
		db_newprod.ExecuteSQL(sql);
		MessageBox(L"상품 추가 완료");
		CDialogEx::OnOK();
	}
	else
		MessageBox(L"모든 항목을 입력해 주십시오");
}


void NewProd::OnBnClickedButton1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	dlg_event_detail = new EventDetail();
	dlg_event_detail->Create(EventDetail::IDD);
	dlg_event_detail->ShowWindow(SW_SHOW);
}
