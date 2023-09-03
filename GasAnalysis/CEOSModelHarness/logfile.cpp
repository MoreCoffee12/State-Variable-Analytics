#include "logfile.h"

/////////////////////////////////////////////////////////////////////
// Method Definition:
//   Opens the log file using the name passed in.
//
// Revision:  
//   26 Nov 2011    
//   B. Howard
//   Revised file functions for safe versions
//
/////////////////////////////////////////////////////////////////////
CLogFile::CLogFile(char *logFileName)
{
	errno_t err;
    err =  fopen_s(&logFile, logFileName, "w");
}


/////////////////////////////////////////////////////////////////////
// Method Definition:
//   Appends messages to log file defined in constructor for 
//   debugging and tracking program execution.	
// 
// Revision:  
//   26 Nov 2011    
//   B. Howard
//   Revised string functions for safe versions
//
/////////////////////////////////////////////////////////////////////
void CLogFile::Append(LPCSTR inString, ...)
{
	va_list Marker;
    char bufferString[1024];
    int iSizeBuffer = sizeof( bufferString ) / sizeof( bufferString[0] );

	//Retreive system data and time
	GetLocalTime(&SysTime); 

	// Compile string sent from user
    va_start(Marker, inString);
    vsprintf_s(bufferString, iSizeBuffer, inString, Marker);
    va_end(Marker);
	
	fprintf(logFile, "\n%i:%i:%i:%i - %s", 
		(int)SysTime.wHour,
		(int)SysTime.wMinute,
		(int)SysTime.wSecond,
		(int)SysTime.wMilliseconds,
		bufferString);
	fflush(logFile);
}

/*********************
* CLogFile Destructor *
*--------------------**
* Closes the logfile. *
**********************/
CLogFile::~CLogFile()
{
	fclose(logFile);
}
