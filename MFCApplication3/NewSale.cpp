#include "stdafx.h"
#include "MFCApplication3.h"
#include "NewSale.h"
#include "afxdialogex.h"
#include "ContentView.h"


CString Customer[8] = { L"C01",L"C02",L"C03",L"C04",L"C05",L"C06",L"C07",L"C08" };


// NewSale ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(NewSale, CDialogEx)

NewSale::NewSale(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_NEW_SALE, pParent)
{

}

NewSale::~NewSale()
{
	Basket.clear();
}

void NewSale::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PRODUCT_LIST, product_list);
	DDX_Control(pDX, IDC_BASKET, basket_list);
	DDX_Control(pDX, IDC_COMBO_SEX, sex_combo);
	DDX_Control(pDX, IDC_COMBO2, age_combo);
	DDX_Control(pDX, IDC_TEXT1, text1);
	DDX_Control(pDX, IDC_TOTAL_PRICE_TEXT, price_text);
	DDX_Control(pDX, IDC_EVENT_TEXT, event_text);
	DDX_Control(pDX, IDC_EDIT3, member_edit);
	DDX_Control(pDX, IDC_BUTTON2, mem_inquery_btn);
	DDX_Control(pDX, IDC_BUTTON3, check_mileage_btn);
}


//�ʱ�ȭ Overriding �Լ� �� ����� DB �ν��Ͻ��� �����ͺ��̽� ���� �۾�
BOOL NewSale::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	total_mileage = 0;
	member_id = L"0";

	TRY
	{
		db_newsale.OpenEx(_T("DSN=UOS25;UID=UOS25;PWD=0000"));
	}
		CATCH(CException, e)
	{
		TCHAR errMSG[255];

		e->GetErrorMessage(errMSG, 255);
		AfxMessageBox(errMSG, MB_ICONERROR);


		if (db_newsale.IsOpen())
			db_newsale.Close();

	}
	END_CATCH

		ModifyStyle();
	ShowData(db_newsale);


	return TRUE;
}

void NewSale::ShowData(CDatabase& db_newsale)
{


	CRecordset recSet(&db_newsale);
	CString strSQL;
	CString product_name, product_maker, product_price, product_amount, product_num;

	strSQL.Format(L"select PROD_NAME, PROD_MAKER, PROD_SALE_PRICE, PROD_STOCK_AMOUNT, PROD_NUM from PRODUCT where PROD_STOCK_AMOUNT > 0 order by PROD_NAME");
	recSet.Open(CRecordset::dynaset, strSQL);

	while (!recSet.IsEOF())
	{
		int idx = 0;

		recSet.GetFieldValue(_T("PROD_NAME"), product_name);
		recSet.GetFieldValue(_T("PROD_MAKER"), product_maker);
		recSet.GetFieldValue(_T("PROD_SALE_PRICE"), product_price);
		recSet.GetFieldValue(_T("PROD_STOCK_AMOUNT"), product_amount);
		recSet.GetFieldValue(_T("PROD_NUM"), product_num);

		// insert data into list
		int nListitm = product_list.InsertItem(0, product_name, 0);
		product_list.SetItem(nListitm, 1, LVFIF_TEXT, product_maker, 0, 0, 0, NULL);
		product_list.SetItem(nListitm, 2, LVFIF_TEXT, product_price, 0, 0, 0, NULL);
		product_list.SetItem(nListitm, 3, LVFIF_TEXT, product_amount, 0, 0, 0, NULL);
		product_list.SetItem(nListitm, 4, LVFIF_TEXT, product_num, 0, 0, 0, NULL);

		//���� ���ڵ�� �̵�
		recSet.MoveNext();
	}

	recSet.Close();
}

