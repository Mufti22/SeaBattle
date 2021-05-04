// Field.cpp : implementation file
//

#include "stdafx.h"
#include "SeaBattle.h"
#include "Field.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifdef CLIENTSERVER 
#define CLIENTSERVER
#include "ClientServer.h"
#endif

/////////////////////////////////////////////////////////////////////////////
// CField

CField::CField()
{
	Alpha = 200;
	m_Direction = RIGHT;
	m_Decks = 4;
	m_Position.x = m_Position.y = -1;
	m_Draw = false;
	m_Operation = NONE;
	m_Parent = NULL;
	for(int i=0; i<10; i++)
		for(int j=0; j<10; j++)
			m_Event[i][j] = EMPTY;
	m_Count = 0;
	KilledShips = 0;
	IsNet = 0;
	NetPoint.x = NetPoint.y = -1;
	m_GoodSteps = m_Steps = 0;
}

CField::~CField()
{
	WaterDC.DeleteDC();
	bmp.Detach();
}


BEGIN_MESSAGE_MAP(CField, CStatic)
	//{{AFX_MSG_MAP(CField)
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_CREATE()
	ON_WM_SETCURSOR()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////

// CField message handlers

void CField::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	int i, j;
	RECT rect;
	char *let = "abcdefghik";
	CPen pn1, *oldpn1;
	int count;
	CBrush br;

	GetClientRect(&rect);
	if(m_Mode == OWN_FIELD)
	{
		dc.BitBlt(dc.m_ps.rcPaint.left, dc.m_ps.rcPaint.top,
			dc.m_ps.rcPaint.right - dc.m_ps.rcPaint.left,
			dc.m_ps.rcPaint.bottom - dc.m_ps.rcPaint.top,
			&WaterDC, dc.m_ps.rcPaint.left,
			dc.m_ps.rcPaint.top, SRCCOPY);
		for(i=(dc.m_ps.rcPaint.top/20)*20; i<=(dc.m_ps.rcPaint.bottom/20)*20; i+=20)
		{
			dc.MoveTo(dc.m_ps.rcPaint.left, i);
			dc.LineTo(dc.m_ps.rcPaint.right, i);
		}
		for(i=(dc.m_ps.rcPaint.left/20)*20; i<=(dc.m_ps.rcPaint.right/20)*20; i+=20)
		{
			dc.MoveTo(i, dc.m_ps.rcPaint.top);
			dc.LineTo(i, dc.m_ps.rcPaint.bottom);
		}

		for(i=0; i<10; i++)
			for(j=0; j<10; j++)
			{
				count = 0;
				if(m_Event[i][j] == SHIP || m_Event[i][j] == SHIPWOUND
					|| m_Event[i][j] == SHIPKILLED)
				{
					if((m_Event[i][j+1] == SHIP || 
						m_Event[i][j+1] == SHIPWOUND ||
						m_Event[i][j+1] == SHIPKILLED) && j+1 < 10)
					{
						count = 1;
						do
						{
							count++;
						} while(
							(m_Event[i][j+count] == SHIP ||
							m_Event[i][j+count] == SHIPWOUND ||
							m_Event[i][j+count] == SHIPKILLED) 
							&& j+count<10);
						DrawShip(CRect(j*20, i*20, (j+count)*20, (i+1)*20), &dc);
						j+=count;
						continue;
					}
					else
					{
						if(
							(m_Event[i+1][j] == SHIP ||
							m_Event[i+1][j] == SHIPWOUND ||
							m_Event[i+1][j] == SHIPKILLED) 
							&& 
							((m_Event[i-1][j] != SHIP &&
							m_Event[i-1][j] != SHIPKILLED &&
							m_Event[i-1][j] != SHIPWOUND) || i-1<0)
							&& i+1<10)
						{
							count = 1;
							do
							{
								count++;
							} while(
								(m_Event[i+count][j] == SHIP ||
								m_Event[i+count][j] == SHIPWOUND ||
								m_Event[i+count][j] == SHIPKILLED)
								&& i+count<10);
							DrawShip(CRect(j*20, i*20, (j+1)*20, (i+count)*20), &dc);
							continue;
						}
						if(((m_Event[i+1][j] != SHIP &&
							m_Event[i+1][j] != SHIPKILLED &&
							m_Event[i+1][j] != SHIPWOUND) && i+1<10)||(i+1==10))
							if(((m_Event[i-1][j] != SHIP &&
								m_Event[i-1][j] != SHIPWOUND &&
								m_Event[i-1][j] != SHIPKILLED) && i-1 >= 0) || (i-1<0))
								if(((m_Event[i][j+1] != SHIP &&
									m_Event[i][j+1] != SHIPWOUND &&
									m_Event[i][j+1] != SHIPKILLED) && j+1<10) || (j+1==10))
									if(((m_Event[i][j-1] != SHIP &&
										m_Event[i][j-1] != SHIPWOUND &&
										m_Event[i][j-1] != SHIPKILLED) && j-1>=0) || (j-1<0))
										DrawShip(CRect(j*20, i*20, (j+1)*20, (i+1)*20), &dc);
					}
				}
			}
		if(m_Draw && m_Operation == PLACEMENT) 
			DrawShip(GetRect(), &dc);
	}

	GetClientRect(&rect);
	if(m_Mode == FOREIGN_FIELD)
	{
		dc.BitBlt(dc.m_ps.rcPaint.left, dc.m_ps.rcPaint.top,
			dc.m_ps.rcPaint.right - dc.m_ps.rcPaint.left,
			dc.m_ps.rcPaint.bottom - dc.m_ps.rcPaint.top,
			&WaterDC, dc.m_ps.rcPaint.left,
			dc.m_ps.rcPaint.top, SRCCOPY);
		for(i=(dc.m_ps.rcPaint.top/20)*20; i<=(dc.m_ps.rcPaint.bottom/20)*20; i+=20)
		{
			dc.MoveTo(dc.m_ps.rcPaint.left, i);
			dc.LineTo(dc.m_ps.rcPaint.right, i);
		}
		for(i=(dc.m_ps.rcPaint.left/20)*20; i<=(dc.m_ps.rcPaint.right/20)*20; i+=20)
		{
			dc.MoveTo(i, dc.m_ps.rcPaint.top);
			dc.LineTo(i, dc.m_ps.rcPaint.bottom);
		}
		for(i=0; i<10; i++)
			for(j=0; j<10; j++)
			{
				count = 0;
				if(m_Event[i][j] == SHIPKILLED)
				{
					if(m_Event[i][j+1] == SHIPKILLED && j+1<10)
					{
						count = 1;
						do
						{
							count++;
						} while(m_Event[i][j+count] == SHIPKILLED && j+count<10);
						DrawShip(CRect(j*20, i*20, (j+count)*20, (i+1)*20), &dc);
						j+=count;
						continue;
					}
					else
					{
						if(m_Event[i+1][j] == SHIPKILLED && 
							(m_Event[i-1][j] != SHIPKILLED || i-1==-1)
							&& i+1 <10)
						{
							count = 1;
							do
							{
								count++;
							} while(m_Event[i+count][j] == SHIPKILLED
								&& i+count<10);
							DrawShip(CRect(j*20, i*20, (j+1)*20, (i+count)*20), &dc);
							continue;
						}
						if((m_Event[i+1][j] != SHIPKILLED
							&& i+1<10)||(i+1==10))
							if((m_Event[i-1][j] != SHIPKILLED
								&& i-1 >= 0) || (i-1<0))
								if((m_Event[i][j+1] != SHIPKILLED
									&& j+1<10) || (j+1==10))
									if((m_Event[i][j-1] != SHIPKILLED
										&& j-1>=0) || (j-1<0))
										DrawShip(CRect(j*20, i*20, (j+1)*20, (i+1)*20), &dc);
					}
				}
			}
	}

	pn1.CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
	oldpn1 = dc.SelectObject(&pn1);
	br.CreateSolidBrush(RGB(22, 60, 84));
	for(i=0; i<10; i++)
		for(j=0; j<10; j++)
		{
			if(m_Event[i][j] == CANNOT)
			{
				dc.MoveTo(j*20, i*20);
				dc.LineTo((j+1)*20, (i+1)*20);
				dc.MoveTo(j*20, (i+1)*20);
				dc.LineTo((j+1)*20, i*20);
			}
			if(m_Event[i][j] == NEARKILLEDSHIP)
				dc.FillRect(CRect(j*20+1, i*20+1, (j+1)*20, (i+1)*20), &br);
			if(m_Event[i][j] == SHIPKILLED || m_Event[i][j] == SHIPWOUND)
				DrawObject(CRect(j*20, i*20, (j+1)*20, (i+1)*20), &dc, m_bFire);
		}
	dc.SelectObject(oldpn1);
	pn1.DeleteObject();
	br.DeleteObject();
	if(m_Mode == NUMBERS)
	{
		dc.SetBkMode(TRANSPARENT);
		dc.TextOut(0, 4, "  1   2   3   4   5   6   7   8   9  10");
	}
	if(m_Mode == LETTERS)
	{
		dc.SetBkMode(TRANSPARENT);
		for(i=0; i<rect.bottom-rect.top; i+=20)
			dc.TextOut(8, i+2, let[i/20]);
	}

}

