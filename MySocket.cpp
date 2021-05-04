// MySocket.cpp: implementation of the CMySocket class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SeaBattle.h"
#include "MySocket.h"
//#include "winsock.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMySocket::CMySocket()
{
	WSADATA data;
	WSAStartup(MAKEWORD(1, 1), &data);
	m_ServSock = 0;
	m_ClientSock = 0;
}

CMySocket::~CMySocket()
{
	if(m_ServSock != 0) closesocket(m_ServSock);
	if(m_ClientSock != 0) closesocket(m_ClientSock);
	WSACleanup();
}

bool CMySocket::Create()
{
	SOCKADDR_IN addr;

	if(socket(PF_INET, SOCK_STREAM, 0) == INVALID_SOCKET)
	{
		::AfxMessageBox("Невозможно создать socket");
		return 0;
	}
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr  = INADDR_ANY; 
	addr.sin_port = htons(PORT_ADDR);
	if(bind(m_ServSock, (SOCKADDR *)&addr, sizeof(addr)) == SOCKET_ERROR)
	{
		::AfxMessageBox("Невозможно связать socket с портом");
		return 0;
	}
	return 1;
}

bool CMySocket::StartServer()
{
	SOCKADDR_IN addr;
	int lenght;

	if(listen(m_ServSock, 2) == SOCKET_ERROR)
	{
		::AfxMessageBox("Невозможна работа сервера");
		return 0;
	}
	m_ClientSock = accept(m_ServSock, (SOCKADDR *)&addr, &lenght);
	if(m_ClientSock == INVALID_SOCKET)
	{
		::AfxMessageBox("Невозможно подключение клиента");
		return 0;
	}
	return 1;
}
