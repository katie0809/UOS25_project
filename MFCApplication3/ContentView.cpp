// ContentView.cpp : 구현 ?�일?�니??
//

#include "stdafx.h"
#include "MFCApplication3.h"
#include "ContentView.h"
#include "ManageOrder.h"
#include "ManageReturn.h"
#include "NewOrder.h"
// CContentView


IMPLEMENT_DYNCREATE(CContentView, CListView)

CContentView::CContentView()
{

}

CContentView::~CContentView()
{
}

BEGIN_MESSAGE_MAP(CContentView, CListView)
	ON_NOTIFY_REFLECT(NM_DBLCLK, &CContentView::OnNMDblclk)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_32771, &CContentView::OnCancelOrder)
	ON_COMMAND(ID_32774, &CContentView::OnCancelReturn)
	ON_NOTIFY_REFLECT(NM_CLICK, &CContentView::OnNMClick)
END_MESSAGE_MAP()


// CContentView 진단?�니??

#ifdef _DEBUG
void CContentView::AssertValid() const
{
	CListView::AssertValid();
}

#ifndef _WIN32_WCE
void CContentView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}
#endif
#endif //_DEBUG


// CContentView 메시지 처리기입?�다.

BOOL CContentView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: ?�기???�수?�된 코드�?추�? �??�는 기본 ?�래?��? ?�출?�니??

	/*
	main contents가 ?�시?�는 CRightContainerView??
	기본?�으�?listview???�태�??�다
	*/
	//cs.style |= LVS_REPORT | LVS_SHOWSELALWAYS | LVS_SINGLESEL;

	
	return CListView::PreCreateWindow(cs);
}

void CContentView::DeleteContent(CListCtrl * m_list)
{
	/*
		컨텐�??�면???��? 지?�다
	*/

	// 리스????��??지?�다

	CHeaderCtrl *p_headerCtrl;

	p_headerCtrl = m_list->GetHeaderCtrl();
	int columnCnt = 0;
	columnCnt = p_headerCtrl->GetItemCount();

	if (columnCnt > 0)
	{
		m_list->DeleteAllItems();
		for (int i = 0; i < columnCnt; i++)
			m_list->DeleteColumn(0);
	}

}

