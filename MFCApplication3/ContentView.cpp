// ContentView.cpp : êµ¬í˜„ ?Œì¼?…ë‹ˆ??
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
END_MESSAGE_MAP()


// CContentView ì§„ë‹¨?…ë‹ˆ??

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


// CContentView ë©”ì‹œì§€ ì²˜ë¦¬ê¸°ì…?ˆë‹¤.

BOOL CContentView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: ?¬ê¸°???¹ìˆ˜?”ëœ ì½”ë“œë¥?ì¶”ê? ë°??ëŠ” ê¸°ë³¸ ?´ë˜?¤ë? ?¸ì¶œ?©ë‹ˆ??

	/*
	main contentsê°€ ?œì‹œ?˜ëŠ” CRightContainerView??
	ê¸°ë³¸?ìœ¼ë¡?listview???•íƒœë¥??ˆë‹¤
	*/
	//cs.style |= LVS_REPORT | LVS_SHOWSELALWAYS | LVS_SINGLESEL;

	
	return CListView::PreCreateWindow(cs);
}

void CContentView::DeleteContent(CListCtrl * m_list)
{
	/*
		ì»¨í…ì¸??”ë©´???„ë? ì§€?´ë‹¤
	*/

	// ë¦¬ìŠ¤????ª©??ì§€?´ë‹¤

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
	// TODO: ?¬ê¸°???¹ìˆ˜?”ëœ ì½”ë“œë¥?ì¶”ê? ë°??ëŠ” ê¸°ë³¸ ?´ë˜?¤ë? ?¸ì¶œ?©ë‹ˆ??

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

			m_list->InsertColumn(1, L"ÁÖ¹® ¹øÈ£", LVCFMT_CENTER, 150);
			m_list->InsertColumn(2, L"ÁÖ¹® ³¯Â¥", LVCFMT_CENTER, 150);
			m_list->InsertColumn(3, L"ÁÖ¹® ¼ö·®", LVCFMT_CENTER, 150);


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
			m_list->InsertItem(idx, L"+ »õ ÁÖ¹®ÇÏ±â", 0);
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

			m_list->InsertColumn(0, L"¹İÇ° À¯Çü", LVCFMT_CENTER, 150);
			m_list->InsertColumn(1, L"¹İÇ° ¹øÈ£", LVCFMT_CENTER, 150);
			m_list->InsertColumn(2, L"¹İÇ° ³¯Â¥", LVCFMT_CENTER, 150);
			m_list->InsertColumn(3, L"¹İÇ° ¼ö·®", LVCFMT_CENTER, 150);


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
					// ¹İÇ° À¯ÇüÀÌ ÁÖ¹® ¹İÇ°
					nListitm = m_list->InsertItem(0, L"ÁÖ¹® ¹İÇ°", 0);
				}
				else {

					// ¹İÇ° À¯ÇüÀÌ ÆÇ¸Å ¹İÇ°
					nListitm = m_list->InsertItem(0, L"ÆÇ¸Å ¹İÇ°", 0);
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
			// ?ë§¤ ê´€ë¦?


			if (m_list->GetItemCount() > 0)
				DeleteContent(m_list);

			MessageBox(L"ÆÇ¸Å °ü¸® ¸Ş´ºÀÔ´Ï´Ù");

			break;
		}
		case 4:
		{
			// ?ê¸ˆ ê´€ë¦?

			if (m_list->GetItemCount() > 0)
				DeleteContent(m_list);

			break;
		}
		case 6:
		{
			// ?„ì¬ ì§ì›
			if (m_list->GetItemCount() > 0)
				DeleteContent(m_list);

			break;
		}
		case 7:
		{
			// ì§ì› ?´ë ¥
			if (m_list->GetItemCount() > 0)
				DeleteContent(m_list);

			// m_list->ModifyStyle(LVS_TYPEMASK, 0);

			break;
		}
		case 8:
		{
			// ë¬¼í’ˆ ê´€ë¦?
			if (m_list->GetItemCount() > 0)
				DeleteContent(m_list);

			break;
		}
		case 9:
		{
			// È¸¿ø °ü¸®
			// Show member list

			CString member_code, name, phone, mileage;

			if (m_list->GetItemCount() > 0)
				DeleteContent(m_list); // If there are any left contents on the pane, delete all


			m_list->ModifyStyle(0, LVS_REPORT | LVS_SHOWSELALWAYS | LVS_SINGLESEL, 0);
			m_list->SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

			// Get the distinct order code and total sum of an amount of the products of the order
			recSet.Open(CRecordset::dynaset, L"SELECT MEMBER_CODE, NAME, PHONE, MILEAGE FROM MEMBER ORDER BY NAME");

			m_list->InsertColumn(0, L"ÀÌ¸§", LVCFMT_CENTER, 150);
			m_list->InsertColumn(1, L"È¸¿ø ÄÚµå", LVCFMT_CENTER, 150);
			m_list->InsertColumn(2, L"ÀüÈ­ ¹øÈ£", LVCFMT_CENTER, 150);
			m_list->InsertColumn(3, L"¸¶ÀÏ¸®Áö", LVCFMT_CENTER, 150);

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
			// UOS25 ë¡œê³ ?”ë©´

			CClientDC pDC(this);//?„ì¬ ?¤ì´?¼ë¡œê·¸ì˜ ?€?´í?ë°”ë? ?œì™¸???ì—­???»ëŠ”??
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

			memDC.CreateCompatibleDC(&pDC);//CDC?€ CClinetDCë¥??°ê²°?´ì£¼??êµ¬ë¬¸

			CBitmap m_bitMain;
			m_bitMain.LoadBitmapW(IDB_LOGO);
			CBitmap *oldbm = memDC.SelectObject(&m_bitMain);
			//	pDC.StretchBlt(17, 23, 300, 300, &memDC, 0, 0, 350, 350, SRCCOPY);
			pDC.BitBlt(150, 100, 888, 396, &memDC, 0, 0, SRCCOPY);
			//bitblt?¨ìˆ˜ë¥??¬ìš©?˜ì—¬ ?¤ì œ bmpê·¸ë¦¼?Œì¼???”ë©´??ì¶œë ¥?œë‹¤.
			//ì¢Œí‘œ 10,10?„ì¹˜??300*300???¬ê¸°ë¡?ê·¸ë¦¼??ê·¸ë¦°??
			//?ë³¸ê·¸ë¦¼???¼ìª½ ???¬ì¸?¸ë? 0,0?¼ë¡œ ?¤ì •?œë‹¤.
			//bmp?Œì¼???¬ìš©?˜ë?ë¡?ëª¨ë“  ì¶œë ¥?€ ?½ì???ê¸°ì??¼ë¡œ ?œë‹¤.

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
				// ¼±ÅÃµÈ ¾ÆÀÌÅÛÀÇ ÀÎµ¦½º ¹øÈ£¸¦ ¾ò´Â´Ù
				NM_LISTVIEW * pNMListView = (NM_LISTVIEW*) pNMHDR;
				int cur_idx = pNMListView->iItem;

				CString order_code, order_num, order_date;
				order_num = m_list->GetItemText(cur_idx, 0);
				order_date = m_list->GetItemText(cur_idx, 1);
				order_code.Format(L"%s01%s", order_date, order_num);

				if (order_num == "+ »õ ÁÖ¹®ÇÏ±â")
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
					// ÀçÁÖ¹® ÄÚµå°¡ µé¾î°¡ ÀÖ°Å³ª È®Á¤µÇ¾úÁö¸¸ ÀçÁÖ¹® ÄÚµå°¡ ¾ø¾î NÀÌ µé¾î°¡ ÀÖ´Â °æ¿ì
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
			// ¹İÇ°¸ñ·Ï
			if (pNMItemActivate->iItem != -1)
			{
				// ¼±ÅÃµÈ ¾ÆÀÌÅÛÀÇ ÀÎµ¦½º ¹øÈ£¸¦ ¾ò´Â´Ù
				NM_LISTVIEW * pNMListView = (NM_LISTVIEW*) pNMHDR;
				int cur_idx = pNMListView->iItem;

				CString ret_code, ret_num, ret_date;
				ret_num = m_list->GetItemText(cur_idx, 1);
				ret_date = m_list->GetItemText(cur_idx, 2);
				ret_code.Format(L"%s01%s", ret_date, ret_num);

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
			// ?ë§¤ ê´€ë¦?? íƒ
		}
		case 4:
		{
			// ?ê¸ˆ ê´€ë¦?? íƒ

		}
		case 6:
		{
			// ì§ì› ê´€ë¦?? íƒ
		}
		case 7:
		{
			// ì§ì› ?´ë ¥ ? íƒ
		}
		case 8:
		{
			// ë¬¼í’ˆ ê´€ë¦?? íƒ
		}
	default:
		break;
	}

	*pResult = 0;
}
