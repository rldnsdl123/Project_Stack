#pragma once

// to express floating point number in communication packet
// it should be 4 bytes, transmit from MSB of mantisa

#define error(x) exit(x);

#define  BUFFER_4096    4096

#define	 CBR_230400		230400
#define	 CBR_460800		460800
#define	 CBR_921600		921600

#define UM_READ_COMM			(WM_USER +100)


////////////////////////////////////////////////////////////////////////////////////////////
#define UM_DATA_DISPLAY				WM_APP + 101
#define MAX_BLOCK					4096
#define BUFFER_LEN					4096
#define TM_TEST						WM_USER + 1000
#define TM_PERIOD_SEND_POS_INFO		WM_USER + 1001

#define UM_TCP_RECEIVE			(WM_USER + 1003) //#define  UM_TCP_RECEIVE			(WM_USER + 109)
#define	UM_SOCKET_ACCEPT		(WM_USER + 1004) //#define	UM_SOCKET_ACCEPT		(WM_USER + 201)
#define	UM_SOCKET_CLOSE			(WM_USER + 1005) //#define	UM_SOCKET_CLOSE			(WM_USER + 202)
//메세지 관련 정의 필요



extern CString GetTime(CTime time);
extern void EnumerateSerialPorts(CUIntArray& ports);
extern CString GetTimeToString();
extern CString GetExecutedPath();
extern CString g_strInIPath;
extern int Find_STX(BYTE* p, int size);
extern unsigned int crc32a(unsigned char *message ,int nLen) ;


enum { eCONNECT_SERIAL = 0, eCONNECT_TCP };
typedef struct _FFH
{
	byte		uPremable[2];
	USHORT uLen ;
	char   cCmd ;

}stFFH_header;

typedef struct _FFH_PERIOD
{
	byte		uPremable[2];
	USHORT uLen;
	char   cCmd;
	double period;
	int crc;
}stFFD_header_period;

//데이터 받아와서 저장하는 구조체
//Preamble (2), PLen(2), CmdResp (1), t(8), x(8), y(8), z(8), θm(8), φm(8), θf(8), φf(8), v(8), RPSθ(8), RPSφ(8), CRC(4)
typedef struct _RPKT
{
	byte		uPremable[2];
	USHORT		Len ;
	char		cmdResp;
	double		t;
	double		x;
	double		y;
	double		z;
	double		theta_m;
	double		pi_m;
	double		theta_f;
	double		pi_f;
	double		v;
	double		rps_theta;
	double		rps_pi;
	int			crc ;

	/*_RPKT()
	{
		Preamble = 0xAA55 ;
	}*/
}PKT_NEXT_INFO;


typedef struct _PKT_RESP
{
	byte		uPremable[2];
	USHORT		uLen ;
	char		cCmd ;
	int		    crc ;
}PKT_RESP;

//남은 위치 데이터 갯수 받아오는 구조체
typedef struct _PKT_SIZE
{
	byte		uPremable[2];
	USHORT		uLen;
	char		cCmd;
	int		    size;
	int			crc;
}PKT_SIZE;

//FFGD 장치정보 요청
//Preamble (2), PLen(2), CmdResp (1), name(32), version(32), serial(4), description(128), CRC(4)
typedef struct _PKT_REQUEST
{
	byte		uPremable[2];
	USHORT		uLen ;
	char		cCmd ;
	char        szName[32];
	char		szVersion[32];
	char		szSerial[4];
	char		szDiscription[128];
	int		    crc ;
}PKT_REQUEST_DEV_INFO;

//파라미터 관련 데이터저장하는 구조체
typedef struct _PKT_PARAM
{
	byte		uPremable[2];
	USHORT		uLen ;
	char		cCmd ;
	char        szParam1 [8];
	char        szParam2 [8];
	char        szParam3 [8];
	char        szParam4 [8];
	char        szParam5 [8];
	char        szParam6 [8];
	char        szParam7 [8];
	char        szParam8 [8];
	char        szParam9 [8];
	char        szParam10[8];
	char        szParam11[8];
	char        szParam12[8];
	int		    crc ;  
}PKT_PARAM;

