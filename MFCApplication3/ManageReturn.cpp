// ManageReturn.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MFCApplication3.h"
#include "ContentView.h"
#include "ManageReturn.h"
#include "afxdialogex.h"


// CManageReturn 대화 상자입니다.

IMPLEMENT_DYNAMIC(CManageReturn, CDialogEx)

CManageReturn::CManageReturn(CWnd * pParent, CString retId)
	: CDialogEx(IDD_RETURN, pParent)
{
	ret_id = retId;
	ret_type = ret_id.Right(1);
}

CManageReturn::~CManageReturn()
{
}

void CManageReturn::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RETURN, m_returnList);
//	DDX_Control(pDX, IDC_MOMMYCODE, m_retType);
	DDX_Control(pDX, IDC_LIST1, m_mommyCode);
	DDX_Control(pDX, IDC_TYPE, s_retType);
}


BEGIN_MESSAGE_MAP(CManageReturn, CDialogEx)
	ON_LBN_SELCHANGE(IDC_LIST1, &CManageReturn::OnLbnSelchangeList1)
	ON_BN_CLICKED(IDC_SAVE, &CManageReturn::OnBnClickedSave)
	ON_NOTIFY(LVN_ENDLABELEDIT, IDC_RETURN, &CManageReturn::OnEndlabeleditReturn)
END_MESSAGE_MAP()


// CManageReturn 메시지 처리기입니다.


BOOL CManageReturn::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	TRY
	{
		db_ret.OpenEx(_T("DSN=UOS25;UID=UOS25;PWD=0000"));
	}
		CATCH(CException, e)
	{
		TCHAR errMSG[255];

		e->GetErrorMessage(errMSG, 255);
		AfxMessageBox(errMSG, MB_ICONERROR);


		if (db_ret.IsOpen())
			db_ret.Close();

	}
	END_CATCH

	ShowData(db_ret);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CManageReturn::ShowData(CDatabase & db_ret)
{
	CRecordset recSet(&db_ret);
	CString strSQL, strNAME, strPRICE, strREMAIN, strCODE, strMAKER, strRETAMT, strDETAIL, mommyCode;

	// Modify list style
	m_returnList.ModifyStyle(0, LVS_REPORT | LVS_SHOWSELALWAYS | LVS_EDITLABELS, 0);
	m_returnList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	// 쿼리문을 통해 특정 날짜의 반품목록만 받아온다
	// Get product code, product name, product maker, product price, order amount, product stock amount, event detail
	strSQL.Format(L"SELECT PRODUCT.PROD_CODE, PROD_NAME, PROD_MAKER, PROD_PRICE, PROD_STOCK_AMOUNT, RETURN_AMOUNT, RETURN_HIGH_CODE, DETAIL FROM product INNER JOIN RETURN ON RETURN.RETURN_CODE = '%s' AND RETURN.PROD_CODE = PRODUCT.PROD_CODE;", ret_id);
	recSet.Open(CRecordset::dynaset, strSQL);

	// Create Column
	m_returnList.InsertColumn(0, L"반품 사유 (반품 사유를 적어주세요)", LVCFMT_CENTER, 380);
	m_returnList.InsertColumn(1, L"상품명", LVCFMT_CENTER, 100);
	m_returnList.InsertColumn(2, L"제조사", LVCFMT_CENTER, 90);
	m_returnList.InsertColumn(3, L"상품 가격", LVCFMT_CENTER, 80);
	m_returnList.InsertColumn(4, L"반품 수량", LVCFMT_CENTER, 80);
	m_returnList.InsertColumn(5, L"재고 수량", LVCFMT_CENTER, 80);

	recSet.GetFieldValue(_T("RETURN_HIGH_CODE"), mommyCode);
	m_mommyCode.AddString(mommyCode);

	if (ret_type == '0')
	{
		s_retType.SetWindowTextW(L"반품 타입 : 주문 반품");
	}
	else s_retType.SetWindowTextW(L"반품 타입 : 판매 반품");
	

	// 받아온 테이블에 남은 데이터가 없을 때까지 실행
	while (!recSet.IsEOF())
	{
		int idx = 0;

		recSet.GetFieldValue(_T("PROD_CODE"), strCODE);
		recSet.GetFieldValue(_T("PROD_NAME"), strNAME);
		recSet.GetFieldValue(_T("PROD_MAKER"), strMAKER);
		recSet.GetFieldValue(_T("PROD_PRICE"), strPRICE);
		recSet.GetFieldValue(_T("RETURN_AMOUNT"), strRETAMT);
		recSet.GetFieldValue(_T("PROD_STOCK_AMOUNT"), strREMAIN);
		recSet.GetFieldValue(_T("DETAIL"), strDETAIL);

		// insert data into list
		int nListitm = m_returnList.InsertItem(0, strDETAIL, 0);
		m_returnList.SetItem(nListitm, 1, LVFIF_TEXT, strNAME, 0, 0, 0, NULL);
		m_returnList.SetItem(nListitm, 2, LVFIF_TEXT, strMAKER, 0, 0, 0, NULL);
		m_returnList.SetItem(nListitm, 3, LVFIF_TEXT, strPRICE, 0, 0, 0, NULL);
		m_returnList.SetItem(nListitm, 4, LVFIF_TEXT, strRETAMT, 0, 0, 0, NULL);
		m_returnList.SetItem(nListitm, 5, LVFIF_TEXT, strREMAIN, 0, 0, 0, NULL);

		//다음 레코드로 이동
		recSet.MoveNext();
	}

	recSet.Close();
}

void CManageReturn::OnLbnSelchangeList1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString code;
	m_mommyCode.GetText(0, code);

	if (ret_type == '0')
	{
		// 주문 반품의 경우 원래 주문 내역 다이얼로그를 연다
		// Open dialog using currently selected index num
		dlg_manage_order = new COrderConfirm(this, code);
		dlg_manage_order->Create(COrderConfirm::IDD);
		dlg_manage_order->ShowWindow(SW_SHOW);
	}
	else
	{

	}

	return;
}


void CManageReturn::OnBnClickedSave()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// 반품 사유 데이터베이스에 저장
	int nItm = 0;
	nItm = m_returnList.GetItemCount();

	for (int i = 0; i < nItm; i++)
	{
		CString SQL;
		SQL.Format(L"UPDATE RETURN SET DETAIL = '%s' WHERE RETURN_CODE = '%s'", m_returnList.GetItemText(i, 0), ret_id);
		//MessageBox(SQL);
		db_ret.ExecuteSQL(SQL);
	}
	MessageBox(L"반품 사유 저장 완료");
	OnOK();
}


void CManageReturn::OnEndlabeleditReturn(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>( pNMHDR );
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = TRUE;
}