void CField::SetMode(int mode)
{
	m_Mode = mode;
}

int CField::ReceiveMessage(int wParam, long lParam)
{
	int i, j;
	LISTDATA listdata;
	switch(wParam)
	{
	case N_MESSAGE:
		listdata.x = NetPoint.x;
		listdata.y = NetPoint.y;
		listdata.str = "ВЫ";	
		m_Steps++;
		if(lParam == N_EMPTY)
		{
			m_Event[NetPoint.y][NetPoint.x] = CANNOT;
			::SendMessage(m_Parent->m_hWnd, WM_STATIC_TO_DLG, WM_SET_STEP, 0);
			m_Operation = NONE;
			listdata.state = EMPTY;
		}
		if(lParam == N_SHIPWOUND)
		{
			m_GoodSteps++;
			m_Event[NetPoint.y][NetPoint.x] = SHIPWOUND;
			m_Operation = FIRE;
			listdata.state = SHIPWOUND;
		}
		if(lParam == N_SHIPKILLED)
		{
			m_GoodSteps++;
			m_Event[NetPoint.y][NetPoint.x] = SHIPWOUND;
			SetShipKilled(NetPoint);
			m_Operation = FIRE;
			KilledShips++;
			::SendMessage(m_Parent->m_hWnd, WM_STATIC_TO_DLG, WM_KILL, 0);
			listdata.state = SHIPKILLED;
			if(KilledShips == 10)
			{
				::SendMessage(m_Parent->m_hWnd, WM_STATIC_TO_DLG, WM_WIN_YOU, 0);
				m_Operation = NONE;
				Invalidate();
				return 10;
			}
		}
		::SendMessage(m_Parent->m_hWnd, WM_STATIC_TO_DLG, WM_SET_MOVE, (LPARAM)&listdata);
		Invalidate();
		break;
	case WM_FINISH:
		m_Operation = NONE;
		Invalidate();
		break;
	case WM_PLACEMENT:
		m_Operation = PLACEMENT;
		m_Direction = RIGHT;
		m_Decks = 4;
		m_Position.x = m_Position.y = -1;
		m_Draw = false;
		for(i=0; i<10; i++)
			for(j=0; j<10; j++)
				m_Event[i][j] = EMPTY;
		m_Count = 0;
		KilledShips = 0;
		m_Steps = m_GoodSteps = 0;
		Invalidate();
		break;
	case WM_NONE:
		m_Operation = NONE;
		Invalidate();
		break;
	case WM_IS_PLACEMENT:
		if(!m_Decks)
			return 1;
		else return 0;
	case WM_IS_OK:
		if(!m_Decks)
		{
			for(i=0; i<10; i++)
				for(j=0; j<10; j++)
				{
					if(m_Event[i][j] == CANNOT) m_Event[i][j] = EMPTY;
					if(m_Event[i][j] == SHIPKILLED || 
						m_Event[i][j] == SHIPWOUND) m_Event[i][j] = SHIP;
					if(m_Event[i][j] == NEARKILLEDSHIP)
						m_Event[i][j] = NEARSHIP;
				}
			KilledShips = 0;
			m_Steps = m_GoodSteps = 0;
			Invalidate();
			return 1;
		}
		else return 0;
	case WM_COUNT_PLACEMENT:
		m_Operation = NONE;
		m_Direction = RIGHT;
		m_Decks = 4;
		m_Position.x = m_Position.y = -1;
		m_Draw = false;
		for(i=0; i<10; i++)
			for(j=0; j<10; j++)
				m_Event[i][j] = EMPTY;
		m_Count = 0;
		m_Steps = m_GoodSteps = 0;
		CountPlacement();
		KilledShips = 0;
		Invalidate();
		break;
	case WM_FIRE:
		m_Operation = FIRE;
		break;
	case WM_CLEARFIELD:
		m_Operation = NONE;
		m_Direction = RIGHT;
		m_Decks = 4;
		m_Position.x = m_Position.y = -1;
		m_Draw = false;
		for(i=0; i<10; i++)
			for(j=0; j<10; j++)
				m_Event[i][j] = EMPTY;
		m_Count = 0;
		m_Steps = m_GoodSteps = 0;
		KilledShips = 0;
		Invalidate();
		break;
	}
	return 1;
}

void CField::OnMouseMove(UINT nFlags, CPoint point) 
{
	if(m_Mode == OWN_FIELD && m_Operation == PLACEMENT)
	{
		if(m_Position.x == -1 && (point.x/20)*20+80 <= 200)
		{
			m_Position.x = (point.x/20)*20;
			m_Position.y = (point.y/20)*20;
			m_Draw = true;
			InvalidateRect(CRect(m_Position.x, m_Position.y, m_Position.x+80, m_Position.y+20));
		}
		if(m_Position.x != (point.x/20)*20 || m_Position.y != (point.y/20)*20)
		{
			if(m_Direction == RIGHT && (point.x/20)*20 + m_Decks*20 <= 200)
			{
				m_Draw = false;
				InvalidateRect(CRect(m_Position.x, m_Position.y,
					m_Position.x + m_Decks*20, m_Position.y + 20));
				UpdateWindow();
				m_Position.x = (point.x/20)*20;
				m_Position.y = (point.y/20)*20;
				m_Draw = true;
				InvalidateRect(CRect(m_Position.x, m_Position.y,
					m_Position.x + m_Decks*20, m_Position.y + 20));
			}
			if(m_Direction == TOP && (point.y/20)*20 - (m_Decks-1)*20 >= 0)
			{
				m_Draw = false;
				InvalidateRect(CRect(m_Position.x, m_Position.y - (m_Decks-1)*20,
					m_Position.x + 21, m_Position.y+21));
				m_Position.x = (point.x/20)*20;
				m_Position.y = (point.y/20)*20;
				UpdateWindow();
				m_Draw = true;
				InvalidateRect(CRect(m_Position.x+1, m_Position.y - (m_Decks-1)*20,
					m_Position.x + 20, m_Position.y+20));
			}
			if(m_Direction == BOTTOM && (point.y/20)*20 + m_Decks*20 <= 200) 
			{
				m_Draw = false;
				InvalidateRect(CRect(m_Position.x, m_Position.y,
					m_Position.x + 21, m_Position.y + m_Decks*20 + 1));
				m_Position.x = (point.x/20)*20;
				m_Position.y = (point.y/20)*20;
				UpdateWindow();
				m_Draw = true;
				InvalidateRect(CRect(m_Position.x, m_Position.y,
					m_Position.x + 21, m_Position.y + m_Decks*20 + 1));
			}
			if(m_Direction == LEFT && (point.x/20)*20 - (m_Decks-1)*20 >= 0)
			{
				m_Draw = false;
				InvalidateRect(CRect(m_Position.x - (m_Decks-1)*20, m_Position.y,
					m_Position.x+21, m_Position.y + 21));
				m_Position.x = (point.x/20)*20;
				m_Position.y = (point.y/20)*20;
				UpdateWindow();
				m_Draw = true;
				InvalidateRect(CRect(m_Position.x - (m_Decks-1)*20, m_Position.y,
					m_Position.x+21, m_Position.y + 21));
			}
		}
	}
	CStatic::OnMouseMove(nFlags, point);
}

