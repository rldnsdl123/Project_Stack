
// DAON_Data_DrawDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "DAON_Data_Draw.h"
#include "DAON_Data_DrawDlg.h"
#include "afxdialogex.h"

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
END_MESSAGE_MAP()


// CDAONDataDrawDlg 대화 상자



CDAONDataDrawDlg::CDAONDataDrawDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DAON_DATA_DRAW_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	//1번 그래프
	graph_buffer_index1 = 0;
	CAsize1 = 0;

	graph_type = eX_GRAPH;
	graph1_x_min = 0;
	graph1_x_max = 0;
	graph1_y_min = 0;
	graph1_y_max = 0;

	graph1_data = NULL;
	graph1_x_Axis = NULL;
	graph1_check = false;

	//2번 그래프
	graph_buffer_index2 = 0;
	CAsize2 = 0;

	graph2_x_min = 0;
	graph2_x_max = 0;
	graph2_y_min = 0;
	graph2_y_max = 0;

	graph2_data = NULL;
	graph2_x_Axis = NULL;
	graph2_check = false;

	//3번 그래프
	graph_buffer_index3 = 0;
	CAsize3 = 0;

	graph3_x_min = 0;
	graph3_x_max = 0;
	graph3_y_min = 0;
	graph3_y_max = 0;

	graph3_data = NULL;
	graph3_x_Axis = NULL;
	graph3_check = false;
}


void CDAONDataDrawDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_DATA, m_Listdata);
	DDX_Control(pDX, IDC_LIST_INFO_DATA, m_Datainfo);
	DDX_Control(pDX, IDC_DRAW_CHART, m_ChartViewer);
	DDX_Control(pDX, IDC_DRAW_CHART2, m_ChartViewer2);
	DDX_Control(pDX, IDC_DRAW_CHART3, m_ChartViewer3);
	DDX_Control(pDX, IDC_RADIO_X, m_RadioButtonX);
	DDX_Control(pDX, IDC_EDIT_GRAPH1_XMAX, m_Graph1XMax);
	DDX_Control(pDX, IDC_EDIT_GRAPH1_XMIN, m_Graph1Min);
	DDX_Control(pDX, IDC_EDIT_GRAPH2_XMAX, m_Graph2Max);
	DDX_Control(pDX, IDC_EDIT_GRAPH3_XMIN, m_Graph3Min);
	DDX_Control(pDX, IDC_EDIT_GRAPH2_XMIN, m_Graph2MIN);
	DDX_Control(pDX, IDC_EDIT_GRAPH3_XMAX, m_Graph3Max);
	DDX_Control(pDX, IDC_EDIT_GRAPH1_YMAX, m_Graph1YMax);
	DDX_Control(pDX, IDC_EDIT_GRAPH1_YMIN, m_Graph1YMin);
	DDX_Control(pDX, IDC_EDIT_GRAPH2_YMAX2, m_Graph2YMax);
	DDX_Control(pDX, IDC_EDIT_GRAPH2_YMIN2, m_Graph2YMin);
	DDX_Control(pDX, IDC_EDIT_GRAPH3_YMAX3, m_Graph3YMax);
	DDX_Control(pDX, IDC_EDIT_GRAPH3_YMIN3, m_Graph3YMin);
}

