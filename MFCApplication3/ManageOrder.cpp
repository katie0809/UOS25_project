// ManageOrder.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "MFCApplication3.h"
#include "ManageOrder.h"
#include "afxdialogex.h"
#include "ContentView.h"

// CManageOrder ��ȭ �����Դϴ�.

/*
	�ֹ� ������ ���� ���̾�α� Ŭ����
*/

IMPLEMENT_DYNAMIC(CManageOrder, CDialogEx)

//������ �����ε�
CManageOrder::CManageOrder(CWnd *pParent, CString selected_itm)
	: CDialogEx(IDD_MANAGE_ORDER, pParent)
{
	order_id = selected_itm;

}

CManageOrder::~CManageOrder()
{
}

void CManageOrder::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RETURN_LIST, m_returnList);
	DDX_Control(pDX, IDC_REORDER_LIST, m_reorderList);
	DDX_Control(pDX, IDC_ORDER_LIST, m_orderList);
}


BEGIN_MESSAGE_MAP(CManageOrder, CDialogEx)
	ON_BN_CLICKED(IDC_RETURN, &CManageOrder::OnBnClickedReturn)
	ON_BN_CLICKED(IDC_CONFIRM, &CManageOrder::OnBnClickedConfirm)
	ON_BN_CLICKED(IDOK, &CManageOrder::OnBnClickedOk)
	ON_BN_CLICKED(IDC_REORDER, &CManageOrder::OnBnClickedReorder)
	ON_NOTIFY(LVN_ENDLABELEDIT, IDC_REORDER_LIST, &CManageOrder::OnEndlabeleditReorderList)
	ON_NOTIFY(LVN_ENDLABELEDIT, IDC_RETURN_LIST, &CManageOrder::OnEndlabeleditReturnList)
//	ON_NOTIFY(LVN_DELETEITEM, IDC_ORDER_LIST, &CManageOrder::OnDeleteitemOrderList)
ON_WM_CONTEXTMENU()
END_MESSAGE_MAP()


// CManageOrder �޽��� ó�����Դϴ�.


BOOL CManageOrder::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	// �����ͺ��̽����� ����
	// CDatabase db_order;

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

	ModifyStyle();
	ShowData(db_order);
	//db_order.Close();
	
	return TRUE;  
	// return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}

void CManageOrder::ModifyStyle()
{
	// Modify return list style
	m_returnList.ModifyStyle(0, LVS_REPORT | LVS_SHOWSELALWAYS | LVS_SINGLESEL | LVS_EDITLABELS, 0);
	m_returnList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	// Create Column
	m_returnList.InsertColumn(0, L"��ǰ ����", LVCFMT_CENTER, 80);
	m_returnList.InsertColumn(1, L"��ǰ �ڵ�", LVCFMT_CENTER, 80);
	m_returnList.InsertColumn(2, L"��ǰ��", LVCFMT_CENTER, 100);
	m_returnList.InsertColumn(3, L"������", LVCFMT_CENTER, 100);
	m_returnList.InsertColumn(4, L"��ǰ ����", LVCFMT_CENTER, 100);
	m_returnList.InsertColumn(5, L"�ֹ� ����", LVCFMT_CENTER, 50);
	m_returnList.InsertColumn(6, L"��� ����", LVCFMT_CENTER, 50);

	// Modify reorder list style
	m_reorderList.ModifyStyle(0, LVS_REPORT | LVS_SHOWSELALWAYS | LVS_SINGLESEL | LVS_EDITLABELS, 0);
	m_reorderList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	// Create Column
	m_reorderList.InsertColumn(0, L"���ֹ� ����", LVCFMT_CENTER, 80);
	m_reorderList.InsertColumn(1, L"��ǰ �ڵ�", LVCFMT_CENTER, 80);
	m_reorderList.InsertColumn(2, L"��ǰ��", LVCFMT_CENTER, 100);
	m_reorderList.InsertColumn(3, L"������", LVCFMT_CENTER, 100);
	m_reorderList.InsertColumn(4, L"��ǰ ����", LVCFMT_CENTER, 100);
	m_reorderList.InsertColumn(5, L"�ֹ� ����", LVCFMT_CENTER, 50);
	m_reorderList.InsertColumn(6, L"��� ����", LVCFMT_CENTER, 50);
}

void CManageOrder::ShowData(CDatabase & db_order)
{

	CRecordset recSet(&db_order);
	CString strSQL, strNAME, strPRICE, strREMAIN, strCODE, strMAKER, strORDERAMT;

	// Modify list style
	m_orderList.ModifyStyle(0, LVS_REPORT | LVS_SHOWSELALWAYS, 0);
	m_orderList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_CHECKBOXES);

	// �������� ���� Ư�� ��¥�� �ֹ���ϸ� �޾ƿ´�
	// Get product code, product name, product maker, product price, order amount, product stock amount, event detail
	strSQL.Format(L"select product.prod_code, prod_name, prod_maker, prod_price, order_amount, prod_stock_amount from product inner join order_list on order_list.order_code = '%s' and order_list.prod_code = product.prod_code", order_id);
	recSet.Open(CRecordset::dynaset, strSQL);

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
		
		recSet.GetFieldValue(_T("PROD_CODE"), strCODE);
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
	
}