//ȭ����� �� ��� ����Ʈ�� �Ӽ� �ʱ�ȭ
void NewSale::ModifyStyle()
{
	//MessageBox(L"ModifyStyle() execute", L"check",NULL);


	// ��ǰ ����Ʈ �Ӽ� �ʱ�ȭ
	product_list.ModifyStyle(0, LVS_REPORT | LVS_SHOWSELALWAYS | LVS_SINGLESEL | LVS_EDITLABELS, 0);
	product_list.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	product_list.InsertColumn(0, L"��ǰ��", LVCFMT_CENTER, 160);
	product_list.InsertColumn(1, L"������", LVCFMT_CENTER, 100);
	product_list.InsertColumn(2, L"����", LVCFMT_CENTER, 50);
	product_list.InsertColumn(3, L"���", LVCFMT_CENTER, 50);
	product_list.InsertColumn(4, L"��ǰ�ڵ�(�����)", LVCFMT_CENTER, 0);

	// ��ٱ��� ����Ʈ �Ӽ� �ʱ�ȭ
	basket_list.ModifyStyle(0, LVS_REPORT | LVS_SHOWSELALWAYS | LVS_SINGLESEL | LVS_EDITLABELS, 0);
	basket_list.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	basket_list.InsertColumn(0, L"����", LVCFMT_CENTER, 50);
	basket_list.InsertColumn(1, L"��ǰ��", LVCFMT_CENTER, 130);
	basket_list.InsertColumn(2, L"������", LVCFMT_CENTER, 130);
	basket_list.InsertColumn(3, L"��ǰ����", LVCFMT_CENTER, 150);
	basket_list.InsertColumn(4, L"���ξ�", LVCFMT_CENTER, 150);
	basket_list.InsertColumn(5, L"�ѱݾ�", LVCFMT_CENTER, 170);
}


BEGIN_MESSAGE_MAP(NewSale, CDialogEx)
	ON_BN_CLICKED(IDC_PUSH_BUTTON, &NewSale::OnBnClickedPushButton)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_PRODUCT_LIST, &NewSale::OnLvnItemchangedProductList)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_BASKET, &NewSale::OnLvnItemchangedBasket)
	ON_BN_CLICKED(IDC_POP_BUTTON, &NewSale::OnBnClickedPopButton)
	ON_BN_CLICKED(IDC_OK_BUTTON, &NewSale::OnBnClickedOkButton)
	ON_BN_CLICKED(IDC_CANCLE_BUTTON, &NewSale::OnBnClickedCancleButton)
	ON_BN_CLICKED(IDC_BUTTON2, &NewSale::OnBnClickedButton2)
END_MESSAGE_MAP()


CString NewSale::ReturnEventCode(CString PROD_NUM)
{
	CString event_num;

	CRecordset recSet(&db_newsale);
	CString strSQL; //SQL���� String���� ����

	strSQL.Format(L"select EVENT_NUM from PRODUCT where PROD_NUM = %s", PROD_NUM);
	recSet.Open(CRecordset::dynaset, strSQL);

	recSet.GetFieldValue(_T("EVENT_NUM"), event_num);



	if (event_num.Compare(L"") != 0) {

		CString event_detail;
		CRecordset recSet2(&db_newsale);
		CString strSQL2;

		strSQL2.Format(L"select DETAIL from EVENT where EVENT_NUM = '%s'", event_num);
		recSet2.Open(CRecordset::dynaset, strSQL2);

		recSet2.GetFieldValue(_T("DETAIL"), event_detail);
		event_text.SetWindowTextW(event_detail);
	}
	else {

		return L"null";
	}

	return event_num;
}