BEGIN_MESSAGE_MAP(CDAONDataDrawDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_REFRESH, &CDAONDataDrawDlg::OnBnClickedButtonRefresh)
	ON_LBN_SELCHANGE(IDC_LIST_DATA, &CDAONDataDrawDlg::OnLbnSelchangeListData)
	ON_BN_CLICKED(IDC_BUTTON_REFRESH_GRAPH, &CDAONDataDrawDlg::OnBnClickedButtonRefreshGraph)
	ON_BN_CLICKED(IDC_RADIO_X, &CDAONDataDrawDlg::OnBnClickedRadioX)
	ON_BN_CLICKED(IDC_RADIO_Y, &CDAONDataDrawDlg::OnBnClickedRadioY)
	ON_BN_CLICKED(IDC_RADIO_Z, &CDAONDataDrawDlg::OnBnClickedRadioZ)
	ON_BN_CLICKED(IDC_RADIO_THETA_M, &CDAONDataDrawDlg::OnBnClickedRadioThetaM)
	ON_BN_CLICKED(IDC_RADIO_PI_M, &CDAONDataDrawDlg::OnBnClickedRadioPiM)
	ON_BN_CLICKED(IDC_RADIO_THETA_F, &CDAONDataDrawDlg::OnBnClickedRadioThetaF)
	ON_BN_CLICKED(IDC_RADIO_PI_F, &CDAONDataDrawDlg::OnBnClickedRadioPiF)
	ON_BN_CLICKED(IDC_RADIO_V, &CDAONDataDrawDlg::OnBnClickedRadioV)
	ON_BN_CLICKED(IDC_RADIO_V_RPS_THETA, &CDAONDataDrawDlg::OnBnClickedRadioVRpsTheta)
	ON_BN_CLICKED(IDC_RADIO_V_RPS_PI, &CDAONDataDrawDlg::OnBnClickedRadioVRpsPi)
	ON_BN_CLICKED(IDC_BUTTON_GRAPH1_INSERT_MINMAX, &CDAONDataDrawDlg::OnBnClickedButtonGraph1InsertMinmax)
	ON_BN_CLICKED(IDC_BUTTON_GRAPH1_INSERT_MINMAX2, &CDAONDataDrawDlg::OnBnClickedButtonGraph1InsertMinmax2)
	ON_BN_CLICKED(IDC_BUTTON_GRAPH1_INSERT_MINMAX3, &CDAONDataDrawDlg::OnBnClickedButtonGraph1InsertMinmax3)
	ON_BN_CLICKED(IDC_BUTTON_GRAPH1_INSERT_YMINMAX, &CDAONDataDrawDlg::OnBnClickedButtonGraph1InsertYminmax)
	ON_BN_CLICKED(IDC_BUTTON_GRAPH1_INSERT_YMINMAX2, &CDAONDataDrawDlg::OnBnClickedButtonGraph1InsertYminmax2)
	ON_BN_CLICKED(IDC_BUTTON_GRAPH1_INSERT_YMINMAX3, &CDAONDataDrawDlg::OnBnClickedButtonGraph1InsertYminmax3)
	ON_BN_CLICKED(IDC_BUTTON_RESET_X_Y, &CDAONDataDrawDlg::OnBnClickedButtonResetXY)
END_MESSAGE_MAP()


// CDAONDataDrawDlg 메시지 처리기

BOOL CDAONDataDrawDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
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

	show_dataList();

	m_RadioButtonX.SetCheck(true);
	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CDAONDataDrawDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CDAONDataDrawDlg::OnPaint()
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
HCURSOR CDAONDataDrawDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CDAONDataDrawDlg::show_dataList(void)
{
	CString strFindPath;
	BOOL bWorking;

	strFindPath.Format("%s\\data\\*.csv", GetExecutedPath());

	CFileFind filefind;
	bWorking = filefind.FindFile(strFindPath);

	while (bWorking)
	{
		bWorking = filefind.FindNextFileA(); //이부분 물어보기 bworking이 
		if (filefind.IsDots()) // 검색된 파일이 폴더일 경우 "." 이나 "..."을 반환하는데 이것음  디렉터리를 나타냄
		{
			return;
		}
		else
		{
			CString path = filefind.GetFilePath(); //찾은 파일의 전체 경로를 가져옴
			CString Name = filefind.GetFileName(); //찾은 파일의 이름을 가져옴
			if (Name.Right(3) == "csv") // 파일이름 오른쪽 3개(확장자명을 확인)
			{
				save_path.Add(path);
				m_Listdata.AddString(Name); //파일명 출력
			}
		}
	}
	filefind.Close();
}


void CDAONDataDrawDlg::OnBnClickedButtonRefresh()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_Listdata.ResetContent();
	save_path.RemoveAll();
	show_dataList();
}


void CDAONDataDrawDlg::OnLbnSelchangeListData()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int select = m_Listdata.GetCurSel();

	if (select < 0)
	{
		return;
	}

	CString select_path = save_path.GetAt(select);

	if (select_path.Find("csv")>0)
	{
		m_Datainfo.ResetContent();
		m_Datainfo.AddString(select_path.Right(12)); //파일명과 간단한 정보 출력
		Get_Data(select_path); //데이터 읽어오는 부분
	}	
}


