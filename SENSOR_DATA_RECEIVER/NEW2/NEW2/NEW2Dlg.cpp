
// NEW2Dlg.cpp : ���� ����
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


// ���� ���α׷� ������ ���Ǵ� CAboutDlg ��ȭ �����Դϴ�.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

// �����Դϴ�.
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


// CNEW2Dlg ��ȭ ����



CNEW2Dlg::CNEW2Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_NEW2_DIALOG, pParent)
	, m_strPortName(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	memset(m_szBuffer, 0, sizeof(BYTE)*BUFFER_LEN);
	g_strInIPath.Format("%s\\NEW2.ini", GetExecutedPath());
	m_bScroll = TRUE;

	// �׷����� �׸� �����Ͱ� �� double�� ������ �޸� �Ҵ�
	draw_data0 = new double[SIZE];
	draw_data1 = new double[SIZE];
	draw_data2 = new double[SIZE];
	//x�� ���� �� x_data ������ �޸� �Ҵ�
	x_data = new double[SIZE];

	//�ʱ�ȭ
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

	//TCP����� ���ؼ� ���
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


// CNEW2Dlg �޽��� ó����

BOOL CNEW2Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// �ý��� �޴��� "����..." �޴� �׸��� �߰��մϴ�.

	// IDM_ABOUTBOX�� �ý��� ��� ������ �־�� �մϴ�.
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

	// �� ��ȭ ������ �������� �����մϴ�.  ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	//  �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.

	// TODO: ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.


	m_Com.SetSendHandle(this->m_hWnd);
	InitPortValue(); // �⺻ �ø��� ��� ���� 
	InitPortList(); // ��밡���� com��Ʈ ����ִ°�
	InitializeComState(); //��żӵ� �ʱ�ȭ

	Reposition(); //��Ʈ�� ũ�⳪ ��ġ ����

	m_CheckXYZ.SetCheck(true); //�׷��� ����Ʈ �� ����
	m_RadioSerial.SetCheck(true); // �⺻ ��� ����Ʈ
	OnBnClickedRadioSerial(); // �⺻ ��� ����Ʈ

	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
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

// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸�����
//  �Ʒ� �ڵ尡 �ʿ��մϴ�.  ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
//  �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void CNEW2Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ�Դϴ�.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Ŭ���̾�Ʈ �簢������ �������� ����� ����ϴ�.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �������� �׸��ϴ�.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ����ڰ� �ּ�ȭ�� â�� ���� ���ȿ� Ŀ���� ǥ�õǵ��� �ý��ۿ���
//  �� �Լ��� ȣ���մϴ�.
HCURSOR CNEW2Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//port �⺻ ���� ��Ʈ������ : 9, �и�Ƽ��Ʈ 0, ��ž��Ʈ0
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

LRESULT CNEW2Dlg::OnCloseSocket(WPARAM wp, LPARAM lp) //wp this�� �ڱ��ڽ��� ���� ���Ͻĺ��ڸ� ������
{
	SOCKET  s = (SOCKET)wp;
	m_pSocketManager->Close(s);

	m_listLog.AddString("client�� ������ �����Ͽ����ϴ�.");
	return 1;
}

LRESULT CNEW2Dlg::OnAccept(WPARAM wp, LPARAM lp) //CLIENT�� ���������� ȣ�� 
{
	SOCKET  serviceSocket = (SOCKET)wp;
	m_pSocketManager->Accept(serviceSocket);
	m_listLog.AddString("client�� �����Ͽ����ϴ�.");
	return 1;
}

//����� ���� �Լ� �ڵ鷯
//cmysocket���� ȣ��
LRESULT CNEW2Dlg::OnSocketReceive(WPARAM wp, LPARAM lp)
{
	//�ø��󿡼��� onreadcomm�̶� �Ȱ���
	OnReadComm(wp, lp);
	return 1;
}

//������ �޾ƿͼ� ��ɺ��� ó�����ִ� �Լ�
//�ӵ��� �ʹ������� �ѹ��� 1���� ��Ŷ�� �ƴ϶� 2�� �̻��� ��Ŷ�� ���� �� �ֱ⶧���� �Ѱ� ��Ŷ�� ���̸�ŭ ó�����ְ� 
//�� �ڷο� ��Ŷ�� ó������
// 1����Ŷũ�� ��ŭ  m_szBuffer����  �������� ����Ʈ
LRESULT CNEW2Dlg::OnReadComm(WPARAM wParam, LPARAM lParam)
{
	char* pData = (char*)wParam; //ó�� data�� pdata�� �޾ƿ�
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

	memcpy(m_szBuffer + m_nBufferLen, pData, nLen);//�޾ƿ� �����͸� m_szBuffer�� ����

	m_nBufferLen += nLen; // m_nbufferlen ���ŵ� ��Ŷ ���� 

REPEAT:
	if (m_nBufferLen > 0)
	{
		TRACE("------------------------------m_nBufferLen : %d\n", m_nBufferLen);
	}
	int head_pos = Find_STX(m_szBuffer, m_nBufferLen); //preamble�� �߸�������� find_stx���� üũ

	//�����޼��� ���
	if (head_pos == -1)
	{
		strLog.Format("not Find Preamble");
		DisplayLog(strLog);
		m_nBufferLen = 0;
		memset(m_szBuffer, 0, sizeof(BYTE)*BUFFER_LEN);
		return 0;
	}

	//���������� �߰��� ������� �߶� �Ǿ����� ������
	if (head_pos > 0)
	{
		memcpy(m_szBuffer, m_szBuffer + head_pos, m_nBufferLen - head_pos);
	}

	int nFFHSize = sizeof(stFFH_header);

	if (m_nBufferLen < nFFHSize) //ù��° �����Ͱ� ���ũ�⺸�� ������ ����� �ؼ� �� �� �����ϱ� ������Ŷ�� ��ٸ�
		return 0;


	char szBuffer[128] = { 0 };
	stFFH_header  ffh;
	memcpy(&ffh, m_szBuffer, nFFHSize);//�޾ƿ� ��ɾ ffh����ü�� ����

	int nPayloadLen = ffh.uLen; //�޾ƿ� ���� nPayloadLen
	int nCommand = (int)ffh.cCmd;   // ���ŵ� ��ɾ� ����
	int crc_index = nFFHSize + nPayloadLen;//crc������ġ

	int nPakcetSize = nPayloadLen + 4 + nFFHSize;
	
	if (nPakcetSize > m_nBufferLen)
	{
		strLog.Format("�����Ͱ� �� �ȵ��� %d", m_nBufferLen);
		DisplayLog(strLog);
		return 0;
	}


	int test_crc = crc32a(m_szBuffer + 2, nPakcetSize - 6);

	int crc_value = 0;
	memcpy(&crc_value, m_szBuffer + crc_index, 4);

	// CRCüũ�ϴ� �κ� 
	// test_crc�� ���� crc�� ����ϴ� �κ�
	// crc_Value�� crc�����ϴ� �κ�
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

	switch (nCommand) //���� ��ɿ� ���� swtich case ����
	{
	case 0x41:    // ���� �������� �ð��� ��ġ���� ��û
	{
		PKT_NEXT_INFO  pkt; //�����͸� �޾Ƽ� ������ ����ü
		int size = sizeof(PKT_NEXT_INFO);

		memcpy(&pkt, &m_szBuffer, size); //pkt�� �޾ƿ� ������ ����
		tprev = pkt.t; //�޾ƿ� �ð��� �������� check_time�� ����-> �Ʒ� ��ġ���� �������⿡�� ���


		//����ü�� ����� �����͸� ������ Ȯ���� �� �ְ� �����
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

		strLog.Format("��ġ ������ ���ſϷ�");
		DisplayLog(strLog);

	}
	break;

	case 0x43:    // ���� ��ġ ������ �� ��������
	{
		PKT_SIZE PS;

		int size = sizeof(PKT_SIZE);

		memset(&PS, 0, size);
		memcpy(&PS, &m_szBuffer, size); //PS �޾ƿ� ������ ����

		//������ Ȯ���� �� �ֵ��� �����
		CString ans;
		ans.Format("%#x", PS.cCmd);					m_EditAnswer.SetWindowTextA(ans);
		ans.Format("%d", PS.size);					m_CountSize.SetWindowTextA(ans);

		strLog.Format("���� ��ġ ������ ���ſϷ�");
		DisplayLog(strLog);

	}
	break;
	case 0x45:    //��ϵ� ������ ��� �����
	{
		PKT_RESP PR;

		int size = sizeof(PKT_RESP);

		memset(&PR, 0, size);
		memcpy(&PR, &m_szBuffer, size);//PS �޾ƿ� ������ ����

		//�޾ƿ��� �����Ͱ� ���� ���⶧���� ��������� �����
		CString ans;
		ans.Format("%#x", PR.cCmd);					m_EditAnswer.SetWindowTextA(ans);

		strLog.Format("������ ���� �Ϸ�");
		DisplayLog(strLog);

	}
	break;
	case 0x47:    // �ֱ��� �˸� ���� �����ϱ�
	{
		PKT_RESP PR;

		int size = sizeof(PKT_RESP);

		memset(&PR, 0, size);
		memcpy(&PR, &m_szBuffer, size);

		CString ans;
		ans.Format("%#x", PR.cCmd);					m_EditAnswer.SetWindowTextA(ans);

		m_Period.GetWindowTextA(ans);
		int period = atoi(ans);
		strLog.Format("%d ms�ֱ� ���� �Ϸ�", period);
		DisplayLog(strLog);

	}
	break;
	case 0x49:    // �ֱ��� ��ġ���� �˸� ����ϱ�
	{
		PKT_RESP PR;

		int size = sizeof(PKT_RESP);

		memset(&PR, 0, size);
		memcpy(&PR, &m_szBuffer, size);

		CString ans;
		ans.Format("%#x", PR.cCmd);					m_EditAnswer.SetWindowTextA(ans);

		strLog.Format("�ֱ� ��� �Ϸ�");
		DisplayLog(strLog);

	}
	break;
	case 0x4B:    // �ֱ��� ��ġ���� �˸� ��ɿ� ���� �˸� ����
	{
		int size = sizeof(PKT_NEXT_INFO);

		PKT_NEXT_INFO pkt;
		memcpy(&pkt, &m_szBuffer, size); //pkt�� �޾ƿ� ������ ����

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

		data_count++; //���ŵ����� ������ ǥ���ϱ� ���� �Ѱ��� ����
		ans.Format("%d", data_count);					m_Receivecount.SetWindowTextA(ans);

		x_data[graph_buffer_index] = graph_buffer_currentindex; //�׷��� �׸��� x�࿡ ���� �ڵ����� �־���

		switch (graph_type) //���� Ŭ���� ���� ��ư�� ���� switch case�� �۵�
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
		draw_data(); //�׷��� �׸��� �Լ� ȣ��
		graph_buffer_index++; 
		graph_buffer_currentindex++;

		if (graph_buffer_index == SIZE) //���� ������ ����� ���� �� ó�� ���� �����͸� ����鼭 ���ο� �����͸� �޾Ƽ� �׸�
		{
			graph_buffer_index--;
			memcpy(&draw_data0[0], &draw_data0[1], sizeof(double)*(SIZE - 1)); //ù��° �ڸ��Ÿ� ����鼭 �������� ��ĭ�� �̵� ������ ���ڸ��� ����
			memcpy(&draw_data1[0], &draw_data1[1], sizeof(double)*(SIZE - 1)); //ù��° �ڸ��Ÿ� ����鼭 �������� ��ĭ�� �̵� ������ ���ڸ��� ����
			memcpy(&draw_data2[0], &draw_data2[1], sizeof(double)*(SIZE - 1)); //ù��° �ڸ��Ÿ� ����鼭 �������� ��ĭ�� �̵� ������ ���ڸ��� ����
			memcpy(&x_data[0], &x_data[1], sizeof(double)*(SIZE - 1)); //ù��° �ڸ��Ÿ� ����鼭 �������� ��ĭ�� �̵� ������ ���ڸ��� ����
		}
		m_csFile.Lock();
		WriteLogFile(&pkt);
		m_csFile.Unlock();// ������ ����ȭ �����ִ� �κ�
	}
	break;
	case 0x4D:    // FFGD ��ġ ������ ��������
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

		strLog.Format("��ġ���� ���ſϷ�");
		DisplayLog(strLog);
	}
	break;
	case 0x4F:    // FFGD ��ġ ������ �����Ѵ�.
	{
		PKT_RESP PR;

		int size = sizeof(PKT_RESP);

		memset(&PR, 0, size);
		memcpy(&PR, &m_szBuffer, size);

		CString ans;
		ans.Format("%#x", PR.cCmd);					m_EditAnswer.SetWindowTextA(ans);

		strLog.Format("��ġ ���� �����Ϸ�");
		DisplayLog(strLog);
	}
	break;
	case 0x51:    // ���� �Ķ���� ���� �����ϱ�(���ֹ߼�)
	{
		PKT_RESP PR;

		int size = sizeof(PKT_RESP);

		memset(&PR, 0, size);
		memcpy(&PR, &m_szBuffer, size);

		CString ans;
		ans.Format("%#x", PR.cCmd);					m_EditAnswer.SetWindowTextA(ans);

		strLog.Format("���ֹ߼� �Ķ���� ���� �Ϸ�");
		DisplayLog(strLog);

	}
	break;
	case 0x53:    // ���� �Ķ���� ���� �����ϱ�(�ֹ߼�)
	{
		PKT_RESP PR;

		int size = sizeof(PKT_RESP);

		memset(&PR, 0, size);
		memcpy(&PR, &m_szBuffer, size);

		CString ans;
		ans.Format("%#x", PR.cCmd);					m_EditAnswer.SetWindowTextA(ans);

		strLog.Format("�ֹ߼� �Ķ���� ���� �Ϸ�");
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
	//1�� �̻��� ��Ŷ�� �������� 1���� ��Ŷ�� ó���ϰ� ���� ��Ŷ�� ó���ϱ����� ������ ���ŵǴ°��� �����ϱ� ���� 
	//goto repeate�� ���
	memset(m_szBuffer, 0, sizeof(BYTE)*BUFFER_LEN);
	m_nBufferLen = 0;
	return 0;
}
//���� ���� ��_��_��_��_��_��.csv �������� ����
void CNEW2Dlg::CreateLogFile()
{
	CString strDate, strDate2, strPath;
	CString strlogF;

	SYSTEMTIME st;
	GetLocalTime(&st);

	strPath.Format(PATH);//�׷��� �׸��� ������Ʈ release������ data�� ���� �����
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

//���ŵ� �����͸� ����Ÿ csv���Ͽ� ����
//���α׷� �����߿� csv���� ���� ����
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
	// fprintf �ߴٰ� �ٷ� ���°� �ƴ�, ������ ���ŵ��� ������ �������� �����Ͱ� ������ ����, fflush�� ���� ������ �ϵ��ũ�� �����
}

//�⺻ ��밡���� ��Ʈ ������
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

	switch (iCurSel) // ����  �ӵ� 
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

	switch (m_dwBaudrate) // ����  �ӵ� 
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
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	int test;
	if (connection_type == eCONNECT_SERIAL) //�ø��� ����� ���
	{
		if (m_pSocketManager == NULL)  
		{
			return;
		}
		if (m_pSocketManager->m_pAcceptSocket != NULL) //���� tcp������ �����ִٸ�
		{
			m_pSocketManager->Close();
			m_listLog.AddString("TCP���� ����");
		}//TCP���� ����
		SerialPort_Open();
	}
	else
	{
		if (m_Com.m_flgConnected) //�ø��� �������� �˷���-> ����Ǿ����� ������
		{
			m_bSaveFile = FALSE;
			m_Com.CloseConnection();
			m_listLog.AddString("SERIAL ��Ʈ����");
		}

		CString port;
		m_ClientPort.GetWindowTextA(port);
		test = atoi(port);
		if (test > 1000 && test < 65000)// ��Ʈ��ȣ�� 1000������ 65000�� ������ ��쿡�� ����
		{
			m_listLog.AddString("TCP���� �����");
			m_pSocketManager->TCPServerStart(this, test);
		}
		else
		{
			AfxMessageBox("��Ʈ��ȣ�� ������ �Է����ּ���"); //�߸��� ���ڰ��� ���ڰ� �ԷµȰ�� ���� �޼����� �����
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

	if (m_Com.m_flgConnected) //�ø��� �������� �˷���
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
		strText.Format("Com %d �����", m_nSelPortNum);
		SetWindowText(strText);
		m_strPortName = strText;
		m_bSaveFile = TRUE;
	}
	else
	{
		CString strText;
		strText.Format("������� �ʾ���", m_nSelPortNum);
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
		strText.Format("������� �ʾ���", m_nSelPortNum);
		m_strPortName = strText;
		SetWindowText(strText);

		InitPortValue();
	}
	else
	{
		AfxMessageBox("������� �ʾҽ��ϴ�.");
	}
}



void CNEW2Dlg::OnBnClickedBtnClosePort()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	//��� ���ǿ� ���缭 ���Ḹ ���ָ� ��
	if (connection_type == eCONNECT_SERIAL)
	{
		SerialPort_Close();
		m_listLog.AddString("SERIAL ��Ʈ����");
	}
	else
	{
		m_pSocketManager->Close();
		m_listLog.AddString("TCP���� ����");
	}
	UpdateData(FALSE);
}

void CNEW2Dlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
}

