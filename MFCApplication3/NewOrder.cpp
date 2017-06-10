// NewOrder.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "MFCApplication3.h"
#include "NewOrder.h"
#include "afxdialogex.h"


// NewOrder ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(NewOrder, CDialogEx)

NewOrder::NewOrder(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_NEW_ORDER, pParent)
{

}

NewOrder::~NewOrder()
{
}

void NewOrder::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_NEW_ORDER, m_newOrder);
}



BEGIN_MESSAGE_MAP(NewOrder, CDialogEx)
	ON_BN_CLICKED(IDC_ORDER, &NewOrder::OnBnClickedOrder)
	ON_BN_CLICKED(IDOK, &NewOrder::OnBnClickedOk)
	ON_NOTIFY(LVN_ENDLABELEDIT, IDC_NEW_ORDER, &NewOrder::OnEndlabeleditNewOrder)
END_MESSAGE_MAP()


// NewOrder �޽��� ó�����Դϴ�.


BOOL NewOrder::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

	CDialogEx::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	// �����ͺ��̽����� ����

	TRY
	{
		db_neworder.OpenEx(_T("DSN=UOS25;UID=UOS25;PWD=0000"));
	}
		CATCH(CException, e)
	{
		TCHAR errMSG[255];

		e->GetErrorMessage(errMSG, 255);
		AfxMessageBox(errMSG, MB_ICONERROR);


		if (db_neworder.IsOpen())
			db_neworder.Close();

	}
	END_CATCH

		ShowData(db_neworder);
	   // db_neworder.Close();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}

void NewOrder::ShowData(CDatabase & db_neworder)
{
	CRecordset recSet(&db_neworder);
	CString strSQL, strCODE, strNAME, strMAKER, strPRICE, strSTOCKAMOUNT;

	// ��ǰ�ڵ�, �̸�, ������, ����, ��� ���� �÷��� �����
	// ����Ʈ ��Ÿ�� ����
	m_newOrder.ModifyStyle(0, LVS_REPORT | LVS_SHOWSELALWAYS | LVS_EDITLABELS);
	m_newOrder.SetExtendedStyle(LVS_EX_CHECKBOXES | LVS_EX_FULLROWSELECT);
	
	// �÷� ����
	m_newOrder.InsertColumn(1, L"�ֹ�����", LVCFMT_CENTER, 80);
	m_newOrder.InsertColumn(2, L"��ǰ�ڵ�", LVCFMT_CENTER, 150);
	m_newOrder.InsertColumn(3, L"��ǰ��", LVCFMT_CENTER, 150);
	m_newOrder.InsertColumn(4, L"������", LVCFMT_CENTER, 100);
	m_newOrder.InsertColumn(5, L"����", LVCFMT_CENTER, 100);
	m_newOrder.InsertColumn(6, L"������", LVCFMT_CENTER, 80);

	// �������� ���� Ư�� ��¥�� �ֹ���ϸ� �޾ƿ´�
	strSQL.Format(L"SELECT * FROM PRODUCT");
	recSet.Open(CRecordset::dynaset, strSQL);

	// �޾ƿ� ���̺� ���� �����Ͱ� ���� ������ ����
	while (!recSet.IsEOF())
	{
		int idx = 0;

		recSet.GetFieldValue(_T("PROD_NUM"), strCODE);
		recSet.GetFieldValue(_T("PROD_NAME"), strNAME);
		recSet.GetFieldValue(_T("PROD_MAKER"), strMAKER);
		recSet.GetFieldValue(_T("PROD_PRICE"), strPRICE);
		recSet.GetFieldValue(_T("PROD_STOCK_AMOUNT"), strSTOCKAMOUNT);
		
		// ���̺� ������ ����
		int nListitm = m_newOrder.InsertItem(0, L"1", 0); // �⺻ �ֹ������� 1��
		m_newOrder.SetItem(nListitm, 1, LVIF_TEXT, strCODE, 0, 0, 0, NULL);
		m_newOrder.SetItem(nListitm, 2, LVIF_TEXT, strNAME, 0, 0, 0, NULL);
		m_newOrder.SetItem(nListitm, 3, LVIF_TEXT, strMAKER, 0, 0, 0, NULL);
		m_newOrder.SetItem(nListitm, 4, LVIF_TEXT, strPRICE, 0, 0, 0, NULL);
		m_newOrder.SetItem(nListitm, 5, LVIF_TEXT, strSTOCKAMOUNT, 0, 0, 0, NULL);


		//���� ���ڵ�� �̵�
		recSet.MoveNext();
	}

	recSet.Close();

}

