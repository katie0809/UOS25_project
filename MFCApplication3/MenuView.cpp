// MenuView.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "MFCApplication3.h"
#include "MenuView.h"
#include "ContentView.h"
#include "Login.h"
#define MENU_ITEM_COUNT 11

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


// CMenuView �����Դϴ�.

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


// CMenuView �޽��� ó�����Դϴ�.

void CMenuView::SetMenu(CDatabase & db_menu)
{


	//Ʈ���� �����
	HTREEITEM  htItem[MENU_ITEM_COUNT];
	
	htItem[0] = GetTreeCtrl().InsertItem(L"UOS25 ������ ���� ���α׷�", 0, 0, TVI_ROOT);
	htItem[1] = GetTreeCtrl().InsertItem(L"�ֹ� ����", 0, 0, htItem[0], TVI_LAST);
	htItem[2] = GetTreeCtrl().InsertItem(L"��ǰ ����", 0, 0, htItem[0], TVI_LAST);
	htItem[3] = GetTreeCtrl().InsertItem(L"�Ǹ� ����", 0, 0, htItem[0], TVI_LAST);
	htItem[4] = GetTreeCtrl().InsertItem(L"�ڱ� ����", 0, 0, htItem[0], TVI_LAST);
	htItem[5] = GetTreeCtrl().InsertItem(L"���� ����", 0, 0, htItem[0]);
	htItem[6] = GetTreeCtrl().InsertItem(L"���� ����", 0, 0, htItem[5], TVI_LAST);
	htItem[7] = GetTreeCtrl().InsertItem(L"���� �̷�", 0, 0, htItem[5], TVI_LAST);
	htItem[8] = GetTreeCtrl().InsertItem(L"��ǰ ����", 0, 0, htItem[0], TVI_LAST);
	htItem[9] = GetTreeCtrl().InsertItem(L"��� ����", 0, 0, htItem[8], TVI_LAST);
	htItem[10] = GetTreeCtrl().InsertItem(L"ȸ�� ����", 0, 0, htItem[0], TVI_LAST);
	htItem[11] = GetTreeCtrl().InsertItem(L"�α���", 0, 0, htItem[0], TVI_LAST);
	
	//Ʈ���� ��ģ��
	for (int i = 0; i < 11; i++)
		GetTreeCtrl().Expand(htItem[i], TVE_EXPAND);
	
	return;
}

BOOL CMenuView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	
	/*
		PreCreateWindow() �Լ��� ������ â ���� ���� ����ȴ�.
		�̶� �Ķ���� cs�� �̿��� �ش� �������� ��Ÿ���� �������ش�.
		CTreeView�� ��ӹ޴� Ŭ������ CMenuView�� ��Ÿ����
		�Ʒ��� ���� �������� �� �ִ�.
	*/

	cs.style |= TVS_HASLINES | TVS_SHOWSELALWAYS | TVS_DISABLEDRAGDROP;

	return CTreeView::PreCreateWindow(cs);
}


void CMenuView::OnInitialUpdate()
{
	CTreeView::OnInitialUpdate();
	
	/*
		��ť��Ʈ�� �����ų� ������ �� ȣ��Ǵ� �Լ��� �� ��ü�� �ʱ�ȭ�Ѵ�.
		Document�� OnUpdate()�Լ��� ȣ���Ͽ� ���� Ŭ���̾�Ʈ ������ �ٽ� �׸����� �Ѵ�.
	
	*/
	// �޴��� ǥ���ϰ� ����� ���� �ݾ��ش�

	SetMenu(db_menu);

	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
}

/*
	CMenuView�� Ʈ�� ������ ��ȭ�� ���� CRightContainerView�� ��ȭ
	
	Ʈ�� �������� ���� ���ΰ� ��ȭ�ϸ� TVN_SELCHANGED �޽����� �߻��Ѵ�.
	�̶� �߻��ϴ� �޼����� �Ʒ��� OnTvnSelchanged �Լ��� �ٷ�� �ȴ�.
	�� �Լ����� ��ť��Ʈ�� UpdateAllViews �Լ��� ȣ���Ͽ�
	CRightContainerView Ŭ������ OnUpdate �Լ����� ����Ʈ�䰡 ���ŵǾ�� ���� �˸� ��
	����� �����͸� ����Ʈ�信 �����ش�.
*/
void CMenuView::OnTvnSelchanged(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>( pNMHDR );
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	// ���õ� �������� selectedItem ��ü�� ����
	HTREEITEM selectedItem = pNMTreeView->itemNew.hItem;

	// ���õ� �������� �̸��� ��´�
	CString selectedItem_name = GetTreeCtrl().GetItemText(selectedItem);
	CString menu_board[11][2] = {
		L"�ֹ� ����",L"1",  L"��ǰ ����", L"1",L"�Ǹ� ����" ,L"1",L"�ڱ� ����",L"1",
		L"���� ����",L"0", L"���� ����", L"1",L"���� �̷�" ,L"1",L"��ǰ ����", L"1",
		L"��� ����", L"1", L"ȸ�� ����", L"1", L"�α���", L"1"
	};
	
	if (selectedItem_name == L"UOS25 ������ ���� ���α׷�")
		GetDocument()->UpdateAllViews(this, 0, (CObject*) (LPCTSTR) selectedItem_name);


	CString is_last;

	for (int i = 0; i < 10; i++)
	{
		if (selectedItem_name == menu_board[10][0])
		{
			// �α��� â�� ����
			// Open dialog for user login
			CLogin * dlg_login;

			dlg_login = new CLogin();
			dlg_login->Create(CLogin::IDD);
			dlg_login->ShowWindow(SW_SHOW);

			break;
		}
		if (selectedItem_name == menu_board[i][0] && menu_board[i][1] == '1')
		{
			// Ʈ���� �� ������ ���� �������� ������ ��쿡 �ش� view���� ������Ʈ�Ѵ�
			GetDocument()->UpdateAllViews(this, i + 1, (CObject*) (LPCTSTR) selectedItem_name);

			break;
		}
	}

	*pResult = 0;
}