void CDAONDataDrawDlg::OnBnClickedButtonRefreshGraph()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	OnLbnSelchangeListData();

	//if (!graph1_check)
	//{
	//	graph1_x_min = 0;
	//	graph1_x_max = save1_x_max;
	//	graph1_y_min = save1_y_min;
	//	graph1_y_max = save1_y_max;

	//	CString ans;
	//	ans.Format("x축 구간 %0.1lf %0.1lf 설정완료", graph1_x_min, graph1_x_max);
	//	m_Datainfo.AddString(ans);
	//	ans.Format("y축 구간 %.01lf %.01lf 설정완료", graph1_y_min, graph1_y_max);
	//	m_Datainfo.AddString(ans);
	//}
}


void CDAONDataDrawDlg::Get_Data(CString filename)
{
	CStdioFile data_file;

	if (!data_file.Open(filename, CFile::modeRead))
	{
		m_Datainfo.AddString("file not open");
		return;
	}
	CString Readline;
	IMUData imu;

	while (data_file.ReadString(Readline))
	{
		Readline.Replace("\r\n", "");
		imu.Insert_Data(Readline);
		arIMUData.Add(imu);
	}

	switch (graph_type)
	{
	case eX_GRAPH:
		graph1_erase();
		graph1_initialize();
		graph1_draw();
		graph_buffer_index1 = 0;
		arIMUData.RemoveAll();
		break;
	case eY_GRAPH:
		graph2_erase();
		graph2_initialize();
		graph2_draw();
		graph_buffer_index2 = 0;
		arIMUData.RemoveAll();
		break;
	case eZ_GRAPH:
		graph3_erase();
		graph3_initialize();
		graph3_draw();
		graph_buffer_index3 = 0;
		arIMUData.RemoveAll();
		break;
	case eTHETA_M_GRAPH:
		graph1_erase();
		graph1_initialize();
		graph1_draw();
		graph_buffer_index1 = 0;
		arIMUData.RemoveAll();
		break;
	case ePI_M_GRAPH:
		graph2_erase();
		graph2_initialize();
		graph2_draw();
		graph_buffer_index2 = 0;
		arIMUData.RemoveAll();
		break;
	case eTHETA_F_GRAPH:
		graph1_erase();
		graph1_initialize();
		graph1_draw();
		graph_buffer_index1 = 0;
		arIMUData.RemoveAll();
		break;
	case ePI_F_GRAPH:
		graph2_erase();
		graph2_initialize();
		graph2_draw();
		graph_buffer_index2 = 0;
		arIMUData.RemoveAll();
		break;
	case eV_GRAPH:
		graph1_erase();
		graph1_initialize();
		graph1_draw();
		graph_buffer_index1 = 0;
		arIMUData.RemoveAll();
		break;
	case eV_RPS_THETA:
		graph2_erase();
		graph2_initialize();
		graph2_draw();
		graph_buffer_index2 = 0;
		arIMUData.RemoveAll();
		break;
	case eV_RPS_PI:
		graph3_erase();
		graph3_initialize();
		graph3_draw();
		graph_buffer_index3 = 0;
		arIMUData.RemoveAll();
		break;
	}

}


void CDAONDataDrawDlg::OnBnClickedRadioX()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (graph_type == eX_GRAPH)
		return;
	graph_type = eX_GRAPH;
	//graph1_erase();
}


void CDAONDataDrawDlg::OnBnClickedRadioY()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (graph_type == eY_GRAPH)
		return;
	graph_type = eY_GRAPH;
	//buffer_erase();
}


void CDAONDataDrawDlg::OnBnClickedRadioZ()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (graph_type == eZ_GRAPH)
		return;
	graph_type = eZ_GRAPH;
	//buffer_erase();
}


void CDAONDataDrawDlg::OnBnClickedRadioThetaM()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (graph_type == eTHETA_M_GRAPH)
		return;
	graph_type = eTHETA_M_GRAPH;
	//buffer_erase();
}


void CDAONDataDrawDlg::OnBnClickedRadioPiM()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (graph_type == ePI_M_GRAPH)
		return;
	graph_type = ePI_M_GRAPH;
	//buffer_erase();
}