void CManageOrder::OnBnClickedReturn()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	// checkbox�� ���õ� �������� ��ǰ ��Ͽ� �߰�

	int itemNum = m_orderList.GetItemCount(); //����Ʈ�� �ִ� ������ ������ ���´�
	int *returnItm;
	int returnItmCnt = 0; //��ǰ �׸��� �ε����� ������ �迭�� ��ǰ �׸��� ����
	returnItm = new int[itemNum];
	CString strSQL, strNAME, strPRICE, strREMAIN, strCODE, strMAKER, strORDERAMT;

	for (int i = 0; i < itemNum; i++)
	{
		//����Ʈ�� ��� �������� ��ȸ�ϸ鼭 üũ�Ǿ����� Ȯ�� ��
		//üũ�� �����ۿ� ���� ���� ����Ʈ���� delete�ϰ� return list�� �߰�

		if (m_orderList.GetCheck(i) == TRUE)
		{
			

			CString SQL_deleteItm, itmName, str, str1;
			strCODE = m_orderList.GetItemText(i, 0);
			strNAME = m_orderList.GetItemText(i, 1);
			strMAKER = m_orderList.GetItemText(i, 2);
			strPRICE = m_orderList.GetItemText(i, 3);
			strORDERAMT = m_orderList.GetItemText(i, 4);
			strREMAIN = m_orderList.GetItemText(i, 5);
			
			// insert data into return list
			int nListitm = m_returnList.InsertItem(0, L"0", 0);
			m_returnList.SetItem(nListitm, 1, LVFIF_TEXT, strCODE, 0, 0, 0, NULL);
			m_returnList.SetItem(nListitm, 2, LVFIF_TEXT, strNAME, 0, 0, 0, NULL);
			m_returnList.SetItem(nListitm, 3, LVFIF_TEXT, strMAKER, 0, 0, 0, NULL);
			m_returnList.SetItem(nListitm, 4, LVFIF_TEXT, strPRICE, 0, 0, 0, NULL);
			m_returnList.SetItem(nListitm, 5, LVFIF_TEXT, strORDERAMT, 0, 0, 0, NULL);
			m_returnList.SetItem(nListitm, 6, LVFIF_TEXT, strREMAIN, 0, 0, 0, NULL);

			//üũ�� �׸� �����ͺ��̽����� ����
			/*
			SQL_deleteItm.Format(L"DELETE FROM ORDER_LIST WHERE PROD_NAME = '%s'", itmName);
			db_order.ExecuteSQL(SQL_deleteItm);
			*/
			//üũ�� �׸� ����Ʈȭ�鿡�� �����ϰ� �޼��� ����
			m_orderList.DeleteItem(i);
			continue;
		}
		else continue;
	}
	
}

void CManageOrder::OnBnClickedReorder()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	// üũ�ڽ��� ���õ� �������� ���ֹ� ��Ͽ� �߰�

	int itemNum = m_orderList.GetItemCount(); //����Ʈ�� �ִ� ������ ������ ���´�
	int *reorderItm;
	int reorderItmCnt = 0; //��ǰ �׸��� �ε����� ������ �迭�� ���ֹ� �׸��� ����
	reorderItm = new int[itemNum];
	CString strSQL, strNAME, strPRICE, strREMAIN, strCODE, strMAKER, strORDERAMT;

	for (int i = 0; i < itemNum; i++)
	{
		//����Ʈ�� ��� �������� ��ȸ�ϸ鼭 üũ�Ǿ����� Ȯ�� ��
		//üũ�� �����ۿ� ���� ���� ����Ʈ���� delete�ϰ� return list�� �߰�

		if (m_orderList.GetCheck(i) == TRUE)
		{
			CString SQL_deleteItm, itmName, str, str1;
			strCODE = m_orderList.GetItemText(i, 0);
			strNAME = m_orderList.GetItemText(i, 1);
			strMAKER = m_orderList.GetItemText(i, 2);
			strPRICE = m_orderList.GetItemText(i, 3);
			strORDERAMT = m_orderList.GetItemText(i, 4);
			strREMAIN = m_orderList.GetItemText(i, 5);

			// insert data into return list
			int nListitm = m_reorderList.InsertItem(0, L"0", 0);
			m_reorderList.SetItem(nListitm, 1, LVFIF_TEXT, strCODE, 0, 0, 0, NULL);
			m_reorderList.SetItem(nListitm, 2, LVFIF_TEXT, strNAME, 0, 0, 0, NULL);
			m_reorderList.SetItem(nListitm, 3, LVFIF_TEXT, strMAKER, 0, 0, 0, NULL);
			m_reorderList.SetItem(nListitm, 4, LVFIF_TEXT, strPRICE, 0, 0, 0, NULL);
			m_reorderList.SetItem(nListitm, 5, LVFIF_TEXT, strORDERAMT, 0, 0, 0, NULL);
			m_reorderList.SetItem(nListitm, 6, LVFIF_TEXT, strREMAIN, 0, 0, 0, NULL);

			//üũ�� �׸� �����ͺ��̽����� ����
			/*
			SQL_deleteItm.Format(L"DELETE FROM ORDER_LIST WHERE PROD_NAME = '%s'", itmName);
			db_order.ExecuteSQL(SQL_deleteItm);
			*/

			//üũ�� �׸� ����Ʈȭ�鿡�� �����ϰ� �޼��� ����
			m_orderList.DeleteItem(i);
			continue;
		}
		else continue;
	}

}



