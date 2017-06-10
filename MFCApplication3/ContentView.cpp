// ContentView.cpp : êµ¬í˜„ ?Œì¼?…ë‹ˆ??
//

#include "stdafx.h"
#include "MFCApplication3.h"
#include "ContentView.h"

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
			recSet.Open(CRecordset::dynaset, L"SELECT DISTINCT ORDER_NUM, SUM(ORDER_AMOUNT) FROM ORDER_LIST GROUP BY order_num ORDER BY order_num");

			m_list->InsertColumn(1, L"ÁÖ¹® ¹øÈ£", LVCFMT_CENTER, 150);
			m_list->InsertColumn(2, L"ÁÖ¹® ³¯Â¥", LVCFMT_CENTER, 150);
			m_list->InsertColumn(3, L"ÁÖ¹® ¼ö·®", LVCFMT_CENTER, 150);


			while (!recSet.IsEOF())
			{
				recSet.GetFieldValue(_T("ORDER_NUM"), order_code);
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

			// °ü¸®ÀÚ·Î Á¢¼ÓÇÑ °æ¿ì¿¡¸¸ »õ ÁÖ¹®À» Ãß°¡ÇÒ ¼ö ÀÖ´Ù.
			if (SYSTEM_USER == USER_MANAGER)
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
			recSet.Open(CRecordset::dynaset, L"SELECT DISTINCT return_num, SUM(RETURN_AMOUNT) FROM RETURN GROUP BY return_num ORDER BY return_num");

			m_list->InsertColumn(0, L"¹İÇ° À¯Çü", LVCFMT_CENTER, 150);
			m_list->InsertColumn(1, L"¹İÇ° ¹øÈ£", LVCFMT_CENTER, 150);
			m_list->InsertColumn(2, L"¹İÇ° ³¯Â¥", LVCFMT_CENTER, 150);
			m_list->InsertColumn(3, L"¹İÇ° ¼ö·®", LVCFMT_CENTER, 150);


			while (!recSet.IsEOF())
			{
				recSet.GetFieldValue(_T("RETURN_NUM"), ret_code);
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
				else if(ret_code.Right(1) == '1') {

					// ¹İÇ° À¯ÇüÀÌ ÆÇ¸Å ¹İÇ°
					nListitm = m_list->InsertItem(0, L"ÆÇ¸Å ¹İÇ°", 0);
				}
				else {

					// ¹İÇ° À¯ÇüÀÌ Ãë¼Ò ¹İÇ°
					nListitm = m_list->InsertItem(0, L"Ãë¼Ò ¹İÇ°", 0);
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
			// Sale Management


			// Setting default of list controller
			if (m_list->GetItemCount() > 0)
				DeleteContent(m_list);

			m_list->ModifyStyle(0, LVS_REPORT | LVS_SHOWSELALWAYS | LVS_SINGLESEL, 0);
			m_list->SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);


			// connect db instance to SALE TABLE
			recSet.Open(CRecordset::dynaset,
				L"select SALE_NUM, SUM(SALE_AMOUNT), SUM(SUM_PRICE) from SALE group by SALE_NUM order by SALE_NUM");


			// Initializing List Controller
			if (m_list->GetItemCount() > 0)
				DeleteContent(m_list);


			//List Controller¿¡¿­Ãß°¡ÇÏ±â
			//InsertColumn( FieldNumber , _T(" FieldName "), Title_Position_option , Size? , -1)
			m_list->InsertColumn(0, _T("ÆÇ¸Å ¹øÈ£"), LVCFMT_CENTER, 250, -1);
			m_list->InsertColumn(1, _T("ÃÑ ÆÇ¸Å °³¼ö"), LVCFMT_CENTER, 250, -1);
			m_list->InsertColumn(2, _T("ÆÇ¸Å ³¯Â¥"), LVCFMT_CENTER, 250, -1);
			m_list->InsertColumn(3, _T("ÃÑ ÆÇ¸Å ±İ¾×"), LVCFMT_CENTER, 250, -1);



			//recSet¿¡ÇØ´çÅ×ÀÌºí³»¿ëÀÌ´ã°ÜÁ®ÀÖÀ½, Æ÷ÀÎÅÍ¸¦ÅëÇØ´ÙÀ½·¹ÄÚµå(Çà´ÜÀ§)·ÎÀÌµ¿
			//¸¶Áö¸·¿¡µµ´ŞÇÒ¶§±îÁö·¹ÄÚµå¸¦ÀĞÀºÈÄ, °¢ÇÊµå¸¶´ÙiÇàj¿­·ÎÇÊµå¼ÂÆÃ

			int idx = 0;

			while (!recSet.IsEOF()) {
				CString sale_num, total_amount, total_price;
				CString sale_date;

				recSet.GetFieldValue(_T("SALE_NUM"), sale_num);
				recSet.GetFieldValue(_T("SUM(SALE_AMOUNT)"), total_amount);
				recSet.GetFieldValue(_T("SUM(SUM_PRICE)"), total_price);

				sale_date = sale_num.Left(8);

				int LinsertIdx = m_list->InsertItem(0, sale_num, 0);
				m_list->SetItem(LinsertIdx, 0, LVIF_TEXT, sale_num, 0, 0, 0, NULL);
				m_list->SetItem(LinsertIdx, 1, LVIF_TEXT, total_amount, 0, 0, 0, NULL);
				m_list->SetItem(LinsertIdx, 2, LVIF_TEXT, sale_date, 0, 0, 0, NULL);
				m_list->SetItem(LinsertIdx, 3, LVIF_TEXT, total_price, 0, 0, 0, NULL);

				idx++;
				recSet.MoveNext();
			}

			recSet.Close();
			m_list->InsertItem(idx, L"+ ÆÇ¸Å ½ÇÇà", 0);

			//sales_view = new CSalesView();
			//sales_view->Create(NULL, NULL, WS_CHILD, CRect(0,0,0,0), pSender, 200, this);
			break;
		}
		case 4:
		{
			manageasset = new ManageAsset();
			manageasset->Create(ManageAsset::IDD);
			manageasset->ShowWindow(SW_SHOW);


			break;
		}
		case 6:
		{
			CString EMP_NUM, EMP_RANK, EMP_NAME;
			int idx = 0;

			// Set the style of Return list
			m_list->ModifyStyle(0, LVS_REPORT | LVS_SHOWSELALWAYS | LVS_SINGLESEL, 0);
			m_list->SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

			// ?”ë©´ ?´ë¦¬??
			if (m_list->GetItemCount() > 0)
				DeleteContent(m_list);

			recSet.Open(CRecordset::dynaset, L"select distinct EMP_NUM, RANK_SORT_CODE, NAME from EMPLOYEE WHERE RANK_SORT_CODE IN ('»çÀå', '¸Å´ÏÀú', '¾ß°£Á÷¿ø', 'ÁÖ°£Á÷¿ø') ORDER BY EMP_NUM DESC");

			m_list->InsertColumn(0, L"ÇöÀç Á÷¿ø ¹øÈ£", LVCFMT_CENTER, 200);
			m_list->InsertColumn(1, L"Á÷±Ş", LVCFMT_CENTER, 200);
			m_list->InsertColumn(2, L"ÀÌ¸§", LVCFMT_CENTER, 200);

			while (!recSet.IsEOF())
			{
				recSet.GetFieldValue(_T("EMP_NUM"), EMP_NUM);
				recSet.GetFieldValue(_T("RANK_SORT_CODE"), EMP_RANK);
				recSet.GetFieldValue(_T("NAME"), EMP_NAME);


				// Insert itm into list
				int nListitm = m_list->InsertItem(0, EMP_NUM, 0);
				m_list->SetItem(nListitm, 1, LVFIF_TEXT, EMP_RANK, 0, 0, 0, NULL);
				m_list->SetItem(nListitm, 2, LVFIF_TEXT, EMP_NAME, 0, 0, 0, NULL);

				idx++;
				recSet.MoveNext();
			}

			recSet.Close();

			if (SYSTEM_USER == USER_MANAGER)
			{
				m_list->InsertItem(idx, L"+ »õ Á÷¿ø Ãß°¡ÇÏ±â", 0);
				idx++;
				m_list->InsertItem(idx, L"+ ¿ù±Ş Áö±Ş ÇÏ±â", 0);
				idx++;
			}
			break;
		}
		case 7:
		{
			if (SYSTEM_USER == USER_MANAGER)
			{
				CString EMP_NUM, EMP_RANK, EMP_NAME;
				int idx = 0;

				// Set the style of Return list
				m_list->ModifyStyle(0, LVS_REPORT | LVS_SHOWSELALWAYS | LVS_SINGLESEL, 0);
				m_list->SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

				// ?”ë©´ ?´ë¦¬??
				if (m_list->GetItemCount() > 0)
					DeleteContent(m_list);

				recSet.Open(CRecordset::dynaset, L"select distinct  EMP_NUM, RANK_SORT_CODE, NAME from EMPLOYEE ORDER BY EMP_NUM DESC");

				m_list->InsertColumn(0, L"ÇöÀç Á÷¿ø ¹øÈ£", LVCFMT_CENTER, 200);
				m_list->InsertColumn(1, L"Á÷±Ş", LVCFMT_CENTER, 200);
				m_list->InsertColumn(2, L"ÀÌ¸§", LVCFMT_CENTER, 200);

				while (!recSet.IsEOF())
				{
					recSet.GetFieldValue(_T("EMP_NUM"), EMP_NUM);
					recSet.GetFieldValue(_T("RANK_SORT_CODE"), EMP_RANK);
					recSet.GetFieldValue(_T("NAME"), EMP_NAME);


					// Insert itm into list
					int nListitm = m_list->InsertItem(0, EMP_NUM, 0);
					m_list->SetItem(nListitm, 1, LVFIF_TEXT, EMP_RANK, 0, 0, 0, NULL);
					m_list->SetItem(nListitm, 2, LVFIF_TEXT, EMP_NAME, 0, 0, 0, NULL);

					idx++;
					recSet.MoveNext();
				}

				break;
			}
			else
			{
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
		case 8:
		{
			CString PROD_NUM, PROD_NAME, PROD_MAKER, PROD_STOCK_AMOUNT, EVENT_NUM;
			int idx = 0;

			// Set the style of Return list
			m_list->ModifyStyle(0, LVS_REPORT | LVS_SHOWSELALWAYS | LVS_SINGLESEL, 0);
			m_list->SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

			// ?”ë©´ ?´ë¦¬??
			if (m_list->GetItemCount() > 0)
				DeleteContent(m_list);

			recSet.Open(CRecordset::dynaset, L"SELECT PROD_NUM, PROD_NAME, PROD_MAKER, EVENT_NUM, PROD_STOCK_AMOUNT FROM PRODUCT ORDER BY PROD_NUM");

			m_list->InsertColumn(0, L"»óÇ° ÄÚµå", LVCFMT_CENTER, 200);
			m_list->InsertColumn(1, L"»óÇ° ÀÌ¸§", LVCFMT_CENTER, 150);
			m_list->InsertColumn(2, L"Á¦Á¶»ç", LVCFMT_CENTER, 100);
			m_list->InsertColumn(3, L"Àç°í", LVCFMT_CENTER, 100);
			m_list->InsertColumn(4, L"ÀÌº¥Æ® ¿©ºÎ", LVCFMT_CENTER, 100);

			while (!recSet.IsEOF())
			{
				recSet.GetFieldValue(_T("PROD_NUM"), PROD_NUM);
				recSet.GetFieldValue(_T("PROD_NAME"), PROD_NAME);
				recSet.GetFieldValue(_T("PROD_MAKER"), PROD_MAKER);
				recSet.GetFieldValue(_T("EVENT_NUM"), EVENT_NUM);
				recSet.GetFieldValue(_T("PROD_STOCK_AMOUNT"), PROD_STOCK_AMOUNT);


				// Insert itm into list
				int nListitm = m_list->InsertItem(0, PROD_NUM, 0);
				m_list->SetItem(nListitm, 1, LVFIF_TEXT, PROD_NAME, 0, 0, 0, NULL);
				m_list->SetItem(nListitm, 2, LVFIF_TEXT, PROD_MAKER, 0, 0, 0, NULL);
				m_list->SetItem(nListitm, 3, LVFIF_TEXT, PROD_STOCK_AMOUNT, 0, 0, 0, NULL);

				if (EVENT_NUM.IsEmpty())
					EVENT_NUM.Format(L"N");
				m_list->SetItem(nListitm, 4, LVFIF_TEXT, EVENT_NUM, 0, 0, 0, NULL);

				idx++;
				recSet.MoveNext();
			}

			recSet.Close();

			m_list->InsertItem(idx, L"+ »õ ¹°Ç° Ãß°¡ÇÏ±â", 0);
			idx++;

			if (SYSTEM_USER == USER_MANAGER)
			{
				m_list->InsertItem(idx, L"+ ÀÌº¥Æ® ¸ñ·Ï °ü¸®", 0);
				idx++;
				m_list->InsertItem(idx, L"+ ±¸ºĞÄÚµå ¸ñ·Ï °ü¸®", 0);
				idx++;
			}
			break;
		}
		case 9:
		{
			// Æó±â °ü¸®
			CString date, prod_num, amount;
			int idx = 0;

			// Set the style of Return list
			m_list->ModifyStyle(0, LVS_REPORT | LVS_SHOWSELALWAYS | LVS_SINGLESEL, 0);
			m_list->SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

			// ?”ë©´ ?´ë¦¬??
			if (m_list->GetItemCount() > 0)
				DeleteContent(m_list);

			recSet.Open(CRecordset::dynaset, L"SELECT * FROM DISUSE ORDER BY DISUSE_DATE");

			m_list->InsertColumn(0, L"Æó±â ³¯Â¥", LVCFMT_CENTER, 200);
			m_list->InsertColumn(1, L"»óÇ° ¹øÈ£", LVCFMT_CENTER, 150);
			m_list->InsertColumn(2, L"¼ö·®", LVCFMT_CENTER, 100);

			while (!recSet.IsEOF())
			{
				recSet.GetFieldValue(_T("DISUSE_DATE"), date);
				recSet.GetFieldValue(_T("PROD_NUM"), prod_num);
				recSet.GetFieldValue(_T("DISUSE_AMOUNT"), amount);


				// Insert itm into list
				int nListitm = m_list->InsertItem(0, date, 0);
				m_list->SetItem(nListitm, 1, LVFIF_TEXT, prod_num, 0, 0, 0, NULL);
				m_list->SetItem(nListitm, 2, LVFIF_TEXT, amount, 0, 0, 0, NULL);

				idx++;
				recSet.MoveNext();
			}

			recSet.Close();
			m_list->InsertItem(idx, L"+ »õ Æó±â Ãß°¡ÇÏ±â", 0);


			break;
		}
		case 10:
		{
			// È¸¿ø °ü¸®
			// Show member list

			CString member_code, name, phone, mileage;
			int idx = 0;

			if (m_list->GetItemCount() > 0)
				DeleteContent(m_list); // If there are any left contents on the pane, delete all


			m_list->ModifyStyle(0, LVS_REPORT | LVS_SHOWSELALWAYS | LVS_SINGLESEL, 0);
			m_list->SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

			// Get the distinct order code and total sum of an amount of the products of the order
			recSet.Open(CRecordset::dynaset, L"SELECT MEMBER_NUM, NAME, PHONE, MILEAGE FROM MEMBER ORDER BY NAME");

			m_list->InsertColumn(0, L"ÀÌ¸§", LVCFMT_CENTER, 150);
			m_list->InsertColumn(1, L"È¸¿ø ÄÚµå", LVCFMT_CENTER, 150);
			m_list->InsertColumn(2, L"ÀüÈ­ ¹øÈ£", LVCFMT_CENTER, 150);
			m_list->InsertColumn(3, L"¸¶ÀÏ¸®Áö", LVCFMT_CENTER, 150);

			while (!recSet.IsEOF())
			{
				recSet.GetFieldValue(_T("MEMBER_NUM"), member_code);
				recSet.GetFieldValue(_T("NAME"), name);
				recSet.GetFieldValue(_T("MILEAGE"), mileage);
				recSet.GetFieldValue(_T("PHONE"), phone);

				// Insert itm into list
				int nListitm = m_list->InsertItem(0, name, 0);
				m_list->SetItem(nListitm, 1, LVFIF_TEXT, member_code, 0, 0, 0, NULL);
				m_list->SetItem(nListitm, 2, LVFIF_TEXT, phone, 0, 0, 0, NULL);
				m_list->SetItem(nListitm, 3, LVFIF_TEXT, mileage, 0, 0, 0, NULL);

				recSet.MoveNext();
				idx++;
			}

			recSet.Close();
			m_list->InsertItem(idx, L"+ »õ È¸¿øÃß°¡", 0);

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
				CUR_CODE = order_code;

				if (order_num == "+ »õ ÁÖ¹®ÇÏ±â")
				{
					// Open dialog for new order
					dlg_new_order = new NewOrder();
					dlg_new_order->Create(NewOrder::IDD);
					dlg_new_order->ShowWindow(SW_SHOW);

					break;
				}

				CString SQL, reorder_val;
				SQL.Format(L"select distinct reorder_num from order_list where order_num='%s'", order_code);
				recSet.Open(CRecordset::dynaset, SQL);
				recSet.GetFieldValue(_T("REORDER_NUM"), reorder_val);

				if (reorder_val != L"")
				{
					// ÀçÁÖ¹®ÀÌ ÀÏ¾î³­ ¿ø·¡ ÁÖ¹®ÀÇ ÄÚµå°¡ µé¾î°¡ ÀÖ°Å³ª È®Á¤µÈ ¾ö¸¶ ·¹ÄÚµå·Î¼­ Y or NÀÌ µé¾î°¡ ÀÖ´Â °æ¿ì
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
			//ÆÇ¸ÅÇÏ±â ¹öÆ°À» ´­·¶À» ¶§¿Í ÆÇ¸ÅÄÚµå ÇàÀ» ´õºíÅ¬¸¯ÇßÀ» ¶§ ´Ù¸£°Ô ±¸¼º
			if (pNMItemActivate->iItem != -1)
			{
				// ¼±ÅÃµÈ ¾ÆÀÌÅÛÀÇ ÀÎµ¦½º ¹øÈ£¸¦ ¾ò´Â´Ù
				NM_LISTVIEW * pNMListView = (NM_LISTVIEW*) pNMHDR;
				int cur_idx = pNMListView->iItem;

				CString sale_num, total_amount, total_price;
				CString sale_date;

				sale_num = m_list->GetItemText(cur_idx, 0);
				total_amount = m_list->GetItemText(cur_idx, 1);
				sale_date = m_list->GetItemText(cur_idx, 2);
				total_price = m_list->GetItemText(cur_idx, 3);


				if (sale_num == "+ ÆÇ¸Å ½ÇÇà")
				{
					// Open dialog for new order
					dlg_new_sale = new NewSale();
					dlg_new_sale->Create(NewSale::IDD);
					dlg_new_sale->ShowWindow(SW_SHOW);
					break;
				}

				// Open dialog using currently selected index num
				dlg_sale_detail = new SaleDetail(this, sale_num);
				dlg_sale_detail->Create(SaleDetail::IDD);
				dlg_sale_detail->ShowWindow(SW_SHOW);

			}


			break;

		}
		case 4:
		{
			
		}
		case 6:
		{
			if (pNMItemActivate->iItem != -1)
			{
				// ¼±ÅÃµÈ ¾ÆÀÌÅÛÀÇ ÀÎµ¦½º ¹øÈ£¸¦ ¾ò´Â´Ù
				NM_LISTVIEW * pNMListView = (NM_LISTVIEW*) pNMHDR;
				int cur_idx = pNMListView->iItem;

				CString EMP_CODE, EMP_RANK, EMP_NAME;
				EMP_CODE = m_list->GetItemText(cur_idx, 0);
				EMP_RANK = m_list->GetItemText(cur_idx, 1);
				EMP_NAME = m_list->GetItemText(cur_idx, 2);

				if (EMP_CODE == "+ »õ Á÷¿ø Ãß°¡ÇÏ±â")
				{
					// Open dialog for new order
					dlg_new_emp = new NewEmp();
					dlg_new_emp->Create(NewEmp::IDD);
					dlg_new_emp->ShowWindow(SW_SHOW);
					break;
				}
				else if (EMP_CODE == "+ ¿ù±Ş Áö±Ş ÇÏ±â")
				{
					// Open dialog for new order
					dlg_emp_salary = new EmpSalary();
					dlg_emp_salary->Create(EmpSalary::IDD);
					dlg_emp_salary->ShowWindow(SW_SHOW);
					break;
				}

				// Open dialog using currently selected index num
				dlg_info_emp = new InfoEmp(this, EMP_CODE);
				dlg_info_emp->Create(InfoEmp::IDD);
				dlg_info_emp->ShowWindow(SW_SHOW);
			}
			else {}
			break;
		}
		case 7:
		{

			if (pNMItemActivate->iItem != -1)
			{
				if (SYSTEM_USER == USER_MANAGER)
				{
					// ¼±ÅÃµÈ ¾ÆÀÌÅÛÀÇ ÀÎµ¦½º ¹øÈ£¸¦ ¾ò´Â´Ù
					NM_LISTVIEW * pNMListView = (NM_LISTVIEW*) pNMHDR;
					int cur_idx = pNMListView->iItem;

					CString EMP_CODE, EMP_RANK, EMP_NAME;
					EMP_CODE = m_list->GetItemText(cur_idx, 0);
					EMP_RANK = m_list->GetItemText(cur_idx, 1);
					EMP_NAME = m_list->GetItemText(cur_idx, 2);

					// Open dialog using currently selected index num
					dlg_info_emp = new InfoEmp(this, EMP_CODE);
					dlg_info_emp->Create(InfoEmp::IDD);
					dlg_info_emp->ShowWindow(SW_SHOW);
				}
				else
				{
					
					break;
				}
			}
			else {}
			break;
		}
		case 8:
		{
			if (pNMItemActivate->iItem != -1)
			{
				// ¼±ÅÃµÈ ¾ÆÀÌÅÛÀÇ ÀÎµ¦½º ¹øÈ£¸¦ ¾ò´Â´Ù
				NM_LISTVIEW * pNMListView = (NM_LISTVIEW*) pNMHDR;
				int cur_idx = pNMListView->iItem;

				CString PROD_CODE, PROD_NAME, PROD_MAKER, PROD_STOCK_AMOUNT, PROD_EVENT_CODE;
				PROD_CODE = m_list->GetItemText(cur_idx, 0);
				PROD_NAME = m_list->GetItemText(cur_idx, 1);
				PROD_MAKER = m_list->GetItemText(cur_idx, 2);
				PROD_STOCK_AMOUNT = m_list->GetItemText(cur_idx, 3);
				PROD_EVENT_CODE = m_list->GetItemText(cur_idx, 4);

				if (PROD_CODE == "+ »õ ¹°Ç° Ãß°¡ÇÏ±â")
				{
					dlg_new_prod = new NewProd();
					dlg_new_prod->Create(NewProd::IDD);
					dlg_new_prod->ShowWindow(SW_SHOW);
					break;
				}
				else if (PROD_CODE == "+ ÀÌº¥Æ® ¸ñ·Ï °ü¸®")
				{
					dlg_event_detail = new EventDetail(this, PROD_CODE);
					dlg_event_detail->Create(EventDetail::IDD);
					dlg_event_detail->ShowWindow(SW_SHOW);
					break;
				}
				else if (PROD_CODE == "+ ±¸ºĞÄÚµå ¸ñ·Ï °ü¸®")
				{
					dlg_code_list = new CodeList();
					dlg_code_list->Create(CodeList::IDD);
					dlg_code_list->ShowWindow(SW_SHOW);
					break;
				}

				// Open dialog using currently selected index num
				dlg_info_prod = new InfoProd(this, PROD_CODE);
				dlg_info_prod->Create(InfoProd::IDD);
				dlg_info_prod->ShowWindow(SW_SHOW);

			}
			else {}
			break;
		}
		case 9:
		{
			if (pNMItemActivate->iItem != -1)
			{
				// ¼±ÅÃµÈ ¾ÆÀÌÅÛÀÇ ÀÎµ¦½º ¹øÈ£¸¦ ¾ò´Â´Ù
				NM_LISTVIEW * pNMListView = (NM_LISTVIEW*) pNMHDR;
				int cur_idx = pNMListView->iItem;

				CString date, prod_num;
				date = m_list->GetItemText(cur_idx, 0);
				prod_num = m_list->GetItemText(cur_idx, 1);

				if (date == "+ »õ Æó±â Ãß°¡ÇÏ±â")
				{
					dlg_new_disuse = new NewDisuse();
					dlg_new_disuse->Create(NewDisuse::IDD);
					dlg_new_disuse->ShowWindow(SW_SHOW);
					break;
				}

				// Open dialog using currently selected index num
				dig_info_disuse = new InfoDisuse(this, date, prod_num);
				dig_info_disuse->Create(InfoDisuse::IDD);
				dig_info_disuse->ShowWindow(SW_SHOW);

			}
			else {}
			break;
		}
		case 10:
		{
			if (pNMItemActivate->iItem != -1)
			{
				// ¼±ÅÃµÈ ¾ÆÀÌÅÛÀÇ ÀÎµ¦½º ¹øÈ£¸¦ ¾ò´Â´Ù
				NM_LISTVIEW * pNMListView = (NM_LISTVIEW*) pNMHDR;
				int cur_idx = pNMListView->iItem;

				CString CUSTOMER_CODE;
				CUSTOMER_CODE = m_list->GetItemText(cur_idx, 0);
				CString mem_cnt = m_list->GetItemText(cur_idx - 1, 1);
				int _mem_cnt = _ttoi(mem_cnt);
				_mem_cnt++;

				if (CUSTOMER_CODE == L"+ »õ È¸¿øÃß°¡")
				{
					// Open dialog for new order
					dlg_customer = new CNewCustomer(_mem_cnt, this);
					dlg_customer->Create(CNewCustomer::IDD);
					dlg_customer->ShowWindow(SW_SHOW);
					break;
				}
			}
			else {}
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

	*pResult = 0;
}


void CContentView::OnContextMenu(CWnd* /*pWnd*/, CPoint point)
{
	// TODO: ¿©±â¿¡ ¸Ş½ÃÁö Ã³¸®±â ÄÚµå¸¦ Ãß°¡ÇÕ´Ï´Ù.
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
	// TODO: ¿©±â¿¡ ¸í·É Ã³¸®±â ÄÚµå¸¦ Ãß°¡ÇÕ´Ï´Ù.

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

	CString SQL, price;
	SQL.Format(L"DELETE FROM ORDER_LIST WHERE order_num = '%s'", CUR_CODE);
	db_content.ExecuteSQL(SQL);

	CRecordset recSet(&db_content);
	SQL.Format(L"SELECT PRICE FROM FUNDS WHERE FUNDS_DETAIL_NUM = '%s'AND FUNDS_SORT_CODE = 'F05'", CUR_CODE);
	recSet.Open(CRecordset::dynaset, SQL);
	recSet.GetFieldValue(_T("PRICE"), price);
	
	CTime cTime = CTime::GetCurrentTime();
	CString funds_date, today;
	funds_date.Format(L"%04d%02d%02d%02d%02d%02d", cTime.GetYear(), cTime.GetMonth(), cTime.GetDay(), cTime.GetHour(), cTime.GetMinute(), cTime.GetSecond());

	SQL.Format(L"INSERT INTO FUNDS(FUNDS_DATE, FUNDS_DETAIL_NUM, PRICE, FUNDS_SORT_CODE) values ('%s', '%s', '-%s', 'F04')", funds_date, CUR_CODE, price);
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

	CString SQL, tmp, NEW_CODE;
	tmp = CUR_CODE.Left(14);
	NEW_CODE.Format(L"%s2", tmp);
	SQL.Format(L"UPDATE RETURN SET DETAIL = 'Ãë¼ÒµÈ ¹İÇ°', RETURN_NUM = '%s' WHERE return_num = '%s'", NEW_CODE, CUR_CODE);
	//MessageBox(SQL);
	db_content.ExecuteSQL(SQL);
	// TODO: ¿©±â¿¡ ¸í·É Ã³¸®±â ÄÚµå¸¦ Ãß°¡ÇÕ´Ï´Ù.
}


void CContentView::OnNMClick(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>( pNMHDR );
	// TODO: ¿©±â¿¡ ÄÁÆ®·Ñ ¾Ë¸² Ã³¸®±â ÄÚµå¸¦ Ãß°¡ÇÕ´Ï´Ù.

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
				CUR_CODE = order_code;

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
			CUR_CODE = ret_code;


		}
		else
		{

		}
		break;
	}

	default:
		break;
	}

	*pResult = 0;
}
