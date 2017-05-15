// SalesView.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MFCApplication3.h"
#include "SalesView.h"


// CSalesView

IMPLEMENT_DYNCREATE(CSalesView, CView)

CSalesView::CSalesView()
{

}

CSalesView::~CSalesView()
{
}

BEGIN_MESSAGE_MAP(CSalesView, CView)
END_MESSAGE_MAP()


// CSalesView 그리기입니다.

void CSalesView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: 여기에 그리기 코드를 추가합니다.

}


// CSalesView 진단입니다.

#ifdef _DEBUG
void CSalesView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CSalesView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CSalesView 메시지 처리기입니다.
