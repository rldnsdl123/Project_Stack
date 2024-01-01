// MySocket.h: interface for the CMySocket class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYSOCKET_H__13E94208_E06B_4E7E_8985_3B4E64E8E817__INCLUDED_)
#define AFX_MYSOCKET_H__13E94208_E06B_4E7E_8985_3B4E64E8E817__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CESocket.h"
class CNEW2Dlg;

class CMySocket : public CCESocket  
{
public:
	CMySocket();
	CMySocket(CNEW2Dlg* parent);
	virtual ~CMySocket();

	virtual bool OnAccept(SOCKET serviceSocket);
	virtual void OnClose(int closeEvent);
	virtual bool OnReceive(char* buf, int len);

protected:
	CNEW2Dlg* m_New2Dlg; //cmysocket accept함수가 new2dlg에게 알려주기 위해서
public:
	HWND		m_RefhWnd;
	CString		m_strIP;
	int			m_nPort;
	CStringA	m_m_strBuffer ;
};

#endif // !defined(AFX_MYSOCKET_H__13E94208_E06B_4E7E_8985_3B4E64E8E817__INCLUDED_)