void CNEW2Dlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
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

//��ġ���� ��������
void CNEW2Dlg::OnBnClickedBtLocation()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	BYTE message[512] = { 0 };
	stFFD_header_period send_hdr;


	send_hdr.uPremable[0] = 0xAA;
	send_hdr.uPremable[1] = 0x55;
	send_hdr.uLen = 8;
	send_hdr.cCmd = 0x40;
	send_hdr.period = tprev;

	int nLen = sizeof(send_hdr) - 2 - 4;
	send_hdr.crc = crc32a(message + 2, nLen); // ���������� ���̸�2�����ְ� ��ü���̿��� �����������2 ���ذ�

	memcpy(message, &send_hdr, sizeof(send_hdr));

	SendPacket(message, sizeof(send_hdr));

	CString ans;
	ans.Format("%#x", send_hdr.cCmd);					m_Editcommand.SetWindowTextA(ans);

}

//���� ��ġ ������ �� ��������
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

//��ϵ� ������ ��� �����
void CNEW2Dlg::OnBnClickedBtRemove()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
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

//�ֱ��� ��ġ���� �˸� �����ϱ�
void CNEW2Dlg::OnBnClickedBtAlarmSetting()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	BYTE message[512] = { 0 };
	stFFD_header_period send_period;
	CString Period, strlog;

	m_Period.GetWindowTextA(Period);
	if (Period == "")
	{
		strlog.Format("�ֱ⸦ �Է����ּ���");
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

//�ֱ��� ��ġ���� �˸� ����ϱ�
void CNEW2Dlg::OnBnClickedBtAlarmCancel()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
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

//FFGD ��ġ ���� ��������
void CNEW2Dlg::OnBnClickedBtInform()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
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

//FFGD ��ġ ���� �����ϱ�
void CNEW2Dlg::OnBnClickedBtInformSetting()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
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

	//�Է��� ��ġ������ sprintf_s�� ����Ͽ� send_dev�� ����ְ� crc�ڵ带 �߰��� �Ŀ� ������ ��Ŷ�� ����

	ans.Format("%#x", send_dev.cCmd);					m_Editcommand.SetWindowTextA(ans);

}

