
// DAON_Data_DrawDlg.h: 헤더 파일
//

#pragma once
#include"ChartViewer.h"
#include"define.h"
#include<string>
#include"IMUData.h"

enum { eX_GRAPH = 0, eY_GRAPH, eZ_GRAPH, eTHETA_M_GRAPH, ePI_M_GRAPH, eTHETA_F_GRAPH, ePI_F_GRAPH, eV_GRAPH, eV_RPS_THETA, eV_RPS_PI };


// CDAONDataDrawDlg 대화 상자
class CDAONDataDrawDlg : public CDialogEx
{
// 생성입니다.
public:
	CDAONDataDrawDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DAON_DATA_DRAW_DIALOG };
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
public:
	CListBox m_Listdata;

	void show_dataList(void);
	afx_msg void OnBnClickedButtonRefresh();

	CStringArray save_path;

	afx_msg void OnLbnSelchangeListData();
	CListBox m_Datainfo;


	void Get_Data(CString datafile);



	double*			m_pChartXTime;
	double*			m_pChartX, *m_pChartY;
	int				m_nChartSize;

	CStringArray m_arIMUTime;
	CArray<IMUData, IMUData> arIMUData;




	CChartViewer m_ChartViewer;
	CChartViewer m_ChartViewer2;
	CChartViewer m_ChartViewer3;
	int graph_type;
	
	afx_msg void OnBnClickedButtonRefreshGraph();

	
	afx_msg void OnBnClickedRadioX();
	afx_msg void OnBnClickedRadioY();
	afx_msg void OnBnClickedRadioZ();
	afx_msg void OnBnClickedRadioThetaM();
	afx_msg void OnBnClickedRadioPiM();
	afx_msg void OnBnClickedRadioThetaF();
	afx_msg void OnBnClickedRadioPiF();
	afx_msg void OnBnClickedRadioV();
	afx_msg void OnBnClickedRadioVRpsTheta();
	afx_msg void OnBnClickedRadioVRpsPi();
	CButton m_RadioButtonX;


	//graph1함수,변수
	double graph1_x_min, graph1_x_max;
	double graph1_y_min, graph1_y_max;

	void graph1_draw(void);
	void graph1_erase(void);
	void graph1_initialize(void);

	double *graph1_data;
	double *graph1_x_Axis;
	   
	bool graph1_check;
	int CAsize1; //데이터 전체 길이
	int graph_buffer_index1;
	
	CEdit m_Graph1XMax;
	CEdit m_Graph1Min;
	CEdit m_Graph1YMax;
	CEdit m_Graph1YMin;
	
	afx_msg void OnBnClickedButtonGraph1InsertYminmax();
	afx_msg void OnBnClickedButtonGraph1InsertMinmax();


	//그래프 2함수,변수
	double graph2_x_min, graph2_x_max;
	double graph2_y_min, graph2_y_max;

	void graph2_draw(void);
	void graph2_erase(void);
	void graph2_initialize(void);

	double *graph2_data;
	double *graph2_x_Axis;

	CEdit m_Graph2Max;
	CEdit m_Graph2MIN;
	CEdit m_Graph2YMax;
	CEdit m_Graph2YMin;

	bool graph2_check;
	int CAsize2; //데이터 전체 길이
	int graph_buffer_index2;
	afx_msg void OnBnClickedButtonGraph1InsertMinmax2();
	afx_msg void OnBnClickedButtonGraph1InsertYminmax2();


	//그래프 3함수,변수
	double graph3_x_min, graph3_x_max;
	double graph3_y_min, graph3_y_max;

	void graph3_draw(void);
	void graph3_erase(void);
	void graph3_initialize(void);

	double *graph3_data;
	double *graph3_x_Axis;

	bool graph3_check;
	int CAsize3; //데이터 전체 길이
	int graph_buffer_index3;

	CEdit m_Graph3Min;
	CEdit m_Graph3Max;
	CEdit m_Graph3YMax;
	CEdit m_Graph3YMin;

	afx_msg void OnBnClickedButtonGraph1InsertMinmax3();
	afx_msg void OnBnClickedButtonGraph1InsertYminmax3();
	afx_msg void OnBnClickedButtonResetXY();
};