void CField::OnLButtonDown(UINT nFlags, CPoint point) 
{
	POINT pos;
	int i, j, IsKilled, sumx, razx, sumy, razy, x, y, cx, cy;
	char buf[2];
	NET_DATA data;
	LISTDATA listdata;

	if(m_Operation == PLACEMENT && m_Mode == OWN_FIELD && m_Parent && m_Draw)
	{
		if(ConvertPropertiesToMassive())
		{
			m_Count++;
			switch(m_Count)
			{
			case 1:
				m_Decks = 3;
				break;
			case 3:
				m_Decks = 2;
				break;
			case 6:
				m_Decks = 1;
				break;
			case 10:
				m_Decks = 0;
				break;
			}
		}
	}
	if(m_Mode == FOREIGN_FIELD && m_Operation == FIRE && IsNet)
	{
		NetPoint.x = point.x/20;
		NetPoint.y = point.y/20;
		if(m_Event[NetPoint.y][NetPoint.x] == SHIPWOUND ||
			m_Event[NetPoint.y][NetPoint.x] == SHIPKILLED || 
			m_Event[NetPoint.y][NetPoint.x] == CANNOT)
			return;
		buf[0] = (char)NetPoint.y;
		buf[1] = (char)NetPoint.x;
		data.buf = buf;
		data.lenght = 2;
		::SendMessage(m_Parent->m_hWnd, WM_STATIC_TO_DLG, WM_SEND_DATA, (LPARAM)&data);
		m_Operation = WAIT;
	}
	if(m_Mode == FOREIGN_FIELD && m_Operation == FIRE)
	{
		point.x = point.x/20;
		point.y = point.y/20;
		listdata.x = point.x;
		listdata.y = point.y;
		listdata.str = "ВЫ";
		if(m_Event[point.y][point.x] == SHIPKILLED ||
			m_Event[point.y][point.x] == SHIPWOUND ||
			m_Event[point.y][point.x] == CANNOT ||
			m_Event[point.y][point.x] == NEARKILLEDSHIP) return;
		m_Steps++;
		if(m_Event[point.y][point.x] == SHIP)
		{
			m_GoodSteps++;
			IsKilled = SHIPKILLED;
			m_Event[point.y][point.x] = SHIPWOUND;
			for(i=1; i<4; i++)
				if(m_Event[point.y][point.x+i] != SHIPWOUND || point.x+i==10) break;
			if(m_Event[point.y][point.x+i] == SHIP && point.x+i<10) 
				IsKilled = SHIPWOUND;
			for(i=1; i<4; i++)
				if(m_Event[point.y][point.x-i] != SHIPWOUND || point.x-i==-1) break;
			if(m_Event[point.y][point.x-i] == SHIP && point.x-i>=0) 
				IsKilled = SHIPWOUND;
			for(i=1; i<4; i++)
				if(m_Event[point.y+i][point.x] != SHIPWOUND || point.y+i==10) break;
			if(m_Event[point.y+i][point.x] == SHIP && point.y+i<10) 
				IsKilled = SHIPWOUND;
			for(i=1; i<4; i++)
				if(m_Event[point.y-i][point.x] != SHIPWOUND || point.y-i==-1) break;
			if(m_Event[point.y-i][point.x] == SHIP && point.y-i>=0) 
				IsKilled = SHIPWOUND;
			if(IsKilled == SHIPKILLED)
			{
				pos = point;
				sumx = razx = sumy = razy = 0;
				for(i=1; i<4; i++)
				{
					if(m_Event[pos.y][pos.x+i] == SHIPWOUND && pos.x+i<10)
					{
						m_Event[pos.y][pos.x+i] = SHIPKILLED;
						sumx++;
					}
					if(m_Event[pos.y][pos.x-i] == SHIPWOUND && pos.x-i>=0)
					{
						m_Event[pos.y][pos.x-i] = SHIPKILLED;
						razx++;
					}
					if(m_Event[pos.y+i][pos.x] == SHIPWOUND && pos.y+i<10)
					{
						m_Event[pos.y+i][pos.x] = SHIPKILLED;
						sumy++;
					}
					if(m_Event[pos.y-i][pos.x] == SHIPWOUND && pos.y-i>=0)
					{
						m_Event[pos.y-i][pos.x] = SHIPKILLED;
						razy++;
					}
				}
				m_Event[pos.y][pos.x] = SHIPKILLED;
				if(pos.x-razx-1>=0) x = pos.x-razx-1;
				else x = pos.x-razx;
				if(pos.x+sumx+1<10) cx = pos.x+sumx+1;
				else cx = pos.x+sumx;
				if(pos.y-razy-1>=0) y = pos.y-razy-1;
				else y = pos.y-razy;
				if(pos.y+sumy+1<10) cy = pos.y+sumy+1;
				else cy = pos.y+sumy;
				for(i=y; i<=cy; i++)
					for(j=x; j<=cx; j++)
						if(m_Event[i][j] != SHIPKILLED)
							m_Event[i][j] = NEARKILLEDSHIP;
				KilledShips++;
				InvalidateRect(CRect(x*20, y*20, (cx+1)*20, (cy+1)*20));
				::SendMessage(m_Parent->m_hWnd, WM_STATIC_TO_DLG, WM_KILL, 0);
				if(KilledShips == 10)
				{
					::SendMessage(m_Parent->m_hWnd, WM_STATIC_TO_DLG, WM_WIN_YOU, 0);
					m_Operation = NONE;
				}
				listdata.state = SHIPKILLED;
			}
			else
			{
				InvalidateRect(CRect(point.x*20, point.y*20, (point.x+1)*20, (point.y+1)*20));
				listdata.state = SHIPWOUND;
			}
			::SendMessage(m_Parent->m_hWnd, WM_STATIC_TO_DLG, WM_SET_MOVE, (LPARAM)&listdata);
		}
		else 
		{
			listdata.state = EMPTY;
			m_Event[point.y][point.x] = CANNOT;
			m_Operation = NONE;
			InvalidateRect(CRect(point.x*20, point.y*20, (point.x+1)*20, (point.y+1)*20));
			::SendMessage(m_Parent->m_hWnd, WM_STATIC_TO_DLG, WM_SET_MOVE, (LPARAM)&listdata);
			::SendMessage(m_Parent->m_hWnd, WM_STATIC_TO_DLG, WM_COMP_MOVE, 0);
		}
	}
	CStatic::OnLButtonDown(nFlags, point);
}

void CField::OnRButtonDown(UINT nFlags, CPoint point) 
{
	if(m_Mode == OWN_FIELD && m_Operation == PLACEMENT)
	{
		switch(m_Direction)
		{
		case LEFT:
			if(m_Position.y - (m_Decks-1)*20 >= 0)
			{
				m_Draw = false;
				InvalidateRect(CRect(m_Position.x - (m_Decks-1)*20, m_Position.y,
					m_Position.x+20, m_Position.y + 21));
				UpdateWindow();
				m_Draw = true;
				m_Direction = TOP;
				InvalidateRect(CRect(m_Position.x, m_Position.y - (m_Decks-1)*20,
					m_Position.x + 20, m_Position.y+21));
			}
			break;
		case TOP:
			if(m_Position.x + m_Decks*20 <= 200) 
			{
				m_Draw = false;
				InvalidateRect(CRect(m_Position.x, m_Position.y - (m_Decks-1)*20,
					m_Position.x + 21, m_Position.y+21));
				UpdateWindow();
				m_Draw = true;
				m_Direction = RIGHT;
				InvalidateRect(CRect(m_Position.x, m_Position.y,
					m_Position.x + m_Decks*20 + 1, m_Position.y + 21));
			}
			break;
		case RIGHT:
			if(m_Position.y + m_Decks*20 <= 200) 
			{
				m_Draw = false;
				InvalidateRect(CRect(m_Position.x, m_Position.y,
					m_Position.x + m_Decks*20 + 1, m_Position.y + 21));
				UpdateWindow();
				m_Draw = true;
				m_Direction = BOTTOM;
				InvalidateRect(CRect(m_Position.x, m_Position.y,
					m_Position.x + 20, m_Position.y + m_Decks*20));
			}
			break;
		case BOTTOM:
			if(m_Position.x - (m_Decks-1)*20 >= 0)
			{
				m_Draw = false;
				InvalidateRect(CRect(m_Position.x, m_Position.y,
					m_Position.x + 21, m_Position.y + m_Decks*20+1));
				m_Direction = LEFT;
				UpdateWindow();
				m_Draw = true;
				InvalidateRect(CRect(m_Position.x - (m_Decks-1)*20, m_Position.y,
					m_Position.x+20, m_Position.y + 20));
			}
			break;
		}
	}
	CStatic::OnRButtonDown(nFlags, point);
}

