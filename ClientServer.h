#include <Winsock2.h>
#pragma comment (lib,"Ws2_32")

#define UDP_PORT 20000
#define TCP_PORT 20001
#define MULTI_ADDR	"224.0.0.1"

//messages
#define WHOSERVER 127
#define IAMSERVER 126
#define CLOSECONNECT 125
#define STARTCONNECT 124
#define ENDSERVER 123

//states
#define NOTCREATED 10000
#define READY 10001
#define CONNECTED 10002

struct ADDRDATA
{
	char name[64];
	DWORD addr;
};

ADDRDATA Data[5];
int count;
DWORD OwnAddr, ForAddr;
SOCKET MultiSock, Sock, ServSock;
CSeaBattleDlg *wnd;
char ServName[64];

int SendBroadcast(int msg);
int GetCount();
void Restart();
void TimeOut(int sec);
void Start();
void Exit();
SOCKET CreateSocket(DWORD dwThisAddr,int Port,bool bMulti);
DWORD WINAPI RecvData(LPVOID lpParameter);
int AcceptSocket();
DWORD WINAPI RecvBroadcastMsg(LPVOID lpParameter);
DWORD WINAPI AcceptSocket(LPVOID lpParameter);
void RelStr(char* ist, char* prim);
void SetStr(char* ist, char* prim);
int Connect();
int SendData(char *buf, int lenght);
void Refresh();
int ConnectTo(DWORD addr);

int ConnectTo(DWORD addr)
{
	SOCKADDR_IN data;
	char c;
	SOCKET dgramsock;
	int len = sizeof(data);
	SYSTEMTIME time;
	int PrevSec, CurSec;
	unsigned long ID;

	if(ServSock || Sock)
		if(::AfxMessageBox("Желаете прервать соединение?", MB_YESNO) == IDNO)
			return 0;
	if(ServSock)
	{
		SendBroadcast(ENDSERVER);
		closesocket(ServSock);
		ServSock = 0;
	}
	if(Sock)
	{
		c = CLOSECONNECT;
		SendData(&c, 1);
		closesocket(Sock);
		Sock = 0;
	}
	Sock = socket(AF_INET, SOCK_STREAM, 0);
	dgramsock = socket(AF_INET, SOCK_DGRAM, 0);
	data.sin_family = AF_INET;
	data.sin_port = htons(UDP_PORT-1);
	data.sin_addr.S_un.S_addr = OwnAddr;
	if(bind(dgramsock, (sockaddr*)&data, len) == SOCKET_ERROR)
		return SOCKET_ERROR;
	c = STARTCONNECT;
	data.sin_addr.S_un.S_addr = addr;
	data.sin_port = htons(UDP_PORT);
	if(sendto(dgramsock, &c, 1, MSG_DONTROUTE, (sockaddr*)&data, len) == SOCKET_ERROR)
		return SOCKET_ERROR;

	data.sin_port = htons(TCP_PORT);
	::GetSystemTime(&time);
	PrevSec = time.wHour*3600 + time.wSecond + time.wMinute*60;
	while(true)
	{
		if(connect(Sock, (sockaddr*)&data, len) != SOCKET_ERROR)
		{
			CreateThread(NULL, NULL, RecvData, 0, NULL, &ID);
			ForAddr = data.sin_addr.S_un.S_addr;
			wnd->m_Process = NET_GAME;
			wnd->NetworkWnd.ShowWindow(SW_HIDE);
			wnd->OwnMove = false;
			wnd->m_ForeignField.IsNet = wnd->m_OwnField.IsNet = 1;
			::SendMessage(wnd->m_hWnd, WM_STATIC_TO_DLG, WM_INV_STR, 0);
			closesocket(dgramsock);
			dgramsock = 0;
			return 0;
		}
		::GetSystemTime(&time);
		CurSec = time.wHour*3600 + time.wSecond + time.wMinute*60;
		if(CurSec - PrevSec >= 2) 
			break;
	}	
	closesocket(Sock);
	closesocket(dgramsock);
	dgramsock = 0;
	Sock = 0;
	return SOCKET_ERROR;
}

void Refresh()
{
	int i;

	count = 0;
	for(i=0; i<5; i++)
	{
		if(Sock && ForAddr == Data[i].addr) { count++; continue; }
		Data[i].addr = 0;
	}
	SendBroadcast(WHOSERVER);
	TimeOut(2);
}
int SendData(char *buf, int lenght)
{
	if(!Sock || !ForAddr) return SOCKET_ERROR;
	return send(Sock, buf, lenght, MSG_DONTROUTE);
}


