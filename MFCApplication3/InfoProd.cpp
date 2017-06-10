// InfoProd.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "MFCApplication3.h"
#include "InfoProd.h"
#include "afxdialogex.h"


// InfoProd ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(InfoProd, CDialogEx)

InfoProd::InfoProd(CWnd* pParent, CString num /*=NULL*/)
	: CDialogEx(IDD_INFO_PROD, pParent)
{
	prod_num = num;
}

InfoProd::~InfoProd()
{
}

void InfoProd::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, 1022, m_infoprod_stock);
	DDX_Control(pDX, 1019, m_infoprod_name);
	DDX_Control(pDX, 1020, m_infoprod_price);
	DDX_Control(pDX, 1021, m_infoprod_saleprice);
	DDX_Control(pDX, 1023, m_infoprod_maker);
	DDX_Control(pDX, 1026, m_infoprod_sort);
	DDX_Control(pDX, 1027, m_infoprod_event);
	DDX_Control(pDX, 1025, m_infoprod_spin);
	DDX_Control(pDX, IDC_EDIT6, m_infoprod_expiration);
}


BEGIN_MESSAGE_MAP(InfoProd, CDialogEx)
	ON_NOTIFY(UDN_DELTAPOS, 1025, &InfoProd::OnDeltaposSpin1)
	ON_BN_CLICKED(IDOK, &InfoProd::OnBnClickedOk)
	ON_BN_CLICKED(1028, &InfoProd::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON1, &InfoProd::OnBnClickedButton1)
END_MESSAGE_MAP()

// InfoProd �޽��� ó�����Դϴ�.

BOOL InfoProd::OnInitDialog() {
	CDialogEx::OnInitDialog();
	CRecordset recSet(&db_infoprod);
	CString sql, sort_code, sort_detail, string, name, maker, price, sale_price, stock, sort, event, expiration;

	// �����ͺ��̽����� ����

	TRY
	{
		db_infoprod.OpenEx(_T("DSN=UOS25;UID=UOS25;PWD=0000"));
	}
		CATCH(CException, e)
	{
		TCHAR errMSG[255];

		e->GetErrorMessage(errMSG, 255);
		AfxMessageBox(errMSG, MB_ICONERROR);


		if (db_infoprod.IsOpen())
			db_infoprod.Close();

	}
	END_CATCH

		// �������� ����
		m_infoprod_name.SetLimitText(30);
	m_infoprod_price.SetLimitText(10);
	m_infoprod_saleprice.SetLimitText(10);
	m_infoprod_maker.SetLimitText(30);

	// ��ǰ ���� �޾ƿ���
	sql.Format(L"SELECT * FROM PRODUCT WHERE PROD_NUM='%s'", prod_num);
	recSet.Open(CRecordset::dynaset, sql);

	if (!recSet.IsEOF())
	{
		recSet.GetFieldValue(_T("PROD_NAME"), name);
		recSet.GetFieldValue(_T("PROD_MAKER"), maker);
		recSet.GetFieldValue(_T("PROD_PRICE"), price);
		recSet.GetFieldValue(_T("PROD_SALE_PRICE"), sale_price);
		recSet.GetFieldValue(_T("PROD_STOCK_AMOUNT"), stock);
		recSet.GetFieldValue(_T("EVENT_NUM"), event);
		recSet.GetFieldValue(_T("CATG_SORT_CODE"), sort);
		recSet.GetFieldValue(_T("PROD_EXPIRATION"), expiration);
	}
	recSet.Close();

	m_infoprod_name.SetWindowTextW(name);
	m_infoprod_maker.SetWindowTextW(maker);
	m_infoprod_price.SetWindowTextW(price);
	m_infoprod_saleprice.SetWindowTextW(sale_price);
	m_infoprod_stock.SetWindowTextW(stock);
	m_infoprod_expiration.SetWindowTextW(expiration);

	//���� �� ����
	sql.Format(L"SELECT SORT_CODE, SORT_DETAIL FROM SORT_CODE WHERE HIGH_SORT_CODE=(SELECT SORT_CODE FROM SORT_CODE WHERE SORT_DETAIL='�����ڵ�')");
	recSet.Open(CRecordset::dynaset, sql);

	while (!recSet.IsEOF())
	{
		recSet.GetFieldValue(_T("SORT_CODE"), sort_code);
		recSet.GetFieldValue(_T("SORT_DETAIL"), sort_detail);

		string.Format(L"%s.%s", sort_code, sort_detail);
		m_infoprod_sort.AddString(string);

		//���� ���ڵ�� �̵�
		recSet.MoveNext();
	}
	recSet.Close();

	for (int i = 0; i < m_infoprod_sort.GetCount(); i++) {
		m_infoprod_sort.GetLBText(i, string);
		AfxExtractSubString(string, string, 0, '.');
		if (string == sort) {
			m_infoprod_sort.SetCurSel(i);
			break;
		}
	}

	// �̺�Ʈ ����
	m_infoprod_event.AddString(L"0.�̺�Ʈ ����");
	sql.Format(L"SELECT EVENT_NUM, EVENT_NAME FROM EVENT");
	recSet.Open(CRecordset::dynaset, sql);
	while (!recSet.IsEOF())
	{
		recSet.GetFieldValue(_T("EVENT_NUM"), sort_code);
		recSet.GetFieldValue(_T("EVENT_NAME"), sort_detail);

		string.Format(L"%s.%s", sort_code, sort_detail);
		m_infoprod_event.AddString(string);

		//���� ���ڵ�� �̵�
		recSet.MoveNext();
	}
	recSet.Close();

	if (event.IsEmpty())
		m_infoprod_event.SetCurSel(0);
	else {
		for (int i = 1; i < m_infoprod_event.GetCount(); i++) {
			m_infoprod_event.GetLBText(i, string);
			AfxExtractSubString(string, string, 0, '.');
			if (string == event) {
				m_infoprod_event.SetCurSel(i);
				break;
			}
		}
	}

	return TRUE;  // return TRUE unless you set the focus to a control
}


