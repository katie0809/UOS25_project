#pragma once
#include "NewCost.h"
#include "ManageWage.h"
#include "afxwin.h"
#include "afxcmn.h"
#include <vector>
#include <string>
using namespace std;


class ManageAsset : public CDialogEx
{
	DECLARE_DYNAMIC(ManageAsset)

public:
	ManageAsset(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~ManageAsset();
	enum { IDD = IDD_ASSET_MANAGEMENT };


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	//뷰의 인스턴스와 Linking
	virtual BOOL OnInitDialog();

	//공과금 추가 클래스
	NewCost * newcost;

	//임금 추가 클래스
	ManageWage * newwage;

	//데이터베이스 인스턴스
	CDatabase db_asset;


	//콤보박스 인스턴스, 각 콤보박스에 선택된 내용의 index를 저장할 인스턴스
	CComboBox high_combo;
	CComboBox middle_combo;
	CComboBox low_combo;
	int highIndex;
	int middleIndex;
	int lowIndex;

	CEdit date_from_edit;
	CEdit date_to_edit;
	CEdit keyword_edit;
	int date_asc_radio;
	int date_desc_radio;
	int price_asc_radio;
	int price_desc_radio;
	int keyword_asc_radio;
	int keyword_desc_radio;
	CButton inquery_btn;
	CListCtrl asset_list;
	afx_msg void OnCbnSelchangeHighCombo();
	afx_msg void OnCbnSelchangeMiddleCombo();
	afx_msg void OnBnClickedInqueryBtn();

	//List Layout 변경
	void ModifyStyle();
	void ModifyStyle2();
	void ModifyStyle3();

	//제품군을 가지고있는 벡터
	vector<CString> ItemCategory;

	//조회 버튼에 의한 조회 메소드 모듈화
	//메소드 이름 의미 : #1#2#3inquery
	//#1 : 대분류 , #2 : 중분류 , #3 : 소분류
	//해당 번호를 지정안했을 경우 N으로 표시
	void NNNinquery();
	void ZeroNNinquery();
	void OneNNinquery();
	void NumNumNinquery(CString fCode);
	void ZeroZeroNuminquery();
	void OneZeroNuminquery();
	void OneOneNinquery();
	void OneOneNuminquery();
	afx_msg void OnCbnSelchangeLowCombo();
	afx_msg void OnBnClickedInitializingBtn();
	afx_msg void OnBnClickedCostBtn();
	afx_msg void OnBnClickedWageBtn();
	afx_msg void OnBnClickedButton2();
	CButton idc_btn2;
};