void NewOrder::OnBnClickedOrder()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	// ��Ͽ� üũ�� �������� �޾� �ֹ� ���� ���� ���̾�α׷� �����ϴ�

	CRecordset recSet(&db_neworder);
	int idx = 0, sum = 0; // �� �ֹ��ݾ�
	int nItmcnt = m_newOrder.GetItemCount();
	CString sql_list, dbItmcnt; // �׸� ���� ���� sql�� ������ ��� �ֹ� ���� ������ ���� ����
	CTime cTime = CTime::GetCurrentTime();
	CString today;

	// Get current YYYYMMDD from system
	today.Format(L"%04d%02d%02d", cTime.GetYear(), cTime.GetMonth(), cTime.GetDay());

	// Get the number of orders from ORDER_ table
	recSet.Open(CRecordset::dynaset, L"SELECT COUNT(DISTINCT ORDER_NUM) FROM ORDER_LIST");
	recSet.GetFieldValue(L"COUNT(DISTINCTORDER_NUM)", dbItmcnt);
	
	// Convert dbItmcnt to int data, and increase the value
	int tmp = 0;
	tmp = _ttoi(dbItmcnt);
	tmp++;
	

	for (int i = 0; i < nItmcnt; i++)
	{
		// ����Ʈ�� ��ü �׸��� ��ȸ�ϸ鼭 üũ �Ǿ����� Ȯ��
		if (m_newOrder.GetCheck(i) == TRUE)
		{
			// If itm is checked, add data to database using SQLct
			// �׸��� üũ�Ǿ����� SQL�� �����ͺ��̽��� �׸� �߰�
			sql_list.Format(L"insert into order_list(order_num, prod_num, order_date, order_amount) values ('%s01%05d', '%s', '%s', %s)", today, tmp, m_newOrder.GetItemText(i,1), today, m_newOrder.GetItemText(i, 0));
			// MessageBox(sql_list);
			db_neworder.ExecuteSQL(sql_list);

			CString price = m_newOrder.GetItemText(i, 4);
			CString orderCnt = m_newOrder.GetItemText(i, 0);
			int _price = _ttoi(price);
			int _orderCnt = _ttoi(orderCnt);

			sum += ( _price * _orderCnt );

			idx++;
		}
		else continue;
	}
	CString funds_date;
	funds_date.Format(L"%04d%02d%02d%02d%02d%02d", cTime.GetYear(), cTime.GetMonth(), cTime.GetDay(), cTime.GetHour(), cTime.GetMinute(), cTime.GetSecond());

	sql_list.Format(L"INSERT INTO FUNDS(FUNDS_DATE, FUNDS_DETAIL_NUM, PRICE, FUNDS_SORT_CODE) values ('%s', '%s01%05d', '-%d', 'F05')", funds_date, today, tmp, sum);
	//MessageBox(sql_list);
	db_neworder.ExecuteSQL(sql_list);
	MessageBox(L"�ֹ� �Ϸ�");
	OnBnClickedOk();
}


void NewOrder::OnBnClickedOk()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	db_neworder.Close();
	CDialogEx::OnOK();
}



void NewOrder::OnEndlabeleditNewOrder(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>( pNMHDR );
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	*pResult = TRUE;
}