void InfoProd::OnDeltaposSpin1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>( pNMHDR );
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CString stock;
	int stock_number;

	m_infoprod_stock.GetWindowTextW(stock);
	stock_number = _ttoi(stock);
	if (pNMUpDown->iDelta < 0) {// ������ư
		if (stock_number > 199)
			return;
		stock_number++;
	}
	else // ���ҹ�ư
	{
		if (stock_number < 1)
			return;
		stock_number--;
	}
	stock.Format(L"%d", stock_number);
	m_infoprod_stock.SetWindowTextW(stock);

	*pResult = 0;
}


void InfoProd::OnBnClickedOk()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CRecordset recSet(&db_infoprod);
	CString sql, name, maker, price, sale_price, stock, sort, event, expiration_string;
	int expiration;

	m_infoprod_name.GetWindowTextW(name);
	m_infoprod_price.GetWindowTextW(price);
	m_infoprod_saleprice.GetWindowTextW(sale_price);
	m_infoprod_maker.GetWindowTextW(maker);
	this->m_infoprod_event.GetLBText(this->m_infoprod_event.GetCurSel(), event);
	this->m_infoprod_sort.GetLBText(this->m_infoprod_sort.GetCurSel(), sort);
	m_infoprod_stock.GetWindowTextW(stock);
	m_infoprod_expiration.GetWindowTextW(expiration_string);
	expiration = _ttoi(expiration_string);

	if (name.IsEmpty() == FALSE && price.IsEmpty() == FALSE && sale_price.IsEmpty() == FALSE && maker.IsEmpty() == FALSE && sort.IsEmpty() == FALSE) {
		if (_ttoi(stock)<201 && _ttoi(stock)>-1)
		{
			AfxExtractSubString(sort, sort, 0, '.');
			AfxExtractSubString(event, event, 0, '.');
			sql.Format(L"UPDATE PRODUCT SET PROD_NAME='%s', PROD_MAKER='%s', PROD_PRICE='%d', PROD_SALE_PRICE='%d', PROD_STOCK_AMOUNT='%d', EVENT_NUM='%s', CATG_SORT_CODE='%s', PROD_EXPIRATION='%d' WHERE PROD_NUM='%s'", name, maker, _ttoi(price), _ttoi(sale_price), _ttoi(stock), event, sort, expiration, prod_num);
			db_infoprod.ExecuteSQL(sql);
			MessageBox(L"��ǰ ���� �Ϸ�");
			CDialogEx::OnOK();
		}
		else
			MessageBox(L"����� ������ 0~200 �Դϴ�.");
	}
	else
		MessageBox(L"��� �׸��� �Է��� �ֽʽÿ�");
}


void InfoProd::OnBnClickedButton2()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	dlg_event_detail = new EventDetail();
	dlg_event_detail->Create(EventDetail::IDD);
	dlg_event_detail->ShowWindow(SW_SHOW);
}


void InfoProd::OnBnClickedButton1()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	CString sql;
	sql.Format(L"DELETE FROM PRODUCT WHERE PROD_NUM='%s'", prod_num);
	db_infoprod.ExecuteSQL(sql);
	MessageBox(L"��ǰ ���� �Ϸ�");
	CDialogEx::OnOK();

}
