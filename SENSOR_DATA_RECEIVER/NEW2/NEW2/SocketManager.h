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
	void Close(); //����Ǿ����� ��� ������� �ʱ�ȭ ���δ� ����
	BOOL m_bServerRun; //������ �۵��ϰ��ִ��� Ȯ���Ҽ��ִ� BOOL����
	void Accept(SOCKET serviceSocket); //���� ����� �޾Ƶ��̴� �κ�
	void Close(SOCKET s ); // Ư�� �����Ѱ��� ���� �Ķ���ͷ� ���޹���
	

	CSocketManager(); //������
	virtual ~CSocketManager();//���� �Ҹ���

	CMySocket*	m_pClientSocket; // ���ο� Ŭ���̾�Ʈ�� �������� �޾Ƽ� �ڱⰡ �����
	CMySocket*	m_pAcceptSocket; // ������ �޾Ƽ� �Ѱ��ֱ⸸ ��, ��⸸��

	CNEW2Dlg*	m_CNEW2Dlg;

	void ServerStop(void);
	BOOL TCPServerStart(CNEW2Dlg* pNew2, int listen_port); //
	void Disconnect();
};

#endif // !defined(AFX_SOCKETMANAGER_H__738A0BEA_A81F_44DE_9B0E_4BD6F0D2DFA0__INCLUDED_)
