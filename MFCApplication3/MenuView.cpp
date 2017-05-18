// MenuView.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MFCApplication3.h"
#include "MenuView.h"
#include "ContentView.h"

#define MENU_ITEM_COUNT 17

// CMenuView

IMPLEMENT_DYNCREATE(CMenuView, CTreeView)

CMenuView::CMenuView()
{

}

CMenuView::~CMenuView()
{
}

BEGIN_MESSAGE_MAP(CMenuView, CTreeView)
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, &CMenuView::OnTvnSelchanged)
END_MESSAGE_MAP()


// CMenuView 진단입니다.

#ifdef _DEBUG


void CMenuView::AssertValid() const
{
	CTreeView::AssertValid();
}

#ifndef _WIN32_WCE
void CMenuView::Dump(CDumpContext& dc) const
{
	CTreeView::Dump(dc);
}
#endif
#endif //_DEBUG


// CMenuView 메시지 처리기입니다.

void CMenuView::SaveMenuState(CDatabase & db_menu)
{
	CRecordset recSet(&db_menu);
	recSet.Open(CRecordset::dynaset, L"select name, is_last from menu_list");
	int idx = 0;

	while (!recSet.IsEOF())
	{
		recSet.GetFieldValue(_T("NAME"), menu_board[idx][0]);
		recSet.GetFieldValue(_T("IS_LAST"), menu_board[idx][1]);
		recSet.MoveNext();
	}

	recSet.Close();
}

void CMenuView::SetMenu(CDatabase & db_menu)
{

	//트리를 만든다
	CRecordset recSet(&db_menu);
	HTREEITEM  htItem[MENU_ITEM_COUNT];
	
	htItem[0] = GetTreeCtrl().InsertItem(L"UOS25 편의점 관리 프로그램", 0, 0, TVI_ROOT);
	htItem[1] = GetTreeCtrl().InsertItem(L"주문 관리", 0, 0, htItem[0], TVI_LAST);
	htItem[2] = GetTreeCtrl().InsertItem(L"반품 관리", 0, 0, htItem[0], TVI_LAST);
	htItem[3] = GetTreeCtrl().InsertItem(L"판매 관리", 0, 0, htItem[0], TVI_LAST);
	htItem[4] = GetTreeCtrl().InsertItem(L"자금 관리", 0, 0, htItem[0], TVI_LAST);
	htItem[5] = GetTreeCtrl().InsertItem(L"직원 관리", 0, 0, htItem[0]);
	htItem[6] = GetTreeCtrl().InsertItem(L"현재 직원", 0, 0, htItem[5], TVI_LAST);
	htItem[7] = GetTreeCtrl().InsertItem(L"직원 이력", 0, 0, htItem[5], TVI_LAST);
	htItem[8] = GetTreeCtrl().InsertItem(L"물품 관리", 0, 0, htItem[0], TVI_LAST);

	
	//트리를 펼친다
	for (int i = 0; i < 9; i++)
		GetTreeCtrl().Expand(htItem[i], TVE_EXPAND);
	
	return;
}

BOOL CMenuView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	
	/*
		PreCreateWindow() 함수는 윈도우 창 생성 전에 시행된다.
		이때 파라미터 cs를 이용해 해당 윈도우의 스타일을 지정해준다.
		CTreeView를 상속받는 클래스인 CMenuView의 스타일은
		아래와 같이 지정해줄 수 있다.
	*/

	cs.style |= TVS_HASLINES | TVS_SHOWSELALWAYS | TVS_DISABLEDRAGDROP;

	return CTreeView::PreCreateWindow(cs);
}


void CMenuView::OnInitialUpdate()
{
	CTreeView::OnInitialUpdate();
	
	/*
		도큐먼트가 열리거나 생성될 때 호출되는 함수로 뷰 객체를 초기화한다.
		Document의 OnUpdate()함수를 호출하여 뷰의 클라이언트 영역을 다시 그리도록 한다.
	
	*/
	// 메뉴를 표시하고 사용한 디비는 닫아준다

	SetMenu(db_menu);

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
}

/*
	CMenuView의 트리 아이템 변화에 따른 CRightContainerView의 변화
	
	트리 아이템의 선택 여부가 변화하면 TVN_SELCHANGED 메시지가 발생한다.
	이때 발생하는 메세지는 아래의 OnTvnSelchanged 함수가 다루게 된다.
	이 함수에서 도큐먼트의 UpdateAllViews 함수를 호출하여
	CRightContainerView 클래스의 OnUpdate 함수에게 리스트뷰가 갱신되어야 함을 알린 후
	변경된 데이터를 리스트뷰에 보여준다.
*/
void CMenuView::OnTvnSelchanged(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>( pNMHDR );
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	// 선택된 아이템을 selectedItem 객체에 저장
	HTREEITEM selectedItem = pNMTreeView->itemNew.hItem;

	// 선택된 아이템의 이름을 얻는다
	CString selectedItem_name = GetTreeCtrl().GetItemText(selectedItem);
	CString menu_board[8][2] = {
		L"주문 관리",L"1",  L"반품 관리", L"1",L"판매 관리" ,L"1",L"자금 관리",L"1",
		L"직원 관리",L"0", L"현재 직원", L"1",L"직원 이력" ,L"1",L"물품 관리", L"1"
	};
	
	if (selectedItem_name == L"UOS25 편의점 관리 프로그램")
		GetDocument()->UpdateAllViews(this, 0, (CObject*) (LPCTSTR) selectedItem_name);


	CString is_last;

	for (int i = 0; i < 8; i++)
	{
		if (selectedItem_name == menu_board[i][0] && menu_board[i][1] == '1')
		{
			// 트리의 맨 마지막 계층 아이템을 선택한 경우에만 view를 업데이트한다
			GetDocument()->UpdateAllViews(this, i + 1, (CObject*) (LPCTSTR) selectedItem_name);

			break;
		}
	}

	*pResult = 0;
}
