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

/*
CManageOrder::CManageOrder(CWnd* pParent /*=NULL)
	: CDialogEx(IDD_MANAGE_ORDER, pParent)
{

}
*/
//생성자 오버로드
CManageOrder::CManageOrder(CWnd *pParent, CString selected_itm)
	: CDialogEx(IDD_MANAGE_ORDER, pParent)
{
	CTime cTime = CTime::GetCurrentTime();

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
	
	ShowData(db_order);
	//db_order.Close();
	
	return TRUE;  
	// return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CManageOrder::ShowData(CDatabase & db_order)
{

	CRecordset recSet(&db_order);
	CString strSQL, strNAME, strPRICE, strREMAIN, strCODE, strMAKER, strORDERAMT;

	// Modify list style
	m_orderList.ModifyStyle(0, LVS_REPORT | LVS_SHOWSELALWAYS | LVS_SINGLESEL, 0);
	m_orderList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_CHECKBOXES);

	// 쿼리문을 통해 특정 날짜의 주문목록만 받아온다
	// Get product code, product name, product maker, product price, order amount, product stock amount, event detail
	strSQL.Format(L"select product.prod_code, prod_name, prod_maker, prod_price, order_amount, prod_stock_amount from product inner join order_ on order_.order_code = '%s' and order_.prod_code = product.prod_code", order_id);
	recSet.Open(CRecordset::dynaset, strSQL);

	// Create Column
	m_orderList.InsertColumn(1, L"상품 코드", LVCFMT_CENTER, 80);
	m_orderList.InsertColumn(2, L"상품명", LVCFMT_CENTER, 100);
	m_orderList.InsertColumn(3, L"제조시", LVCFMT_CENTER, 100);
	m_orderList.InsertColumn(4, L"상품 가격", LVCFMT_CENTER, 100);
	m_orderList.InsertColumn(5, L"주문 수량", LVCFMT_CENTER, 50);
	m_orderList.InsertColumn(6, L"재고 수량", LVCFMT_CENTER, 50);

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
	// checkbox로 선택된 아이템을 주문수정한다.
/*
	int itemNum = m_orderlist.GetItemCount(); //리스트에 있는 아이템 개수를 얻어온다
	int *returnItm, returnItmCnt = 0; //반품 항목의 인덱스를 저장할 배열과 재주문 항목의 개수
	returnItm = new int[itemNum]; 

	for (int i = 0; i < itemNum; i++)
	{
		//리스트의 모든 아이템을 순회하면서 체크되었는지 확인 후
		//체크된 아이템에 한해 delete한다

		if (m_orderlist.GetCheck(i) == BST_CHECKED)
		{
			CString SQL_deleteItm, itmName, str, str1;
			m_orderlist.GetText(i, itmName);
			AfxExtractSubString(itmName, itmName, 0, ' ');
			//MessageBox(itmName);

			//체크된 항목 데이터베이스에서 삭제
			SQL_deleteItm.Format(L"DELETE FROM ORDER_LIST WHERE PROD_NAME = '%s'", itmName);
			db_order.ExecuteSQL(SQL_deleteItm);

			//반품 항목의 인덱스 번호 배열에 저장 후 배열 카운트 +1
			returnItm[returnItmCnt] = i;
			returnItmCnt++;
		}
	}
	db_order.Close();
	
	//체크된 항목 리스트화면에서 삭제하고 메세지 띄운다
	for (int i = 0; i <= returnItmCnt; i++)
		m_orderList.DeleteString(i);
		*/
	MessageBox(L"재주문 완료");
}


void CManageOrder::OnBnClickedConfirm()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// 주문 목록 전체의 아이템에 대해 주문 확정
}


void CManageOrder::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialogEx::OnOK();
	db_order.Close();
}


void CManageOrder::OnBnClickedReorder()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
