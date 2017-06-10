// SaleDetail.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MFCApplication3.h"
#include "SaleDetail.h"
#include "afxdialogex.h"
#include "ContentView.h"


// SaleDetail 대화 상자입니다.

IMPLEMENT_DYNAMIC(SaleDetail, CDialogEx)

SaleDetail::SaleDetail(CWnd* pParent, CString sale_num)

	: CDialogEx(IDD_SALE_DETAIL, pParent)
{
	SaleDetail::sale_num = sale_num;
}

SaleDetail::~SaleDetail()
{
}

void SaleDetail::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, saledetail_list);
	DDX_Control(pDX, IDCANCEL, close_btn);
	DDX_Control(pDX, IDOK, return_btn);
}


BEGIN_MESSAGE_MAP(SaleDetail, CDialogEx)
	ON_BN_CLICKED(IDOK, &SaleDetail::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &SaleDetail::OnBnClickedCancel)
END_MESSAGE_MAP()


// SaleDetail 메시지 처리기입니다.


BOOL SaleDetail::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	return_flag = 1;
	TRY
	{
		db_saledetail.OpenEx(_T("DSN=UOS25;UID=UOS25;PWD=0000"));
	}
		CATCH(CException, e)
	{
		TCHAR errMSG[255];

		e->GetErrorMessage(errMSG, 255);
		AfxMessageBox(errMSG, MB_ICONERROR);


		if (db_saledetail.IsOpen())
			db_saledetail.Close();

	}
	END_CATCH

		ModifyStyle();
	ShowData(db_saledetail);

	return TRUE;
}


void SaleDetail::ShowData(CDatabase & db_saledetail)
{
	CRecordset recSet(&db_saledetail);
	CString strSQL;
	CString product_name, sale_amount, product_total_price, discount_price, total_price;
	CString prod_code;

	strSQL.Format(L"select PROD_NAME, SALE_AMOUNT, SALE_PRICE, DISCOUNT_PRICE, SUM_PRICE, RETURN_SALE_NUM from SALE, PRODUCT where SALE_NUM = '%s' and SALE.PROD_NUM = PRODUCT.PROD_NUM", SaleDetail::sale_num);
	recSet.Open(CRecordset::dynaset, strSQL);

	while (!recSet.IsEOF())
	{
		int idx = 0;

		recSet.GetFieldValue(_T("PROD_NAME"), product_name);
		recSet.GetFieldValue(_T("SALE_AMOUNT"), sale_amount);
		recSet.GetFieldValue(_T("SALE_PRICE"), product_total_price);
		recSet.GetFieldValue(_T("DISCOUNT_PRICE"), discount_price);
		recSet.GetFieldValue(_T("SUM_PRICE"), total_price);
		recSet.GetFieldValue(_T("RETURN_SALE_NUM"), return_sale_num);

		if (return_sale_num.Compare(L"") != 0) {
			return_flag = 0;
			MessageBox(L"현재 상품은 이미 반품되어 있습니다.\n 반품 버튼을 누를 경우 반품이 되지 않습니다.");
		}

		// insert data into list
		int nListitm = saledetail_list.InsertItem(0, product_name, 0);
		saledetail_list.SetItem(nListitm, 1, LVFIF_TEXT, sale_amount, 0, 0, 0, NULL);
		saledetail_list.SetItem(nListitm, 2, LVFIF_TEXT, product_total_price, 0, 0, 0, NULL);
		saledetail_list.SetItem(nListitm, 3, LVFIF_TEXT, discount_price, 0, 0, 0, NULL);
		saledetail_list.SetItem(nListitm, 4, LVFIF_TEXT, total_price, 0, 0, 0, NULL);
		saledetail_list.SetItem(nListitm, 5, LVFIF_TEXT, prod_code, 0, 0, 0, NULL);

		//다음 레코드로 이동
		recSet.MoveNext();
	}

	recSet.Close();
}

void SaleDetail::ModifyStyle()
{
	// 상품 리스트 속성 초기화
	saledetail_list.ModifyStyle(0, LVS_REPORT | LVS_SHOWSELALWAYS | LVS_SINGLESEL | LVS_EDITLABELS, 0);
	saledetail_list.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	saledetail_list.InsertColumn(0, L"상품명", LVCFMT_CENTER, 160);
	saledetail_list.InsertColumn(1, L"판매 개수", LVCFMT_CENTER, 80);
	saledetail_list.InsertColumn(2, L"상품 금액", LVCFMT_CENTER, 100);
	saledetail_list.InsertColumn(3, L"할인 금액", LVCFMT_CENTER, 100);
	saledetail_list.InsertColumn(4, L"총 금액", LVCFMT_CENTER, 100);
	saledetail_list.InsertColumn(5, L"상품 코드", LVCFMT_CENTER, 100);
}


