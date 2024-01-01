
// NEW2Dlg.h : 헤더 파일
//

#pragma once
#include "afxwin.h"
#include "define.h"
#include"Comm.h"
#include "ChartViewer.h"
#include"MySocket.h"

enum{eXYZ_GRAPH=0,eTHETA_PI_M,eTHETA_PI_F,eV_RPS_THETA_PI};// 그래프 그리는대 필요한 라디오버틍과 연결된 열거상수

//#define PATH "C:\\Users\\VLSI-KIM\\Desktop\\DAON_Data_Draw\\Release" //데이터가 저장될 폴더 경로
#define PATH "C:\\DAON_Data_Draw\\Release" //데이터가 저장될 폴더 경로
#define SIZE 1000 //한번에 그래프를 그릴수 있는 데이터 개수

// CNEW2Dlg 대화 상자
class CSocketManger;

class CNEW2Dlg : public CDialogEx
{
	// 생성입니다.
public:
	CNEW2Dlg(CWnd* pParent = NULL);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_NEW2_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

	afx_msg LRESULT OnReadComm(WPARAM wParam, LPARAM lParam); //시리얼 통신에서 데이터 받아오는 함수

public:

	CComm			m_Com;
	TTYSTRUCT	    m_tty;
	int				m_nSelPortNum;
	int				m_dwBaudrate;

	BYTE			m_szBuffer[BUFFER_LEN], m_szBufferTemp[BUFFER_LEN];
	int				m_nBufferLen;
	CString			m_strBuffer;
	BOOL			m_bSaveFile;

	CUIntArray	m_arPortList; // 처음 포트 띄어줄때 사용하는 변수
	void InitPortList(void); // 사용가능한 포트 자동으로 띄어주는 함수
	void ProfileWrite(); //ini파일 쓰기
	void ProfileRead(); //ini파일 일기
	void InitPortValue(); // 포트 기본 설정해주는 함수 
	void Reposition(); //크기를 자동으로 바꿀수있게 해주는 함수 ->작동아직안됨
	void DisplayLog(CString strLog); //오류가 났을경우 표시해줌
	void SendPacket(BYTE* pBuffer, int nLen); //데이터를 패킷화 해서 보내줌
	void draw_data(); //그래프 그려주는 함수
	void CreateLogFile(); //데이터 파일 저장을 위해 OPEN
	void WriteLogFile(PKT_NEXT_INFO *pkt); //데이터 파일 전부다 저장
	void SerialPort_Open(); //시리얼 포트 연결
	void SerialPort_Close(); //시리얼 포트 연결해제
	void buffer_erase(); //다른데이터가 그려질때 기존에 저장되어있던 버퍼인덱스값들이 초기화됨

	afx_msg void OnBnClickedBtnOpenPort(); //open 버튼 눌렀을때 연결해주는 함수
	afx_msg void OnBnClickedBtnClosePort(); //close 버튼 눌렀을때 연결 끊어줌
	afx_msg void OnDestroy(); 
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedBtLocation(); //위치정보가져오기
	afx_msg void OnBnClickedBtData(); //남은 위치데이터 수 가져오기 
	afx_msg void OnBnClickedBtRemove();// 기록된 데이터 모두 지우기 
	afx_msg void OnBnClickedBtAlarmSetting();//주기적 위치정보 알림 설정하기  
	afx_msg void OnBnClickedBtAlarmCancel(); //주기정 위치정보 알림 취소하기
	afx_msg void OnBnClickedBtInform(); //FFGD장치정보 가져오기 
	afx_msg void OnBnClickedBtInformSetting(); //FFGD 장치 정보 설정하기
	afx_msg void OnBnClickedBtParamNovo(); //각종 파라메터 정보 설정하기 (비휘발성)
	afx_msg void OnBnClickedBtParamVo1(); //각종 파라메터 정보 설정하기 (휘발성)
	afx_msg void OnBnClickedRadioXyz(); // 버튼클릭에 따라 각각 맞는 데이터가 그려짐
	afx_msg void OnBnClickedRadioThetaPiM(); // 버튼클릭에 따라 각각 맞는 데이터가 그려짐
	afx_msg void OnBnClickedRadioThetaPiF(); // 버튼클릭에 따라 각각 맞는 데이터가 그려짐
	afx_msg void OnBnClickedRadioVRpsThetaPi(); // 버튼클릭에 따라 각각 맞는 데이터가 그려짐
	afx_msg void OnBnClickedButtonSavestart(); //cv저장시작
	afx_msg void OnBnClickedButtonSavefinish(); //csv저장끝
	afx_msg void OnBnClickedRadioSerial(); //시리얼 통신 라디오 버튼 클릭
	afx_msg void OnBnClickedRadioTcpip(); //TCP IP 통신버튼 클릭