void CManageOrder::OnBnClickedConfirm()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	// �ֹ� ��� ��ü�� �����ۿ� ���� �ֹ� Ȯ��

	CString SQL, orderCode, prodCode, today, dbItmcnt, dbRetcnt;
	int reorderNum, returnNum, confirmNum;
	CTime cTime = CTime::GetCurrentTime();

	// Get current YYYYMMDD from system
	today.Format(L"%04d%02d%02d", cTime.GetYear(), cTime.GetMonth(), cTime.GetDay());

	CRecordset recSet(&db_order);

	// Get the number of orders from ORDER_ table
	recSet.Open(CRecordset::dynaset, L"SELECT COUNT(DISTINCT ORDER_CODE), COUNT(DISTINCT RETURN_CODE) FROM ORDER_LIST, RETURN");
	recSet.GetFieldValue(L"COUNT(DISTINCTORDER_CODE)", dbItmcnt);
	recSet.GetFieldValue(L"COUNT(DISTINCTRETURN_CODE)", dbRetcnt);

	// Convert dbItmcnt to int data, and increase the value
	int tmp = 0;
	tmp = _ttoi(dbItmcnt);
	tmp++;

	int tmp2;
	tmp2 = _ttoi(dbRetcnt);
	tmp2++;

	confirmNum = m_orderList.GetItemCount();
	reorderNum = m_reorderList.GetItemCount();
	returnNum = m_returnList.GetItemCount();

	for (int i = 0; i < confirmNum; i++)
	{
		// Ȯ���� �ֹ���Ͽ� ����
		// ���� ��ǰ �ֹ� ������ŭ ��ǰ ���̺� �ش� ��ǰ ��� �߰�
		CString strStock, strOrderamt;
		int stock = 0, orderamt = 0;

		strStock = m_orderList.GetItemText(i, 5); // ���� ��� ����
		strOrderamt = m_orderList.GetItemText(i, 4); // �ش� ��ǰ�� �ֹ� ����
		stock = _ttoi(strStock);
		orderamt = _ttoi(strOrderamt);
		
		stock += orderamt; // ��� �ֹ� ������ �����ش�
		
		SQL.Format(L"UPDATE PRODUCT SET prod_stock_amount = %d WHERE prod_code = '%s'", stock, m_orderList.GetItemText(i, 0));
		db_order.ExecuteSQL(SQL);

	}

	for (int i = 0; i < reorderNum; i++)
	{
		// ���ֹ� ��
		// �ֹ� ���̺� �� �ֹ��� �����
		SQL.Format(L"INSERT INTO ORDER_LIST(ORDER_CODE, PROD_CODE, ORDER_AMOUNT, ORDER_DATE) VALUES ('%s01%05d', '%s', '%s', '%s')", today, tmp, m_reorderList.GetItemText(i,1), m_reorderList.GetItemText(i,0), today);
		//MessageBox(L"SQL");
		db_order.ExecuteSQL(SQL);

		// ��ǰ ���̺� ���� ��ǰ�� ���� ��� ������ �ֹ����� - ���ֹ� ������ŭ ���ؼ� ������Ʈ
		CString stockAmt, reorderAmt, orderAmt; // ���� ���, ���ֹ� ����, �ֹ� ����
		int _stock, _reorder, _order;

		stockAmt = m_reorderList.GetItemText(i, 6); // ���� ��� ����
		reorderAmt = m_reorderList.GetItemText(i, 0); // ���ֹ��� ���ϴ� ����
		orderAmt = m_reorderList.GetItemText(i, 5); // ���� �ֹ� ����

		_stock = _ttoi(stockAmt);
		_reorder = _ttoi(reorderAmt);
		_order = _ttoi(orderAmt);

		_order -= _reorder;
		_stock += _order;

		SQL.Format(L"UPDATE PRODUCT SET prod_stock_amount = %d WHERE prod_code = '%s'", _stock, m_reorderList.GetItemText(i, 1));
		db_order.ExecuteSQL(SQL);
	}

	for (int i = 0; i < returnNum; i++)
	{
		// ��ǰ ��
		// ��ǰ ���̺� ���� �ֹ��� ���� �ֹ� ��ǰ ���ڵ带 ���� �����

		SQL.Format(L"INSERT INTO RETURN(RETURN_CODE, PROD_CODE, RETURN_HIGH_CODE, RETURN_AMOUNT) VALUES ('%s01%04d0', '%s', '%s', %s)", today, tmp2, m_returnList.GetItemText(i,1), order_id, m_returnList.GetItemText(i,0));
		db_order.ExecuteSQL(SQL);

		// ��ǰ ���̺� ���� ��ǰ�� ���� ��� ������ �ֹ����� - ��ǰ ������ŭ ���ؼ� ������Ʈ
		CString stockAmt, returnAmt, orderAmt; // ���� ���, ���ֹ� ����, �ֹ� ����
		int _stock, _return, _order;

		stockAmt = m_returnList.GetItemText(i, 6); // ���� ��� ����
		returnAmt = m_returnList.GetItemText(i, 0); // ���ֹ��� ���ϴ� ����
		orderAmt = m_returnList.GetItemText(i, 5); // ���� �ֹ� ����

		_stock = _ttoi(stockAmt);
		_return = _ttoi(returnAmt);
		_order = _ttoi(orderAmt);

		_order -= _return;
		_stock += _order;

		SQL.Format(L"UPDATE PRODUCT SET prod_stock_amount = %d WHERE prod_code = '%s'", _stock, m_returnList.GetItemText(i, 1));
		db_order.ExecuteSQL(SQL);
	}
	
	if (reorderNum > 0)
	{
		SQL.Format(L"UPDATE ORDER_LIST SET REORDER = '%s01%05d' WHERE ORDER_CODE = '%s'", today, tmp, order_id); // ���ֹ��� �߻��ϴ� ���� ���ڵ��� REORDER���� ���ֹ����� ��������� �� �ڽ� �ֹ� ���ڵ尡 ��
		db_order.ExecuteSQL(SQL);
	}
	else
	{
		SQL.Format(L"UPDATE ORDER_LIST SET REORDER ='N' WHERE ORDER_CODE = '%s'", order_id); // ���ֹ��� �߻��ϴ� ���� ���ڵ��� REORDER���� ���ֹ� ���� ���ΰ� ��
		db_order.ExecuteSQL(SQL);
	}

	OnBnClickedOk();

}