void CDAONDataDrawDlg::OnBnClickedRadioThetaF()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (graph_type == eTHETA_F_GRAPH)
		return;
	graph_type = eTHETA_F_GRAPH;
	//buffer_erase();
}


void CDAONDataDrawDlg::OnBnClickedRadioPiF()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (graph_type == ePI_F_GRAPH)
		return;
	graph_type = ePI_F_GRAPH;
	//buffer_erase();
}


void CDAONDataDrawDlg::OnBnClickedRadioV()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (graph_type == eV_GRAPH)
		return;
	graph_type = eV_GRAPH;
	//buffer_erase();
}


void CDAONDataDrawDlg::OnBnClickedRadioVRpsTheta()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (graph_type == eV_RPS_THETA)
		return;
	graph_type = eV_RPS_THETA;
	//buffer_erase();
}


void CDAONDataDrawDlg::OnBnClickedRadioVRpsPi()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (graph_type == eV_RPS_PI)
		return;
	graph_type = eV_RPS_PI;
	//buffer_erase();
}


void CDAONDataDrawDlg::graph1_erase(void)
{
	if (graph1_data != NULL)
	{
		delete graph1_data;
		graph1_data = NULL;
	}
	if (graph1_x_Axis != NULL)
	{
		delete graph1_x_Axis;
		graph1_x_Axis = NULL;
	}

	CAsize1 = arIMUData.GetSize();

	if (CAsize1 == 0)
	{
		m_Datainfo.AddString("측정된 데이터가 없습니다.");
		return;
	}
}

void CDAONDataDrawDlg::graph1_initialize(void)
{
	graph1_data = new double[CAsize1];
	graph1_x_Axis = new double[CAsize1];

	memset(graph1_data, 0, CAsize1);
	memset(graph1_x_Axis, 0, CAsize1);

	CAsize1 = arIMUData.GetSize();
	if (!graph1_check&&graph1_x_max==0)
	{
		graph1_x_max = CAsize1;
	}
	switch (graph_type)
	{
	case eX_GRAPH:
	{
		for (int i = 0; i < CAsize1; i++)
		{
			IMUData imu = arIMUData.GetAt(i);
			graph1_data[i] = atof(imu.x_data);
			graph1_x_Axis[i] = i;
			graph_buffer_index1++;
			if (graph1_y_max < graph1_data[i] && graph1_check == false)
			{
				graph1_y_max = graph1_data[i];
			}
			if (graph1_y_min > graph1_data[i] && graph1_check == false)
			{
				graph1_y_min = graph1_data[i];
			}
		}
	}
	break;
	case eTHETA_M_GRAPH:
	{
		for (int i = 0; i < CAsize1; i++)
		{
			IMUData imu = arIMUData.GetAt(i);
			graph1_data[i] = atof(imu.theta_m_data);
			graph1_x_Axis[i] = i;
			graph_buffer_index1++;
			if (graph1_y_max < graph1_data[i] && graph1_check == false)
			{
				graph1_y_max = graph1_data[i];
			}
			if (graph1_y_min > graph1_data[i] && graph1_check == false)
			{
				graph1_y_min = graph1_data[i];
			}
		}
	}
	break;
	case eTHETA_F_GRAPH:
	{
		for (int i = 0; i < CAsize1; i++)
		{
			IMUData imu = arIMUData.GetAt(i);
			graph1_data[i] = atof(imu.theta_f_data);
			graph1_x_Axis[i] = i;
			graph_buffer_index1++;
			if (graph1_y_max < graph1_data[i] && graph1_check == false)
			{
				graph1_y_max = graph1_data[i];
			}
			if (graph1_y_min > graph1_data[i] && graph1_check == false)
			{
				graph1_y_min = graph1_data[i];
			}
		}
	}
	break;
	case eV_GRAPH:
	{
		for (int i = 0; i < CAsize1; i++)
		{
			IMUData imu = arIMUData.GetAt(i);
			graph1_data[i] = atof(imu.V_data);
			graph1_x_Axis[i] = i;
			graph_buffer_index1++;
			if (graph1_y_max < graph1_data[i] && graph1_check == false)
			{
				graph1_y_max = graph1_data[i];
			}
			if (graph1_y_min > graph1_data[i] && graph1_check == false)
			{
				graph1_y_min = graph1_data[i];
			}
		}
	}
	break;
	}//SWITCH
}


