//Comm.cpp Rs232c통신을 하기 위한 클래스
#include "stdafx.h"
#include "comm.h"
#include "define.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

DWORD procCommWatch ( LPVOID lpData);

/////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNCREATE(CComm, CObject)

	CComm::CComm( )
{
	m_overRead.hEvent	= 0;
	m_overWrite.hEvent	= 0;
	m_hComDev			= NULL;
	//m_bFlowCtrl		= FC_XONXOFF ;
	m_flgConnected		= FALSE ;
	//m_dwBaudRate		= DEF_BAUD_RATE;//m_pInitPort.m_dwRate;
}

CComm::~CComm( )
{
	DestroyComm();

	if(m_overRead.hEvent)
	{
		CloseHandle(m_overRead.hEvent);
		m_overRead.hEvent = NULL ;
	}

	if(m_overWrite.hEvent)
	{
		CloseHandle(m_overWrite.hEvent);
		m_overWrite.hEvent = NULL;
	}
}

//BEGIN_MESSAGE_MAP(CComm, CObject)
//{{AFX_MSG_MAP(CComm)
// NOTE - the ClassWizard will add and remove mapping macros here.
//}}AFX_MSG_MAP
//END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////


//com 포트를 열고 연결을 시도한다.
//OpenComport()
BOOL CComm::OpenComPort(LPTTYSTRUCT lpTTY )
{            
	char			szPort[ 64 ] = {0};
	COMMTIMEOUTS	CommTimeOuts ;
	CString			strText;

	if(m_nCommPort >=10)
		sprintf_s( szPort, 64,"\\\\.\\COM%d", lpTTY->byCommPort );
	else
		sprintf_s( szPort,64, "COM%d", lpTTY->byCommPort);


	// COMM device를 화일형식으로 연결한다.

	if ((m_hComDev =CreateFile( szPort, GENERIC_READ | GENERIC_WRITE,
		0,                    // exclusive access
		NULL,                 // no security attrs
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL | 
		FILE_FLAG_OVERLAPPED, // overlapped is I/O
		NULL )) == (HANDLE) -1 )
	{
		LPVOID lpMsgBuf;
		DWORD dw = GetLastError(); 

		FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER | 
			FORMAT_MESSAGE_FROM_SYSTEM,
			NULL,
			dw,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPTSTR) &lpMsgBuf,
			0, NULL );

		strText.Format( "COM%d Port Open failed with error %d: %s", m_nCommPort, dw, lpMsgBuf); 
		AfxMessageBox(strText);
	
		LocalFree(lpMsgBuf);
		return ( FALSE ) ;
	}
	else 
	{
		//컴포트에서 데이타를 교환하는 방법을 char단위를 기본으로 설정
		SetCommMask( m_hComDev, EV_RXCHAR ) ;
		SetupComm( m_hComDev, 4096, 4096 ) ;    // 포트의 버퍼를 4K 로 설정 
		PurgeComm( m_hComDev, PURGE_TXABORT | PURGE_RXABORT |PURGE_TXCLEAR | PURGE_RXCLEAR ) ; //디바이스에 쓰레기가 있을지 모르니까 깨끗이 청소를 하자!


		CommTimeOuts.ReadIntervalTimeout = 0xFFFFFFFF ;  
		CommTimeOuts.ReadTotalTimeoutMultiplier = 0 ;
		CommTimeOuts.ReadTotalTimeoutConstant = 300 ;   // Time Out 2초 
		CommTimeOuts.WriteTotalTimeoutMultiplier = 0 ;
		CommTimeOuts.WriteTotalTimeoutConstant = 300;

		SetCommTimeouts( m_hComDev, &CommTimeOuts ) ;
	}

	m_flgConnected = SetupConnection(lpTTY) ;

	if(!m_flgConnected)
	{
		m_flgConnected = FALSE ;
		CloseHandle( m_hComDev) ;
		
		CString strText;
		strText.Format("Com Port가 연결되지 않았습니다, 프로그램을 다시 시작하십시요.");
		AfxMessageBox(strText);
	}

	return ( m_flgConnected ) ;
} 

