#include "stdafx.h"
#include "define.h"
#include <math.h>

CString			g_strInIPath;



int Find_STX(BYTE* p, int size)
{
	BYTE s[2] = { 0xAA ,0x55 };
	for (int i = 0; i < size-1 ; i++, p++)
	{
		if (*p == s[0]  && *(p+1) == s[1])
			return i;
	}

	return -1;
}

CString GetTime(CTime time )
{
	CString strTime ;
	
	strTime.Format(_T("%04d-%02d-%02d 23:59:59"),	time.GetYear(), time.GetMonth(), time.GetDay());
	
	return strTime ;
}

CString GetTimeToString()
{
	
	CString strTime;
	CTime time = CTime::GetCurrentTime();

	strTime.Format(_T("[%04d-%02d-%02d %02d:%02d:%02d]"), time.GetYear(), time.GetMonth(), time.GetDay(),
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
///////////////////////////// Implementation //////////////////////////////////
#include <VersionHelpers.h>


void EnumerateSerialPorts(CUIntArray& ports)
{
  //Make sure we clear out any elements which may already be in the array
  ports.RemoveAll();

  //Determine what OS we are running on
//  OSVERSIONINFO osvi;
//  osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
//  BOOL bGetVer = GetVersionEx(&osvi);
  BOOL bGetVer = IsWindowsXPOrGreater();

  //On NT use the QueryDosDevice API
  if (bGetVer )//&& (osvi.dwPlatformId == VER_PLATFORM_WIN32_NT))
  {
    //Use QueryDosDevice to look for all devices of the form COMx. This is a better
    //solution as it means that no ports have to be opened at all.
    TCHAR szDevices[65535];
    DWORD dwChars = QueryDosDevice(NULL, szDevices, 65535);
    if (dwChars)
    {
      int i=0;

      for (;;)
      {
        //Get the current device name
        TCHAR* pszCurrentDevice = &szDevices[i];

        //If it looks like "COMX" then
        //add it to the array which will be returned
        int nLen = _tcslen(pszCurrentDevice);
        if (nLen > 3 && nLen < 6 && _tcsnicmp(pszCurrentDevice, _T("COM"), 3) == 0)
        {
          //Work out the port number
          int nPort = _ttoi(&pszCurrentDevice[3]);
          ports.Add(nPort);
        }

        // Go to next NULL character
        while(szDevices[i] != _T('\0'))
          i++;

        // Bump pointer to the next string
        i++;

        // The list is double-NULL terminated, so if the character is
        // now NULL, we're at the end
        if (szDevices[i] == _T('\0'))
          break;
      }
    }
    else
      TRACE(_T("Failed in call to QueryDosDevice, GetLastError:%d\n"), GetLastError());
  }
  else
  {
    //On 95/98 open up each port to determine their existence

    //Up to 255 COM ports are supported so we iterate through all of them seeing
    //if we can open them or if we fail to open them, get an access denied or general error error.
    //Both of these cases indicate that there is a COM port at that number. 
    for (UINT i=1; i<256; i++)
    {
      //Form the Raw device name
      CString sPort;
      sPort.Format(_T("\\\\.\\COM%d"), i);

      //Try to open the port
      BOOL bSuccess = FALSE;
      HANDLE hPort = ::CreateFile(sPort, GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, 0, 0);
      if (hPort == INVALID_HANDLE_VALUE)
      {
        DWORD dwError = GetLastError();

        //Check to see if the error was because some other app had the port open or a general failure
        if (dwError == ERROR_ACCESS_DENIED || dwError == ERROR_GEN_FAILURE)
          bSuccess = TRUE;
      }
      else
      {
        //The port was opened successfully
        bSuccess = TRUE;

        //Don't forget to close the port, since we are going to do nothing with it anyway
        CloseHandle(hPort);
      }

      //Add the port number to the array which will be returned
      if (bSuccess)
        ports.Add(i);
    }
  }
}

//////////////////////////////////////////////////////////////////////////////////////////
// ---------------------------- reverse --------------------------------

// Reverses (reflects) bits in a 32-bit word.
unsigned reverse(unsigned x) {
   x = ((x & 0x55555555) <<  1) | ((x >>  1) & 0x55555555);
   x = ((x & 0x33333333) <<  2) | ((x >>  2) & 0x33333333);
   x = ((x & 0x0F0F0F0F) <<  4) | ((x >>  4) & 0x0F0F0F0F);
   x = (x << 24) | ((x & 0xFF00) << 8) |
       ((x >> 8) & 0xFF00) | (x >> 24);
   return x;
}

// ----------------------------- crc32a --------------------------------

/* This is the basic CRC algorithm with no optimizations. It follows the
logic circuit as closely as possible. */

unsigned int crc32a(unsigned char *message ,int nLen) 
{
   int  j = 0 ;
   unsigned int byte, crc;

   crc = 0xFFFFFFFF;
   for(int i = 0 ; i < nLen ; i++) 
   {
      byte = message[i];            // Get next byte.
      byte = reverse(byte);         // 32-bit reversal.
      for (j = 0; j <= 7; j++) {    // Do eight times.
         if ((int)(crc ^ byte) < 0)
              crc = (crc << 1) ^ 0x04C11DB7;
         else crc = crc << 1;
         byte = byte << 1;          // Ready next msg bit.
      }
   }
   return reverse(~crc);
}