// apply events to the selected items 
void NewSale::SetSaleTotalPrice(BasketItem * basket_item)
{
	if (basket_item->event_num.Compare(L"null") == 0) {
		basket_item->product_discounted_price = L"0";
		int price = _ttoi(basket_item->product_price);
		price *= basket_item->product_number;
		basket_item->total_price.Format(_T("%d"), price);
		event_text.SetWindowTextW(L"�̺�Ʈ ����");
		return;
	}
	else {
		CRecordset recSet(&db_newsale);
		CString strSQL;

		strSQL.Format(L"select EVENT_NAME, DISCOUNT_AGREE, DETAIL from EVENT where EVENT_NUM = '%s'", basket_item->event_num);
		recSet.Open(CRecordset::dynaset, strSQL);

		CString event_name, discount_agree, detail;
		recSet.GetFieldValue(_T("EVENT_NAME"), event_name);
		recSet.GetFieldValue(_T("DISCOUNT_AGREE"), discount_agree);
		recSet.GetFieldValue(_T("DETAIL"), detail);

		if (discount_agree.Compare(L"Y") == 0) { // discount_agree is 1, apply discount
			int price = _ttoi(basket_item->product_price);
			int discount_percent = _ttoi(detail);
			price *= basket_item->product_number;

			int discount_price(0), total_price(0);
			discount_price = ( price * discount_percent ) / 100;
			total_price = price - discount_price;

			basket_item->product_discounted_price.Format(_T("%d"), discount_price);
			basket_item->total_price.Format(_T("%d"), total_price);
			event_text.SetWindowTextW(detail + "% ������ ����Ǵ� ��ǰ�Դϴ�.");
		}
		else if (discount_agree.Compare(L"N") == 0) { // etc events

													  // n + n Event
			if (detail.Find(L"+") != -1) {
				wchar_t first = detail.GetAt(0);
				wchar_t second = detail.GetAt(2);
				int divisor = _wtoi(&first) + _wtoi(&second);

				int price = _ttoi(basket_item->product_price);
				int share, remainder;
				share = basket_item->product_number / divisor;
				remainder = basket_item->product_number % divisor;

				int discount_price(0), total_price(0);
				discount_price = price * share;
				total_price = ( price * basket_item->product_number ) - discount_price;

				basket_item->product_discounted_price.Format(_T("%d"), discount_price);
				basket_item->total_price.Format(_T("%d"), total_price);
				event_text.SetWindowTextW(detail + " ����ǰ�Դϴ�. ������ �°� ���Ž� ������ ����˴ϴ�.");
			}
			else { // etc events
				int price = _ttoi(basket_item->product_price);
				price *= basket_item->product_number;
				basket_item->total_price.Format(_T("%d"), price);
				basket_item->product_discounted_price.Format(_T("%d"), 0);
				basket_item->total_price.Format(_T("%d"), price);
				MessageBox(detail);
				event_text.SetWindowTextW(detail);
			}
		}
	}
}

// ���� ��ǰ�� ��ٱ��Ϸ� �̵�
void NewSale::OnBnClickedPushButton()
{
	if (selectedItemIdx < 0) {
		return;
	}
	else {
		CString product_name, product_maker, product_price, product_amount, product_num, event_num;
		int i = 0;

		product_name = product_list.GetItemText(selectedItemIdx, 0);
		product_maker = product_list.GetItemText(selectedItemIdx, 1);
		product_price = product_list.GetItemText(selectedItemIdx, 2);
		product_amount = product_list.GetItemText(selectedItemIdx, 3);
		product_num = product_list.GetItemText(selectedItemIdx, 4);

		//��ٱ��Ϸ� ��� ��ǰ�� ���� �̺�Ʈ ����� �ڵ带 ����
		event_num = ReturnEventCode(product_num);


		int amount = _ttoi(product_amount);

		//���� �ٽ��Ͽ� �����ϴ� �������߿� ���� �������� �ִ� �� �˻�
		for (i = 0; i < Basket.size(); i++) {
			//���� �������� ���� ��� ������ ����
			if (product_name.Compare(Basket[i].product_name) == 0) {
				if (Basket[i].product_number + 1 > amount) {
					MessageBox(L"�ش� ��ǰ�� �� �̻��� ��� �����ϴ�.\n���Ӱ� �ֹ��Ͽ� �ֽʽÿ�.");
				}
				else {
					Basket[i].product_number++;
					SetSaleTotalPrice(&Basket[i]);
				}
				break;
			}
		}


		//�������� ���� �ٽ��Ͽ� ���� ��� ���Ӱ� �߰�
		if (i == Basket.size()) {
			BasketItem temp;
			temp.product_name = product_name;
			temp.product_maker = product_maker;
			temp.product_price = product_price;
			temp.product_number = 1;
			temp.product_num = product_num;
			temp.event_num = event_num;

			Basket.push_back(temp);

			SetSaleTotalPrice(&Basket[i]);

		}

		UpdateBasket();
	}
}