int SendBroadcast(int msg)
{
	SOCKADDR_IN addr;
	char c;

	c = msg;
	addr.sin_port = htons(UDP_PORT);
	addr.sin_family = AF_INET;
	addr.sin_addr.S_un.S_addr = inet_addr(MULTI_ADDR);
	if(sendto(MultiSock, &c, 1, MSG_DONTROUTE, (sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR)
		return SOCKET_ERROR;
	else 
		return 0;
}
int GetCount()
{
	return count;
}
int Connect()
{
	SYSTEMTIME time;
	long PrevSec, CurSec;
	int cursel;
	SOCKADDR_IN addr;
	unsigned long ID;
	char c;

	if(ServSock || Sock)
		if(::AfxMessageBox("Желаете прервать соединение?", MB_YESNO) == IDNO)
			return 0;
	if(ServSock) 
	{
		SendBroadcast(ENDSERVER);
		closesocket(ServSock);
		ServSock = 0;
	}
	if(Sock)
	{
		c = CLOSECONNECT;
		SendData(&c, 1);
		closesocket(Sock);
		Sock = 0;
	}
	
	cursel = wnd->NetworkWnd.m_Servers.GetCurSel();
	Sock = socket(AF_INET, SOCK_STREAM, 0);
	addr.sin_family = AF_INET;
	addr.sin_port = htons(UDP_PORT);
	addr.sin_addr.S_un.S_addr = Data[cursel].addr;

	c = STARTCONNECT;
	if(sendto(MultiSock, &c, 1, MSG_DONTROUTE, (sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR)
		return SOCKET_ERROR;

	addr.sin_port = htons(TCP_PORT);
	::GetSystemTime(&time);
	PrevSec = time.wHour*3600 + time.wSecond + time.wMinute*60;
	while(true)
	{
		if(connect(Sock, (sockaddr*)&addr, sizeof(addr)) != SOCKET_ERROR)
		{
			CreateThread(NULL, NULL, RecvData, 0, NULL, &ID);
			ForAddr = addr.sin_addr.S_un.S_addr;
			wnd->m_Process = NET_GAME;
			wnd->NetworkWnd.ShowWindow(SW_HIDE);
			wnd->OwnMove = false;
			wnd->m_ForeignField.IsNet = wnd->m_OwnField.IsNet = 1;
			::SendMessage(wnd->m_hWnd, WM_STATIC_TO_DLG, WM_INV_STR, 0);
			return 0;
		}
		::GetSystemTime(&time);
		CurSec = time.wHour*3600 + time.wSecond + time.wMinute*60;
		if(CurSec - PrevSec >= 2) 
			break;
	}	
	closesocket(Sock);
	Sock = 0;
	return SOCKET_ERROR;
}
void SetStr(char* ist, char* prim)
{
	int i = 0;

	prim[0] = IAMSERVER;
	while(true)
	{
		prim[i+1] = ist[i];
		if(ist[i] == 0) break;
		i++;
	}
}

void RelStr(char* ist, char* prim)
{
	int i = 0;

	while(true)
	{
		prim[i] = ist[i+1];
		if(ist[i+1] == 0) break;
		i++;
	}
}

void Restart()
{
	if(MultiSock) Exit();
	Start();
}

void Exit()
{
	if(Sock) closesocket(Sock);
	if(ServSock) closesocket(ServSock);
	closesocket(MultiSock);
	ServSock = Sock = MultiSock = 0;
	WSACleanup();
}
void Start()
{	
	WSADATA data;
	char name[64], c;
	hostent* phost;
	char *ip;
	SOCKADDR_IN addr;
	unsigned long ID;
	int len;
	CString str;

	WSAStartup(MAKEWORD(1, 1), &data);
	gethostname(name, sizeof(name));
	phost = gethostbyname(name);
	ip = inet_ntoa(*(in_addr *)(*phost->h_addr_list));
	str.Format("Сетевая игра. IP компа: %s", ip);
	wnd->NetworkWnd.SetWindowText(str);
	ServSock = Sock = 0;
	ForAddr = 0;
	OwnAddr = inet_addr(ip);
	count = 0;
	for(len=0; len<5; len++)
		Data[len].addr = 0;
	ServName[0] = 0;

/*
	MultiSock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if(MultiSock == INVALID_SOCKET)
		::AfxMessageBox("Can't create broadcast socket");
	addr.sin_family = AF_INET;
	addr.sin_port = htons(UDP_PORT);
	addr.sin_addr.S_un.S_addr = INADDR_ANY;
	if(bind(MultiSock, (sockaddr *)&addr, sizeof(addr)) == SOCKET_ERROR)
		::AfxMessageBox("Can't bind socket");
	addr.sin_port = htons(UDP_PORT-1);
	addr.sin_addr.S_un.S_addr = OwnAddr;
	len = sizeof(addr);
	c = WHOSERVER;
	sendto(MultiSock, &c, 1, MSG_DONTROUTE, (sockaddr*)&addr, len);
*/

	MultiSock = CreateSocket(OwnAddr, UDP_PORT, true);
	SendBroadcast(WHOSERVER);

	CreateThread(NULL, 0, RecvBroadcastMsg, 0, 0, &ID);
}

SOCKET CreateSocket(DWORD dwThisAddr,int Port,bool bMulti)
{
	SOCKADDR_IN Addr;
	int l=sizeof(SOCKADDR_IN);
	SOCKET sock,tmp;
	DWORD MultiFlag=WSA_FLAG_MULTIPOINT_C_LEAF | WSA_FLAG_MULTIPOINT_D_LEAF;

	Addr.sin_family = AF_INET;
//	Addr.sin_addr.S_un.S_addr=GlobalExchangeData::ThisIPAddr;
	Addr.sin_addr.S_un.S_addr=INADDR_ANY;;
	Addr.sin_port = htons(Port);
	if(bMulti)
	{
		sock=WSASocket(AF_INET,SOCK_DGRAM,IPPROTO_UDP,NULL,0,      
					   WSA_FLAG_OVERLAPPED | (true && bMulti)*MultiFlag);
		setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,(char *)&tmp,sizeof(tmp));
	}
	else sock=socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
	if(bind(sock,(PSOCKADDR)&Addr,l))
	{
		closesocket(sock);
		return INVALID_SOCKET;
	}
	if(bMulti)
	{
		Addr.sin_addr.S_un.S_addr=inet_addr(MULTI_ADDR);
		tmp=WSAJoinLeaf(sock,(PSOCKADDR)&Addr,l,
						NULL,NULL,NULL,NULL,JL_BOTH);
		if(tmp==INVALID_SOCKET) closesocket(sock);
		sock=tmp;
	}
	return sock;
}

DWORD WINAPI RecvData(LPVOID lpParameter)
{
	char buf[1000];
	int error, i;
	char c;

	while(true)
	{
		i = recv(Sock, buf, sizeof(buf), 0);
		error = WSAGetLastError();
		if(error == WSAESHUTDOWN || (buf[0] == CLOSECONNECT && i>0)) 
		{
			::AfxMessageBox("Соединение разорвано");
			closesocket(Sock);
			if(ServSock) closesocket(ServSock);
			Sock = ServSock = 0;
			wnd->m_Process = FUCK;
			wnd->m_ForeignField.IsNet = wnd->m_OwnField.IsNet = 0;
			wnd->m_ForeignField.m_Operation = wnd->m_OwnField.m_Operation = NONE;
			::SendMessage(wnd->m_hWnd, WM_STATIC_TO_DLG, WM_INV_STR, 0);
			ExitThread(0);
			return false;
		}
		if(error != 0) continue;
		if(i == 1 && buf[0] == N_NO_WIN_YOU)
			::SendMessage(wnd->m_hWnd, WM_STATIC_TO_DLG, WM_WIN_COMP, 0);
		if(i == 1 && buf[0] == N_WIN_YOU)
			::SendMessage(wnd->m_hWnd, WM_STATIC_TO_DLG, WM_WIN_YOU, 0);
		if(i == 2 && buf[0] == N_MESSAGE)
		{
			if(wnd->m_ForeignField.ReceiveMessage(N_MESSAGE, buf[1]) == 10)
			{
				c = N_NO_WIN_YOU;
				SendData(&c, 1);
			}
			if(buf[1] != N_EMPTY) continue;
		}
		if(buf[0] >= 0 && buf[0] < 10 && buf[1] >= 0 && buf[1] < 10)
			wnd->m_OwnField.RecvNetData(buf[1], buf[0]);
	}
	ExitThread(0);
	return true;
}

int Accept()
{
	SOCKADDR_IN addr;
	int len = sizeof(addr);
	char buf[70], c;

	if(ServSock || Sock)
		if(::AfxMessageBox("Желаете прервать соединение?", MB_YESNO) == IDNO)
			return 0;
	if(ServSock) 
	{
		SendBroadcast(ENDSERVER);
		closesocket(ServSock);
		ServSock = 0;
	}
	if(Sock)
	{
		c = CLOSECONNECT;
		SendData(&c, 1);
		closesocket(Sock);
		Sock = 0;
	}

	ServSock = socket(AF_INET, SOCK_STREAM, 0);
	addr.sin_family = AF_INET;
	addr.sin_port = htons(TCP_PORT);
	addr.sin_addr.S_un.S_addr = OwnAddr;
	if(bind(ServSock, (sockaddr *)&addr, len) == SOCKET_ERROR)
	{
		closesocket(ServSock);
		ServSock = 0;
		return SOCKET_ERROR;
	}
	wnd->m_Process = FUCK;
	listen(ServSock, 1);
	wnd->GetServName(ServName);
	if(ServName[0] == 0) gethostname(ServName, sizeof(ServName));
	SetStr(ServName, buf);
	addr.sin_port = htons(UDP_PORT);
	addr.sin_family = AF_INET;
	addr.sin_addr.S_un.S_addr = inet_addr(MULTI_ADDR);
	sendto(MultiSock, buf, sizeof(buf), MSG_DONTROUTE, (sockaddr*)&addr, sizeof(addr));
	return 0;
}

DWORD WINAPI RecvBroadcastMsg(LPVOID lpParameter)
{
	SOCKADDR_IN addr;
	char buf[70];
	CString str;
	int len, i, error, j;
	unsigned long ID;

	len = sizeof(addr);
	while(true)
	{
		i = recvfrom(MultiSock, buf, sizeof(buf), 0, (sockaddr*)&addr, &len);
		error = WSAGetLastError();
		if(error == WSAESHUTDOWN) ExitThread(0);
		if(i == 1 && buf[0] == ENDSERVER)
		{
			for(i=0; i<count; i++)
			{
				if(Data[i].addr == addr.sin_addr.S_un.S_addr)
				{
					count--;
					for(j=i; j<count; j++)
						Data[j] = Data[j+1];
					wnd->RefreshList();
					break;
				}
			}
		}
		if(buf[0] == WHOSERVER && i == 1 &&
			OwnAddr != addr.sin_addr.S_un.S_addr &&
			ServSock && !Sock)
		{
			SetStr(ServName, buf);
			sendto(MultiSock, buf, sizeof(buf), MSG_DONTROUTE, (sockaddr*)&addr, len);
		}
		if(buf[0] == IAMSERVER 
			&& OwnAddr != addr.sin_addr.S_un.S_addr && count < 5)
		{
			Data[count].addr = addr.sin_addr.S_un.S_addr;
			RelStr(buf, Data[count].name);
			count++;
		}
		if(i == 1 && buf[0] == STARTCONNECT && ServSock && !Sock)
		{
			len = sizeof(addr);
			Sock = accept(ServSock, (sockaddr*)&addr, &len);
			if(Sock == INVALID_SOCKET)
			{
				::AfxMessageBox("Невозможно создать сервер", MB_SYSTEMMODAL);
				continue;
			}
			::AfxMessageBox("Присоединился игрок!",MB_SYSTEMMODAL);
			wnd->NetworkWnd.ShowWindow(SW_HIDE);
			CreateThread(NULL, 0, RecvData, 0, 0, &ID);
			ForAddr = addr.sin_addr.S_un.S_addr;
			SendBroadcast(ENDSERVER);
			wnd->OwnMove = true;
			wnd->m_Process = NET_GAME;
			wnd->m_ForeignField.IsNet = wnd->m_OwnField.IsNet = 1;
			::SendMessage(wnd->m_hWnd, WM_STATIC_TO_DLG, WM_INV_STR, 0);
			::SendMessage(wnd->m_hWnd, WM_STATIC_TO_DLG, WM_NEW_GAME, 0);
			wnd->m_ForeignField.ReceiveMessage(WM_FIRE, 0);
		}
	}
	ExitThread(0);
	return 0;
}

void TimeOut(int sec)
{
	SYSTEMTIME time;
	long PrevSec, CurSec;
	::GetSystemTime(&time);
	PrevSec = time.wHour*3600 + time.wSecond + time.wMinute*60;
	while(true)
	{
		::GetSystemTime(&time);
		CurSec = time.wHour*3600 + time.wSecond + time.wMinute*60;
		if(CurSec - PrevSec >= sec) 
			break;
	}
}
