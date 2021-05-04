// SeaBattleDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SeaBattle.h"
#include "SeaBattleDlg.h"

//#ifdef CLIENTSERVER 
//#define CLIENTSERVER
#include "ClientServer.h"
//#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CSeaBattleDlg dialog

CSeaBattleDlg::CSeaBattleDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSeaBattleDlg::IDD, pParent), m_iButtonState(BS_RESULTS)
{
	//{{AFX_DATA_INIT(CSeaBattleDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_Process = FUCK;
	OwnMove = true;
	IsTurn = false;
	m_MinResult = 0; 
}

BEGIN_MESSAGE_MAP(CSeaBattleDlg, CDialog)
	//{{AFX_MSG_MAP(CSeaBattleDlg)
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_DESTROY()
	ON_WM_SETCURSOR()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_STATIC_TO_DLG, ReceiveMessage)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSeaBattleDlg message handlers

BOOL CSeaBattleDlg::OnInitDialog()
{	
	CDialog::OnInitDialog();
	HINSTANCE hInst;
	RECT rect;
	CDC* dc, tmpDC;
	CSize sz;
	int i, j;
	CBitmap bmp, *ptmpBmp;
	CPicture m_Fon;

	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	hInst = AfxGetInstanceHandle();
	
	m_OwnField.Create(NULL, WS_CHILD|WS_VISIBLE|SS_NOTIFY, CRect(30, 100, 230, 300), this);
	m_OwnField.SetMode(OWN_FIELD);
	m_ForeignField.Create(NULL, WS_CHILD|WS_VISIBLE|SS_NOTIFY, CRect(260, 100, 460, 300), this);
	m_ForeignField.SetMode(FOREIGN_FIELD);
	m_NumberO.Create(NULL, WS_CHILD|WS_VISIBLE, CRect(30, 80, 230, 95), this);
	m_NumberO.SetMode(NUMBERS);
	m_NumberF.Create(NULL, WS_CHILD|WS_VISIBLE, CRect(260, 80, 460, 95), this);
	m_NumberF.SetMode(NUMBERS);
	m_LetterO.Create(NULL, WS_CHILD|WS_VISIBLE, CRect(10, 100, 25, 300), this);
	m_LetterO.SetMode(LETTERS);
	m_LetterF.Create(NULL, WS_CHILD|WS_VISIBLE, CRect(240, 100, 255, 300), this);
	m_LetterF.SetMode(LETTERS);
	m_Sight = (HCURSOR)::LoadCursor(hInst, MAKEINTRESOURCE(IDC_SIGHT));

	m_Process = PLACE;
	m_Operation = UNDEFINED;

	GetClientRect(&rect);
	dc = GetDC();
	m_FonDC.CreateCompatibleDC(dc);
	bmp.CreateCompatibleBitmap(dc, rect.right+150, rect.bottom);
	ReleaseDC(dc);
	pOld = m_FonDC.SelectObject(&bmp);
	m_Fon.Load(JPG_BACKGROUND);
	m_Fon.Render(&m_FonDC, CRect(0, 0, 50, 50));
	for(i=0; i<=rect.right/50+3; i++)
		for(j=0; j<=rect.bottom/50; j++)
			m_FonDC.BitBlt(i*50, j*50, 50, 50, &m_FonDC, 0, 0, SRCCOPY);
	m_FonDC.SetBkMode(TRANSPARENT);
	m_FonDC.TextOut(32, 306, "Убито:");
	m_FonDC.TextOut(262, 306, "Убито:");
	m_OwnField.SetParent(this);
	m_ForeignField.SetParent(this);
	m_Fon.Load(JPG_TITLE);
	m_Fon.Render(&m_FonDC, CRect(170, 1, 300, 20));

	bClose.LoadBitmap(IDB_CLOSE);
	bHide.LoadBitmap(IDB_HIDE);
	bTnOn.LoadBitmap(IDB_TURNON);
	bTnOut.LoadBitmap(IDB_TURNOUT);
	tmpDC.CreateCompatibleDC(&m_FonDC);
	ptmpBmp = tmpDC.SelectObject(&bClose);
	m_FonDC.BitBlt(rect.right-21, 1, 20, 20, &tmpDC, 0, 0, SRCCOPY);
	tmpDC.SelectObject(&bHide);
	m_FonDC.BitBlt(rect.right-45, 1, 20, 20, &tmpDC, 0, 0, SRCCOPY);
	tmpDC.SelectObject(&bTnOn);
	m_FonDC.BitBlt(rect.right-69, 1, 20, 20, &tmpDC, 0, 0, SRCCOPY);
	tmpDC.SelectObject(ptmpBmp);
	tmpDC.DeleteDC();

	m_FonDC.TextOut(rect.right+20, 83, "История игры");
	m_FonDC.SetTextAlign(TA_CENTER);
	
	m_cHand = LoadCursor(hInst, MAKEINTRESOURCE(IDC_HAND));
	m_cDefault = LoadCursor(hInst, MAKEINTRESOURCE(IDC_DEFAULT));
	SetWindowText("Морской бой. Автор: Волков Алексей");
	Help.Create(IDD_HELP, this);

	NetworkWnd.Create(IDD_NETWORK);
	wnd = this;
	NetworkWnd.parent = this;
	Start();

//Красивые Кнопки 
	m_ShipWnd.Create(NULL, NULL, WS_CHILD|WS_VISIBLE, CRect(0, 0, 103, 88), this, 0);
	m_ShipWnd.SetMode(BT_SHIPS, this);
	m_NetWnd.Create(NULL, NULL, WS_CHILD|WS_VISIBLE, CRect(105, 20, 105+112, 20+65), this, 0);
	m_NetWnd.SetMode(BT_NET, this);
	m_SingleWnd.Create(NULL, NULL, WS_CHILD|WS_VISIBLE, CRect(220, 18, 220+142, 18+68), this, 0);
	m_SingleWnd.SetMode(BT_SINGLE, this);
	m_HelpWnd.Create(NULL, NULL, WS_CHILD|WS_VISIBLE, CRect(365, 20, 365+118, 20+58), this, 0);
	m_HelpWnd.SetMode(BT_HELP, this);

	m_MovesBox.Create(WS_CHILD|WS_BORDER|WS_HSCROLL|WS_TABSTOP|LVS_LIST,
		CRect(rect.right-10, 100, rect.right+140, 300), this, 0);
	m_MovesBox.Init(this);
		
	m_Results.Create(WS_CHILD|WS_BORDER|LVS_LIST,
		CRect(rect.right-10, 100, rect.right+140, 300), this, 0);
	m_Results.SetBkColor(RGB(35, 89, 127));
	m_Results.SetTextBkColor(RGB(35, 89, 127));

	m_ResFile.Open("Data.dat", CFile::modeCreate | CFile::modeNoTruncate
		| CFile::modeReadWrite | CFile::shareExclusive);
	ReadFile();

	m_SaveDialog.Create(IDD_SAVE_RESULT, this);
	m_SaveDialog.SetParent(this);
	m_SaveDialog.ShowWindow(SW_HIDE);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CSeaBattleDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	RECT rect;
	CString str;

	GetClientRect(&rect);

	dc.BitBlt(dc.m_ps.rcPaint.left, dc.m_ps.rcPaint.top,
		dc.m_ps.rcPaint.right-dc.m_ps.rcPaint.left,
		dc.m_ps.rcPaint.bottom-dc.m_ps.rcPaint.top,
		&m_FonDC, dc.m_ps.rcPaint.left, 
		dc.m_ps.rcPaint.top, SRCCOPY);

	dc.SetBkMode(TRANSPARENT);
	str.Format("%d", m_OwnField.KilledShips);
	dc.TextOut(77, 306, str);
	str.Format("%d", m_ForeignField.KilledShips);
	dc.TextOut(307, 306, str);
	dc.SetBkMode(OPAQUE);
	DrawMove();
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.



HRESULT CSeaBattleDlg::ReceiveMessage(WPARAM wParam, LPARAM lParam)
{
	DATA *data;
	NET_DATA *netdata;
	LISTDATA *listdata;
	CDC *dc;
	CSize sz;
	RECT rect;
	CString str;
	int cnt, res, i, j;
	char *buf, tmp[100];

	data = (DATA*)lParam;

	switch(wParam)
	{
	case WM_ADD_RESULT:
		buf = (char *)lParam;
		cnt = m_Results.GetItemCount();
		str.Format("%s %d", buf, m_ForeignField.GetReport());
		if(cnt == 0)
		{
			m_Results.InsertItem(cnt, str.GetBuffer(100));
			m_Results.SetItemData(cnt, m_ForeignField.GetReport());
			break;
		}
		for(i=0; i<cnt; i++)
		{
			res = m_Results.GetItemData(i);
			if(res <= m_ForeignField.GetReport())
			{
				m_Results.GetItemText(cnt-1, 0, tmp, 100);
				m_Results.InsertItem(cnt, tmp);
				m_Results.SetItemData(cnt, m_Results.GetItemData(cnt-1));
				for(j=cnt-1; j>i; j--)
				{
					m_Results.SetItemData(j, m_Results.GetItemData(j-1));
					m_Results.GetItemText(j-1, 0, tmp, 100);
					m_Results.SetItemText(j, 0, tmp);
				}
				m_Results.SetItemText(i, 0, str.GetBuffer(100));
				m_Results.SetItemData(i, m_ForeignField.GetReport());
				if(cnt >= 15) m_Results.DeleteItem(cnt-1);
				break;
			}
			else if(i == cnt-1 && cnt < 15)
			{
				m_Results.InsertItem(cnt, str.GetBuffer(100));
				m_Results.SetItemData(cnt, m_ForeignField.GetReport());
			}
		}
		break;
	case WM_LBUTTONDOWN_BUT:
		if(m_iButtonState == BS_RESULTS)
		{
			m_Results.ShowWindow(SW_SHOW);
			m_MovesBox.ShowWindow(SW_HIDE);
			m_iButtonState = BS_HISTORY;
		}
		else
		{
			m_Results.ShowWindow(SW_HIDE);
			m_MovesBox.ShowWindow(SW_SHOW);
			m_iButtonState = BS_RESULTS;
		}
		break;
	case WM_NEW_GAME:
		m_OwnField.ReceiveMessage(WM_IS_OK, 0);
		m_ForeignField.ReceiveMessage(WM_CLEARFIELD, 0);
		break;
	case WM_SET_MOVE:
		listdata = (LISTDATA*)lParam;
		if(listdata->y == 9) listdata->y++;
		switch(listdata->state)
		{
		case EMPTY:
			str.Format("%s:[%c%d] - промах", listdata->str, listdata->y+97, listdata->x+1);
			break;
		case SHIPKILLED:
			str.Format("%s:[%c%d] - убил", listdata->str, listdata->y+97, listdata->x+1);
			break;
		case SHIPWOUND:
			str.Format("%s:[%c%d] - ранил", listdata->str, listdata->y+97, listdata->x+1);
			break;
		}
		cnt = m_MovesBox.GetItemCount();
		m_MovesBox.InsertItem(cnt, str);
		m_MovesBox.Scroll(CSize(cnt/13*100, 0));
		break;
	case WM_LBT_DOWN:
		SetBtDown((int)lParam);
		break;
	case WM_FILL_DC:
		m_OwnField.GetClientRect(&rect);
		dc = (CDC*)lParam;
		dc->BitBlt(0, 0, rect.right, rect.bottom, &m_FonDC, 0, 0, SRCCOPY);
		break;
	case WM_FILL_LIST_DC:
		m_OwnField.GetClientRect(&rect);
		dc = (CDC*)lParam;
		dc->BitBlt(0, 0, 150, 300, &m_FonDC, 0, 0, SRCCOPY);
		break;
	case WM_SEND_DATA:
		netdata = (NET_DATA *)lParam;
		SendData(netdata->buf, netdata->lenght);
		break;
	case WM_SET_STEP:
		OwnMove = (bool)lParam;
		InvalidateRect(CRect(340, 307, 430, 325));
		break;
	case WM_OWN_STEP:
//		m_OwnField.Invalidate();
		OwnMove = true;
		DrawMove();
		m_ForeignField.ReceiveMessage(WM_FIRE, 0);
		break;
	case WM_INV_STR:
		InvalidateRect(CRect(340, 307, 430, 325));
		break;
	case WM_COMP_MOVE:
		InvalidateRect(CRect(305, 303, 330, 325));
		OwnMove = false;
		DrawMove();
		m_OwnField.CompMove();
//		m_OwnField.Invalidate();
		InvalidateRect(CRect(77, 303, 110, 325));
		if(m_Process == SINGLE_PLAY)
		{
			OwnMove = true;
			DrawMove();
			m_ForeignField.ReceiveMessage(WM_FIRE, 0);
		}
		return 1;
	case WM_WIN_COMP:
		m_ForeignField.ReceiveMessage(WM_FINISH, 0);
		str.Format("К сожалению ты проиграл. :-(( Твой рейтинг: %d",
			m_ForeignField.GetReport());
		::AfxMessageBox(str);
		m_Process = FUCK;
		DrawMove();
		break;
	case WM_WIN_YOU:
		str.Format("Поздаравляю. Ты выйграл. :-))) Твой рейтинг: %d",
			m_ForeignField.GetReport());
		::AfxMessageBox(str);
		m_Process = FUCK;
		DrawMove();		
		if(m_ForeignField.GetReport() <= m_MinResult) break;
		MoveOwnWnd(&m_SaveDialog);
		m_SaveDialog.ShowWindow(SW_SHOW);
		break;
	case WM_KILL:
		InvalidateRect(CRect(305, 303, 330, 320));
		InvalidateRect(CRect(77, 303, 110, 320));
		break;
	}
	return NULL;
}

int CSeaBattleDlg::GenerateNumber(int a)
{
	CTime t = CTime::GetCurrentTime();
	srand(t.GetHour()+t.GetMinute()+t.GetSecond());
	return rand()%a;
}

void CSeaBattleDlg::OnMouseMove(UINT nFlags, CPoint point) 
{
	RECT rect;
	int x, y;
	int cx, cy;

	x = point.x;
	y = point.y;
	cx = GetSystemMetrics(SM_CXSCREEN);
	cy = GetSystemMetrics(SM_CXSCREEN);
	GetWindowRect(&rect);
	if(m_Operation == LBUTTONDOWN)
	{
		rect.left += point.x - MovePoint.x;
		rect.right += point.x - MovePoint.x;
		rect.top += point.y - MovePoint.y;
		rect.bottom += point.y - MovePoint.y;
		if(rect.right >= cx-10 && rect.right <= cx+10)
		{
			rect.left = cx - (rect.right - rect.left);
			rect.right = cx;
		}
		if(rect.bottom >= cy-10 && rect.bottom <= cy+10)
		{
			rect.top = cy - (rect.bottom - rect.top);
			rect.bottom = cy;
		}
		if(rect.left <= 10 && rect.left >= -10)
		{
			rect.right -= rect.left;
			rect.left = 0;
		}
		if(rect.top <= 10 && rect.top >= -10)
		{
			rect.bottom -= rect.top;
			rect.top = 0;
		}
		MoveWindow(&rect);
		return;
	}
/*	if(IsButton(point) != B_NONE)
	{
		m_Operation = FINDITEM;
//		::SetCursor(m_cHand);
	}
	else 
		if(m_Operation != LBUTTONDOWN)
		{
			m_Operation = FUCK;
//			::SetCursor(m_cDefault);
		}*/
	CDialog::OnMouseMove(nFlags, point);
}

void CSeaBattleDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	int but;
	RECT rect;
	CDC *dc;

	but = IsButton(point);
	switch(but)
	{
	case B_TURN:
		if(IsTurn) IsTurn = false;
		else IsTurn = true;
		SetTurn();
		break;
	case B_HIDE:
		CloseWindow(); 
		break;
	case B_CLOSE:
		DestroyWindow();
		break;
	case B_RESHIS:
		GetClientRect(&rect);
		dc = GetDC();
		dc->FillRect(CRect(rect.right-150, 83, rect.right, 15+83),
			&CBrush(RGB(35, 89, 127)));
		dc->SetTextAlign(TA_CENTER);
		dc->SetBkColor(RGB(35, 89, 127));
		if(m_iButtonState == BS_HISTORY)
		{

			dc->TextOut(rect.right-85, 83, "История игры");
			m_Results.ShowWindow(SW_HIDE);
			m_MovesBox.ShowWindow(SW_SHOW);
			m_iButtonState = BS_RESULTS;
		}
		else
		{
			dc->TextOut(rect.right-85, 83, "Чемпионы");
			m_Results.ShowWindow(SW_SHOW);
			m_MovesBox.ShowWindow(SW_HIDE);
			m_iButtonState = BS_HISTORY;
		}
		ReleaseDC(dc);
		DrawHisResButton();
		break;
	default:
		m_Operation = LBUTTONDOWN;
		SetCapture();
		MovePoint = point;
		break;
	}
	CDialog::OnLButtonDown(nFlags, point);
}