void CDAONDataDrawDlg::graph1_draw(void)
{

	XYChart *c = new XYChart(630, 300, 0xeeeeff, 0x000000, 1);
	c->setRoundedFrame();

	c->setPlotArea(55, 58, 520, 195, 0xffffff, -1, -1, 0xcccccc, 0xcccccc);

	c->addLegend(50, 30, false, "arialbd.ttf", 9)->setBackground(Chart::Transparent);

	switch (graph_type)
	{
	case eX_GRAPH:
	{
		c->addTitle("X_GRAPH", "timesbi.ttf", 15)->setBackground(0xccccff,
		0x000000, Chart::glassEffect());
	}
	break;
	case eTHETA_M_GRAPH:
	{
		c->addTitle("THETA_M_GRAPH", "timesbi.ttf", 15)->setBackground(0xccccff,
			0x000000, Chart::glassEffect());
	}
	break;
	case eTHETA_F_GRAPH:
	{
		c->addTitle("THETA_F_GRAPH", "timesbi.ttf", 15)->setBackground(0xccccff,
			0x000000, Chart::glassEffect());
	}
	break;
	case eV_GRAPH:
	{
		c->addTitle("V_GRAPH", "timesbi.ttf", 15)->setBackground(0xccccff,
			0x000000, Chart::glassEffect());
	}
	break;
	}

	// Add a title to the y axis
	c->yAxis()->setTitle("VALUE");
	//c->yAxis2()->setTitle("Throughtput (MBytes)");

	c->xAxis()->setLabelStep(3);

	// Add a title to the x axis
	c->xAxis()->setTitle("COUNT");

	// Add a line layer to the chart
	LineLayer *layer = c->addLineLayer();

	// Set the default line width to 2 pixels
	layer->setLineWidth(2);
	if (graph1_check)
	{
		c->xAxis()->setLinearScale(graph1_x_min, graph1_x_max, 0);
		c->yAxis()->setLinearScale(graph1_y_min, graph1_y_max, 0);
		CString ans;
		ans.Format("x축 구간 %0.1lf %0.1lf 설정완료", graph1_x_min, graph1_x_max);
		m_Datainfo.AddString(ans);
		ans.Format("y축 구간 %.01lf %.01lf 설정완료", graph1_y_min, graph1_y_max);
		m_Datainfo.AddString(ans);

		graph1_check = false;
	}
	c->setClipping();

	layer->addDataSet(DoubleArray(graph1_data, graph_buffer_index1), 0xff0000,
		"DATA");

	layer->setXData(DoubleArray(graph1_x_Axis, graph_buffer_index1));

	CString ans;
	ans.Format("데이터 길이 %d ", CAsize1);
	m_Datainfo.AddString(ans);

	m_ChartViewer.setChart(c);

	delete(c);

}
//X축 최대 최소값 받아오기
void CDAONDataDrawDlg::OnBnClickedButtonGraph1InsertMinmax()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString Minmax1;
	m_Graph1XMax.GetWindowTextA(Minmax1);
	graph1_x_max = atof(Minmax1);

	m_Graph1Min.GetWindowTextA(Minmax1);
	graph1_x_min = atof(Minmax1);

	m_Datainfo.AddString("X축 설정완료");

	graph1_check = true;
	OnLbnSelchangeListData();
}


void CDAONDataDrawDlg::OnBnClickedButtonGraph1InsertYminmax()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString YMinmax1;
	m_Graph1YMax.GetWindowTextA(YMinmax1);
	graph1_y_max = atof(YMinmax1);

	m_Graph1YMin.GetWindowTextA(YMinmax1);
	graph1_y_min = atof(YMinmax1);

	m_Datainfo.AddString("Y축 설정완료");

	graph1_check = true;
	OnLbnSelchangeListData();
}


