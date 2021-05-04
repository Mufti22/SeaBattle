// NetworkDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SeaBattle.h"
#include "NetworkDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNetworkDlg dialog

int Connect();
int Accept();

CNetworkDlg::CNetworkDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNetworkDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNetworkDlg)
	//}}AFX_DATA_INIT
	IsByHand = false;
}


void CNetworkDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNetworkDlg)
	DDX_Control(pDX, IDC_CANSEL, m_Cansel);
	DDX_Control(pDX, IDC_LIST_OF_SERVERS, m_Static);
	DDX_Control(pDX, IDC_OK_CON, m_Ok);
	DDX_Control(pDX, IDC_IPADDRESS3, m_IPAddr);
	DDX_Control(pDX, IDC_LIST1, m_Servers);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNetworkDlg, CDialog)
	//{{AFX_MSG_MAP(CNetworkDlg)
	ON_BN_CLICKED(ID_CONNECT, OnConnect)
	ON_BN_CLICKED(ID_CREATE_SERV, OnCreateServ)
	ON_WM_CREATE()
	ON_BN_CLICKED(ID_REFRESH, OnRefresh)
	ON_BN_CLICKED(ID_BY_HAND, OnByHand)
	ON_BN_CLICKED(IDC_OK_CON, OnOkCon)
	ON_BN_CLICKED(IDC_CANSEL, OnCansel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNetworkDlg message handlers
int GetCount();
void CNetworkDlg::OnConnect() 
{
	if(!GetCount()) 
	{
		::AfxMessageBox("Нету доступных серверов");
		return;
	}
	if(m_Servers.GetCurSel() == LB_ERR)
	{
		::AfxMessageBox("Выделите сервер для подключения");
		return;
	}
	if(!Connect()) 
	{
		::AfxMessageBox("Можно начать играть", MB_SYSTEMMODAL);
		::SendMessage(parent->m_hWnd, WM_STATIC_TO_DLG, WM_NEW_GAME, 0);
	}
	else
		::AfxMessageBox("Невозможно соединиться с сервером");
}

void CNetworkDlg::OnCreateServ() 
{
	if(Accept())
		::AfxMessageBox("Невозможно создать сервер");
	else
		::SendMessage(parent->m_hWnd, WM_STATIC_TO_DLG, WM_INV_STR, 0);
}

int CNetworkDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	return 0;
}

void Refresh();
void CNetworkDlg::OnRefresh() 
{
	if(!parent) return;
	Refresh();	
	::SendMessage(parent->m_hWnd, WM_STATIC_TO_DLG, WM_REFRESH_LIST, 0);
}

void CNetworkDlg::OnByHand() 
{
	m_Static.ShowWindow(SW_HIDE);
	m_Servers.ShowWindow(SW_HIDE);
	m_IPAddr.ShowWindow(SW_SHOW);
	m_Ok.ShowWindow(SW_SHOW);
	m_Cansel.ShowWindow(SW_SHOW);
}

BOOL CNetworkDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_IPAddr.ShowWindow(SW_HIDE);
	m_Ok.ShowWindow(SW_HIDE);
	m_Cansel.ShowWindow(SW_HIDE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
int ConnectTo(DWORD addr);
void CNetworkDlg::OnOkCon() 
{
	DWORD addr;

	m_IPAddr.GetAddress(addr);
	if(!addr)
		::AfxMessageBox("Введите IP адресс");
	else
	{
		if(!ConnectTo(addr)) 
		{
			::AfxMessageBox("Можно начать играть", MB_SYSTEMMODAL);
			::SendMessage(parent->m_hWnd, WM_STATIC_TO_DLG, WM_NEW_GAME, 0);
		}
		else
			::AfxMessageBox("Невозможно соединиться с сервером");
	}
}

void CNetworkDlg::OnCansel() 
{
	m_IPAddr.ShowWindow(SW_HIDE);
	m_Ok.ShowWindow(SW_HIDE);
	m_Cansel.ShowWindow(SW_HIDE);
	m_Servers.ShowWindow(SW_SHOW);
	m_Static.ShowWindow(SW_SHOW);
}
