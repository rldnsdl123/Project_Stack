// SocketManager.cpp: implementation of the CSocketManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SocketManager.h"
#include "define.h"
#include "NEW2Dlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSocketManager::CSocketManager()
{
	m_bServerRun	= FALSE;
	m_CNEW2Dlg		= NULL ;
	m_pClientSocket = NULL ;
	m_pAcceptSocket	= NULL ;

}

CSocketManager::~CSocketManager()
{
	m_pAcceptSocket->Disconnect();

	if(m_pAcceptSocket != NULL)
		delete m_pAcceptSocket ;

}

void CSocketManager::Accept(SOCKET serviceSocket) 
{
	CString strMsg ;
	
	if (m_pClientSocket != NULL)
	{
		m_pClientSocket->Disconnect();
		delete m_pClientSocket;
	}

	m_pClientSocket = new CMySocket(m_CNEW2Dlg);

	if (m_pClientSocket == NULL)
	{
		m_CNEW2Dlg->m_listLog.AddString("ClientSocket 생성실패");
		return ;
	}
	m_pClientSocket->AcceptServiceSocket(serviceSocket);
	m_pClientSocket->m_strIP = GetIPAddress(m_pClientSocket->s);

	strMsg.Format("%s가 접속하였습니다.", m_pClientSocket->m_strIP);
	m_CNEW2Dlg->m_listLog.AddString(strMsg);
}

//내가 추가한 함수
CString GetIPAddress(SOCKET s)
{
	SOCKADDR_IN sin;
	int nameSize = sizeof(struct sockaddr_in);
	getpeername(s, (SOCKADDR*)&sin, &nameSize);

	CString strIP;

	strIP.Format("%d.%d.%d.%d", (int)sin.sin_addr.S_un.S_un_b.s_b1,
		(int)sin.sin_addr.S_un.S_un_b.s_b2,
		(int)sin.sin_addr.S_un.S_un_b.s_b3,
		(int)sin.sin_addr.S_un.S_un_b.s_b4);

	return strIP;
}


void CSocketManager::Close(SOCKET s )
{
	CMySocket* pSocket = NULL;
	CString strMsg ;
	
	if (m_pClientSocket == NULL)
		return;

	if (m_pClientSocket->GetSocket() == s)
	{
		strMsg.Format("%s가 접속하였습니다.", m_pClientSocket->m_strIP);
		m_CNEW2Dlg->m_listLog.AddString(strMsg);
	
		m_pClientSocket->Disconnect();
		delete m_pClientSocket;
		m_pClientSocket = NULL;
	}
}

void CSocketManager::Disconnect()
{
	CString strMsg;
	if (m_pClientSocket != NULL)
	{
		strMsg.Format("%s가 접속하였습니다.", m_pClientSocket->m_strIP);
		m_CNEW2Dlg->m_listLog.AddString(strMsg);
	
		m_pClientSocket->Disconnect();
		delete m_pClientSocket;
		m_pClientSocket = NULL;
	}
}

void CSocketManager::Close()
{
	if (m_pClientSocket != NULL)
	{
		m_pClientSocket->Disconnect();
		delete m_pClientSocket;
		m_pClientSocket = NULL;
	}

	if (m_pAcceptSocket != NULL)
	{
		m_pAcceptSocket->Disconnect();
		delete m_pAcceptSocket;
		m_pAcceptSocket = NULL;
	}
	m_bServerRun = FALSE;
}


void CSocketManager::ServerStop(void)
{
	CString strMsg;
	Close();
	strMsg.Format("TCP 서버(%s)가 중지됐습니다.", m_pClientSocket->m_strIP);
	m_CNEW2Dlg->m_listLog.AddString(strMsg);
}


BOOL CSocketManager::TCPServerStart(CNEW2Dlg* pNew2, int listen_port)
{
	m_CNEW2Dlg = pNew2;
	if (m_pAcceptSocket != NULL)
	{
		m_pAcceptSocket->Disconnect();
		delete m_pAcceptSocket;
		m_pAcceptSocket = NULL;
	}
	m_pAcceptSocket = new CMySocket(pNew2);
	m_bServerRun	= m_pAcceptSocket->Create(SOCK_STREAM);

	if (m_bServerRun)
		m_pAcceptSocket->Accept(listen_port);

	return m_bServerRun;
}


