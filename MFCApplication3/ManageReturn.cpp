// ManageReturn.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "MFCApplication3.h"
#include "ContentView.h"
#include "ManageReturn.h"
#include "afxdialogex.h"


// CManageReturn ��ȭ �����Դϴ�.

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


// CManageReturn �޽��� ó�����Դϴ�.


BOOL CManageReturn::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

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
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}

void CManageReturn::ShowData(CDatabase & db_ret)
{
	CRecordset recSet(&db_ret);
	CString strSQL, strNAME, strPRICE, strREMAIN, strCODE, strMAKER, strRETAMT, strDETAIL, mommyCode;

	// Modify list style
	m_returnList.ModifyStyle(0, LVS_REPORT | LVS_SHOWSELALWAYS | LVS_EDITLABELS, 0);
	m_returnList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	// �������� ���� Ư�� ��¥�� ��ǰ��ϸ� �޾ƿ´�
	// Get product code, product name, product maker, product price, order amount, product stock amount, event detail
	strSQL.Format(L"SELECT PRODUCT.PROD_CODE, PROD_NAME, PROD_MAKER, PROD_PRICE, PROD_STOCK_AMOUNT, RETURN_AMOUNT, RETURN_HIGH_CODE, DETAIL FROM product INNER JOIN RETURN ON RETURN.RETURN_CODE = '%s' AND RETURN.PROD_CODE = PRODUCT.PROD_CODE;", ret_id);
	recSet.Open(CRecordset::dynaset, strSQL);

	// Create Column
	m_returnList.InsertColumn(0, L"��ǰ ���� (��ǰ ������ �����ּ���)", LVCFMT_CENTER, 380);
	m_returnList.InsertColumn(1, L"��ǰ��", LVCFMT_CENTER, 100);
	m_returnList.InsertColumn(2, L"������", LVCFMT_CENTER, 90);
	m_returnList.InsertColumn(3, L"��ǰ ����", LVCFMT_CENTER, 80);
	m_returnList.InsertColumn(4, L"��ǰ ����", LVCFMT_CENTER, 80);
	m_returnList.InsertColumn(5, L"��� ����", LVCFMT_CENTER, 80);

	recSet.GetFieldValue(_T("RETURN_HIGH_CODE"), mommyCode);
	m_mommyCode.AddString(mommyCode);

	if (ret_type == '0')
	{
		s_retType.SetWindowTextW(L"��ǰ Ÿ�� : �ֹ� ��ǰ");
	}
	else s_retType.SetWindowTextW(L"��ǰ Ÿ�� : �Ǹ� ��ǰ");
	

	// �޾ƿ� ���̺� ���� �����Ͱ� ���� ������ ����
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

		//���� ���ڵ�� �̵�
		recSet.MoveNext();
	}

	recSet.Close();
}

void CManageReturn::OnLbnSelchangeList1()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CString code;
	m_mommyCode.GetText(0, code);

	if (ret_type == '0')
	{
		// �ֹ� ��ǰ�� ��� ���� �ֹ� ���� ���̾�α׸� ����
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
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	// ��ǰ ���� �����ͺ��̽��� ����
	int nItm = 0;
	nItm = m_returnList.GetItemCount();

	for (int i = 0; i < nItm; i++)
	{
		CString SQL;
		SQL.Format(L"UPDATE RETURN SET DETAIL = '%s' WHERE RETURN_CODE = '%s'", m_returnList.GetItemText(i, 0), ret_id);
		//MessageBox(SQL);
		db_ret.ExecuteSQL(SQL);
	}
	MessageBox(L"��ǰ ���� ���� �Ϸ�");
	OnOK();
}


void CManageReturn::OnEndlabeleditReturn(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>( pNMHDR );
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	*pResult = TRUE;
}