void ViewComPortError(DWORD errors)
{
	CString s;

	if(errors==0)
		return; 

	if(errors & CE_BREAK)
		s=s+"CE_BREAK The hardware detected a break condition.\r\n"; 

	if(errors & CE_DNS)
		s=s+"CE_DNS Windows 95 and Windows 98: A parallel device is not selected.\r\n"; 

	if(errors & CE_FRAME)
		s=s+"CE_FRAME The hardware detected a framing error. \r\n";

	if(errors & CE_IOE)
		s=s+"CE_IOE An I/O error occurred during communications with the device.\r\n"; 

	if(errors & CE_MODE)
	{
		s=s+"CE_MODE The requested mode is not supported, or the hFile parameter is invalid. \r\n";
		s=s+"If this value is specified, it is the only valid error. \r\n";
	} 
	if(errors & CE_OOP)
		s=s+"CE_OOP Windows 95 and Windows 98: A parallel device signaled that it is out of paper. \r\n";

	if(errors & CE_OVERRUN)
		s=s+"CE_OVERRUN A character-buffer overrun has occurred. The next character is lost. \r\n";

	if(errors & CE_PTO)
		s=s+"CE_PTO Windows 95 and Windows 98: A time-out occurred on a parallel device. \r\n"; 

	if(errors & CE_RXOVER)
	{
		s=s+"CE_RXOVER An input buffer overflow has occurred. There is either no room in the input buffer, \r\n";
		s=s+"or a character was received after the end-of-file (EOF)"
			" character. \r\n";
	} 

	if(errors & CE_RXPARITY)
		s=s+"CE_RXPARITY The hardware detected a parity error. \r\n"; 

	if(errors & CE_TXFULL)
		s=s+"CE_TXFULL The application tried to transmit a character, but the output buffer was full. \r\n"; 

	AfxMessageBox(s);
} 

//화일로 설정된 컴포트와 실질 포트와 연결을 시킨다.
//SetupConnection 이전에 CreateComPort를 해주어야 한다.
BOOL CComm::SetupConnection(LPTTYSTRUCT lpTTY)
{
	BOOL       fSuccess;
	DCB        dcb ={0} ;

	dcb.DCBlength = sizeof( DCB ) ;

	fSuccess = GetCommState(m_hComDev, &dcb);

	if (!fSuccess) 
	{
		ViewComPortError(GetLastError()) ;
		return FALSE;
	}

	//필요시 이부분을 수정.

	dcb.BaudRate = lpTTY->dwBaudRate;
	//dcb.BaudRate = 9600;
	dcb.ByteSize = lpTTY->byByteSize;
	dcb.Parity   = lpTTY->byParity;
	dcb.StopBits = lpTTY->byStopBits;

	/*
	dcb.fOutxDsrFlow	= 0 ;//Dsr Flow
	dcb.fOutxCtsFlow	= 0 ;//Cts Flow
	dcb.fDtrControl		= FALSE ;//Dtr Control
	dcb.fRtsControl		= FALSE ; //Ctr Control
	dcb.fInX= dcb.fOutX = 1 ; //XON/XOFF 관한것
	dcb.XonChar			= ASCII_XON ;
	dcb.XoffChar		= ASCII_XOFF ;
	dcb.XonLim			= 100 ;
	dcb.XoffLim			= 100 ;
	dcb.fParity			= TRUE ;
	*/
	// other various settings
	dcb.fNull   = FALSE;
	dcb.fBinary = TRUE;


	fSuccess = SetCommState(m_hComDev, &dcb);

	if(!fSuccess)
	{
		ViewComPortError(GetLastError()) ;
	}

	return ( fSuccess ) ;
} 

//컴포트를 완전히 해제한다.