void CSeaBattleDlg::OnLButtonUp(UINT nFlags, CPoint point) 
{
	m_Operation = UNDEFINED;
	ReleaseCapture();
	CDialog::OnLButtonUp(nFlags, point);
}

void CSeaBattleDlg::OnDestroy() 
{
	char buf;
	CBitmap *bmp;

	SaveFile();
	m_ResFile.Close();

	if(ServSock) 
	{
		SendBroadcast(ENDSERVER);
		closesocket(ServSock);
		ServSock = 0;
	}
	if(Sock)
	{
		buf = CLOSECONNECT;
		SendData(&buf, 1);
		closesocket(Sock);
		Sock = 0;
	}
	Exit();
	CDialog::OnDestroy();
	bmp = m_FonDC.SelectObject(pOld);
	m_FonDC.DeleteDC();		
	bmp->DeleteObject();
	bClose.DeleteObject();
	bHide.DeleteObject();
	bTnOn.DeleteObject();
	bTnOut.DeleteObject();
	::DeleteObject(m_Sight);
	::DeleteObject(m_cHand);
	::DeleteObject(m_cDefault);
}

BOOL CSeaBattleDlg::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	if(wParam == 1 || (wParam == 2 && !lParam)) return 1;
	return CDialog::OnCommand(wParam, lParam);
}