void CManageOrder::OnBnClickedOk()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	db_order.Close();
	CDialogEx::OnOK();
}


void CManageOrder::OnEndlabeleditReorderList(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>( pNMHDR );
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	/*
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>( pNMHDR );
	CString orderAmt, modifiedAmt; // ��ǰ�� ���, ������ ����
	int SelectedIdx, _orderAmt, _modifiedAmt;

	SelectedIdx = pNMItemActivate->iItem; // Get the index number of the edited item
	orderAmt = m_reorderList.GetItemText(SelectedIdx, 5);
	modifiedAmt = m_reorderList.GetItemText(SelectedIdx, 0);
	_orderAmt = _ttoi(orderAmt);
	_modifiedAmt = _ttoi(modifiedAmt);

	// ���� �ֹ��� �纸�� ������ ���� ũ�� ������ ���´�
	if( _orderAmt < _modifiedAmt )
		*pResult = 0;
	else */*pResult = TRUE;

}


void CManageOrder::OnEndlabeleditReturnList(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>( pNMHDR );
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	/*
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>( pNMHDR );
	CString orderAmt, modifiedAmt; // ��ǰ�� ���, ������ ����
	int SelectedIdx, _orderAmt, _modifiedAmt;

	SelectedIdx = pNMItemActivate->iItem; // Get the index number of the edited item
	orderAmt = m_returnList.GetItemText(SelectedIdx, 5);
	modifiedAmt = m_returnList.GetItemText(SelectedIdx, 0);
	_orderAmt = _ttoi(orderAmt);
	_modifiedAmt = _ttoi(modifiedAmt);

	MessageBox(orderAmt);
	MessageBox(modifiedAmt);

	// ���� �ֹ��� �纸�� ������ ���� ũ�� ������ ���´�
	if (_orderAmt < _modifiedAmt)
		*pResult = 0;
	else */*pResult = TRUE;
}



void CManageOrder::OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
}
