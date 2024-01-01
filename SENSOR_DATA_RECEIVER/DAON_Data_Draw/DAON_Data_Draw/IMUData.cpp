#include "stdafx.h"
#include "IMUData.h"

IMUData::IMUData()
{
	//dev_time=0;
	//x_data = 0;
	//y_data = 0;
	//z_data = 0;
	//theta_m_data = 0;
	//pi_m_data = 0;
	//theta_f_data = 0;
	//pi_f_data = 0;
	//V_data = 0;
	//rps_theta_data = 0;
	//rps_pi_data = 0;
}

IMUData::~IMUData()
{
}

IMUData::IMUData(const IMUData & imu)
{
	date = imu.date;
	x_data = imu.x_data;
	y_data = imu.y_data;
	z_data = imu.z_data;
	theta_m_data = imu.theta_m_data;
	pi_m_data = imu.pi_m_data;
	theta_f_data = imu.theta_f_data;
	pi_f_data = imu.pi_f_data;
	V_data = imu.V_data;
	rps_pi_data = imu.rps_pi_data;
	rps_theta_data = imu.rps_theta_data;
}

IMUData & IMUData::operator=(const IMUData & imu)
{
	// TODO: 여기에 반환 구문을 삽입합니다.
	date = imu.date;
	x_data = imu.x_data;
	y_data = imu.y_data;
	z_data = imu.z_data;
	theta_m_data = imu.theta_m_data;
	pi_m_data = imu.pi_m_data;
	theta_f_data = imu.theta_f_data;
	pi_f_data = imu.pi_f_data;
	V_data = imu.V_data;
	rps_pi_data = imu.rps_pi_data;
	rps_theta_data = imu.rps_theta_data;

	return *this;

}

void IMUData::Insert_Data(CString readline)
{
	AfxExtractSubString(this->date, readline, 0, ',');
	AfxExtractSubString(this->dev_time, readline, 1, ',');
	AfxExtractSubString(this->x_data, readline, 2, ',');
	AfxExtractSubString(this->y_data, readline, 3, ',');
	AfxExtractSubString(this->z_data, readline, 4, ',');
	AfxExtractSubString(this->theta_m_data, readline, 5, ',');
	AfxExtractSubString(this->pi_m_data, readline, 6, ',');
	AfxExtractSubString(this->theta_f_data, readline, 7, ',');
	AfxExtractSubString(this->pi_f_data, readline, 8, ',');
	AfxExtractSubString(this->V_data, readline, 9, ',');
	AfxExtractSubString(this->rps_theta_data, readline, 10, ',');
	AfxExtractSubString(this->rps_pi_data, readline, 11, ',');
	
	/*IMUData imu;
	AfxExtractSubString(imu.date, readline, 0, ',');
	AfxExtractSubString(imu.dev_time, readline, 1, ',');
	AfxExtractSubString(imu.x_data, readline, 2, ',');
	AfxExtractSubString(imu.y_data, readline, 3, ',');
	AfxExtractSubString(imu.z_data, readline, 4, ',');
	AfxExtractSubString(imu.theta_m_data, readline, 5, ',');
	AfxExtractSubString(imu.pi_m_data, readline, 6, ',');
	AfxExtractSubString(imu.theta_f_data, readline, 7, ',');
	AfxExtractSubString(imu.pi_f_data, readline, 8, ',');
	AfxExtractSubString(imu.V_data, readline, 9, ',');
	AfxExtractSubString(imu.rps_theta_data, readline, 10, ',');
	AfxExtractSubString(imu.rps_pi_data, readline, 11, ',');*/
}
