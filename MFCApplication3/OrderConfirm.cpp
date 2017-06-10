// OrderConfirm.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "MFCApplication3.h"
#include "OrderConfirm.h"
#include "afxdialogex.h"


// COrderConfirm ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(COrderConfirm, CDialogEx)

COrderConfirm::COrderConfirm(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_CONFIRMED_ORDER, pParent)
{

}

COrderConfirm::COrderConfirm(CWnd * pParent, CString order_code)
{
	order_id = order_code;
}

COrderConfirm::~COrderConfirm()
{
}

void COrderConfirm::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ORDER_LIST, m_orderList);
	//DDX_Control(pDX, IDC_EDIT2, reorder_code);
	//DDX_Control(pDX, IDC_EDIT1, return_num);
	DDX_Control(pDX, IDC_LIST2, reorderCode);
	DDX_Control(pDX, IDC_LIST1, returnCode);
}


BEGIN_MESSAGE_MAP(COrderConfirm, CDialogEx)
END_MESSAGE_MAP()


// COrderConfirm �޽��� ó�����Դϴ�.


BOOL COrderConfirm::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

	TRY
	{
		db_order.OpenEx(_T("DSN=UOS25;UID=UOS25;PWD=0000"));
	}
		CATCH(CException, e)
	{
		TCHAR errMSG[255];

		e->GetErrorMessage(errMSG, 255);
		AfxMessageBox(errMSG, MB_ICONERROR);


		if (db_order.IsOpen())
			db_order.Close();

	}
	END_CATCH

		ShowData(db_order);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}

void COrderConfirm::ShowData(CDatabase & db_order)
{

	CRecordset recSet(&db_order);
	CString strSQL, strNAME, strPRICE, strREMAIN, strCODE, strMAKER, strORDERAMT, strREORDER, strRETURN;

	// Modify list style
	m_orderList.ModifyStyle(0, LVS_REPORT | LVS_SHOWSELALWAYS, 0);
	m_orderList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	// �������� ���� Ư�� ��¥�� �ֹ���ϸ� �޾ƿ´�
	// Get product code, product name, product maker, product price, order amount, product stock amount, event detail
	strSQL.Format(L"select product.prod_num, prod_name, prod_maker, prod_price, order_amount, prod_stock_amount, reorder_num from product inner join order_list on order_list.order_num = '%s' and order_list.prod_num = product.prod_num", order_id);
	recSet.Open(CRecordset::dynaset, strSQL);
	recSet.GetFieldValue(_T("REORDER_NUM"), strREORDER);

	// Create Column
	m_orderList.InsertColumn(0, L"��ǰ �ڵ�", LVCFMT_CENTER, 80);
	m_orderList.InsertColumn(1, L"��ǰ��", LVCFMT_CENTER, 100);
	m_orderList.InsertColumn(2, L"������", LVCFMT_CENTER, 100);
	m_orderList.InsertColumn(3, L"��ǰ ����", LVCFMT_CENTER, 100);
	m_orderList.InsertColumn(4, L"�ֹ� ����", LVCFMT_CENTER, 50);
	m_orderList.InsertColumn(5, L"��� ����", LVCFMT_CENTER, 50);

	// �޾ƿ� ���̺� ���� �����Ͱ� ���� ������ ����
	while (!recSet.IsEOF())
	{
		int idx = 0;

		recSet.GetFieldValue(_T("PROD_NUM"), strCODE);
		recSet.GetFieldValue(_T("PROD_NAME"), strNAME);
		recSet.GetFieldValue(_T("PROD_MAKER"), strMAKER);
		recSet.GetFieldValue(_T("PROD_PRICE"), strPRICE);
		recSet.GetFieldValue(_T("ORDER_AMOUNT"), strORDERAMT);
		recSet.GetFieldValue(_T("PROD_STOCK_AMOUNT"), strREMAIN);

		// insert data into list
		int nListitm = m_orderList.InsertItem(0, strCODE, 0);
		m_orderList.SetItem(nListitm, 1, LVFIF_TEXT, strNAME, 0, 0, 0, NULL);
		m_orderList.SetItem(nListitm, 2, LVFIF_TEXT, strMAKER, 0, 0, 0, NULL);
		m_orderList.SetItem(nListitm, 3, LVFIF_TEXT, strPRICE, 0, 0, 0, NULL);
		m_orderList.SetItem(nListitm, 4, LVFIF_TEXT, strORDERAMT, 0, 0, 0, NULL);
		m_orderList.SetItem(nListitm, 5, LVFIF_TEXT, strREMAIN, 0, 0, 0, NULL);

		//���� ���ڵ�� �̵�
		recSet.MoveNext();
	}

	recSet.Close();

	//strREORDER = L"";
	//strRETURN = L"";

	// ���ֹ� �ڵ带 �����ش�
	reorderCode.AddString(strREORDER);

	// ��ǰ �ڵ带 �����ش�
	strSQL.Format(L"select DISTINCT return_num from return where return_high_num = '%s'", order_id);
	recSet.Open(CRecordset::dynaset, strSQL);

	if (recSet.GetRecordCount() > 0)
	{
		recSet.GetFieldValue(_T("RETURN_NUM"), strRETURN);
		returnCode.AddString(strRETURN);
	}
}

