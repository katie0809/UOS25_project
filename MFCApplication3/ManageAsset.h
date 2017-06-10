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
	ManageAsset(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~ManageAsset();
	enum { IDD = IDD_ASSET_MANAGEMENT };


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	//���� �ν��Ͻ��� Linking
	virtual BOOL OnInitDialog();

	//������ �߰� Ŭ����
	NewCost * newcost;

	//�ӱ� �߰� Ŭ����
	ManageWage * newwage;

	//�����ͺ��̽� �ν��Ͻ�
	CDatabase db_asset;


	//�޺��ڽ� �ν��Ͻ�, �� �޺��ڽ��� ���õ� ������ index�� ������ �ν��Ͻ�
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

	//List Layout ����
	void ModifyStyle();
	void ModifyStyle2();
	void ModifyStyle3();

	//��ǰ���� �������ִ� ����
	vector<CString> ItemCategory;

	//��ȸ ��ư�� ���� ��ȸ �޼ҵ� ���ȭ
	//�޼ҵ� �̸� �ǹ� : #1#2#3inquery
	//#1 : ��з� , #2 : �ߺз� , #3 : �Һз�
	//�ش� ��ȣ�� ���������� ��� N���� ǥ��
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
