#include "stdafx.h"
#include "define.h"

CString GetTimeToString()
{
	CString strTime;
	CTime time = CTime::GetCurrentTime();

	strTime.Format("[%04d-%02d-%02d %02d:%02d:%02d]", time.GetYear(), time.GetMonth(), time.GetDay(),
		time.GetHour(), time.GetMinute(), time.GetSecond());
	//strTime.Format("%02d:%02d:%02d:%03d", (int)lt.wHour, (int)lt.wMinute, (int)lt.wSecond, (int)lt.wMilliseconds);
	return strTime;
}

CString GetExecutedPath()
{
	CString strResult;
	CString strPath;

	if (GetModuleFileName(nullptr, strPath.GetBuffer(_MAX_PATH + 1), MAX_PATH) != FALSE)
	{
		strPath.ReleaseBuffer();

		strResult = strPath.Left(strPath.ReverseFind('\\'));
	}
	else
	{
	}

	return strResult;
}

CString ExtractFilePath(CString strPath)
{
	int i, iMax = -1;
	CString s;


	s.Format("%s", strPath);
	for (i = 0; i >= 0;)
	{
		i = s.Find("\\", 0);
		if (i >= 0)
		{
			s.Delete(0, i + 1);
			iMax += i + 1;
		}
	}
	if (iMax >= 0)
	{
		s.Format("%s", strPath);
		s.Delete(iMax + 1, s.GetLength() - iMax - 1);
	}
	return s;
}