void CContentView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	// TODO: ?�기???�수?�된 코드�?추�? �??�는 기본 ?�래?��? ?�출?�니??

	CUR_CONTENT = lHint;
	m_list = &GetListCtrl();

	// Open database
	CDatabase db_content;
	CRecordset recSet(&db_content);

	TRY
	{
		db_content.OpenEx(_T("DSN=UOS25;UID=UOS25;PWD=0000"));
	}
		CATCH(CException, e)
	{
		TCHAR errMSG[255];

		e->GetErrorMessage(errMSG, 255);
		AfxMessageBox(errMSG, MB_ICONERROR);


		if (db_content.IsOpen())
			db_content.Close();

	}
	END_CATCH

	

	switch (lHint)
	{
		case 1:	
		{
			// Show Order List

			CString order_code, order_num, order_date, order_sum;
			int idx = 0;

			if (m_list->GetItemCount() > 0)
				DeleteContent(m_list); // If there are any left contents on the pane, delete all


			m_list->ModifyStyle(0, LVS_REPORT | LVS_SHOWSELALWAYS | LVS_SINGLESEL, 0);
			m_list->SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

			// Get the distinct order code and total sum of an amount of the products of the order
			recSet.Open(CRecordset::dynaset, L"SELECT DISTINCT ORDER_CODE, SUM(ORDER_AMOUNT) FROM ORDER_LIST GROUP BY ORDER_CODE ORDER BY ORDER_CODE");

			m_list->InsertColumn(1, L"�ֹ� ��ȣ", LVCFMT_CENTER, 150);
			m_list->InsertColumn(2, L"�ֹ� ��¥", LVCFMT_CENTER, 150);
			m_list->InsertColumn(3, L"�ֹ� ����", LVCFMT_CENTER, 150);


			while (!recSet.IsEOF())
			{
				recSet.GetFieldValue(_T("ORDER_CODE"), order_code);
				recSet.GetFieldValue(_T("SUM(ORDER_AMOUNT)"), order_sum);
				
				// Extract date and num from order code
				order_date = order_code.Left(8);
				order_num = order_code.Right(5);

				// Insert itm into list
				int nListitm = m_list->InsertItem(0, order_num, 0);
				m_list->SetItem(nListitm, 1, LVFIF_TEXT, order_date, 0, 0, 0, NULL);
				m_list->SetItem(nListitm, 2, LVFIF_TEXT, order_sum, 0, 0, 0, NULL);

				idx++;
				recSet.MoveNext();
			}

			recSet.Close();
			
			// �����ڷ� ������ ��쿡�� �� �ֹ��� �߰��� �� �ִ�.
			if(SYSTEM_USER == USER_MANAGER)
				m_list->InsertItem(idx, L"+ �� �ֹ��ϱ�", 0);
	
			break;
		}
		case 2:
		{
			// Show Return List

			CString ret_code, ret_num, ret_date, ret_sum;


			if (m_list->GetItemCount() > 0)
				DeleteContent(m_list); // If there are any left contents on the pane, delete all


			// Set the style of return list
			m_list->ModifyStyle(0, LVS_REPORT | LVS_SHOWSELALWAYS | LVS_SINGLESEL, 0);
			m_list->SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

			// Get the distinct order code and total sum of an amount of the products of the order
			recSet.Open(CRecordset::dynaset, L"SELECT DISTINCT RETURN_CODE, SUM(RETURN_AMOUNT) FROM RETURN GROUP BY RETURN_CODE ORDER BY RETURN_CODE");

			m_list->InsertColumn(0, L"��ǰ ����", LVCFMT_CENTER, 150);
			m_list->InsertColumn(1, L"��ǰ ��ȣ", LVCFMT_CENTER, 150);
			m_list->InsertColumn(2, L"��ǰ ��¥", LVCFMT_CENTER, 150);
			m_list->InsertColumn(3, L"��ǰ ����", LVCFMT_CENTER, 150);


			while (!recSet.IsEOF())
			{
				recSet.GetFieldValue(_T("RETURN_CODE"), ret_code);
				recSet.GetFieldValue(_T("SUM(RETURN_AMOUNT)"), ret_sum);

				// Extract date and num from order code
				ret_date = ret_code.Left(8);
				ret_num = ret_code.Mid(10, 13);

				// Insert itm into list
				int nListitm = 0;
				if (ret_code.Right(1) == '0')
				{
					// ��ǰ ������ �ֹ� ��ǰ
					nListitm = m_list->InsertItem(0, L"�ֹ� ��ǰ", 0);
				}
				else {

					// ��ǰ ������ �Ǹ� ��ǰ
					nListitm = m_list->InsertItem(0, L"�Ǹ� ��ǰ", 0);
				}
				m_list->SetItem(nListitm, 1, LVFIF_TEXT, ret_num, 0, 0, 0, NULL);
				m_list->SetItem(nListitm, 2, LVFIF_TEXT, ret_date, 0, 0, 0, NULL);
				m_list->SetItem(nListitm, 3, LVFIF_TEXT, ret_sum, 0, 0, 0, NULL);

				recSet.MoveNext();
			}

			recSet.Close();

			break;
		}
		case 3:
		{
			// ?�매 관�?


			if (m_list->GetItemCount() > 0)
				DeleteContent(m_list);

			break;
		}
		case 4:
		{
			// ?�금 관�?

			if (m_list->GetItemCount() > 0)
				DeleteContent(m_list);

			break;
		}
		case 6:
		{
			// ?�재 직원
			if (m_list->GetItemCount() > 0)
				DeleteContent(m_list);

			break;
		}
		case 7:
		{
			// 직원 ?�력
			if (m_list->GetItemCount() > 0)
				DeleteContent(m_list);

			// m_list->ModifyStyle(LVS_TYPEMASK, 0);

			break;
		}
		case 8:
		{
			// 물품 관�?
			if (m_list->GetItemCount() > 0)
				DeleteContent(m_list);

			break;
		}
		case 9:
		{
			// ȸ�� ����
			// Show member list

			CString member_code, name, phone, mileage;

			if (m_list->GetItemCount() > 0)
				DeleteContent(m_list); // If there are any left contents on the pane, delete all


			m_list->ModifyStyle(0, LVS_REPORT | LVS_SHOWSELALWAYS | LVS_SINGLESEL, 0);
			m_list->SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

			// Get the distinct order code and total sum of an amount of the products of the order
			recSet.Open(CRecordset::dynaset, L"SELECT MEMBER_CODE, NAME, PHONE, MILEAGE FROM MEMBER ORDER BY NAME");

			m_list->InsertColumn(0, L"�̸�", LVCFMT_CENTER, 150);
			m_list->InsertColumn(1, L"ȸ�� �ڵ�", LVCFMT_CENTER, 150);
			m_list->InsertColumn(2, L"��ȭ ��ȣ", LVCFMT_CENTER, 150);
			m_list->InsertColumn(3, L"���ϸ���", LVCFMT_CENTER, 150);

			while (!recSet.IsEOF())
			{
				recSet.GetFieldValue(_T("MEMBER_CODE"), member_code);
				recSet.GetFieldValue(_T("NAME"), name);
				recSet.GetFieldValue(_T("MILEAGE"), mileage);
				recSet.GetFieldValue(_T("PHONE"), phone);

				// Insert itm into list
				int nListitm = m_list->InsertItem(0, name, 0);
				m_list->SetItem(nListitm, 1, LVFIF_TEXT, member_code, 0, 0, 0, NULL);
				m_list->SetItem(nListitm, 2, LVFIF_TEXT, phone, 0, 0, 0, NULL);
				m_list->SetItem(nListitm, 3, LVFIF_TEXT, mileage, 0, 0, 0, NULL);

				recSet.MoveNext();
			}

			recSet.Close();

			break;
		}
		default:
		{
			// UOS25 로고?�면

			CClientDC pDC(this);//?�재 ?�이?�로그의 ?�?��?바�? ?�외???�역???�는??
			CDC memDC;
			CPen pen;
			CBrush brush;
			pen.CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
			brush.CreateSolidBrush(RGB(255, 255, 255));

			CPen *p = pDC.SelectObject(&pen);
			CBrush *b = pDC.SelectObject(&brush);

			pDC.Rectangle(0, 0, 1500, 800);
			pDC.SelectObject(p);
			pDC.SelectObject(b);

			memDC.CreateCompatibleDC(&pDC);//CDC?� CClinetDC�??�결?�주??구문

			CBitmap m_bitMain;
			m_bitMain.LoadBitmapW(IDB_LOGO);
			CBitmap *oldbm = memDC.SelectObject(&m_bitMain);
			//	pDC.StretchBlt(17, 23, 300, 300, &memDC, 0, 0, 350, 350, SRCCOPY);
			pDC.BitBlt(150, 100, 888, 396, &memDC, 0, 0, SRCCOPY);
			//bitblt?�수�??�용?�여 ?�제 bmp그림?�일???�면??출력?�다.
			//좌표 10,10?�치??300*300???�기�?그림??그린??
			//?�본그림???�쪽 ???�인?��? 0,0?�로 ?�정?�다.
			//bmp?�일???�용?��?�?모든 출력?� ?��???기�??�로 ?�다.

			break;
		}
	}

	return;
}