RECT CField::GetRect()
{
	RECT rect;
	if(m_Position.x == -1)
	{
		rect.left=rect.top=rect.bottom=rect.right=0;
		return rect;
	}
	switch(m_Direction)
	{
	case RIGHT:
		rect.left = m_Position.x;
		rect.top = m_Position.y;
		rect.right = m_Position.x+m_Decks*20;
		rect.bottom = m_Position.y+20;
		break;
	case TOP:
		rect.left = m_Position.x;
		rect.bottom = m_Position.y+20;
		rect.right = m_Position.x+20;
		rect.top = m_Position.y-(m_Decks-1)*20;
		break;
	case LEFT:
		rect.right = m_Position.x+20;
		rect.top = m_Position.y;
		rect.left = m_Position.x-(m_Decks-1)*20;
		rect.bottom = m_Position.y+20;
		break;
	case BOTTOM:
		rect.left = m_Position.x;
		rect.top = m_Position.y;
		rect.right = m_Position.x+20;
		rect.bottom = m_Position.y+m_Decks*20;
		break;
	}
	return rect;
}

bool CField::ConvertPropertiesToMassive()
{
	int i;
	int x, y, cx, cy;
	POINT pos = m_Position;
	int decks = m_Decks;
	int dir = m_Direction;

	for(i=0; i<decks; i++)
	{
		switch(dir)
		{
		case LEFT:
			if(m_Event[pos.y/20][pos.x/20-i] != EMPTY) return 0;
			break;
		case TOP:
			if(m_Event[pos.y/20-i][pos.x/20] != EMPTY) return 0;
			break;
		case RIGHT:
			if(m_Event[pos.y/20][pos.x/20+i] != EMPTY) return 0;
			break;
		case BOTTOM:
			if(m_Event[pos.y/20+i][pos.x/20] != EMPTY) return 0;
			break;
		}
	}

	if(pos.y-20>=0) y = pos.y/20-1;
	else y = pos.y/20;
	if(pos.x-20>=0) x = pos.x/20-1;
	else x = pos.x/20;
	switch(dir)
	{
	case RIGHT:
		if(pos.x+decks*20+20<=200) { cx = pos.x/20+decks+1; }
		else { cx = pos.x/20+decks; }
		if(pos.y+40<=200) cy = pos.y/20+2;
		else cy = pos.y/20+1;
		break;
	case BOTTOM:
		if(pos.y+decks*20+20<=200) cy = pos.y/20+decks+1;
		else cy = pos.y/20+decks;
		if(pos.x+40<=200) cx = pos.x/20+2;
		else cx = pos.x/20+1;
		break;
	case LEFT:
		if(pos.x+40<=200) cx = pos.x/20+2;
		else cx = pos.x/20+1;
		if(pos.y+40<=200) cy = pos.y/20+2;
		else cy = pos.y/20+1;
		if(pos.x-decks*20>=0) x = pos.x/20-decks;
		else x = pos.x/20-decks+1;
		break;
	case TOP:
		if(pos.y-decks*20>=0) y = pos.y/20-decks;
		else y = pos.y/20-decks+1;
		if(pos.y+40<=200) cy = pos.y/20+2;
		else cy = pos.y/20+1;
		if(pos.x+40<=200) cx = pos.x/20+2;
		else cx = pos.x/20+1;
		break;
	}
	cx--;
	cy--;
	for(; y<=cy; y++)
		for(i = x; i<=cx; i++)
			m_Event[y][i] = NEARSHIP;
	for(i=0; i<decks; i++)
	{
		switch(dir)
		{
		case LEFT:
			m_Event[pos.y/20][pos.x/20-i] = SHIP;
			break;
		case TOP:
			m_Event[pos.y/20-i][pos.x/20] = SHIP;
			break;
		case RIGHT:
			m_Event[pos.y/20][pos.x/20+i] = SHIP;
			break;
		case BOTTOM:
			m_Event[pos.y/20+i][pos.x/20] = SHIP;
			break;
		}
	}
	return 1;
}

int CField::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CStatic::OnCreate(lpCreateStruct) == -1)
		return -1;
	HINSTANCE hInst = AfxGetInstanceHandle();
//	HBITMAP hbm = (HBITMAP)::LoadImage(hInst, "Water.bmp", IMAGE_BITMAP,
//		0, 0, LR_DEFAULTSIZE|LR_LOADFROMFILE);
//	bmp.Attach(hbm);


	m_Bmp[0] = (HBITMAP)::LoadImage(hInst, MAKEINTRESOURCE(IDB_ONE_DECK), IMAGE_BITMAP,
		0, 0, LR_DEFAULTSIZE|LR_LOADTRANSPARENT);
	m_Bmp[1] = (HBITMAP)::LoadImage(hInst, MAKEINTRESOURCE(IDB_ONE_DECK), IMAGE_BITMAP,
		0, 0, LR_DEFAULTSIZE|LR_LOADTRANSPARENT);
	m_Bmp[2] = (HBITMAP)::LoadImage(hInst, MAKEINTRESOURCE(IDB_TWO_RIGHT_DECK), IMAGE_BITMAP,
		0, 0, LR_DEFAULTSIZE|LR_LOADTRANSPARENT);
	m_Bmp[3] = (HBITMAP)::LoadImage(hInst, MAKEINTRESOURCE(IDB_TWO_BOTTOM_DECK), IMAGE_BITMAP,
		0, 0, LR_DEFAULTSIZE|LR_LOADTRANSPARENT);
	m_Bmp[4] = (HBITMAP)::LoadImage(hInst, MAKEINTRESOURCE(IDB_THREE_RIGHT_DECK), IMAGE_BITMAP,
		0, 0, LR_DEFAULTSIZE|LR_LOADTRANSPARENT);
	m_Bmp[5] = (HBITMAP)::LoadImage(hInst, MAKEINTRESOURCE(IDB_THREE_BOTTOM_DECK), IMAGE_BITMAP,
		0, 0, LR_DEFAULTSIZE|LR_LOADTRANSPARENT);
	m_Bmp[6] = (HBITMAP)::LoadImage(hInst, MAKEINTRESOURCE(IDB_FOUR_RIGHT_DECK), IMAGE_BITMAP,
		0, 0, LR_DEFAULTSIZE|LR_LOADTRANSPARENT);
	m_Bmp[7] = (HBITMAP)::LoadImage(hInst, MAKEINTRESOURCE(IDB_FOUR_BOTTOM_DECK), IMAGE_BITMAP,
		0, 0, LR_DEFAULTSIZE|LR_LOADTRANSPARENT);
	m_bFire = (HBITMAP)::LoadImage(hInst, MAKEINTRESOURCE(IDB_FIRE), IMAGE_BITMAP,
		0, 0, LR_DEFAULTSIZE|LR_LOADTRANSPARENT);
	m_bMonoFire = (HBITMAP)::LoadImage(hInst, MAKEINTRESOURCE(IDB_MONOCHROME_FIRE), IMAGE_BITMAP,
		0, 0, LR_DEFAULTSIZE|LR_LOADTRANSPARENT|LR_MONOCHROME);
	m_cFire = ::LoadCursor(hInst, MAKEINTRESOURCE(IDC_FIRE));

	return 0;
}

void CField::DrawShip(RECT rect, CDC *dc)
{
	int x, y;
	int lenght, dir;
	CDC memDC;
	CBitmap bmp, *oldbmp;
	BITMAP bm, bmist;
	unsigned char *bitspr, *bitsist;
	CBitmap bmpist;
	int i, j;
	int cnt;

	x = (rect.right - rect.left)/20;
	y = (rect.bottom- rect.top)/20;
	if(x > y) { lenght = x; dir = RT; }
	else {lenght = y; dir = BT; }

	memDC.CreateCompatibleDC(dc);
	bmpist.Attach(m_Bmp[(lenght-1)*2+dir]);
	oldbmp = memDC.SelectObject(&bmpist);
	dc->BitBlt(rect.left+1, rect.top+1, rect.right-rect.left-1,
		rect.bottom-rect.top-1, &memDC, 0, 0, SRCCOPY);
/*
	memDC.CreateCompatibleDC(dc);
	bmp.CreateCompatibleBitmap(dc, rect.right-rect.left, rect.bottom-rect.top);
	oldbmp = memDC.SelectObject(&bmp);
	memDC.BitBlt(0, 0, rect.right-rect.left, rect.bottom-rect.top, dc,
		rect.left, rect.top, SRCCOPY);
	bmp.GetBitmap(&bm);
	bitspr = new unsigned char[bm.bmHeight*bm.bmWidthBytes];
	bmp.GetBitmapBits(bm.bmHeight*bm.bmWidthBytes, bitspr);

	bmpist.Attach(m_Bmp[(lenght-1)*2+dir]);
	bmpist.GetBitmap(&bmist);
	bitsist = new unsigned char[bmist.bmHeight*bmist.bmWidthBytes];
	bmpist.GetBitmapBits(bmist.bmHeight*bmist.bmWidthBytes, bitsist);
	cnt = bmist.bmBitsPixel/8;
	for(i=1; i<rect.bottom-rect.top; i++)
		for(j=1; j<rect.right-rect.left; j++)
		{
			if(!(bitsist[j*cnt+i*bmist.bmWidthBytes] == 22 &&
				bitsist[j*cnt+1+i*bmist.bmWidthBytes] == 60 &&
				bitsist[j*cnt+2+i*bmist.bmWidthBytes] == 84))
			{
				bitspr[j*cnt+i*bmist.bmWidthBytes] = 
					bitsist[j*4+i*bmist.bmWidthBytes];
				bitspr[j*cnt+1+i*bmist.bmWidthBytes] = 
					bitsist[j*cnt+1+i*bmist.bmWidthBytes];
				bitspr[j*cnt+2+i*bmist.bmWidthBytes] = 
					bitsist[j*cnt+2+i*bmist.bmWidthBytes];
			}
		}
	bmp.SetBitmapBits(bmist.bmHeight*bmist.bmWidthBytes, bitspr);
	dc->BitBlt(rect.left, rect.top, rect.right-rect.left,
		rect.bottom-rect.top, &memDC, 0, 0, SRCCOPY);*/
	memDC.SelectObject(oldbmp);
	memDC.DeleteDC();
//	bmp.DeleteObject();
	bmpist.Detach();
//	delete[] bitspr;
//	delete[] bitsist;
}

