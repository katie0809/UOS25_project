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
	DDX_Control(pDX, IDC_NEW_ORDER, m_neworderList);
}



BEGIN_MESSAGE_MAP(NewOrder, CDialogEx)
	ON_BN_CLICKED(IDC_ORDER, &NewOrder::OnBnClickedOrder)
END_MESSAGE_MAP()


// NewOrder 메시지 처리기입니다.


BOOL NewOrder::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	// 데이터베이스와의 연결
	// CDatabase db_order;

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

	// 쿼리문을 통해 특정 날짜의 주문목록만 받아온다
	strSQL.Format(L"SELECT * FROM PRODUCT");
	recSet.Open(CRecordset::dynaset, strSQL);

	// 받아온 테이블에 남은 데이터가 없을 때까지 실행
	while (!recSet.IsEOF())
	{
		int idx = 0;

		recSet.GetFieldValue(_T("PROD_CODE"), strCODE);
		recSet.GetFieldValue(_T("PROD_NAME"), strNAME);
		recSet.GetFieldValue(_T("PROD_MAKER"), strMAKER);
		recSet.GetFieldValue(_T("PROD_PRICE"), strPRICE);
		recSet.GetFieldValue(_T("PROD_STOCK_AMOUNT"), strSTOCKAMOUNT);

		CString str;
		str.Format(L"%s \t %s \t %s \t %s \t %s", strCODE, strNAME, strMAKER, strPRICE, strSTOCKAMOUNT);
		m_neworderList.AddString(str);

		//다음 레코드로 이동
		recSet.MoveNext();
	}

	recSet.Close();

}

void NewOrder::OnBnClickedOrder()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