BOOL CComm::DestroyComm()
{

	if (m_flgConnected)
	{
		CloseConnection( ) ;
		m_flgConnected	= FALSE;
	}

	return ( TRUE ) ;
} 

//컴포트 정보를 만든다.
//이것을 만들때 이전에 할일이
// SetComPort(); -> SetXonOff() ->SetDtrRts() 한다음 설정한다.
BOOL CComm::CreateCommInfo()
{
	m_overWrite.Offset = 0 ;
	m_overWrite.OffsetHigh = 0 ;
	m_overRead.Offset = 0 ;
	m_overRead.OffsetHigh = 0 ;

	//이벤트 창구 설정
	m_overRead.hEvent = CreateEvent( NULL, TRUE, FALSE, NULL ) ; 


	if (m_overRead.hEvent == NULL)
	{
		return FALSE ;
	}
	m_overWrite.hEvent = CreateEvent( NULL,   TRUE,  FALSE,   NULL ) ;


	if (NULL == m_overWrite.hEvent)
	{
		CloseHandle( m_overRead.hEvent ) ;
		return FALSE;
	}

	return TRUE ;
} 

BOOL CComm::ConnectionPort(LPTTYSTRUCT lpTTY)
{
	//컴포트를 맞춘다.

	m_nCommPort= lpTTY->byCommPort ;

	//컴포트의 정보를 만든다.
	CreateCommInfo();

	//컴토포트를 연다
	m_flgConnected	= OpenComPort( lpTTY);

	if(m_flgConnected)
		CreateCommThread(this);

	return m_flgConnected; 
}

//연결을 닫는다.
BOOL CComm::CloseConnection()
{

	// set connected flag to FALSE
	m_flgConnected = FALSE ;

	// disable event notification and wait for thread
	// to halt
	SetCommMask( m_hComDev, 0 ) ;
	EscapeCommFunction( m_hComDev, CLRDTR | SETXON | SETXOFF ) ;
	PurgeComm( m_hComDev, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR ) ;

	if(m_hComDev != NULL)
	{
		CloseHandle( m_hComDev ) ;
		m_hComDev = NULL ;
	}


	//이벤트 객체 해제
	if (m_overRead.hEvent != NULL)
	{
		CloseHandle(m_overRead.hEvent);
		m_overRead.hEvent = NULL;
	}

	if (NULL != m_overWrite.hEvent)
	{
		CloseHandle( m_overWrite.hEvent ) ;
		m_overWrite.hEvent = NULL ;
	}

	return ( TRUE ) ;

} 

BOOL CComm::WriteCommBlock( LPSTR lpByte, DWORD dwBytesToWrite)
{
	DWORD	dwWritten =0 , dwError =0, dwErrorFlags =0;
	COMSTAT	comstat;

	memset( &comstat, 0 , sizeof(COMSTAT));


	if (! WriteFile( m_hComDev, lpByte, dwBytesToWrite, &dwWritten, &m_overWrite))
	{
		int res = GetLastError() ;
		if (res == ERROR_IO_PENDING)
		{
			// 읽을 문자가 남아 있거나 전송할 문자가 남아 있을 경우 Overapped IO의
			// 특성에 따라 ERROR_IO_PENDING 에러 메시지가 전달된다.
			//timeouts에 정해준 시간만큼 기다려준다.
			while ( !(res=GetOverlappedResult( m_hComDev, &m_overWrite, &dwWritten, FALSE) ))
			{
				dwError = GetLastError();
				if (dwError != ERROR_IO_INCOMPLETE)
				{
					ClearCommError( m_hComDev, &dwErrorFlags, &comstat);
					return dwWritten;
				}
			}
		}
		else
		{
		//	g_pLogView->AddMessage("WriteCommBlock(Port:%d)  Error:%d\n", m_nCommPort, res);
			dwWritten = 0;
			ClearCommError( m_hComDev, &dwErrorFlags, &comstat);
		}
	}

	return dwWritten;
}

