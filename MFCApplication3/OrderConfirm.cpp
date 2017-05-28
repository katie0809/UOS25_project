// OrderConfirm.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MFCApplication3.h"
#include "OrderConfirm.h"
#include "afxdialogex.h"


// COrderConfirm 대화 상자입니다.

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
	//DDX_Control(pDX, IDC_EDIT1, return_code);
	DDX_Control(pDX, IDC_LIST2, reorderCode);
	DDX_Control(pDX, IDC_LIST1, returnCode);
}


BEGIN_MESSAGE_MAP(COrderConfirm, CDialogEx)
END_MESSAGE_MAP()


// COrderConfirm 메시지 처리기입니다.


BOOL COrderConfirm::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

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
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void COrderConfirm::ShowData(CDatabase & db_order)
{

	CRecordset recSet(&db_order);
	CString strSQL, strNAME, strPRICE, strREMAIN, strCODE, strMAKER, strORDERAMT, strREORDER, strRETURN;

	// Modify list style
	m_orderList.ModifyStyle(0, LVS_REPORT | LVS_SHOWSELALWAYS, 0);
	m_orderList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	// 쿼리문을 통해 특정 날짜의 주문목록만 받아온다
	// Get product code, product name, product maker, product price, order amount, product stock amount, event detail
	strSQL.Format(L"select product.prod_code, prod_name, prod_maker, prod_price, order_amount, prod_stock_amount, reorder from product inner join order_list on order_list.order_code = '%s' and order_list.prod_code = product.prod_code", order_id);
	recSet.Open(CRecordset::dynaset, strSQL);
	recSet.GetFieldValue(_T("REORDER"), strREORDER);

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

	//strREORDER = L"";
	//strRETURN = L"";

	// 재주문 코드를 보여준다
	reorderCode.AddString(strREORDER);

	// 반품 코드를 보여준다
	strSQL.Format(L"select DISTINCT return_code from return where return_high_code = '%s'", order_id);
	recSet.Open(CRecordset::dynaset, strSQL);

	if (recSet.GetRecordCount() > 0)
	{
		recSet.GetFieldValue(_T("RETURN_CODE"), strRETURN);
		returnCode.AddString(strRETURN);
	}
}