void CContentView::OnInitialUpdate()
{
	CListView::OnInitialUpdate();

}

void CContentView::OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>( pNMHDR );

	switch (CUR_CONTENT)
	{
		case 1:
		{
			// Open database
			CDatabase db_content;
			CRecordset recSet(&db_content);

			TRY
			{
				db_content.OpenEx(_T("DSN=UOS25;UID=UOS25;PWD=0000"));
			}
				CATCH(CException, e)
			{
				TCHAR errMSG[255];

				e->GetErrorMessage(errMSG, 255);
				AfxMessageBox(errMSG, MB_ICONERROR);


				if (db_content.IsOpen())
					db_content.Close();

			}
			END_CATCH

			if (pNMItemActivate->iItem != -1)
			{
				// ���õ� �������� �ε��� ��ȣ�� ��´�
				NM_LISTVIEW * pNMListView = (NM_LISTVIEW*) pNMHDR;
				int cur_idx = pNMListView->iItem;

				CString order_code, order_num, order_date;
				order_num = m_list->GetItemText(cur_idx, 0);
				order_date = m_list->GetItemText(cur_idx, 1);
				order_code.Format(L"%s01%s", order_date, order_num);
				CUR_CODE = order_code;

				if (order_num == "+ �� �ֹ��ϱ�")
				{
					// Open dialog for new order
					dlg_new_order = new NewOrder();
					dlg_new_order->Create(NewOrder::IDD);
					dlg_new_order->ShowWindow(SW_SHOW);

					break;
				}

				CString SQL, reorder_val;
				SQL.Format(L"select distinct reorder from order_list where order_code='%s'", order_code);
				recSet.Open(CRecordset::dynaset, SQL);
				recSet.GetFieldValue(_T("REORDER"), reorder_val);

				if (reorder_val != L"")
				{
					// ���ֹ��� �Ͼ ���� �ֹ��� �ڵ尡 �� �ְų� Ȯ���� ���� ���ڵ�μ� Y or N�� �� �ִ� ���
					// Open dialog for confirmed order
					dlg_confirmed_order = new COrderConfirm(this, order_code);
					dlg_confirmed_order->Create(COrderConfirm::IDD);
					dlg_confirmed_order->ShowWindow(SW_SHOW);

					break;
				}
				else 
				{
					// Open dialog using currently selected index num
					dlg_manage_order = new CManageOrder(this, order_code);
					dlg_manage_order->Create(CManageOrder::IDD);
					dlg_manage_order->ShowWindow(SW_SHOW);

					break;
				}				
			}
			else
			{

			}
			break;
			
		}
		case 2:
		{
			// ��ǰ���
			if (pNMItemActivate->iItem != -1)
			{
				// ���õ� �������� �ε��� ��ȣ�� ��´�
				NM_LISTVIEW * pNMListView = (NM_LISTVIEW*) pNMHDR;
				int cur_idx = pNMListView->iItem;

				CString ret_code, ret_num, ret_date;
				ret_num = m_list->GetItemText(cur_idx, 1);
				ret_date = m_list->GetItemText(cur_idx, 2);
				ret_code.Format(L"%s01%s", ret_date, ret_num);
				CUR_CODE = ret_code;

				// Open dialog using currently selected index num
				dlg_manage_return = new CManageReturn(this, ret_code);
				dlg_manage_return->Create(CManageReturn::IDD);
				dlg_manage_return->ShowWindow(SW_SHOW);

			}
			else
			{

			}
			break;
		}
		case 3:
		{
			// ?�매 관�??�택
		}
		case 4:
		{
			// ?�금 관�??�택

		}
		case 6:
		{
			// 직원 관�??�택
		}
		case 7:
		{
			// 직원 ?�력 ?�택
		}
		case 8:
		{
			// 물품 관�??�택
		}
	default:
		break;
	}

	*pResult = 0;
}


