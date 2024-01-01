
// NEW2Dlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "NEW2.h"
#include "NEW2Dlg.h"
#include "afxdialogex.h"
#include "CRCchecker.h"
#include <math.h>
#include <vector>
#include <sstream>
#include"SocketManager.h"

using namespace std;


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
	//	ON_WM_TIMER()
END_MESSAGE_MAP()


// CNEW2Dlg 대화 상자



CNEW2Dlg::CNEW2Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_NEW2_DIALOG, pParent)
	, m_strPortName(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	memset(m_szBuffer, 0, sizeof(BYTE)*BUFFER_LEN);
	g_strInIPath.Format("%s\\NEW2.ini", GetExecutedPath());
	m_bScroll = TRUE;

	// 그래프를 그릴 데이터가 들어갈 double형 포인터 메모리 할당
	draw_data0 = new double[SIZE];
	draw_data1 = new double[SIZE];
	draw_data2 = new double[SIZE];
	//x축 값이 들어갈 x_data 포인터 메모리 할당
	x_data = new double[SIZE];

	//초기화
	memset(draw_data0, 0, SIZE * sizeof(double));
	memset(draw_data1, 0, SIZE * sizeof(double));
	memset(draw_data2, 0, SIZE * sizeof(double));
	memset(x_data, 0, SIZE * sizeof(double));

	graph_buffer_index = 0;
	graph_buffer_currentindex = 0;

	data_count = 0;

	graph_type = eXYZ_GRAPH;

	save_data = NULL;
	save_count = 0;

	connection_type = eCONNECT_SERIAL;

	//TCP통신을 위해서 사용
	m_pSocketManager = new CSocketManager;
}

void CNEW2Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_BAUDRATE, m_comboBaudrate);
	DDX_Control(pDX, IDC_LIST_PORT, m_LIst);
	DDX_Text(pDX, IDC_STATIC_PORT_STATE, m_strPortName);
	DDX_Control(pDX, IDC_LIST_LOG, m_listLog);
	DDX_Control(pDX, IDC_EDIT_POS_Y, m_EditposY);
	DDX_Control(pDX, IDC_EDIT_POS_Z, m_EditposZ);
	DDX_Control(pDX, IDC_EDIT_POS_X, m_EditposX);
	DDX_Control(pDX, IDC_EDIT_DIR_THETA_M, m_EditposThetaM);
	DDX_Control(pDX, IDC_EDIT_DIR_THETA_F, m_EditposThetaF);
	DDX_Control(pDX, IDC_EDIT_DIR_PI_M, m_EditPiM);
	DDX_Control(pDX, IDC_EDIT_DIR_PI_F, m_EditPiF);
	DDX_Control(pDX, IDC_EDIT_SPEED_V, m_EditV);
	DDX_Control(pDX, IDC_EDIT_SPEED_RPS_PI, m_EditRpsPi);
	DDX_Control(pDX, IDC_EDIT_SPEED_RPS_THETA, m_EditRpsTheta);
	DDX_Control(pDX, IDC_EDIT_ANSWER_CODE, m_EditAnswer);
	DDX_Control(pDX, IDC_EDIT_COUNT, m_CountSize);
	DDX_Control(pDX, IDC_EDIT_TIMER, m_Time);
	DDX_Control(pDX, IDC_EDIT_PERIOD, m_Period);
	DDX_Control(pDX, IDC_EDIT_NAME, m_Editname);
	DDX_Control(pDX, IDC_EDIT_DISCRIPTION, m_EditDiscription);
	DDX_Control(pDX, IDC_EDIT_SERIAL, m_EditSerial);
	DDX_Control(pDX, IDC_EDIT_VERSION, m_EditVersion);
	DDX_Control(pDX, IDC_PARAM1, m_EditParam1);
	DDX_Control(pDX, IDC_PARAM10, m_EditParam10);
	DDX_Control(pDX, IDC_PARAM11, m_EditParam11);
	DDX_Control(pDX, IDC_PARAM12, m_EditParam12);
	DDX_Control(pDX, IDC_PARAM2, m_EditParam2);
	DDX_Control(pDX, IDC_PARAM3, m_EditParam3);
	DDX_Control(pDX, IDC_PARAM4, m_EditParam4);
	DDX_Control(pDX, IDC_PARAM5, m_EditParam5);
	DDX_Control(pDX, IDC_PARAM6, m_EditParam6);
	DDX_Control(pDX, IDC_PARAM7, m_EditParam7);
	DDX_Control(pDX, IDC_PARAM8, m_EditParam8);
	DDX_Control(pDX, IDC_PARAM9, m_EditParam9);
	DDX_Control(pDX, IDC_ChartViewer, m_ChartViewer);
	DDX_Control(pDX, IDC_EDIT_COMMAND_CODE, m_Editcommand);
	DDX_Control(pDX, IDC_EDIT_COUNT2, m_Receivecount);
	DDX_Control(pDX, IDC_RADIO_XYZ, m_CheckXYZ);
	DDX_Control(pDX, IDC_EDIT_SAVE_COUNT, m_Savecount);
	DDX_Control(pDX, IDC_EDIT_CLIENT_PORT, m_ClientPort);
	DDX_Control(pDX, IDC_RADIO_SERIAL, m_RadioSerial);
}

BEGIN_MESSAGE_MAP(CNEW2Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_OPEN_PORT, &CNEW2Dlg::OnBnClickedBtnOpenPort)
	ON_BN_CLICKED(IDC_BTN_CLOSE_PORT, &CNEW2Dlg::OnBnClickedBtnClosePort)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_MESSAGE(UM_READ_COMM, &CNEW2Dlg::OnReadComm)
	ON_BN_CLICKED(IDC_BT_LOCATION, &CNEW2Dlg::OnBnClickedBtLocation)
	ON_BN_CLICKED(IDC_BT_DATA, &CNEW2Dlg::OnBnClickedBtData)
	ON_BN_CLICKED(IDC_BT_REMOVE, &CNEW2Dlg::OnBnClickedBtRemove)
	ON_BN_CLICKED(IDC_BT_ALARM_SETTING, &CNEW2Dlg::OnBnClickedBtAlarmSetting)
	ON_BN_CLICKED(IDC_BT_ALARM_CANCEL, &CNEW2Dlg::OnBnClickedBtAlarmCancel)
	ON_BN_CLICKED(IDC_BT_INFORM, &CNEW2Dlg::OnBnClickedBtInform)
	ON_BN_CLICKED(IDC_BT_INFORM_SETTING, &CNEW2Dlg::OnBnClickedBtInformSetting)
	ON_BN_CLICKED(IDC_BT_PARAM_NOVO, &CNEW2Dlg::OnBnClickedBtParamNovo)
	ON_BN_CLICKED(IDC_BT_PARAM_VO1, &CNEW2Dlg::OnBnClickedBtParamVo1)
	ON_WM_HSCROLL()

	ON_BN_CLICKED(IDC_RADIO_XYZ, &CNEW2Dlg::OnBnClickedRadioXyz)
	ON_BN_CLICKED(IDC_RADIO_THETA_PI_M, &CNEW2Dlg::OnBnClickedRadioThetaPiM)
	ON_BN_CLICKED(IDC_RADIO_THETA_PI_F, &CNEW2Dlg::OnBnClickedRadioThetaPiF)
	ON_BN_CLICKED(IDC_RADIO_V_RPS_THETA_PI, &CNEW2Dlg::OnBnClickedRadioVRpsThetaPi)
	ON_BN_CLICKED(IDC_BUTTON_SAVESTART, &CNEW2Dlg::OnBnClickedButtonSavestart)
	ON_BN_CLICKED(IDC_BUTTON_SAVEFINISH, &CNEW2Dlg::OnBnClickedButtonSavefinish)
	ON_BN_CLICKED(IDC_RADIO_SERIAL, &CNEW2Dlg::OnBnClickedRadioSerial)
	ON_BN_CLICKED(IDC_RADIO_TCPIP, &CNEW2Dlg::OnBnClickedRadioTcpip)

	ON_MESSAGE(UM_TCP_RECEIVE, &CNEW2Dlg::OnSocketReceive)
	ON_MESSAGE(UM_SOCKET_CLOSE, &CNEW2Dlg::OnCloseSocket)
	ON_MESSAGE(UM_SOCKET_ACCEPT, &CNEW2Dlg::OnAccept)

