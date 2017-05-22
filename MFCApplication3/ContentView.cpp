// ContentView.cpp : Й╣╛М≤└ ?▄Л²╪?┘К▀┬??
//

#include "stdafx.h"
#include "MFCApplication3.h"
#include "ContentView.h"
#include "ManageOrder.h"
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
END_MESSAGE_MAP()


// CContentView Л╖└К▀╗?┘К▀┬??

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


// CContentView К╘■Л▀°Л╖─ Л╡≤К╕╛Й╦╟Л·┘?┬К▀╓.

BOOL CContentView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: ?╛Й╦╟???╧Л┬≤?■К░° Л╫■К⌠°К╔?Л╤■Й? К╟??░К┼■ Й╦╟КЁ╦ ?╢К·≤?╓К? ?╦Л╤°?╘К▀┬??

	/*
	main contentsЙ╟─ ?°Л▀°?≤К┼■ CRightContainerView??
	Й╦╟КЁ╦?│Л°╪К║?listview???∙М┐°К╔??┬К▀╓
	*/
	//cs.style |= LVS_REPORT | LVS_SHOWSELALWAYS | LVS_SINGLESEL;

	
	return CListView::PreCreateWindow(cs);
}

void CContentView::DeleteContent(CListCtrl * m_list)
{
	/*
		Л╩╗М┘░Л╦??■К╘╢???└К? Л╖─?╢К▀╓
	*/

	// К╕╛Л┼╓????╙╘??Л╖─?╢К▀╓

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
	// TODO: ?╛Й╦╟???╧Л┬≤?■К░° Л╫■К⌠°К╔?Л╤■Й? К╟??░К┼■ Й╦╟КЁ╦ ?╢К·≤?╓К? ?╦Л╤°?╘К▀┬??

	CUR_CONTENT = lHint;
	m_list = &GetListCtrl();

	// ?╟Л²╢?╟К╡═?╢Л┼╓???╟Й╡╟
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

			m_list->ModifyStyle(0, LVS_REPORT | LVS_SHOWSELALWAYS | LVS_SINGLESEL, 0);
			m_list->SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

			if (m_list->GetItemCount() > 0)
				DeleteContent(m_list); // If there are any left contents on the pane, delete all

			// Get the distinct order code and total sum of an amount of the products of the order
			recSet.Open(CRecordset::dynaset, L"SELECT DISTINCT ORDER_CODE, SUM(ORDER_AMOUNT) FROM ORDER_ GROUP BY ORDER_CODE");

			m_list->InsertColumn(1, L"аж╧╝ ╧Ьхё", LVCFMT_CENTER, 150);
			m_list->InsertColumn(2, L"аж╧╝ Ё╞б╔", LVCFMT_CENTER, 150);
			m_list->InsertColumn(3, L"аж╧╝ ╪Ж╥╝", LVCFMT_CENTER, 150);


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
			m_list->InsertItem(idx, L"+ ╩У аж╧╝го╠Б", 0);
			break;
		}
		case 2:
		{
			// RETURN
		
			// Set the style of Return list
			m_list->ModifyStyle(0, LVS_REPORT | LVS_SHOWSELALWAYS | LVS_SINGLESEL, 0);
			m_list->SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

			// ?■К╘╢ ?╢К╕╛??
			if (m_list->GetItemCount() > 0)
				DeleteContent(m_list);

			int idx = 1;
			recSet.Open(CRecordset::dynaset, L"select distinct order_date, order_id from order_list order by order_id");
			// recSet.Open(CRecordset::dynaset, L"select distinct RETURN_CODE, RETURN_AMOUNT from RETURN order by RETURN_CODE");


			m_list->InsertColumn(idx, L"╧щг╟ ╦Я╥о", LVCFMT_CENTER, 400);

			while (!recSet.IsEOF())
			{
				CString order_date = L" ", order_list;
				recSet.GetFieldValue(_T("ORDER_DATE"), order_date);
				AfxExtractSubString(order_date, order_date, 0, ' ');
				order_list.Format(L"Лё╪К╛╦К╡┬М≤╦ %d | %s", idx + 1000, order_date);

				m_list->InsertItem(idx, order_list, 30);
				m_list->SetItemText(idx, 0, order_list);
				idx++;
				recSet.MoveNext();
			}

			recSet.Close();
			m_list->InsertItem(idx, L"+ ??К╟≤М▓┬ ?≤Й╦╟", 30);
			m_list->SetItemText(idx, 0, L"+ ??К╟≤М▓┬ ?≤Й╦╟");
			break;
		
			break;
		}
		case 3:
		{
			// ?░К╖╓ Й╢─К╕?


			if (m_list->GetItemCount() > 0)
				DeleteContent(m_list);

			break;
		}
		case 4:
		{
			// ?░Й╦┬ Й╢─К╕?

			if (m_list->GetItemCount() > 0)
				DeleteContent(m_list);

			break;
		}
		case 6:
		{
			// ?└Л·╛ Л╖│Л⌡░
			if (m_list->GetItemCount() > 0)
				DeleteContent(m_list);

			break;
		}
		case 7:
		{
			// Л╖│Л⌡░ ?╢К═╔
			if (m_list->GetItemCount() > 0)
				DeleteContent(m_list);

			// m_list->ModifyStyle(LVS_TYPEMASK, 0);

			break;
		}
		case 8:
		{
			// К╛╪М▓┬ Й╢─К╕?
			if (m_list->GetItemCount() > 0)
				DeleteContent(m_list);

			break;
		}
		default:
		{
			// UOS25 К║°ЙЁ═?■К╘╢

			CClientDC pDC(this);//?└Л·╛ ?╓Л²╢?╪К║°Й╥╦Л²≤ ?─?╢М?К╟■К? ?°Л≥╦???│Л≈╜???╩К┼■??
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

			memDC.CreateCompatibleDC(&pDC);//CDC?─ CClinetDCК╔??╟Й╡╟?╢Лё╪??Й╣╛К╛╦

			CBitmap m_bitMain;
			m_bitMain.LoadBitmapW(IDB_LOGO);
			CBitmap *oldbm = memDC.SelectObject(&m_bitMain);
			//	pDC.StretchBlt(17, 23, 300, 300, &memDC, 0, 0, 350, 350, SRCCOPY);
			pDC.BitBlt(150, 100, 888, 396, &memDC, 0, 0, SRCCOPY);
			//bitblt?╗Л┬≤К╔??╛Л ╘?≤Л≈╛ ?╓Л═° bmpЙ╥╦К╕╪?▄Л²╪???■К╘╢??Л╤°К═╔?°К▀╓.
			//Л╒▄М▒° 10,10?└Л╧≤??300*300???╛Й╦╟К║?Й╥╦К╕╪??Й╥╦К╕╟??
			//?░КЁ╦Й╥╦К╕╪???╪Л╙╫ ???╛Л²╦?╦К? 0,0?╪К║° ?╓Л═∙?°К▀╓.
			//bmp?▄Л²╪???╛Л ╘?≤К?К║?К╙╗К⌠═ Л╤°К═╔?─ ?╫Л???Й╦╟Л??╪К║° ?°К▀╓.

			break;
		}
	}

	return;
}