//////////////////////////////////2번 그래프
void CDAONDataDrawDlg::graph2_erase(void)
{
	if (graph2_data != NULL)
	{
		delete graph2_data;
		graph2_data = NULL;
	}
	if (graph2_x_Axis != NULL)
	{
		delete graph2_x_Axis;
		graph2_x_Axis = NULL;
	}

	CAsize2 = arIMUData.GetSize();

	if (CAsize2 == 0)
	{
		m_Datainfo.AddString("측정된 데이터가 없습니다.");
		return;
	}
}
void CDAONDataDrawDlg::graph2_initialize(void)
{
	graph2_data = new double[CAsize2];
	graph2_x_Axis = new double[CAsize2];

	memset(graph2_data, 0, CAsize2);
	memset(graph2_x_Axis, 0, CAsize2);

	CAsize2 = arIMUData.GetSize();
	if (!graph2_check&& graph2_x_max == 0)
	{
		graph2_x_max = CAsize2;
	}
	switch (graph_type)
	{
	case eY_GRAPH:
	{
		for (int i = 0; i < CAsize2; i++)
		{
			IMUData imu = arIMUData.GetAt(i);
			graph2_data[i] = atof(imu.y_data);
			graph2_x_Axis[i] = i;
			graph_buffer_index2++;
			if (graph2_y_max < graph2_data[i] && graph2_check == false)
			{
				graph2_y_max = graph2_data[i];
			}
			if (graph2_y_min > graph2_data[i] && graph2_check == false)
			{
				graph2_y_min = graph2_data[i];
			}
		}
	}
	break;
	case ePI_M_GRAPH:
	{
		for (int i = 0; i < CAsize2; i++)
		{
			IMUData imu = arIMUData.GetAt(i);
			graph2_data[i] = atof(imu.pi_m_data);
			graph2_x_Axis[i] = i;
			graph_buffer_index2++;
			if (graph2_y_max < graph2_data[i] && graph2_check == false)
			{
				graph2_y_max = graph2_data[i];
			}
			if (graph2_y_min > graph2_data[i] && graph2_check == false)
			{
				graph2_y_min = graph2_data[i];
			}
		}
	}
	break;
	case ePI_F_GRAPH:
	{
		for (int i = 0; i < CAsize2; i++)
		{
			IMUData imu = arIMUData.GetAt(i);
			graph2_data[i] = atof(imu.pi_f_data);
			graph2_x_Axis[i] = i;
			graph_buffer_index2++;
			if (graph2_y_max < graph2_data[i] && graph2_check == false)
			{
				graph2_y_max = graph2_data[i];
			}
			if (graph2_y_min > graph2_data[i] && graph2_check == false)
			{
				graph2_y_min = graph2_data[i];
			}
		}
	}
	break;
	case eV_RPS_THETA:
	{
		for (int i = 0; i < CAsize2; i++)
		{
			IMUData imu = arIMUData.GetAt(i);
			graph2_data[i] = atof(imu.rps_theta_data);
			graph2_x_Axis[i] = i;
			graph_buffer_index2++;
			if (graph2_y_max < graph2_data[i] && graph2_check == false)
			{
				graph2_y_max = graph2_data[i];
			}
			if (graph2_y_min > graph2_data[i] && graph2_check == false)
			{
				graph2_y_min = graph2_data[i];
			}
		}
	}
	}//SWITCH

}
void CDAONDataDrawDlg::graph2_draw(void)
{
	XYChart *c = new XYChart(630, 300, 0xeeeeff, 0x000000, 1);
	c->setRoundedFrame();

	c->setPlotArea(55, 58, 520, 195, 0xffffff, -1, -1, 0xcccccc, 0xcccccc);

	c->addLegend(50, 30, false, "arialbd.ttf", 9)->setBackground(Chart::Transparent);

	switch (graph_type)
	{
	case eY_GRAPH:
	{
		c->addTitle("Y_GRAPH", "timesbi.ttf", 15)->setBackground(0xccccff,
			0x000000, Chart::glassEffect());
	}
	break;
	case ePI_M_GRAPH:
	{
		c->addTitle("PI_M_GRAPH", "timesbi.ttf", 15)->setBackground(0xccccff,
			0x000000, Chart::glassEffect());
	}
	break;
	case ePI_F_GRAPH:
	{
		c->addTitle("PI_F_GRAPH", "timesbi.ttf", 15)->setBackground(0xccccff,
			0x000000, Chart::glassEffect());
	}
	break;
	case eV_RPS_THETA:
	{
		c->addTitle("V_RPS_GRAPH", "timesbi.ttf", 15)->setBackground(0xccccff,
			0x000000, Chart::glassEffect());
	}
	break;
	}

	// Add a title to the y axis
	c->yAxis()->setTitle("VALUE");
	//c->yAxis2()->setTitle("Throughtput (MBytes)");

	c->xAxis()->setLabelStep(3);

	// Add a title to the x axis
	c->xAxis()->setTitle("COUNT");

	// Add a line layer to the chart
	LineLayer *layer = c->addLineLayer();

	// Set the default line width to 2 pixels
	layer->setLineWidth(2);
	if (graph2_check)
	{
		c->xAxis()->setLinearScale(graph2_x_min, graph2_x_max, 0);
		c->yAxis()->setLinearScale(graph2_y_min, graph2_y_max, 0);

		CString ans;
		ans.Format("x축 구간 %0.1lf %0.1lf 설정완료", graph2_x_min, graph2_x_max);
		m_Datainfo.AddString(ans);
		ans.Format("y축 구간 %.01lf %.01lf 설정완료", graph2_y_min, graph2_y_max);
		m_Datainfo.AddString(ans);

		graph2_check = false;

	}
	c->setClipping();

	layer->addDataSet(DoubleArray(graph2_data, graph_buffer_index2), 0xff0000,
		"DATA");

	layer->setXData(DoubleArray(graph2_x_Axis, graph_buffer_index2));

	CString ans;
	ans.Format("데이터 길이 %d ", CAsize2);
	m_Datainfo.AddString(ans);

	m_ChartViewer2.setChart(c);

	delete(c);

}