BOOL CSeaBattleDlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
/*
	if(m_Operation == FINDITEM) 
	{
		::SetCursor(m_cHand);
		return 1;
	}
	else
	{
		::SetCursor(m_cDefault);
		return 1;
	}
*/
	return CDialog::OnSetCursor(pWnd, nHitTest, message);
}

int CSeaBattleDlg::IsButton(POINT point)
{
	RECT rect; 
	GetClientRect(&rect);
	int x, y;
	x = point.x;
	y = point.y;
	if(x >= rect.right-20 && x <= rect.right && y>=0 && y<=20) return B_CLOSE;
	if(x >= rect.right-45 && x <= rect.right-25 && y >= 0 && 
		y <= 20) return B_HIDE;
	if(x >= rect.right-69 && x <= rect.right-49 && y >= 0 && 
		y <= 20) return B_TURN;
	if(rect.right > 500 && x >= rect.right - 160 && 
		x <= rect.right - 10 && y >= 307 && y <= 327) 
		return B_RESHIS;
	return B_NONE;
}

void CSeaBattleDlg::MoveHelp()
{
	int w, h;
	RECT WndRect, NetRect;

	GetWindowRect(&WndRect);
	Help.GetWindowRect(&NetRect);
	w = NetRect.right-NetRect.left;
	h = NetRect.bottom-NetRect.top;
	NetRect.left = (WndRect.right-w+WndRect.left)/2;
	NetRect.top = (WndRect.bottom-h+WndRect.top)/2;
	NetRect.right = (WndRect.right+w+WndRect.left)/2;
	NetRect.bottom = (WndRect.bottom+h+WndRect.top)/2;
	Help.MoveWindow(&NetRect);
}