int CField::GenerateNumber(int a)
{
	return rand()%a;
}

void CField::CountPlacement()
{
	int i, j, a, dir;
	POINT pos;
	CString str[10];

	m_Decks = 4;
	m_Count = 4;

	CTime t = CTime::GetCurrentTime();
	srand(t.GetHour()+t.GetMinute()+t.GetSecond());

	for(i=1; i<=m_Decks; i++)
	{
		j=0;
		while(j<m_Count)
		{
			a = GenerateNumber(100);
			pos.x = (a/10)*20;
			pos.y = (a - (a/10)*10)*20;
			dir = GenerateNumber(4);
			switch(dir)
			{
			case 0:
				dir = RIGHT;
				break;
			case 1:
				dir = BOTTOM;
				break;
			case 2:
				dir = LEFT;
				break;
			case 3:
				dir = TOP;
				break;
			}
			if(ConvertPropertiesToMassive(pos, i, dir)) 
			{
				j++;
				if(1 == i) str[j-1].Format("x, y, dir (%d) = %d, %d, %d", j, pos.x/20, pos.y/20, dir);
			}
		}
		m_Count--;
	}

}

bool CField::ConvertPropertiesToMassive(POINT pos, int decks, int dir)
{
	int i;
	int x, y, cx, cy;

	if(dir == RIGHT && (pos.x/20)*20 + decks*20 > 200)
		return 0;
	if(dir == TOP && (pos.y/20)*20 - (decks-1)*20 < 0)
		return 0;
	if(dir == BOTTOM && (pos.y/20)*20 + decks*20 > 200) 
		return 0;
	if(dir == LEFT && (pos.x/20)*20 - (decks-1)*20 < 0)
		return 0;
	for(i=0; i<decks; i++)
	{
		switch(dir)
		{
		case LEFT:
			if(m_Event[pos.y/20][pos.x/20-i] != EMPTY) return 0;
			break;
		case TOP:
			if(m_Event[pos.y/20-i][pos.x/20] != EMPTY) return 0;
			break;
		case RIGHT:
			if(m_Event[pos.y/20][pos.x/20+i] != EMPTY) return 0;
			break;
		case BOTTOM:
			if(m_Event[pos.y/20+i][pos.x/20] != EMPTY) return 0;
			break;
		}
	}
	if(pos.y-20>=0) y = pos.y/20-1;
	else y = pos.y/20;
	if(pos.x-20>=0) x = pos.x/20-1;
	else x = pos.x/20;
	switch(dir)
	{
	case RIGHT:
		if(pos.x+decks*20+20<=200) { cx = pos.x/20+decks+1; }
		else { cx = pos.x/20+decks; }
		if(pos.y+40<=200) cy = pos.y/20+2;
		else cy = pos.y/20+1;
		break;
	case BOTTOM:
		if(pos.y+decks*20+20<=200) cy = pos.y/20+decks+1;
		else cy = pos.y/20+decks;
		if(pos.x+40<=200) cx = pos.x/20+2;
		else cx = pos.x/20+1;
		break;
	case LEFT:
		if(pos.x+40<=200) cx = pos.x/20+2;
		else cx = pos.x/20+1;
		if(pos.y+40<=200) cy = pos.y/20+2;
		else cy = pos.y/20+1;
		if(pos.x-decks*20>=0) x = pos.x/20-decks;
		else x = pos.x/20-decks+1;
		break;
	case TOP:
		if(pos.y-decks*20>=0) y = pos.y/20-decks;
		else y = pos.y/20-decks+1;
		if(pos.y+40<=200) cy = pos.y/20+2;
		else cy = pos.y/20+1;
		if(pos.x+40<=200) cx = pos.x/20+2;
		else cx = pos.x/20+1;
		break;
	}
	cx--;
	cy--;
	for(; y<=cy; y++)
		for(i = x; i<=cx; i++)
			m_Event[y][i] = NEARSHIP;
	for(i=0; i<decks; i++)
	{
		switch(dir)
		{
		case LEFT:
			m_Event[pos.y/20][pos.x/20-i] = SHIP;
			break;
		case TOP:
			m_Event[pos.y/20-i][pos.x/20] = SHIP;
			break;
		case RIGHT:
			m_Event[pos.y/20][pos.x/20+i] = SHIP;
			break;
		case BOTTOM:
			m_Event[pos.y/20+i][pos.x/20] = SHIP;
			break;
		}
	}
	return 1;
}

