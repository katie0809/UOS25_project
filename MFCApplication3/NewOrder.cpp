// NewOrder.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MFCApplication3.h"
#include "NewOrder.h"
#include "afxdialogex.h"


// NewOrder 대화 상자입니다.

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


// NewOrder 메시지 처리기입니다.


BOOL NewOrder::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	// 데이터베이스와의 연결

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
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void NewOrder::ShowData(CDatabase & db_neworder)
{
	CRecordset recSet(&db_neworder);
	CString strSQL, strCODE, strNAME, strMAKER, strPRICE, strSTOCKAMOUNT;

	// 상품코드, 이름, 제조사, 가격, 재고 정보 컬럼을 만든다
	// 리스트 스타일 설정
	m_newOrder.ModifyStyle(0, LVS_REPORT | LVS_SHOWSELALWAYS | LVS_EDITLABELS);
	m_newOrder.SetExtendedStyle(LVS_EX_CHECKBOXES | LVS_EX_FULLROWSELECT);
	
	// 컬럼 삽입
	m_newOrder.InsertColumn(1, L"주문수량", LVCFMT_CENTER, 80);
	m_newOrder.InsertColumn(2, L"상품코드", LVCFMT_CENTER, 150);
	m_newOrder.InsertColumn(3, L"상품명", LVCFMT_CENTER, 150);
	m_newOrder.InsertColumn(4, L"제조사", LVCFMT_CENTER, 100);
	m_newOrder.InsertColumn(5, L"가격", LVCFMT_CENTER, 100);
	m_newOrder.InsertColumn(6, L"재고수량", LVCFMT_CENTER, 80);

	// 쿼리문을 통해 특정 날짜의 주문목록만 받아온다
	strSQL.Format(L"SELECT * FROM PRODUCT");
	recSet.Open(CRecordset::dynaset, strSQL);

	// 받아온 테이블에 남은 데이터가 없을 때까지 실행
	while (!recSet.IsEOF())
	{
		int idx = 0;

		recSet.GetFieldValue(_T("PROD_NUM"), strCODE);
		recSet.GetFieldValue(_T("PROD_NAME"), strNAME);
		recSet.GetFieldValue(_T("PROD_MAKER"), strMAKER);
		recSet.GetFieldValue(_T("PROD_PRICE"), strPRICE);
		recSet.GetFieldValue(_T("PROD_STOCK_AMOUNT"), strSTOCKAMOUNT);
		
		// 테이블에 데이터 삽입
		int nListitm = m_newOrder.InsertItem(0, L"1", 0); // 기본 주문수량은 1개
		m_newOrder.SetItem(nListitm, 1, LVIF_TEXT, strCODE, 0, 0, 0, NULL);
		m_newOrder.SetItem(nListitm, 2, LVIF_TEXT, strNAME, 0, 0, 0, NULL);
		m_newOrder.SetItem(nListitm, 3, LVIF_TEXT, strMAKER, 0, 0, 0, NULL);
		m_newOrder.SetItem(nListitm, 4, LVIF_TEXT, strPRICE, 0, 0, 0, NULL);
		m_newOrder.SetItem(nListitm, 5, LVIF_TEXT, strSTOCKAMOUNT, 0, 0, 0, NULL);


		//다음 레코드로 이동
		recSet.MoveNext();
	}

	recSet.Close();

}

void NewOrder::OnBnClickedOrder()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// 목록에 체크된 아이템을 받아 주문 개수 조정 다이얼로그로 보냅니다

	CRecordset recSet(&db_neworder);
	int idx = 0, sum = 0; // 총 주문금액
	int nItmcnt = m_newOrder.GetItemCount();
	CString sql_list, dbItmcnt; // 항목 삽입 위한 sql문 변수와 디비 주문 개수 얻어오기 위한 변수
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
		// 리스트의 전체 항목을 순회하면서 체크 되었는지 확인
		if (m_newOrder.GetCheck(i) == TRUE)
		{
			// If itm is checked, add data to database using SQLct
			// 항목이 체크되었으면 SQL로 데이터베이스에 항목 추가
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
	MessageBox(L"주문 완료");
	OnBnClickedOk();
}


void NewOrder::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	db_neworder.Close();
	CDialogEx::OnOK();
}



void NewOrder::OnEndlabeleditNewOrder(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>( pNMHDR );
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = TRUE;
}