void CContentView::OnContextMenu(CWnd* /*pWnd*/, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	CMenu menu;
	menu.LoadMenuW(IDR_MENU1);

	switch(CUR_CONTENT)
	{
	case 1:
	{
		CMenu * pmenu = menu.GetSubMenu(0);
		pmenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, AfxGetMainWnd());
		break;
	}
	case 2:
	{
		CMenu * pmenu = menu.GetSubMenu(1);
		pmenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, AfxGetMainWnd());

		break;
	}
	default:
		break;
	}
}


void CContentView::OnCancelOrder()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.

	// Open database
	CDatabase db_content;

	TRY
	{
		db_content.OpenEx(_T("DSN=UOS25;UID=UOS25;PWD=0000"));
	}
		CATCH(CException, e)
	{
		TCHAR errMSG[255];

		e->GetErrorMessage(errMSG, 255);
		AfxMessageBox(errMSG, MB_ICONERROR);


		if (db_content.IsOpen())
			db_content.Close();

	}
	END_CATCH

	CString SQL;
	SQL.Format(L"DELETE FROM ORDER_LIST WHERE ORDER_CODE = '%s'", CUR_CODE);
	//MessageBox(SQL);
	db_content.ExecuteSQL(SQL);

}


void CContentView::OnCancelReturn()
{
	// Open database
	CDatabase db_content;

	TRY
	{
		db_content.OpenEx(_T("DSN=UOS25;UID=UOS25;PWD=0000"));
	}
		CATCH(CException, e)
	{
		TCHAR errMSG[255];

		e->GetErrorMessage(errMSG, 255);
		AfxMessageBox(errMSG, MB_ICONERROR);


		if (db_content.IsOpen())
			db_content.Close();

	}
	END_CATCH

	CString SQL;
	SQL.Format(L"UPDATE RETURN SET DETAIL = '��ҵ� ��ǰ' WHERE RETURN_CODE = '%s'", CUR_CODE);
	db_content.ExecuteSQL(SQL);
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
}


void CContentView::OnNMClick(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>( pNMHDR );
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	switch (CUR_CONTENT)
	{
	case 1:
	{
			if (pNMItemActivate->iItem != -1)
			{
				// ���õ� �������� �ε��� ��ȣ�� ��´�
				NM_LISTVIEW * pNMListView = (NM_LISTVIEW*) pNMHDR;
				int cur_idx = pNMListView->iItem;

				CString order_code, order_num, order_date;
				order_num = m_list->GetItemText(cur_idx, 0);
				order_date = m_list->GetItemText(cur_idx, 1);
				order_code.Format(L"%s01%s", order_date, order_num);
				CUR_CODE = order_code;

			}
			else
			{

			}
		break;

	}
	case 2:
	{
		// ��ǰ���
		if (pNMItemActivate->iItem != -1)
		{
			// ���õ� �������� �ε��� ��ȣ�� ��´�
			NM_LISTVIEW * pNMListView = (NM_LISTVIEW*) pNMHDR;
			int cur_idx = pNMListView->iItem;

			CString ret_code, ret_num, ret_date;
			ret_num = m_list->GetItemText(cur_idx, 1);
			ret_date = m_list->GetItemText(cur_idx, 2);
			ret_code.Format(L"%s01%s", ret_date, ret_num);
			CUR_CODE = ret_code;


		}
		else
		{

		}
		break;
	}
	case 3:
	{
		// ?�매 관�??�택
	}
	case 4:
	{
		// ?�금 관�??�택

	}
	case 6:
	{
		// 직원 관�??�택
	}
	case 7:
	{
		// 직원 ?�력 ?�택
	}
	case 8:
	{
		// 물품 관�??�택
	}
	default:
		break;
	}

	*pResult = 0;
}