GENDATA CField::GenerateMove(int x, int y)
{
	int a, i, j, sumx, razx, razy, sumy, cx, cy;
	static c = -1;
	POINT pos = {-1, -1};
	int dir;
	int IsKilled;
	GENDATA data;
	POINT pt;
	int u;

//генерирование следующего случайного хода
	pt.x = x;
	pt.y = y;
	if(pt.x == -1 || pt.y == -1)
	{
		if(c == -1) 
		{
			a = GenerateNumber(100);
			c = (a/10)%2 + a%2;
		}
		u=0;
		while(1)
		{
			u++;
			if(u>20)
			{
				a = GenerateNumber(100);
				if(m_Event[a/10][a-(a/10)*10] != CANNOT &&
					m_Event[a/10][a-(a/10)*10] != SHIPKILLED &&
					m_Event[a/10][a-(a/10)*10] != NEARKILLEDSHIP &&
					m_Event[a/10][a-(a/10)*10] != SHIPWOUND)
				{
					pos.x = (a-(a/10)*10);
					pos.y = (a/10);
					break;
				}
			}
			a = GenerateNumber(100);
			if((((a/10)%2+a%2)%2 == c%2) && 
				m_Event[a/10][a-(a/10)*10] != CANNOT &&
				m_Event[a/10][a-(a/10)*10] != SHIPKILLED &&
				m_Event[a/10][a-(a/10)*10] != NEARKILLEDSHIP &&
				m_Event[a/10][a-(a/10)*10] != SHIPWOUND)
			{
				pos.x = (a-(a/10)*10);
				pos.y = (a/10);
				break;
			}
		}
	}
	else
	{
		if((pt.y+1<10 && m_Event[pt.y+1][pt.x] == SHIPWOUND)
			|| (pt.y-1>=0 && m_Event[pt.y-1][pt.x] == SHIPWOUND))
			dir  = VERT;
		if((pt.x+1<10 && m_Event[pt.y][pt.x+1] == SHIPWOUND)
			|| (pt.x-1>=0 && m_Event[pt.y][pt.x-1] == SHIPWOUND))
			dir  = HOR;
		if(dir != HOR && dir != VERT) 
			dir = ONLYONE;

//здесь должно выполняться (к примеру вправо): след. элемент существует,
//и неравен CANNOT(тобишь по нему не стреляли), через один элемент, если
//он существует, то вправо, вверх и вниз не должно быть кораблей.
		switch(dir)
		{
		case HOR:
			for(i=1; i<4; i++)
				if(pt.x+i==10 || m_Event[pt.y][pt.x+i] != SHIPWOUND) break;
			if(pt.x+i<10 && m_Event[pt.y][pt.x+i] != CANNOT
				  && m_Event[pt.y][pt.x+i] != NEARKILLEDSHIP &&
				  ((pt.x+i+1<10 && ((pt.y-1>=0 &&
					m_Event[pt.y-1][pt.x+i+1] != SHIP &&
					m_Event[pt.y-1][pt.x+i+1] != SHIPKILLED &&
					m_Event[pt.y-1][pt.x+i+1] != SHIPWOUND)
					|| pt.y-1==-1) &&
				   m_Event[pt.y][pt.x+i+1] != SHIPKILLED &&
				   m_Event[pt.y][pt.x+i+1] != SHIPWOUND &&
				   ((pt.y+1<10 &&
				    m_Event[pt.y+1][pt.x+i+1] != SHIP &&
					m_Event[pt.y+1][pt.x+i+1] != SHIPKILLED &&
					m_Event[pt.y+1][pt.x+i+1] != SHIPWOUND)
					|| pt.y+1==10))
				  || pt.x+i+1==10)) { pos.x=pt.x+i; pos.y=pt.y; break; } 
			for(i=1; i<4; i++)
				if(m_Event[pt.y][pt.x-i] != SHIPWOUND || pt.x-i==-1) break;
			if(pt.x-i>=0 && m_Event[pt.y][pt.x-i] != CANNOT &&
				  m_Event[pt.y][pt.x-i] != NEARKILLEDSHIP &&
				  ((pt.x-i-1>=0 && 
				    ((pt.y-1>=0 && m_Event[pt.y-1][pt.x-i-1] != SHIP &&
					 m_Event[pt.y-1][pt.x-i-1] != SHIPKILLED) || pt.y-1==-1) &&
				    m_Event[pt.y][pt.x-i-1] != SHIPKILLED &&
				    ((pt.y+1<10 && m_Event[pt.y+1][pt.x-i-1] != SHIP &&
					 m_Event[pt.y+1][pt.x-i-1] != SHIPKILLED) || pt.y+1==10))
				  || pt.x-i-1==-1)) { pos.x=pt.x-i; pos.y=pt.y; break;}
			break;
		case VERT:
			for(i=1; i<4; i++)
				if(m_Event[pt.y+i][pt.x] != SHIPWOUND || pt.y+i==10) break;
			if(pt.y+i<10 && m_Event[pt.y+i][pt.x] != CANNOT
				  && m_Event[pt.y+i][pt.x] != NEARKILLEDSHIP &&
				  ((pt.y+i+1<10 && 
				  ((pt.x-1>=0 && m_Event[pt.y+i+1][pt.x-1] != SHIP &&
				  m_Event[pt.y+i+1][pt.x-1] != SHIPKILLED) || pt.x-1==-1)
				  && m_Event[pt.y+i+1][pt.x] != SHIPKILLED
				  && ((pt.x+1<10 && m_Event[pt.y+i+1][pt.x+1] != SHIP &&
				  m_Event[pt.y+i+1][pt.x+1] != SHIPKILLED) || pt.x+1==10))
				|| pt.y+i+1==10)) { pos.x=pt.x; pos.y=pt.y+i; break;}
			for(i=1; i<4; i++)
				if(m_Event[pt.y-i][pt.x] != SHIPWOUND || pt.y-i==-1) break;
			if(pt.y-i>=0 && m_Event[pt.y-i][pt.x] != CANNOT
				  && m_Event[pt.y-i][pt.x] != NEARKILLEDSHIP &&
				  ((pt.y-i-1>=0 && 
				  ((pt.x-1>=0 && m_Event[pt.y-i-1][pt.x-1] != SHIP && 
				  m_Event[pt.y-i-1][pt.x-1] != SHIPKILLED) || pt.x-1==-1)
				  && m_Event[pt.y-i-1][pt.x] != SHIPKILLED
				  && ((pt.x+1<10 && m_Event[pt.y-i-1][pt.x+1] != SHIP &&
				  m_Event[pt.y-i-1][pt.x+1] != SHIPKILLED) || pt.x+1==10))
				|| pt.y-i-1==-1)) { pos.x=pt.x; pos.y=pt.y-i; break;}
			break;
		case ONLYONE:
			if(pt.x+1<10 && m_Event[pt.y][pt.x+1] != CANNOT
				  && m_Event[pt.y][pt.x+1] != NEARKILLEDSHIP &&
				  ((pt.x+2<10 && ((pt.y-1>=0 && m_Event[pt.y-1][pt.x+2] != SHIP &&
				  m_Event[pt.y-1][pt.x+2] != SHIPKILLED) || pt.y-1==-1)
				  && m_Event[pt.y][pt.x+2] != SHIPKILLED
				  && ((pt.y+1<10 && m_Event[pt.y+1][pt.x+2] != SHIP &&
				  m_Event[pt.y+1][pt.x+2] != SHIPKILLED) || pt.y+1==10))
				|| pt.x+2==10)) { pos.x=pt.x+1; pos.y=pt.y; break; }
			if(pt.x-1>=0 && m_Event[pt.y][pt.x-1] != CANNOT
				  && m_Event[pt.y][pt.x-1] != NEARKILLEDSHIP &&
			      ((pt.x-2>=0 && ((pt.y-1>=0 && m_Event[pt.y-1][pt.x-2] != SHIP &&
				  m_Event[pt.y-1][pt.x-2] != SHIPKILLED) || pt.y-1==-1)
				  && m_Event[pt.y][pt.x-2] != SHIPKILLED
				  && ((pt.y+1<10 && m_Event[pt.y+1][pt.x-2] != SHIP &&
				  m_Event[pt.y+1][pt.x-2] != SHIPKILLED) || pt.y+1==10))
				|| pt.x-2==-1)) { pos.x=pt.x-1; pos.y=pt.y; break;}
			if(pt.y+1<10 && m_Event[pt.y+1][pt.x] != CANNOT
				  && m_Event[pt.y+1][pt.x] != NEARKILLEDSHIP &&
			      ((pt.y+2<10 && ((pt.x-1>=0 && m_Event[pt.y+2][pt.x-1] != SHIP &&
				  m_Event[pt.y+2][pt.x-1] != SHIPKILLED) || pt.x-1==-1)
				  && m_Event[pt.y+2][pt.x] != SHIPKILLED
				  && ((pt.x+1<10 && m_Event[pt.y+2][pt.x+1] != SHIP &&
				  m_Event[pt.y+2][pt.x+1] != SHIPKILLED) || pt.x+1==10))
				|| pt.y+2==10)) { pos.x=pt.x; pos.y=pt.y+1; break;}
			if(pt.y-1>=0 && m_Event[pt.y-1][pt.x] != CANNOT
				  && m_Event[pt.y-1][pt.x] != NEARKILLEDSHIP &&
				  ((pt.y-2>=0 && ((pt.x-1>=0 && m_Event[pt.y-2][pt.x-1] != SHIP &&
				  m_Event[pt.y-2][pt.x-1] != SHIPKILLED) || pt.x-1==-1)
				  && m_Event[pt.y-2][pt.x] != SHIPKILLED
				  && ((pt.x+1<10 && m_Event[pt.y-2][pt.x+1] != SHIP && 
				  m_Event[pt.y-2][pt.x+1] != SHIPKILLED) || pt.x+1==10))
				|| pt.y-2==-1)) { pos.x=pt.x; pos.y=pt.y-1; break;}
			break;
		}
	}

//проверка на попадание
	if(m_Event[pos.y][pos.x] == SHIP)
	{
		m_Event[pos.y][pos.x] = SHIPWOUND;
		IsKilled = SHIPKILLED;	
		for(i=1; i<4; i++)
			if(m_Event[pos.y][pos.x+i] != SHIPWOUND || pos.x+i==10)break;
		if(m_Event[pos.y][pos.x+i] == SHIP && pos.x+i<10) 
			IsKilled = SHIPWOUND;
		for(i=1; i<4; i++)
			if(m_Event[pos.y][pos.x-i] != SHIPWOUND || pos.x-i==-1)break;
		if(m_Event[pos.y][pos.x-i] == SHIP && pos.x-i>=0) 
			IsKilled = SHIPWOUND;
		for(i=1; i<4; i++)
			if(m_Event[pos.y+i][pos.x] != SHIPWOUND || pos.y+i==10)break;
		if(m_Event[pos.y+i][pos.x] == SHIP && pos.y+i<10) 
			IsKilled = SHIPWOUND;
		for(i=1; i<4; i++)
			if(m_Event[pos.y-i][pos.x] != SHIPWOUND || pos.y-i==-1)break;
		if(m_Event[pos.y-i][pos.x] == SHIP && pos.y-i>=0) 
			IsKilled = SHIPWOUND;
		if(IsKilled == SHIPKILLED)
		{
			sumx = razx = sumy = razy = 0;
			for(i=1; i<4; i++)
			{
				if(m_Event[pos.y][pos.x+i] == SHIPWOUND && pos.x+i<10)
				{
					m_Event[pos.y][pos.x+i] = SHIPKILLED;
					sumx++;
				}
				if(m_Event[pos.y][pos.x-i] == SHIPWOUND && pos.x-i>=0)
				{
					m_Event[pos.y][pos.x-i] = SHIPKILLED;
					razx++;
				}
				if(m_Event[pos.y+i][pos.x] == SHIPWOUND && pos.y+i<10)
				{
					m_Event[pos.y+i][pos.x] = SHIPKILLED;
					sumy++;
				}
				if(m_Event[pos.y-i][pos.x] == SHIPWOUND && pos.y-i>=0)
				{
					m_Event[pos.y-i][pos.x] = SHIPKILLED;
					razy++;
				}
			}
			m_Event[pos.y][pos.x] = SHIPKILLED;
			if(pos.x-razx-1>=0) x = pos.x-razx-1;
			else x = pos.x-razx;
			if(pos.x+sumx+1<10) cx = pos.x+sumx+1;
			else cx = pos.x+sumx;
			if(pos.y-razy-1>=0) y = pos.y-razy-1;
			else y = pos.y-razy;
			if(pos.y+sumy+1<10) cy = pos.y+sumy+1;
			else cy = pos.y+sumy;
			for(i=y; i<=cy; i++)
				for(j=x; j<=cx; j++)
					if(m_Event[i][j] != SHIPKILLED)
						m_Event[i][j] = NEARKILLEDSHIP;
			InvalidateRect(CRect(x*20, y*20, (cx+1)*20, (cy+1)*20));
			data.x = (int)pos.x;
			data.y = (int)pos.y;
			data.IsKilled = SHIPKILLED;
		}
		else 
		{
			data.x = (int)pos.x;
			data.y = (int)pos.y;
			data.IsKilled = SHIPWOUND;
			InvalidateRect(CRect(pos.x*20, pos.y*20, (pos.x+1)*20, (pos.y+1)*20));
		}
	}
	else
	{
		m_Event[pos.y][pos.x] = CANNOT;
		if(pt.x == -1 || pt.y == -1) 
		{
			data.x = (int)pos.x;
			data.y = (int)pos.y;
			data.IsKilled = EMPTY;
			InvalidateRect(CRect(pos.x*20, pos.y*20, (pos.x+1)*20, (pos.y+1)*20));
		}
		else
		{
			data.x = (int)pt.x*10+pos.x;
			data.y = (int)pt.y*10 + pos.y;
			data.IsKilled = SHIPNOTKILLED;
			InvalidateRect(CRect(pos.x*20, pos.y*20, (pos.x+1)*20, (pos.y+1)*20));
		}
	}
	return data;
}


