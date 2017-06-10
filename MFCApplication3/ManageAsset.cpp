// ManageAsset.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MFCApplication3.h"
#include "ManageAsset.h"
#include "afxdialogex.h"


// ManageAsset 대화 상자입니다.

IMPLEMENT_DYNAMIC(ManageAsset, CDialogEx)

ManageAsset::ManageAsset(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_ASSET_MANAGEMENT, pParent)
	, date_asc_radio(0)
	, date_desc_radio(0)
	, price_asc_radio(0)
	, price_desc_radio(0)
	, keyword_asc_radio(0)
	, keyword_desc_radio(0)
{

}

ManageAsset::~ManageAsset()
{
}

void ManageAsset::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_HIGH_COMBO, high_combo);
	DDX_Control(pDX, IDC_MIDDLE_COMBO, middle_combo);
	DDX_Control(pDX, IDC_LOW_COMBO, low_combo);
	DDX_Control(pDX, IDC_INQUERY_BTN, inquery_btn);
	DDX_Control(pDX, IDC_ASSET_LIST, asset_list);
	DDX_Control(pDX, IDC_BUTTON2, idc_btn2);
}


BEGIN_MESSAGE_MAP(ManageAsset, CDialogEx)
	ON_CBN_SELCHANGE(IDC_HIGH_COMBO, &ManageAsset::OnCbnSelchangeHighCombo)
	ON_CBN_SELCHANGE(IDC_MIDDLE_COMBO, &ManageAsset::OnCbnSelchangeMiddleCombo)
	ON_BN_CLICKED(IDC_INQUERY_BTN, &ManageAsset::OnBnClickedInqueryBtn)
	ON_CBN_SELCHANGE(IDC_LOW_COMBO, &ManageAsset::OnCbnSelchangeLowCombo)
	ON_BN_CLICKED(IDC_INITIALIZING_BTN, &ManageAsset::OnBnClickedInitializingBtn)
	ON_BN_CLICKED(IDC_COST_BTN, &ManageAsset::OnBnClickedCostBtn)
	ON_BN_CLICKED(IDC_WAGE_BTN, &ManageAsset::OnBnClickedWageBtn)
	ON_BN_CLICKED(IDC_BUTTON2, &ManageAsset::OnBnClickedButton2)
END_MESSAGE_MAP()


// ManageAsset 메시지 처리기입니다.


BOOL ManageAsset::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	high_combo.AddString(L"수입");
	high_combo.AddString(L"지출");

	TRY
	{
		db_asset.OpenEx(_T("DSN=UOS25;UID=UOS25;PWD=0000"));
	}
		CATCH(CException, e)
	{
		TCHAR errMSG[255];

		e->GetErrorMessage(errMSG, 255);
		AfxMessageBox(errMSG, MB_ICONERROR);


		if (db_asset.IsOpen())
			db_asset.Close();

	}
	END_CATCH

		highIndex = -1;
	middleIndex = -1;
	lowIndex = -1;

	ModifyStyle();

	return TRUE;
}

//List Layout 변경 : 조회하는 목적에 따라 변경
//소분류를 제외한 나머지 리스트 레이아웃 조회
void ManageAsset::ModifyStyle()
{
	while (asset_list.GetHeaderCtrl()->GetItemCount() > 0)
	{
		asset_list.DeleteColumn(0);
	}
	asset_list.ModifyStyle(0, LVS_REPORT | LVS_SHOWSELALWAYS | LVS_SINGLESEL | LVS_EDITLABELS, 0);
	asset_list.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	asset_list.InsertColumn(0, L"날짜 및 시간", LVCFMT_CENTER, 160);
	asset_list.InsertColumn(1, L"금액", LVCFMT_CENTER, 100);
	asset_list.InsertColumn(2, L"비고사항", LVCFMT_CENTER, 200);
}
//물품군에 따라 조회하였을 때
void ManageAsset::ModifyStyle2()
{
	while (asset_list.GetHeaderCtrl()->GetItemCount() > 0)
	{
		asset_list.DeleteColumn(0);
	}

	asset_list.ModifyStyle(0, LVS_REPORT | LVS_SHOWSELALWAYS | LVS_SINGLESEL | LVS_EDITLABELS, 0);
	asset_list.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	asset_list.InsertColumn(0, L"날짜", LVCFMT_CENTER, 200);
	asset_list.InsertColumn(1, L"제품명", LVCFMT_CENTER, 200);
	asset_list.InsertColumn(2, L"판매금액", LVCFMT_CENTER, 200);
}

