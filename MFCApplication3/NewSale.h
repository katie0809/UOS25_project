#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include <vector>
#include <string>
using namespace std;



// ��ٱ��Ͽ� �ӽ������� ��� �������� �����ϴ� ����ü
typedef struct basket_item {
	CString product_num;
	CString product_name;
	CString product_maker;
	CString product_price;
	CString product_discounted_price;
	CString total_price;
	int product_number;
	CString event_num;
}BasketItem;




class NewSale : public CDialogEx
{
	DECLARE_DYNAMIC(NewSale)


	// ��ü��
public:
	NewSale(CWnd* pParent = NULL);
	virtual ~NewSale();
	CDatabase db_newsale;


	//PRODUCT_LIST�� BASKET_LIST�� �ν��Ͻ�
	CListCtrl product_list;
	CListCtrl basket_list;

	//PRODUCT_LIST�� BASKET_LIST���� ���õ� �������� �ε���
	int selectedItemIdx;
	int RemovedItemIdx;


	//��ٱ��� Container
	vector<BasketItem> Basket;


	//���з��ڵ带 ��Ÿ�� �ν��Ͻ���
	CComboBox sex_combo;
	CComboBox age_combo;
	int sexIndex;
	int ageIndex;


	//�������̽� �ؽ�Ʈ �ν��Ͻ���
	CFont font1;
	CStatic text1;
	enum { IDD = IDD_NEW_SALE };

	//ȸ�� ���ϸ��� ��ȸ �� ����/���
	CString member_id;
	CString mileage;
	int total_mileage;


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.
	DECLARE_MESSAGE_MAP()



	//�޼ҵ��
public:
	//�ʱ�ȭ �޼ҵ�
	virtual BOOL OnInitDialog();
	void ShowData(CDatabase& db_newsale);
	void ModifyStyle();


	//PRODUCT_LIST�� BASKET_LIST�� ���ڵ� �̵��� ���� �̺�Ʈ ó�� �޼ҵ�
	afx_msg void OnBnClickedPushButton();
	afx_msg void OnLvnItemchangedProductList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnItemchangedBasket(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedPopButton();
	void UpdateBasket();


	//�Ǹ� ���� �� ��� �̺�Ʈ ó�� �޼ҵ�
	afx_msg void OnBnClickedOkButton();
	afx_msg void OnBnClickedCancleButton();
	CStatic price_text;
	void CreateSaleRecords(int * funds_total_price, CString SALE_NUM, int customerIdx);

	//PROD_CODE�� �Ű����ڷ� �����Ͽ�, �̺�Ʈ ���θ� ��ȯ���ִ� �޼ҵ�
	CString ReturnEventCode(CString PROD_CODE);
	void SetSaleTotalPrice(BasketItem* basket_item);
	CStatic event_text;
	CEdit member_edit;
	CButton mem_inquery_btn;
	CButton check_mileage_btn;
	afx_msg void OnBnClickedButton2();
};
