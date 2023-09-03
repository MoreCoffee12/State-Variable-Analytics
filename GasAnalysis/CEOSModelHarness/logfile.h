/********************************************************************
* CLogFile CLASS: This class opens a log file upon instatiation and  *
* appends to it throughout program execution.   It is closed when   *
* the class is destroyed.											*
********************************************************************/
#ifndef _CLogFile
#define _CLogFile

#include <windows.h>
#include <fstream>
#include <stdio.h>
#include <string.h>

class CLogFile
{
public:
	CLogFile(char *logFileName);
	~CLogFile();

	void Append(LPCSTR inString, ...);

private:
	FILE		*logFile;
	SYSTEMTIME  SysTime;
};

#endif