//PRODUCT_LIST ���õ� INDEX ���
void NewSale::OnLvnItemchangedProductList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>( pNMHDR );
	selectedItemIdx = pNMLV->iItem;
	*pResult = 0;
}


//BASKET_LIST ���õ� INDEX ���
void NewSale::OnLvnItemchangedBasket(NMHDR * pNMHDR, LRESULT * pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>( pNMHDR );
	RemovedItemIdx = pNMLV->iItem;
	*pResult = 0;
}



//��ٱ��Ͽ��� ���õ� ������ ����
void NewSale::OnBnClickedPopButton()
{
	if (RemovedItemIdx < 0) {
		return;
	}
	else {
		CString product_name;
		product_name = basket_list.GetItemText(RemovedItemIdx, 1);
		int i = 0;
		for (i = 0; i < Basket.size(); i++) {
			if (product_name.Compare(Basket[i].product_name) == 0) {
				if (Basket[i].product_number > 1) {
					Basket[i].product_number--;
				}
				else {
					Basket.erase(Basket.begin() + i);
				}
			}
		}
		UpdateBasket();
	}
}


//��ǰ �̵��� ��ٱ��� ������Ʈ
void NewSale::UpdateBasket()
{
	int tPrice = 0;

	basket_list.DeleteAllItems();

	//Basket�� �������� ó������ ���鼭 �̺�Ʈ ���� ���� Ȯ�� �� ����
	for (int i = 0; i < Basket.size(); i++) {
		int nListitm = basket_list.InsertItem(0, L"0", 0);

		CString numOfItem;
		numOfItem.Format(_T("%d"), Basket[i].product_number);
		basket_list.SetItem(nListitm, 0, LVFIF_TEXT, numOfItem, 0, 0, 0, NULL); //���� ����
		basket_list.SetItem(nListitm, 1, LVFIF_TEXT, Basket[i].product_name, 0, 0, 0, NULL); //��ǰ �̸�
		basket_list.SetItem(nListitm, 2, LVFIF_TEXT, Basket[i].product_maker, 0, 0, 0, NULL); //��ǰ ������

		tPrice += _ttoi(Basket[i].total_price);
		int prod_price = _ttoi(Basket[i].product_price);
		prod_price *= Basket[i].product_number;

		CString prodPrice;
		prodPrice.Format(_T("%d"), prod_price);
		basket_list.SetItem(nListitm, 3, LVFIF_TEXT, prodPrice, 0, 0, 0, NULL); //�� �Һ����������
		basket_list.SetItem(nListitm, 4, LVFIF_TEXT, Basket[i].product_discounted_price, 0, 0, 0, NULL); //���αݾ�
		basket_list.SetItem(nListitm, 5, LVFIF_TEXT, Basket[i].total_price, 0, 0, 0, NULL); //�� �ݾ�
	}
	CString temp;
	temp.Format(_T("%d"), tPrice);

	price_text.SetWindowTextW(temp);
}