int CComm::ReadCommBlock(LPSTR lpszBlock, int nMaxLength )
{
	BOOL       fReadStat  = FALSE ;
	COMSTAT    ComStat ;
	DWORD      dwErrorFlags = 0;
	DWORD      dwLength		= 0;
	DWORD      dwLengthRead = 0;

	// only try to read number of bytes in queue 
	ClearCommError( m_hComDev, &dwErrorFlags, &ComStat) ;
	dwLength = min( (DWORD) nMaxLength, ComStat.cbInQue ) ;

	if(dwLength >0 )
	{
		if( (fReadStat = ReadFile(m_hComDev, lpszBlock,	dwLength, &dwLengthRead,&m_overRead)) == 0 )
		{
			int errcode = GetLastError() ;
			if(errcode == ERROR_IO_PENDING)
			{
				while(!GetOverlappedResult( m_hComDev, &m_overRead, &dwLengthRead,TRUE) )
				{
					if(GetLastError() == ERROR_IO_INCOMPLETE)
					{
						ClearCommError( m_hComDev, &dwErrorFlags, &ComStat) ;
						break;
					}
				}
				ReadFile(m_hComDev, lpszBlock,dwLength, &dwLengthRead,  &m_overRead);
			}
			else 
			{
				dwLength	= 0;
				ClearCommError( m_hComDev, &dwErrorFlags, &ComStat) ;
			}
			
		}
	}
	return dwLength;
}

DWORD procCommWatch(LPVOID lpData)
{
	DWORD		dwEvtMask ;
	OVERLAPPED	os ;
	CComm*		pComm = (CComm*) lpData ;
	BOOL		fOk =TRUE;
	DWORD		dwLength=0 ;
	CString		strLine ;
	BOOL		obj_flag = true ;
	//idCommDev 라는 핸들에 아무런 com 포트가 안붙어 있으면
	// 에라 리턴

	if ( pComm == NULL ||!pComm->IsKindOf( RUNTIME_CLASS( CComm ) ) )
		return (DWORD)(-1);

	memset( &os, 0, sizeof( OVERLAPPED ) ) ;

	os.hEvent = CreateEvent( NULL, TRUE,    // explicit reset req
		FALSE,   // initial event reset
		NULL ) ; // no name
	if ( !os.hEvent )
		fOk	= FALSE;

	DWORD dwEventFlags =  EV_RXCHAR ;

	if (!SetCommMask(pComm->m_hComDev, EV_RXCHAR ))
		fOk	= FALSE;

	if(!fOk) 
	{

		strLine.Format("Failed to create event for procCommWatch thread!, Com Port(%d) Error!" , pComm->m_nCommPort);
		AfxMessageBox(strLine);
		return ( FALSE ) ;
	}

	char	InData[MAXBLOCK]={0};
	CString m_strBuffer ;
	int cnt =0 , nPos =0;
	CString strError, strOldError;

	while (pComm->m_flgConnected )
	{
		SetCommMask(pComm->m_hComDev, EV_RXCHAR );
		dwEvtMask = 0 ;

		if(WaitCommEvent(pComm->m_hComDev, &dwEvtMask, NULL ) )
		{

			if ((dwEvtMask & EV_RXCHAR) == EV_RXCHAR)
			{
				do
				{
					memset((char*)InData, 0, MAXBLOCK);
					if (dwLength = pComm->ReadCommBlock((LPSTR) InData, MAXBLOCK ))
					{
						SendMessage(pComm->m_hSendHwnd, UM_READ_COMM, (WPARAM)InData, (LPARAM)dwLength);
					}
				}while ( dwLength > 0 ) ;

			}
		}
	}
	CloseHandle( os.hEvent );
	return( TRUE ) ;
}


CWinThread* CComm::CreateCommThread(LPVOID lParam)
{
	m_pThreadComm	= AfxBeginThread((AFX_THREADPROC)procCommWatch, (LPVOID)lParam);

	return m_pThreadComm;
}
