// ManageOrder.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MFCApplication3.h"
#include "ManageOrder.h"
#include "afxdialogex.h"
#include "ContentView.h"

// CManageOrder 대화 상자입니다.

/*
	주문 관리를 위한 다이얼로그 클래스
*/

IMPLEMENT_DYNAMIC(CManageOrder, CDialogEx)

//생성자 오버로드
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


// CManageOrder 메시지 처리기입니다.


BOOL CManageOrder::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	// 데이터베이스와의 연결
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
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CManageOrder::ModifyStyle()
{
	// Modify return list style
	m_returnList.ModifyStyle(0, LVS_REPORT | LVS_SHOWSELALWAYS | LVS_SINGLESEL | LVS_EDITLABELS, 0);
	m_returnList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	// Create Column
	m_returnList.InsertColumn(0, L"반품 수량", LVCFMT_CENTER, 80);
	m_returnList.InsertColumn(1, L"상품 코드", LVCFMT_CENTER, 80);
	m_returnList.InsertColumn(2, L"상품명", LVCFMT_CENTER, 100);
	m_returnList.InsertColumn(3, L"제조사", LVCFMT_CENTER, 100);
	m_returnList.InsertColumn(4, L"상품 가격", LVCFMT_CENTER, 100);
	m_returnList.InsertColumn(5, L"주문 수량", LVCFMT_CENTER, 50);
	m_returnList.InsertColumn(6, L"재고 수량", LVCFMT_CENTER, 50);

	// Modify reorder list style
	m_reorderList.ModifyStyle(0, LVS_REPORT | LVS_SHOWSELALWAYS | LVS_SINGLESEL | LVS_EDITLABELS, 0);
	m_reorderList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	// Create Column
	m_reorderList.InsertColumn(0, L"재주문 수량", LVCFMT_CENTER, 80);
	m_reorderList.InsertColumn(1, L"상품 코드", LVCFMT_CENTER, 80);
	m_reorderList.InsertColumn(2, L"상품명", LVCFMT_CENTER, 100);
	m_reorderList.InsertColumn(3, L"제조사", LVCFMT_CENTER, 100);
	m_reorderList.InsertColumn(4, L"상품 가격", LVCFMT_CENTER, 100);
	m_reorderList.InsertColumn(5, L"주문 수량", LVCFMT_CENTER, 50);
	m_reorderList.InsertColumn(6, L"재고 수량", LVCFMT_CENTER, 50);
}

void CManageOrder::ShowData(CDatabase & db_order)
{

	CRecordset recSet(&db_order);
	CString strSQL, strNAME, strPRICE, strREMAIN, strCODE, strMAKER, strORDERAMT;

	// Modify list style
	m_orderList.ModifyStyle(0, LVS_REPORT | LVS_SHOWSELALWAYS, 0);
	m_orderList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_CHECKBOXES);

	// 쿼리문을 통해 특정 날짜의 주문목록만 받아온다
	// Get product code, product name, product maker, product price, order amount, product stock amount, event detail
	strSQL.Format(L"select product.prod_code, prod_name, prod_maker, prod_price, order_amount, prod_stock_amount from product inner join order_list on order_list.order_code = '%s' and order_list.prod_code = product.prod_code", order_id);
	recSet.Open(CRecordset::dynaset, strSQL);

	// Create Column
	m_orderList.InsertColumn(0, L"상품 코드", LVCFMT_CENTER, 80);
	m_orderList.InsertColumn(1, L"상품명", LVCFMT_CENTER, 100);
	m_orderList.InsertColumn(2, L"제조사", LVCFMT_CENTER, 100);
	m_orderList.InsertColumn(3, L"상품 가격", LVCFMT_CENTER, 100);
	m_orderList.InsertColumn(4, L"주문 수량", LVCFMT_CENTER, 50);
	m_orderList.InsertColumn(5, L"재고 수량", LVCFMT_CENTER, 50);

	// 받아온 테이블에 남은 데이터가 없을 때까지 실행
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

		//다음 레코드로 이동
		recSet.MoveNext();
	}

	recSet.Close();
	
}