void CDAONDataDrawDlg::OnBnClickedButtonGraph1InsertMinmax2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString Minmax2;
	m_Graph2Max.GetWindowTextA(Minmax2);
	graph2_x_max = atof(Minmax2);

	m_Graph2MIN.GetWindowTextA(Minmax2);
	graph2_x_min = atof(Minmax2);

	m_Datainfo.AddString("X축 설정완료");

	graph2_check = true;
	OnLbnSelchangeListData();
}

void CDAONDataDrawDlg::OnBnClickedButtonGraph1InsertYminmax2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString Minmax2;
	m_Graph2YMax.GetWindowTextA(Minmax2);
	graph2_y_max = atof(Minmax2);

	m_Graph2YMin.GetWindowTextA(Minmax2);
	graph2_y_min = atof(Minmax2);

	m_Datainfo.AddString("X축 설정완료");

	graph2_check = true;
	OnLbnSelchangeListData();
}

////////////////////////////////3번 그래프
void CDAONDataDrawDlg::graph3_erase(void)
{
	if (graph3_data != NULL)
	{
		delete graph3_data;
		graph3_data = NULL;
	}
	if (graph3_x_Axis != NULL)
	{
		delete graph3_x_Axis;
		graph3_x_Axis = NULL;
	}

	CAsize3 = arIMUData.GetSize();

	if (CAsize3 == 0)
	{
		m_Datainfo.AddString("측정된 데이터가 없습니다.");
		return;
	}
}

void CDAONDataDrawDlg::graph3_initialize(void)
{
	graph3_data = new double[CAsize3];
	graph3_x_Axis = new double[CAsize3];

	memset(graph3_data, 0, CAsize3);
	memset(graph3_x_Axis, 0, CAsize3);

	CAsize3 = arIMUData.GetSize();
	if (!graph3_check&&graph3_x_max == 0)
	{
		graph3_x_max = CAsize3;
	}
	switch (graph_type)
	{
	case eZ_GRAPH:
	{
		for (int i = 0; i < CAsize3; i++)
		{
			IMUData imu = arIMUData.GetAt(i);
			graph3_data[i] = atof(imu.z_data);
			graph3_x_Axis[i] = i;
			graph_buffer_index3++;
			if (graph3_y_max < graph3_data[i] && graph3_check == false)
			{
				graph3_y_max = graph3_data[i];
			}
			if (graph3_y_min > graph3_data[i] && graph3_check == false)
			{
				graph3_y_min = graph3_data[i];
			}
		}
	}
	break;
	case eV_RPS_PI:
	{
		for (int i = 0; i < CAsize3; i++)
		{
			IMUData imu = arIMUData.GetAt(i);
			graph3_data[i] = atof(imu.rps_pi_data);
			graph3_x_Axis[i] = i;
			graph_buffer_index3++;
			if (graph3_y_max < graph3_data[i] && graph3_check == false)
			{
				graph3_y_max = graph3_data[i];
			}
			if (graph3_y_min > graph3_data[i] && graph3_check == false)
			{
				graph3_y_min = graph3_data[i];
			}
		}
	}
	break;
	}//SWITCH
}

