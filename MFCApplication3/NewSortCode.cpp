// NewSortCode.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MFCApplication3.h"
#include "NewSortCode.h"
#include "afxdialogex.h"


// NewSortCode 대화 상자입니다.

IMPLEMENT_DYNAMIC(NewSortCode, CDialogEx)

NewSortCode::NewSortCode(CWnd* pParent, CString code /*=NULL*/)
	: CDialogEx(IDD_NEW_SORT_CODE, pParent)
{
	sort_code = code;
}

NewSortCode::~NewSortCode()
{
}
void NewSortCode::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, 1019, m_newsortcode_code);
	DDX_Control(pDX, 1025, m_newsortcode_high);
	DDX_Control(pDX, 1020, m_newsortcode_detail);
}


BEGIN_MESSAGE_MAP(NewSortCode, CDialogEx)
END_MESSAGE_MAP()



// NewSortCode 메시지 처리기입니다.


BOOL NewSortCode::OnInitDialog() {
	CDialogEx::OnInitDialog();

	// 데이터베이스와의 연결

	TRY
	{
		db_newsortcode.OpenEx(_T("DSN=UOS25;UID=UOS25;PWD=0000"));
	}
		CATCH(CException, e)
	{
		TCHAR errMSG[255];

		e->GetErrorMessage(errMSG, 255);
		AfxMessageBox(errMSG, MB_ICONERROR);


		if (db_newsortcode.IsOpen())
			db_newsortcode.Close();

	}
	END_CATCH

		ShowData();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void NewSortCode::ShowData() {
	CRecordset recSet(&db_newsortcode);
	CString sql, code, high_sort_code, detail, high_combo;

	sql.Format(L"SELECT SORT_CODE, SORT_DETAIL FROM SORT_CODE WHERE HIGH_SORT_CODE IS NULL;");
	recSet.Open(CRecordset::dynaset, sql);
	// 코드 목록 출력
	while (!recSet.IsEOF())
	{
		recSet.GetFieldValue(_T("SORT_CODE"), code);
		recSet.GetFieldValue(_T("SORT_DETAIL"), detail);

		high_combo.Format(L"%s.%s", code, detail);
		m_newsortcode_high.AddString(high_combo);

		//다음 레코드로 이동
		recSet.MoveNext();
	}
	recSet.Close();
	m_newsortcode_high.SetCurSel(0);

	// 수정인 경우
	if (!sort_code.IsEmpty()) {
		sql.Format(L"SELECT SORT_CODE, HIGH_SORT_CODE, SORT_DETAIL FROM SORT_CODE WHERE SORT_CODE= '%s';", sort_code);
		recSet.Open(CRecordset::dynaset, sql);

		// 코드 목록 출력
		if (!recSet.IsEOF())
		{
			recSet.GetFieldValue(_T("SORT_CODE"), code);
			recSet.GetFieldValue(_T("HIGH_SORT_CODE"), high_sort_code);
			recSet.GetFieldValue(_T("SORT_DETAIL"), detail);
		}
		recSet.Close();

		m_newsortcode_code.SetWindowTextW(code);
		m_newsortcode_detail.SetWindowTextW(detail);

		for (int i = 0; i < m_newsortcode_high.GetCount(); i++) {
			m_newsortcode_high.GetLBText(i, high_combo);
			if (high_combo == high_sort_code) {
				m_newsortcode_high.SetCurSel(i);
				break;
			}
		}
		GetDlgItem(IDOK)->SetWindowText(TEXT("수정"));
	}

}

void NewSortCode::OnBnClickedOk()
{
	CString sql, code, high_sort_code, detail, high_combo;
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	m_newsortcode_code.GetWindowTextW(code);
	m_newsortcode_detail.GetWindowTextW(detail);
	this->m_newsortcode_high.GetLBText(this->m_newsortcode_high.GetCurSel(), high_sort_code);

	if (!code.IsEmpty() && !detail.IsEmpty()) {
		AfxExtractSubString(high_sort_code, high_sort_code, 0, '.');

		if (!sort_code.IsEmpty()) {
			sql.Format(L"UPDATE SORT_CODE SET SORT_CODE='%s', HIGH_SORT_CODE='%s', SORT_DETAIL='%s' WHERE SORT_CODE='%s'", code, high_sort_code, detail, sort_code);
			db_newsortcode.ExecuteSQL(sql);
			MessageBox(L"구분코드 수정 완료");
			CDialogEx::OnOK();
		}
		else {
			sql.Format(L"INSERT INTO SORT_CODE(SORT_CODE, HIGH_SORT_CODE, SORT_DETAIL) values ('%s', '%s', '%s')", code, high_sort_code, detail);
			db_newsortcode.ExecuteSQL(sql);
			MessageBox(L"구분코드 추가 완료");
			CDialogEx::OnOK();
		}
	}
	else
		MessageBox(L"모든 항목을 입력하십시오.");
}