void ManageAsset::ModifyStyle3()
{
	while (asset_list.GetHeaderCtrl()->GetItemCount() > 0)
	{
		asset_list.DeleteColumn(0);
	}

	asset_list.ModifyStyle(0, LVS_REPORT | LVS_SHOWSELALWAYS | LVS_SINGLESEL | LVS_EDITLABELS, 0);
	asset_list.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	asset_list.InsertColumn(0, L"근무년월", LVCFMT_CENTER, 200);
	asset_list.InsertColumn(1, L"직원이름", LVCFMT_CENTER, 200);
	asset_list.InsertColumn(2, L"급여", LVCFMT_CENTER, 200);
}



void ManageAsset::OnCbnSelchangeHighCombo()
{
	highIndex = high_combo.GetCurSel();

	if (highIndex == 0) {
		middle_combo.ResetContent();
		middle_combo.AddString(L"물품 판매");
	}
	else {
		middle_combo.ResetContent();
		middle_combo.AddString(L"주문");
		middle_combo.AddString(L"인건비");
		middle_combo.AddString(L"유지 및 관리비");
	}
}




void ManageAsset::OnCbnSelchangeMiddleCombo()
{
	middleIndex = middle_combo.GetCurSel();

	//highIndex = 0 : 물품 , 1 : 주문/인건비/유지관리비
	if (highIndex == 0) {

		if (middleIndex == 0) {

			//DB에서 제품군 코드 가져옴
			CRecordset recSet(&db_asset);
			CString cate_name;
			CString sql_str = ( L"select SORT_DETAIL from SORT_CODE where HIGH_SORT_CODE = 1" );
			recSet.Open(CRecordset::dynaset, sql_str);

			low_combo.ResetContent();
			ItemCategory.clear();

			while (!recSet.IsEOF()) {
				recSet.GetFieldValue(_T("SORT_DETAIL"), cate_name);
				low_combo.AddString(cate_name);
				ItemCategory.push_back(cate_name);
				recSet.MoveNext();
			}
		}
		else {
			low_combo.ResetContent();
			low_combo.AddString(L"하위목록 없음");
		}
	}
	else {
		if (middleIndex == 0) {

			//DB에서 제품군 코드 가져옴
			CRecordset recSet(&db_asset);
			CString cate_name;
			CString sql_str = ( L"select SORT_DETAIL from SORT_CODE where HIGH_SORT_CODE = 1" );
			recSet.Open(CRecordset::dynaset, sql_str);

			low_combo.ResetContent();
			ItemCategory.clear();

			while (!recSet.IsEOF()) {
				recSet.GetFieldValue(_T("SORT_DETAIL"), cate_name);
				low_combo.AddString(cate_name);
				ItemCategory.push_back(cate_name);
				recSet.MoveNext();
			}
		}
		else if (middleIndex == 1) {
			CRecordset recSet(&db_asset);
			CString cate_name;
			CString sql_str = ( L"select NAME from EMPLOYEE" );
			recSet.Open(CRecordset::dynaset, sql_str);

			low_combo.ResetContent();

			while (!recSet.IsEOF()) {
				recSet.GetFieldValue(_T("NAME"), cate_name);
				low_combo.AddString(cate_name);
				recSet.MoveNext();
			}
		}
		else {
			low_combo.ResetContent();
		}
	}
}

void ManageAsset::OnCbnSelchangeLowCombo()
{
	lowIndex = low_combo.GetCurSel();
}