END_MESSAGE_MAP()


// CNEW2Dlg 메시지 처리기

BOOL CNEW2Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.


	m_Com.SetSendHandle(this->m_hWnd);
	InitPortValue(); // 기본 시리얼 통신 설정 
	InitPortList(); // 사용가능한 com포트 띄어주는것
	InitializeComState(); //통신속도 초기화

	Reposition(); //컨트롤 크기나 위치 조정

	m_CheckXYZ.SetCheck(true); //그래프 디폴트 값 설정
	m_RadioSerial.SetCheck(true); // 기본 통신 디폴트
	OnBnClickedRadioSerial(); // 기본 통신 디폴트

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CNEW2Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CNEW2Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CNEW2Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//port 기본 설정 비트사이즈 : 9, 패리티비트 0, 스탑비트0
void CNEW2Dlg::InitPortValue()
{
	m_tty.byByteSize = 8;
	m_tty.byFlowCtrl = 0;
	m_tty.byParity = 0;
	m_tty.byStopBits = 0;

	m_bSaveFile = FALSE;

	memset(m_szBuffer, 0, sizeof(BYTE)*BUFFER_LEN);
	m_nBufferLen = 0;
}

void CNEW2Dlg::Reposition()
{
	CRect rect;
	GetClientRect(&rect);
}

LRESULT CNEW2Dlg::OnCloseSocket(WPARAM wp, LPARAM lp) //wp this로 자기자신을 보냄 소켓식별자를 보내줌
{
	SOCKET  s = (SOCKET)wp;
	m_pSocketManager->Close(s);

	m_listLog.AddString("client가 연결을 종료하였습니다.");
	return 1;
}

LRESULT CNEW2Dlg::OnAccept(WPARAM wp, LPARAM lp) //CLIENT가 접속했을때 호출 
{
	SOCKET  serviceSocket = (SOCKET)wp;
	m_pSocketManager->Accept(serviceSocket);
	m_listLog.AddString("client가 접속하였습니다.");
	return 1;
}

//사용자 정의 함수 핸들러
//cmysocket에서 호출
LRESULT CNEW2Dlg::OnSocketReceive(WPARAM wp, LPARAM lp)
{
	//시리얼에서는 onreadcomm이랑 똑같음
	OnReadComm(wp, lp);
	return 1;
}

