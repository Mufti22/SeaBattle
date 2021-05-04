// MySocket.h: interface for the CMySocket class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYSOCKET_H__83E211E0_0FC3_11D8_9461_9AB2745A8561__INCLUDED_)
#define AFX_MYSOCKET_H__83E211E0_0FC3_11D8_9461_9AB2745A8561__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define PORT_ADDR 5555

class CMySocket  
{
public:
	bool StartServer();
	bool Create();
	CMySocket();
	virtual ~CMySocket();
private:
	SOCKET m_ServSock, m_ClientSock;
};

#endif // !defined(AFX_MYSOCKET_H__83E211E0_0FC3_11D8_9461_9AB2745A8561__INCLUDED_)
