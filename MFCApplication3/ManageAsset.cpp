// ManageAsset.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "MFCApplication3.h"
#include "ManageAsset.h"
#include "afxdialogex.h"


// ManageAsset ��ȭ �����Դϴ�.

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


// ManageAsset �޽��� ó�����Դϴ�.


BOOL ManageAsset::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	high_combo.AddString(L"����");
	high_combo.AddString(L"����");

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

//List Layout ���� : ��ȸ�ϴ� ������ ���� ����
//�Һз��� ������ ������ ����Ʈ ���̾ƿ� ��ȸ
void ManageAsset::ModifyStyle()
{
	while (asset_list.GetHeaderCtrl()->GetItemCount() > 0)
	{
		asset_list.DeleteColumn(0);
	}
	asset_list.ModifyStyle(0, LVS_REPORT | LVS_SHOWSELALWAYS | LVS_SINGLESEL | LVS_EDITLABELS, 0);
	asset_list.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	asset_list.InsertColumn(0, L"��¥ �� �ð�", LVCFMT_CENTER, 160);
	asset_list.InsertColumn(1, L"�ݾ�", LVCFMT_CENTER, 100);
	asset_list.InsertColumn(2, L"������", LVCFMT_CENTER, 200);
}
//��ǰ���� ���� ��ȸ�Ͽ��� ��
void ManageAsset::ModifyStyle2()
{
	while (asset_list.GetHeaderCtrl()->GetItemCount() > 0)
	{
		asset_list.DeleteColumn(0);
	}

	asset_list.ModifyStyle(0, LVS_REPORT | LVS_SHOWSELALWAYS | LVS_SINGLESEL | LVS_EDITLABELS, 0);
	asset_list.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	asset_list.InsertColumn(0, L"��¥", LVCFMT_CENTER, 200);
	asset_list.InsertColumn(1, L"��ǰ��", LVCFMT_CENTER, 200);
	asset_list.InsertColumn(2, L"�Ǹűݾ�", LVCFMT_CENTER, 200);
}

void ManageAsset::ModifyStyle3()
{
	while (asset_list.GetHeaderCtrl()->GetItemCount() > 0)
	{
		asset_list.DeleteColumn(0);
	}

	asset_list.ModifyStyle(0, LVS_REPORT | LVS_SHOWSELALWAYS | LVS_SINGLESEL | LVS_EDITLABELS, 0);
	asset_list.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	asset_list.InsertColumn(0, L"�ٹ����", LVCFMT_CENTER, 200);
	asset_list.InsertColumn(1, L"�����̸�", LVCFMT_CENTER, 200);
	asset_list.InsertColumn(2, L"�޿�", LVCFMT_CENTER, 200);
}



void ManageAsset::OnCbnSelchangeHighCombo()
{
	highIndex = high_combo.GetCurSel();

	if (highIndex == 0) {
		middle_combo.ResetContent();
		middle_combo.AddString(L"��ǰ �Ǹ�");
	}
	else {
		middle_combo.ResetContent();
		middle_combo.AddString(L"�ֹ�");
		middle_combo.AddString(L"�ΰǺ�");
		middle_combo.AddString(L"���� �� ������");
	}
}




void ManageAsset::OnCbnSelchangeMiddleCombo()
{
	middleIndex = middle_combo.GetCurSel();

	//highIndex = 0 : ��ǰ , 1 : �ֹ�/�ΰǺ�/����������
	if (highIndex == 0) {

		if (middleIndex == 0) {

			//DB���� ��ǰ�� �ڵ� ������
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
			low_combo.AddString(L"������� ����");
		}
	}
	else {
		if (middleIndex == 0) {

			//DB���� ��ǰ�� �ڵ� ������
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
				//���� - ��ǰ - �뼱��
				ModifyStyle();
				NumNumNinquery(L"F01");
				return;
			}
			else {
				//���� - ��ǰ - Ư�� ��ǰ�� ����
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
			//����
		case -1:
		{
			ModifyStyle();
			OneNNinquery();
			return;
		}
		case 0:
		{
			//���� - �ֹ�
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
			//���� - �ΰǺ�
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
			//���� - ����������
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






//��ȸ ��ư�� ���� ��ȸ �޼ҵ� ���ȭ
//�޼ҵ� �̸� �ǹ� : #1#2#3inquery
//#1 : ��з� , #2 : �ߺз� , #3 : �Һз�
//�ش� ��ȣ�� ���������� ��� N���� ǥ��
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

//2�� �з����� ������ ��� ��ȸ�� ���� �޼ҵ�(F01~F05)
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

//�Ǹ� �������� ��ǰ���� ���ؼ� �˻�
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

//���� - ǰ�� - Ư�� ��ǰ��
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

// ���� - �ΰǺ� ���� (��� ���� �ӱ� ��ȸ)
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

//���� - �ΰǺ� - ���� �� ����
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







//��ȸ ���� �ʱ�ȭ ��ư
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