//데이터 받아와서 명령별로 처리해주는 함수
//속도가 너무빠르면 한번에 1개의 패킷이 아니라 2개 이상의 패킷이 들어올 수 있기때문에 한개 패킷의 길이만큼 처리해주고 
//그 뒤로온 패킷을 처리해줌
// 1개패킷크기 만큼  m_szBuffer에서  왼쪽으로 쉬프트
LRESULT CNEW2Dlg::OnReadComm(WPARAM wParam, LPARAM lParam)
{
	char* pData = (char*)wParam; //처음 data를 pdata에 받아옴
	int  nLen = (int)lParam;
	CString strHex, strLine, strLog;


	if (m_nBufferLen + nLen >= BUFFER_LEN)
	{
		strLog.Format("Buffer OverFlow Len = %d\n", m_nBufferLen + nLen);
		DisplayLog(strLog);
		m_nBufferLen = 0;
		memset(m_szBuffer, 0, sizeof(BYTE)*BUFFER_LEN);
		return 0;
	}

	memcpy(m_szBuffer + m_nBufferLen, pData, nLen);//받아온 데이터를 m_szBuffer에 저장

	m_nBufferLen += nLen; // m_nbufferlen 수신된 패킷 길이 

REPEAT:
	if (m_nBufferLen > 0)
	{
		TRACE("------------------------------m_nBufferLen : %d\n", m_nBufferLen);
	}
	int head_pos = Find_STX(m_szBuffer, m_nBufferLen); //preamble이 잘못왔을경우 find_stx에서 체크

	//오류메세지 출력
	if (head_pos == -1)
	{
		strLog.Format("not Find Preamble");
		DisplayLog(strLog);
		m_nBufferLen = 0;
		memset(m_szBuffer, 0, sizeof(BYTE)*BUFFER_LEN);
		return 0;
	}

	//프리엠블이 중간에 있을경우 잘라서 맨앞으로 가져옴
	if (head_pos > 0)
	{
		memcpy(m_szBuffer, m_szBuffer + head_pos, m_nBufferLen - head_pos);
	}

	int nFFHSize = sizeof(stFFH_header);

	if (m_nBufferLen < nFFHSize) //첫번째 데이터가 헤더크기보다 작으면 헤더를 해석 할 수 없으니까 다음패킷을 기다림
		return 0;


	char szBuffer[128] = { 0 };
	stFFH_header  ffh;
	memcpy(&ffh, m_szBuffer, nFFHSize);//받아온 명령어를 ffh구조체에 저장

	int nPayloadLen = ffh.uLen; //받아온 길이 nPayloadLen
	int nCommand = (int)ffh.cCmd;   // 수신된 명령어 추출
	int crc_index = nFFHSize + nPayloadLen;//crc시작위치

	int nPakcetSize = nPayloadLen + 4 + nFFHSize;
	
	if (nPakcetSize > m_nBufferLen)
	{
		strLog.Format("데이터가 다 안들어옴 %d", m_nBufferLen);
		DisplayLog(strLog);
		return 0;
	}


	int test_crc = crc32a(m_szBuffer + 2, nPakcetSize - 6);

	int crc_value = 0;
	memcpy(&crc_value, m_szBuffer + crc_index, 4);

	// CRC체크하는 부분 
	// test_crc는 내가 crc를 계산하는 부분
	// crc_Value는 crc추출하는 부분
	if (test_crc == crc_value)
	{
		strLog.Format("----good----");
		DisplayLog(strLog);
	}
	else
	{
		strLog.Format("----CRC ERROR----");
		DisplayLog(strLog);
		memset(m_szBuffer, 0, sizeof(BYTE)*BUFFER_LEN);
		m_nBufferLen = 0;
		return 0;
	}

	switch (nCommand) //응답 명령에 따라 swtich case 실행
	{
	case 0x41:    // 다음 데이터의 시각과 위치정보 요청
	{
		PKT_NEXT_INFO  pkt; //데이터를 받아서 저장할 구조체
		int size = sizeof(PKT_NEXT_INFO);

		memcpy(&pkt, &m_szBuffer, size); //pkt에 받아온 데이터 저장
		tprev = pkt.t; //받아온 시간을 전역변수 check_time에 저장-> 아래 위치정보 가져오기에서 사용


		//구조체에 저장된 데이터를 눈으로 확인할 수 있게 띄어줌
		CString ans;
		ans.Format("%#x", pkt.cmdResp);					m_EditAnswer.SetWindowTextA(ans);
		ans.Format("%lf", pkt.t);						m_Time.SetWindowTextA(ans);
		ans.Format("%lf", pkt.x);						m_EditposX.SetWindowTextA(ans);
		ans.Format("%lf", pkt.y);						m_EditposY.SetWindowTextA(ans);
		ans.Format("%lf", pkt.z);						m_EditposZ.SetWindowTextA(ans);
		ans.Format("%lf", pkt.theta_m);					m_EditposThetaM.SetWindowTextA(ans);
		ans.Format("%lf", pkt.pi_m);					m_EditPiM.SetWindowTextA(ans);
		ans.Format("%lf", pkt.theta_f);					m_EditposThetaF.SetWindowTextA(ans);
		ans.Format("%lf", pkt.pi_f);					m_EditPiF.SetWindowTextA(ans);
		ans.Format("%lf", pkt.v);						m_EditV.SetWindowTextA(ans);
		ans.Format("%lf", pkt.rps_theta);				m_EditRpsTheta.SetWindowTextA(ans);
		ans.Format("%lf", pkt.rps_pi);					m_EditRpsPi.SetWindowTextA(ans);

		strLog.Format("위치 데이터 수신완료");
		DisplayLog(strLog);

	}
	break;

	case 0x43:    // 남은 위치 데이터 수 가져오기
	{
		PKT_SIZE PS;

		int size = sizeof(PKT_SIZE);

		memset(&PS, 0, size);
		memcpy(&PS, &m_szBuffer, size); //PS 받아온 데이터 저장

		//눈으로 확인할 수 있도록 띄어줌
		CString ans;
		ans.Format("%#x", PS.cCmd);					m_EditAnswer.SetWindowTextA(ans);
		ans.Format("%d", PS.size);					m_CountSize.SetWindowTextA(ans);

		strLog.Format("남은 위치 데이터 수신완료");
		DisplayLog(strLog);

	}
	break;
	case 0x45:    //기록된 데이터 모두 지우기
	{
		PKT_RESP PR;

		int size = sizeof(PKT_RESP);

		memset(&PR, 0, size);
		memcpy(&PR, &m_szBuffer, size);//PS 받아온 데이터 저장

		//받아오는 데이터가 따로 없기때문에 명령정보만 띄어줌
		CString ans;
		ans.Format("%#x", PR.cCmd);					m_EditAnswer.SetWindowTextA(ans);

		strLog.Format("데이터 삭제 완료");
		DisplayLog(strLog);

	}
	break;
	case 0x47:    // 주기적 알림 정보 설정하기
	{
		PKT_RESP PR;

		int size = sizeof(PKT_RESP);

		memset(&PR, 0, size);
		memcpy(&PR, &m_szBuffer, size);

		CString ans;
		ans.Format("%#x", PR.cCmd);					m_EditAnswer.SetWindowTextA(ans);

		m_Period.GetWindowTextA(ans);
		int period = atoi(ans);
		strLog.Format("%d ms주기 설정 완료", period);
		DisplayLog(strLog);

	}
	break;
	case 0x49:    // 주기적 위치정보 알림 취소하기
	{
		PKT_RESP PR;

		int size = sizeof(PKT_RESP);

		memset(&PR, 0, size);
		memcpy(&PR, &m_szBuffer, size);

		CString ans;
		ans.Format("%#x", PR.cCmd);					m_EditAnswer.SetWindowTextA(ans);

		strLog.Format("주기 취소 완료");
		DisplayLog(strLog);

	}
	break;
	case 0x4B:    // 주기적 위치정보 알림 명령에 의한 알림 정보
	{
		int size = sizeof(PKT_NEXT_INFO);

		PKT_NEXT_INFO pkt;
		memcpy(&pkt, &m_szBuffer, size); //pkt에 받아온 데이터 저장

		CString ans;
		ans.Format("%#x", pkt.cmdResp);					m_EditAnswer.SetWindowTextA(ans);
		ans.Format("%lf", pkt.t);						m_Time.SetWindowTextA(ans);
		ans.Format("%lf", pkt.x);						m_EditposX.SetWindowTextA(ans);
		ans.Format("%lf", pkt.y);						m_EditposY.SetWindowTextA(ans);
		ans.Format("%lf", pkt.z);						m_EditposZ.SetWindowTextA(ans);
		ans.Format("%lf", pkt.theta_m);					m_EditposThetaM.SetWindowTextA(ans);
		ans.Format("%lf", pkt.pi_m);					m_EditPiM.SetWindowTextA(ans);
		ans.Format("%lf", pkt.theta_f);					m_EditposThetaF.SetWindowTextA(ans);
		ans.Format("%lf", pkt.pi_f);					m_EditPiF.SetWindowTextA(ans);
		ans.Format("%lf", pkt.v);						m_EditV.SetWindowTextA(ans);
		ans.Format("%lf", pkt.rps_theta);				m_EditRpsTheta.SetWindowTextA(ans);
		ans.Format("%lf", pkt.rps_pi);					m_EditRpsPi.SetWindowTextA(ans);

		data_count++; //수신데이터 개수를 표시하기 위해 한개씩 증가
		ans.Format("%d", data_count);					m_Receivecount.SetWindowTextA(ans);

		x_data[graph_buffer_index] = graph_buffer_currentindex; //그래프 그릴때 x축에 값을 자동으로 넣어줌

		switch (graph_type) //내가 클릭한 라디오 버튼에 따라 switch case문 작동
		{
		case eXYZ_GRAPH:
			draw_data0[graph_buffer_index] = pkt.x;
			draw_data1[graph_buffer_index] = pkt.y;
			draw_data2[graph_buffer_index] = pkt.z;
			break;
		case eTHETA_PI_M:
			draw_data0[graph_buffer_index] = pkt.theta_m;
			draw_data1[graph_buffer_index] = pkt.pi_m;
			break;
		case eTHETA_PI_F:
			draw_data0[graph_buffer_index] = pkt.theta_f;
			draw_data1[graph_buffer_index] = pkt.pi_f;
			break;
		case eV_RPS_THETA_PI:
			draw_data0[graph_buffer_index] = pkt.v;
			draw_data1[graph_buffer_index] = pkt.rps_theta;
			draw_data2[graph_buffer_index] = pkt.rps_pi;
			break;
		}
		draw_data(); //그래프 그리기 함수 호출
		graph_buffer_index++; 
		graph_buffer_currentindex++;

		if (graph_buffer_index == SIZE) //내가 지정한 사이즈에 따라 맨 처음 들어온 데이터를 지우면서 새로운 데이터를 받아서 그림
		{
			graph_buffer_index--;
			memcpy(&draw_data0[0], &draw_data0[1], sizeof(double)*(SIZE - 1)); //첫번째 자리거를 지우면서 왼쪽으로 한칸씩 이동 마지막 한자리가 생김
			memcpy(&draw_data1[0], &draw_data1[1], sizeof(double)*(SIZE - 1)); //첫번째 자리거를 지우면서 왼쪽으로 한칸씩 이동 마지막 한자리가 생김
			memcpy(&draw_data2[0], &draw_data2[1], sizeof(double)*(SIZE - 1)); //첫번째 자리거를 지우면서 왼쪽으로 한칸씩 이동 마지막 한자리가 생김
			memcpy(&x_data[0], &x_data[1], sizeof(double)*(SIZE - 1)); //첫번째 자리거를 지우면서 왼쪽으로 한칸씩 이동 마지막 한자리가 생김
		}
		m_csFile.Lock();
		WriteLogFile(&pkt);
		m_csFile.Unlock();// 스레드 동기화 맞춰주는 부분
	}
	break;
	case 0x4D:    // FFGD 장치 정보를 가져오기
	{
		PKT_REQUEST_DEV_INFO recv_dev;

		int size = sizeof(PKT_REQUEST_DEV_INFO);

		memset(&recv_dev, 0, size);
		memcpy(&recv_dev, &m_szBuffer, size);

		CString ans;
		ans.Format("%#x", recv_dev.cCmd);

		m_EditAnswer.SetWindowTextA(ans);
		m_Editname.SetWindowTextA((char*)recv_dev.szName);
		m_EditVersion.SetWindowTextA((char*)recv_dev.szVersion);
		m_EditSerial.SetWindowTextA((char*)recv_dev.szSerial);
		m_EditDiscription.SetWindowTextA((char*)recv_dev.szDiscription);

		strLog.Format("장치정보 수신완료");
		DisplayLog(strLog);
	}
	break;
	case 0x4F:    // FFGD 장치 정보를 설정한다.
	{
		PKT_RESP PR;

		int size = sizeof(PKT_RESP);

		memset(&PR, 0, size);
		memcpy(&PR, &m_szBuffer, size);

		CString ans;
		ans.Format("%#x", PR.cCmd);					m_EditAnswer.SetWindowTextA(ans);

		strLog.Format("장치 정보 설정완료");
		DisplayLog(strLog);
	}
	break;
	case 0x51:    // 각종 파라메터 정보 설정하기(비휘발성)
	{
		PKT_RESP PR;

		int size = sizeof(PKT_RESP);

		memset(&PR, 0, size);
		memcpy(&PR, &m_szBuffer, size);

		CString ans;
		ans.Format("%#x", PR.cCmd);					m_EditAnswer.SetWindowTextA(ans);

		strLog.Format("비휘발성 파라메터 설정 완료");
		DisplayLog(strLog);

	}
	break;
	case 0x53:    // 각종 파라메터 정보 설정하기(휘발성)
	{
		PKT_RESP PR;

		int size = sizeof(PKT_RESP);

		memset(&PR, 0, size);
		memcpy(&PR, &m_szBuffer, size);

		CString ans;
		ans.Format("%#x", PR.cCmd);					m_EditAnswer.SetWindowTextA(ans);

		strLog.Format("휘발성 파라메터 설정 완료");
		DisplayLog(strLog);

	}
	break;
	}//switch

	memcpy(m_szBuffer, m_szBuffer + nPakcetSize, m_nBufferLen - nPakcetSize);
	m_nBufferLen -= nPakcetSize;

	if (m_nBufferLen > 0)
	{
		TRACE("------------------------------m_nBufferLen : %d\n", m_nBufferLen);

		goto REPEAT;
	}
	//1개 이상의 패킷이 들어왔을떄 1개의 패킷을 처리하고 다음 패킷을 처리하기전에 데이터 수신되는것을 방지하기 위해 
	//goto repeate문 사용
	memset(m_szBuffer, 0, sizeof(BYTE)*BUFFER_LEN);
	m_nBufferLen = 0;
	return 0;
}
//파일 쓰기 년_월_일_시_분_초.csv 형식으로 생성
void CNEW2Dlg::CreateLogFile()
{
	CString strDate, strDate2, strPath;
	CString strlogF;

	SYSTEMTIME st;
	GetLocalTime(&st);

	strPath.Format(PATH);//그래프 그리기 프로젝트 release폴더에 data로 들어가서 저장됨
	strPath = strPath + "\\" + "data";
	CreateDirectory(strPath, NULL);

	strDate2.Format("\\%02d_%02d_%02d_%02d_%02d.csv", st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
	strDate = strPath + strDate2;

	errno_t err;

	err = fopen_s(&save_data, strDate, "w");
	if (err)
	{
		strlogF.Format("fail");
		DisplayLog(strlogF);
	}
}

//수신된 데이터를 전부타 csv파일에 저장
//프로그램 실행중에 csv파일 열수 없음
void CNEW2Dlg::WriteLogFile(PKT_NEXT_INFO *pkt)
{
	if (save_data == NULL)
	{
		return;
	}

	fprintf(save_data, "%s,%lf, %lf, %lf, %lf, %lf, %lf,%lf,%lf,%lf,%lf,%lf\n", GetTimeToString(),
		pkt->t, pkt->x, pkt->y, pkt->z, pkt->theta_m, pkt->pi_m, pkt->theta_f, pkt->pi_f,
		pkt->v, pkt->rps_theta, pkt->rps_pi);

	fflush(save_data);
	save_count++;

	CString ans;
	ans.Format("%d", save_count);
	m_Savecount.SetWindowTextA(ans);
	// fprintf 했다고 바로 쓰는게 아님, 데이터 수신도중 파일을 열었을때 데이터가 적을수 있음, fflush를 쓰면 강제로 하드디스크에 저장됨
}

//기본 사용가능한 포트 보여줌
void CNEW2Dlg::InitPortList(void)
{
	m_LIst.ResetContent();
	m_arPortList.RemoveAll();

	EnumerateSerialPorts(m_arPortList);

	int nPort = 0, nPortSel = 0;
	int nSize = m_arPortList.GetSize();
	CString strText;

	for (int i = 0; i < nSize; i++)
	{
		nPort = m_arPortList.GetAt(i);
		strText.Format("COM% 2d", nPort);
		m_LIst.AddString(strText);

		if (nPort == m_nSelPortNum)
			nPortSel = i;
	}

	m_LIst.SetCurSel(nPortSel);
}
 
void CNEW2Dlg::ProfileRead()
{
	CWinApp* pApp = (CWinApp*)AfxGetApp();

	CString strSection, strKey;
	strSection.Format("PORT_PROPERTY");

	strKey.Format("COM_PORT_NUM");
	m_tty.byCommPort = GetPrivateProfileInt(strSection, strKey, 1, g_strInIPath);

	strKey.Format("COM_PORT_BORATE");
	m_tty.dwBaudRate = GetPrivateProfileInt(strSection, strKey, CBR_115200, g_strInIPath);
}

void CNEW2Dlg::ProfileWrite()
{
	CWinApp* pApp = (CWinApp*)AfxGetApp();

	CString strSection, strKey, strValue;
	strSection.Format("PORT_PROPERTY");


	strKey.Format("COM_PORT_NUM");
	strValue.Format("%d", (int)m_tty.byCommPort);
	WritePrivateProfileString(strSection, strKey, strValue.GetBuffer(strValue.GetLength()), g_strInIPath);

	strKey.Format("COM_PORT_BORATE");
	strValue.Format("%d", (int)m_tty.dwBaudRate);
	WritePrivateProfileString(strSection, strKey, strValue.GetBuffer(strValue.GetLength()), g_strInIPath);
}


BOOL CNEW2Dlg::SetComState()
{
	UpdateData(true);

	int iCurSel = m_comboBaudrate.GetCurSel();

	switch (iCurSel) // 전송  속도 
	{
	case 0:
		m_dwBaudrate = CBR_110;
		break;
	case 1:
		m_dwBaudrate = CBR_300;
		break;
	case 2:
		m_dwBaudrate = CBR_600;
		break;
	case 3:
		m_dwBaudrate = CBR_1200;
		break;
	case 4:
		m_dwBaudrate = CBR_2400;
		break;
	case 5:
		m_dwBaudrate = CBR_4800;
		break;
	case 6:
		m_dwBaudrate = CBR_9600;
		break;
	case 7:
		m_dwBaudrate = CBR_14400;
		break;
	case 8:
		m_dwBaudrate = CBR_19200;
		break;
	case 9:
		m_dwBaudrate = CBR_38400;
		break;
	case 10:
		m_dwBaudrate = CBR_57600;
		break;
	case 11:
		m_dwBaudrate = CBR_115200;
		break;
	case 12:
		m_dwBaudrate = CBR_128000;
		break;
	case 13:
		m_dwBaudrate = CBR_256000;
		break;
	case 14:
		m_dwBaudrate = CBR_230400;
		break;
	case 15:
		m_dwBaudrate = CBR_460800;
		break;
	case 16:
		m_dwBaudrate = CBR_921600;
		break;
	default:
		m_dwBaudrate = CBR_9600;
		break;
	}

	return (1);
}

BOOL CNEW2Dlg::InitializeComState()
{
	int iCurSel;

	switch (m_dwBaudrate) // 전송  속도 
	{
	case CBR_110:
		iCurSel = 0;
		break;
	case CBR_300:
		iCurSel = 1;
		break;
	case CBR_600:
		iCurSel = 2;
		break;
	case CBR_1200:
		iCurSel = 3;
		break;
	case CBR_2400:
		iCurSel = 4;
		break;
	case CBR_4800:
		iCurSel = 5;
		break;
	case CBR_9600:
		iCurSel = 6;
		break;
	case CBR_14400:
		iCurSel = 7;
		break;
	case CBR_19200:
		iCurSel = 8;
		break;
	case CBR_38400:
		iCurSel = 9;
		break;
	case CBR_57600:
		iCurSel = 10;
		break;
	case CBR_115200:
		iCurSel = 11;
		break;
	case CBR_128000:
		iCurSel = 12;
		break;
	case CBR_256000:
		iCurSel = 13;
		break;
	case CBR_230400:
		iCurSel = 14;
		break;
	case CBR_460800:
		iCurSel = 15;
		break;
	case CBR_921600:
		iCurSel = 16;
		break;
	default:
		iCurSel = 6;
		break;
	}
	m_comboBaudrate.SetCurSel(iCurSel);
	return TRUE;
}

void CNEW2Dlg::OnBnClickedBtnOpenPort()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int test;
	if (connection_type == eCONNECT_SERIAL) //시리얼 통신일 경우
	{
		if (m_pSocketManager == NULL)  
		{
			return;
		}
		if (m_pSocketManager->m_pAcceptSocket != NULL) //기존 tcp서버가 열려있다면
		{
			m_pSocketManager->Close();
			m_listLog.AddString("TCP서버 종료");
		}//TCP연결 종료
		SerialPort_Open();
	}
	else
	{
		if (m_Com.m_flgConnected) //시리얼 연결됬는지 알려줌-> 연결되었으면 끊어줌
		{
			m_bSaveFile = FALSE;
			m_Com.CloseConnection();
			m_listLog.AddString("SERIAL 포트종료");
		}

		CString port;
		m_ClientPort.GetWindowTextA(port);
		test = atoi(port);
		if (test > 1000 && test < 65000)// 포트번호가 1000번에서 65000번 사이일 경우에만 연결
		{
			m_listLog.AddString("TCP서버 대기중");
			m_pSocketManager->TCPServerStart(this, test);
		}
		else
		{
			AfxMessageBox("포트번호에 정수를 입력해주세요"); //잘못된 숫자가나 문자가 입력된경우 연결 메세지를 띄어줌
		}
	}
	ProfileWrite();
	UpdateData(FALSE);
}