void CContentView::OnInitialUpdate()
{
	CListView::OnInitialUpdate();

	// TODO: ?╛Й╦╟???╧Л┬≤?■К░° Л╫■К⌠°К╔?Л╤■Й? К╟??░К┼■ Й╦╟КЁ╦ ?╢К·≤?╓К? ?╦Л╤°?╘К▀┬??
	
	//m_list = &GetListCtrl();
	//m_list->SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );
	
	// Л╩╛К÷╪??КЁ╢Л²╢Л╖─ ?┼К▐└К║??≤К┼■ ?╓М???
	//m_list->ModifyStyle(0, LVS_NOCOLUMNHEADER);
}

/*
	К╕╛Л┼╓?╦К? ?■К╦■?╢К╕╜??Й╡╫Л ╟ ?≤Л²╢Л╖─???╟К²╪ ?└Л ■???╓Л²╢?╪К║°Й╥??└Л ╢??
*/
void CContentView::OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>( pNMHDR );
	// TODO: ?╛Й╦╟??Л╩╗М┼╦К║??▄К╕╪ Л╡≤К╕╛Й╦?Л╫■К⌠°К╔?Л╤■Й??╘К▀┬??

	switch (CUR_CONTENT)
	{
		case 1:
		{
			if (pNMItemActivate->iItem != -1)
			{
				// ?╢К╕╜?????≤К╡└Й╟▓Л²└ К╟⌡Л∙└?╗К▀╓
				NM_LISTVIEW * pNMListView = (NM_LISTVIEW*) pNMHDR;
				int cur_idx = pNMListView->iItem;

			
				// idxК╡┬Л╖╦ ?┴Л²≤ К╖???Й╟▓Л²└ К╟⌡Л∙└?─ ?└Л▀° ?╓М┼╦К╖│Л°╪К║??─??
				// ?═М┐²????╙╘??CStringЙ╟▓Л²└ К╟⌡Л∙└?╓К┼■ Л╫■К⌠°?┘К▀┬??
				CString tmp_str;
				tmp_str = m_list->GetItemText(cur_idx, 0);

				if (tmp_str == "+ ╩У аж╧╝го╠Б")
				{
					//??Лё╪К╛╦?≤Й╦╟ ??╙╘ ?═М┐² ??
					dlg_new_order = new NewOrder();
					dlg_new_order->Create(NewOrder::IDD);
					dlg_new_order->ShowWindow(SW_SHOW);
					break;
				}

				// current_date = m_list->GetItemText(cur_idx, 0);
			
				// ?╓Л²╢?╪К║°Й╥??²Л└╠
				dlg_manage_order = new CManageOrder(this, cur_idx);
				dlg_manage_order->Create(CManageOrder::IDD);
				dlg_manage_order->ShowWindow(SW_SHOW);

			}
			else
			{

			}
			break;
		}
		case 2:
		{
			//К╟≤М▓┬ Й╢─К╕??═М┐²

			break;
		}
		case 3:
		{
			// ?░К╖╓ Й╢─К╕??═М┐²
		}
		case 4:
		{
			// ?░Й╦┬ Й╢─К╕??═М┐²
		}
		case 6:
		{
			// Л╖│Л⌡░ Й╢─К╕??═М┐²
		}
		case 7:
		{
			// Л╖│Л⌡░ ?╢К═╔ ?═М┐²
		}
		case 8:
		{
			// К╛╪М▓┬ Й╢─К╕??═М┐²
		}
	default:
		break;
	}

	*pResult = 0;
}
