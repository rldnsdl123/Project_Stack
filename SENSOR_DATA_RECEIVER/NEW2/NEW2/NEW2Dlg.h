
// NEW2Dlg.h : ��� ����
//

#pragma once
#include "afxwin.h"
#include "define.h"
#include"Comm.h"
#include "ChartViewer.h"
#include"MySocket.h"

enum{eXYZ_GRAPH=0,eTHETA_PI_M,eTHETA_PI_F,eV_RPS_THETA_PI};// �׷��� �׸��´� �ʿ��� �������v�� ����� ���Ż��

//#define PATH "C:\\Users\\VLSI-KIM\\Desktop\\DAON_Data_Draw\\Release" //�����Ͱ� ����� ���� ���
#define PATH "C:\\DAON_Data_Draw\\Release" //�����Ͱ� ����� ���� ���
#define SIZE 1000 //�ѹ��� �׷����� �׸��� �ִ� ������ ����

// CNEW2Dlg ��ȭ ����
class CSocketManger;

class CNEW2Dlg : public CDialogEx
{
	// �����Դϴ�.
public:
	CNEW2Dlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_NEW2_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �����Դϴ�.


// �����Դϴ�.
protected:
	HICON m_hIcon;

	// ������ �޽��� �� �Լ�
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

	afx_msg LRESULT OnReadComm(WPARAM wParam, LPARAM lParam); //�ø��� ��ſ��� ������ �޾ƿ��� �Լ�

public:

	CComm			m_Com;
	TTYSTRUCT	    m_tty;
	int				m_nSelPortNum;
	int				m_dwBaudrate;

	BYTE			m_szBuffer[BUFFER_LEN], m_szBufferTemp[BUFFER_LEN];
	int				m_nBufferLen;
	CString			m_strBuffer;
	BOOL			m_bSaveFile;

	CUIntArray	m_arPortList; // ó�� ��Ʈ ����ٶ� ����ϴ� ����
	void InitPortList(void); // ��밡���� ��Ʈ �ڵ����� ����ִ� �Լ�
	void ProfileWrite(); //ini���� ����
	void ProfileRead(); //ini���� �ϱ�
	void InitPortValue(); // ��Ʈ �⺻ �������ִ� �Լ� 
	void Reposition(); //ũ�⸦ �ڵ����� �ٲܼ��ְ� ���ִ� �Լ� ->�۵������ȵ�
	void DisplayLog(CString strLog); //������ ������� ǥ������
	void SendPacket(BYTE* pBuffer, int nLen); //�����͸� ��Ŷȭ �ؼ� ������
	void draw_data(); //�׷��� �׷��ִ� �Լ�
	void CreateLogFile(); //������ ���� ������ ���� OPEN
	void WriteLogFile(PKT_NEXT_INFO *pkt); //������ ���� ���δ� ����
	void SerialPort_Open(); //�ø��� ��Ʈ ����
	void SerialPort_Close(); //�ø��� ��Ʈ ��������
	void buffer_erase(); //�ٸ������Ͱ� �׷����� ������ ����Ǿ��ִ� �����ε��������� �ʱ�ȭ��

	afx_msg void OnBnClickedBtnOpenPort(); //open ��ư �������� �������ִ� �Լ�
	afx_msg void OnBnClickedBtnClosePort(); //close ��ư �������� ���� ������
	afx_msg void OnDestroy(); 
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedBtLocation(); //��ġ������������
	afx_msg void OnBnClickedBtData(); //���� ��ġ������ �� �������� 
	afx_msg void OnBnClickedBtRemove();// ��ϵ� ������ ��� ����� 
	afx_msg void OnBnClickedBtAlarmSetting();//�ֱ��� ��ġ���� �˸� �����ϱ�  
	afx_msg void OnBnClickedBtAlarmCancel(); //�ֱ��� ��ġ���� �˸� ����ϱ�
	afx_msg void OnBnClickedBtInform(); //FFGD��ġ���� �������� 
	afx_msg void OnBnClickedBtInformSetting(); //FFGD ��ġ ���� �����ϱ�
	afx_msg void OnBnClickedBtParamNovo(); //���� �Ķ���� ���� �����ϱ� (���ֹ߼�)
	afx_msg void OnBnClickedBtParamVo1(); //���� �Ķ���� ���� �����ϱ� (�ֹ߼�)
	afx_msg void OnBnClickedRadioXyz(); // ��ưŬ���� ���� ���� �´� �����Ͱ� �׷���
	afx_msg void OnBnClickedRadioThetaPiM(); // ��ưŬ���� ���� ���� �´� �����Ͱ� �׷���
	afx_msg void OnBnClickedRadioThetaPiF(); // ��ưŬ���� ���� ���� �´� �����Ͱ� �׷���
	afx_msg void OnBnClickedRadioVRpsThetaPi(); // ��ưŬ���� ���� ���� �´� �����Ͱ� �׷���
	afx_msg void OnBnClickedButtonSavestart(); //cv�������
	afx_msg void OnBnClickedButtonSavefinish(); //csv���峡
	afx_msg void OnBnClickedRadioSerial(); //�ø��� ��� ���� ��ư Ŭ��
	afx_msg void OnBnClickedRadioTcpip(); //TCP IP ��Ź�ư Ŭ��

