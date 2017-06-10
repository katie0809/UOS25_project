#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include <vector>
#include <string>
using namespace std;



// 장바구니에 임시적으로 담길 아이템을 정의하는 구조체
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


	// 객체들
public:
	NewSale(CWnd* pParent = NULL);
	virtual ~NewSale();
	CDatabase db_newsale;


	//PRODUCT_LIST와 BASKET_LIST의 인스턴스
	CListCtrl product_list;
	CListCtrl basket_list;

	//PRODUCT_LIST와 BASKET_LIST에서 선택된 아이템의 인덱스
	int selectedItemIdx;
	int RemovedItemIdx;


	//장바구니 Container
	vector<BasketItem> Basket;


	//고객분류코드를 나타낼 인스턴스들
	CComboBox sex_combo;
	CComboBox age_combo;
	int sexIndex;
	int ageIndex;


	//인터페이스 텍스트 인스턴스들
	CFont font1;
	CStatic text1;
	enum { IDD = IDD_NEW_SALE };

	//회원 마일리지 조회 및 적립/사용
	CString member_id;
	CString mileage;
	int total_mileage;


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()



	//메소드들
public:
	//초기화 메소드
	virtual BOOL OnInitDialog();
	void ShowData(CDatabase& db_newsale);
	void ModifyStyle();


	//PRODUCT_LIST와 BASKET_LIST의 레코드 이동을 위한 이벤트 처리 메소드
	afx_msg void OnBnClickedPushButton();
	afx_msg void OnLvnItemchangedProductList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnItemchangedBasket(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedPopButton();
	void UpdateBasket();


	//판매 결정 및 취소 이벤트 처리 메소드
	afx_msg void OnBnClickedOkButton();
	afx_msg void OnBnClickedCancleButton();
	CStatic price_text;
	void CreateSaleRecords(int * funds_total_price, CString SALE_NUM, int customerIdx);

	//PROD_CODE를 매개인자로 전달하여, 이벤트 여부를 반환해주는 메소드
	CString ReturnEventCode(CString PROD_CODE);
	void SetSaleTotalPrice(BasketItem* basket_item);
	CStatic event_text;
	CEdit member_edit;
	CButton mem_inquery_btn;
	CButton check_mileage_btn;
	afx_msg void OnBnClickedButton2();
};