// click return button
void SaleDetail::OnBnClickedOk()
{
	if (return_flag == 0) {
		MessageBox(L"이미 반품이 된 상품입니다.\n조회만 가능합니다.");
	}
	else {
		CRecordset recSet(&db_saledetail);
		CString strSQL;


		int numOfItems = saledetail_list.GetItemCount(); // total return items count

														 // set sale_num 
		CString sale_num2;
		SetSaleNum(&sale_num2);

		// each database instance
		CRecordset recSet3(&db_saledetail);

		CString return_code;
		SetReturnCode(&return_code);

		for (int i = 0; i < numOfItems; i++) {
			CString prod_name, prod_code, sale_amount, sale_price, discount_price, sum_price;
			prod_name = saledetail_list.GetItemText(i, 0);
			sale_amount = saledetail_list.GetItemText(i, 1);
			sale_price = saledetail_list.GetItemText(i, 2);
			discount_price = saledetail_list.GetItemText(i, 3);
			sum_price = saledetail_list.GetItemText(i, 4);

			if (_ttoi(sum_price) < 0) {
				MessageBox(L"반품된 상품 정보 조회입니다. 반품 버튼이 비활성화됩니다.");
				return;
			}

			//Prod_code를 찾는다.
			strSQL.Format(L"select PROD_NUM from PRODUCT where PROD_NAME = '%s'", prod_name);
			recSet.Open(CRecordset::dynaset, strSQL);
			recSet.GetFieldValue(_T("PROD_NUM"), prod_code);
			recSet.Close();

			/*반품 레코드에 추가*/
			CString InsertReturnRecordSQL;


			InsertReturnRecordSQL.Format(L"insert into RETURN(RETURN_NUM, PROD_NUM, RETURN_AMOUNT, DETAIL, RETURN_HIGH_NUM) values ('%s', '%s', %s, '판매 반품입니다. 사유를 수정해주세요.', '%s')", return_code, prod_code, sale_amount, SaleDetail::sale_num);
			db_saledetail.ExecuteSQL(InsertReturnRecordSQL);
			/*반품 레코드 추가 끝*/


			/*판매 테이블에 -금액으로 추가하면서 기존 레코드의 반품 상위번호 대입*/
			CString InsertSaleReturnRecordSQL;
			InsertSaleReturnRecordSQL.Format(L"insert into SALE(SALE_NUM, PROD_NUM, SALE_AMOUNT, SALE_PRICE, DISCOUNT_PRICE, SUM_PRICE, CUSTOM_SORT_CODE) values ('%s', '%s', '-%s', '-%s', '-%s', '-%s', 'C09')", sale_num2, prod_code, sale_amount, sale_price, discount_price, sum_price);
			db_saledetail.ExecuteSQL(InsertSaleReturnRecordSQL);

			CString UpdateParentSaleRecordSQL;
			UpdateParentSaleRecordSQL.Format(L"update SALE set RETURN_SALE_NUM = '%s' where SALE_NUM = '%s' and PROD_NUM = '%s'", sale_num2, SaleDetail::sale_num, prod_code);
			db_saledetail.ExecuteSQL(UpdateParentSaleRecordSQL);
		}
		MessageBox(L"판매 반품이 완료되었습니다.");
		CDialogEx::OnCancel();
	}
}




void SaleDetail::SetReturnCode(CString * return_code)
{
	CString today;


	// 반품 테이블의 반품코드의 맨 끝자리가 1로 끝나는 애중 가장 최신것을 찾음
	CRecordset recSet(&db_saledetail);
	CString strSQL;
	CString return_code2;
	CString final_return_code;

	strSQL.Format(L"select RETURN_NUM from RETURN order by RETURN_NUM");
	recSet.Open(CRecordset::dynaset, strSQL);

	while (!recSet.IsEOF())
	{
		recSet.GetFieldValue(_T("RETURN_NUM"), return_code2);
		CString tempString = return_code2.Right(1);
		if (tempString.Compare(L"1") == 0) {
			final_return_code = return_code2;
		}
		recSet.MoveNext();
	}

	CTime ctime = CTime::GetCurrentTime();

	today.Format(L"%04d%02d%02d", ctime.GetYear(), ctime.GetMonth(), ctime.GetDay());
	CString temp = final_return_code.Left(8);



	if (today.Compare(temp) == 0) {
		CString code_num = final_return_code.Mid(9, 5);
		int temp2 = _ttoi(code_num);
		temp2++;
		code_num.Format(_T("%d"), temp2);
		temp = temp + L"0" + code_num + L"1";
	}
	else {
		int temp2 = _ttoi(today);
		temp.Format(_T("%d"), temp2);
		temp = temp + L"0100011";
	}

	*return_code = temp;
}


// After clicking return button, firstly, set sail_num for inserting record to sale table
void SaleDetail::SetSaleNum(CString * sale_num)
{
	CRecordset recSet(&db_saledetail);
	CString strSQL;
	CString SALE_NUM;

	strSQL.Format(L"select SALE_NUM from SALE");
	recSet.Open(CRecordset::dynaset, strSQL);

	while (!recSet.IsEOF())
	{
		recSet.GetFieldValue(_T("SALE_NUM"), SALE_NUM);
		recSet.MoveNext();
	}

	CString temp = SALE_NUM.Left(8);


	CString today;
	CTime cTime = CTime::GetCurrentTime();
	today.Format(L"%04d%02d%02d", cTime.GetYear(), cTime.GetMonth(), cTime.GetDay());



	if (temp.Compare(today) == 0) {
		CString temp2 = SALE_NUM.Right(6);
		int temp3 = _ttoi(temp2);
		temp3++;
		temp2.Format(_T("%d"), temp3);
		SALE_NUM = temp;
		*sale_num = SALE_NUM + "0" + temp2;
	}
	else {
		today += "0100001";
		*sale_num = today;
	}
}



// Cancle Button
void SaleDetail::OnBnClickedCancel()
{
	CDialogEx::OnCancel();
}