void CNEW2Dlg::SerialPort_Open()
{
	int nSel = m_LIst.GetCurSel();

	m_nSelPortNum = m_arPortList.GetAt(nSel);
	SetComState();

	if (m_Com.m_flgConnected) //시리얼 연결됬는지 알려줌
	{
		m_bSaveFile = FALSE;
		m_Com.CloseConnection();
	}

	m_tty.byCommPort = m_nSelPortNum;
	m_tty.dwBaudRate = m_dwBaudrate;

	BOOL ret = m_Com.ConnectionPort(&m_tty);

	if (ret)
	{
		CString strText;
		strText.Format("Com %d 연결됨", m_nSelPortNum);
		SetWindowText(strText);
		m_strPortName = strText;
		m_bSaveFile = TRUE;
	}
	else
	{
		CString strText;
		strText.Format("연결되지 않았음", m_nSelPortNum);
		SetWindowText(strText);
		m_strPortName = strText;
		m_bSaveFile = FALSE;
	}
}

void CNEW2Dlg::SerialPort_Close()
{
	if (m_Com.m_flgConnected)
	{
		m_Com.CloseConnection();

		CString strText;
		strText.Format("연결되지 않았음", m_nSelPortNum);
		m_strPortName = strText;
		SetWindowText(strText);

		InitPortValue();
	}
	else
	{
		AfxMessageBox("연결되지 않았습니다.");
	}
}