void ManageAsset::OnBnClickedInqueryBtn()
{
	switch (highIndex)
	{
	case -1:
	{
		ModifyStyle();
		NNNinquery();
		return;
	}
	case 0:
	{
		switch (middleIndex)
		{
		case -1:
		{
			ModifyStyle();
			ZeroNNinquery();
			return;
		}
		case 0:
		{
			if (lowIndex == -1) {
				//수입 - 물품 - 노선택
				ModifyStyle();
				NumNumNinquery(L"F01");
				return;
			}
			else {
				//수입 - 물품 - 특정 제품군 선택
				ModifyStyle2();
				ZeroZeroNuminquery();
				return;
			}
		}
		}
	}
	case 1:
	{
		switch (middleIndex)
		{
			//지출
		case -1:
		{
			ModifyStyle();
			OneNNinquery();
			return;
		}
		case 0:
		{
			//지출 - 주문
			if (lowIndex == -1) {
				ModifyStyle();
				NumNumNinquery(L"F05");
				return;
			}
			else {
				ModifyStyle();
				OneZeroNuminquery();
				return;
			}
		}
		case 1:
		{
			//지출 - 인건비
			ModifyStyle3();
			if (lowIndex == -1) {
				OneOneNinquery();
				return;
			}
			else {
				OneOneNuminquery();
				return;
			}
		}
		case 2:
		{
			//지출 - 유지관리비
			if (lowIndex == -1) {
				ModifyStyle();
				NumNumNinquery(L"F03");
				return;
			}
			else {
			}
		}
		}
	}
	}
}






//조회 버튼에 의한 조회 메소드 모듈화
//메소드 이름 의미 : #1#2#3inquery
//#1 : 대분류 , #2 : 중분류 , #3 : 소분류
//해당 번호를 지정안했을 경우 N으로 표시
void ManageAsset::NNNinquery()
{
	CRecordset recSet(&db_asset);
	CString funds_date, funds_money, detail;
	CString sql_str = ( L"select FUNDS_DATE, PRICE, FUNDS_DETAIL_NUM from FUNDS" );
	recSet.Open(CRecordset::dynaset, sql_str);

	asset_list.DeleteAllItems();

	while (!recSet.IsEOF()) {
		recSet.GetFieldValue(_T("FUNDS_DATE"), funds_date);
		recSet.GetFieldValue(_T("PRICE"), funds_money);
		recSet.GetFieldValue(_T("FUNDS_DETAIL_NUM"), detail);

		// insert data into list
		int nListitm = asset_list.InsertItem(0, funds_date, 0);
		asset_list.SetItem(nListitm, 1, LVFIF_TEXT, funds_money, 0, 0, 0, NULL);
		asset_list.SetItem(nListitm, 2, LVFIF_TEXT, detail, 0, 0, 0, NULL);
		recSet.MoveNext();
	}
}

void ManageAsset::ZeroNNinquery()
{

	CRecordset recSet(&db_asset);
	CString funds_date, funds_money, detail;
	CString sql_str = ( L"select FUNDS_DATE, PRICE, FUNDS_DETAIL_NUM from FUNDS where FUNDS_SORT_CODE in ('F01')" );
	recSet.Open(CRecordset::dynaset, sql_str);

	asset_list.DeleteAllItems();

	while (!recSet.IsEOF()) {
		recSet.GetFieldValue(_T("FUNDS_DATE"), funds_date);
		recSet.GetFieldValue(_T("PRICE"), funds_money);
		recSet.GetFieldValue(_T("FUNDS_DETAIL_NUM"), detail);

		// insert data into list
		int nListitm = asset_list.InsertItem(0, funds_date, 0);
		asset_list.SetItem(nListitm, 1, LVFIF_TEXT, funds_money, 0, 0, 0, NULL);
		asset_list.SetItem(nListitm, 2, LVFIF_TEXT, detail, 0, 0, 0, NULL);
		recSet.MoveNext();
	}
}

void ManageAsset::OneNNinquery()
{
	CRecordset recSet(&db_asset);
	CString funds_date, funds_money, detail;
	CString sql_str = ( L"select FUNDS_DATE, PRICE, FUNDS_DETAIL_NUM from FUNDS where FUNDS_SORT_CODE in ('F02','F03','F04','F05')" );
	recSet.Open(CRecordset::dynaset, sql_str);
	asset_list.DeleteAllItems();

	while (!recSet.IsEOF()) {
		recSet.GetFieldValue(_T("FUNDS_DATE"), funds_date);
		recSet.GetFieldValue(_T("PRICE"), funds_money);
		recSet.GetFieldValue(_T("FUNDS_DETAIL_NUM"), detail);

		// insert data into list
		int nListitm = asset_list.InsertItem(0, funds_date, 0);
		asset_list.SetItem(nListitm, 1, LVFIF_TEXT, funds_money, 0, 0, 0, NULL);
		asset_list.SetItem(nListitm, 2, LVFIF_TEXT, detail, 0, 0, 0, NULL);
		recSet.MoveNext();
	}
}

