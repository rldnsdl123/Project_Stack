//Comm.h
#if !defined(_COMMDEV)
#define _COMMDEV

//Rs232c를 하기위한 클래스 헤더
#define MAXBLOCK        4096
// Flow control flags

#define FC_DTRDSR       0x01
#define FC_RTSCTS       0x02
#define FC_XONXOFF      0x04

// ascii definitions

#define ASCII_BEL       0x07
#define ASCII_BS        0x08
#define ASCII_LF        0x0A
#define ASCII_CR        0x0D
#define ASCII_XON       0x11
#define ASCII_XOFF      0x13

#pragma  pack(push,1)

typedef struct _TTYSTRUCT
{
   BYTE		byCommPort;  // zero based port - 3 or higher implies TELNET
   BYTE		byXonXoff;
   BYTE		byByteSize;
   BYTE		byFlowCtrl;
   BYTE		byParity;
   BYTE		byStopBits;
   DWORD	dwBaudRate;

} TTYSTRUCT, *LPTTYSTRUCT;

#pragma  pack(pop,1)

#define DEF_BAUD_RATE	9600
/////////////////////////////////////////////////////////////////////////////
// CComm window

void ViewComPortError(DWORD errors);


class CComm : public CObject
{
   DECLARE_DYNCREATE( CComm )
public:

   // Construction
public:
   CComm( );
	virtual	~CComm();

	// Attributes
public:
	int	m_dwBaudrate	;
	int	m_nDatabit		;
	int	m_nStopbit		;
	int	m_nParitybit	;
	int	m_nMessageID ;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CComm)
	//}}AFX_VIRTUAL

// Implementation
public:

	BOOL	ConnectionPort (LPTTYSTRUCT lpTTY);
	int		ReadCommBlock(LPSTR lpszBlock, int nMaxLength );

	BOOL	DestroyComm();			//comm 포트를 해제한다.
	BOOL	WriteCommBlock( LPSTR szBuffer, DWORD dwLength);   //컴포트에 데이타를 넣는다.
	BOOL	OpenComPort(LPTTYSTRUCT lpTTY) ;			//컴포트를 열고 연결을 시도한다.
	BOOL	SetupConnection(LPTTYSTRUCT lpTTY ) ;	//포트를 연결한다.
	BOOL	CloseConnection( ) ;	//연결을 해제한다.
	CWinThread*	CreateCommThread(LPVOID lParam);

	HWND		m_hSendHwnd;
	void		SetSendHandle(HWND hWnd) { m_hSendHwnd = hWnd; };
	BOOL        m_flgConnected;			// 컴포트가 연결되면 1로 설정
	BOOL		CreateCommInfo();

public:
	int			m_iReadSize;
	int			m_nCommPort;
	CWinThread*	m_pThreadComm ;
	HANDLE      m_hComDev ;				// 컴포트 디바이스 연결 핸들
	OVERLAPPED	m_overWrite, m_overRead ;

	BOOL        m_flgXonXoff;
};

#endif

/////////////////////////////////////////////////////////////////////////////
