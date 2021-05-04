// MyButton.cpp : implementation file
//

#include "stdafx.h"
#include "SeaBattle.h"
#include "MyButton.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyButton

//states
#define ST_RESULTS 1000
#define ST_HISTORY 1001

CMyButton::CMyButton(): m_iState(ST_RESULTS)
{
}

CMyButton::~CMyButton()
{
}


BEGIN_MESSAGE_MAP(CMyButton, CButton)
	//{{AFX_MSG_MAP(CMyButton)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyButton message handlers

int CMyButton::GetState() const
{
	return m_iState;
}

void CMyButton::SetState(int state)
{
	m_iState = state;
}

void CMyButton::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	RECT rect;
	GetClientRect(&rect);
	dc.FillRect(&rect, &CBrush(RGB(35, 89, 127)));
	dc.SetBkColor(RGB(35, 89, 127));
	dc.MoveTo(0, 0);
	dc.LineTo(rect.right, 0);
	dc.LineTo(rect.right, rect.bottom);
	dc.LineTo(0, rect.bottom);
	dc.LineTo(0, 0);
	dc.SetTextAlign(TA_CENTER);
	dc.TextOut(rect.right/2, 2, "Результаты");
}


void CMyButton::Init(CWnd *pParent)
{
	m_pParent = pParent;
}

void CMyButton::OnLButtonDown(UINT nFlags, CPoint point) 
{
	::SendMessage(m_pParent->m_hWnd, WM_STATIC_TO_DLG, WM_LBUTTONDOWN_BUT, 0);
	CButton::OnLButtonDown(nFlags, point);
}