void CSeaBattleDlg::MoveNetworkWnd()
{
	int w, h;
	RECT WndRect, NetRect;

	GetWindowRect(&WndRect);
	NetworkWnd.GetWindowRect(&NetRect);
	w = NetRect.right-NetRect.left;
	h = NetRect.bottom-NetRect.top;
	NetRect.left = (WndRect.right-w+WndRect.left)/2;
	NetRect.top = (WndRect.bottom-h+WndRect.top)/2;
	NetRect.right = (WndRect.right+w+WndRect.left)/2;
	NetRect.bottom = (WndRect.bottom+h+WndRect.top)/2;
	NetworkWnd.MoveWindow(&NetRect);
}

void CSeaBattleDlg::NetworkStart()
{
	if(MultiSock) Exit();
	Start();
	RefreshList();
}

void CSeaBattleDlg::GetServName(char *name)
{
	name[0] = 0;
}

void CSeaBattleDlg::RefreshList()
{
	int c, i;
	c = NetworkWnd.m_Servers.GetCount();
	if(c)
		for(i=0; i<c; i++)
			NetworkWnd.m_Servers.DeleteString(i);
	c = GetCount();
	for(i=0; i<c; i++)
		NetworkWnd.m_Servers.AddString(Data[i].name);
}

void CSeaBattleDlg::DrawMove()
{
	CDC *dc = GetDC();
	CString str;

	dc->SetBkMode(TRANSPARENT);
	if(OwnMove)
		str.Format("Ваш ход");
	else
	{
		if(m_Process == SINGLE_PLAY)
			str.Format("Ход компа");
		if(m_Process == NET_GAME)
			str.Format("Ход cоперника");
	}
	dc->BitBlt(380, 307, 100, 20,
		&m_FonDC, 380, 307, SRCCOPY);
	if(m_Process == SINGLE_PLAY || m_Process == NET_GAME)
		dc->TextOut(380, 307, str);
	if(m_Process == FUCK && ServSock && !Sock)
	{
		str.Format("Ожидание соерника");
		dc->TextOut(340, 307, str);
	}
	ReleaseDC(dc);
}