void CDAONDataDrawDlg::graph3_draw(void)
{
	XYChart *c = new XYChart(630, 300, 0xeeeeff, 0x000000, 1);
	c->setRoundedFrame();

	c->setPlotArea(55, 58, 520, 195, 0xffffff, -1, -1, 0xcccccc, 0xcccccc);

	c->addLegend(50, 30, false, "arialbd.ttf", 9)->setBackground(Chart::Transparent);

	switch (graph_type)
	{
	case eZ_GRAPH:
	{
		c->addTitle("Y_GRAPH", "timesbi.ttf", 15)->setBackground(0xccccff,
			0x000000, Chart::glassEffect());
	}
	break;
	case eV_RPS_PI:
	{
		c->addTitle("V_PI_M_GRAPH", "timesbi.ttf", 15)->setBackground(0xccccff,
			0x000000, Chart::glassEffect());
	}
	break;

	}

	// Add a title to the y axis
	c->yAxis()->setTitle("VALUE");
	//c->yAxis2()->setTitle("Throughtput (MBytes)");

	c->xAxis()->setLabelStep(3);

	// Add a title to the x axis
	c->xAxis()->setTitle("COUNT");

	// Add a line layer to the chart
	LineLayer *layer = c->addLineLayer();

	// Set the default line width to 2 pixels
	layer->setLineWidth(2);
	if (graph3_check)
	{
		c->xAxis()->setLinearScale(graph3_x_min, graph3_x_max, 0);
		c->yAxis()->setLinearScale(graph3_y_min, graph3_y_max, 0);

		CString ans;
		ans.Format("x축 구간 %0.1lf %0.1lf 설정완료", graph3_x_min, graph3_x_max);
		m_Datainfo.AddString(ans);
		ans.Format("y축 구간 %.01lf %.01lf 설정완료", graph3_y_min, graph3_y_max);
		m_Datainfo.AddString(ans);

		graph3_check = false;
	}
	c->setClipping();

	layer->addDataSet(DoubleArray(graph3_data, graph_buffer_index3), 0xff0000,
		"DATA");

	layer->setXData(DoubleArray(graph3_x_Axis, graph_buffer_index3));

	CString ans;
	ans.Format("데이터 길이 %d ", CAsize3);
	m_Datainfo.AddString(ans);

	m_ChartViewer3.setChart(c);

	delete(c);
}

void CDAONDataDrawDlg::OnBnClickedButtonGraph1InsertMinmax3()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString Minmax3;
	m_Graph3Max.GetWindowTextA(Minmax3);
	graph3_x_max = atof(Minmax3);

	m_Graph3Min.GetWindowTextA(Minmax3);
	graph3_x_min = atof(Minmax3);

	m_Datainfo.AddString("X축 설정완료");

	graph3_check = true;
	OnLbnSelchangeListData();
}

void CDAONDataDrawDlg::OnBnClickedButtonGraph1InsertYminmax3()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString Minmax3;
	m_Graph3YMax.GetWindowTextA(Minmax3);
	graph3_y_max = atof(Minmax3);

	m_Graph3YMin.GetWindowTextA(Minmax3);
	graph3_y_min = atof(Minmax3);

	m_Datainfo.AddString("X축 설정완료");

	graph3_check = true;
	OnLbnSelchangeListData();
}


void CDAONDataDrawDlg::OnBnClickedButtonResetXY()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	graph1_x_min = 0;
	graph1_x_max = 0;
	graph1_y_min = 0;
	graph1_y_max = 0;
	
	graph2_x_min = 0;
	graph2_x_max = 0;
	graph2_y_min = 0;
	graph2_y_max = 0;
	
	graph3_x_min = 0;
	graph3_x_max = 0;
	graph3_y_min = 0;
	graph3_y_max = 0;
}
