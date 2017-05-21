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
CManageOrder::CManageOrder(CWnd *pParent, int selected_itm)
	: CDialogEx(IDD_MANAGE_ORDER, pParent)
{
	order_id = selected_itm + 1;
	/*
	// 2017-03-09 00:00:00과 같은 값을 받아와 09-MAR-2017 텍스트로 변환해주는 코드입니다

	CString year, month, day;

	AfxExtractSubString(selected_itm, selected_itm, 0, ' ');
	AfxExtractSubString(year, selected_itm, 0, '-');
	AfxExtractSubString(month, selected_itm, 1, '-');
	AfxExtractSubString(day, selected_itm, 2, '-');

	CString Calendar[12] = { L"JAN" ,L"FEB" ,L"MAR" ,L"APR" ,L"MAY" ,L"JUN" ,L"JUL" ,L"AUG" ,L"SEP" ,L"OCT" ,L"NOV" ,L"DEC" };

	current_date.Format(L"%s-%s-%s", day, Calendar[_ttoi(month)-1], year);
	*/
}

CManageOrder::~CManageOrder()
{
}

void CManageOrder::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_orderList);
	DDX_Control(pDX, IDC_LIST2, m_orderlist);
}


BEGIN_MESSAGE_MAP(CManageOrder, CDialogEx)
	ON_BN_CLICKED(IDC_RETURN, &CManageOrder::OnBnClickedReturn)
	ON_BN_CLICKED(IDC_CONFIRM, &CManageOrder::OnBnClickedConfirm)
	ON_BN_CLICKED(IDOK, &CManageOrder::OnBnClickedOk)
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
	CString strSQL, strNAME, strPRICE, strREMAIN, strISRETURN;

	// 쿼리문을 통해 특정 날짜의 주문목록만 받아온다
	strSQL.Format(L"SELECT * FROM order_list WHERE order_id = %d", order_id);
	recSet.Open(CRecordset::dynaset, strSQL);

	// 받아온 테이블에 남은 데이터가 없을 때까지 실행
	while (!recSet.IsEOF())
	{
		int idx = 0;
		
		recSet.GetFieldValue(_T("PROD_NAME"), strNAME);
		recSet.GetFieldValue(_T("PROD_PRICE"), strPRICE);
		recSet.GetFieldValue(_T("PROD_REMAIN"), strREMAIN);
		recSet.GetFieldValue(_T("PROD_ISRETURN"), strISRETURN);

		CString str;
		str.Format(L"%s \t %s \t %s \t %s", strNAME, strPRICE, strREMAIN, strISRETURN);
		m_orderList.AddString(str);

		//다음 레코드로 이동
		recSet.MoveNext();
	}

	recSet.Close();

}



void CManageOrder::OnBnClickedReturn()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// checkbox로 선택된 아이템을 재주문한다.

	int itemNum = m_orderList.GetCount(); //리스트에 있는 아이템 개수를 얻어온다
	int *returnItm, returnItmCnt = 0; //반품 항목의 인덱스를 저장할 배열과 재주문 항목의 개수
	returnItm = new int[itemNum]; 

	for (int i = 0; i < itemNum; i++)
	{
		//리스트의 모든 아이템을 순회하면서 체크되었는지 확인 후
		//체크된 아이템에 한해 delete한다

		if (m_orderList.GetCheck(i) == BST_CHECKED)
		{
			CString SQL_deleteItm, itmName, str, str1;
			m_orderList.GetText(i, itmName);
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

	MessageBox(L"재주문 완료");
}


void CManageOrder::OnBnClickedConfirm()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CManageOrder::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialogEx::OnOK();
	db_order.Close();
}
