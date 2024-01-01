// SocketManager.h: interface for the CSocketManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SOCKETMANAGER_H__738A0BEA_A81F_44DE_9B0E_4BD6F0D2DFA0__INCLUDED_)
#define AFX_SOCKETMANAGER_H__738A0BEA_A81F_44DE_9B0E_4BD6F0D2DFA0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <map>

#include "define.h"
#include"MySocket.h"


extern  CString GetIPAddress(SOCKET s);

class CNEW2Dlg;

class	CSocketManager   
{
public:
	void Close(); //연결되어있을 경우 연결끊고 초기화 전부다 해제
	BOOL m_bServerRun; //서버가 작동하고있는지 확인할수있는 BOOL변수
	void Accept(SOCKET serviceSocket); //소켓 통신을 받아들이는 부분
	void Close(SOCKET s ); // 특정 소켓한개만 해제 파라미터로 전달받음
	

	CSocketManager(); //생성자
	virtual ~CSocketManager();//가상 소멸자

	CMySocket*	m_pClientSocket; // 새로운 클라이언트가 들어왔을떄 받아서 자기가 통신함
	CMySocket*	m_pAcceptSocket; // 접속을 받아서 넘겨주기만 함, 대기만함

	CNEW2Dlg*	m_CNEW2Dlg;

	void ServerStop(void);
	BOOL TCPServerStart(CNEW2Dlg* pNew2, int listen_port); //
	void Disconnect();
};

#endif // !defined(AFX_SOCKETMANAGER_H__738A0BEA_A81F_44DE_9B0E_4BD6F0D2DFA0__INCLUDED_)
