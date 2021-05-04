// MovesBox.cpp : implementation file
//

#include "stdafx.h"
#include "SeaBattle.h"
#include "MovesBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMovesBox

CMovesBox::CMovesBox()
{
}

CMovesBox::~CMovesBox()
{
}


BEGIN_MESSAGE_MAP(CMovesBox, CListCtrl)
	//{{AFX_MSG_MAP(CMovesBox)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMovesBox message handlers

int CMovesBox::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CListCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
	return 0;
}

void CMovesBox::Init(CWnd *pParent)
{
//	CDC *dc;
	RECT rect;

	m_Parent = pParent;
	GetClientRect(&rect);
//	dc = GetDC();
//	m_FonDC.CreateCompatibleDC(dc);
//	m_bmp.CreateCompatibleBitmap(dc, 150, 300);
//	pOld = m_FonDC.SelectObject(&m_bmp);
//	::SendMessage(m_Parent->m_hWnd, WM_STATIC_TO_DLG, WM_FILL_LIST_DC, (LPARAM)&m_FonDC);
	SetBkColor(RGB(25, 89, 127));
	SetTextBkColor(RGB(25, 89, 127));
//	ReleaseDC(dc);
}

//DEL void CMovesBox::OnPaint() 
//DEL {
//DEL 	CPaintDC dc(this); // device context for painting
//DEL 	dc.BitBlt(dc.m_ps.rcPaint.left, dc.m_ps.rcPaint.top,
//DEL 		dc.m_ps.rcPaint.right-dc.m_ps.rcPaint.left,
//DEL 		dc.m_ps.rcPaint.bottom - dc.m_ps.rcPaint.top,
//DEL 		&m_FonDC, dc.m_ps.rcPaint.left,
//DEL 		dc.m_ps.rcPaint.top, SRCCOPY);
//DEL }

void CMovesBox::OnDestroy() 
{
//	m_FonDC.SelectObject(pOld);
//	m_FonDC.DeleteDC();
//	m_bmp.DeleteObject();
	CListCtrl::OnDestroy();
}

void CMovesBox::OnLButtonDown(UINT nFlags, CPoint point) 
{
	
	CListCtrl::OnLButtonDown(nFlags, point);
}