void CNEW2Dlg::OnBnClickedBtnClosePort()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//통신 조건에 맞춰서 종료만 해주면 됨
	if (connection_type == eCONNECT_SERIAL)
	{
		SerialPort_Close();
		m_listLog.AddString("SERIAL 포트종료");
	}
	else
	{
		m_pSocketManager->Close();
		m_listLog.AddString("TCP서버 종료");
	}
	UpdateData(FALSE);
}

void CNEW2Dlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

void CNEW2Dlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	Reposition();
}

void CNEW2Dlg::DisplayLog(CString strLog)
{
	m_listLog.AddString(strLog);

	if (m_bScroll)
		m_listLog.SetCurSel(m_listLog.GetCount() - 1);
}

void CNEW2Dlg::SendPacket(BYTE * pBuffer, int nLen)
{
	if (connection_type == eCONNECT_SERIAL)
	{
		m_Com.WriteCommBlock((LPSTR)pBuffer, nLen);
	}
	else
	{
		if (m_pSocketManager != NULL && m_pSocketManager->m_pClientSocket != NULL)
			m_pSocketManager->m_pClientSocket->Send((LPSTR)pBuffer, nLen);
	}
}

//위치정보 가져오기
void CNEW2Dlg::OnBnClickedBtLocation()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	BYTE message[512] = { 0 };
	stFFD_header_period send_hdr;


	send_hdr.uPremable[0] = 0xAA;
	send_hdr.uPremable[1] = 0x55;
	send_hdr.uLen = 8;
	send_hdr.cCmd = 0x40;
	send_hdr.period = tprev;

	int nLen = sizeof(send_hdr) - 2 - 4;
	send_hdr.crc = crc32a(message + 2, nLen); // 프리엠블의 길이를2더해주고 전체길이에서 프리엠블길이2 빼준거

	memcpy(message, &send_hdr, sizeof(send_hdr));

	SendPacket(message, sizeof(send_hdr));

	CString ans;
	ans.Format("%#x", send_hdr.cCmd);					m_Editcommand.SetWindowTextA(ans);

}