void CSeaBattleDlg::SetBtDown(int but)
{
	int code;
	char buf;
	switch(but)
	{
	case B_HELP:
		MoveHelp();
		Help.ShowWindow(SW_SHOW);
		break;
	case B_NETWORK:
		if(m_OwnField.ReceiveMessage(WM_IS_OK, 0))
		{
			if(!NetworkWnd.m_hWnd)
			{
				Create(IDD_NETWORK);
				Exit();
				Start();
				TimeOut(3);
			}
			m_MovesBox.DeleteAllItems();
			m_ForeignField.ReceiveMessage(WM_CLEARFIELD, 0);
			RefreshList();
			MoveNetworkWnd();
			NetworkWnd.ShowWindow(SW_SHOW);
		}
		else 
		{
			::AfxMessageBox("Расставьте сначала свои корабли");
			m_NetWnd.IsFocus = false;
			m_NetWnd.Invalidate();
		}
		break;
	case B_PLACEMENT:
		if(ServSock || Sock)
		{
			m_ShipWnd.IsFocus = false;
			m_ShipWnd.Invalidate();
			if(::AfxMessageBox("Желаете прервать соединение?", MB_YESNO) == IDNO)
				return;
		}
		if(ServSock)
		{
			SendBroadcast(ENDSERVER);
			closesocket(ServSock);
			ServSock = 0;
		}
		if(Sock)
		{
			buf = CLOSECONNECT;
			SendData(&buf, 1);
			closesocket(Sock);
			Sock = 0;
		}
		m_MovesBox.DeleteAllItems();
		m_OwnField.ReceiveMessage(WM_PLACEMENT, 0);
		m_ForeignField.ReceiveMessage(WM_CLEARFIELD, 0);
		InvalidateRect(CRect(335, 300, 355, 345));
		InvalidateRect(CRect(105, 300, 130, 345));
		m_Process = PLACE;
		DrawMove();
		break;
	case B_SINGLE:
		if(ServSock || Sock)
		{
			m_SingleWnd.IsFocus = false;
			m_SingleWnd.Invalidate();
			if(::AfxMessageBox("Желаете прервать соединение?", MB_YESNO) == IDNO)
				return;
		}
		if(ServSock) 
		{
			SendBroadcast(ENDSERVER);
			closesocket(ServSock);
			ServSock = 0;
		}
		if(Sock)
		{
			buf = CLOSECONNECT;
			SendData(&buf, 1);
			closesocket(Sock);
			Sock = 0;
		}
		if(m_OwnField.ReceiveMessage(WM_IS_OK, 0))
		{
			m_Process = SINGLE_PLAY;
			m_ForeignField.m_Operation = NONE;
			m_ForeignField.IsNet = m_OwnField.IsNet = false;
			code = ::AfxMessageBox("Вы первым начнете???", MB_YESNO);
			m_MovesBox.DeleteAllItems();
			m_SingleWnd.IsFocus = false;
			m_SingleWnd.Invalidate();
			if(IDYES == code) m_CompFirstStep = 0;
			if(IDNO == code) m_CompFirstStep = 1;
			m_ForeignField.ReceiveMessage(WM_COUNT_PLACEMENT, 0);		
			if(m_CompFirstStep)
			{
				m_OwnField.CompMove();
				m_OwnField.Invalidate();
			}
			m_ForeignField.ReceiveMessage(WM_FIRE, 0);
			DrawMove();
			ReceiveMessage(WM_INV_STR, 0);
		}
		else
		{
			::AfxMessageBox("Расставьте сначала свои корабли");
			m_SingleWnd.IsFocus = false;
			m_SingleWnd.Invalidate();
		}
		break;
	}
}