//FFGD ���� �Ķ���� ���� �����ϱ�(���ֹ߼�) �� ���޵�
void CNEW2Dlg::OnBnClickedBtParamNovo()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
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

//���� �Ķ���� ���� �����ϱ�(�ֹ߼�)
void CNEW2Dlg::OnBnClickedBtParamVo1()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
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
	//������ ���õ� �׷��� Ÿ�Կ� ���� ���� �´� �׷����� �׷���
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
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if (graph_type == eXYZ_GRAPH)
		return;
	graph_type = eXYZ_GRAPH;
	buffer_erase();
}


void CNEW2Dlg::OnBnClickedRadioThetaPiM()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if (graph_type == eTHETA_PI_M)
		return;
	graph_type = eTHETA_PI_M;
	buffer_erase();
}


void CNEW2Dlg::OnBnClickedRadioThetaPiF()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if (graph_type == eTHETA_PI_F)
		return;
	graph_type = eTHETA_PI_F;
	buffer_erase();
}


void CNEW2Dlg::OnBnClickedRadioVRpsThetaPi()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if (graph_type == eV_RPS_THETA_PI)
		return;
	graph_type = eV_RPS_THETA_PI;
	buffer_erase();
}

//������ �׸��� �׷����� ��� �����͸� �ʱ�ȭ ������
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
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CreateLogFile();
	SetDlgItemText(IDC_STATIC_STATUS, GetTimeToString());
	SetDlgItemText(IDC_STATIC_STATUS2, "");
}


void CNEW2Dlg::OnBnClickedButtonSavefinish()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_csFile.Lock();
	if (save_data != NULL)
	{
		fclose(save_data);
		save_data = nullptr; //c++������ ���� �ֽŹ��������� nullptr�� ��
	}
	m_csFile.Unlock();

	SetDlgItemText(IDC_STATIC_STATUS2, GetTimeToString());
	CString count;
	count.Format("%d", save_count);					m_Savecount.SetWindowTextA(count);
	save_count = 0;
}


void CNEW2Dlg::OnBnClickedRadioSerial()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_LIst.EnableWindow(TRUE);
	m_comboBaudrate.EnableWindow(TRUE);
	m_ClientPort.EnableWindow(FALSE);

	connection_type = eCONNECT_SERIAL;
}


void CNEW2Dlg::OnBnClickedRadioTcpip()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_LIst.EnableWindow(FALSE);
	m_comboBaudrate.EnableWindow(FALSE);
	m_ClientPort.EnableWindow(TRUE);

	connection_type = eCONNECT_TCP;
}