//2차 분류까지 설정한 모든 조회에 대한 메소드(F01~F05)
void ManageAsset::NumNumNinquery(CString fCode)
{
	CRecordset recSet(&db_asset);
	CString funds_date, funds_money, detail;
	CString sql_str;
	sql_str.Format(L"select FUNDS_DATE, PRICE, FUNDS_DETAIL_NUM from FUNDS where FUNDS_SORT_CODE = '%s'", fCode);
	recSet.Open(CRecordset::dynaset, sql_str);
	asset_list.DeleteAllItems();

	while (!recSet.IsEOF()) {
		recSet.GetFieldValue(_T("FUNDS_DATE"), funds_date);
		recSet.GetFieldValue(_T("PRICE"), funds_money);
		recSet.GetFieldValue(_T("FUNDS_DETAIL_NUM"), detail);

		// insert data into list
		int nListitm = asset_list.InsertItem(0, funds_date, 0);
		asset_list.SetItem(nListitm, 1, LVFIF_TEXT, funds_money, 0, 0, 0, NULL);
		asset_list.SetItem(nListitm, 2, LVFIF_TEXT, detail, 0, 0, 0, NULL);
		recSet.MoveNext();
	}
}

//판매 아이템을 물품군에 한해서 검색
void ManageAsset::ZeroZeroNuminquery()
{
	CString prod_catg_code = L"P0";

	lowIndex++;
	CString temp2;
	temp2.Format(_T("%d"), lowIndex);

	prod_catg_code = prod_catg_code + temp2;

	CRecordset recSet(&db_asset);
	CString sale_code, prod_name, sale_price;
	CString sql_str;
	sql_str.Format(L"select SALE_NUM, PROD_NAME, SUM_PRICE from PRODUCT, SALE where SALE.PROD_NUM = PRODUCT.PROD_NUM and PRODUCT.CATG_SORT_CODE = '%s'", prod_catg_code);
	recSet.Open(CRecordset::dynaset, sql_str);
	asset_list.DeleteAllItems();

	while (!recSet.IsEOF()) {
		recSet.GetFieldValue(_T("SALE_NUM"), sale_code);
		recSet.GetFieldValue(_T("PROD_NAME"), prod_name);
		recSet.GetFieldValue(_T("SUM_PRICE"), sale_price);

		CString temp = sale_code.Left(8);
		// insert data into list
		int nListitm = asset_list.InsertItem(0, temp, 0);
		asset_list.SetItem(nListitm, 1, LVFIF_TEXT, prod_name, 0, 0, 0, NULL);
		asset_list.SetItem(nListitm, 2, LVFIF_TEXT, sale_price, 0, 0, 0, NULL);
		recSet.MoveNext();
	}
}

//지출 - 품목 - 특정 상품군
void ManageAsset::OneZeroNuminquery()
{
	CString prod_catg_code = L"P0";

	lowIndex++;
	CString temp2;
	temp2.Format(_T("%d"), lowIndex);

	prod_catg_code = prod_catg_code + temp2;

	CRecordset recSet(&db_asset);
	CString sql_str;
	sql_str.Format(L"select ORDER_NUM, PROD_NAME, ORDER_AMOUNT, PROD_PRICE from PRODUCT, ORDER_LIST where ORDER_LIST.PROD_NUM = PRODUCT.PROD_NUM and PRODUCT.CATG_SORT_CODE = '%s'", prod_catg_code);
	recSet.Open(CRecordset::dynaset, sql_str);
	asset_list.DeleteAllItems();

	while (!recSet.IsEOF()) {
		CString order_code, prod_name, order_amount, prod_price;
		recSet.GetFieldValue(_T("ORDER_NUM"), order_code);
		recSet.GetFieldValue(_T("PROD_NAME"), prod_name);
		recSet.GetFieldValue(_T("ORDER_AMOUNT"), order_amount);
		recSet.GetFieldValue(_T("PROD_PRICE"), prod_price);

		int a = _ttoi(order_amount);
		int b = _ttoi(prod_price);

		a *= b;

		prod_price.Format(_T("-%d"), a);

		CString temp = order_code.Left(8);
		// insert data into list
		int nListitm = asset_list.InsertItem(0, temp, 0);
		asset_list.SetItem(nListitm, 1, LVFIF_TEXT, prod_name, 0, 0, 0, NULL);
		asset_list.SetItem(nListitm, 2, LVFIF_TEXT, prod_price, 0, 0, 0, NULL);
		recSet.MoveNext();
	}
}