void CManageOrder::OnBnClickedReturn()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// checkbox로 선택된 아이템을 반품 목록에 추가

	int itemNum = m_orderList.GetItemCount(); //리스트에 있는 아이템 개수를 얻어온다
	int *returnItm;
	int returnItmCnt = 0; //반품 항목의 인덱스를 저장할 배열과 반품 항목의 개수
	returnItm = new int[itemNum];
	CString strSQL, strNAME, strPRICE, strREMAIN, strCODE, strMAKER, strORDERAMT;

	for (int i = 0; i < itemNum; i++)
	{
		//리스트의 모든 아이템을 순회하면서 체크되었는지 확인 후
		//체크된 아이템에 한해 원래 리스트에서 delete하고 return list에 추가

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

			//체크된 항목 데이터베이스에서 삭제
			/*
			SQL_deleteItm.Format(L"DELETE FROM ORDER_LIST WHERE PROD_NAME = '%s'", itmName);
			db_order.ExecuteSQL(SQL_deleteItm);
			*/
			//체크된 항목 리스트화면에서 삭제하고 메세지 띄운다
			m_orderList.DeleteItem(i);
			continue;
		}
		else continue;
	}
	
}

void CManageOrder::OnBnClickedReorder()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// 체크박스로 선택된 아이템을 재주문 목록에 추가

	int itemNum = m_orderList.GetItemCount(); //리스트에 있는 아이템 개수를 얻어온다
	int *reorderItm;
	int reorderItmCnt = 0; //반품 항목의 인덱스를 저장할 배열과 재주문 항목의 개수
	reorderItm = new int[itemNum];
	CString strSQL, strNAME, strPRICE, strREMAIN, strCODE, strMAKER, strORDERAMT;

	for (int i = 0; i < itemNum; i++)
	{
		//리스트의 모든 아이템을 순회하면서 체크되었는지 확인 후
		//체크된 아이템에 한해 원래 리스트에서 delete하고 return list에 추가

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

			//체크된 항목 데이터베이스에서 삭제
			/*
			SQL_deleteItm.Format(L"DELETE FROM ORDER_LIST WHERE PROD_NAME = '%s'", itmName);
			db_order.ExecuteSQL(SQL_deleteItm);
			*/

			//체크된 항목 리스트화면에서 삭제하고 메세지 띄운다
			m_orderList.DeleteItem(i);
			continue;
		}
		else continue;
	}

}