	CEdit m_EditposX; //현재위치 X
	CEdit m_EditposY; //현재위치 Y
	CEdit m_EditposZ; //현재위치 Z
	CEdit m_EditposThetaM; //현재THETA M
	CEdit m_EditposThetaF; //현재THETA F
	CEdit m_EditPiM; //현재PI M
	CEdit m_EditPiF; //현재PI F
	CEdit m_EditV; //현재속도 V
	CEdit m_EditRpsPi; //현재 RPS PI
	CEdit m_EditRpsTheta; //현재 RPS THETA
	CEdit m_EditAnswer; //응답명령
	CEdit m_CountSize; //위치데이터 갯수
	CEdit m_Time; // 작동시간
	CEdit m_Period; //내가 설정하고 싶은 주기
	CEdit m_Editname; //장치정보_이름
	CEdit m_EditDiscription;//장치정보_Discription
	CEdit m_EditSerial; //장치정보_시리얼
	CEdit m_EditVersion;//장치정보_version
	CEdit m_EditParam1; //파라미터1~12
	CEdit m_EditParam10;
	CEdit m_EditParam11;
	CEdit m_EditParam12;
	CEdit m_EditParam2;
	CEdit m_EditParam3;
	CEdit m_EditParam4;
	CEdit m_EditParam5;
	CEdit m_EditParam6;
	CEdit m_EditParam7;
	CEdit m_EditParam8;
	CEdit m_EditParam9;
	CEdit m_Editcommand; //내가 전송한 명령어 띄어줌
	CEdit m_ClientPort; //TCPIP EDIT CONTROL 
	CEdit m_Receivecount; //수신 데이터 개수 표시
	CEdit m_Savecount; //저장 갯수 표시할 edit
	 
	BOOL SetComState(); //comport 속도 설정
	BOOL InitializeComState(); //comport 속도 설정
	BOOL m_bScroll;
	
	CListBox m_listLog; //왼쪽아래 기록 띄어주는 부분
	CListBox m_LIst; // list box에서 comport선택했을때 저장되는 변수
	
	CButton m_RadioSerial;
	CButton m_CheckXYZ; //디폴트로 XYZ 그래프가 그려지도록 설정하는 변수
	
	CComboBox m_comboBaudrate; // 속도 설정해주는 combobox
	CString m_strPortName; //연결 되었다고 표시해줄때 사용
	
	clock_t m_startTime, m_elipsTime; //시작시간 체크, 

	CChartViewer m_ChartViewer; //그래프 표시해주는 객체


	CCriticalSection m_csFile; //스레드 동기화 맞추가 

	FILE *save_data; //파일입출력을 위해 사용

	CSocketManager* m_pSocketManager;

	LRESULT OnCloseSocket(WPARAM wp, LPARAM lp); //소켓연결 해제 
	LRESULT OnAccept(WPARAM wp, LPARAM lp); //소켓 연결을 승인함
	LRESULT OnSocketReceive(WPARAM wp, LPARAM lp); //소켓데이터 수신해서 ONREADCOMM호출

	double tprev = 0; //시간 정보 받아와서 저장할 데이터
	double *draw_data0; // 각각 내가 받아와서 그리고 싶은 데이터
	double *draw_data1; // 각각 내가 받아와서 그리고 싶은 데이터
	double *draw_data2; // 각각 내가 받아와서 그리고 싶은 데이터

	double *x_data; // 그래프의 x축 데이터

	int graph_buffer_index; // 데이터가 몇개 들어왔는지 알려주는 변수
	int graph_buffer_currentindex; //x축에 레이블에 사용될 변수
	int data_count; //수신 데이터 개수가 저장
	int graph_type; // 그래프 그려줄 값을 바꿔주는 변수 라디오버튼과 연결되있음
	int save_count; //저장갯수 
	int connection_type; //통신 연결 속성, 시리얼인지 TCP IP인지
};