void CField::CompMove()
{
	GENDATA data;
	LISTDATA listdata;
	static x = -1, y = -1;

	data = GenerateMove(x, y);

	while(1)
	{
		m_Steps++;
		listdata.x = data.x;
		listdata.y = data.y;
		listdata.str = "КОМП";
		switch(data.IsKilled)
		{
		case EMPTY:
			listdata.state = EMPTY;
			::SendMessage(m_Parent->m_hWnd, WM_STATIC_TO_DLG, WM_SET_MOVE, (LPARAM)&listdata);
			x = -1;
			y = -1;
			return;
		case SHIPKILLED:
			x = -1;
			y = -1;
			listdata.state = SHIPKILLED;
			::SendMessage(m_Parent->m_hWnd, WM_STATIC_TO_DLG, WM_SET_MOVE, (LPARAM)&listdata);
			KilledShips++;
			m_GoodSteps++;
			if(KilledShips == 10) 
			{
				Invalidate();
				::SendMessage(m_Parent->m_hWnd, WM_STATIC_TO_DLG, WM_WIN_COMP, 0);
				return;
			}
			data = GenerateMove();
			break;
		case SHIPWOUND:
			m_GoodSteps++;
			x = -1;
			y = -1;
			listdata.state = SHIPWOUND;
			::SendMessage(m_Parent->m_hWnd, WM_STATIC_TO_DLG, WM_SET_MOVE, (LPARAM)&listdata);
			data = GenerateMove(data.x, data.y);
			break;
		case SHIPNOTKILLED:
			listdata.x = data.x%10;
			listdata.y = data.y%10;
			data.x /= 10;
			data.y /= 10;
			listdata.state = EMPTY;
			::SendMessage(m_Parent->m_hWnd, WM_STATIC_TO_DLG, WM_SET_MOVE, (LPARAM)&listdata);
			x = data.x;
			y = data.y;
			return;
		}
	}
}

BOOL CField::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	if(m_Mode == FOREIGN_FIELD && (m_Operation == FIRE || m_Operation == WAIT))
	{
		::SetCursor(m_cFire);
		return true;
	}
	return CStatic::OnSetCursor(pWnd, nHitTest, message);
}

void CField::OnDestroy() 
{
	for(int i=0; i<8; i++)
		DeleteObject(m_Bmp[i]);
	DeleteObject(m_bFire);
	DestroyCursor(m_cFire);
	WaterDC.DeleteDC();
	bmp.DeleteObject();
	CStatic::OnDestroy();
}

void CField::DrawObject(RECT rect, CDC *dc, HBITMAP hbm)
{
	CDC memDC;
	CBitmap bmp, *oldbmp;
	BITMAP bm, bmist;
	unsigned char *bitspr, *bitsist, r_i, r_p, g_i, g_p, b_i, b_p;
	CBitmap bmpist, bmpmono;
	int i, j, cnt;

	memDC.CreateCompatibleDC(dc);
	bmp.CreateCompatibleBitmap(dc, rect.right-rect.left, rect.bottom-rect.top);
	oldbmp = memDC.SelectObject(&bmp);
	memDC.BitBlt(0, 0, rect.right-rect.left, rect.bottom-rect.top, dc,
		rect.left, rect.top, SRCCOPY);
	bmp.GetBitmap(&bm);
	bitspr = new unsigned char[bm.bmHeight*bm.bmWidthBytes];
	bmp.GetBitmapBits(bm.bmHeight*bm.bmWidthBytes, bitspr);

	bmpist.Attach(hbm);
	bmpist.GetBitmap(&bmist);
	bitsist = new unsigned char[bmist.bmHeight*bmist.bmWidthBytes];
	bmpist.GetBitmapBits(bmist.bmHeight*bmist.bmWidthBytes, bitsist);
	cnt = bm.bmBitsPixel/8;
	if(cnt > 2)
		for(i=0; i<rect.bottom-rect.top; i++)
			for(j=0; j<rect.right-rect.left; j++)
			{
				r_i = bitsist[j*cnt+i*bmist.bmWidthBytes];
				g_i = bitsist[j*cnt+1+i*bmist.bmWidthBytes];
				b_i = bitsist[j*cnt+2+i*bmist.bmWidthBytes];
				if(!(r_i == 255 && g_i == 255 && b_i == 255))
				{
					bitspr[j*cnt+i*bmist.bmWidthBytes] = r_i;
					bitspr[j*cnt+1+i*bmist.bmWidthBytes] = g_i;
					bitspr[j*cnt+2+i*bmist.bmWidthBytes] = b_i;
				}
			}
	else if(cnt == 2)
		for(i=0; i<rect.bottom-rect.top; i++)
			for(j=0; j<rect.right-rect.left; j++)
			{
				r_i = bitsist[j*cnt+i*bmist.bmWidthBytes];
				g_i = bitsist[j*cnt+1+i*bmist.bmWidthBytes];
				if(!(r_i == 255 && g_i == 255))
				{
					bitspr[j*cnt+i*bmist.bmWidthBytes] = r_i;
					bitspr[j*cnt+1+i*bmist.bmWidthBytes] = g_i;
				}
			}
	bmp.SetBitmapBits(bm.bmHeight*bm.bmWidthBytes, bitspr);
	dc->BitBlt(rect.left, rect.top, rect.right-rect.left,
		rect.bottom-rect.top, &memDC, 0, 0, SRCCOPY);
	memDC.SelectObject(oldbmp);
	memDC.DeleteDC();
	bmpist.Detach();
	bmp.DeleteObject();
	delete[] bitspr;
	delete[] bitsist;
}