//남은 위치 데이터 수 가져오기
void CNEW2Dlg::OnBnClickedBtData()
{
	BYTE message[512] = { 0 };
	stFFH_header send_hdr;

	send_hdr.uPremable[0] = 0xAA;
	send_hdr.uPremable[1] = 0x55;
	send_hdr.uLen = 0;
	send_hdr.cCmd = 0x42;

	memcpy(message, &send_hdr, sizeof(send_hdr));

	int nLen = sizeof(send_hdr) - 2 - 4;
	int crc = crc32a(message + 2, nLen);
	memcpy(message + sizeof(send_hdr), &crc, sizeof(crc));

	SendPacket(message, sizeof(message));

	CString ans;
	ans.Format("%#x", send_hdr.cCmd);					m_Editcommand.SetWindowTextA(ans);

}

//기록된 데이터 모두 지우기
void CNEW2Dlg::OnBnClickedBtRemove()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	BYTE message[512] = { 0 };
	stFFH_header send_hdr;

	send_hdr.uPremable[0] = 0xAA;
	send_hdr.uPremable[1] = 0x55;
	send_hdr.uLen = 0;
	send_hdr.cCmd = 0x44;

	memcpy(message, &send_hdr, sizeof(send_hdr));

	int nLen = sizeof(send_hdr) - 2 - 4;
	int crc = crc32a(message + 2, nLen);
	memcpy(message + sizeof(send_hdr), &crc, sizeof(crc));

	SendPacket(message, sizeof(message));
	CString ans;
	ans.Format("%#x", send_hdr.cCmd);					m_Editcommand.SetWindowTextA(ans);
}

//주기적 위치정보 알림 설정하기
void CNEW2Dlg::OnBnClickedBtAlarmSetting()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	BYTE message[512] = { 0 };
	stFFD_header_period send_period;
	CString Period, strlog;

	m_Period.GetWindowTextA(Period);
	if (Period == "")
	{
		strlog.Format("주기를 입력해주세요");
		DisplayLog(strlog);
		return;
	}
	double D_Period = atof(Period);
	printf("%lf", D_Period);
	send_period.uPremable[0] = 0xAA;
	send_period.uPremable[1] = 0x55;
	send_period.uLen = 4;
	send_period.cCmd = 0x46;
	send_period.period = D_Period;

	memcpy(message, &send_period, sizeof(send_period));

	int nLen = sizeof(send_period) - 2 - 4;
	int crc = crc32a(message + 2, nLen);
	memcpy(message + sizeof(send_period), &crc, sizeof(crc));

	SendPacket(message, sizeof(message));

	CString ans;
	ans.Format("%#x", send_period.cCmd);					m_Editcommand.SetWindowTextA(ans);

}

//주기적 위치정보 알림 취소하기
void CNEW2Dlg::OnBnClickedBtAlarmCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	BYTE message[512] = { 0 };
	stFFH_header send_hdr;

	send_hdr.uPremable[0] = 0xAA;
	send_hdr.uPremable[1] = 0x55;
	send_hdr.uLen = 0;
	send_hdr.cCmd = 0x48;

	memcpy(message, &send_hdr, sizeof(send_hdr));

	int nLen = sizeof(send_hdr) - 2 - 4;
	int crc = crc32a(message + 2, nLen);
	memcpy(message + sizeof(send_hdr), &crc, sizeof(crc));

	SendPacket(message, sizeof(message));
	CString ans;
	ans.Format("%#x", send_hdr.cCmd);					m_Editcommand.SetWindowTextA(ans);
}

//FFGD 장치 정보 가져오기
void CNEW2Dlg::OnBnClickedBtInform()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	BYTE message[512] = { 0 };
	stFFH_header send_hdr;

	send_hdr.uPremable[0] = 0xAA;
	send_hdr.uPremable[1] = 0x55;
	send_hdr.uLen = 0;
	send_hdr.cCmd = 0x4C;

	memcpy(message, &send_hdr, sizeof(send_hdr));

	int nLen = sizeof(send_hdr) - 2 - 4;
	int crc = crc32a(message + 2, nLen);
	memcpy(message + sizeof(send_hdr), &crc, sizeof(crc));

	SendPacket(message, sizeof(message));

	CString ans;
	ans.Format("%#x", send_hdr.cCmd);					m_Editcommand.SetWindowTextA(ans);

}

