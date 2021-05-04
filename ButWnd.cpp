// ButWnd.cpp : implementation file
//

#include "stdafx.h"
#include "SeaBattle.h"
#include "ButWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CButWnd

CButWnd::CButWnd()
{
	mode = BT_NONE;
	IsFocus = false;
	LastPoint.x = LastPoint.y = -1;
}

CButWnd::~CButWnd()
{
}


BEGIN_MESSAGE_MAP(CButWnd, CWnd)
	//{{AFX_MSG_MAP(CButWnd)
	ON_WM_KILLFOCUS()
	ON_WM_SETFOCUS()
	ON_WM_LBUTTONDOWN()
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CButWnd message handlers

void CButWnd::OnKillFocus(CWnd* pNewWnd) 
{
	CWnd::OnKillFocus(pNewWnd);
	IsFocus = false;	
	Invalidate();
}

void CButWnd::OnSetFocus(CWnd* pOldWnd) 
{
	CWnd::OnSetFocus(pOldWnd);
	IsFocus = true;
	Invalidate();
}

void CButWnd::OnLButtonDown(UINT nFlags, CPoint point) 
{
	int but;
	switch(mode)
	{
	case BT_SHIPS:
		but = B_PLACEMENT;
		LastPoint.x = LastPoint.y = -1;
		break;
	case BT_NET:
		but = B_NETWORK;
		LastPoint.x = LastPoint.y = -1;
		break;
	case BT_SINGLE:
		but = B_SINGLE;
		LastPoint.x = LastPoint.y = -1;
		break;
	case BT_HELP:
		but = B_HELP;
		LastPoint.x = LastPoint.y = -1;
		break;
	}
	::SendMessage(m_Parent->m_hWnd, WM_STATIC_TO_DLG, WM_LBT_DOWN, but);
	Invalidate();
	CWnd::OnLButtonDown(nFlags, point);
}

void CButWnd::SetMode(int md, CWnd *parent)
{
	mode = md;
	m_Parent = parent;
	switch(md)
	{
	case BT_SHIPS:
		Pic.Load(JPG_BT_SHIPS);
		ActPic.Load(JPG_BT_ACT_SHIPS);
		break;
	case BT_NET:
		Pic.Load(JPG_BT_NETWORK);
		ActPic.Load(JPG_BT_ACT_NETWORK);
		break;
	case BT_SINGLE:
		Pic.Load(JPG_BT_SINGLE);
		ActPic.Load(JPG_BT_ACT_SINGLE);
		break;
	case BT_HELP:
		Pic.Load(JPG_BT_HELP);
		ActPic.Load(JPG_BT_ACT_HELP);
		break;
	}
}

void CButWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	RECT rect;
	GetClientRect(&rect);
	if(IsFocus)
		ActPic.Render(&dc, &rect);
	else
		Pic.Render(&dc, &rect);
}

void CButWnd::OnMouseMove(UINT nFlags, CPoint point) 
{
	RECT rect;
	int w, h;

	GetWindowRect(&rect);
	w = rect.right-rect.left;
	h = rect.bottom-rect.top;
	if(point.x >= 0 && point.y >= 0 && point.x <= w && point.y <= h)
	{
		if(LastPoint.x < 0 || LastPoint.y < 0 || LastPoint.x > w ||
			LastPoint.y > h)
		{
			SetCapture();
			IsFocus = true;
			Invalidate();
		}
	}
	else
	{
		if(LastPoint.x >= 0 && LastPoint.y >= 0 && LastPoint.x <= w &&
			LastPoint.y <= h)
		{
			ReleaseCapture();
			IsFocus = false;
			Invalidate();
		}
	}
	LastPoint = point;
	CWnd::OnMouseMove(nFlags, point);
}