void CSeaBattleDlg::SetTurn()
{
	RECT rect;
	CDC* dc = GetDC(), tmpDC;
	CBitmap *old;

	tmpDC.CreateCompatibleDC(dc);
	GetWindowRect(&rect);
	if(IsTurn)
	{
		rect.right += 150;
		MoveWindow(&rect);
		m_FonDC.BitBlt(rect.right-rect.left-69-150, 1, 70, 20, &m_FonDC, 0, 0, SRCCOPY);
		old = tmpDC.SelectObject(&bClose);
		m_FonDC.BitBlt(rect.right-rect.left-21, 1, 20, 20, &tmpDC, 0, 0, SRCCOPY);
		tmpDC.SelectObject(&bHide);
		m_FonDC.BitBlt(rect.right-rect.left-45, 1, 20, 20, &tmpDC, 0, 0, SRCCOPY);
		tmpDC.SelectObject(&bTnOut);
		m_FonDC.BitBlt(rect.right-rect.left-69, 1, 20, 20, &tmpDC, 0, 0, SRCCOPY);
		InvalidateRect(CRect(rect.right-rect.left-70-150-3, 1, rect.right, 20));
		MoveButtons(true);
		if(m_iButtonState == BS_RESULTS)
			m_MovesBox.ShowWindow(SW_SHOW);
		else
			m_Results.ShowWindow(SW_SHOW);
		DrawHisResButton();
	}
	else
	{
		rect.right -= 150;
		MoveWindow(&rect);
		m_FonDC.BitBlt(rect.right-rect.left-69+150, 1, 70, 20, &m_FonDC, 0, 0, SRCCOPY);
		old = tmpDC.SelectObject(&bClose);
		m_FonDC.BitBlt(rect.right-rect.left-21, 1, 20, 20, &tmpDC, 0, 0, SRCCOPY);
		tmpDC.SelectObject(&bHide);
		m_FonDC.BitBlt(rect.right-rect.left-45, 1, 20, 20, &tmpDC, 0, 0, SRCCOPY);
		tmpDC.SelectObject(&bTnOn);
		m_FonDC.BitBlt(rect.right-rect.left-69, 1, 20, 20, &tmpDC, 0, 0, SRCCOPY);
		MoveButtons(false);
		m_MovesBox.ShowWindow(SW_HIDE);
		m_Results.ShowWindow(SW_HIDE);
		InvalidateRect(CRect(rect.right-rect.left-69-150, 1, rect.right, 20));
		DrawHisResButton(false);
	}
	tmpDC.SelectObject(old);
	tmpDC.DeleteDC();
	ReleaseDC(dc);
}