//FFGD 장치 정보 설정하기
void CNEW2Dlg::OnBnClickedBtInformSetting()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	BYTE message[512] = { 0 };
	PKT_REQUEST_DEV_INFO send_dev;
	CString ans;

	send_dev.uPremable[0] = 0xAA;
	send_dev.uPremable[1] = 0x55;

	send_dev.uLen = 196;
	send_dev.cCmd = 0x4E;
	m_Editname.GetWindowTextA(ans);										sprintf_s(send_dev.szName, 32, "%s", ans);
	m_EditVersion.GetWindowTextA(ans);									sprintf_s(send_dev.szVersion, 32, "%s", ans);
	m_EditSerial.GetWindowTextA(ans);									sprintf_s(send_dev.szSerial, 4, "%s", ans);
	m_EditDiscription.GetWindowTextA(ans);								sprintf_s(send_dev.szDiscription, 128, "%s", ans);

	memcpy(message, &send_dev, sizeof(send_dev));

	int nLen = sizeof(send_dev) - 2 - 4;
	int crc = crc32a(message + 2, nLen);
	memcpy(message + sizeof(send_dev), &crc, sizeof(crc));

	SendPacket(message, sizeof(message));

	//입력한 장치정보는 sprintf_s를 사용하여 send_dev에 집어넣고 crc코드를 추가한 후에 데이터 패킷을 전송

	ans.Format("%#x", send_dev.cCmd);					m_Editcommand.SetWindowTextA(ans);

}

//FFGD 각종 파라메터 정보 설정하기(비휘발성) 잘 전달됨
void CNEW2Dlg::OnBnClickedBtParamNovo()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	BYTE message[512] = { 0 };
	PKT_PARAM send_param;
	CString ans;

	send_param.uPremable[0] = 0xAA;
	send_param.uPremable[1] = 0x55;
	send_param.uLen = 0;
	send_param.cCmd = 0x50;

	m_EditParam1.GetWindowTextA(ans);									sprintf_s(send_param.szParam1, 8, "%s", ans);
	m_EditParam2.GetWindowTextA(ans);									sprintf_s(send_param.szParam2, 8, "%s", ans);
	m_EditParam3.GetWindowTextA(ans);									sprintf_s(send_param.szParam3, 8, "%s", ans);
	m_EditParam4.GetWindowTextA(ans);									sprintf_s(send_param.szParam4, 8, "%s", ans);
	m_EditParam5.GetWindowTextA(ans);									sprintf_s(send_param.szParam5, 8, "%s", ans);
	m_EditParam6.GetWindowTextA(ans);									sprintf_s(send_param.szParam6, 8, "%s", ans);
	m_EditParam7.GetWindowTextA(ans);									sprintf_s(send_param.szParam7, 8, "%s", ans);
	m_EditParam8.GetWindowTextA(ans);									sprintf_s(send_param.szParam8, 8, "%s", ans);
	m_EditParam9.GetWindowTextA(ans);									sprintf_s(send_param.szParam9, 8, "%s", ans);
	m_EditParam10.GetWindowTextA(ans);									sprintf_s(send_param.szParam10, 8, "%s", ans);
	m_EditParam11.GetWindowTextA(ans);									sprintf_s(send_param.szParam11, 8, "%s", ans);
	m_EditParam12.GetWindowTextA(ans);									sprintf_s(send_param.szParam12, 8, "%s", ans);

	memcpy(message, &send_param, sizeof(send_param));

	int nLen = sizeof(send_param);
	int crc = crc32a(message + 2, nLen - 2);
	memcpy(message + sizeof(send_param), &crc, sizeof(crc));

	SendPacket(message, sizeof(message));

	ans.Format("%#x", send_param.cCmd);					m_Editcommand.SetWindowTextA(ans);
}

//각종 파라메터 정보 설정하기(휘발성)
void CNEW2Dlg::OnBnClickedBtParamVo1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	BYTE message[512] = { 0 };
	PKT_PARAM send_param;
	CString ans;

	send_param.uPremable[0] = 0xAA;
	send_param.uPremable[1] = 0x55;
	send_param.uLen = 0;
	send_param.cCmd = 0x52;

	m_EditParam1.GetWindowTextA(ans);								sprintf_s(send_param.szParam1, 8, "%s", ans);
	m_EditParam2.GetWindowTextA(ans);								sprintf_s(send_param.szParam2, 8, "%s", ans);
	m_EditParam3.GetWindowTextA(ans);								sprintf_s(send_param.szParam3, 8, "%s", ans);
	m_EditParam4.GetWindowTextA(ans);								sprintf_s(send_param.szParam4, 8, "%s", ans);
	m_EditParam5.GetWindowTextA(ans);								sprintf_s(send_param.szParam5, 8, "%s", ans);
	m_EditParam6.GetWindowTextA(ans);								sprintf_s(send_param.szParam6, 8, "%s", ans);
	m_EditParam7.GetWindowTextA(ans);								sprintf_s(send_param.szParam7, 8, "%s", ans);
	m_EditParam8.GetWindowTextA(ans);								sprintf_s(send_param.szParam8, 8, "%s", ans);
	m_EditParam9.GetWindowTextA(ans);								sprintf_s(send_param.szParam9, 8, "%s", ans);
	m_EditParam10.GetWindowTextA(ans);								sprintf_s(send_param.szParam10, 8, "%s", ans);
	m_EditParam11.GetWindowTextA(ans);								sprintf_s(send_param.szParam11, 8, "%s", ans);
	m_EditParam12.GetWindowTextA(ans);								sprintf_s(send_param.szParam12, 8, "%s", ans);

	memcpy(message, &send_param, sizeof(send_param));

	int nLen = sizeof(send_param);
	int crc = crc32a(message + 2, nLen - 2);
	memcpy(message + sizeof(send_param), &crc, sizeof(crc));

	SendPacket(message, sizeof(message));

	ans.Format("%#x", send_param.cCmd);					m_Editcommand.SetWindowTextA(ans);
}