//�Ǹ� ��� �̺�Ʈ
void NewSale::OnBnClickedOkButton()
{
	//�ڱ� ���� ���̺� ���� �Ǹŵ� �ѱݾ��� �����ϱ� ����
	int funds_total_price(0);

	if (Basket.empty() == true) {
		MessageBox(L"��ϵ� �������� �����ϴ�.\n�������� Ȯ���ϰ� �ٽ� �����ֽʽÿ�.");
		return;
	}


	//Customer �з� �ʵ� �ĺ� ����
	sexIndex = sex_combo.GetCurSel();
	ageIndex = age_combo.GetCurSel();

	if (sexIndex == -1 || ageIndex == -1) {
		MessageBox(L"���� �� ������ �������� �ʾҽ��ϴ�.\nȮ���ϰ� �ٽ� �����ֽʽÿ�.");
		return;
	}

	int customerIdx = ageIndex * 2 + sexIndex;



	// �Ǹ� ���̺��� ���� �ֽ� ���ڵ��� �Ǹ��ڵ带 ����
	CRecordset recSet(&db_newsale);
	CString strSQL; //SQL���� String���� ����
	CString SALE_NUM;

	strSQL.Format(L"select SALE_NUM from SALE");
	recSet.Open(CRecordset::dynaset, strSQL);

	while (!recSet.IsEOF())
	{
		recSet.GetFieldValue(_T("SALE_NUM"), SALE_NUM);
		recSet.MoveNext();
	}

	CString temp = SALE_NUM.Left(8);


	//���� ��¥ ���� �ҷ�����
	CString today;
	CTime cTime = CTime::GetCurrentTime();
	today.Format(L"%04d%02d%02d", cTime.GetYear(), cTime.GetMonth(), cTime.GetDay());



	//���� ��¥�� SALE_NUM�� ������ ���� ���� ��쿡�� �Ǹ� ��ȣ�� ������ ���ڵ带 �߰���
	//���� ��¥�� SALE_NUM�� �ٸ� ��� ��¥�� ������ ���ڵ�κ��� ����(1������)
	if (temp.Compare(today) == 0) {
		CString temp2 = SALE_NUM.Right(6);
		int temp3 = _ttoi(temp2);
		temp3++;
		temp2.Format(_T("%d"), temp3);
		SALE_NUM = temp;
		SALE_NUM = SALE_NUM + "0" + temp2;
		CreateSaleRecords(&funds_total_price, SALE_NUM, customerIdx);
	}
	else {
		today += "0100001";
		SALE_NUM = today;
		CreateSaleRecords(&funds_total_price, SALE_NUM, customerIdx);
	}

	if (member_id.Compare(L"0") != 0) {
		int temp = _ttoi(mileage);
		int temp2 = total_mileage;
		total_mileage += temp;

		mileage.Format(_T("%d"), total_mileage);

		strSQL.Format(L"update MEMBER set MILEAGE = '%s' where MEMBER_NUM = '%s'", mileage, member_id);
		db_newsale.ExecuteSQL(strSQL);


		CString mileageCheckText;
		mileageCheckText.Format(L"%d ���ϸ����� �����Ǿ�\n�� %d ���ϸ����� �Ǿ����ϴ�.", temp2, total_mileage);
		MessageBox(mileageCheckText);
	}

	//�Ǹ� �Ϸ��� �ڱݰ��� ���̺� ���� ��¥�ú���, �ڱݰ����ڵ�, �ѱݾ� ���ڵ� ����
	CRecordset recSet3(&db_newsale);
	CString InsertFundsRecordSQL;

	CString todayHHMMSS;
	CTime cTime2 = CTime::GetCurrentTime();
	todayHHMMSS.Format(L"%04d%02d%02d%02d%02d%02d", cTime2.GetYear(), cTime2.GetMonth(), cTime2.GetDay(), cTime2.GetHour(), cTime2.GetMinute(), cTime2.GetSecond());

	CString ftp;
	ftp.Format(_T("%d"), funds_total_price);
	InsertFundsRecordSQL.Format(L"insert into FUNDS(FUNDS_DATE, FUNDS_SORT_CODE, PRICE, FUNDS_DETAIL_NUM) values ('%s', 'F01', '%s', '%s');", todayHHMMSS, ftp, SALE_NUM);
	db_newsale.ExecuteSQL(InsertFundsRecordSQL);
	MessageBox(L"������ �������� �ǸŰ� �Ϸ�Ǿ����ϴ�.");

	db_newsale.Close();
	CDialogEx::OnOK();
}