void CManageOrder::OnBnClickedConfirm()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// 주문 목록 전체의 아이템에 대해 주문 확정

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
		// 확정된 주문목록에 대해
		// 현재 상품 주문 수량만큼 상품 테이블 해당 상품 재고에 추가
		CString strStock, strOrderamt;
		int stock = 0, orderamt = 0;

		strStock = m_orderList.GetItemText(i, 5); // 현재 재고 개수
		strOrderamt = m_orderList.GetItemText(i, 4); // 해당 상품의 주문 개수
		stock = _ttoi(strStock);
		orderamt = _ttoi(strOrderamt);
		
		stock += orderamt; // 재고에 주문 수량을 더해준다
		
		SQL.Format(L"UPDATE PRODUCT SET prod_stock_amount = %d WHERE prod_code = '%s'", stock, m_orderList.GetItemText(i, 0));
		db_order.ExecuteSQL(SQL);

	}

	for (int i = 0; i < reorderNum; i++)
	{
		// 재주문 시
		// 주문 테이블에 새 주문을 만든다
		SQL.Format(L"INSERT INTO ORDER_LIST(ORDER_CODE, PROD_CODE, ORDER_AMOUNT, ORDER_DATE) VALUES ('%s01%05d', '%s', '%s', '%s')", today, tmp, m_reorderList.GetItemText(i,1), m_reorderList.GetItemText(i,0), today);
		//MessageBox(L"SQL");
		db_order.ExecuteSQL(SQL);

		// 물품 테이블에 현재 상품에 대한 재고 수량을 주문수량 - 재주문 수량만큼 더해서 업데이트
		CString stockAmt, reorderAmt, orderAmt; // 현재 재고, 재주문 수량, 주문 수량
		int _stock, _reorder, _order;

		stockAmt = m_reorderList.GetItemText(i, 6); // 현재 재고 개수
		reorderAmt = m_reorderList.GetItemText(i, 0); // 재주문을 원하는 개수
		orderAmt = m_reorderList.GetItemText(i, 5); // 원래 주문 개수

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
		// 반품 시
		// 반품 테이블에 현재 주문에 대한 주문 반품 레코드를 새로 만든다

		SQL.Format(L"INSERT INTO RETURN(RETURN_CODE, PROD_CODE, RETURN_HIGH_CODE, RETURN_AMOUNT) VALUES ('%s01%04d0', '%s', '%s', %s)", today, tmp2, m_returnList.GetItemText(i,1), order_id, m_returnList.GetItemText(i,0));
		db_order.ExecuteSQL(SQL);

		// 물품 테이블에 현재 상품에 대한 재고 수량을 주문수량 - 반품 수량만큼 더해서 업데이트
		CString stockAmt, returnAmt, orderAmt; // 현재 재고, 재주문 수량, 주문 수량
		int _stock, _return, _order;

		stockAmt = m_returnList.GetItemText(i, 6); // 현재 재고 개수
		returnAmt = m_returnList.GetItemText(i, 0); // 재주문을 원하는 개수
		orderAmt = m_returnList.GetItemText(i, 5); // 원래 주문 개수

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
		SQL.Format(L"UPDATE ORDER_LIST SET REORDER = '%s01%05d' WHERE ORDER_CODE = '%s'", today, tmp, order_id); // 재주문이 발생하는 원래 레코드의 REORDER에는 재주문으로 만들어지는 새 자식 주문 레코드가 들어감
		db_order.ExecuteSQL(SQL);
	}
	else
	{
		SQL.Format(L"UPDATE ORDER_LIST SET REORDER ='N' WHERE ORDER_CODE = '%s'", order_id); // 재주문이 발생하는 원래 레코드의 REORDER에는 재주문 존재 여부가 들어감
		db_order.ExecuteSQL(SQL);
	}

	OnBnClickedOk();

}


void CManageOrder::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	db_order.Close();
	CDialogEx::OnOK();
}


void CManageOrder::OnEndlabeleditReorderList(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>( pNMHDR );
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	/*
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>( pNMHDR );
	CString orderAmt, modifiedAmt; // 상품의 재고, 수정된 수량
	int SelectedIdx, _orderAmt, _modifiedAmt;

	SelectedIdx = pNMItemActivate->iItem; // Get the index number of the edited item
	orderAmt = m_reorderList.GetItemText(SelectedIdx, 5);
	modifiedAmt = m_reorderList.GetItemText(SelectedIdx, 0);
	_orderAmt = _ttoi(orderAmt);
	_modifiedAmt = _ttoi(modifiedAmt);

	// 원래 주문한 양보다 수정된 값이 크면 수정을 막는다
	if( _orderAmt < _modifiedAmt )
		*pResult = 0;
	else */*pResult = TRUE;

}


void CManageOrder::OnEndlabeleditReturnList(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>( pNMHDR );
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	/*
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>( pNMHDR );
	CString orderAmt, modifiedAmt; // 상품의 재고, 수정된 수량
	int SelectedIdx, _orderAmt, _modifiedAmt;

	SelectedIdx = pNMItemActivate->iItem; // Get the index number of the edited item
	orderAmt = m_returnList.GetItemText(SelectedIdx, 5);
	modifiedAmt = m_returnList.GetItemText(SelectedIdx, 0);
	_orderAmt = _ttoi(orderAmt);
	_modifiedAmt = _ttoi(modifiedAmt);

	MessageBox(orderAmt);
	MessageBox(modifiedAmt);

	// 원래 주문한 양보다 수정된 값이 크면 수정을 막는다
	if (_orderAmt < _modifiedAmt)
		*pResult = 0;
	else */*pResult = TRUE;
}



void CManageOrder::OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}