void CField::RecvNetData(int x, int y)
{
	char buf[2];
	NET_DATA data;
	LISTDATA listdata;

	buf[0] = N_MESSAGE;
	buf[1] = NetStep(x, y);
	listdata.x = x;
	listdata.y = y;
	listdata.str = "СОП";
	listdata.state = SHIPWOUND;
	m_Steps++;
	if(buf[1] == N_SHIPKILLED)
	{
		m_GoodSteps++;
		KilledShips++;
		::SendMessage(m_Parent->m_hWnd, WM_STATIC_TO_DLG, WM_KILL, 0);
		if(KilledShips == 10)
		{
			m_Operation = NONE;
			::SendMessage(m_Parent->m_hWnd, WM_STATIC_TO_DLG, WM_WIN_NET, 0);
			buf[0] = N_WIN_YOU;
			data.buf = &buf[0];
			data.lenght = 1;
			::SendMessage(m_Parent->m_hWnd, WM_STATIC_TO_DLG, WM_SEND_DATA, (LPARAM)&data);
		}
		listdata.state = SHIPKILLED;
	}
	if(buf[1] == N_SHIPWOUND)
		m_GoodSteps++;
	data.buf = buf;
	data.lenght = 2;
	::SendMessage(m_Parent->m_hWnd, WM_STATIC_TO_DLG, WM_SEND_DATA, (LPARAM)&data);
	if(buf[1] == N_EMPTY)
	{
		::SendMessage(m_Parent->m_hWnd, WM_STATIC_TO_DLG, WM_OWN_STEP, 0);
		listdata.state = EMPTY;
	}
	Invalidate();
	::SendMessage(m_Parent->m_hWnd, WM_STATIC_TO_DLG, WM_SET_MOVE, (LPARAM)&listdata);
}

int CField::NetStep(int x, int y)
{
	POINT pos;
	int i, j, sumx, razx, sumy, razy, cx, cy;
	int IsKilled;

	pos.x = x;
	pos.y = y;
	if(m_Event[pos.y][pos.x] == SHIP)
	{
		m_Event[pos.y][pos.x] = SHIPWOUND;
		IsKilled = SHIPKILLED;	
		for(i=1; i<4; i++)
			if(m_Event[pos.y][pos.x+i] != SHIPWOUND || pos.x+i==10)break;
		if(m_Event[pos.y][pos.x+i] == SHIP && pos.x+i<10) 
			IsKilled = SHIPWOUND;
		for(i=1; i<4; i++)
			if(m_Event[pos.y][pos.x-i] != SHIPWOUND || pos.x-i==-1)break;
		if(m_Event[pos.y][pos.x-i] == SHIP && pos.x-i>=0) 
			IsKilled = SHIPWOUND;
		for(i=1; i<4; i++)
			if(m_Event[pos.y+i][pos.x] != SHIPWOUND || pos.y+i==10)break;
		if(m_Event[pos.y+i][pos.x] == SHIP && pos.y+i<10) 
			IsKilled = SHIPWOUND;
		for(i=1; i<4; i++)
			if(m_Event[pos.y-i][pos.x] != SHIPWOUND || pos.y-i==-1)break;
		if(m_Event[pos.y-i][pos.x] == SHIP && pos.y-i>=0) 
			IsKilled = SHIPWOUND;
		if(IsKilled == SHIPKILLED)
		{
			sumx = razx = sumy = razy = 0;
			for(i=1; i<4; i++)
			{
				if(m_Event[pos.y][pos.x+i] == SHIPWOUND && pos.x+i<10)
				{
					m_Event[pos.y][pos.x+i] = SHIPKILLED;
					sumx++;
				}
				if(m_Event[pos.y][pos.x-i] == SHIPWOUND && pos.x-i>=0)
				{
					m_Event[pos.y][pos.x-i] = SHIPKILLED;
					razx++;
				}
				if(m_Event[pos.y+i][pos.x] == SHIPWOUND && pos.y+i<10)
				{
					m_Event[pos.y+i][pos.x] = SHIPKILLED;
					sumy++;
				}
				if(m_Event[pos.y-i][pos.x] == SHIPWOUND && pos.y-i>=0)
				{
					m_Event[pos.y-i][pos.x] = SHIPKILLED;
					razy++;
				}
			}
			m_Event[pos.y][pos.x] = SHIPKILLED;
/*	
			if(dir == HOR)
			{
				if(pos.x-raz-1>=0) x = pos.x-raz-1;
				else x = pos.x-raz;
				if(pos.x+sum+1<10) cx = pos.x+sum+1;
				else cx = pos.x+sum;
				if(pos.y-1>=0) y = pos.y-1;
				else y = pos.y;
				if(pos.y+1<10) cy = pos.y+1;
				else cy = pos.y;
			}
			if(dir == VERT)
			{
				if(pos.y-raz-1>=0) y = pos.y-raz-1;
				else y = pos.y-raz;
				if(pos.y+sum+1<10) cy = pos.y+sum+1;
				else cy = pos.y+sum;
				if(pos.x-1>=0) x = pos.x-1;
				else x = pos.x;
				if(pos.x+1<10) cx = pos.x+1;
				else cx = pos.x;
			}
*/
			if(pos.x-razx-1>=0) x = pos.x-razx-1;
			else x = pos.x-razx;
			if(pos.x+sumx+1<10) cx = pos.x+sumx+1;
			else cx = pos.x+sumx;
			if(pos.y-razy-1>=0) y = pos.y-razy-1;
			else y = pos.y-razy;
			if(pos.y+sumy+1<10) cy = pos.y+sumy+1;
			else cy = pos.y+sumy;
			for(i=y; i<=cy; i++)
				for(j=x; j<=cx; j++)
					if(m_Event[i][j] != SHIPKILLED)
						m_Event[i][j] = NEARKILLEDSHIP;
			return N_SHIPKILLED;
		}
		else 
			return N_SHIPWOUND;
	}
	m_Event[pos.y][pos.x] = CANNOT;
	return N_EMPTY;	
}

void CField::SetShipKilled(POINT pt)
{
	int i, sumx, sumy, razx, razy, x, y, cx, cy, j;
	POINT pos;
	pos = pt;
	sumx = razx = sumy = razy = 0;
	for(i=1; i<4; i++)
	{
		if(m_Event[pos.y][pos.x+i] == SHIPWOUND && pos.x+i<10)
		{
			m_Event[pos.y][pos.x+i] = SHIPKILLED;
			sumx++;
		}
		if(m_Event[pos.y][pos.x-i] == SHIPWOUND && pos.x-i>=0)
		{
			m_Event[pos.y][pos.x-i] = SHIPKILLED;
			razx++;
		}
		if(m_Event[pos.y+i][pos.x] == SHIPWOUND && pos.y+i<10)
		{
			m_Event[pos.y+i][pos.x] = SHIPKILLED;
			sumy++;
		}
		if(m_Event[pos.y-i][pos.x] == SHIPWOUND && pos.y-i>=0)
		{
			m_Event[pos.y-i][pos.x] = SHIPKILLED;
			razy++;
		}
	}
	m_Event[pos.y][pos.x] = SHIPKILLED;
	if(pos.x-razx-1>=0) x = pos.x-razx-1;
	else x = pos.x-razx;
	if(pos.x+sumx+1<10) cx = pos.x+sumx+1;
	else cx = pos.x+sumx;
	if(pos.y-razy-1>=0) y = pos.y-razy-1;
	else y = pos.y-razy;
	if(pos.y+sumy+1<10) cy = pos.y+sumy+1;
	else cy = pos.y+sumy;
	for(i=y; i<=cy; i++)
		for(j=x; j<=cx; j++)
			if(m_Event[i][j] != SHIPKILLED)
				m_Event[i][j] = NEARKILLEDSHIP;
}

void CField::SetParent(CWnd *wnd)
{
	RECT rect;
	CBitmap bmp;
	CDC *dc = GetDC();

	m_Parent = wnd;
	GetClientRect(&rect);
	WaterDC.CreateCompatibleDC(dc);
	bmp.CreateCompatibleBitmap(dc, rect.right, rect.bottom);
	pOld = WaterDC.SelectObject(&bmp);
	::SendMessage(m_Parent->m_hWnd, WM_STATIC_TO_DLG, WM_FILL_DC, (LPARAM)&WaterDC);
	ReleaseDC(dc);
}

int CField::GetReport()
{
	return (int)((100*m_GoodSteps)/(float)m_Steps);
}