void NewSale::CreateSaleRecords(int *funds_total_price, CString SALE_NUM, int customerIdx)
{
	CString sql_str;
	for (int i = 0; i < Basket.size(); i++) {



		CString product_num = Basket[i].product_num;
		CString product_name;
		CString product_maker;
		CString product_price = Basket[i].product_price;
		int product_number = Basket[i].product_number;
		int total_price = _ttoi(product_price);
		total_price = total_price*product_number;

		CString dsc_price = Basket[i].product_discounted_price;
		CString tot_price = Basket[i].total_price;
		*funds_total_price += _ttoi(Basket[i].total_price);

		// Mileage ������ ����
		if (Basket[i].event_num.Compare(L"C006") == 0) {
			int temp = _ttoi(tot_price);
			total_mileage += temp / 10;
		}


		sql_str.Format(L"insert into SALE(SALE_NUM, PROD_NUM, CUSTOM_SORT_CODE, SALE_AMOUNT, SALE_PRICE, DISCOUNT_PRICE, SUM_PRICE) values ('%s', '%s', '%s', '%d', '%d', '%s', '%s')", SALE_NUM, product_num, Customer[customerIdx], product_number, total_price, dsc_price, tot_price);
		db_newsale.ExecuteSQL(sql_str);

		/*��� ���̺��� �ش� ��ǰ ���� ����~*/
		CRecordset recSet2(&db_newsale);
		CString prod_amount;
		CString strSQL;
		strSQL.Format(L"select PROD_NUM, PROD_STOCK_AMOUNT from PRODUCT where PROD_NUM = %s", product_num);
		recSet2.Open(CRecordset::dynaset, strSQL);
		recSet2.GetFieldValue(_T("PROD_STOCK_AMOUNT"), prod_amount);


		//���� ����� ��
		int temp = _ttoi(prod_amount);
		temp -= product_number;


		//��ǰ���� �þ ����� ��
		prod_amount.Format(_T("%d"), temp);


		//��ǰ ���̺��� �ش� ��ǰ ���ڵ��� ��� ������Ʈ
		strSQL.Format(L"update PRODUCT set PROD_STOCK_AMOUNT = %s where PROD_NUM = %s", prod_amount, product_num);
		db_newsale.ExecuteSQL(strSQL);
		/*~��� �߰� ��*/
	}
}



//�Ǹ� ��� �޼ҵ�
void NewSale::OnBnClickedCancleButton()
{
	db_newsale.Close();
	CDialogEx::OnOK();
}




void NewSale::OnBnClickedButton2()
{
	GetDlgItemText(IDC_EDIT3, member_id);

	CRecordset recSet(&db_newsale);
	CString strSQL;
	CString name;

	strSQL.Format(L"select NAME, MILEAGE from MEMBER where MEMBER_NUM = '%s'", member_id);
	recSet.Open(CRecordset::dynaset, strSQL);

	if (recSet.GetRecordCount() == 0) {
		MessageBox(L"���Ե��� ���� ȸ���Դϴ�.\nȸ����ȣ�� Ȯ���Ͽ� �ֽʽÿ�.");
		return;
	}
	else {
		recSet.GetFieldValue(_T("MILEAGE"), mileage);
		recSet.GetFieldValue(_T("NAME"), name);
		CString temp;
		temp.Format(L"ȸ���� ���������� ��ȸ�Ǿ����ϴ�.\nȸ���̸� : %s\n������ : %s", name, mileage);
		MessageBox(temp);
	}
}