// 지출 - 인건비 선택 (모든 직원 임금 조회)
void ManageAsset::OneOneNinquery()
{
	CRecordset recSet(&db_asset);
	CString sql_str;
	sql_str.Format(L"select FUNDS_DATE, PRICE, FUNDS_DETAIL_NUM from FUNDS where FUNDS_SORT_CODE = 'F02'");
	recSet.Open(CRecordset::dynaset, sql_str);
	asset_list.DeleteAllItems();

	while (!recSet.IsEOF()) {
		CString funds_date, funds_money, detail;
		recSet.GetFieldValue(_T("FUNDS_DATE"), funds_date);
		recSet.GetFieldValue(_T("PRICE"), funds_money);
		recSet.GetFieldValue(_T("FUNDS_DETAIL_NUM"), detail);

		// insert data into list
		int nListitm = asset_list.InsertItem(0, funds_date, 0);
		asset_list.SetItem(nListitm, 1, LVFIF_TEXT, funds_money, 0, 0, 0, NULL);
		asset_list.SetItem(nListitm, 2, LVFIF_TEXT, detail, 0, 0, 0, NULL);
		recSet.MoveNext();
	}
}

//지출 - 인건비 - 직원 중 선택
void ManageAsset::OneOneNuminquery()
{
	CString name;
	int index = low_combo.GetCurSel();
	low_combo.GetLBText(index, name);

	CRecordset recSet(&db_asset);
	CString sql_str;
	sql_str.Format(L"select FUNDS_DATE, PRICE, FUNDS_DETAIL_NUM from FUNDS where FUNDS_SORT_CODE = 'F03' and FUNDS_DETAIL_NUM = '%s'", name);
	recSet.Open(CRecordset::dynaset, sql_str);
	asset_list.DeleteAllItems();

	while (!recSet.IsEOF()) {
		CString funds_date, funds_money, detail;
		recSet.GetFieldValue(_T("FUNDS_DATE"), funds_date);
		recSet.GetFieldValue(_T("PRICE"), funds_money);
		recSet.GetFieldValue(_T("FUNDS_DETAIL_NUM"), detail);

		// insert data into list
		int nListitm = asset_list.InsertItem(0, funds_date, 0);
		asset_list.SetItem(nListitm, 1, LVFIF_TEXT, funds_money, 0, 0, 0, NULL);
		asset_list.SetItem(nListitm, 2, LVFIF_TEXT, detail, 0, 0, 0, NULL);
		recSet.MoveNext();
	}
}







//조회 설정 초기화 버튼
void ManageAsset::OnBnClickedInitializingBtn()
{
	highIndex = -1;
	middleIndex = -1;
	lowIndex = -1;

	high_combo.SetCurSel(-1);
	middle_combo.SetCurSel(-1);
	low_combo.SetCurSel(-1);
}


void ManageAsset::OnBnClickedCostBtn()
{
	newcost = new NewCost(this);
	newcost->Create(NewCost::IDD);
	newcost->ShowWindow(SW_SHOW);
}


void ManageAsset::OnBnClickedWageBtn()
{
	newwage = new ManageWage(this);
	newwage->Create(ManageWage::IDD);
	newwage->ShowWindow(SW_SHOW);
}


void ManageAsset::OnBnClickedButton2()
{
	CDialogEx::OnCancel();
}