	CEdit m_EditposX; //������ġ X
	CEdit m_EditposY; //������ġ Y
	CEdit m_EditposZ; //������ġ Z
	CEdit m_EditposThetaM; //����THETA M
	CEdit m_EditposThetaF; //����THETA F
	CEdit m_EditPiM; //����PI M
	CEdit m_EditPiF; //����PI F
	CEdit m_EditV; //����ӵ� V
	CEdit m_EditRpsPi; //���� RPS PI
	CEdit m_EditRpsTheta; //���� RPS THETA
	CEdit m_EditAnswer; //������
	CEdit m_CountSize; //��ġ������ ����
	CEdit m_Time; // �۵��ð�
	CEdit m_Period; //���� �����ϰ� ���� �ֱ�
	CEdit m_Editname; //��ġ����_�̸�
	CEdit m_EditDiscription;//��ġ����_Discription
	CEdit m_EditSerial; //��ġ����_�ø���
	CEdit m_EditVersion;//��ġ����_version
	CEdit m_EditParam1; //�Ķ����1~12
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
	CEdit m_Editcommand; //���� ������ ��ɾ� �����
	CEdit m_ClientPort; //TCPIP EDIT CONTROL 
	CEdit m_Receivecount; //���� ������ ���� ǥ��
	CEdit m_Savecount; //���� ���� ǥ���� edit
	 
	BOOL SetComState(); //comport �ӵ� ����
	BOOL InitializeComState(); //comport �ӵ� ����
	BOOL m_bScroll;
	
	CListBox m_listLog; //���ʾƷ� ��� ����ִ� �κ�
	CListBox m_LIst; // list box���� comport���������� ����Ǵ� ����
	
	CButton m_RadioSerial;
	CButton m_CheckXYZ; //����Ʈ�� XYZ �׷����� �׷������� �����ϴ� ����
	
	CComboBox m_comboBaudrate; // �ӵ� �������ִ� combobox
	CString m_strPortName; //���� �Ǿ��ٰ� ǥ�����ٶ� ���
	
	clock_t m_startTime, m_elipsTime; //���۽ð� üũ, 

	CChartViewer m_ChartViewer; //�׷��� ǥ�����ִ� ��ü


	CCriticalSection m_csFile; //������ ����ȭ ���߰� 

	FILE *save_data; //����������� ���� ���

	CSocketManager* m_pSocketManager;

	LRESULT OnCloseSocket(WPARAM wp, LPARAM lp); //���Ͽ��� ���� 
	LRESULT OnAccept(WPARAM wp, LPARAM lp); //���� ������ ������
	LRESULT OnSocketReceive(WPARAM wp, LPARAM lp); //���ϵ����� �����ؼ� ONREADCOMMȣ��

	double tprev = 0; //�ð� ���� �޾ƿͼ� ������ ������
	double *draw_data0; // ���� ���� �޾ƿͼ� �׸��� ���� ������
	double *draw_data1; // ���� ���� �޾ƿͼ� �׸��� ���� ������
	double *draw_data2; // ���� ���� �޾ƿͼ� �׸��� ���� ������

	double *x_data; // �׷����� x�� ������

	int graph_buffer_index; // �����Ͱ� � ���Դ��� �˷��ִ� ����
	int graph_buffer_currentindex; //x�࿡ ���̺� ���� ����
	int data_count; //���� ������ ������ ����
	int graph_type; // �׷��� �׷��� ���� �ٲ��ִ� ���� ������ư�� ���������
	int save_count; //���尹�� 
	int connection_type; //��� ���� �Ӽ�, �ø������� TCP IP����
};


