// InfoDisuse.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "MFCApplication3.h"
#include "InfoDisuse.h"
#include "afxdialogex.h"


// InfoDisuse ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(InfoDisuse, CDialogEx)

InfoDisuse::InfoDisuse(CWnd* pParent, CString date, CString num/*=NULL*/)
	: CDialogEx(IDD_INFO_DISUSE, pParent)
{
	disuse_date = date;
	prod_num = num;
}

InfoDisuse::~InfoDisuse()
{
}

void InfoDisuse::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_infodisuse_date);
	DDX_Control(pDX, IDC_EDIT2, m_infodisuse_prodnum);
	DDX_Control(pDX, IDC_EDIT4, m_infodisuse_name);
	DDX_Control(pDX, IDC_EDIT3, m_infodisuse_maker);
	DDX_Control(pDX, IDC_EDIT5, m_infodisuse_amount);
}


BEGIN_MESSAGE_MAP(InfoDisuse, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &InfoDisuse::OnBnClickedButton1)
END_MESSAGE_MAP()


// InfoDisuse �޽��� ó�����Դϴ�.

BOOL InfoDisuse::OnInitDialog() {
	CDialogEx::OnInitDialog();

	// �����ͺ��̽����� ����

	TRY
	{
		db_infodisuse.OpenEx(_T("DSN=UOS25;UID=UOS25;PWD=0000"));
	}
		CATCH(CException, e)
	{
		TCHAR errMSG[255];

		e->GetErrorMessage(errMSG, 255);
		AfxMessageBox(errMSG, MB_ICONERROR);


		if (db_infodisuse.IsOpen())
			db_infodisuse.Close();

	}
	END_CATCH

		ShowData();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}

void InfoDisuse::ShowData() {
	CRecordset recSet(&db_infodisuse);
	CString sql, name, maker, amount;

	m_infodisuse_date.SetWindowTextW(disuse_date);
	m_infodisuse_prodnum.SetWindowTextW(prod_num);

	sql.Format(L"SELECT DISUSE.DISUSE_DATE, DISUSE.PROD_NUM, PRODUCT.PROD_NAME, PRODUCT.PROD_MAKER, DISUSE.DISUSE_AMOUNT FROM DISUSE, PRODUCT WHERE DISUSE.DISUSE_DATE='%s' AND DISUSE.PROD_NUM='%s' AND DISUSE.PROD_NUM=PRODUCT.PROD_NUM", disuse_date, prod_num);
	recSet.Open(CRecordset::dynaset, sql);

	// �ڵ� ��� ���
	if (!recSet.IsEOF())
	{
		recSet.GetFieldValue(_T("PROD_NAME"), name);
		recSet.GetFieldValue(_T("PROD_MAKER"), maker);
		recSet.GetFieldValue(_T("DISUSE_AMOUNT"), amount);

		m_infodisuse_date.SetWindowTextW(disuse_date);
		m_infodisuse_prodnum.SetWindowTextW(prod_num);
		m_infodisuse_name.SetWindowTextW(name);
		m_infodisuse_maker.SetWindowTextW(maker);
		m_infodisuse_amount.SetWindowTextW(amount);
	}
	else
		MessageBox(L"������ ����");
	recSet.Close();
}


// �������� ��� ���� �����ϵ���...
void InfoDisuse::OnBnClickedButton1()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CRecordset recSet(&db_infodisuse);
	CString sql, stock, amount;
	sql.Format(L"DELETE FROM DISUSE WHERE DISUSE_DATE='%s' AND PROD_NUM='%s'", disuse_date, prod_num);
	db_infodisuse.ExecuteSQL(sql);

	sql.Format(L"SELECT PROD_STOCK_AMOUNT FROM PRODUCT WHERE PROD_NUM='%s'", prod_num);
	recSet.Open(CRecordset::dynaset, sql);

	// �ڵ� ��� ���
	if (!recSet.IsEOF()) {
		recSet.GetFieldValue(_T("PROD_STOCK_AMOUNT"), stock);
	}
	recSet.Close();
	m_infodisuse_amount.GetWindowTextW(amount);
	sql.Format(L"UPDATE PRODUCT SET PROD_STOCK_AMOUNT='%d' WHERE PROD_NUM='%s'", ( _ttoi(amount) ) + ( _ttoi(stock) ), prod_num);
	db_infodisuse.ExecuteSQL(sql);

	MessageBox(L"��� ���� �Ϸ�");
	CDialogEx::OnOK();
}