void CSeaBattleDlg::MoveButtons(bool IsTn)
{
	RECT rect;
	int w;

	GetClientRect(&rect);
	w = rect.right-118-103-112-142;
	w = w/3;
	if(IsTn)
	{
		m_HelpWnd.MoveWindow(103+112+142+3*w, 20, 118, 58);
		m_SingleWnd.MoveWindow(103+w+112+w, 18, 142, 68);
		m_NetWnd.MoveWindow(103+w, 20, 112, 65);
	}
	else
	{
		m_NetWnd.MoveWindow(103+w, 20, 112, 65);
		m_SingleWnd.MoveWindow(103+w+112+w, 18, 142, 68);
		m_HelpWnd.MoveWindow(103+112+142+3*w, 20, 118, 58);
	}
}

void CSeaBattleDlg::DrawHisResButton(bool IsDraw)
{
	RECT rect;

	GetClientRect(&rect);
	if(IsDraw)
	{
		m_FonDC.FillRect(CRect(rect.right-170, 307, rect.right, 331),
			&CBrush(RGB(35, 89, 127)));
		m_FonDC.MoveTo(rect.right - 160, 307);
		m_FonDC.LineTo(rect.right - 10, 307);
		m_FonDC.LineTo(rect.right - 10, 327);
		m_FonDC.LineTo(rect.right - 160, 327);
		m_FonDC.LineTo(rect.right - 160, 307);
		if(m_iButtonState == BS_RESULTS)
			m_FonDC.TextOut(rect.right - 85, 310, "Чемпионы");
		else
			m_FonDC.TextOut(rect.right - 85, 310, "История игры");
		InvalidateRect(CRect(rect.right-170, 307, rect.right, 331));
	}
	else
	{
		m_FonDC.FillRect(CRect(rect.right-170, 307, rect.right, 331),
			&CBrush(RGB(35, 89, 127)));
		InvalidateRect(CRect(rect.right-170, 307, rect.right, 331));
	}
}

