// MySocket.cpp: implementation of the CMySocket class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MySocket.h"
#include "define.h"
#include "SocketManager.h"
#include "NEW2Dlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMySocket::CMySocket() : CCESocket()
{
}

CMySocket::CMySocket(CNEW2Dlg* pView) : CCESocket()
{
	m_New2Dlg = pView;
}


CMySocket::~CMySocket()
{
}

bool CMySocket::OnAccept(SOCKET serviceSocket)
{
	if (m_New2Dlg)
		m_New2Dlg->SendMessage(UM_SOCKET_ACCEPT, (WPARAM)serviceSocket, 0);

	return 1 ;
}

void CMySocket::OnClose(int closeEvent)
{
	if (m_New2Dlg)
		m_New2Dlg->PostMessage(UM_SOCKET_CLOSE, (WPARAM) this->s, 0);
}

bool CMySocket::OnReceive(char* buf, int len)
{
	if (m_socketState != CONNECTED)
		return false;
	m_New2Dlg->SendMessage(UM_TCP_RECEIVE, (WPARAM)buf, (LPARAM)len);
	return 1;
}

