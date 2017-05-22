// ContentView.cpp : êµ¬í˜„ ?Œì¼?…ë‹ˆ??
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

			m_list->ModifyStyle(0, LVS_REPORT | LVS_SHOWSELALWAYS | LVS_SINGLESEL, 0);
			m_list->SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

			if (m_list->GetItemCount() > 0)
				DeleteContent(m_list); // If there are any left contents on the pane, delete all

			// Get the distinct order code and total sum of an amount of the products of the order
			recSet.Open(CRecordset::dynaset, L"SELECT DISTINCT ORDER_CODE, SUM(ORDER_AMOUNT) FROM ORDER_ GROUP BY ORDER_CODE ORDER BY ORDER_CODE");

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
			// RETURN
		
			// Set the style of Return list
			m_list->ModifyStyle(0, LVS_REPORT | LVS_SHOWSELALWAYS | LVS_SINGLESEL, 0);
			m_list->SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

			// ?”ë©´ ?´ë¦¬??
			if (m_list->GetItemCount() > 0)
				DeleteContent(m_list);

			int idx = 1;
			recSet.Open(CRecordset::dynaset, L"select distinct order_date, order_id from order_list order by order_id");
			// recSet.Open(CRecordset::dynaset, L"select distinct RETURN_CODE, RETURN_AMOUNT from RETURN order by RETURN_CODE");


			m_list->InsertColumn(idx, L"¹İÇ° ¸ñ·Ï", LVCFMT_CENTER, 400);

			while (!recSet.IsEOF())
			{
				CString order_date = L" ", order_list;
				recSet.GetFieldValue(_T("ORDER_DATE"), order_date);
				AfxExtractSubString(order_date, order_date, 0, ' ');
				order_list.Format(L"ì£¼ë¬¸ë²ˆí˜¸ %d | %s", idx + 1000, order_date);

				m_list->InsertItem(idx, order_list, 30);
				m_list->SetItemText(idx, 0, order_list);
				idx++;
				recSet.MoveNext();
			}

			recSet.Close();
			m_list->InsertItem(idx, L"+ ??ë°˜í’ˆ ?˜ê¸°", 30);
			m_list->SetItemText(idx, 0, L"+ ??ë°˜í’ˆ ?˜ê¸°");
			break;
		
			break;
		}
		case 3:
		{
			// ?ë§¤ ê´€ë¦?


			if (m_list->GetItemCount() > 0)
				DeleteContent(m_list);

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

	// TODO: ?¬ê¸°???¹ìˆ˜?”ëœ ì½”ë“œë¥?ì¶”ê? ë°??ëŠ” ê¸°ë³¸ ?´ë˜?¤ë? ?¸ì¶œ?©ë‹ˆ??
	
	//m_list = &GetListCtrl();
	//m_list->SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );
	
	// ì»¬ëŸ¼??ë³´ì´ì§€ ?Šë„ë¡??˜ëŠ” ?¤í???
	//m_list->ModifyStyle(0, LVS_NOCOLUMNHEADER);
}

/*
	ë¦¬ìŠ¤?¸ë? ?”ë¸”?´ë¦­??ê²½ìš° ?˜ì´ì§€???°ë¼ ?„ìš”???¤ì´?¼ë¡œê·??„ìš´??
*/
void CContentView::OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>( pNMHDR );
	// TODO: ?¬ê¸°??ì»¨íŠ¸ë¡??Œë¦¼ ì²˜ë¦¬ê¸?ì½”ë“œë¥?ì¶”ê??©ë‹ˆ??

	switch (CUR_CONTENT)
	{
		case 1:
		{
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
			
				// Open dialog using currently selected index num
				dlg_manage_order = new CManageOrder(this, order_code);
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
			//ë°˜í’ˆ ê´€ë¦?? íƒ

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