bool CSeaBattleDlg::ReadFile()
{
	char buf[100], tmp[100];
	CString str;
	int i, count, curpos, res, numb, digit;
	bool IsName;

	count = m_ResFile.Read(buf, 100);
	if(count < 4) return false;
	IsName = true;
	curpos = 0;
	res = 0;
	digit = 0;
	for(i=0; i<count; i++)
	{
		if(IsName)
		{
			if(buf[i] == 11) { IsName = false; tmp[curpos] = 32; }
			else tmp[curpos] = buf[i];
		}
		else
		{
			if(buf[i] == 11 || buf[i] == 12)
			{
				numb = m_Results.GetItemCount();
				tmp[curpos] = 0;
				m_Results.InsertItem(numb, tmp);
				m_Results.SetItemData(numb, digit);
				IsName = true;
				curpos = 0;
//				if(m_MinResult < digit) m_MinResult = digit;
				digit = 0;
				if(buf[i] == 12) break;
				continue;
			}
			else
			{
				if(buf[i] == 13)
					tmp[curpos] = 48;
				else
					tmp[curpos] = buf[i]+48;
				digit = digit*10 + tmp[curpos]-48;
			}
		}
		curpos++;
	}
	return true;
}

void CSeaBattleDlg::MoveOwnWnd(CWnd *wnd)
{
	int w, h;
	RECT WndRect, NetRect;

	GetWindowRect(&WndRect);
	wnd->GetWindowRect(&NetRect);
	w = NetRect.right-NetRect.left;
	h = NetRect.bottom-NetRect.top;
	NetRect.left = (WndRect.right-w+WndRect.left)/2;
	NetRect.top = (WndRect.bottom-h+WndRect.top)/2;
	NetRect.right = (WndRect.right+w+WndRect.left)/2;
	NetRect.bottom = (WndRect.bottom+h+WndRect.top)/2;
	wnd->MoveWindow(&NetRect);
}

void CSeaBattleDlg::SaveFile()
{
	int cnt, i, j;
	char buf[100];
	CString str;
	int data;

	m_ResFile.SetLength(0);
	cnt = m_Results.GetItemCount();
	for(i=0; i<cnt; i++)
	{
		m_Results.GetItemText(i, 0, buf, 100);
		for(j=0; j<100; j++)
			if(buf[j] == ' ') break;
		buf[j] = 11;
		j--;
		data = m_Results.GetItemData(i);
		{
			if(data/10)
				buf[j+2] = data/10;
			else
				buf[j+2] = 13;
			if(data%10)
				buf[j+3] = data%10;
			else
				buf[j+3] = 13;
		}
		if(i != cnt-1)
			buf[j+4] = 11;
		else
			buf[j+4] = 12;
		buf[j+5] = 0;
		m_ResFile.Write(buf, j+5);
	}
	str.Format("  <Data file for Sea Battle. All Rights reserved. NPT corporation. 2004. e-mail:uruc@rambler.ru>");
	m_ResFile.Write(str.GetBuffer(str.GetLength()), str.GetLength());
}
