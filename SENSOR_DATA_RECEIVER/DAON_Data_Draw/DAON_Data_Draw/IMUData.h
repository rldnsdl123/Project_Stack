#pragma once
class IMUData
{
public:
	IMUData();
	~IMUData();

	IMUData(const IMUData & imu);
	IMUData& operator=(const IMUData& imu);

	void Insert_Data(CString);
	static int n_mIndex;
	CString date;
	CString dev_time;
	CString x_data;
	CString y_data;
	CString z_data;
	CString theta_m_data;
	CString pi_m_data;
	CString theta_f_data;
	CString pi_f_data;
	CString V_data;
	CString rps_theta_data;
	CString rps_pi_data;
};