void CNEW2Dlg::draw_data()
{
	// Create an XYChart object of size 600 x 300 pixels, with a light blue (EEEEFF) background,
	// black border, 1 pxiel 3D border effect and rounded corners
	XYChart *c = new XYChart(630, 300, 0xeeeeff, 0x000000, 1);
	c->setRoundedFrame();

	// Set the plotarea at (55, 58) and of size 520 x 195 pixels, with white background. Turn on
	// both horizontal and vertical grid lines with light grey color (0xcccccc)
	c->setPlotArea(55, 58, 520, 195, 0xffffff, -1, -1, 0xcccccc, 0xcccccc);

	// Add a legend box at (50, 30) (top of the chart) with horizontal layout. Use 9pt Arial Bold
	// font. Set the background and border color to Transparent.
	c->addLegend(50, 30, false, "arialbd.ttf", 9)->setBackground(Chart::Transparent);

	// Add a title box to the chart using 15pt Times Bold Italic font, on a light blue (CCCCFF)
	// background with glass effect. white (0xffffff) on a dark red (0x800000) background, with a 1
	// pixel 3D border.

	// Set the labels on the x axis.
	//c->xAxis()->setLabels(StringArray(labels, (int)(sizeof(labels) / sizeof(labels[0]))));

	// Display 1 out of 3 labels on the x-axis.
	c->xAxis()->setLabelStep(3);

	// Add a title to the x axis
	c->xAxis()->setTitle("Jun 12, 2006");

	// Add a line layer to the chart
	LineLayer *layer = c->addLineLayer();

	// Set the default line width to 2 pixels
	layer->setLineWidth(2);

	// Add the three data sets to the line layer. For demo purpose, we use a dash line color for the
	// last line
	//위에서 선택된 그래프 타입에 따라 각각 맞는 그래프를 그려줌
	switch (graph_type)
	{
	case eXYZ_GRAPH:
	{
		c->addTitle("XYZ_GRAPH", "timesbi.ttf", 15)->setBackground(0xccccff,
			0x000000, Chart::glassEffect());

		// Add a title to the y axis
		c->yAxis()->setTitle("MBytes per hour");
		c->yAxis2()->setTitle("Throughtput (MBytes)");

		layer->addDataSet(DoubleArray(draw_data0, graph_buffer_index), 0xff0000,
			"x");

		/*	layer->addDataSet(DoubleArray(draw_data1, graph_buffer_index), 0x00c000,
				"Y")->setUseYAxis2();
		*/	layer->addDataSet(DoubleArray(draw_data1, graph_buffer_index), 0x00c000,
			"Y");

		layer->addDataSet(DoubleArray(draw_data2, graph_buffer_index), c->dashLineColor(
			0x3333ff, Chart::DashLine), "Z");
	}
	break;
	case eTHETA_PI_M:
	{
		c->addTitle("THETA_PI_M_GRAPH", "timesbi.ttf", 15)->setBackground(0xccccff,
			0x000000, Chart::glassEffect());

		layer->addDataSet(DoubleArray(draw_data0, graph_buffer_index), 0xff0000,
			"theta_m");
		layer->addDataSet(DoubleArray(draw_data1, graph_buffer_index), c->dashLineColor(
			0x3333ff, Chart::DashLine), "pi_m");
	}
	break;
	case eTHETA_PI_F:
	{
		c->addTitle("THETA_PI_F_GRAPH", "timesbi.ttf", 15)->setBackground(0xccccff,
			0x000000, Chart::glassEffect());
		layer->addDataSet(DoubleArray(draw_data0, graph_buffer_index), 0xff0000,
			"theta_f");
		layer->addDataSet(DoubleArray(draw_data1, graph_buffer_index), c->dashLineColor(
			0x3333ff, Chart::DashLine), "pi_f");
	}
	break;
	case eV_RPS_THETA_PI:
	{
		c->addTitle("V_RPS_THETA_PI_GRAPH", "timesbi.ttf", 15)->setBackground(0xccccff,
			0x000000, Chart::glassEffect());
		layer->addDataSet(DoubleArray(draw_data0, graph_buffer_index), 0xff0000,
			"V");

		/*layer->addDataSet(DoubleArray(draw_data1, graph_buffer_index), 0x00c000,
			"RPS_THETA")->setUseYAxis2();*/
		layer->addDataSet(DoubleArray(draw_data1, graph_buffer_index), 0x00c000,
			"RPS_THETA");
		layer->addDataSet(DoubleArray(draw_data2, graph_buffer_index), c->dashLineColor(
			0x3333ff, Chart::DashLine), "RPS_PI");
	}
	break;
	}
	layer->setXData(DoubleArray(x_data, (int)graph_buffer_index));
	// Output the chart

	m_ChartViewer.setChart(c);
	//free up resources
	delete c;
	//return 0;
}


void CNEW2Dlg::OnBnClickedRadioXyz()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (graph_type == eXYZ_GRAPH)
		return;
	graph_type = eXYZ_GRAPH;
	buffer_erase();
}


void CNEW2Dlg::OnBnClickedRadioThetaPiM()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (graph_type == eTHETA_PI_M)
		return;
	graph_type = eTHETA_PI_M;
	buffer_erase();
}


void CNEW2Dlg::OnBnClickedRadioThetaPiF()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (graph_type == eTHETA_PI_F)
		return;
	graph_type = eTHETA_PI_F;
	buffer_erase();
}


void CNEW2Dlg::OnBnClickedRadioVRpsThetaPi()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (graph_type == eV_RPS_THETA_PI)
		return;
	graph_type = eV_RPS_THETA_PI;
	buffer_erase();
}

//기존에 그리던 그래프의 모든 데이터를 초기화 시켜줌
void CNEW2Dlg::buffer_erase()
{
	memset(draw_data0, 0, SIZE * sizeof(double));
	memset(draw_data1, 0, SIZE * sizeof(double));
	memset(draw_data2, 0, SIZE * sizeof(double));
	memset(x_data, 0, SIZE * sizeof(double));
	graph_buffer_currentindex = 0;
	graph_buffer_index = 0;
}


void CNEW2Dlg::OnBnClickedButtonSavestart()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CreateLogFile();
	SetDlgItemText(IDC_STATIC_STATUS, GetTimeToString());
	SetDlgItemText(IDC_STATIC_STATUS2, "");
}


void CNEW2Dlg::OnBnClickedButtonSavefinish()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_csFile.Lock();
	if (save_data != NULL)
	{
		fclose(save_data);
		save_data = nullptr; //c++버전에 따라 최신버전에서는 nullptr로 함
	}
	m_csFile.Unlock();

	SetDlgItemText(IDC_STATIC_STATUS2, GetTimeToString());
	CString count;
	count.Format("%d", save_count);					m_Savecount.SetWindowTextA(count);
	save_count = 0;
}


void CNEW2Dlg::OnBnClickedRadioSerial()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_LIst.EnableWindow(TRUE);
	m_comboBaudrate.EnableWindow(TRUE);
	m_ClientPort.EnableWindow(FALSE);

	connection_type = eCONNECT_SERIAL;
}


void CNEW2Dlg::OnBnClickedRadioTcpip()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_LIst.EnableWindow(FALSE);
	m_comboBaudrate.EnableWindow(FALSE);
	m_ClientPort.EnableWindow(TRUE);

	connection_type = eCONNECT_TCP;
}

