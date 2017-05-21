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

	// ?°ì´?°ë² ?´ìŠ¤???°ê²°
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
			// ì£¼ë¬¸ ê´€ë¦?
			// m_list = &GetListCtrl();

			m_list->ModifyStyle(0, LVS_REPORT | LVS_SHOWSELALWAYS | LVS_SINGLESEL, 0);
			m_list->SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

			if (m_list->GetItemCount() > 0)
				DeleteContent(m_list);

			int idx = 1;
			recSet.Open(CRecordset::dynaset, L"select distinct order_date, order_id from order_list order by order_id");
			// recSet.Open(CRecordset::dynaset, L"select distinct ORDER_CODE from _ORDER order by ORDER_CODE");

			// m_list->InsertColumn(idx, L"ì£¼ë¬¸ ? ì§œ", LVCFMT_CENTER, 400);
			m_list->InsertColumn(idx, L"ì£¼ë¬¸ ëª©ë¡", LVCFMT_CENTER, 400);
			// m_list->InsertColumn(idx, L"ì£¼ë¬¸ ?˜ëŸ‰", LVCFMT_CENTER, 400);

			while (!recSet.IsEOF())
			{
				CString order_date = L" ", order_list;
				recSet.GetFieldValue(_T("ORDER_DATE"), order_date);
				AfxExtractSubString(order_date, order_date, 0, ' ');
				order_list.Format(L"ì£¼ë¬¸ë²ˆí˜¸ %d | %s", idx+1000, order_date);

				m_list->InsertItem(idx, order_list, 30);
				m_list->SetItemText(idx, 0, order_list);
				idx++;
				recSet.MoveNext();
			
				/*
				CString order_code = L" ", order_date;
				recSet.GetFieldValue(_T("ORDER_CODE"), order_code);;
				order_date = order_code.Left(8); //ì£¼ë¬¸ ì½”ë“œ?ì„œ ì£¼ë¬¸? ì§œ ì¶”ì¶œ
				list_itm.Format(L"ì£¼ë¬¸? ì§œ %s | %s", order_date, order_code);

				m_list->InsertItem(idx, list_itm, 30);
				m_list->SetItemText(idx, 0, list_itm);
				idx++;
				recSet.MoveNext();
				*/
			}

			recSet.Close();
			m_list->InsertItem(idx, L"+ ??ì£¼ë¬¸ ?˜ê¸°", 30);
			m_list->SetItemText(idx, 0, L"+ ??ì£¼ë¬¸ ?˜ê¸°");
			break;
		}
		case 2:
		{
			// ë°˜í’ˆ ê´€ë¦?
		
			// ?”ë©´ ?¤í???ì§€??
			m_list->ModifyStyle(0, LVS_REPORT | LVS_SHOWSELALWAYS | LVS_SINGLESEL, 0);
			m_list->SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

			// ?”ë©´ ?´ë¦¬??
			if (m_list->GetItemCount() > 0)
				DeleteContent(m_list);

			int idx = 1;
			recSet.Open(CRecordset::dynaset, L"select distinct order_date, order_id from order_list order by order_id");
			// recSet.Open(CRecordset::dynaset, L"select distinct RETURN_CODE, RETURN_AMOUNT from RETURN order by RETURN_CODE");


			m_list->InsertColumn(idx, L"ë°˜í’ˆ ëª©ë¡", LVCFMT_CENTER, 400);

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
				// ?´ë¦­?????˜ë²„ê°’ì„ ë°›ì•„?¨ë‹¤
				NM_LISTVIEW * pNMListView = (NM_LISTVIEW*) pNMHDR;
				int cur_idx = pNMListView->iItem;

			
				// idxë²ˆì§¸ ?‰ì˜ ë§???ê°’ì„ ë°›ì•„?€ ?„ì‹œ ?¤íŠ¸ë§ìœ¼ë¡??€??
				// ? íƒ????ª©??CStringê°’ì„ ë°›ì•„?¤ëŠ” ì½”ë“œ?…ë‹ˆ??
				CString tmp_str;
				tmp_str = m_list->GetItemText(cur_idx, 0);

				if (tmp_str == "+ ??ì£¼ë¬¸ ?˜ê¸°")
				{
					//??ì£¼ë¬¸?˜ê¸° ??ª© ? íƒ ??
					dlg_new_order = new NewOrder();
					dlg_new_order->Create(NewOrder::IDD);
					dlg_new_order->ShowWindow(SW_SHOW);
					break;
				}

				// current_date = m_list->GetItemText(cur_idx, 0);
			
				// ?¤ì´?¼ë¡œê·??ì„±
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
